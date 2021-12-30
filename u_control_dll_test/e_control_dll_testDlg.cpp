// e_control_dll_testDlg.cpp : implementation file
//

#include "stdafx.h"
#include "e_control_dll_test.h"
#include "e_control_dll_testDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CE_control_dll_testDlg *pDlg;

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
// CE_control_dll_testDlg dialog

CE_control_dll_testDlg::CE_control_dll_testDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CE_control_dll_testDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CE_control_dll_testDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_ipIndex=1;
	m_euarfcn=10763;
	m_mcc=460;
	m_mnc=_T("01");		
	m_pci=200;
	m_tac=12345;
	m_txpower=0;
	m_periodTac=0;
	m_bandwidth=10;
	m_cellid=_T("11234567");
	//m_cellid=125;

	m_category=0;
	m_priority= _T("Others");
	m_GeranRedirect=0;	
	m_arfcn=0;
	m_UtranRedirect=0;
	m_uarfcn=0;
	m_EutranRedirect=0;
	m_earfcn=0;
	m_RejectMethod=255;

	m_active_mode=1;
	m_mode=1;

	m_addWhite= _T("460015952100828");		
	m_addBlack= _T("460110146756889");

	m_deleteWhite= _T("460000000000005");
	m_deleteBlack= _T("460110146756889");

	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CE_control_dll_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CE_control_dll_testDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Text(pDX, IDC_EDIT_ipIndex, m_ipIndex);

	DDX_Text(pDX, IDC_EDIT_euarfcn, m_euarfcn);
	DDX_Text(pDX, IDC_EDIT_mcc, m_mcc);
	DDX_Text(pDX, IDC_EDIT_mnc, m_mnc);		
	DDX_Text(pDX, IDC_EDIT_pci, m_pci);
	DDX_Text(pDX, IDC_EDIT_tac, m_tac);
	DDX_Text(pDX, IDC_EDIT_txpower, m_txpower);
	DDX_Text(pDX, IDC_EDIT_periodTac, m_periodTac);
	DDX_Text(pDX, IDC_EDIT_bandwidth, m_bandwidth);
	DDX_Text(pDX, IDC_EDIT_celid, m_cellid);

	DDX_Text(pDX, IDC_EDIT_category, m_category);
	DDX_Text(pDX, IDC_EDIT_priority, m_priority);
	DDX_Text(pDX, IDC_EDIT_GeranRedirect, m_GeranRedirect);	
	DDX_Text(pDX, IDC_EDIT_arfcn, m_arfcn);
	DDX_Text(pDX, IDC_EDIT_UtranRedirect, m_UtranRedirect);
	DDX_Text(pDX, IDC_EDIT_uarfcn, m_uarfcn);
	DDX_Text(pDX, IDC_EDIT_EutranRedirect, m_EutranRedirect);
	DDX_Text(pDX, IDC_EDIT_earfcn, m_earfcn);
	DDX_Text(pDX, IDC_EDIT_RejectMethod, m_RejectMethod);

	DDX_Text(pDX, IDC_EDIT_active_mode, m_active_mode);
	DDX_Text(pDX, IDC_EDIT_mode, m_mode);

	DDX_Text(pDX, IDC_EDIT_addWhite, m_addWhite);		
	DDX_Text(pDX, IDC_EDIT_addBblack, m_addBlack);

	DDX_Text(pDX, IDC_EDIT_deleteWhite, m_deleteWhite);
	DDX_Text(pDX, IDC_EDIT_deleteBlack, m_deleteBlack);

	DDX_Control(pDX, IDC_LIST1, m_list);
	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CE_control_dll_testDlg, CDialog)
	//{{AFX_MSG_MAP(CE_control_dll_testDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BUTTON_InitRun, OnInitRun)
	ON_BN_CLICKED(BUTTON_CellConfig, OnBtnCellConfig)
	ON_BN_CLICKED(BUTTON_Redirection, OnBtnRedirection)
	ON_BN_CLICKED(BUTTON_activeCell, OnBtnActiveCell)
	ON_BN_CLICKED(BUTTON_addWhite, OnBtnAddWhite)
	ON_BN_CLICKED(BUTTON_addBlack, OnBtnAddBlack)
	ON_BN_CLICKED(BUTTON_deleteWhite, OnBtnDeleteWhite)
	ON_BN_CLICKED(BUTTON_deleteBlack, OnBtnDeleteBlack)
	ON_BN_CLICKED(BUTTON_search, OnBtnSearch)
	ON_BN_CLICKED(IDC_BUTTON1, cellStatusCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CE_control_dll_testDlg message handlers

BOOL CE_control_dll_testDlg::OnInitDialog()
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

void CE_control_dll_testDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CE_control_dll_testDlg::OnPaint() 
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
HCURSOR CE_control_dll_testDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int e_statusReportFun(int ipIndex, int id,int param){
	//CString str;
	//str.Format("id:%d, param:%d",id,param);
	//pDlg->m_list.AddString(str);
	//int count = pDlg->m_list.GetCount();
	//pDlg->m_list.SetCurSel(count -1);
	if(ipIndex==1){
		if(id==0){
			if(param==1){
				//pDlg->m_list.AddString("心跳接收");
				//int count = pDlg->m_list.GetCount();
				//pDlg->m_list.SetCurSel(count -1);
			}
			else{
				AfxMessageBox("DLL加载失败");
			}
		}else if(id==1){
			if(param==0){
				pDlg->m_list.AddString("小区设置成功");
				int count = pDlg->m_list.GetCount();
				pDlg->m_list.SetCurSel(count -1);
			}
			else if(param==1){
				pDlg->m_list.AddString("小区设置失败");
				int count = pDlg->m_list.GetCount();
				pDlg->m_list.SetCurSel(count -1);
			}
		}else if(id==2){
			if(param==0){
				pDlg->m_list.AddString("策略重定向成功");
				int count = pDlg->m_list.GetCount();
				pDlg->m_list.SetCurSel(count -1);
			}
			else if(param==1){
				pDlg->m_list.AddString("策略重定向失败");
				int count = pDlg->m_list.GetCount();
				pDlg->m_list.SetCurSel(count -1);
				return 0;
			}
		}else if(id==3){
			if(param==0){
				pDlg->m_list.AddString("小区激活成功");
				int count = pDlg->m_list.GetCount();
				pDlg->m_list.SetCurSel(count -1);
			}
			else if(param==1){
				pDlg->m_list.AddString("小区激活失败");
				int count = pDlg->m_list.GetCount();
				pDlg->m_list.SetCurSel(count -1);
				return 0;

			}
		}else if(id==4){
			if(param==0){
				pDlg->m_list.AddString("黑白名单添加成功");
				int count = pDlg->m_list.GetCount();
				pDlg->m_list.SetCurSel(count -1);
			}
			else if(param==1){
				pDlg->m_list.AddString("黑白名单添加失败");
				int count = pDlg->m_list.GetCount();
				pDlg->m_list.SetCurSel(count -1);
				return 0;
			}
		}else if(id==5){
			if(param==0){
				pDlg->m_list.AddString("黑白名单删除成功");
				int count = pDlg->m_list.GetCount();
				pDlg->m_list.SetCurSel(count -1);
			}
			else if(param==1){
				pDlg->m_list.AddString("黑白名单删除失败");
				int count = pDlg->m_list.GetCount();
				pDlg->m_list.SetCurSel(count -1);
				return 0;
			}
		}
	}

	return 1;
}

int  e_getIMSIReportFun(E_IMSIReportInfo*p){
	CString str;
	str.Format("ipIndex:%d\t userType:%d\n\t imsi:%s \t imei:%s rsrp:%d \n ",p->ipIndex, p->userType,p->imsi,p->imei,p->rsrp);
	//str.Format("ipIndex:%d\t userType:%d\t imei:%s \t  rsrp:%d \n ",p->ipIndex, p->userType,p->imei,p->rsrp);
	pDlg->m_list.AddString(str);
	int count = pDlg->m_list.GetCount();
	pDlg->m_list.SetCurSel(count -1);

	return 1;
}

int  e_whiteIMSICheckFun(int ipIndex, char*imsiList, int rows){
	char imsi[16];	
	for(int i=0;i<rows;++i){
		memset(imsi,0,16);
		memcpy(imsi,&imsiList[i*15],15);

		//imsi[15]='\0';
		CString str;
		str.Format("check_whiteImsi ipIndex:%d\t imsi:%s \n ",ipIndex,imsi);
		pDlg->m_list.AddString(str);
		int count = pDlg->m_list.GetCount();
		pDlg->m_list.SetCurSel(count -1);
	}
	return 1;
}

int  e_blackIMSICheckFun(int ipIndex, char*imsiList, int rows){
	char imsi[16];
	for(int i=0;i<rows;++i){
		memset(imsi,0,16);
		memcpy(imsi,&imsiList[i*15],15);
		//imsi[15]='\0';
		CString str;
		str.Format("check_blackImsi ipIndex:%d\t imsi:%s \n ",ipIndex,imsi);
		pDlg->m_list.AddString(str);
		int count = pDlg->m_list.GetCount();
		pDlg->m_list.SetCurSel(count -1);
	}
	return 1;
}

int  e_cellStatusFun(int ipIndex, int status, CellConfigInfo* cellInfo){
	CString str;
	str.Format("cellStatus& ipIndex:%d status:%d euarfcn:%d, pci:%d \n ",ipIndex, status, cellInfo->euarfcn, cellInfo->pci);
	pDlg->m_list.AddString(str);
	int count = pDlg->m_list.GetCount();
	pDlg->m_list.SetCurSel(count -1);
	return 1;
}


void CE_control_dll_testDlg::OnInitRun() {
	// TODO: Add your control notification handler code here
	hDll = LoadLibrary("u_control_dll.dll");

	if (NULL==hDll){
		//AfxMessageBox("DLL加载失败");
		pDlg->m_list.AddString("DLL加载失败");
		int count = pDlg->m_list.GetCount();
		pDlg->m_list.SetCurSel(count -1);
		return;
	}
	else{
		pDlg->m_list.AddString("DLL加载成功");
		int count = pDlg->m_list.GetCount();
		pDlg->m_list.SetCurSel(count -1);
	}

	
	typedef int (*e_statusReportCallBack)(int (*func)(int ipIndex, int id,int param));
	e_statusReportCallBack pf1=(e_statusReportCallBack)GetProcAddress(hDll,"u_getStatusCallBack");
	if(pf1)
	{
		pf1(e_statusReportFun);
	}
	
	typedef int (*e_getIMSIReportCallBack)(int (*func)(E_IMSIReportInfo*p));
	e_getIMSIReportCallBack pf2=(e_getIMSIReportCallBack)GetProcAddress(hDll,"u_getIMSIReportCallBack");
	if(pf2)
	{
		pf2(e_getIMSIReportFun);
	}
	
	typedef int (*e_whiteIMSICheckCallBack)(int (*func)(int ipIndex, char* imsiList, int rows));
	e_whiteIMSICheckCallBack pf3=(e_whiteIMSICheckCallBack)GetProcAddress(hDll,"u_whiteImsiCheckCallBack");
	if(pf3)
	{
		pf3(e_whiteIMSICheckFun);
	}
	
	typedef int (*e_blackIMSICheckCallBack)(int (*func)(int ipIndex, char* imsiList, int rows));
	e_blackIMSICheckCallBack pf4=(e_blackIMSICheckCallBack)GetProcAddress(hDll,"u_blackImsiCheckCallBack");
	if(pf4)
	{
		pf4(e_blackIMSICheckFun);
	}

	typedef int (*e_cellStatusCallBack)(int (*func)(int ipIndex, int status, CellConfigInfo* cellInfo));
	e_cellStatusCallBack pf5=(e_cellStatusCallBack)GetProcAddress(hDll,"u_cellStatusCallBack");
	if(pf5)
	{
		pf5(e_cellStatusFun);
	}

	typedef	 void (* e_initRun)();
	e_initRun pInit = (e_initRun)GetProcAddress(hDll,"u_initRun");
	
	if (NULL==pInit)
	{
		//MessageBox("DLL中函数寻找失败"); return;
		pDlg->m_list.AddString("DLL中函数寻找失败");
		int count = pDlg->m_list.GetCount();
		pDlg->m_list.SetCurSel(count -1);
	}

    pInit();
}

void CE_control_dll_testDlg::OnBtnCellConfig(){
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;
	
	UpdateData(TRUE);
	char p_mnc[10];
	memset(p_mnc,0,10);
	strcpy(p_mnc,m_mnc);

	char p_cellid[15];
	memset(p_cellid,0,15);

	strcpy(p_cellid,m_cellid);

	typedef	 void (* CELLCONFIG)(int ipIndex, int euarfcn, int mcc,char* mnc, int pci, int tac, int txpower, int periodTac, int bandwidth, char* cellid);
/*pow_level   1-5   max:5*/
	CELLCONFIG pCellConfig = (CELLCONFIG)GetProcAddress(hDll,"u_setCellConfig");

	if (NULL==pCellConfig){
		MessageBox("DLL中函数寻找失败"); return ;
	}
    pCellConfig(m_ipIndex, m_euarfcn, m_mcc,p_mnc, m_pci,m_tac, m_txpower, m_periodTac, m_bandwidth, p_cellid);
}

void CE_control_dll_testDlg::OnBtnRedirection() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;
	
	UpdateData(TRUE);
	char p_priority[10];
	memset(p_priority,0,10);
	strcpy(p_priority,m_priority);


	typedef	 void (* REDIRECTION)(int ipIndex, int category, char* priority,int GeranRedirect, int arfcn, int UtranRedirect, int uarfcn, int EutranRedirect, int earfcn,int RejectMethod);
/*pow_level   1-5   max:5*/
	REDIRECTION pRedirect = (REDIRECTION)GetProcAddress(hDll,"u_setRedirection");

	if (NULL==pRedirect){
		MessageBox("DLL中函数寻找失败"); return ;
	}
    pRedirect(m_ipIndex, m_category, p_priority,m_GeranRedirect, m_arfcn, m_UtranRedirect, m_uarfcn, m_EutranRedirect, m_earfcn,m_RejectMethod);
}

void CE_control_dll_testDlg::OnBtnActiveCell()
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;
	
	UpdateData(TRUE);
	typedef	 void (* ACTIVECELL)(int ipIndex, int active_mode, int mode);
/*pow_level   1-5   max:5*/
	ACTIVECELL pActiveCell = (ACTIVECELL)GetProcAddress(hDll,"u_activeCell");

	if (NULL==pActiveCell){
		MessageBox("DLL中函数寻找失败"); return ;
	}
    pActiveCell(m_ipIndex, m_active_mode, m_mode);

}

void CE_control_dll_testDlg::OnBtnAddWhite() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;
	
	UpdateData(TRUE);
	char number[16];
	memset(number,0,16);
	if(m_addWhite.GetLength()!=15){
		AfxMessageBox("imsi错");
		return;
	}
	strcpy(number,m_addWhite);


	typedef	 void (* ADDWHITEIMSI)(int ipIndex, char* whiteImsi);
/*pow_level   1-5   max:5*/
	ADDWHITEIMSI pAddWhiteImsi = (ADDWHITEIMSI)GetProcAddress(hDll,"u_addWhiteImsi");

	if (NULL==pAddWhiteImsi){
		MessageBox("DLL中函数寻找失败"); return ;
	}
    pAddWhiteImsi(m_ipIndex, number);

}

void CE_control_dll_testDlg::OnBtnAddBlack() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;
	
	UpdateData(TRUE);
	char number[16];
	memset(number,0,16);
	if(m_addBlack.GetLength()!=15){
		AfxMessageBox("imsi错");
		return;
	}
	strcpy(number,m_addBlack);
	
	typedef	 void (* ADDWHITEIMSI)(int ipIndex, char* blackImsi);
/*pow_level   1-5   max:5*/
	ADDWHITEIMSI pAddBlackImsi = (ADDWHITEIMSI)GetProcAddress(hDll,"u_addBlackImsi");

	if (NULL==pAddBlackImsi){
		MessageBox("DLL中函数寻找失败"); return ;
	}
    pAddBlackImsi(m_ipIndex, number);
}

void CE_control_dll_testDlg::OnBtnDeleteWhite() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;
	
	UpdateData(TRUE);
	int lenth=m_deleteWhite.GetLength();
	int num=m_deleteWhite.GetLength()/15;
	char* imsiChar=(char* )malloc(lenth+1);
	memset(imsiChar,0,lenth+1);
	if(m_deleteWhite.GetLength()<15){
		AfxMessageBox("imsi错");
		return;
	}
	strcpy(imsiChar,m_deleteWhite);
	typedef	 void (* DELETEWHITEIMSI)(int ipIndex, char* whiteImsi,int num);
/*pow_level   1-5   max:5*/
	DELETEWHITEIMSI pDeleteWhiteImsi = (DELETEWHITEIMSI)GetProcAddress(hDll,"u_deleteWhiteImsi");

	if (NULL==pDeleteWhiteImsi){
		MessageBox("DLL中函数寻找失败"); return ;
	}
    pDeleteWhiteImsi(m_ipIndex, imsiChar,num);
	free(imsiChar);
	
}

void CE_control_dll_testDlg::OnBtnDeleteBlack() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;
	
	UpdateData(TRUE);
	int lenth=m_deleteBlack.GetLength();
	int num=m_deleteBlack.GetLength()/15;
	char* imsiChar=(char* )malloc(lenth+1);
	memset(imsiChar,0,lenth+1);
	if(m_deleteBlack.GetLength()<15){
		AfxMessageBox("imsi错");
		return;
	}
	strcpy(imsiChar,m_deleteBlack);
	typedef	 void (* DELETEBLACKIMSI)(int ipIndex, char* blackImsi,int num);
/*pow_level   1-5   max:5*/
	DELETEBLACKIMSI pDeleteBlackImsi = (DELETEBLACKIMSI)GetProcAddress(hDll,"u_deleteBlackImsi");

	if (NULL==pDeleteBlackImsi){
		MessageBox("DLL中函数寻找失败"); return ;
	}
    pDeleteBlackImsi(m_ipIndex, imsiChar,num);
	free(imsiChar);
}

void CE_control_dll_testDlg::OnBtnSearch()
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;
	
	typedef	 void (* IMSILISTCHECK)(int ipIndex);
/*pow_level   1-5   max:5*/
	IMSILISTCHECK pImsiListCheck = (IMSILISTCHECK)GetProcAddress(hDll,"u_imsiListCheck");

	if (NULL==pImsiListCheck){
		MessageBox("DLL中函数寻找失败"); return ;
	}
    pImsiListCheck(m_ipIndex);
}

void CE_control_dll_testDlg::cellStatusCheck() 
{
	// TODO: Add your control notification handler code here
	if(hDll==NULL)
		return;
	
	typedef	 void (* FEMTOSTATUSCHECK)(int ipIndex);
	/*pow_level   1-5   max:5*/
	FEMTOSTATUSCHECK pFemtoStatusCheck = (FEMTOSTATUSCHECK)GetProcAddress(hDll,"u_femtoStatusRequest");

	if (NULL==pFemtoStatusCheck){
		MessageBox("DLL中函数寻找失败"); return ;
	}
    pFemtoStatusCheck(m_ipIndex);
}
