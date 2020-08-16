
// SpaceViewerView.h : CSpaceViewerView Ŭ������ �������̽�
//


#pragma once


class CSpaceViewerView : public CView
{
protected: // serialization������ ��������ϴ�.
	CSpaceViewerView();
	DECLARE_DYNCREATE(CSpaceViewerView)

// Ư���Դϴ�.
public:
	CSpaceViewerDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// �����Դϴ�.
public:
	virtual ~CSpaceViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // SpaceViewerView.cpp�� ����� ����
inline CSpaceViewerDoc* CSpaceViewerView::GetDocument() const
   { return reinterpret_cast<CSpaceViewerDoc*>(m_pDocument); }
#endif

