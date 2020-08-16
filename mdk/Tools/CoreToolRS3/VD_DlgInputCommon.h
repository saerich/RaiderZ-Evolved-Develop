#pragma once
#include "afxwin.h"

#include "resource.h"

// CDlgInputCommon ��ȭ �����Դϴ�.

class CORETOOL_API CDlgInputCommon : public CDialog
{
	DECLARE_DYNAMIC(CDlgInputCommon)

public:
	CDlgInputCommon(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CDlgInputCommon(const char* _pSzValue, const char* _pSzWindowName, const char* _pSzExtrType);
	virtual ~CDlgInputCommon();

	const char* GetInputData();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_INPUT_COMMON };
	static bool m_isActive;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CString m_cstrComboType_;
	CString m_cstrCommonValue_;
	CString m_cstrComBoValue_;
	CString m_cstrWindowName_;
	CComboBox m_ctrlComboBox_;
	afx_msg void OnCbnSelchangeEditCommonCombo();
};
