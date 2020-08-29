#pragma once

#include "afxspinbuttonctrl.h"

class CReflectiveFloatProperty;

class CReflectiveSpinCtrl : public CMFCSpinButtonCtrl
{
public:
	CReflectiveSpinCtrl(CReflectiveFloatProperty* pProperty);
	virtual ~CReflectiveSpinCtrl();

	void	SetCaptureTimer();
	void	ReleaseCaptureTimer();

protected:
	CReflectiveFloatProperty* m_pProperty;

	// ������Ʈ���� ���÷� �ٽ� ��������� static ���� ����
	static float	m_fOriginalValue;
	static CPoint	m_ptBeginDrag;
	static BOOL		m_bDrag;
	float			m_fLastValue;

	//{{AFX_MSG(ReflectiveSpinCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDraw(CDC* pDC);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};