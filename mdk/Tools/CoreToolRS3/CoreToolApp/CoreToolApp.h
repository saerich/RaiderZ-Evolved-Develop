// exe.h : main header file for the EXE application
//
#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// forward declarations
struct MyIDocument;

/////////////////////////////////////////////////////////////////////////////
// CExeApp:
// See exe.cpp for the implementation of this class
//

class CExeApp : public CWinAppEx
{
public:
	CExeApp();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExeApp)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	// Implementation
	//{{AFX_MSG(CExeApp)
	afx_msg void OnAppAbout();
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	MyIDocument *pIDoc;

	void InitDll();
	void ReleaseDll();		
public:
	virtual BOOL OnIdle(LONG lCount);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

