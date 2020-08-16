#pragma once

#include "resource.h"

// CViewControlSidebar ��ȭ �����Դϴ�.

class CViewControlSidebar : public CDialog
{
	DECLARE_DYNAMIC(CViewControlSidebar)

public:
	//{{AFX_DATA(CViewControlSidebar)        
	enum { IDD = IDD_DIALOG_VIEWCONTROL };

	CSliderCtrl m_SliderVisibility;
	CSliderCtrl m_SliderBoneLOD;
	//}}AFX_DATA 


public:
	CViewControlSidebar(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CViewControlSidebar();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	//{{AFX_MSG(CViewControlSidebar)
	afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnVisibilityReset();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
