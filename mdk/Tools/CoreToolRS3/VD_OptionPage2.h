#pragma once
#include "VL_ListenableView.h"

// V_OptionPage2 ��ȭ �����Դϴ�.

class V_OptionPage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(V_OptionPage2)

public:
	V_OptionPage2();
	virtual ~V_OptionPage2();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_OPTION_PAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	afx_msg void OnBnClickedButtonRootpath();
	afx_msg void OnBnClickedButtonFieldPath();

	DECLARE_MESSAGE_MAP()
public:
	CString m_strDataPath;
	CString m_strTilePath;
	CString m_strTreePath;
	CString m_strEluPath;
	CString m_strEftPath;
	CString m_strShaderPath;
	CString m_strTexturePath;

	CString m_strServerPath;
};
