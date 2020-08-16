
// MakeCRCDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MakeCRC.h"
#include "MakeCRCDlg.h"
#include "XCheckSum.h"
#include "XCheckSumEncrypt.h"
//#include "vld.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMakeCRCDlg ��ȭ ����




CMakeCRCDlg::CMakeCRCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMakeCRCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMakeCRCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMakeCRCDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_START, &CMakeCRCDlg::OnBnClickedStart)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMakeCRCDlg �޽��� ó����

BOOL CMakeCRCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CString strCmd = AfxGetApp()->m_lpCmdLine;
	if ( strCmd.IsEmpty() == FALSE)
	{
		CButton* pStartBtn = (CButton*)GetDlgItem( IDC_START);
		if ( pStartBtn != NULL)		pStartBtn->EnableWindow( FALSE);

		m_strPath = strCmd;
		m_strPath.Replace( "\"", "");

		SetTimer( 0, 2000, NULL);
	}

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMakeCRCDlg::OnPaint()
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
HCURSOR CMakeCRCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMakeCRCDlg::OnBnClickedStart()
{
	CButton* pStartBtn = (CButton*)GetDlgItem( IDC_START);
	if ( pStartBtn != NULL)		pStartBtn->EnableWindow( FALSE);


	// Read directories
	char szPath[ MAX_PATH];
	GetCurrentDirectory( sizeof( szPath), szPath);

	Run( szPath);


	if ( pStartBtn != NULL)		pStartBtn->EnableWindow( TRUE);
}


void CMakeCRCDlg::OnTimer(UINT_PTR nIDEvent)
{
	if ( nIDEvent == 0)
	{
		KillTimer( 0);

		Run( m_strPath.GetBuffer());

		SetTimer( 1, 2000, NULL);
	}
	else if ( nIDEvent == 1)
	{
		KillTimer( 1);

		SendMessage( WM_CLOSE, 0, 0);
	}

	CDialog::OnTimer(nIDEvent);
}


void CMakeCRCDlg::Run( const char* szPath)
{
	XCheckSum _crc;

	vector< _FILEINFO> vPathList;
	_crc.GetFileInfo( szPath, &vPathList);


	// Output listbox
	CListBox* pListBox = (CListBox*)GetDlgItem( IDC_LIST);
	if ( pListBox != NULL)
	{
		pListBox->ResetContent();


		for ( vector< _FILEINFO>::iterator itr = vPathList.begin();  itr != vPathList.end();  itr++)
		{
			char _buff[ 512];
			sprintf_s( _buff, "%05u,%03u,%010u,%s", (*itr).nCheckSum, (*itr).nCheckBit, (*itr).nFileSize, (*itr).strName.c_str());

			pListBox->AddString( _buff);
		}
	}



	// Output encrypted data
	string strCRC;
	for ( vector< _FILEINFO>::iterator itr = vPathList.begin();  itr != vPathList.end();  itr++)
	{
		char _buff[ 512];
		sprintf_s( _buff, "%05u,%03u,%010u,%s\n", (*itr).nCheckSum, (*itr).nCheckBit, (*itr).nFileSize, (*itr).strName.c_str());
		strCRC += _buff;
	}


	XCheckSumEncrypt Encryptor;
	size_t _size = strCRC.length() + 1;
	char* szEncrypt = new char[ _size];
	Encryptor.Encrypt( strCRC.c_str(), szEncrypt, _size);

	FILE* pFile;
	char szFileName[ MAX_PATH];
	sprintf_s( szFileName, "%s\\file_crc.msf", szPath);
	fopen_s( &pFile, szFileName, "wb");
	if ( pFile != NULL)
	{
		fwrite( szEncrypt, 1, _size, pFile);
		fclose( pFile);
	}

	delete [] szEncrypt;


/*
	{
		FILE* fp;
		fopen_s( &fp, szFileName, "rb");
		if ( fp != NULL)
		{
			fseek( fp, 0, SEEK_END);

			long _filesize = ftell( fp);
			fseek( fp, 0, SEEK_SET);

			char* _buff = new char[ _filesize];
			fread( _buff, 1 , _filesize, fp);

			char* szDecrypt = new char[ _filesize];
			Encryptor.Decrypt( _buff, szDecrypt, _filesize);

			delete [] _buff;
			delete [] szDecrypt;

			fclose( fp);
		}
	}
*/
}
