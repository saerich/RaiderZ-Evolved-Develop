
// SpaceViewerView.cpp : CSpaceViewerView Ŭ������ ����
//

#include "stdafx.h"
#include "SpaceViewer.h"

#include "SpaceViewerDoc.h"
#include "SpaceViewerView.h"
#include "Frame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSpaceViewerView

IMPLEMENT_DYNCREATE(CSpaceViewerView, CView)

BEGIN_MESSAGE_MAP(CSpaceViewerView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CSpaceViewerView ����/�Ҹ�

CSpaceViewerView::CSpaceViewerView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CSpaceViewerView::~CSpaceViewerView()
{
}

BOOL CSpaceViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CSpaceViewerView �׸���

void CSpaceViewerView::OnDraw(CDC* /*pDC*/)
{
	CSpaceViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	CFrame& frame = CFrame::GetReference();
	frame.Render();
}


// CSpaceViewerView ����

#ifdef _DEBUG
void CSpaceViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CSpaceViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSpaceViewerDoc* CSpaceViewerView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSpaceViewerDoc)));
	return (CSpaceViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CSpaceViewerView �޽��� ó����

int CSpaceViewerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	CDC *pDC = GetDC();

	CFrame& frame = CFrame::GetReference();
	frame.Init(pDC->m_hDC);

	return 0;
}

void CSpaceViewerView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CFrame& frame = CFrame::GetReference();
	frame.Release();
}

void CSpaceViewerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CFrame& frame = CFrame::GetReference();
	frame.OnSize(cx,cy);
}
