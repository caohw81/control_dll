// g_control_dll_testDlg.cpp : implementation file
//

#include "stdafx.h"
#include "g_control_dll_test.h"
#include "g_control_dll_testDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CG_control_dll_testDlg *pDlg;


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CG_control_dll_testDlg dialog

CG_control_dll_testDlg::CG_control_dll_testDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CG_control_dll_testDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CG_control_dll_testDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_ipIndex=1;
	m_carrierIndicat_int=0;
	m_mcc=_T("460");
	m_mnc=_T("00");
	m_lac=_T("3080");
	m_ci=_T("8003");
	m_tac=_T("60");
	m_carrierFrePoint=_T("60");
	m_downAttenuation=_T("30");
	
	m_controlMode_int=0;
	m_workMode_int=0;
	m_msgSendMode_int=0;
	
	m_addBlackImsi=_T("460000000000005");
	m_addWhiteImsi=_T("460000000000050");	
	
	m_deleteBlackImsi=_T("460000000000005");	
	m_deleteWhiteImsi=_T("460000000000050");	;
		
		
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CG_control_dll_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CG_control_dll_testDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	
	DDX_Text(pDX, IDC_EDIT1, m_ipIndex);
	DDX_Text(pDX, IDC_EDIT2, m_carrierIndicat_int);
	
	DDX_Text(pDX, IDC_EDIT3, m_mcc);
	DDX_Text(pDX, IDC_EDIT4, m_mnc);		
	DDX_Text(pDX, IDC_EDIT5, m_lac);
	DDX_Text(pDX, IDC_EDIT6, m_ci);
	DDX_Text(pDX, IDC_EDIT7, m_tac);
	DDX_Text(pDX, IDC_EDIT8, m_carrierFrePoint);
	DDX_Text(pDX, IDC_EDIT9, m_downAttenuation);

	DDX_Text(pDX, IDC_EDIT10, m_controlMode_int);
	DDX_Text(pDX, IDC_EDIT11, m_workMode_int);
	DDX_Text(pDX, IDC_EDIT12, m_msgSendMode_int);	
	
	DDX_Text(pDX, IDC_EDIT13, m_addBlackImsi);
	DDX_Text(pDX, IDC_EDIT14, m_addWhiteImsi);
	DDX_Text(pDX, IDC_EDIT15, m_deleteBlackImsi);
	DDX_Text(pDX, IDC_EDIT16, m_deleteWhiteImsi);

	DDX_Control(pDX, IDC_LIST1, m_list);
	
}

BEGIN_MESSAGE_MAP(CG_control_dll_testDlg, CDialog)
	//{{AFX_MSG_MAP(CG_control_dll_testDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, InitRun)
	ON_BN_CLICKED(IDC_BUTTON2, setCellConfig)
	ON_BN_CLICKED(IDC_BUTTON3, setRunningMode)
	ON_BN_CLICKED(IDC_BUTTON4, turnOnRF)
	ON_BN_CLICKED(IDC_BUTTON5, OnturnOffRF)
	ON_BN_CLICKED(IDC_BUTTON6, restartSystem)
	ON_BN_CLICKED(IDC_BUTTON7, restartCard)
	ON_BN_CLICKED(IDC_BUTTON8, addBlack)
	ON_BN_CLICKED(IDC_BUTTON9, addWhite)
	ON_BN_CLICKED(IDC_BUTTON14, clearBlack)
	ON_BN_CLICKED(IDC_BUTTON15, clearWhite)
	ON_BN_CLICKED(IDC_BUTTON10, deleteBlack)
	ON_BN_CLICKED(IDC_BUTTON11, deleteWhite)
	ON_BN_CLICKED(IDC_BUTTON16, checkBlackImsi)
	ON_BN_CLICKED(IDC_BUTTON17, checkWhiteImsi)
	ON_BN_CLICKED(IDC_BUTTON12, CarrFrePoinCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CG_control_dll_testDlg message handlers

BOOL CG_control_dll_testDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
pDlg = this;
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CG_control_dll_testDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CG_control_dll_testDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CG_control_dll_testDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


int g_getHeartBeatCallBackFun(int state){
	CString str;
	str.Format("–ƒÃ¯ state:%d", state);
	pDlg->m_list.AddString(str);
	//int count = pDlg->m_list.GetCount();
	//pDlg->m_list.SetCurSel(count -1);

	return 1;
}


int g_getMsgReportCallBackFun(MsgInfo* p_msgInfo){
	string reportMsg;
	switch(p_msgInfo->msgType){
	case 0x02:
		reportMsg="–°«¯…Ë÷√";
		break;
	case 0x03:
		reportMsg="‘À––ƒ£ Ω…Ë÷√";
		break;
	case 0x04:
		reportMsg="ø™∆Ù…‰∆µ";
		break;
	case 0x05:
		reportMsg="πÿ±’…‰∆µ";
		break;
	case 0x06:
		reportMsg="÷ÿ∆Ù…‰∆µ";
		break;
	case 0x07:
		reportMsg="÷ÿ∆Ù∞Âø®";
		break;
	case 0x50:
		reportMsg="ÃÌº”∫⁄√˚µ•";
		break;
	case 0x51:
		reportMsg="ÃÌº”∞◊√˚µ•";
		break;
	case 0x52:
		reportMsg="…æ≥˝∫⁄√˚µ•";
		break;
	case 0x53:
		reportMsg="…æ≥˝∞◊√˚µ•";
		break;
	case 0x54:
		reportMsg="«Âø’∫⁄√˚µ•";
		break;
	case 0x55:
		reportMsg="«Âø’∞◊√˚µ•";
		break;
	}
	
	CString str;
	str.Format("%s  ipIndex:%d\t carrierIndex:%d\t ",reportMsg.c_str(), p_msgInfo->ipIndex, p_msgInfo->carrierIndex);
	pDlg->m_list.AddString(str);
//	int count = pDlg->m_list.GetCount();
//	pDlg->m_list.SetCurSel(count -1);
	return 1;
}


int g_getImsiCheckCallBackFun(ImsiCheckInfo* p_imsiCheckInfo){
	CString str;
	CString str2;
	switch(p_imsiCheckInfo->msgType){
		
		case 0x56:	
			str2.Format("%s",p_imsiCheckInfo->imsi);
			str.Format("≤È—Ø∫⁄√˚µ•£∫ipIndex:%d  carrierIndex:%d  imsi:%s",p_imsiCheckInfo->ipIndex, p_imsiCheckInfo->carrierIndex,str2);
			pDlg->m_list.AddString(str);
			//pDlg->m_list.AddString(p_imsiCheckInfo->imsi);
			//int count = pDlg->m_list.GetCount();
			//pDlg->m_list.SetCurSel(count -1);
			break;
		case 0x57:
			str2.Format("%s",p_imsiCheckInfo->imsi);
			str.Format("≤È—Ø∞◊√˚µ•£∫ipIndex:%d  carrierIndex:%d  imsi:%s",p_imsiCheckInfo->ipIndex, p_imsiCheckInfo->carrierIndex,str2);
			pDlg->m_list.AddString(str);
			//int count = pDlg->m_list.GetCount();
			//pDlg->m_list.SetCurSel(count -1);
			break;
		default:
			break;
	}
	return 1;
}


int g_getImsiReportCallBackFun(ImsiReportInfo* p_imsiReportInfo){
	CString str;
	str.Format("imsi…œ±®@imsi:%s, imei:%s", p_imsiReportInfo->imsi,p_imsiReportInfo->imei );
	pDlg->m_list.AddString(str);

	return 1;
}

int g_carrFrePointCheckCallBackFun(CarrFrePointCheckInfo* p_carrInfo){
	CString str;
	str.Format("carrFrePoint@ ipIndex:%d, carIndex:%d, carrFre:%d", p_carrInfo->ipIndex,p_carrInfo->carrierIndex, p_carrInfo->carrFrePoint);
	pDlg->m_list.AddString(str);

	return 1;
}


void CG_control_dll_testDlg::InitRun() 
{
	// TODO: Add your control notification handler code here
	hDll = LoadLibrary("g_control_dll.dll");

	if (NULL==hDll){
		//AfxMessageBox("DLLº”‘ÿ ß∞‹");
		pDlg->m_list.AddString("DLLº”‘ÿ ß∞‹");
		int count = pDlg->m_list.GetCount();
		pDlg->m_list.SetCurSel(count -1);
		return;
	}
	else{
		pDlg->m_list.AddString("DLLº”‘ÿ≥…π¶");
		int count = pDlg->m_list.GetCount();
		pDlg->m_list.SetCurSel(count -1);
	}

	typedef int (*g_getHeartBeatCallBack)(int (*func)(int state));
	g_getHeartBeatCallBack pf1=(g_getHeartBeatCallBack)GetProcAddress(hDll,"g_getHeartBeatCallBack");
	if(pf1)
	{
		pf1(g_getHeartBeatCallBackFun);
	}

	typedef int (*g_getMsgReportCallBack)(int (*func)(MsgInfo* p_msgInfo));
	g_getMsgReportCallBack pf2=(g_getMsgReportCallBack)GetProcAddress(hDll,"g_getMsgReportCallBack");
	if(pf2)
	{
		pf2(g_getMsgReportCallBackFun);
	}

	typedef int (*g_getImsiCheckCallBack)(int (*func)(ImsiCheckInfo* p_imsiCheckInfo));
	g_getImsiCheckCallBack pf3=(g_getImsiCheckCallBack)GetProcAddress(hDll,"g_getImsiCheckCallBack");
	if(pf3)
	{
		pf3(g_getImsiCheckCallBackFun);
	}

	typedef int(* g_getImsiReportCallBack)(int (*pFun)(ImsiReportInfo* p_imsiReportInfo));
	g_getImsiReportCallBack pf4=(g_getImsiReportCallBack)GetProcAddress(hDll,"g_getImsiReportCallBack");
	if(pf4){
		pf4(g_getImsiReportCallBackFun);
	}
	
	typedef int(* g_carrCallBack)(int (*pFun)(CarrFrePointCheckInfo* p_carrFreInfo));
	g_carrCallBack pf5=(g_carrCallBack)GetProcAddress(hDll,"g_carrFrePointCheckCallBack");
	if(pf5){
		pf5(g_carrFrePointCheckCallBackFun);
	}

	UpdateData(TRUE);
	
	typedef	 void (* INITRUN)();
	INITRUN pInitRun = (INITRUN)GetProcAddress(hDll,"g_initRun");

	if (NULL==pInitRun){
		MessageBox("DLL÷–∫Ø ˝—∞’“ ß∞‹"); return ;
	}
    pInitRun();
}

void CG_control_dll_testDlg::setCellConfig() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;

	UpdateData(TRUE);
	m_carrierIndicat= m_carrierIndicat_int;

	_int64 mcc_64=_atoi64(m_mcc);
	_int64 mnc_64=_atoi64(m_mnc);
	_int64 lac_64=_atoi64(m_lac);
	_int64 ci_64=_atoi64(m_ci);
	_int64 carrierFrePoint_64=_atoi64(m_carrierFrePoint);
	_int64 downAttenuation_64=_atoi64(m_downAttenuation);
	
	typedef	 void (* SETCELLCONFIG)(int ipIndex, U8 carrierIndicat,  U8* mcc,  U8* mnc,  U8* lac,  U8* ci,  U8* tac,  U8* carrierFrePoint,  U8* downAttenuation);
	SETCELLCONFIG pSetCellConfig = (SETCELLCONFIG)GetProcAddress(hDll,"g_setCellConfig");

	if (NULL==pSetCellConfig){
		MessageBox("DLL÷–∫Ø ˝—∞’“ ß∞‹"); return ;
	}
	char *p_m_mcc=(LPSTR)(LPCTSTR)m_mcc;
	char *p_m_mnc=(LPSTR)(LPCTSTR)m_mnc;
	char *p_m_lac=(LPSTR)(LPCTSTR)m_lac;
	char *p_m_ci=(LPSTR)(LPCTSTR)m_ci;
	char *p_m_tac=(LPSTR)(LPCTSTR)m_tac;
	char *p_m_carrierFrePoint=(LPSTR)(LPCTSTR)m_carrierFrePoint;
	char *p_m_downAttenuation=(LPSTR)(LPCTSTR)m_downAttenuation;


    pSetCellConfig(m_ipIndex, m_carrierIndicat,p_m_mcc,p_m_mnc,p_m_lac,p_m_ci,p_m_tac,p_m_carrierFrePoint,p_m_downAttenuation);
	//pSetCellConfig(m_ipIndex, m_carrierIndicat,mcc_64,mnc_64,lac_64,ci_64,m_tac,carrierFrePoint_64,downAttenuation_64);

}

void CG_control_dll_testDlg::setRunningMode() 
{
	// TODO: Add your control notification handler code here

	if(hDll==NULL)
		return;

	UpdateData(TRUE);
	//m_carrierIndicat='0'+m_carrierIndicat_int;
//	m_controlMode='0'+m_controlMode_int;
//	m_workMode='0'+m_workMode_int;
//	m_msgSendMode='0'+m_msgSendMode_int;


/*	m_carrierIndicat=m_carrierIndicat_int;
	m_controlMode=m_controlMode_int;
	m_workMode=m_workMode_int;
	m_msgSendMode=m_msgSendMode_int;
*/
	m_carrierIndicat=m_carrierIndicat_int;
	m_controlMode=0x01;
	m_workMode=0x02;
	m_msgSendMode=0x01;


	typedef	 void (* SETRUNNINGMODE)(int ipIndex,U8 carrierIndicat, U8 controlMode, U8 workMode, U8 msgSendMode);
	SETRUNNINGMODE pSetRunnigMode = (SETRUNNINGMODE)GetProcAddress(hDll,"g_setRunningMode");

	if (NULL==pSetRunnigMode){
		MessageBox("DLL÷–∫Ø ˝—∞’“ ß∞‹"); return ;
	}
    pSetRunnigMode(m_ipIndex, m_carrierIndicat,m_controlMode,m_workMode,m_msgSendMode );	
}

void CG_control_dll_testDlg::turnOnRF(){
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;

	UpdateData(TRUE);
	
	typedef	 void (* TURNONRFFUN)(int ipIndex, U8 carrierIndicat);
	TURNONRFFUN pTurnOnRF = (TURNONRFFUN)GetProcAddress(hDll,"g_turnOnRF");

	if (NULL==pTurnOnRF){
		MessageBox("DLL÷–∫Ø ˝—∞’“ ß∞‹"); return ;
	}
    pTurnOnRF(m_ipIndex, m_carrierIndicat);	
}

void CG_control_dll_testDlg::OnturnOffRF() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
			return;
	UpdateData(TRUE);
	m_carrierIndicat=m_carrierIndicat_int;

	typedef	 void (* TURNOFFRFFUN)(int ipIndex,U8 carrierIndicat);
	TURNOFFRFFUN pTurnOffRF = (TURNOFFRFFUN)GetProcAddress(hDll,"g_turnOffRF");

	if (NULL==pTurnOffRF){
		MessageBox("DLL÷–∫Ø ˝—∞’“ ß∞‹"); return ;
	}
    pTurnOffRF(m_ipIndex, m_carrierIndicat);	
}

void CG_control_dll_testDlg::restartSystem() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
			return;
	UpdateData(TRUE);
	m_carrierIndicat=m_carrierIndicat_int;

	typedef	 void (* RESTARTSYSTEMFUN)(int ipIndex,U8 carrierIndicat);
	RESTARTSYSTEMFUN pRrestartSystem = (RESTARTSYSTEMFUN)GetProcAddress(hDll,"g_restartSystem");

	if (NULL==pRrestartSystem){
		MessageBox("DLL÷–∫Ø ˝—∞’“ ß∞‹"); return ;
	}
    pRrestartSystem(m_ipIndex, m_carrierIndicat);	
}

void CG_control_dll_testDlg::restartCard() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;
	UpdateData(TRUE);
	m_carrierIndicat=m_carrierIndicat_int;

	typedef	 void (* RESTARTCARDFUN)(int ipIndex,U8 carrierIndicat);
	RESTARTCARDFUN pRrestartCard = (RESTARTCARDFUN)GetProcAddress(hDll,"g_restartCard");

	if (NULL==pRrestartCard){
		MessageBox("DLL÷–∫Ø ˝—∞’“ ß∞‹"); return ;
	}
    pRrestartCard(m_ipIndex, m_carrierIndicat);	
}

void CG_control_dll_testDlg::addBlack() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;
	UpdateData(TRUE);
	m_carrierIndicat=m_carrierIndicat_int;

	int lenth=m_addBlackImsi.GetLength();

	if(m_addBlackImsi.GetLength()<15){
		AfxMessageBox("imsi¥Ì");
		return;
	}
	char* imsiChar=(char* )malloc(lenth+1);
	memset(imsiChar,0,lenth+1);
	memcpy(imsiChar,m_addBlackImsi,m_addBlackImsi.GetLength());


/*	char imsiChar[16];
	memset(imsiChar,0,16);
	strcpy(imsiChar,m_addBlackImsi);
*/
	typedef	 void (* ADDIMSI)(int ipIndex,U8 carrierIndicat, char* imsi);
	ADDIMSI pAddImsi = (ADDIMSI)GetProcAddress(hDll,"g_addBlackImsi");

	if (NULL==pAddImsi){
		MessageBox("DLL÷–∫Ø ˝—∞’“ ß∞‹"); return;
	}
	
	CString str;
	str.Format("add∫⁄√˚µ•£∫imsi:%s",imsiChar);
	pDlg->m_list.AddString(str);

    pAddImsi(m_ipIndex, m_carrierIndicat,imsiChar);
	free(imsiChar);
}

void CG_control_dll_testDlg::addWhite() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;
	UpdateData(TRUE);
	m_carrierIndicat=m_carrierIndicat_int;

	int lenth=m_addWhiteImsi.GetLength();
	if(m_addWhiteImsi.GetLength()<15){
		AfxMessageBox("imsi¥Ì");
		return;
	}
	char* imsiChar=(char* )malloc(lenth+1);
	memset(imsiChar,0,lenth+1);
	memcpy(imsiChar,m_addWhiteImsi,m_addWhiteImsi.GetLength());
	//strcpy(imsiChar,m_addWhiteImsi);

	typedef	 void (* ADDIMSI)(int ipIndex,U8 carrierIndicat, char* imsi);
	ADDIMSI pAddImsi = (ADDIMSI)GetProcAddress(hDll,"g_addWhiteImsi");

	if (NULL==pAddImsi){
		MessageBox("DLL÷–∫Ø ˝—∞’“ ß∞‹"); 
		free(imsiChar);
		return;
	}
	
		
	CString str;
	str.Format("add∞◊√˚µ•£∫imsi:%s",imsiChar);
	pDlg->m_list.AddString(str);
	
    pAddImsi(m_ipIndex, m_carrierIndicat,imsiChar);
	free(imsiChar);
}

void CG_control_dll_testDlg::deleteBlack() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;
	UpdateData(TRUE);
	m_carrierIndicat=m_carrierIndicat_int;

	int lenth=m_deleteBlackImsi.GetLength();
	if(m_deleteBlackImsi.GetLength()<15){
		AfxMessageBox("imsi¥Ì");
		return;
	}
	
	char* imsiChar=(char* )malloc(lenth+1);
	memset(imsiChar,0,lenth+1);
	memcpy(imsiChar,m_deleteBlackImsi,m_deleteBlackImsi.GetLength());
	//strcpy(imsiChar,m_deleteBlackImsi);

	typedef	 void (* DELETEIMSI)(int ipIndex,U8 carrierIndicat, char* imsi);
	DELETEIMSI pDeleteImsi = (DELETEIMSI)GetProcAddress(hDll,"g_deleteBlackImsi");

	if (NULL==pDeleteImsi){
		MessageBox("DLL÷–∫Ø ˝—∞’“ ß∞‹"); 
		free(imsiChar);
		return;
	}
    pDeleteImsi(m_ipIndex, m_carrierIndicat,imsiChar);
	free(imsiChar);
}

void CG_control_dll_testDlg::deleteWhite() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;
	UpdateData(TRUE);
	m_carrierIndicat=m_carrierIndicat_int;

	int lenth=m_deleteWhiteImsi.GetLength();
	if(m_deleteWhiteImsi.GetLength()<15){
		AfxMessageBox("imsi¥Ì");
		return;
	}
	
	char* imsiChar=(char* )malloc(lenth+1);
	memset(imsiChar,0,lenth+1);
	memcpy(imsiChar,m_deleteWhiteImsi,m_deleteWhiteImsi.GetLength());
	//strcpy(imsiChar,m_deleteWhiteImsi);

	typedef	 void (* DELETEIMSI)(int ipIndex,U8 carrierIndicat, char* imsi);
	DELETEIMSI pDeleteImsi = (DELETEIMSI)GetProcAddress(hDll,"g_deleteWhiteImsi");

	if (NULL==pDeleteImsi){
		MessageBox("DLL÷–∫Ø ˝—∞’“ ß∞‹"); 
		free(imsiChar);
		return;
	}
    pDeleteImsi(m_ipIndex, m_carrierIndicat,imsiChar);
	free(imsiChar);
}

void CG_control_dll_testDlg::clearBlack() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;

	UpdateData(TRUE);
	m_carrierIndicat=m_carrierIndicat_int;
	
	typedef	 void (* CLEARBLACKLIST)(int ipIndex,U8 carrierIndicat);
	CLEARBLACKLIST pClearBlackList = (CLEARBLACKLIST)GetProcAddress(hDll,"g_clearBlackImsiList");

	if (NULL==pClearBlackList){
		MessageBox("DLL÷–∫Ø ˝—∞’“ ß∞‹"); return ;
	}
    pClearBlackList(m_ipIndex, m_carrierIndicat);
}

void CG_control_dll_testDlg::clearWhite() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;

	UpdateData(TRUE);
	m_carrierIndicat=m_carrierIndicat_int;

	typedef	 void (* CLEARWHITELIST)(int ipIndex,U8 carrierIndicat);
	CLEARWHITELIST pClearWhiteList = (CLEARWHITELIST)GetProcAddress(hDll,"g_clearWhiteImsiList");

	if (NULL==pClearWhiteList){
		MessageBox("DLL÷–∫Ø ˝—∞’“ ß∞‹"); return ;
	}
    pClearWhiteList(m_ipIndex, m_carrierIndicat);
	
}


void CG_control_dll_testDlg::checkBlackImsi() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;

	UpdateData(TRUE);
	m_carrierIndicat=m_carrierIndicat_int;

	typedef	 void (* CHECKBLACKLIST)(int ipIndex,U8 carrierIndicatt);
	CHECKBLACKLIST pCHeckBlackList = (CHECKBLACKLIST)GetProcAddress(hDll,"g_checkBlackImsi");

	if (NULL==pCHeckBlackList){
		MessageBox("DLL÷–∫Ø ˝—∞’“ ß∞‹"); return ;
	}
    pCHeckBlackList(m_ipIndex, m_carrierIndicat);
}

void CG_control_dll_testDlg::checkWhiteImsi() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;

	UpdateData(TRUE);
	m_carrierIndicat=m_carrierIndicat_int;

	typedef	 void (* CHECKWHITELIST)(int ipIndex,U8 carrierIndicatt);
	CHECKWHITELIST pCHeckWhiteList = (CHECKWHITELIST)GetProcAddress(hDll,"g_checkWhiteImsi");

	if (NULL==pCHeckWhiteList){
		MessageBox("DLL÷–∫Ø ˝—∞’“ ß∞‹"); return ;
	}
    pCHeckWhiteList(m_ipIndex, m_carrierIndicat);
}


void CG_control_dll_testDlg::CarrFrePoinCheck() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;

	UpdateData(TRUE);
	m_carrierIndicat=m_carrierIndicat_int;
	
	typedef	 void (* CARRFREPOINTCHECKFUNN)(int ipIndex,U8 carrierIndicatt);
	CARRFREPOINTCHECKFUNN pCarrFrePointCheck = (CARRFREPOINTCHECKFUNN)GetProcAddress(hDll,"g_carrFrePointCheck");

	if (NULL==pCarrFrePointCheck){
		MessageBox("DLL÷–∫Ø ˝—∞’“ ß∞‹"); 
		return ;
	}
    pCarrFrePointCheck(m_ipIndex, m_carrierIndicat);
	
}
