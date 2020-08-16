#pragma once

#include "resource.h"

// CAniControlBar ��ȭ �����Դϴ�.

class CFrameControlBar : public CDialogBar
{
	DECLARE_DYNAMIC(CFrameControlBar)

public:
	//{{AFX_DATA(CFrameControlBar)        
	enum { IDD = IDD_DIALOG_FRAMECONTROL };

	CSliderCtrl m_SliderAnimation;
	CString		m_strFrame;
	CString		m_strSecond;
	CSliderCtrl m_SliderAniSpeed;
	//}}AFX_DATA 


public:
	CFrameControlBar(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CFrameControlBar();

protected:
	void _resize( int nID, int delta, bool right_align);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	//{{AFX_MSG(CFrameControlBar)
	afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	// No message handlers
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
