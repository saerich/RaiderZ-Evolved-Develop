#pragma once
// App.h : header file
//

#include "DLL_Interface.h"

// redefine MFC's METHOD_PROLOGUE for non-automation application
#define MY_METHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = \
		((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass)));

class CMainFrame;
class CCoreToolRS3View;
class CCoreToolRS3Doc;

/////////////////////////////////////////////////////////////////////////////
// App command target

class App : public CCmdTarget
{
private:
	CWinApp*			m_pWinApp;

	CMultiDocTemplate	*m_pDoc1Templ, *m_pDoc2Templ;
	HMODULE				m_hDLL;
	HINSTANCE			m_hEXE; 

    BOOL ExternalQueryInterface(int nId, void** ppvObj);
	void SetRsc();
	void RestoreRsc();

public:
	App();
	virtual ~App();
	virtual void OnFinalRelease( );

    class XDocument : public MyIDocument
    {
		CMainFrame*			m_pMainFrame_;
		CCoreToolRS3View*	m_pView_;
		CCoreToolRS3Doc*	m_pDoc_;

		int					m_nExitCode_;
		ULONG_PTR			m_gdiplusToken;

	public:
        XDocument()
		{
			TRACE("Entering XDocument ctor %p\n", this);
			m_nExitCode_ = 0;
		}

        virtual BOOL QueryInterface( int nId, void** ppvObj );
        virtual DWORD Release( );
        virtual DWORD AddRef( );

		virtual CDocTemplate* CreateDocTempl( CWinApp*, int );

		virtual void CreateFrame();

		virtual void OnInitInstance() override;
		virtual void OnPostCreate() override;
		virtual void OnExitInstance() override;

		virtual BOOL OnIdle(LONG lCount);
		virtual void OnShellOpenFile();
		virtual bool PreTranslateMessage(MSG* pMsg) override;

		virtual int GetExitCode(){ return m_nExitCode_; }

	} m_xDocument;
	friend class XDocument;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
