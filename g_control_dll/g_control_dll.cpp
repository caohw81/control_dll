// g_control_dll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "g_control_dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_PATH 512

SOCKET  mserver;
sockaddr_in m_atrget_addr;
vector<string>m_recvIpVec;
vector<int> m_recvIpPortVec;
vector<int> m_stopIpVec;

char recvBuf[1024]={0};
int udpConnectFlag=1;

int m_send_Port=5557;

typedef int (*HEARTBEATCALLBACKFUN)(int ipIndex, char carrIndex,int state);
typedef int (*MSGREPORTCALLBACKFUN)(MsgInfo* p_msgInfo);
typedef int (*IMSICHECKCALLBACKFUN)(ImsiCheckInfo* p_imsiCheckInfo);
typedef int (*IMSIREPORTCALLBACKFUN)(ImsiReportInfo* p_imsiReportInfo);
//typedef int (*CARRFREPOINTCHECKFUN)(CarrFrePointCheckInfo* p_carrCheckInfo);
typedef int (*CARRFREPOINTCHECKFUN)(int ipIndex, char carrIndex,int carr);

HEARTBEATCALLBACKFUN pHeartBeatFun=0;
MSGREPORTCALLBACKFUN pMsgReportFun=0;
IMSICHECKCALLBACKFUN pImsiCheckFun=0;
IMSIREPORTCALLBACKFUN pImsiReportFun=0;
CARRFREPOINTCHECKFUN pCarrCheckInfo=0;

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
// CG_control_dllApp

BEGIN_MESSAGE_MAP(CG_control_dllApp, CWinApp)
	//{{AFX_MSG_MAP(CG_control_dllApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CG_control_dllApp construction

CG_control_dllApp::CG_control_dllApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CG_control_dllApp object

CG_control_dllApp theApp;


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
	GetPrivateProfileString("g_connect","send_ip","172.17.18.11",m_send_ip.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
	
	CString send_Port="5557";
	GetPrivateProfileString("g_connect","send_Port","5557",send_Port.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
	m_send_Port = _ttoi(send_Port);

	CString ip_recvNum="1";
	GetPrivateProfileString("g_connect","ip_recvNum","1",ip_recvNum.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
	int m_ip_recvNum = _ttoi(ip_recvNum);

	CString ip_addr=_T("172.17.18.80");
	CString recv_Port="5558";
	for(int i=0;i<m_ip_recvNum;++i){
		m_stopIpVec.push_back(1);
		CString ip_addr_str;
		ip_addr_str.Format("ip_addr%d",i+1);		
		GetPrivateProfileString("g_connect",ip_addr_str,ip_addr,ip_addr.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
		m_recvIpVec.push_back(ip_addr.GetBuffer(0));
		
		CString ip_Port;
		ip_Port.Format("recv_Port%d",i+1);		
		GetPrivateProfileString("g_connect",ip_Port,recv_Port,recv_Port.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);	
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
	MsgHeader heartBeat;	
	heartBeat.msgLenth=8;
	heartBeat.msgSerialNum=HEARTBEAT;
	heartBeat.msgCodeNum=0x01;
	
	heartBeat.msgParam=0;
	
	char buff[10];
	
	while (udpConnectFlag){	
		for(int i=0;i<m_recvIpVec.size();++i){
			heartBeat.msgCarrier=0;	
			memset(buff,0,sizeof(buff));
			memcpy(buff,&heartBeat,sizeof(MsgHeader));	
			sendMsg(i+1,buff,sizeof(MsgHeader));
			
			Sleep(10);
			heartBeat.msgCarrier=1;
			memset(buff,0,sizeof(buff));
			memcpy(buff,&heartBeat,sizeof(MsgHeader));	
			sendMsg(i+1,buff,sizeof(MsgHeader));
		}
		Sleep(5000);		
	}
	return 1;
}

void dispRecvedMsg(int ipIndex, char* p_buff, int len){
	if(len < sizeof(MsgHeader)){
		return ;
	}
	
	MsgHeader* pMsgHeader=(MsgHeader*)p_buff;
	
	if(pMsgHeader->msgCodeNum==0x11){
		if(pMsgHeader->msgSerialNum==HEARTBEAT){	//心跳
			
			Log("heatBeat & ipIndex:%d,carNum:%d",ipIndex,pMsgHeader->msgCarrier);		
			(*pHeartBeatFun)(ipIndex,pMsgHeader->msgCarrier,1);			
		}
		else if(pMsgHeader->msgSerialNum==CARRFREPOINTCHECK){
			CarrFrePointCheck *carrInfo=(CarrFrePointCheck *)p_buff;
			
			CarrFrePointCheckInfo reportInfo;
			reportInfo.ipIndex=ipIndex;	
			reportInfo.carrierIndex=carrInfo->header.msgCarrier;
			reportInfo.carrFrePoint=atoi(carrInfo->carr.data);

			//(*pCarrCheckInfo)(&reportInfo);
			(*pCarrCheckInfo)(ipIndex,carrInfo->header.msgCarrier,atoi(carrInfo->carr.data));
		}
	}
	else if(pMsgHeader->msgCodeNum==0x12){
		if(pMsgHeader->msgSerialNum==CELLCONFIG){	//小区设置
			MsgInfo info;
			info.ipIndex=ipIndex;
			info.carrierIndex=pMsgHeader->msgCarrier;
			info.msgType=CELLCONFIG;
			Log("setCellConfig Ok & ipIndex:%d,carNum:%d",ipIndex,pMsgHeader->msgCarrier);
			(*pMsgReportFun)(&info);			
		}else if(pMsgHeader->msgSerialNum==RUNNINGMODE){//运行模式设置
			MsgInfo info;
			info.ipIndex=ipIndex;
			info.carrierIndex=pMsgHeader->msgCarrier;
			info.msgType=RUNNINGMODE;
			Log("setRunningMode Ok & ipIndex:%d,carNum:%d",ipIndex,pMsgHeader->msgCarrier);

			(*pMsgReportFun)(&info);			
		}
	}
	else if(pMsgHeader->msgCodeNum==0x15){//开启射频
		MsgInfo info;
		info.ipIndex=ipIndex;
		info.carrierIndex=pMsgHeader->msgCarrier;
		info.msgType=TURNONRF;
		Log("turnOnRF Ok & ipIndex:%d,carNum:%d",ipIndex,pMsgHeader->msgCarrier);

		(*pMsgReportFun)(&info);
	}
	else if(pMsgHeader->msgCodeNum==0x16){//关闭射频
		MsgInfo info;
		info.ipIndex=ipIndex;
		info.carrierIndex=pMsgHeader->msgCarrier;
		info.msgType=TURNOFFRF;
		Log("turnOffRF Ok & ipIndex:%d,carNum:%d",ipIndex,pMsgHeader->msgCarrier);

		(*pMsgReportFun)(&info);
	}
	else if(pMsgHeader->msgCodeNum==0x17){//重启系统
		MsgInfo info;
		info.ipIndex=ipIndex;
		info.carrierIndex=pMsgHeader->msgCarrier;
		info.msgType=RESTARTSYSTEM;
		Log("restartSystem Ok & ipIndex:%d,carNum:%d",ipIndex,pMsgHeader->msgCarrier);
		(*pMsgReportFun)(&info);
	}
	else if(pMsgHeader->msgCodeNum==0x19){//重启板卡
		MsgInfo info;
		info.ipIndex=ipIndex;
		info.carrierIndex=pMsgHeader->msgCarrier;
		info.msgType=RESTARTCARD;
		Log("restartCard Ok & ipIndex:%d,carNum:%d",ipIndex,pMsgHeader->msgCarrier);
		(*pMsgReportFun)(&info);
	}
	else  if(pMsgHeader->msgCodeNum==0x1b){//黑白名单查询配置
		if(pMsgHeader->msgSerialNum==ADDBLACKIMSI){	//addBlackImsi
			DataInfo_35* addImsiInfo=(DataInfo_35*) p_buff;	
			MsgInfo info;
			info.ipIndex=ipIndex;
			info.carrierIndex=addImsiInfo->header.msgCarrier;
			info.msgType=ADDBLACKIMSI;
			Log("addBlackImsi Ok & ipIndex:%d,carNum:%d",ipIndex,pMsgHeader->msgCarrier);

			(*pMsgReportFun)(&info);
		}else if(pMsgHeader->msgSerialNum==ADDWHITEIMSI){//addWhiteImsi
			DataInfo_35* addImsiInfo=(DataInfo_35*) p_buff;	
			MsgInfo info;
			info.ipIndex=ipIndex;
			info.carrierIndex=addImsiInfo->header.msgCarrier;
			info.msgType=ADDWHITEIMSI;
			Log("addWhiteImsi Ok & ipIndex:%d,carNum:%d",ipIndex,pMsgHeader->msgCarrier);
			(*pMsgReportFun)(&info);	
		}
		else if(pMsgHeader->msgSerialNum==CHECKBLACKIMSI){//checkBlack
			DataInfo_35* checkImsiInfo=(DataInfo_35*) p_buff;
			ImsiCheckInfo imsiInfo;
			imsiInfo.ipIndex=ipIndex;
			imsiInfo.carrierIndex=checkImsiInfo->header.msgCarrier;
			imsiInfo.msgType=CHECKBLACKIMSI;
			memset(imsiInfo.imsi,0,15);
			memcpy(imsiInfo.imsi,checkImsiInfo->msgInfo.data,15);
			imsiInfo.imsi[15]='\0';
			Log("checkBlackImsi Ok & ipIndex:%d,carNum:%d,imsi:%s",imsiInfo.ipIndex,imsiInfo.carrierIndex,imsiInfo.imsi);
			(*pImsiCheckFun)(&imsiInfo);
		}else if(pMsgHeader->msgSerialNum==CHECKWHITEIMSI){	//checkWhite
			DataInfo_35* checkImsiInfo=(DataInfo_35*) p_buff;
			ImsiCheckInfo imsiInfo;
			imsiInfo.ipIndex=ipIndex;
			imsiInfo.carrierIndex=checkImsiInfo->header.msgCarrier;
			imsiInfo.msgType=CHECKWHITEIMSI;
			memset(imsiInfo.imsi,0,15);
			memcpy(imsiInfo.imsi,checkImsiInfo->msgInfo.data,15);
			imsiInfo.imsi[15]='\0';
			Log("checkWhiteImsi Ok & ipIndex:%d,carNum:%d,imsi:%s",imsiInfo.ipIndex,imsiInfo.carrierIndex,imsiInfo.imsi);

			(*pImsiCheckFun)(&imsiInfo);				
		}
		else if(pMsgHeader->msgSerialNum==DELETEBLACKIMSI){
			DataInfo_35* deleteImsiInfo=(DataInfo_35*) p_buff;
			MsgInfo info;
			info.ipIndex=ipIndex;
			info.carrierIndex=deleteImsiInfo->header.msgCarrier;
			info.msgType=DELETEBLACKIMSI;
			Log("deleteBlackImsi Ok & ipIndex:%d,carNum:%d",ipIndex,pMsgHeader->msgCarrier);

			(*pMsgReportFun)(&info);
		}
		else if(pMsgHeader->msgSerialNum==DELETEWHITEIMSI){//deleteWhiteImsi
			DataInfo_35* deleteImsiInfo=(DataInfo_35*) p_buff;
			MsgInfo info;
			info.ipIndex=ipIndex;
			info.carrierIndex=deleteImsiInfo->header.msgCarrier;
			info.msgType=DELETEWHITEIMSI;
			Log("deleteWhiteImsi Ok & ipIndex:%d,carNum:%d",ipIndex,pMsgHeader->msgCarrier);
			(*pMsgReportFun)(&info);	
		}
		else if(pMsgHeader->msgSerialNum==CLEARBLACKIMSI){
			DataInfo_0* clearImsiInfo=(DataInfo_0*)p_buff;
			MsgInfo info;
			info.ipIndex=ipIndex;
			info.carrierIndex=clearImsiInfo->header.msgCarrier;
			info.msgType=CLEARBLACKIMSI;
			Log("clearBlackImsi Ok & ipIndex:%d,carNum:%d",ipIndex,pMsgHeader->msgCarrier);
			(*pMsgReportFun)(&info);
		}
		else if(pMsgHeader->msgSerialNum==CLEARWHITEIMSI){//clearWhiteImsi
			DataInfo_0* clearImsiInfo=(DataInfo_0*)p_buff;
			MsgInfo info;
			info.ipIndex=ipIndex;
			info.carrierIndex=clearImsiInfo->header.msgCarrier;
			info.msgType=CLEARWHITEIMSI;
			Log("clearWhiteImsi Ok & ipIndex:%d,carNum:%d",ipIndex,pMsgHeader->msgCarrier);
			(*pMsgReportFun)(&info);		
		}
	}
	else if(pMsgHeader->msgCodeNum==IMSIREPORT){
		if(len!=sizeof(GetImsiReport)){
			return ;
		}

		GetImsiReport *imsiInfo=(GetImsiReport*)p_buff;
		ImsiReportInfo reportInfo;
		reportInfo.ipIndex=ipIndex;
		reportInfo.carrierIndex=imsiInfo->header.msgCarrier;
		memset(reportInfo.imsi,0, 15 );
		memcpy(reportInfo.imsi, imsiInfo->data2.data,15);
		//reportInfo.imsi[15]='\0';
		memset(reportInfo.imei,0, 15 );
		memcpy(reportInfo.imei, imsiInfo->data3.data,15);
		//reportInfo.imei[15]='\0';
		Log("getImsiReport & ipIndex:%d,carNum:%d,imsi:%s,imei:%s",ipIndex,reportInfo.carrierIndex,reportInfo.imsi,reportInfo.imei);
		(*pImsiReportFun)(&reportInfo);
	}
}

UINT Main_Socket_Thread(LPVOID pr){	
	InitLog();
	udpSocketInit();
	Log("init dll");
	AfxBeginThread( heartBeatTheard, NULL );//心跳
	
	struct sockaddr_in from;
	int fromlen =sizeof(from);		
	while (udpConnectFlag){
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

DllExport void g_initRun(){
	udpConnectFlag=1;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,2),&wsaData)) //调用Windows Sockets DLL
	{ 
		WSACleanup();
		return ;
	}
	AfxBeginThread( Main_Socket_Thread, NULL );
}

DllExport void g_carrFrePointCheck(int ipIndex,U8 carrierIndicat){
	CarrFrePointCheck carrCheckInfo;
	
	carrCheckInfo.header.msgLenth=sizeof(CarrFrePointCheck);
	carrCheckInfo.header.msgSerialNum=CARRFREPOINTCHECK;
	carrCheckInfo.header.msgCodeNum=0x01;
	carrCheckInfo.header.msgCarrier=carrierIndicat;
	carrCheckInfo.header.msgParam=0;
	
	carrCheckInfo.carr.lenth=0x0b;
	carrCheckInfo.carr.serialNum=0x0150;
	memset(carrCheckInfo.carr.data,0,8);
	for(int i=0;i<8;++i){
		carrCheckInfo.carr.data[i]=0x00;
	}

	
	char buff[sizeof(CarrFrePointCheck)+1];
	memset(buff,0,sizeof(buff));
	memcpy(buff,&carrCheckInfo,sizeof(CarrFrePointCheck));
	buff[sizeof(CarrFrePointCheck)]='\0';
	Log("g_carrFrePointCheck&&ipIndex:%d,carNum:%d",ipIndex,carrierIndicat);

	sendMsg(ipIndex, buff, sizeof(CarrFrePointCheck));
}

DllExport void g_setCellConfig(int ipIndex,U8 carrierIndicat, U8* mcc,U8* mnc, U8* lac,U8* ci, U8* tac,U8* carrierFrePoint, U8* downAttenuation){

	CellConfig cellInfo;
	cellInfo.header.msgLenth=sizeof(CellConfig);
	cellInfo.header.msgSerialNum=CELLCONFIG;
	cellInfo.header.msgCodeNum=0x02;
	cellInfo.header.msgCarrier=carrierIndicat;
	cellInfo.header.msgParam=0;
	
	cellInfo.mcc.lenth=0x0b;
	cellInfo.mcc.serialNum=0x0101;
	memset(cellInfo.mcc.data,0,8);
	memcpy(cellInfo.mcc.data,mcc,strlen(mcc));
	//sprintf(cellInfo.mcc.data,"%s",mcc);
	//cellInfo.mcc.data=mcc;
	
	cellInfo.mnc.lenth=0x0b;
	cellInfo.mnc.serialNum=0x0102;
		memset(cellInfo.mnc.data,0,8);
	memcpy(cellInfo.mnc.data,mnc,strlen(mnc));
	//cellInfo.mnc.data=mnc;
	//sprintf(cellInfo.mnc.data,"%s",mnc);
	
	cellInfo.lac.lenth=0x0b;
	cellInfo.lac.serialNum=0x0103;
		memset(cellInfo.lac.data,0,8);
	memcpy(cellInfo.lac.data,lac,strlen(lac));
	//cellInfo.lac.data=lac;
	//sprintf(cellInfo.lac.data,"%s",lac);

	cellInfo.ci.lenth=0x0b;
	cellInfo.ci.serialNum=0x0104;
		memset(cellInfo.ci.data,0,8);
	memcpy(cellInfo.ci.data,ci,strlen(ci));
	//cellInfo.ci.data=ci;
	//sprintf(cellInfo.ci.data,"%s",ci);
	
	cellInfo.tac.lenth=0x07;
	cellInfo.tac.serialNum=0x011b;
		memset(cellInfo.tac.data,0,4);
	memcpy(cellInfo.tac.data,tac,strlen(tac));
	//cellInfo.tac.data=tac;
	//sprintf(cellInfo.tac.data,"%s",tac);
	
	
	cellInfo.carrierFrePoint.lenth=0x0b;
	cellInfo.carrierFrePoint.serialNum=0x0150;
		memset(cellInfo.carrierFrePoint.data,0,8);
	memcpy(cellInfo.carrierFrePoint.data,carrierFrePoint,strlen(carrierFrePoint));
	//cellInfo.carrierFrePoint.data=carrierFrePoint;
	//sprintf(cellInfo.carrierFrePoint.data,"%s",carrierFrePoint);
	
	cellInfo.downAttenuation.lenth=0x0b;
	cellInfo.downAttenuation.serialNum=0x0151;
		memset(cellInfo.downAttenuation.data,0,8);
	memcpy(cellInfo.downAttenuation.data,downAttenuation,strlen(downAttenuation));
	//cellInfo.downAttenuation.data=downAttenuation;
//	sprintf(cellInfo.downAttenuation.data,"%s",downAttenuation);	
	

	char buff[sizeof(CellConfig)+1];
	memset(buff,0,sizeof(buff));
	memcpy(buff,&cellInfo,sizeof(CellConfig));
	Log("g_setCellConfig&&ipIndex:%d,carNum:%d,mcc:%s,mnc:%s,lac:%s,ci:%s,tac:%s,carrier:%s,downAtten:%s",ipIndex,carrierIndicat,mcc,mnc,lac,ci,tac,carrierFrePoint,downAttenuation);
	//Log("setCellConfig:%s",buff);
	sendMsg(ipIndex, buff, sizeof(CellConfig));
}

DllExport void g_setRunningMode(int ipIndex,U8 carrierIndicat, U8 controlMode, U8 workMode, U8 msgSendMode ){
	RunningMode runningModeInfo;
	runningModeInfo.header.msgLenth=sizeof(RunningMode);
	runningModeInfo.header.msgSerialNum=RUNNINGMODE;
	runningModeInfo.header.msgCodeNum=0x02;
	runningModeInfo.header.msgCarrier=carrierIndicat;
	runningModeInfo.header.msgParam=0;
	
	runningModeInfo.controlMode.lenth=0x04;
	runningModeInfo.controlMode.serialNum=0x010a;
	runningModeInfo.controlMode.data=controlMode;
	
	runningModeInfo.workMode.lenth=0x04;
	runningModeInfo.workMode.serialNum=0x010b;
	runningModeInfo.workMode.data=workMode;
	
	runningModeInfo.msgSendMode.lenth=0x04;
	runningModeInfo.msgSendMode.serialNum=0x010c;
	runningModeInfo.msgSendMode.data=msgSendMode;
	
	char buff[sizeof(RunningMode)+1];
	memset(buff,0,sizeof(buff));
	memcpy(buff,&runningModeInfo,sizeof(RunningMode));
	
	Log("g_setRunningMode&&ipIndex:%d,carNum:%d,controlMode:%d,workMode:%d,msgSendMode:%d",ipIndex,carrierIndicat,controlMode,workMode,msgSendMode);

	sendMsg(ipIndex, buff, sizeof(RunningMode));
}

DllExport void g_turnOnRF(int ipIndex,U8 carrierIndicat){
	MsgHeader headerInfo;
	headerInfo.msgLenth=sizeof(MsgHeader);
	headerInfo.msgSerialNum=TURNONRF;
	headerInfo.msgCodeNum=0x05;
	headerInfo.msgCarrier=carrierIndicat;
	headerInfo.msgParam=0;
	
	char buff[sizeof(MsgHeader)+1];
	memset(buff,0,sizeof(buff));
	memcpy(buff,&headerInfo,sizeof(MsgHeader));
	Log("g_turnOnRF&&ipIndex:%d,carNum:%d",ipIndex,carrierIndicat);

	sendMsg(ipIndex, buff, sizeof(MsgHeader));
}

DllExport void g_turnOffRF(int ipIndex,U8 carrierIndicat){
	MsgHeader headerInfo;
	headerInfo.msgLenth=sizeof(MsgHeader);
	headerInfo.msgSerialNum=TURNOFFRF;
	headerInfo.msgCodeNum=0x06;
	headerInfo.msgCarrier=carrierIndicat;
	headerInfo.msgParam=0;
	
	char buff[sizeof(MsgHeader)+1];
	memset(buff,0,sizeof(buff));
	memcpy(buff,&headerInfo,sizeof(MsgHeader));
	Log("g_turnOffRF&&ipIndex:%d,carNum:%d",ipIndex,carrierIndicat);
	sendMsg(ipIndex, buff, sizeof(MsgHeader));
}

DllExport void g_restartSystem(int ipIndex,U8 carrierIndicat){
	MsgHeader headerInfo;
	headerInfo.msgLenth=sizeof(MsgHeader);
	headerInfo.msgSerialNum=RESTARTSYSTEM;
	headerInfo.msgCodeNum=0x07;
	headerInfo.msgCarrier=carrierIndicat;
	headerInfo.msgParam=0;
	
	char buff[sizeof(MsgHeader)+1];
	memset(buff,0,sizeof(buff));
	memcpy(buff,&headerInfo,sizeof(MsgHeader));
	Log("g_restartSystem&&ipIndex:%d,carNum:%d",ipIndex,carrierIndicat);
	sendMsg(ipIndex, buff, sizeof(MsgHeader));
}

DllExport void g_restartCard(int ipIndex,U8 carrierIndicat){
	MsgHeader headerInfo;
	headerInfo.msgLenth=sizeof(MsgHeader);
	headerInfo.msgSerialNum=RESTARTCARD;
	headerInfo.msgCodeNum=0x09;
	headerInfo.msgCarrier=carrierIndicat;
	headerInfo.msgParam=0;
	
	char buff[sizeof(MsgHeader)+1];
	memset(buff,0,sizeof(buff));
	memcpy(buff,&headerInfo,sizeof(MsgHeader));
	buff[sizeof(MsgHeader)]='\0';	
	Log("g_restartCard&&ipIndex:%d,carNum:%d",ipIndex,carrierIndicat);
	sendMsg(ipIndex, buff, sizeof(MsgHeader));
}

DllExport void g_addBlackImsi(int ipIndex,U8 carrierIndicat,char* imsi){
	DataInfo_35 addImsiInfo;
	addImsiInfo.header.msgLenth=sizeof(DataInfo_35);
	addImsiInfo.header.msgSerialNum=ADDBLACKIMSI;
	addImsiInfo.header.msgCodeNum=0x0b;
	addImsiInfo.header.msgCarrier=carrierIndicat;
	addImsiInfo.header.msgParam=0;
	
	addImsiInfo.msgInfo.lenth=sizeof(Unit_35);
	addImsiInfo.msgInfo.serialNum=0x0050;
	for(int i=0;i<35;++i){
		addImsiInfo.msgInfo.data[i]=0;
	}
	//memset(addImsiInfo.msgInfo.data,0,35);
	memcpy(addImsiInfo.msgInfo.data,imsi,strlen(imsi));
	addImsiInfo.msgInfo.data[15]=':';
	addImsiInfo.msgInfo.data[31]=':';
	
	char buff[sizeof(DataInfo_35)+1];
	memset(buff,0,sizeof(buff));
	memcpy(buff,&addImsiInfo,sizeof(DataInfo_35));
	buff[sizeof(DataInfo_35)]='\0';	
	Log("g_addBlackImsi&&ipIndex:%d,carNum:%d,imsi:%s",ipIndex,carrierIndicat,imsi);
	sendMsg(ipIndex, buff, sizeof(DataInfo_35));
}

DllExport void g_addWhiteImsi(int ipIndex,U8 carrierIndicat,char* imsi){
	DataInfo_35 addImsiInfo;
	addImsiInfo.header.msgLenth=sizeof(DataInfo_35);
	addImsiInfo.header.msgSerialNum=ADDWHITEIMSI;
	addImsiInfo.header.msgCodeNum=0x0b;
	addImsiInfo.header.msgCarrier=carrierIndicat;
	addImsiInfo.header.msgParam=0;
	
	addImsiInfo.msgInfo.lenth=sizeof(Unit_35);
	addImsiInfo.msgInfo.serialNum=0x0051;
	for(int i=0;i<35;++i){
		addImsiInfo.msgInfo.data[i]=0;
	}
	//memset(addImsiInfo.msgInfo.data,0,35);
	memcpy(addImsiInfo.msgInfo.data,imsi,strlen(imsi));
	addImsiInfo.msgInfo.data[15]=':';
	addImsiInfo.msgInfo.data[31]=':';
	
	char buff[sizeof(DataInfo_35)+1];
	memset(buff,0,sizeof(buff));
	memcpy(buff,&addImsiInfo,sizeof(DataInfo_35));
	buff[sizeof(DataInfo_35)]='\0';	
	Log("g_addWhiteImsi&&ipIndex:%d,carNum:%d,imsi:%s",ipIndex,carrierIndicat,imsi);
	sendMsg(ipIndex, buff, sizeof(DataInfo_35));
}

DllExport void g_checkBlackImsi(int ipIndex,U8 carrierIndicat){
	DataInfo_0 checkImsiInfo;	
	checkImsiInfo.header.msgLenth=sizeof(DataInfo_0);
	checkImsiInfo.header.msgSerialNum=CHECKBLACKIMSI;
	checkImsiInfo.header.msgCodeNum=0x0b;
	checkImsiInfo.header.msgCarrier=carrierIndicat;
	checkImsiInfo.header.msgParam=0;

	checkImsiInfo.msgInfo.lenth=sizeof(Unit_0);
	checkImsiInfo.msgInfo.serialNum=0x0058;
	
	char buff[sizeof(DataInfo_0)+1];
	memset(buff,0,sizeof(buff));
	memcpy(buff,&checkImsiInfo,sizeof(DataInfo_0));	
	buff[sizeof(DataInfo_0)]='\0';	
	Log("g_checkBlackImsi&&ipIndex:%d,carNum:%d",ipIndex,carrierIndicat);

	sendMsg(ipIndex, buff, sizeof(DataInfo_0));
}

DllExport void g_checkWhiteImsi(int ipIndex,U8 carrierIndicat){
	DataInfo_0 checkImsiInfo;	
	checkImsiInfo.header.msgLenth=sizeof(DataInfo_0);
	checkImsiInfo.header.msgSerialNum=CHECKWHITEIMSI;
	checkImsiInfo.header.msgCodeNum=0x0b;
	checkImsiInfo.header.msgCarrier=carrierIndicat;
	checkImsiInfo.header.msgParam=0;
	
	checkImsiInfo.msgInfo.lenth=sizeof(Unit_0);
	checkImsiInfo.msgInfo.serialNum=0x0059;
	
	char buff[sizeof(DataInfo_0)+1];
	memset(buff,0,sizeof(buff));
	memcpy(buff,&checkImsiInfo,sizeof(DataInfo_0));
	buff[sizeof(DataInfo_0)]='\0';		
	Log("g_checkWhiteImsi&&ipIndex:%d,carNum:%d",ipIndex,carrierIndicat);

	sendMsg(ipIndex, buff, sizeof(DataInfo_0));
}

DllExport void g_deleteBlackImsi(int ipIndex,U8 carrierIndicat,char* imsi){
	DataInfo_35 deleteImsiInfo;
	deleteImsiInfo.header.msgLenth=sizeof(DataInfo_35);
	deleteImsiInfo.header.msgSerialNum=DELETEBLACKIMSI;
	deleteImsiInfo.header.msgCodeNum=0x0b;
	deleteImsiInfo.header.msgCarrier=carrierIndicat;
	deleteImsiInfo.header.msgParam=0;
	
	deleteImsiInfo.msgInfo.lenth=sizeof(Unit_35);
	deleteImsiInfo.msgInfo.serialNum=0x0052;
	for(int i=0;i<35;++i){
		deleteImsiInfo.msgInfo.data[i]=0;
	}
	//memset(deleteImsiInfo.msgInfo.data,0,35);
	memcpy(deleteImsiInfo.msgInfo.data,imsi,strlen(imsi));
	deleteImsiInfo.msgInfo.data[15]=':';
	deleteImsiInfo.msgInfo.data[31]=':';
	
	char buff[sizeof(DataInfo_35)+1];
	memset(buff,0,sizeof(buff));
	memcpy(buff,&deleteImsiInfo,sizeof(DataInfo_35));
	buff[sizeof(DataInfo_35)]='\0';	
	Log("g_deleteBlackImsi&&ipIndex:%d,carNum:%d,imsi:%s",ipIndex,carrierIndicat,imsi);

	sendMsg(ipIndex, buff, sizeof(DataInfo_35));
}

DllExport void g_deleteWhiteImsi(int ipIndex,U8 carrierIndicat,char* imsi){
	DataInfo_35 deleteImsiInfo;
	deleteImsiInfo.header.msgLenth=sizeof(DataInfo_35);
	deleteImsiInfo.header.msgSerialNum=DELETEWHITEIMSI;
	deleteImsiInfo.header.msgCodeNum=0x0b;
	deleteImsiInfo.header.msgCarrier=carrierIndicat;
	deleteImsiInfo.header.msgParam=0;
	
	deleteImsiInfo.msgInfo.lenth=sizeof(Unit_35);
	deleteImsiInfo.msgInfo.serialNum=0x0053;
	for(int i=0;i<35;++i){
		deleteImsiInfo.msgInfo.data[i]=0;
	}
	//memset(deleteImsiInfo.msgInfo.data,0,35);
	memcpy(deleteImsiInfo.msgInfo.data,imsi,strlen(imsi));
	deleteImsiInfo.msgInfo.data[15]=':';
	deleteImsiInfo.msgInfo.data[31]=':';
	
	char buff[sizeof(DataInfo_35)+1];
	memset(buff,0,sizeof(buff));
	memcpy(buff,&deleteImsiInfo,sizeof(DataInfo_35));
	buff[sizeof(DataInfo_35)]='\0';	
	Log("g_deleteWhiteImsi&&ipIndex:%d,carNum:%d,imsi:%s",ipIndex,carrierIndicat,imsi);

	sendMsg(ipIndex, buff, sizeof(DataInfo_35));
}

DllExport void g_clearBlackImsiList(int ipIndex,U8 carrierIndicat){
	DataInfo_0 clearImsiInfo;	
	clearImsiInfo.header.msgLenth=sizeof(DataInfo_0);
	clearImsiInfo.header.msgSerialNum=CLEARBLACKIMSI;
	clearImsiInfo.header.msgCodeNum=0x0b;
	clearImsiInfo.header.msgCarrier=carrierIndicat;
	clearImsiInfo.header.msgParam=0;
	
	clearImsiInfo.msgInfo.lenth=sizeof(Unit_0);
	clearImsiInfo.msgInfo.serialNum=0x0054;
	
	char buff[sizeof(DataInfo_0)+1];
	memset(buff,0,sizeof(buff));
	memcpy(buff,&clearImsiInfo,sizeof(DataInfo_0));
	buff[sizeof(DataInfo_0)]='\0';	
	Log("g_clearBlackImsiList&&ipIndex:%d,carNum:%d",ipIndex,carrierIndicat);

	sendMsg(ipIndex, buff, sizeof(DataInfo_0));
}

DllExport void g_clearWhiteImsiList(int ipIndex,U8 carrierIndicat){
	DataInfo_0 clearImsiInfo;	
	clearImsiInfo.header.msgLenth=sizeof(DataInfo_0);
	clearImsiInfo.header.msgSerialNum=CLEARWHITEIMSI;
	clearImsiInfo.header.msgCodeNum=0x0b;
	clearImsiInfo.header.msgCarrier=carrierIndicat;
	clearImsiInfo.header.msgParam=0;
	
	clearImsiInfo.msgInfo.lenth=sizeof(Unit_0);
	clearImsiInfo.msgInfo.serialNum=0x0055;
	
	char buff[sizeof(DataInfo_0)+1];
	memset(buff,0,sizeof(buff));
	memcpy(buff,&clearImsiInfo,sizeof(DataInfo_0));
	buff[sizeof(DataInfo_0)]='\0';
	Log("g_clearWhiteImsiList&&ipIndex:%d,carNum:%d",ipIndex,carrierIndicat);

	sendMsg(ipIndex, buff, sizeof(DataInfo_0));
}

DllExport int g_getHeartBeatCallBack(int (*pFun)(int ipIndex,char carrIndex, int state)){
	pHeartBeatFun=pFun;
	return 1;
}

DllExport int g_getMsgReportCallBack(int (*pFun)(MsgInfo* p_msgInfo)){
	pMsgReportFun=pFun;
	return 1;
}

DllExport int g_getImsiCheckCallBack(int (*pFun)(ImsiCheckInfo* p_imsiCheckInfo)){
	pImsiCheckFun=pFun;
	return 1;
}

DllExport int g_getImsiReportCallBack(int (*pFun)(ImsiReportInfo* p_imsiReportInfo)){
	pImsiReportFun=pFun;
	return 1;
}


//DllExport int  g_carrFrePointCheckCallBack(int (*pFun)(CarrFrePointCheckInfo* p_carrCheckInfo)){
DllExport int  g_carrFrePointCheckCallBack(int (*pFun)(int ipIndex, char carrIndex,int carr)){
	pCarrCheckInfo=pFun;
	return 1;
}
