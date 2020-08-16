// VD_DlgTestFilePath.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "VD_DlgInputMapName.h"


// CDlgInputMapName 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgInputMapName, CDialog)

CDlgInputMapName::CDlgInputMapName(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputMapName::IDD, pParent)
{

}

CDlgInputMapName::~CDlgInputMapName()
{
}

BOOL CDlgInputMapName::OnInitDialog(){
	CDialog::OnInitDialog();
	SetWindowText(m_strDlgCaption.c_str());
	return TRUE;
}

void CDlgInputMapName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInputMapName, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TEST_DIRECTORY, &CDlgInputMapName::OnBnClickedButtonTestDirectory)
	ON_BN_CLICKED(IDOK, &CDlgInputMapName::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgInputMapName 메시지 처리기입니다.

void CDlgInputMapName::OnBnClickedButtonTestDirectory()
{
	ITEMIDLIST *pidlBrowse;
	char pszPathname[1024];
	BROWSEINFO BrInfo;
	//BrInfo.hwndOwner = GetSafeHwnd(); 
	BrInfo.hwndOwner = 0;
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "테스트 디렉토리 설정";
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if (NULL != pidlBrowse)
	{
	  ::SHGetPathFromIDList(pidlBrowse, pszPathname);
	  GetDlgItem(IDC_EDIT_DIRECTORY_PATH)->SetWindowText(pszPathname);
	}
}

void CDlgInputMapName::OnBnClickedOk()
{
	GetDlgItemText(IDC_EDIT_DIRECTORY_PATH, m_cstrTestDirectory);
	GetDlgItemText(IDC_EDIT_FILE_PATH, m_cstrTestFile);
	if(m_cstrTestDirectory.GetLength() == 0)
	{
		AfxMessageBox("폴더 설정이 비어있습니다.");
		return;
	}
	if(GetFileAttributes(m_cstrTestDirectory) == INVALID_FILE_ATTRIBUTES)
	{
		AfxMessageBox("올바른 폴더가 아닙니다.");
		return;
	}
	if(m_cstrTestFile.GetLength() == 0)
	{
		AfxMessageBox("파일설정이 비어있습니다.");
		return;
	}
	if(m_cstrTestFile.Right(4).CompareNoCase(".xml") == 0 )
	{
		AfxMessageBox("확장자를 붙일 필요가 없습니다.");
		return;
	}
	m_cstrTestFile = m_cstrTestFile + ".zone.xml";
	OnOK();
}
