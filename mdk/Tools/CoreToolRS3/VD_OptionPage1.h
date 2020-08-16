#pragma once
#include "VL_ListenableView.h"
#include "afxwin.h"

// V_OptionPage1 ��ȭ �����Դϴ�.

class V_OptionPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(V_OptionPage1)

public:
	V_OptionPage1();
	virtual ~V_OptionPage1();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_OPTION_PAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	CButton m_ctlSaveToConfigXml;
	afx_msg void OnBnClickedCheckSaveConfigXml();
	afx_msg void OnBnClickedButtonTextEditor();

	DECLARE_MESSAGE_MAP()
public:
	int m_nShadowMapSize;
	float m_fMaxFarZ;
	float m_fMinFarZ;
	float m_fWheelZoomFactor;
	float m_fKeyZoomFactor;
	float m_fRotateStep;
	float m_fPanStep;
	bool m_bConfigXmlSave;

	BOOL m_nRememberWork;

	CString m_strTextEditor;
	CString m_strTextLoginID;
	CString m_strTextLoginIP;
};
