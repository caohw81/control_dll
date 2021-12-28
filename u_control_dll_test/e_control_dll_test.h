// e_control_dll_test.h : main header file for the E_CONTROL_DLL_TEST application
//

#if !defined(AFX_E_CONTROL_DLL_TEST_H__65FDE096_144D_48A6_8491_B4DFF1FE2FC2__INCLUDED_)
#define AFX_E_CONTROL_DLL_TEST_H__65FDE096_144D_48A6_8491_B4DFF1FE2FC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CE_control_dll_testApp:
// See e_control_dll_test.cpp for the implementation of this class
//

class CE_control_dll_testApp : public CWinApp
{
public:
	CE_control_dll_testApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CE_control_dll_testApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CE_control_dll_testApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_E_CONTROL_DLL_TEST_H__65FDE096_144D_48A6_8491_B4DFF1FE2FC2__INCLUDED_)
