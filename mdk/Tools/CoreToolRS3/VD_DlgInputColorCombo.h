#pragma once
#include "ext_lib/ColourPickerCB.h"

// CDlgInputColorCombo ��ȭ �����Դϴ�.

class CORETOOL_API CDlgInputColorCombo : public CDialog
{
	DECLARE_DYNAMIC(CDlgInputColorCombo)

public:
	CDlgInputColorCombo(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CDlgInputColorCombo(CString& _refColorName);
	virtual ~CDlgInputColorCombo();

	const char* GetColorName();
	COLORREF GetColorValue(){ return m_crColor_; }

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_INPUT_COLOR_COMBOBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CColourPickerCB	m_ctlColorCombo_;
	COLORREF m_crColor_;
	CString m_cstrColorName_;
};
