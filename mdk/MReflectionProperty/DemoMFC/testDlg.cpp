
// testDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "test.h"
#include "testDlg.h"


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


// CtestDlg ��ȭ ����




CtestDlg::CtestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtestDlg::IDD, pParent)
	, m_pPropCtrl( NULL )
	, m_pPropCtrl2( NULL )
	, m_pPropCtrl3( NULL )
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif

CtestDlg::~CtestDlg()
{
	SAFE_DELETE(m_pPropCtrl);
	SAFE_DELETE(m_pPropCtrl2);
	SAFE_DELETE(m_pPropCtrl3);
}

void CtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CtestDlg �޽��� ó����

BOOL CtestDlg::OnInitDialog()
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

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//InitPropGrid();
	 
	//**************************** MFC ReflectiveProperty ��� ��� **************************************************
	m_pPropCtrl =	m_UI.createPropGridCtrl( GetDlgItem(IDC_STATIC_PROPERTY) );		//�� ���̾˷α׸� �θ�� ������ ������Ƽ�׸�����Ʈ�� ����
	m_UI.BuildUIForObject(m_pPropCtrl,&m_box);														//�ش�׷쳻�� ������Ƽ�׸���������߰�

	m_pPropCtrl2 =	m_UI.createPropGridCtrl( GetDlgItem(IDC_STATIC_PROPERTY2) );		//�� ���̾˷α׸� �θ�� ������ ������Ƽ�׸�����Ʈ�� ����
	m_UI.BuildUIForObject(m_pPropCtrl2,&m_circle);

// 	// multiple object
// 	CReflectivePropertyGroup* pGroupMulti =		m_UI.createGridGroup(m_pPropCtrl,"Multiple Objects");	//������Ƽ�׸��� �׷����

	OBJECT_LIST listObjects;
	listObjects.push_back( &m_multiBox1 );
	listObjects.push_back( &m_multiBox2 );

	m_multiBox2.setNumber( 1 );			// �ٸ���
	m_multiBox2.setHeight( 1 );			// �޶� ���� UI �� �ȳ��;� �Ѵ�
	m_multiBox1.setName( "SameName" );
	m_multiBox2.setName( "SameName" );	// ���Ƽ� ���´�
	m_multiBox1.setTestName( "DiffName1" );	// �޶� �ȳ��;� ��
	m_multiBox2.setTestName( "DiffName2" );

	m_pPropCtrl3 =	m_UI.createPropGridCtrl( GetDlgItem(IDC_STATIC_PROPERTY3) );		//�� ���̾˷α׸� �θ�� ������ ������Ƽ�׸�����Ʈ�� ����
	m_UI.BuildUIForObjects(m_pPropCtrl3, listObjects);								//�ش�׷쳻�� ������Ƽ�׸���������߰�

	//****************************************************************************************************************

	CReflectivePropertyGridCtrl::PROPERTY_CHANGE_HANDLER handler(this, &CtestDlg::OnPropertyChanged );
	m_pPropCtrl->SetCallback( handler );

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CtestDlg::OnPropertyChanged()
{
	// !! ������Ƽ�� ����Ǿ�����
}

CMFCPropertyGridProperty* CtestDlg::createGridGroup( std::string groupName )
{
	CMFCPropertyGridProperty * pGroup = new CMFCPropertyGridProperty( _T(groupName.c_str()) );
	m_wndPropList.AddProperty( pGroup );
	return pGroup;
}

//MFC�� ������Ƽ �׸��带 ����ϴ� ����. �ش�������Ʈ�ϰ�� ������� �Լ�. ��, �̼ҽ� �Ⱥ��� �ȴٴ� �̾߱�.
//������, MFC ������Ƽ �׸������ϴ� ���� �˰� ������ ������.
void CtestDlg::InitPropGrid()  
{
	// Switch look and feel to office 2007 style
 	CMFCVisualManagerOffice2007::SetStyle( CMFCVisualManagerOffice2007::Office2007_ObsidianBlack );
 	CMFCVisualManager::SetDefaultManager( RUNTIME_CLASS( CMFCVisualManagerOffice2007 ));
 
 	CRect rect;
 	this->GetClientRect( rect );
 	this->MapWindowPoints( this, &rect );
 
	rect.bottom-= 50;

 	m_wndPropList.Create( WS_CHILD | WS_BORDER | WS_VISIBLE | WS_TABSTOP, rect, this, 1231 );
 	m_wndPropList.EnableHeaderCtrl( TRUE, _T( "LEFT Property" ), _T( "Right Value" ));
 	m_wndPropList.SetVSDotNetLook( TRUE );

	// Create a property group for appearance


	CMFCPropertyGridProperty * pGroupTest = new CMFCPropertyGridProperty( _T("TEST") );
	m_wndPropList.AddProperty( pGroupTest );
	const int MaxNesting = 5;
	CMFCPropertyGridProperty *pParent = pGroupTest;
	for( int Index = 0; Index < MaxNesting; ++Index )
	{
		CString Text;
		Text.Format( _T( "Nesting %d" ), Index + 1 );

		CMFCPropertyGridProperty* pChild = new CMFCPropertyGridProperty( Text );

		// Display's a combo with options as True, False, Cool!
		COleVariant Bool((short)VARIANT_FALSE, VT_BOOL);
		pParent->AddSubItem(new CMFCPropertyGridProperty(_T("Bool test"), Bool, false));//_T("Testing kids")));
		pParent->AddSubItem( pChild );
		pParent = pChild;
	}

	// A font property
	LOGFONT lf = { 0 };
	GetFont()->GetLogFont( &lf );
	CMFCPropertyGridFontProperty* pFntProp = new CMFCPropertyGridFontProperty( _T( "Font (Font dialog comes up)" ), lf );
	pGroupTest->AddSubItem( pFntProp );

	// Combo property, set sub options which are displayed in a combo
	CMFCPropertyGridProperty* pCmbProp = new CMFCPropertyGridProperty(_T("Border (A combo box)"), _T("Dialog Frame"), _T("One of: None, Thin, Resizable, or Dialog Frame"));
	pCmbProp->AddOption(_T("None"));
	pCmbProp->AddOption(_T("Thin"));
	pCmbProp->AddOption(_T("Resizable"));
	pCmbProp->AddOption(_T("Dialog Frame"));
	pCmbProp->AllowEdit(FALSE);
	pGroupTest->AddSubItem( pCmbProp );

	// A folder browse dialog property
	CMFCPropertyGridFileProperty* pFolderProp = new CMFCPropertyGridFileProperty( _T( "Select folder (Browse for folder dialog)" ), _T( "C:\\Windows" ));
	pGroupTest->AddSubItem( pFolderProp );

	// A file open dialog property
	CMFCPropertyGridFileProperty* pFileProp = new CMFCPropertyGridFileProperty( _T( "Select file (Open file dialog)" ), TRUE, _T( "C:\\Windows" ));
	pGroupTest->AddSubItem( pFileProp );

	// A masked edit control for phone number
	pGroupTest->AddSubItem( new CMFCPropertyGridProperty(_T("Phone (Masked edit)"), _T("(123) 123-12-12"), _T("Enter a phone number"), 0, _T(" ddd  ddd dd dd"), _T("(___) ___-__-__")));

	// A color property
	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty( _T( "Select color" ), RGB( 120, 198, 250 ));
	pGroupTest->AddSubItem( pColorProp );

	// Set custom colors for property grid
	m_wndPropList.SetCustomColors(RGB(228, 243, 254), RGB(46, 70, 165), RGB(200, 236, 209), RGB(33, 102, 49), RGB(255, 229, 216), RGB(128, 0, 0), RGB(159, 159, 255));
}

void CtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CtestDlg::OnPaint()
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
HCURSOR CtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


