#pragma once

#include "AnimationSidebar.h"
#include "ModelInfoSidebar.h"
#include "ViewControlSidebar.h"
#include "LightSidebar.h"

// CControlToolBar ��ȭ �����Դϴ�.

class CControlToolBar : public CDialogBar
{
	DECLARE_DYNAMIC(CControlToolBar)

public:
	//{{AFX_DATA(CControlToolBar)        
	enum { IDD = IDD_DIALOG_CONTROL };

	CAnimationSidebar		m_cAnimationSidebar;
	CModelInfoSidebar		m_cModelInfoSidebar;
	CViewControlSidebar		m_cViewControlSidebar;
	CLightSidebar			m_cLightSidebar;
	//}}AFX_DATA 


public:
	CControlToolBar(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CControlToolBar();

	CAnimationSidebar* GetAnimationSidebar() { return &m_cAnimationSidebar; }
	CModelInfoSidebar* GetModelInfoSidebar() { return &m_cModelInfoSidebar; }
	CViewControlSidebar* GetViewControlSidebar() { return &m_cViewControlSidebar; }
	CLightSidebar* GetLightSidebar() { return &m_cLightSidebar; }

	void SelectNodeByID(int nID);	// tree control ���� node �� �����Ѵ�

protected:
	LRESULT InitDialog(WPARAM wParam, LPARAM lParam);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	void _resize( int nID, int delta, bool top_anchor);

	//{{AFX_MSG(CControlToolBar)
	afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);
	afx_msg void OnTcnSelchangeTabMenu(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()
};
