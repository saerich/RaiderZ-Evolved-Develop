#pragma once
#include "afxwin.h"


// CDlgClipPlaneHeight ��ȭ �����Դϴ�.

class CDlgClipPlaneHeight : public CDialog
{
	DECLARE_DYNAMIC(CDlgClipPlaneHeight)

private:
	bool	m_bShow_;
	CControlMediator* m_pControlMediator_;
	bool	m_bStaticMeshRender;

public:
	CDlgClipPlaneHeight(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgClipPlaneHeight();

	void	ShowToggle();
	void	Hide();
	void	SetControl(CControlMediator* pControlMediator_);
	bool	IsShow()	{ return m_bShow_; }

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CLIP_HEIGHT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	void OnViewStaticRenderAll();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit();
	CEdit m_EditMinZ;
	CEdit m_EditMaxZ;
	CSliderCtrl MinZ_Slider;
	CSliderCtrl MaxZ_Slider;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
