
#include "stdafx.h"
#include "NewAppEasterEgg.h"



/************************************************************************************
  Dialog
*************************************************************************************/
// Ŭ���� ������
CNewAppEasterEgg::CNewAppEasterEgg( CWnd* pParent) : CDDBDialog( IDD_EASTEREGG, pParent)
{
	//{{AFX_DATA_INIT( CNewAppEasterEgg)
	//}}AFX_DATA_INIT

	// �̰����� ������ ���̾�α��� ������ �Է��Ѵ�
	// �� �� �ڼ��� ������ ���ϸ� CDDBDialog.h ȭ���� Structure of CDDBDIALOGINFO ������ �����Ѵ�
	CDDBDLGINFO DlgInfo;
	DlgInfo.nWidth = 300;
	DlgInfo.nHeight = 180;
	DlgInfo.IDSkinResource = IDR_BMP_POPUPDLG;
	DlgInfo.nTextColor = RGB( 150, 150, 150);
	DlgInfo.nTextBkColor = RGB( 46, 47, 48);
	SetDialogInfo( DlgInfo);

	m_pParentDialog = NULL;
}


// DoDataExchange
void CNewAppEasterEgg::DoDataExchange( CDataExchange* pDX)
{
	CDDBDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewAppEasterEgg)
	DDX_Control(pDX, IDC_MESSAGE, m_cMessage);
	DDX_Control(pDX, IDC_SHOWLATER, m_cCheck);
	DDX_Control(pDX, IDOK,        m_cOK);
	DDX_Control(pDX, IDCANCEL,    m_cCancel);
	//}}AFX_DATA_MAP
}


// Message map
BEGIN_MESSAGE_MAP( CNewAppEasterEgg, CDDBDialog)
	//{{AFX_MSG_MAP(CNewAppEasterEgg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/************************************************************************************
  Ŭ���� �޼��� �ڵ鷯
*************************************************************************************/
// OnInitDialog : ���̾�α׸� �ʱ�ȭ�� �Լ�
BOOL CNewAppEasterEgg::OnInitDialog() 
{
	CDDBDialog::OnInitDialog();


	// ������ �̵�
	if ( m_pParentDialog != NULL)
	{
		CRect rect;
		m_pParentDialog->GetWindowRect( &rect);
		int x = rect.left + rect.Width() / 2 - GetWidth() / 2;
		int y = rect.top + rect.Height() / 2 - GetHeight() / 2;
		MoveWindow( x, y, GetWidth(), GetHeight());
	}
	

	// �г� �׸���
	DrawPanel( 6, 25, GetWidth()-12, GetHeight()-31);


	// ��ư �ʱ�ȭ
	m_cOK.InitDDBButton( IDR_BMP_BUTTON, IDR_WAV_PUSHBUTTON);
	m_cCancel.InitDDBButton( IDR_BMP_BUTTON, IDR_WAV_PUSHBUTTON);


	// ĸ�� ǥ��
	SetWindowText( m_strCaption);
	GetScreen()->PutText(  9, 5, 17, "Arial", m_strCaption, RGB( 0, 0, 0), TS_BOLD);
	GetScreen()->PutText( 10, 6, 17, "Arial", m_strCaption, RGB( 180, 180, 180), TS_BOLD);


	// �޽��� ǥ��
	GetScreen()->PutBitmap( 15, 35, IDR_BMP_CAKE, 0, 0, 60, 60);
	m_cMessage.SetWindowText( m_strMessage);
	m_cMessage.MoveWindow( 83, 35, GetWidth() - 96, GetHeight() - 100);

	// üũ��ư ��ġ
	m_cCheck.MoveWindow( 20, GetHeight() - 70, GetWidth() - 40, 20);


	// ��ư �ʱ�ȭ
	if ( m_nButtonType == MB_OKCANCEL)
	{
		m_cOK.SetWindowText( "OK");
		m_cOK.ShowWindow( SW_SHOW);
		m_cOK.EnableWindow( TRUE);
		m_cOK.MoveWindow( GetWidth() / 2 - m_cOK.GetWidth() - 5, GetHeight() - 35);

		m_cCancel.SetWindowText( "Cancel");
		m_cCancel.ShowWindow( SW_SHOW);
		m_cCancel.EnableWindow( TRUE);
		m_cCancel.MoveWindow( GetWidth() / 2 + 5, GetHeight() - 35);
	}
	else if ( m_nButtonType == MB_YESNO)
	{
		m_cOK.SetWindowText( "Yes");
		m_cOK.ShowWindow( SW_SHOW);
		m_cOK.EnableWindow( TRUE);
		m_cOK.MoveWindow( GetWidth() / 2 - m_cOK.GetWidth() - 5, GetHeight() - 35);

		m_cCancel.SetWindowText( "No");
		m_cCancel.ShowWindow( SW_SHOW);
		m_cCancel.EnableWindow( TRUE);
		m_cCancel.MoveWindow( GetWidth() / 2 + 5, GetHeight() - 35);
	}
	else if ( m_nButtonType == MB_OK)
	{
		m_cOK.SetWindowText( "OK");
		m_cOK.ShowWindow( SW_SHOW);
		m_cOK.EnableWindow( TRUE);
		m_cOK.MoveWindow( GetWidth() / 2 - m_cOK.GetWidth() / 2, GetHeight() - 35);

		m_cCancel.ShowWindow( SW_HIDE);
	}

	m_Sound.PlayWavSound( IDR_WAV_BIRTHDAYSONG);


	return true;
}


// SetDialog
void CNewAppEasterEgg::SetDialog( CDialog* pDialog, CString strCaption, CString strMessage, UINT nButtonType)
{
	m_pParentDialog = pDialog;
	m_strCaption = strCaption;
	m_strMessage = strMessage;
	m_nButtonType = nButtonType;
}


// OnOK
void CNewAppEasterEgg::OnOK()
{
	if ( m_cCheck.GetCheck() == TRUE)
	{
		CTime time = CTime::GetCurrentTime();
		AfxGetApp()->WriteProfileInt( "Today", "month", time.GetMonth());
		AfxGetApp()->WriteProfileInt( "Today", "day", time.GetDay());
	}

	CDialog::OnOK();
}
