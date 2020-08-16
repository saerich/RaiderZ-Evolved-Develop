
// PackageValidatorDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "PackageValidator.h"
#include "PackageValidatorDlg.h"
#include "MMaietXmlTestReporter.h"
#include "MMaietTestResult.h"
#include <stdio.h>
#include <io.h>
#include <vector>
#include <string>
#include <fstream>
using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPackageValidatorDlg ��ȭ ����
enum
{
	TIMER_START		= 0,
	TIMER_END
};



CPackageValidatorDlg::CPackageValidatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPackageValidatorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPackageValidatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPackageValidatorDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_START, &CPackageValidatorDlg::OnBnClickedStart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CPackageValidatorDlg �޽��� ó����

BOOL CPackageValidatorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_nSuccess = 0;
	m_nFail = 0;

	// Set timer
	SetTimer( TIMER_START, 1000, NULL);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CPackageValidatorDlg::OnPaint()
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
HCURSOR CPackageValidatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPackageValidatorDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch ( nIDEvent)
	{
	case TIMER_START :
		KillTimer( TIMER_START);
		Validate();
		break;

	case TIMER_END :
		KillTimer( TIMER_END);
		exit( (m_nFail > 0)  ?  1 : 0);
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CPackageValidatorDlg::OnBnClickedStart()
{
	Validate();
}


struct PATHINFO
{
	string		strName;
	string		strPath;

	PATHINFO()	{}
	PATHINFO( const char* szName, const char* szPath) : strName( szName), strPath( szPath)		{}
};

void GetDirectories( const char* szPath, vector< PATHINFO>* pPathList)
{
	char _path[ MAX_PATH];
	sprintf_s( _path, "%s\\*.*", szPath);

	long hFile;
	struct _finddata_t file_t;

	if ( (hFile = _findfirst( _path, &file_t)) != -1L ) 
	{
		do
		{
			if ( (file_t.attrib & _A_SUBDIR) == 0 )		continue;
			if ( strcmp( file_t.name, "." ) == 0)		continue;
			if ( strcmp( file_t.name, "..") == 0)		continue;
			if ( strcmp( file_t.name, ".svn") == 0)		continue;


			char _subdir[ MAX_PATH];
			sprintf_s( _subdir, "%s\\%s", szPath, file_t.name);

			pPathList->push_back( PATHINFO( file_t.name, _subdir));

			GetDirectories( _subdir, pPathList);

		}	while( _findnext( hFile, &file_t) == 0);

		_findclose( hFile);
	}
}

void CPackageValidatorDlg::Validate()
{
	// Reset UI
	GetDlgItem( IDC_START)->EnableWindow( FALSE);

	CListBox* pListBox = (CListBox*)GetDlgItem( IDC_INVALIDLIST);
	if ( pListBox == NULL)		return;
	pListBox->ResetContent();


	// Read packaging.bat
	FILE* pFile = NULL;
	fopen_s( &pFile, "pack_common_resource.bat", "r");
	if ( pFile == NULL)		return;

	fseek( pFile, 0, SEEK_END);
	size_t _size = ftell( pFile);
	fseek( pFile, 0, SEEK_SET);
	char* szPackaging = new char[ _size];
	memset( szPackaging, 0, _size);
	fread( (void*)szPackaging, 1, _size, pFile);
	fclose( pFile);

	string strPackaging( szPackaging);
	delete [] szPackaging;



	// Read directories
	char szCurPath[ MAX_PATH];
	GetCurrentDirectory( sizeof( szCurPath), szCurPath);

	char szPath[ MAX_PATH];
	sprintf_s( szPath, "%s\\Data\\Model", szCurPath);

	vector< PATHINFO> vPathList;
	GetDirectories( szPath, &vPathList);


	// Check validate
	std::ofstream strOutLog( "packagevalidator.xml");
	MMaietTestResultList testResulList;
	MMaietXmlTestReporter xmlReporter( strOutLog);

	for ( vector< PATHINFO>::iterator itr = vPathList.begin();  itr != vPathList.end();  itr++)
	{
		if ( strPackaging.find( (*itr).strName, 0) == string::npos)
		{
			// ���� ó��
			if ( (*itr).strName == string( "_tool"))		continue;
			

			// ����Ʈ �ڽ� ���
			pListBox->AddString( (*itr).strPath.c_str());


			// �α� ����
			xmlReporter.AddMessage( (*itr).strPath.c_str());


			m_nFail++;
		}
		else
		{
			m_nSuccess++;
		}
	}

	xmlReporter.ReportSummary( m_nSuccess + m_nFail, m_nFail, m_nFail, 0.0f, testResulList);


	// Restore UI
	GetDlgItem( IDC_START)->EnableWindow( TRUE);


	// Set timer
	SetTimer( TIMER_END, 2000, NULL);
}
