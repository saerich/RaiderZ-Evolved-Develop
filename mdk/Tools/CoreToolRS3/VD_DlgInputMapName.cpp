// VD_DlgTestFilePath.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "VD_DlgInputMapName.h"


// CDlgInputMapName ��ȭ �����Դϴ�.

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


// CDlgInputMapName �޽��� ó�����Դϴ�.

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
	BrInfo.lpszTitle = "�׽�Ʈ ���丮 ����";
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
		AfxMessageBox("���� ������ ����ֽ��ϴ�.");
		return;
	}
	if(GetFileAttributes(m_cstrTestDirectory) == INVALID_FILE_ATTRIBUTES)
	{
		AfxMessageBox("�ùٸ� ������ �ƴմϴ�.");
		return;
	}
	if(m_cstrTestFile.GetLength() == 0)
	{
		AfxMessageBox("���ϼ����� ����ֽ��ϴ�.");
		return;
	}
	if(m_cstrTestFile.Right(4).CompareNoCase(".xml") == 0 )
	{
		AfxMessageBox("Ȯ���ڸ� ���� �ʿ䰡 �����ϴ�.");
		return;
	}
	m_cstrTestFile = m_cstrTestFile + ".zone.xml";
	OnOK();
}
