#pragma once

#include "VD_OptionPage1.h"
#include "VD_OptionPage2.h"

// CDlgOption ��ȭ �����Դϴ�.

class CDlgOption : public CPropertySheet
{
	DECLARE_DYNAMIC(CDlgOption)

public:
	//CDlgOption(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CDlgOption(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDlgOption(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	virtual ~CDlgOption();

	virtual BOOL OnInitDialog();
	void InitFromConfig();

	static bool m_isActive;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_OPTION };

protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	V_OptionPage1*		m_pPage1;
	V_OptionPage2*		m_pPage2;
};
