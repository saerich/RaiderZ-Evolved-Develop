// SDIdocView.h : CCoreToolRS3View Ŭ������ �������̽�
//


#pragma once

class CControlMediator;
class CCoreToolRS3Doc;
class CCoreToolRS3View : public CView
{
protected: // serialization������ ��������ϴ�.
	CCoreToolRS3View();
	DECLARE_DYNCREATE(CCoreToolRS3View)

// Ư���Դϴ�.
public:
	CCoreToolRS3Doc* GetDocument() const;

// �����Դϴ�.
public:
	virtual ~CCoreToolRS3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct); ///< ����
	afx_msg void OnMouseMove(UINT nFlags, CPoint point); ///< ���콺 ó��
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt); ///< ���콺 ó��
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); ///< ���콺 ó��
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);   ///< ���콺 ó��
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point); ///< ���콺 ó��
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);   ///< ���콺 ó��
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point); ///< ���콺 ó��
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);   ///< ���콺 ó��
	afx_msg void OnSize(UINT nType, int cx, int cy); ///< �������� ó��
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDropFiles(HDROP hDropInfo);

	DECLARE_MESSAGE_MAP()

public:
	void SetControlMediator(CControlMediator* _pControlMediator){m_pControlMediator_ = _pControlMediator;}
	void SetUserTimer(DWORD dwElpasedTime);
	void KillUserTimer();
	void OnUserTimer();	///<���� ������

private:
	struct MOUSE_ACT_EVENT {
		static const UINT INIT =	0x00000;
		static const UINT MOVE =	0x10000;
		static const UINT M_WHEEL = 0x20000;

		static const UINT L_BN =	0x01000;
		static const UINT R_BN =	0x02000;
		static const UINT M_BN =	0x04000;

		static const UINT BN_DOWN =	0x00001;
		static const UINT BN_UP	=	0x00002;
	};

	CControlMediator* m_pControlMediator_;
	struct MOUSE_BUFFER {
		MOUSE_BUFFER() : nEvent(MOUSE_ACT_EVENT::INIT){}
		UINT nFlags; ///< MFC ���콺 �÷���
		UINT nEvent; ///< ���콺 �׼� ����
		LONG x;      ///< ���콺 x��ǥ
		LONG y;      ///< ���콺 y��ǥ
		LONG zDelta; ///< ���콺 ��
	} m_eventedMouseBuffer_, m_previousMouseBuffer_;
	
	// Ÿ�̸� �÷���
	bool m_bTimer;
	DWORD m_dwTimerElpasedTime;
	
	//��ũ��ġ�� ũ��
	UINT m_wbWidth_;
	UINT m_wbHeight_;

	//���콺 ���� ���� �Լ�
	void WB_DecideMouseState_();
};

#ifndef _DEBUG  // SDIdocView.cpp�� ����� ����
inline CCoreToolRS3Doc* CCoreToolRS3View::GetDocument() const
   { return reinterpret_cast<CCoreToolRS3Doc*>(m_pDocument); }
#endif
