// RCPUAnalyzeDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "RS3Analyzer.h"
#include "RCPUAnalyzeDialog.h"
#include "RDebuggingInfo.h"

// RCPUAnalyzeDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNCREATE(RCPUAnalyzeDialog, CDialog)

RCPUAnalyzeDialog::RCPUAnalyzeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(RCPUAnalyzeDialog::IDD, pParent)
{
	m_bPause = false;
	m_bTopMost = false;
	m_pProfileIDToName = NULL;
}

RCPUAnalyzeDialog::~RCPUAnalyzeDialog()
{
	if( NULL != m_pProfileIDToName )
	{
		delete m_pProfileIDToName;
		m_pProfileIDToName = NULL;
	}
}

void RCPUAnalyzeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ProfileTreeListCtrlLeft);
	DDX_Control(pDX, IDC_LIST2, m_ProfileTreeListCtrlRight);
	DDX_Control(pDX, IDC_STATIC_GRAPH, m_GraphCtrl);
	DDX_Control(pDX, IDC_EDIT_YSCALE,m_YScaleCtrl);
}

BOOL RCPUAnalyzeDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_ProfileTreeListCtrlLeft.Init();
	m_ProfileTreeListCtrlRight.Init();
	m_GraphCtrl.SetProfileDialog(this);
	m_GraphCtrl.SetInit(true);
	m_YScaleCtrl.SetWindowText("700");	//GraphCtrl�� ���� m_YScale�� 700���� �ϵ��ڵ��Ǿ�����.

	SetWindowText(DEBUGGER_PROFILE_DIALOG_NAME); 

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

BEGIN_MESSAGE_MAP(RCPUAnalyzeDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &RCPUAnalyzeDialog::OnBnClickedButtonPause)
	ON_EN_CHANGE(IDC_EDIT_YSCALE, &RCPUAnalyzeDialog::OnEnChangeEditYscale)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_TOPMOST, &RCPUAnalyzeDialog::OnBnClickedCheckTopmost)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_LEFT, &RCPUAnalyzeDialog::OnBnClickedButtonExportLeft)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_RIGHT, &RCPUAnalyzeDialog::OnBnClickedButtonExportRight)
END_MESSAGE_MAP()





// RCPUAnalyzeDialog �޽��� ó�����Դϴ�.

void RCPUAnalyzeDialog::AddProfileInfo( char* pBuffer,int Size )
{
	if( true == m_bPause ) return;

	MBuffer ProfileBuffer;
	ProfileBuffer.SetBuffer(pBuffer,Size);

	CProfileManager* pM = new CProfileManager;
	pM->MakeFromBinary(&ProfileBuffer);

	m_GraphCtrl.AddItem(pM);
}

void RCPUAnalyzeDialog::SetProfileIDToName( char* pBuffer,int Size )
{
	if( true == m_bPause ) return;

	MBuffer ProfileBuffer;
	ProfileBuffer.SetBuffer(pBuffer,Size);

	if( m_pProfileIDToName )
	{
		delete	m_pProfileIDToName;
		m_pProfileIDToName = NULL;
	}

	m_pProfileIDToName = new CProfileIDToName;
	m_pProfileIDToName->MakeFromNameBinary(&ProfileBuffer);
}

void RCPUAnalyzeDialog::SetLeftProfileManager( CProfileManager* pProfileManager )
{
	m_ProfileTreeListCtrlLeft.SetProfileManager(pProfileManager,m_pProfileIDToName);
}

void RCPUAnalyzeDialog::SetRightProfileManager( CProfileManager* pProfileManager )
{
	m_ProfileTreeListCtrlRight.SetProfileManager(pProfileManager,m_pProfileIDToName);
}

void RCPUAnalyzeDialog::OnBnClickedButtonPause()	//Pause��ư Ŭ���ÿ�
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bPause = !m_bPause;
}

void RCPUAnalyzeDialog::OnEnChangeEditYscale()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString String;
	m_YScaleCtrl.GetWindowText(String);
	int YScale = _ttoi(String);

	m_GraphCtrl.SetYScale(YScale);
}

void RCPUAnalyzeDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	static const int DX = 10;
	static const int DY = 10;
	static const int BASE_Y = 200;

	if( m_GraphCtrl.IsInitialized() )
	{
		CRect PR;
		this->GetClientRect(&PR);

		/*
		// TODO: �׷����� �������� �Ǹ鼭 ������ ��Ʈ�ѵ��� ����׿�, 
		// �����ʿ� ��Ʈ�ѵ鸸ŭ ������ �ְ� �׷��� ������� �����߰ڽ��ϴ�.

		m_GraphCtrl.SetWindowPos(NULL,
								PR.left + DX, PR.top + DY,
								(PR.right - DX) - (PR.left + DX ) , (PR.top + BASE_Y - DY) - (PR.top + DY),
								SWP_SHOWWINDOW								);
		*/
	}

	if( true == m_ProfileTreeListCtrlLeft.IsInitialized() )
	{
		CRect PR;
		this->GetClientRect(&PR);

		m_ProfileTreeListCtrlLeft.SetWindowPos(NULL,
			PR.left + DX ,			PR.top + BASE_Y + DY,
			(int)(PR.right * 0.5 - DX) - (PR.left + DX ) ,	(PR.bottom - DY) - (PR.top + BASE_Y + DY) ,
			SWP_SHOWWINDOW);
	}

	if( true == m_ProfileTreeListCtrlRight.IsInitialized() )
	{
		CRect PR;
		this->GetClientRect(&PR);

		m_ProfileTreeListCtrlRight.SetWindowPos(NULL,
			(int)(PR.right * 0.5 + DX) ,			PR.top + BASE_Y + DY,
			(PR.right - DX) - (int)(PR.right * 0.5 + DX ) ,	(PR.bottom - DY) - (PR.top + BASE_Y + DY) ,
			SWP_SHOWWINDOW);
	}

}

void RCPUAnalyzeDialog::OnBnClickedCheckTopmost()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bTopMost = !m_bTopMost;
	if( m_bTopMost )
	{
		SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
	}
	else
	{
		SetWindowPos(&wndNoTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	}
}

void RCPUAnalyzeDialog::ExportToFile( const char* szFileName, CCPUAnalyzeTreeListCtrl& listCtrl )
{
	FILE* fp;
	fopen_s(&fp, szFileName, "w+" );

	if(fp)
	{
		listCtrl.ExportToFile(fp);

		fclose(fp);

		ShellExecute( m_hWnd, _T("open"), szFileName, NULL, NULL, SW_SHOW );
	}
	else
	{
		MessageBox("������ ���µ� �����߽��ϴ�.");
	}
}

void RCPUAnalyzeDialog::OnBnClickedButtonExportLeft()
{
	ExportToFile("cpu_result_left.txt", m_ProfileTreeListCtrlLeft);
}

void RCPUAnalyzeDialog::OnBnClickedButtonExportRight()
{
	ExportToFile("cpu_result_right.txt", m_ProfileTreeListCtrlRight);
}
