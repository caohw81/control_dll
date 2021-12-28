// u_control_dll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "u_control_dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_PATH 512

#pragma pack(1)		//1字节对齐
typedef struct imsiReportInfo
{
	int ipIndex;
	int userType;
	char imsi[15];
	char imei[15];
	int rsrp;
} E_IMSIReportInfo;

typedef struct cellConfigInfo
{
	int euarfcn;
	int mcc;
	char mnc[5];
	int pci;
	int tac;
	int periodTac;
	char cellid[15];
	int txpower;
	int maxTxPower;
} CellConfigInfo;

#pragma pack()

SOCKET  mserver;
sockaddr_in m_atrget_addr;
char recvBuf[1024]={0};
int udpConnectFlag=1;

int m_send_Port=51888;
vector<string>m_recvIpVec;
vector<int> m_recvIpPortVec;
vector<int>m_stopIpVec;

typedef int (*PIMSIREPORTCALLBACKFUN)(E_IMSIReportInfo* p_IMSIReportInfo);
typedef int ( *PSTATECALLBACKFUN)(int ipIndex, int id,int param);
typedef int ( *PWHITEIMSICHECKCALLBACKFUN)(int ipIndex, char* imsiList,int rows);
typedef int ( *PBLACKIMSICHECKCALLBACKFUN)(int ipIndex, char* imsiList,int rows);
typedef int ( *PFEMTOSTATUSCALLBACKFUN)(int ipIndex, int status, CellConfigInfo* cellInfo);


PIMSIREPORTCALLBACKFUN pIMSIReportFun=0;
PSTATECALLBACKFUN	pStatusFun = 0;
PWHITEIMSICHECKCALLBACKFUN pWhiteImsiCheckFun=0;
PBLACKIMSICHECKCALLBACKFUN pBlackImsiCheckFun=0;
PFEMTOSTATUSCALLBACKFUN pCellStatusResponseFun=0;
//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CU_control_dllApp

BEGIN_MESSAGE_MAP(CU_control_dllApp, CWinApp)
	//{{AFX_MSG_MAP(CU_control_dllApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CU_control_dllApp construction

CU_control_dllApp::CU_control_dllApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CU_control_dllApp object

CU_control_dllApp theApp;





int sendMsg(int ipIndex, char* buff, int buffLenth){
	//CString strip="192.168.2.52";	
	//CString strip="127.0.0.1";
	//char clientIp[20]={0};
	//int clientPort=51888;
	//int target_port =51888;

	m_atrget_addr.sin_family = AF_INET;
	m_atrget_addr.sin_port = htons(m_recvIpPortVec[ipIndex-1]);  //端口
	m_atrget_addr.sin_addr.s_addr = inet_addr(m_recvIpVec[ipIndex-1].c_str());
	
	
	int ret = sendto(mserver, (char*)buff, buffLenth, 0, 
        (SOCKADDR *)&(m_atrget_addr), sizeof(SOCKADDR));
    if (SOCKET_ERROR == ret)
    {
        return 0;
    }
    return ret;
}

int udpSocketInit(){
	TCHAR AppPathName[MAX_PATH];
	GetModuleFileName(NULL,AppPathName,MAX_PATH);
	CString strAppPath = AppPathName;
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\'));
	CString strIniPath = strAppPath;
	strIniPath += "\\setup.ini";
	
	CString m_send_ip;		
	GetPrivateProfileString("u_connect","send_ip","172.18.18.11",m_send_ip.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);

	CString send_Port="51888";
	GetPrivateProfileString("u_connect","send_Port","51888",send_Port.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
	m_send_Port = _ttoi(send_Port);

	CString ip_recvNum="1";
	GetPrivateProfileString("u_connect","ip_recvNum","1",ip_recvNum.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
	int m_ip_recvNum = _ttoi(ip_recvNum);

	CString ip_addr=_T("192.168.2.52");
	CString recv_Port="51888";
	for(int i=0;i<m_ip_recvNum;++i){
		m_stopIpVec.push_back(1);
		CString ip_addr_str;
		ip_addr_str.Format("ip_addr%d",i+1);		
		GetPrivateProfileString("u_connect",ip_addr_str,ip_addr,ip_addr.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
		m_recvIpVec.push_back(ip_addr.GetBuffer(0));
		
		CString ip_Port;
		ip_Port.Format("recv_Port%d",i+1);		
		GetPrivateProfileString("u_connect",ip_Port,recv_Port,recv_Port.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);	
		m_recvIpPortVec.push_back(_ttoi(recv_Port));
	}

	mserver=socket(AF_INET,SOCK_DGRAM,0);
	if (mserver < 0)
	{
		//printf("create socket fail!\n");
		return 0;
	}
	int ret = 0;

	//int main_port = 51888;
	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(m_send_Port); //监听端口
	//local.sin_addr.s_addr=INADDR_ANY; //本机
	local.sin_addr.s_addr=inet_addr(m_send_ip); //本机
	ret=bind(mserver,(struct sockaddr*)&local,sizeof(local));
	if (ret < 0)
	{ 
		//printf("socket bind fail!\n");
		return 0;
	}
/*		
	struct timeval timeOut;			//设置接收超时
	timeOut.tv_sec=5;
	timeOut.tv_usec=0;
	if(setsockopt(mserver,SOL_SOCKET,SO_RCVTIMEO,(const char *)&timeOut,sizeof(timeOut))>=0){
		printf("setting timeOut success!");
	}else{
		printf("setting timeOut failed!");	
	}
*/	

	return 1;
}

UINT heartBeatTheard(LPVOID pr){
	TiXmlDocument *docXml = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	docXml->LinkEndChild(decl);

	TiXmlElement *rootElement = new TiXmlElement("message_content");
	docXml->LinkEndChild(rootElement);

	TiXmlElement *statusElement = new TiXmlElement("status_request");
    rootElement->LinkEndChild(statusElement);
	
	TiXmlElement *timeoutElement = new TiXmlElement("timeout");
    statusElement->LinkEndChild(timeoutElement);
	
	TiXmlElement *timestampElement = new TiXmlElement("timestamp");
    statusElement->LinkEndChild(timestampElement);
	
	TiXmlPrinter *printer = new TiXmlPrinter();
	rootElement->Accept(printer );//保存该节点及其子节点到字符串
	CString strXML = printer->CStr();
	
	char *buff;
	int lenth=0;
#ifdef _UNICODE 
	lenth = WideCharToMultiByte(CP_ACP, 0, strXML, strXML.GetLength(), NULL, 0, NULL, NULL);
	buff = new char[lenth+1];
    memset(buff,0,lenth + 1);
	WideCharToMultiByte(CP_ACP,0,strXML,strXML.GetLength(),buff,lenth,NULL,NULL);
	buff[lenth+1]='\0';
#else
	//lenth=strXML.GetAllocLength();
	lenth=strXML.GetLength();
	buff = new char [lenth+1];
	sprintf(buff,_T("%s"),strXML); 


/*	const char* p_buff= strXML.CStr();
	int lenth=strlen(p_buff);
	if(lenth == 0)
        return ;
    buff = new char[lenth+1 ];
    memcpy(buff, p_buff,lenth);f
*/	
	buff[lenth]='\0';
#endif

	while (udpConnectFlag){
		Sleep(5000);
		for(int i=0;i<m_recvIpVec.size();++i){
			sendMsg(i+1,buff,lenth-1);
			Sleep(10);
		}		
	}
	delete docXml;
	docXml = NULL;
	delete[] buff;
	strXML.ReleaseBuffer();
	return 1;
}

void stringToHex(const char *pUserInput, unsigned char *pKeyArray){
	if (NULL == pUserInput || NULL == pKeyArray)
	{
		return;
	}

	unsigned int uiKeySize = strlen(pUserInput) / 2;
	int i = 0;
	char cTempor = 0;

	while (i < uiKeySize){
		if (*pUserInput >= '0' && *pUserInput <= '9'){
			cTempor = *pUserInput - 48;
		}
		else{
			cTempor = 0xa + (*pUserInput - 'a');
		}

		pKeyArray[i] = cTempor;
		pUserInput++;

		if (*pUserInput >= '0' && *pUserInput <= '9'){
			cTempor = *pUserInput - 48;
		}
		else{
			cTempor = 0xa + (*pUserInput - 'a');
		}

		pKeyArray[i] = (pKeyArray[i] << 4) | cTempor;
		pUserInput++;
		i++;
	}
	return;
}

void dispRecvedMsg(int ipIndex, char* p_buff, int len){
	
	/*const string buff_str = p_buff;
	TiXmlDocument* docXml = new TiXmlDocument(buff_str);
    if (!docXml->LoadFile())
    {
        return ;
    }
	*/
	TiXmlDocument* docXml = new TiXmlDocument();
    docXml->Parse(p_buff);

	TiXmlElement* pRootElement = docXml->RootElement();	
    if (pRootElement){
		TiXmlElement* p_secLeaf = pRootElement->FirstChildElement();
        string secLeaf_str=p_secLeaf->Value();
		CString str(secLeaf_str.c_str());
		str.Format("RECV:%s",secLeaf_str);
		//AfxMessageBox(str);
		if(secLeaf_str=="status_response"){			//%%心跳 
		    TiXmlElement* p_status = p_secLeaf->FirstChildElement("status");
			const char *status_char = p_status->GetText();  //判断是否为空:if(status)
			if(status_char){
				const char *status_char2=status_char+2;
				unsigned char status_char_convert[1];
				stringToHex(status_char2,status_char_convert);
				int m_status=status_char_convert[0];
				//printf("heartbeat status:%d\n",m_status);
				//AfxMessageBox("0");
				Log("status_response@@ipIndex:%d, m_status:%d",ipIndex,m_status);
				(*pStatusFun)(ipIndex,0,m_status);	
			}
		}
		else if(secLeaf_str=="femto_status_response"){			
			CellConfigInfo cellInfo;
			TiXmlElement* p_status = p_secLeaf->FirstChildElement("status");
			const char *status_char = p_status->GetText();
			int m_status=atoi(status_char);
					
			TiXmlElement* p_euarfcn = p_secLeaf->FirstChildElement("euarfcn");
			const char *p_euarfcn_char = p_euarfcn->GetText();
			cellInfo.euarfcn = atoi(p_euarfcn_char);

			TiXmlElement* p_mcc = p_secLeaf->FirstChildElement("mcc");
			const char *mcc_char = p_mcc->GetText();
			cellInfo.mcc=atoi(mcc_char);
			
			TiXmlElement* p_mnc = p_secLeaf->FirstChildElement("mnc");
			const char *mnc_char = p_mnc->GetText();
			memset(cellInfo.mnc,0,sizeof(cellInfo.mnc));
			memcpy(cellInfo.mnc,mnc_char,strlen(mnc_char));
			//strcpy(cellInfo.mnc,mnc_char);
			
			TiXmlElement* p_pci = p_secLeaf->FirstChildElement("pci");
			const char *pci_char = p_pci->GetText();
			cellInfo.pci=atoi(pci_char);
			
			TiXmlElement* p_tac = p_secLeaf->FirstChildElement("tac");
			const char *tac_char = p_tac->GetText();
			cellInfo.tac=atoi(tac_char);
			
			TiXmlElement* p_periodTac = p_secLeaf->FirstChildElement("periodTac");
			const char *periodTac_char = p_periodTac->GetText();
			cellInfo.periodTac=atoi(periodTac_char);
			
			TiXmlElement* p_cellid = p_secLeaf->FirstChildElement("cellid");
			const char *cellid_char = p_cellid->GetText();
			memset(cellInfo.cellid,0,sizeof(cellInfo.cellid));
			memcpy(cellInfo.cellid,cellid_char,strlen(cellid_char));
			//strcpy(cellInfo.cellid,cellid_char);		
			
			TiXmlElement* p_txpower = p_secLeaf->FirstChildElement("txpower");
			const char *txpower_char = p_txpower->GetText();
			cellInfo.txpower=atoi(txpower_char);	
			
			TiXmlElement* p_maxTxPower = p_secLeaf->FirstChildElement("maxTxPower");
			const char *maxTxPower_char = p_maxTxPower->GetText();
			cellInfo.maxTxPower=atoi(maxTxPower_char);	
	
			Log("femto_status@@ipIndex:%d, euarfcn:%d,mcc:%d,mnc:%s,pci:%d",ipIndex, cellInfo.euarfcn,cellInfo.mcc,cellInfo.mnc,cellInfo.pci);
			(*pCellStatusResponseFun)(ipIndex, m_status, &cellInfo);
		}
		else if(secLeaf_str=="set_configuration_result"){	//%%小区设置
			TiXmlElement* p_result = p_secLeaf->FirstChildElement("result");
			const char *result_char = p_result->GetText();
			if(result_char){
				int m_result=atoi(result_char);
				//printf("set_configuration_result status:%d\n",m_status);
				//CString str;
				//str.Format("config status:%d",m_status);
				//AfxMessageBox(str);
				Log("set_configuration_result@@ipIndex:%d,m_result:%d",ipIndex, m_result);
				(*pStatusFun)(ipIndex, 1,m_result); 				
			}
		}
		else if(secLeaf_str=="set_redirection_rsp"){		//%%置用户策略重定向
			TiXmlElement* p_result = p_secLeaf->FirstChildElement("result");
			const char *result_char = p_result->GetText(); 
			int m_result=atoi(result_char);
			//printf("set_redirection_rsp result:%d\n",m_result);
			Log("set_redirection_rsp@@ipIndex:%d,m_result:%d",ipIndex,m_result);
			(*pStatusFun)(ipIndex, 2,m_result); 
		}
		else if(secLeaf_str=="activate_nodeb_result"){		//%%激活小区
			TiXmlElement *p_result = p_secLeaf->FirstChildElement("result");
			const char *result_char = p_result->GetText();
			int m_result=atoi(result_char);
			//printf("activate_nodeb_result result:%d\n",m_result);
			Log("activate_nodeb_result@@ipIndex:%d, m_result:%d",ipIndex, m_result);
			(*pStatusFun)(ipIndex, 3,m_result); 
		}
		else if(secLeaf_str=="imsi_list_config_result"){	//%%黑白名单添加
			TiXmlElement *p_result = p_secLeaf->FirstChildElement("result");
			const char *result_char = p_result->GetText();
			int m_result=atoi(result_char);
			//printf("imsi_list_config_result result:%d\n",m_result);
			Log("imsi_list_config_result@@ipIndex:%d,m_result:%d",ipIndex, m_result);
			(*pStatusFun)(ipIndex, 4,m_result); 
		}
		else if(secLeaf_str=="imsi_list_check_result"){		//%%黑白名单查询
			vector<string>whiteVec,blackVec;
			TiXmlElement *p_whiteimsi = p_secLeaf->FirstChildElement("whiteimsi");
			if(p_whiteimsi){
				TiXmlElement *p_w_imsi = p_whiteimsi->FirstChildElement("imsi");
				if(p_w_imsi){
					while(p_w_imsi){
						string imsi = p_w_imsi->GetText();
						Log("imsi_list_check_result@@ipIndex:%d, whiteimsi:%s",ipIndex, imsi.c_str());
						whiteVec.push_back(imsi);
						p_w_imsi=p_w_imsi->NextSiblingElement();
					}
					int imsiNum=whiteVec.size();
					if(imsiNum>0){
						char* whiteImsiList=(char* )malloc(imsiNum*15*sizeof(char));
						memset(whiteImsiList,0,imsiNum*15);
						for(int i=0;i<imsiNum;++i){
							memcpy(&whiteImsiList[i*15],whiteVec[i].c_str(),15);
							//whiteImsiList[i][15]='\0';
						}
						(*pWhiteImsiCheckFun)(ipIndex, whiteImsiList,imsiNum );
						free(whiteImsiList);
					}					
				}else{
					(*pWhiteImsiCheckFun)(ipIndex, "NULL",0 );
				}			
			}
			TiXmlElement *p_blackimsi = p_secLeaf->FirstChildElement("blackimsi");
			if(p_blackimsi){
				TiXmlElement *p_b_imsi = p_blackimsi->FirstChildElement("imsi");
				if(p_b_imsi){
					while(p_b_imsi){
						string imsi = p_b_imsi->GetText();
						Log("imsi_list_check_result@@ipIndex:%d, blackimsi:%s",ipIndex, imsi.c_str());
						blackVec.push_back(imsi);
						p_b_imsi=p_b_imsi->NextSiblingElement();
						p_b_imsi=p_b_imsi->NextSiblingElement();
						p_b_imsi=p_b_imsi->NextSiblingElement();
					}
					int imsiNum=blackVec.size();
					if(imsiNum>0){					
						char* blackImsiList=(char* )malloc(imsiNum*15*sizeof(char));
						memset(blackImsiList,0,imsiNum*15);
						for(int i=0;i<imsiNum;++i){
							memcpy(&blackImsiList[i*15],blackVec[i].c_str(),15);
						}
						(*pBlackImsiCheckFun)(ipIndex, blackImsiList,imsiNum );						
						free(blackImsiList);
					}				
				}else{
					(*pBlackImsiCheckFun)(ipIndex, "NULL",0 );		
				}
				
			}
			//printf("white_list.size:%d \t black_list.size:%d \n",white_list.size(),black_list.size());
		}
		else if(secLeaf_str=="imsi_list_delconfig_result"){	//%%黑白名单删除
			TiXmlElement *P_result = p_secLeaf->FirstChildElement("result");
			const char *result_char = P_result->GetText();
			int m_result=atoi(result_char);
			//printf("imsi_list_delconfig_result result:%d\n",m_result);
			Log("imsi_list_delconfig_result@@ipIndex:%d, m_result:%d",ipIndex, m_result);
			(*pStatusFun)(ipIndex, 5,m_result); 			
		}
		else if(secLeaf_str=="scanner"){					//%%捕号IMSI上报
			TiXmlElement* p_userType = p_secLeaf->FirstChildElement("userType");
			const char *userType_char = p_userType->GetText();
			int m_userType=atoi(userType_char);			
			
			TiXmlElement* p_imsi = p_secLeaf->FirstChildElement("imsi");
			const char *imsi_char = p_imsi->GetText();

			TiXmlElement* p_imei = p_secLeaf->FirstChildElement("imei");
			const char *imei_char = p_imei->GetText();
			
			TiXmlElement* p_rsrp = p_secLeaf->FirstChildElement("rsrp");
			const char *rsrp_char = p_rsrp->GetText();
			int m_rsrp=atoi(rsrp_char);
			
			//printf("scanner userType:%d \t imsi_char:%s \t m_rsrp:%d \n",m_userType,imsi_char,m_rsrp);

			E_IMSIReportInfo imsiInfo;
			imsiInfo.ipIndex=ipIndex;
			imsiInfo.userType=m_userType;
		
			memset(imsiInfo.imsi,0,sizeof(imsiInfo.imsi));
			memcpy(imsiInfo.imsi,imsi_char,15);
			//imsiInfo.imsi[sizeof(imsiInfo.imsi)]='\0';
			
			memset(imsiInfo.imei,0,sizeof(imsiInfo.imei));
			memcpy(imsiInfo.imei,imei_char,15);
			//imsiInfo.imsi[sizeof(imsiInfo.imsi)]='\0';
			
			imsiInfo.rsrp=m_rsrp;

			Log("scanner@@ipIndex:%d, userType:%d,imsi:%s,imei:%s,rsrp:%d",ipIndex, m_userType,imsi_char,imei_char,m_rsrp);
			(*pIMSIReportFun)(&imsiInfo);			
		}
	}
}

UINT Main_Socket_Thread(LPVOID pr){
	InitLog();
	udpSocketInit();
	Log("init dll");
	AfxBeginThread( heartBeatTheard, NULL );//心跳
	
	struct sockaddr_in from;
	int fromlen =sizeof(from);		
	while (1){
		memset(recvBuf,0,sizeof(recvBuf));		
		int ret = recvfrom(mserver,(char*)recvBuf,sizeof(recvBuf),0,(struct sockaddr*)&from,&fromlen);
		if (ret!=SOCKET_ERROR){
			int ipIndex=0;
			for(int i=0;i<m_recvIpVec.size();++i){
				if(strcmp(inet_ntoa(from.sin_addr),m_recvIpVec[i].c_str())==0){
					ipIndex=i+1;
					break;
				}	
			}
			dispRecvedMsg(ipIndex,recvBuf,ret);			
		}
	}
	closesocket(mserver);
	return 1;
}

#define DllExport extern "C" __declspec(dllexport) 


DllExport void u_initRun(){
	udpConnectFlag=1;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,2),&wsaData)) //调用Windows Sockets DLL
	{ 
		WSACleanup();
		return ;
	}
	AfxBeginThread( Main_Socket_Thread, NULL );
}

const char* itoa(int val){
    static char result[sizeof(int)<<3+2];
    sprintf(result,"%d",val);
    return result;
}

DllExport void u_femtoStatusRequest(int ipIndex){
	TiXmlDocument *docXml = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	docXml->LinkEndChild(decl);

	TiXmlElement *rootElement = new TiXmlElement("message_content");
	docXml->LinkEndChild(rootElement);

	TiXmlElement *femtoElement = new TiXmlElement("femto_status_request");
    rootElement->LinkEndChild(femtoElement);
	
	TiXmlElement *timeoutElement = new TiXmlElement("timeout");
    femtoElement->LinkEndChild(timeoutElement);

	TiXmlPrinter *printer = new TiXmlPrinter();
	rootElement->Accept(printer );//保存该节点及其子节点到字符串
	CString strXML = printer->CStr();
	
	char *buff;
	int lenth=strXML.GetLength();
	buff = new char [lenth+1];
	sprintf(buff,_T("%s"),strXML); 

	buff[lenth]='\0';
	
	Log("femto_status_request@@ipIndex:%d",ipIndex);
	if (udpConnectFlag){
		sendMsg(ipIndex,buff,lenth-1);
	}
	delete docXml;
	docXml = NULL;
	delete[] buff;
	strXML.ReleaseBuffer();
}

DllExport void u_setCellConfig(int ipIndex, int euarfcn, int mcc,char* mnc, int pci, int tac, int txpower, int periodTac, int bandwidth, char* cellid){
	TiXmlDocument *docXml = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	docXml->LinkEndChild(decl);

	TiXmlElement *rootElement = new TiXmlElement("message_content");
	docXml->LinkEndChild(rootElement);

	TiXmlElement *configurationElement = new TiXmlElement("set_configuration");
    rootElement->LinkEndChild(configurationElement);
	
	TiXmlElement *euarfcnElement = new TiXmlElement("euarfcn");
    configurationElement->LinkEndChild(euarfcnElement);
	TiXmlText *euarfcnContent = new TiXmlText(itoa(euarfcn));
	euarfcnElement->LinkEndChild(euarfcnContent);

	TiXmlElement *mccElement = new TiXmlElement("mcc");
    configurationElement->LinkEndChild(mccElement);
	TiXmlText *mccContent = new TiXmlText(itoa(mcc));
	mccElement->LinkEndChild(mccContent);
	
	TiXmlElement *mncElement = new TiXmlElement("mnc");
    configurationElement->LinkEndChild(mncElement);
	TiXmlText *mncContent = new TiXmlText(mnc);
	mncElement->LinkEndChild(mncContent);
	
	TiXmlElement *pciElement = new TiXmlElement("pci");
    configurationElement->LinkEndChild(pciElement);
	TiXmlText *pciContent = new TiXmlText(itoa(pci));
	pciElement->LinkEndChild(pciContent);

	TiXmlElement *tacElement = new TiXmlElement("tac");
    configurationElement->LinkEndChild(tacElement);
	TiXmlText *tacContent = new TiXmlText(itoa(tac));
	tacElement->LinkEndChild(tacContent);
	
	TiXmlElement *txpowerElement = new TiXmlElement("txpower");
    configurationElement->LinkEndChild(txpowerElement);
	TiXmlText *txpowerContent = new TiXmlText(itoa(txpower));
	txpowerElement->LinkEndChild(txpowerContent);
	
	TiXmlElement *periodTacElement = new TiXmlElement("periodTac");
    configurationElement->LinkEndChild(periodTacElement);
	TiXmlText *periodTacContent = new TiXmlText(itoa(periodTac));
	periodTacElement->LinkEndChild(periodTacContent);
	
	TiXmlElement *bandwidthElement = new TiXmlElement("bandwidth");
    configurationElement->LinkEndChild(bandwidthElement);
	TiXmlText *bandwidthContent = new TiXmlText(itoa(bandwidth));
	bandwidthElement->LinkEndChild(bandwidthContent);
	
	TiXmlElement *cellidElement = new TiXmlElement("cellid");
    configurationElement->LinkEndChild(cellidElement);
	TiXmlText *cellidContent = new TiXmlText(cellid);
	cellidElement->LinkEndChild(cellidContent);
	
	TiXmlElement *timeoutElement = new TiXmlElement("timeout");
    configurationElement->LinkEndChild(timeoutElement);

	TiXmlPrinter *printer = new TiXmlPrinter();
	rootElement->Accept(printer );//保存该节点及其子节点到字符串
	CString strXML = printer->CStr();
	
	char *buff;
	int lenth=0;
#ifdef _UNICODE 
	lenth = WideCharToMultiByte(CP_ACP, 0, strXML, strXML.GetLength(), NULL, 0, NULL, NULL);
	buff = new char[lenth+1];
    memset(buff,0,lenth + 1);
	WideCharToMultiByte(CP_ACP,0,strXML,strXML.GetLength(),buff,lenth,NULL,NULL);
	buff[lenth+1]='\0';
#else
	//lenth=strXML.GetAllocLength();
	lenth=strXML.GetLength();
	buff = new char [lenth+1];
	sprintf(buff,_T("%s"),strXML); 


/*	const char* p_buff= strXML.CStr();
	int lenth=strlen(p_buff);
	if(lenth == 0)
        return ;
    buff = new char[lenth+1 ];
    memcpy(buff, p_buff,lenth);
*/	
	buff[lenth]='\0';
#endif

	Log("setCellConfig@@ipIndex:%d, euarfcn:%d,mcc:%d,mnc:%s,pci:%d,tac:%d,periodTac:%d,bandwidth:%d,cellid:%s",ipIndex, euarfcn,mcc,mnc,pci,tac,periodTac,bandwidth,cellid);
	if (udpConnectFlag){
		sendMsg(ipIndex,buff,lenth-1);
	}
	delete docXml;
	docXml = NULL;
	delete[] buff;
	strXML.ReleaseBuffer();
}

DllExport void u_setRedirection(int ipIndex, int category, char* priority,int GeranRedirect, int arfcn, int UtranRedirect, int uarfcn, int EutranRedirect, int earfcn,int RejectMethod){
	TiXmlDocument *docXml = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	docXml->LinkEndChild(decl);

	TiXmlElement *rootElement = new TiXmlElement("message_content");
	docXml->LinkEndChild(rootElement);

	TiXmlElement *redirection_reqElement = new TiXmlElement("set_redirection_req");
    rootElement->LinkEndChild(redirection_reqElement);
	
	TiXmlElement *categoryElement = new TiXmlElement("category");
    redirection_reqElement->LinkEndChild(categoryElement);
	TiXmlText *categoryContent = new TiXmlText(itoa(category));
	categoryElement->LinkEndChild(categoryContent);

	TiXmlElement *priorityElement = new TiXmlElement("priority");
    redirection_reqElement->LinkEndChild(priorityElement);
	TiXmlText *priorityContent = new TiXmlText(priority);
	priorityElement->LinkEndChild(priorityContent);

	TiXmlElement *GeranRedirectElement = new TiXmlElement("GeranRedirect");
    redirection_reqElement->LinkEndChild(GeranRedirectElement);	
	TiXmlText *GeranRedirectContent = new TiXmlText(itoa(GeranRedirect));
	GeranRedirectElement->LinkEndChild(GeranRedirectContent);
	
	TiXmlElement *arfcnElement = new TiXmlElement("arfcn");
    redirection_reqElement->LinkEndChild(arfcnElement);
	TiXmlText *arfcnContent = new TiXmlText(itoa(arfcn));
	arfcnElement->LinkEndChild(arfcnContent);
	
	TiXmlElement *UtranRedirectElement = new TiXmlElement("UtranRedirect");
    redirection_reqElement->LinkEndChild(UtranRedirectElement);
	TiXmlText *UtranRedirectContent = new TiXmlText(itoa(UtranRedirect));
	UtranRedirectElement->LinkEndChild(UtranRedirectContent);
	
	TiXmlElement *uarfcnTacElement = new TiXmlElement("uarfcn");
    redirection_reqElement->LinkEndChild(uarfcnTacElement);
	TiXmlText *uarfcnTacContent = new TiXmlText(itoa(uarfcn));
	uarfcnTacElement->LinkEndChild(uarfcnTacContent);
	
	TiXmlElement *EutranRedirectElement = new TiXmlElement("EutranRedirect");
    redirection_reqElement->LinkEndChild(EutranRedirectElement);
	TiXmlText *EutranRedirectContent = new TiXmlText(itoa(EutranRedirect));
	EutranRedirectElement->LinkEndChild(EutranRedirectContent);
	
	TiXmlElement *earfcnElement = new TiXmlElement("earfcn");
    redirection_reqElement->LinkEndChild(earfcnElement);
	TiXmlText *earfcnContent = new TiXmlText(itoa(earfcn));
	earfcnElement->LinkEndChild(earfcnContent);

	TiXmlElement *RejectMethodElement = new TiXmlElement("RejectMethod");
    redirection_reqElement->LinkEndChild(RejectMethodElement);
	TiXmlText *RejectMethodContent = new TiXmlText(itoa(RejectMethod));
	RejectMethodElement->LinkEndChild(RejectMethodContent);

	TiXmlPrinter *printer = new TiXmlPrinter();
	rootElement->Accept(printer );//保存该节点及其子节点到字符串
	CString strXML = printer->CStr();
	
	char *buff;
	int lenth=0;
#ifdef _UNICODE 
	lenth = WideCharToMultiByte(CP_ACP, 0, strXML, strXML.GetLength(), NULL, 0, NULL, NULL);
	buff = new char[lenth+1];
    memset(buff,0,lenth + 1);
	WideCharToMultiByte(CP_ACP,0,strXML,strXML.GetLength(),buff,lenth,NULL,NULL);
	buff[lenth+1]='\0';
#else
	//lenth=strXML.GetAllocLength();
	lenth=strXML.GetLength();
	buff = new char [lenth+1];
	sprintf(buff,_T("%s"),strXML); 


/*	const char* p_buff= strXML.CStr();
	int lenth=strlen(p_buff);
	if(lenth == 0)
        return ;
    buff = new char[lenth+1 ];
    memcpy(buff, p_buff,lenth);
*/	
	buff[lenth]='\0';
#endif

	Log("setRedirection@@ipIndex:%d, category:%d,priority:%s,GeranRedirect:%d,arfcn:%d,UtranRedirect:%d,uarfcn:%d,EutranRedirect:%d,earfcn:%d,RejectMethod:%d",ipIndex, category, priority,GeranRedirect, arfcn, UtranRedirect, uarfcn, EutranRedirect, earfcn,RejectMethod);
	if (udpConnectFlag){
		sendMsg(ipIndex, buff,lenth-1);
	}
	delete docXml;
	docXml = NULL;
	delete[] buff;
	strXML.ReleaseBuffer();
}

DllExport void u_activeCell(int ipIndex,int active_mode, int mode){
	TiXmlDocument *docXml = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	docXml->LinkEndChild(decl);

	TiXmlElement *rootElement = new TiXmlElement("message_content");
	docXml->LinkEndChild(rootElement);

	TiXmlElement *redirection_reqElement = new TiXmlElement("activate_nodeb_request");
    rootElement->LinkEndChild(redirection_reqElement);
	
	TiXmlElement *active_modeElement = new TiXmlElement("active_mode");
    redirection_reqElement->LinkEndChild(active_modeElement);
	TiXmlText *active_modeContent = new TiXmlText(itoa(active_mode));
	active_modeElement->LinkEndChild(active_modeContent);

	TiXmlElement *modeElement = new TiXmlElement("mode");
    redirection_reqElement->LinkEndChild(modeElement);
	TiXmlText *modeContent = new TiXmlText(itoa(mode));
	modeElement->LinkEndChild(modeContent);

	TiXmlElement *timeoutElement = new TiXmlElement("timeout");
    redirection_reqElement->LinkEndChild(timeoutElement);

	TiXmlPrinter *printer = new TiXmlPrinter();
	rootElement->Accept(printer );//保存该节点及其子节点到字符串
	CString strXML = printer->CStr();
	
	char *buff;
	int lenth=0;
#ifdef _UNICODE 
	lenth = WideCharToMultiByte(CP_ACP, 0, strXML, strXML.GetLength(), NULL, 0, NULL, NULL);
	buff = new char[lenth+1];
    memset(buff,0,lenth + 1);
	WideCharToMultiByte(CP_ACP,0,strXML,strXML.GetLength(),buff,lenth,NULL,NULL);
	buff[lenth+1]='\0';
#else
	//lenth=strXML.GetAllocLength();
	lenth=strXML.GetLength();
	buff = new char [lenth+1];
	sprintf(buff,_T("%s"),strXML); 


/*	const char* p_buff= strXML.CStr();
	int lenth=strlen(p_buff);
	if(lenth == 0)
        return ;
    buff = new char[lenth+1 ];
    memcpy(buff, p_buff,lenth);
*/	
	buff[lenth]='\0';
#endif

	Log("activeCell@@ipIndex:%d, active_mode:%d,mode:%d",ipIndex, active_mode,mode);
	if (udpConnectFlag){
		sendMsg(ipIndex, buff,lenth-1);
	}
	delete docXml;
	docXml = NULL;
	delete[] buff;
	strXML.ReleaseBuffer();
}

DllExport void u_addWhiteImsi(int ipIndex, char* whiteImsi){
	TiXmlDocument *docXml = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	docXml->LinkEndChild(decl);

	TiXmlElement *rootElement = new TiXmlElement("message_content");
	docXml->LinkEndChild(rootElement);

	TiXmlElement *set_imsi_list_setconfigElement = new TiXmlElement("imsi_list_setconfig");
    rootElement->LinkEndChild(set_imsi_list_setconfigElement);
	TiXmlElement *whiteimsiElement = new TiXmlElement("whiteimsi");
	set_imsi_list_setconfigElement->LinkEndChild(whiteimsiElement);
	
	int len =strlen(whiteImsi)/15;
	for(int i=0;i<len;++i){
		char imsi_str[16];
		memset(imsi_str,0,sizeof(imsi_str));
		memcpy(imsi_str,&whiteImsi[i*15],15);
		TiXmlElement *imsiElement = new TiXmlElement("imsi");
		whiteimsiElement->LinkEndChild(imsiElement);
		TiXmlText *imsiContent = new TiXmlText((const char*)imsi_str);
		imsiElement->LinkEndChild(imsiContent);
	}

	TiXmlPrinter *printer = new TiXmlPrinter();
	rootElement->Accept(printer );//保存该节点及其子节点到字符串
	CString strXML = printer->CStr();
	
	char *buff;
	int lenth=0;
#ifdef _UNICODE 
	lenth = WideCharToMultiByte(CP_ACP, 0, strXML, strXML.GetLength(), NULL, 0, NULL, NULL);
	buff = new char[lenth+1];
    memset(buff,0,lenth + 1);
	WideCharToMultiByte(CP_ACP,0,strXML,strXML.GetLength(),buff,lenth,NULL,NULL);
	buff[lenth+1]='\0';
#else
	//lenth=strXML.GetAllocLength();
	lenth=strXML.GetLength();
	buff = new char [lenth+1];
	sprintf(buff,_T("%s"),strXML); 

/*	const char* p_buff= strXML.CStr();
	int lenth=strlen(p_buff);
	if(lenth == 0)
        return ;
    buff = new char[lenth+1 ];
    memcpy(buff, p_buff,lenth);
*/	
	buff[lenth]='\0';
#endif

	Log("addWhiteImsi@@ipIndex:%d, whiteImsi:%s",ipIndex, whiteImsi);
	if (udpConnectFlag){
		sendMsg(ipIndex, buff,lenth-1);
	}
	delete docXml;
	docXml = NULL;
	delete[] buff;
	strXML.ReleaseBuffer();
}

DllExport void u_addBlackImsi(int ipIndex,char* blackImsi){
	TiXmlDocument *docXml = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	docXml->LinkEndChild(decl);

	TiXmlElement *rootElement = new TiXmlElement("message_content");
	docXml->LinkEndChild(rootElement);

	TiXmlElement *imsi_list_setconfigElement = new TiXmlElement("imsi_list_setconfig");
    rootElement->LinkEndChild(imsi_list_setconfigElement);	
	TiXmlElement *blackimsiElement = new TiXmlElement("blackimsi");
    imsi_list_setconfigElement->LinkEndChild(blackimsiElement);
	
	
	int len =strlen(blackImsi)/15;
	for(int i=0;i<len;++i){
		char imsi_str[16];
		memset(imsi_str,0,sizeof(imsi_str));
		memcpy(imsi_str,&blackImsi[i*15],15);

		TiXmlElement *imsiElement = new TiXmlElement("imsi");
		blackimsiElement->LinkEndChild(imsiElement);
		TiXmlText *imsiContent = new TiXmlText((const char*)imsi_str);
		imsiElement->LinkEndChild(imsiContent);
		
		TiXmlElement *dedicatedRB_startElement = new TiXmlElement("dedicatedRB_start");
		blackimsiElement->LinkEndChild(dedicatedRB_startElement);
		TiXmlText *dedicatedRB_startContent = new TiXmlText("12");
		dedicatedRB_startElement->LinkEndChild(dedicatedRB_startContent);
		
		TiXmlElement *dedicatedRB_endElement = new TiXmlElement("dedicatedRB_end");
		blackimsiElement->LinkEndChild(dedicatedRB_endElement);
		TiXmlText *dedicatedRB_endContent = new TiXmlText("14");
		dedicatedRB_endElement->LinkEndChild(dedicatedRB_endContent);
	}	

	TiXmlPrinter *printer = new TiXmlPrinter();
	rootElement->Accept(printer );//保存该节点及其子节点到字符串
	CString strXML = printer->CStr();
	
	char *buff;
	int lenth=0;
#ifdef _UNICODE 
	lenth = WideCharToMultiByte(CP_ACP, 0, strXML, strXML.GetLength(), NULL, 0, NULL, NULL);
	buff = new char[lenth+1];
    memset(buff,0,lenth + 1);
	WideCharToMultiByte(CP_ACP,0,strXML,strXML.GetLength(),buff,lenth,NULL,NULL);
	buff[lenth+1]='\0';
#else
	//lenth=strXML.GetAllocLength();
	lenth=strXML.GetLength();
	buff = new char [lenth+1];
	sprintf(buff,_T("%s"),strXML); 

/*	const char* p_buff= strXML.CStr();
	int lenth=strlen(p_buff);
	if(lenth == 0)
        return ;
    buff = new char[lenth+1 ];
    memcpy(buff, p_buff,lenth);
*/	
	buff[lenth]='\0';
#endif

	Log("addBlackImsi@@ipIndex:%d, blackImsi:%s",ipIndex, blackImsi);
	if (udpConnectFlag){
		sendMsg(ipIndex, buff,lenth-1);
	}
	delete docXml;
	docXml = NULL;
	delete[] buff;
	strXML.ReleaseBuffer();
}

DllExport void u_deleteWhiteImsi(int ipIndex,char* whiteImsi, int n){
	char m_imsi[16];
	TiXmlDocument *docXml = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	docXml->LinkEndChild(decl);

	TiXmlElement *rootElement = new TiXmlElement("message_content");
	docXml->LinkEndChild(rootElement);

	TiXmlElement *imsi_list_delconfigElement = new TiXmlElement("imsi_list_delconfig");
    rootElement->LinkEndChild(imsi_list_delconfigElement);	
	TiXmlElement *whiteimsiElement = new TiXmlElement("whiteimsi");
    imsi_list_delconfigElement->LinkEndChild(whiteimsiElement);
	
	if(n>0){
		for(int i= 0 ;i<n;++i){
			memset(m_imsi,0,16);
			memcpy(m_imsi,whiteImsi+i*15,15);
			//m_imsi[16]='\0';
			TiXmlElement *imsiElement = new TiXmlElement("imsi");
			whiteimsiElement->LinkEndChild(imsiElement);
			TiXmlText *imsiContent = new TiXmlText((const char*)m_imsi);
			imsiElement->LinkEndChild(imsiContent);		
		}
	}

	TiXmlElement *blackimsiElement = new TiXmlElement("blackimsi");
    imsi_list_delconfigElement->LinkEndChild(blackimsiElement);
	
	TiXmlElement *timeoutElement = new TiXmlElement("timeout");
    imsi_list_delconfigElement->LinkEndChild(timeoutElement);
	
	TiXmlPrinter *printer = new TiXmlPrinter();
	rootElement->Accept(printer );
	CString strXML = printer->CStr();
	
	char *buff;
	int lenth=0;
#ifdef _UNICODE 
	lenth = WideCharToMultiByte(CP_ACP, 0, strXML, strXML.GetLength(), NULL, 0, NULL, NULL);
	buff = new char[lenth+1];
    memset(buff,0,lenth + 1);
	WideCharToMultiByte(CP_ACP,0,strXML,strXML.GetLength(),buff,lenth,NULL,NULL);
	buff[lenth+1]='\0';
#else
	//lenth=strXML.GetAllocLength();
	lenth=strXML.GetLength();
	buff = new char [lenth+1];
	sprintf(buff,_T("%s"),strXML); 


/*	const char* p_buff= strXML.CStr();
	int lenth=strlen(p_buff);
	if(lenth == 0)
        return ;
    buff = new char[lenth+1 ];
    memcpy(buff, p_buff,lenth);
*/	
	buff[lenth]='\0';
#endif

	Log("deleteWhiteImsi@@ipIndex:%d, num:%d,whiteImsi:%s",ipIndex, n,whiteImsi);
	if (udpConnectFlag){
		sendMsg(ipIndex, buff,lenth-1);
	}
	delete docXml;
	docXml = NULL;
	delete[] buff;
	strXML.ReleaseBuffer();
}

DllExport void u_deleteBlackImsi(int ipIndex,char* blackImsi,int n){
	char m_imsi[16];
	TiXmlDocument *docXml = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	docXml->LinkEndChild(decl);

	TiXmlElement *rootElement = new TiXmlElement("message_content");
	docXml->LinkEndChild(rootElement);

	TiXmlElement *imsi_list_delconfigElement = new TiXmlElement("imsi_list_delconfig");
    rootElement->LinkEndChild(imsi_list_delconfigElement);	
	TiXmlElement *whiteimsiElement = new TiXmlElement("whiteimsi");
    imsi_list_delconfigElement->LinkEndChild(whiteimsiElement);
		
	TiXmlElement *blackimsiElement = new TiXmlElement("blackimsi");
    imsi_list_delconfigElement->LinkEndChild(blackimsiElement);
	
	if(n>0){
		for(int i= 0 ;i<n;++i){
			memset(m_imsi,0,16);
			memcpy(m_imsi,blackImsi+i*15,15);
			//m_imsi[16]='\0';
			TiXmlElement *imsiElement = new TiXmlElement("imsi");
			blackimsiElement->LinkEndChild(imsiElement);
			TiXmlText *imsiContent = new TiXmlText((const char*)m_imsi);
			imsiElement->LinkEndChild(imsiContent);		
		}
	}

	
	TiXmlElement *timeoutElement = new TiXmlElement("timeout");
    imsi_list_delconfigElement->LinkEndChild(timeoutElement);
	
	TiXmlPrinter *printer = new TiXmlPrinter();
	rootElement->Accept(printer );
	CString strXML = printer->CStr();
	
	char *buff;
	int lenth=0;
#ifdef _UNICODE 
	lenth = WideCharToMultiByte(CP_ACP, 0, strXML, strXML.GetLength(), NULL, 0, NULL, NULL);
	buff = new char[lenth+1];
    memset(buff,0,lenth + 1);
	WideCharToMultiByte(CP_ACP,0,strXML,strXML.GetLength(),buff,lenth,NULL,NULL);
	buff[lenth+1]='\0';
#else
	//lenth=strXML.GetAllocLength();
	lenth=strXML.GetLength();
	buff = new char [lenth+1];
	sprintf(buff,_T("%s"),strXML); 


/*	const char* p_buff= strXML.CStr();
	int lenth=strlen(p_buff);
	if(lenth == 0)
        return ;
    buff = new char[lenth+1 ];
    memcpy(buff, p_buff,lenth);
*/	
	buff[lenth]='\0';
#endif

	Log("deleteBlackImsi@@ipIndex:%d, num:%d,blackImsi:%s",ipIndex, n,blackImsi);
	if (udpConnectFlag){
		sendMsg(ipIndex, buff,lenth-1);
	}
	delete docXml;
	docXml = NULL;
	delete[] buff;
	strXML.ReleaseBuffer();
}

DllExport void u_imsiListCheck(int ipIndex){
	TiXmlDocument *docXml = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	docXml->LinkEndChild(decl);

	TiXmlElement *rootElement = new TiXmlElement("message_content");
	docXml->LinkEndChild(rootElement);

	TiXmlElement *imsi_list_checkElement = new TiXmlElement("imsi_list_check");
    rootElement->LinkEndChild(imsi_list_checkElement);

	TiXmlElement *timeoutElement = new TiXmlElement("timeout");
    imsi_list_checkElement->LinkEndChild(timeoutElement);
		
	TiXmlPrinter *printer = new TiXmlPrinter();
	rootElement->Accept(printer );
	CString strXML = printer->CStr();
	
	char *buff;
	int lenth=0;
#ifdef _UNICODE 
	lenth = WideCharToMultiByte(CP_ACP, 0, strXML, strXML.GetLength(), NULL, 0, NULL, NULL);
	buff = new char[lenth+1];
    memset(buff,0,lenth + 1);
	WideCharToMultiByte(CP_ACP,0,strXML,strXML.GetLength(),buff,lenth,NULL,NULL);
	buff[lenth+1]='\0';
#else
	//lenth=strXML.GetAllocLength();
	lenth=strXML.GetLength();
	buff = new char [lenth+1];
	sprintf(buff,_T("%s"),strXML); 

/*	const char* p_buff= strXML.CStr();
	int lenth=strlen(p_buff);
	if(lenth == 0)
        return ;
    buff = new char[lenth+1 ];
    memcpy(buff, p_buff,lenth);
*/	
	buff[lenth]='\0';
#endif
	
	Log("imsiListCheck@@ipIndex:%d",ipIndex);
	if (udpConnectFlag){
		sendMsg(ipIndex,buff,lenth-1);
	}
	delete docXml;
	docXml = NULL;
	delete[] buff;
	strXML.ReleaseBuffer();
}

DllExport void u_quit(int ipIndex){
	udpConnectFlag=0;
	if(mserver)
		closesocket(mserver);	
	mserver=INVALID_SOCKET;
}

DllExport int u_getIMSIReportCallBack(int (*pFun)(E_IMSIReportInfo*p)){ //IMSI上报
	pIMSIReportFun = pFun;
	return 1;
}

DllExport int u_getStatusCallBack(int (*pFun)(int ipIndex, int id,int param)){ //状态 定位值
	pStatusFun = pFun;
	return 1;
}

DllExport int u_whiteImsiCheckCallBack(int (*pFun)(int ipIndex, char*imsiList, int rows)){ //状态 定位值
	pWhiteImsiCheckFun = pFun;
	return 1;
}

DllExport int u_blackImsiCheckCallBack(int (*pFun)(int ipIndex, char*imsiList, int rows)){ //状态 定位值
	pBlackImsiCheckFun = pFun;
	return 1;
}

DllExport int u_cellStatusCallBack(int (*pFun)(int ipIndex, int status, CellConfigInfo* cellInfo)){ //状态 定位值
	pCellStatusResponseFun = pFun;
	return 1;
}
