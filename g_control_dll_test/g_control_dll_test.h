// g_control_dll_test.h : main header file for the G_CONTROL_DLL_TEST application
//

#if !defined(AFX_G_CONTROL_DLL_TEST_H__F407A47B_9212_4A4B_AC33_EEF6E1B9C7FA__INCLUDED_)
#define AFX_G_CONTROL_DLL_TEST_H__F407A47B_9212_4A4B_AC33_EEF6E1B9C7FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


/////////////////////////////////////////////////////////////////////////////
// CG_control_dll_testApp:
// See g_control_dll_test.cpp for the implementation of this class
//

class CG_control_dll_testApp : public CWinApp
{
public:
	CG_control_dll_testApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CG_control_dll_testApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CG_control_dll_testApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_G_CONTROL_DLL_TEST_H__F407A47B_9212_4A4B_AC33_EEF6E1B9C7FA__INCLUDED_)
