// V_OptionPage1.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "VD_OptionPage1.h"

#include "VD_DlgCommonFileOpen.h"


// V_OptionPage1 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(V_OptionPage1, CPropertyPage)

V_OptionPage1::V_OptionPage1()
	: CPropertyPage(V_OptionPage1::IDD)
	, m_nShadowMapSize(2048)
	, m_fMaxFarZ(0)
	, m_fMinFarZ(0)
	, m_fWheelZoomFactor(0)
	, m_fKeyZoomFactor(0)
	, m_fRotateStep(0)
	, m_fPanStep(0)
{
	m_bConfigXmlSave = false;
}

V_OptionPage1::~V_OptionPage1()
{
}

BOOL V_OptionPage1::OnInitDialog()
{
	return CPropertyPage::OnInitDialog();
}

void V_OptionPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SHADOWSIZE, m_nShadowMapSize);
	DDV_MinMaxInt(pDX, m_nShadowMapSize, 0, 8192);
	DDX_Text(pDX, IDC_EDIT2, m_fMaxFarZ);
	DDX_Text(pDX, IDC_EDIT4, m_fMinFarZ);
	DDX_Text(pDX, IDC_EDIT5, m_fWheelZoomFactor);
	DDX_Text(pDX, IDC_EDIT3, m_fKeyZoomFactor);
	DDX_Text(pDX, IDC_EDIT7, m_fRotateStep);
	DDX_Text(pDX, IDC_EDIT6, m_fPanStep);
	DDX_Control(pDX, IDC_CHECK_SAVE_CONFIG_XML, m_ctlSaveToConfigXml);

	DDX_Text(pDX, IDC_EDIT_TEXT_EDITOR, m_strTextEditor);
	DDX_Text(pDX, IDC_EDIT_OPTION_LOGIN_ID, m_strTextLoginID);
	DDX_Text(pDX, IDC_EDIT_OPTION_LOGIN_IP, m_strTextLoginIP);

	DDX_Check(pDX, IDC_CHECK_SAVE_LAST_WORK, m_nRememberWork);
}


BEGIN_MESSAGE_MAP(V_OptionPage1, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_SAVE_CONFIG_XML, &V_OptionPage1::OnBnClickedCheckSaveConfigXml)
	ON_BN_CLICKED(IDC_BUTTON_TEXT_EDITOR, &V_OptionPage1::OnBnClickedButtonTextEditor)
END_MESSAGE_MAP()


// V_OptionPage1 �޽��� ó�����Դϴ�.

void V_OptionPage1::OnBnClickedCheckSaveConfigXml()
{
	if(m_ctlSaveToConfigXml.GetCheck())
		m_bConfigXmlSave = true;
	else
		m_bConfigXmlSave = false;
}

void V_OptionPage1::OnBnClickedButtonTextEditor()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CListViewOpenDlg dlg("C:\\");

	dlg.SetTitle("�ؽ�Ʈ ������ ����");
	dlg.SetFilter("exe ����\0*.exe;");
	if(dlg.DoModal() == IDOK)
	{
		CString fileName(dlg.GetPathName());
		if(fileName.IsEmpty())
			return;

		m_strTextEditor = fileName.GetString();
		UpdateData(FALSE);
	}
}
