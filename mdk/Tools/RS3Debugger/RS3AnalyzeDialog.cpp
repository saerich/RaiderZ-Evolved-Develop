
// RS3DebuggerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "RS3Analyzer.h"
#include "RS3AnalyzeDialog.h"
#include "MProfiler.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CRS3AnalyzeDialoga ��ȭ ����




CRS3AnalyzeDialoga::CRS3AnalyzeDialoga(CWnd* pParent /*=NULL*/)
	: CDialog(CRS3AnalyzeDialoga::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//���� �ʱ�ȭ
	m_pPerformanceViewerDialog = NULL;
	m_pProfileDialog = NULL;
}

CRS3AnalyzeDialoga::~CRS3AnalyzeDialoga( void )
{
	if( NULL != m_pPerformanceViewerDialog )
	{
		m_pPerformanceViewerDialog->DestroyWindow();
		delete m_pPerformanceViewerDialog;
		m_pPerformanceViewerDialog = NULL;
	}

	if( NULL != m_pProfileDialog)
	{
		m_pProfileDialog->DestroyWindow();
		delete m_pProfileDialog;
		m_pProfileDialog = NULL;
	}
}

void CRS3AnalyzeDialoga::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRS3AnalyzeDialoga, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BUTTON1, &CRS3AnalyzeDialoga::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CRS3AnalyzeDialoga::OnBnClickedButton2)
END_MESSAGE_MAP()


// CRS3AnalyzeDialoga �޽��� ó����

BOOL CRS3AnalyzeDialoga::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	SetWindowText(DEBUGGER_DIALOG_NAME); 
	
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CRS3AnalyzeDialoga::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CRS3AnalyzeDialoga::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CRS3AnalyzeDialoga::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





BOOL CRS3AnalyzeDialoga::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if( pMsg->message == WM_COPYDATA)
	{
		int d=0;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CRS3AnalyzeDialoga::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	switch( pCopyDataStruct->dwData )
	{
		case DI_CM_PROFILEINFO:
		{
			if( NULL != m_pProfileDialog )
				m_pProfileDialog->AddProfileInfo((char*)pCopyDataStruct->lpData,pCopyDataStruct->cbData);
		}break;

		case DI_CM_PROFILESTRING:
		{
			if( NULL != m_pProfileDialog )
				m_pProfileDialog->SetProfileIDToName((char*)pCopyDataStruct->lpData,pCopyDataStruct->cbData);
		}break;

		case DI_CM_PERFORMANCEINFO:
		{
			if( NULL != m_pPerformanceViewerDialog )
				m_pPerformanceViewerDialog->UpdateProfileInfo((char*)pCopyDataStruct->lpData,pCopyDataStruct->cbData);
		}break;
	}

	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

void CRS3AnalyzeDialoga::OnBnClickedButton1()//PerformanceViewer��ư
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	PerformanceViewerDialogOnOff();
}

void CRS3AnalyzeDialoga::PerformanceViewerDialogOnOff( void )
{
	if( NULL == m_pPerformanceViewerDialog )
	{//ON
		m_pPerformanceViewerDialog = new RResourceAnalyzeDialog;
		m_pPerformanceViewerDialog->Create(IDD_DIALOG_PERFORMANCE,this);
		m_pPerformanceViewerDialog->ShowWindow(SW_SHOW);
	}
	else
	{//OFF
		m_pPerformanceViewerDialog->DestroyWindow();
		delete m_pPerformanceViewerDialog;
		m_pPerformanceViewerDialog = NULL;
	}
}
void CRS3AnalyzeDialoga::OnBnClickedButton2()//Profile��ư
{
	if( NULL == m_pProfileDialog)
	{
		m_pProfileDialog = new RCPUAnalyzeDialog;
		m_pProfileDialog->Create(IDD_DIALOG_PROFILE,this);
		m_pProfileDialog->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pProfileDialog->DestroyWindow();
		delete m_pProfileDialog;
		m_pProfileDialog = NULL;
	}

}
