// BAAgreementDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BAReport.h"
#include "BAAgreementDlg.h"
#include "BAReportDlg.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// BAAgreementDlg dialog


BAAgreementDlg::BAAgreementDlg(CWnd* pParent /*=NULL*/)
	: CDialog(BAAgreementDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(BAAgreementDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void BAAgreementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RICHEDIT_DETAIL, m_Detail);
	//{{AFX_DATA_MAP(BAAgreementDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(BAAgreementDlg, CDialog)
	//{{AFX_MSG_MAP(BAAgreementDlg)
	ON_BN_CLICKED(ID_AGREE, OnAgree)
	ON_BN_CLICKED(ID_DISAGREE, OnDisagree)
	//}}AFX_MSG_MAP
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// BAAgreementDlg message handlers

BOOL BAAgreementDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_AGREEMENT);
	if (!g_bAgree)
		pEdit->SetWindowText(L"���α׷��� ġ������ ������ �߻��Ͽ����Ƿ� ���� ������ ���߻翡 �����ϵ��� �����մϴ�.\r\n\r\n�� ���α׷��� ���α׷� ������ ������ �м��� ���Ͽ� ���α׷� �������� �� ������ �ý��ۻ��(�ü��/�޸�/����ī�� ����)�� ���翡 �����Ұ��Դϴ�.\r\n\r\n���۵� �ڷ�� ���α׷� ���� ����� ���ؼ��� ���˴ϴ�.\r\n������ �����ðڽ��ϱ�?");

	//m_Detail.Create()
	m_Detail.SetWindowText(L"");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void BAAgreementDlg::OnCancel()
{
}

void BAAgreementDlg::OnAgree() 
{
	CString strDetail;
	m_Detail.GetWindowText(strDetail);

	std::wstring strDetailString = (LPCTSTR)strDetail;

	if (strDetailString.length() > 1)
	{
		std::wstring strFileName = g_pFileTransfer->GetTextFileNameInQueue();
		if (strFileName.length() > 1)
		{
			FILE* fp;
			_wfopen_s(&fp, strFileName.c_str(), L"a");

			if (fp)
			{
				fprintf(fp, "\n--------------------------------------\n");
				fprintf(fp, "%s", ConvUTF16ToUTF8(L"ũ���� ����\n").c_str());
				fprintf(fp, "--------------------------------------\n");
				fprintf(fp, "%s", ConvUTF16ToUTF8(strDetailString.c_str()).c_str());
				fprintf(fp, "\n");
				fclose(fp);
			}
		}
	}

	CBAReportDlg* pDlg = (CBAReportDlg*)AfxGetMainWnd();
	pDlg->Upload();
	DestroyWindow();
}

void BAAgreementDlg::OnDisagree() 
{
	CBAReportDlg* pDlg = (CBAReportDlg*)AfxGetMainWnd();
	pDlg->DestroyWindow();
}

BOOL BAAgreementDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (LOWORD(wParam) == ID_AGREE)
		TRACE("SHIT!");
	return CDialog::OnCommand(wParam, lParam);
}

void BAAgreementDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

}

std::string	BAAgreementDlg::ConvUTF16ToUTF8(const wchar_t* pszString)
{
	// �ʿ��� ������ ���۸� �غ��Ѵ�.
	int nReqLen = WideCharToMultiByte(CP_UTF8, 0, pszString, wcslen(pszString) + 1, 0, 0, 0, 0);

	std::auto_ptr<char> szDest(new char[nReqLen]);


	// ���ۿ� �����ڵ�� ��ȯ�� ���ڿ��� �ִ´�.
	int nLen = WideCharToMultiByte(CP_UTF8, 0, pszString, wcslen(pszString) + 1, szDest.get(), nReqLen, 0, 0);


	// �����ϸ� ���ڿ�, �����ϸ� ��ȯ�� ���ڿ��� ��ȯ�Ѵ�.
	if (nLen <= 0)
	{
		return std::string();
	}
	else
	{
		return szDest.get();
	}	
}
