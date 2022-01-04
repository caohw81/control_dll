// e_control_dll.h : main header file for the E_CONTROL_DLL DLL
//

#if !defined(AFX_E_CONTROL_DLL_H__B7520624_3F87_403A_A159_35F3D3B38A86__INCLUDED_)
#define AFX_E_CONTROL_DLL_H__B7520624_3F87_403A_A159_35F3D3B38A86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "tinystr.h"
#include "tinyxml.h"
#include <atlbase.h>
#include <cstring>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include<pthread.h>
#include "log_out.h"

#pragma comment(lib, "pthreadVC2.lib")

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CE_control_dllApp
// See e_control_dll.cpp for the implementation of this class
//

class CE_control_dllApp : public CWinApp
{
public:
	CE_control_dllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CE_control_dllApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CE_control_dllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_E_CONTROL_DLL_H__B7520624_3F87_403A_A159_35F3D3B38A86__INCLUDED_)
