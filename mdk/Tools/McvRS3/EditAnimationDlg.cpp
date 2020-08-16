// EditAnimationDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Mcv.h"
#include "EditAnimationDlg.h"


// EditAnimationDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(EditAnimationDlg, CDialog)
EditAnimationDlg::EditAnimationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(EditAnimationDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strFileName(_T(""))
	, m_bLoop(FALSE)
{
}

EditAnimationDlg::~EditAnimationDlg()
{
}

void EditAnimationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ANIMATION_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_ANIMATION_FILENAME, m_strFileName);
	DDX_Check(pDX, IDC_CHECK_LOOP, m_bLoop);
}


BEGIN_MESSAGE_MAP(EditAnimationDlg, CDialog)
END_MESSAGE_MAP()


// EditAnimationDlg �޽��� ó�����Դϴ�.
