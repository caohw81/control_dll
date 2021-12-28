// g_control_dll.h : main header file for the G_CONTROL_DLL DLL
//

#if !defined(AFX_G_CONTROL_DLL_H__10FD0A8C_DC91_4E03_A4BF_D8ED0F67F54A__INCLUDED_)
#define AFX_G_CONTROL_DLL_H__10FD0A8C_DC91_4E03_A4BF_D8ED0F67F54A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "stdafx.h"
#include "msg_type.h"
#include <afxsock.h>		// MFC socket extensions
#include "resource.h"		// main symbols
#include "tinystr.h"
#include "tinyxml.h"
#include <atlbase.h>
#include <cstring>
#include <string>
#include <list>
#include <vector>
#include "log_out.h"

/////////////////////////////////////////////////////////////////////////////
// CG_control_dllApp
// See g_control_dll.cpp for the implementation of this class
//
using namespace std;

class CG_control_dllApp : public CWinApp
{
public:
	CG_control_dllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CG_control_dllApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CG_control_dllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_G_CONTROL_DLL_H__10FD0A8C_DC91_4E03_A4BF_D8ED0F67F54A__INCLUDED_)
