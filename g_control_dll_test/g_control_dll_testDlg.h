// g_control_dll_testDlg.h : header file
//

#if !defined(AFX_G_CONTROL_DLL_TESTDLG_H__E7286ADD_EF98_4C60_8998_7BC297592869__INCLUDED_)
#define AFX_G_CONTROL_DLL_TESTDLG_H__E7286ADD_EF98_4C60_8998_7BC297592869__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afxsock.h>
#include "msg_type.h"
#include <iostream>
#include <cstring>

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CG_control_dll_testDlg dialog

class CG_control_dll_testDlg : public CDialog
{
// Construction
public:
	CG_control_dll_testDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CG_control_dll_testDlg)
	enum { IDD = IDD_G_CONTROL_DLL_TEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	HINSTANCE hDll;

	int m_ipIndex;
	int m_carrierIndicat_int;
	CString m_mcc;
	CString m_mnc;
	CString m_lac;
	CString m_ci;
	CString m_tac;
	CString m_carrierFrePoint;
	CString m_downAttenuation;
	
	int m_controlMode_int;
	int m_workMode_int;
	int m_msgSendMode_int;
	
	CString m_addBlackImsi;
	CString m_addWhiteImsi;
	
	CString m_deleteBlackImsi;
	CString m_deleteWhiteImsi;

	U8 m_carrierIndicat;
	U8 m_controlMode;
	U8 m_workMode;
	U8 m_msgSendMode;
		
	CListBox m_list;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CG_control_dll_testDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CG_control_dll_testDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void InitRun();
	afx_msg void setCellConfig();
	afx_msg void setRunningMode();
	afx_msg void turnOnRF();
	afx_msg void OnturnOffRF();
	afx_msg void restartSystem();
	afx_msg void restartCard();
	afx_msg void addBlack();
	afx_msg void addWhite();
	afx_msg void clearBlack();
	afx_msg void clearWhite();
	afx_msg void deleteBlack();
	afx_msg void deleteWhite();
	afx_msg void OnButton16();
	afx_msg void checkBlackImsi();
	afx_msg void checkWhiteImsi();
	afx_msg void CarrFrePoinCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_G_CONTROL_DLL_TESTDLG_H__E7286ADD_EF98_4C60_8998_7BC297592869__INCLUDED_)
