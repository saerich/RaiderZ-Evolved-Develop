#pragma once
#include "afxwin.h"

#include <string>
#include <vector>

// CDlgNewMapTypeSelector ��ȭ �����Դϴ�.

class CDlgNewMapTypeSelector : public CDialog
{
	DECLARE_DYNAMIC(CDlgNewMapTypeSelector)

public:
	CDlgNewMapTypeSelector(std::vector<std::string>& _rInput, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgNewMapTypeSelector();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_NEW_MAP_TYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ctlComboMapType;
	std::vector<std::string> m_vecMapType;
	CString m_strSelected;
	afx_msg void OnCbnSelchangeComboMapType();
};
