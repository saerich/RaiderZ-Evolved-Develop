// CDlgNewMapTypeSelector.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
//#include "CoreToolRS3.h"
#include "VD_DlgNewMapTypeSelector.h"


// CDlgNewMapTypeSelector ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgNewMapTypeSelector, CDialog)

CDlgNewMapTypeSelector::CDlgNewMapTypeSelector(std::vector<std::string>& _rInput, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNewMapTypeSelector::IDD, pParent)
{
	m_vecMapType = _rInput;
}

CDlgNewMapTypeSelector::~CDlgNewMapTypeSelector()
{
}

void CDlgNewMapTypeSelector::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MAP_TYPE, m_ctlComboMapType);
}

BOOL CDlgNewMapTypeSelector::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (m_vecMapType.empty())
	{
		m_ctlComboMapType.AddString("���������� �� ����");
		m_ctlComboMapType.SetCurSel(0);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
	else
	{
		for (std::vector<std::string>::iterator itr = m_vecMapType.begin(); itr != m_vecMapType.end(); ++itr)
		{
			m_ctlComboMapType.AddString(itr->c_str());
		}

		m_ctlComboMapType.SetCurSel(0);
		m_ctlComboMapType.GetLBText(m_ctlComboMapType.GetCurSel(), m_strSelected);
	}
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDlgNewMapTypeSelector, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_MAP_TYPE, &CDlgNewMapTypeSelector::OnCbnSelchangeComboMapType)
END_MESSAGE_MAP()


// CDlgNewMapTypeSelector �޽��� ó�����Դϴ�.

void CDlgNewMapTypeSelector::OnCbnSelchangeComboMapType()
{
	int nCurrSelect = m_ctlComboMapType.GetCurSel();
	if ( nCurrSelect > -1)
		m_ctlComboMapType.GetLBText(m_ctlComboMapType.GetCurSel(), m_strSelected);
}
