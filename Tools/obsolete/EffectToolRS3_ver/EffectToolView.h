// EffectToolView.h : iCEffectToolView Ŭ������ �������̽�
//


#pragma once


class CEffectToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CEffectToolView();
	DECLARE_DYNCREATE(CEffectToolView)

// Ư��
public:
	CEffectToolDoc* GetDocument() const;

// �۾�
public:

// ������
	public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ����
public:
	virtual ~CEffectToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // EffectToolView.cpp�� ����� ����
inline CEffectToolDoc* CEffectToolView::GetDocument() const
   { return reinterpret_cast<CEffectToolDoc*>(m_pDocument); }
#endif

