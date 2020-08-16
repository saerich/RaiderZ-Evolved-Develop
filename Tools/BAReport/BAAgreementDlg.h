#if !defined(AFX_BAAGREEMENTDLG_H__41013A16_A16F_45F3_896B_BE59EEA20CFD__INCLUDED_)
#define AFX_BAAGREEMENTDLG_H__41013A16_A16F_45F3_896B_BE59EEA20CFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BAAgreementDlg.h : header file
//
#include <string>

/////////////////////////////////////////////////////////////////////////////
// BAAgreementDlg dialog

class BAAgreementDlg : public CDialog
{
// Construction
public:
	BAAgreementDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(BAAgreementDlg)
	enum { IDD = IDD_DLG_AGREEMENT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(BAAgreementDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(BAAgreementDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAgree();
	afx_msg void OnDisagree();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CRichEditCtrl m_Detail;

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	std::string	ConvUTF16ToUTF8(const wchar_t* pszString);	// UTF8�� �������ؼ� MLocale���� �����ؿԽ��ϴ�.
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BAAGREEMENTDLG_H__41013A16_A16F_45F3_896B_BE59EEA20CFD__INCLUDED_)
