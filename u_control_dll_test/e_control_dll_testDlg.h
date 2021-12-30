// e_control_dll_testDlg.h : header file
//

#if !defined(AFX_E_CONTROL_DLL_TESTDLG_H__2BD8547D_67CD_445E_9C56_5C1CD5EC9391__INCLUDED_)
#define AFX_E_CONTROL_DLL_TESTDLG_H__2BD8547D_67CD_445E_9C56_5C1CD5EC9391__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CE_control_dll_testDlg dialog

class CE_control_dll_testDlg : public CDialog
{
// Construction
public:
	CE_control_dll_testDlg(CWnd* pParent = NULL);	// standard constructor
	HINSTANCE hDll;
// Dialog Data
	//{{AFX_DATA(CE_control_dll_testDlg)
	enum { IDD = IDD_E_CONTROL_DLL_TEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	int m_ipIndex;
	int m_euarfcn;
	int m_mcc;
	CString m_mnc;		
	int m_pci;
	int m_tac;
	int m_txpower;
	int m_periodTac;
	int m_bandwidth;
	CString m_cellid;

	int m_category;
	CString m_priority;
	int m_GeranRedirect;	
	int m_arfcn;
	int m_UtranRedirect;
	int m_uarfcn;
	int m_EutranRedirect;
	int m_earfcn;
	int m_RejectMethod;

	int m_active_mode;
	int m_mode;

	CString m_addWhite;		
	CString m_addBlack;

	CString m_deleteWhite;

	CString m_deleteBlack;

	CListBox m_list;

	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CE_control_dll_testDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CE_control_dll_testDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnInitRun();
	afx_msg void OnBtnCellConfig();
	afx_msg void OnBtnRedirection();
	afx_msg void OnBtnActiveCell();
	afx_msg void OnBtnAddWhite();
	afx_msg void OnBtnAddBlack();
	afx_msg void OnBtnDeleteWhite();
	afx_msg void OnBtnDeleteBlack();
	afx_msg void OnBtnSearch();
	afx_msg void cellStatusCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_E_CONTROL_DLL_TESTDLG_H__2BD8547D_67CD_445E_9C56_5C1CD5EC9391__INCLUDED_)
