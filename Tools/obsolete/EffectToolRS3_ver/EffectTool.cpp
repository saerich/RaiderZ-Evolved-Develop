// EffectTool.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "MainFrm.h"

#include "EffectToolDoc.h"
#include "EffectToolView.h"
#include ".\effecttool.h"
#include "shlwapi.h"
#include "MCrashDump.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// App Globals
TCHAR*         g_strAppTitle	= _T( "EffectTool" );
CRView*			g_rView			= NULL;
CControlView*	g_ControlView	= NULL;

// CEffectToolApp

BEGIN_MESSAGE_MAP(CEffectToolApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
//	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// ǥ�� �μ� ���� ����Դϴ�.
//	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CEffectToolApp ����

CEffectToolApp::CEffectToolApp()
{
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
	MCrashDump::Init(NULL, NULL, false);
}


// ������ CEffectToolApp ��ü�Դϴ�.

CEffectToolApp theApp;

// CEffectToolApp �ʱ�ȭ

BOOL CEffectToolApp::InitInstance()
{
	// Current Directory�� �����.
	char szModuleFileName[_MAX_DIR] = {0,};
	GetModuleFileName(NULL, szModuleFileName, _MAX_DIR);
	PathRemoveFileSpec(szModuleFileName);
	SetCurrentDirectory(szModuleFileName);

	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControls()�� �ʿ��մϴ�. 
	// InitCommonControls()�� ������� ������ â�� ���� �� �����ϴ�.
	InitCommonControls();

	CWinApp::InitInstance();

	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.
	// ���� ���α׷��� ���� ���ø��� ����մϴ�. ���� ���ø���
	// ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEffectToolDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
		RUNTIME_CLASS(CEffectToolView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// ����ٿ� ������ ����� ����ġ�մϴ�. ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	//  [8/11/2006 madduck]
	//g_ControlView->GetParentFrame()->RecalcLayout();   
	m_pMainWnd->SetWindowText( g_strAppTitle );

	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	// SDI ���� ���α׷������� ProcessShellCommand �Ŀ� �̷��� ȣ���� �߻��ؾ� �մϴ�.
	return TRUE;
}



// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� ������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ����

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CEffectToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CEffectToolApp �޽��� ó����


BOOL CEffectToolApp::OnIdle(LONG lCount)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	if(g_rView)
		g_rView->RenderScene();

	// [9/4/2006 madduck]
	// TODO : ��û�� ���ε�...
	Sleep(1);

	return TRUE;//CWinApp::OnIdle(lCount);
}
