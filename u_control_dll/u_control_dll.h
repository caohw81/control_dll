// u_control_dll.h : main header file for the U_CONTROL_DLL DLL
//

#if !defined(AFX_U_CONTROL_DLL_H__CF8D2BC9_D87D_4D7A_B20D_E4757452D438__INCLUDED_)
#define AFX_U_CONTROL_DLL_H__CF8D2BC9_D87D_4D7A_B20D_E4757452D438__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <afxsock.h>		// MFC socket extensions
#include "resource.h"		// main symbols
#include "log_out.h"
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

/////////////////////////////////////////////////////////////////////////////
// CU_control_dllApp
// See u_control_dll.cpp for the implementation of this class
//

using namespace std;

class CU_control_dllApp : public CWinApp
{
public:
	CU_control_dllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CU_control_dllApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CU_control_dllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_U_CONTROL_DLL_H__CF8D2BC9_D87D_4D7A_B20D_E4757452D438__INCLUDED_)
