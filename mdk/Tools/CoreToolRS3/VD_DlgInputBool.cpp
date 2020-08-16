// VD_DlgInputBool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
//#include "CoreToolRS3.h"
#include "VD_DlgInputBool.h"


// CDlgInputBool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgInputBool, CDialog)

CDlgInputBool::CDlgInputBool(const char* _pSzWindowName, int _nTrueChecked, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputBool::IDD, pParent), m_cstrWindowName_(_pSzWindowName)
{
	m_nBoolValue = _nTrueChecked;
}

CDlgInputBool::~CDlgInputBool()
{
}

BOOL CDlgInputBool::OnInitDialog()
{
	CDialog::OnInitDialog();
	if( !m_cstrWindowName_.IsEmpty() )
		SetWindowText(m_cstrWindowName_);

	return TRUE;
}

void CDlgInputBool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_FALSE, m_nBoolValue);
}


BEGIN_MESSAGE_MAP(CDlgInputBool, CDialog)
END_MESSAGE_MAP()

// CDlgInputBool �޽��� ó�����Դϴ�.
