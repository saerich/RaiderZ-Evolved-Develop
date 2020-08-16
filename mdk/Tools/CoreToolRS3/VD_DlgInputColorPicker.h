#pragma once

#include "ext_lib/CAutoEdit/AutoEdit.h"
#include "ext_lib/CAutoEdit/AutoEditValidator.h"


// CDlgInputColorPicker ��ȭ �����Դϴ�.

class CORETOOL_API CDlgInputColorPicker : public CDialog
{
	DECLARE_DYNAMIC(CDlgInputColorPicker)

public:
	CDlgInputColorPicker(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CDlgInputColorPicker(DWORD _color, BOOL _bAutoColor);
	virtual ~CDlgInputColorPicker();

	COLORREF GetColorValue(){ return m_crColor_; }
	int m_nAutoColor;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_INPUT_COLOR_PICKER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	afx_msg void OnStnClickedColorPickColor();
	afx_msg void OnBnClickedCheckAutoColor();

	DECLARE_MESSAGE_MAP()

private:

	void UpdateColorEditBox_();
	void EnableManualColor_();

	CString m_cstrColorValue_;
	COLORREF m_crColor_;

	CAutoEdit m_ctlAutoEditR_;
	CAutoEdit m_ctlAutoEditG_;
	CAutoEdit m_ctlAutoEditB_;

	CColorPickerValidate m_validatorColorPicker_;
};
