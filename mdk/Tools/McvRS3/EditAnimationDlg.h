#pragma once
#include "afxwin.h"


// EditAnimationDlg ��ȭ �����Դϴ�.

class EditAnimationDlg : public CDialog
{
	DECLARE_DYNAMIC(EditAnimationDlg)

public:
	EditAnimationDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~EditAnimationDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_EDIT_ANIMATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	CString m_strFileName;
	CComboBox m_cbLoopType;
	BOOL m_bLoop;
};
