// MDBPressor.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "MDBPressorApp.h"
#include "MDBPressorDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// MDBPressorApp

BEGIN_MESSAGE_MAP(MDBPressorApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// MDBPressorApp ����

MDBPressorApp::MDBPressorApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
	m_dlg			= NULL;

	m_bShutdown		= FALSE;
}



void MDBPressorApp::HeartBeat()
{
	if (FALSE == m_dlg->Run())
		return;
}



// ������ MDBPressorApp ��ü�Դϴ�.

MDBPressorApp theApp;


// MDBPressorApp �ʱ�ȭ

BOOL MDBPressorApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�.
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));

	m_dlg	= new MDBPressorDlg;

	m_pMainWnd = m_dlg;
	m_dlg->Create( IDD_MDBPRESSOR_DIALOG );


	return TRUE;
}

int MDBPressorApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	m_dlg->DestroyWindow();
	delete m_dlg;

	return CWinApp::ExitInstance();
}

int MDBPressorApp::Run()
{
	ASSERT_VALID(this);
	_AFX_THREAD_STATE* pState = AfxGetThreadState();

	// acquire and dispatch messages until a WM_QUIT message is received.
	for (;;)
	{
		// phase1: check to see if we can do idle work
		if (::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE))
		{
			if (!PumpMessage())
				return ExitInstance();
		}

		if (m_bShutdown == FALSE)
			this->HeartBeat();
		else
		{
			this->ExitInstance();
			break;
		}

	}
	return 0;
}
