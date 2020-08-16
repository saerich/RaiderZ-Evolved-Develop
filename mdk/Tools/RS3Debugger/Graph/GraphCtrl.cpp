#include "stdafx.h"
#include "GraphCtrl.h"
#include "..\resource.h"
#include "..\RCPUAnalyzeDialog.h"

CGraphCtrl::CGraphCtrl( void )
	:m_RectNumLimit(120),m_YScale(700),m_bInit(false),m_LeftSelectedRectangle(NULL),m_RightSelectedRectangle(NULL)
{

}

CGraphCtrl::~CGraphCtrl( void )
{
	m_dcGraph.SelectObject(m_pOldBitmap);
	m_Bitmap.DeleteObject();
	m_dcGraph.DeleteDC();

	ReSizeItem(0);	//�Ѱ��� ������� ��� �����.
	m_ListOfRect.clear();

	if( m_LeftSelectedRectangle )
	{
		m_LeftSelectedRectangle->m_bLeftSelected = false;
		DeleteItem(m_LeftSelectedRectangle);
	}
	if( m_RightSelectedRectangle )
	{
		m_RightSelectedRectangle->m_bRightSelected = false;
		DeleteItem(m_RightSelectedRectangle);
	}
}

BEGIN_MESSAGE_MAP(CGraphCtrl, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

void CGraphCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CStatic::OnPaint()��(��) ȣ������ ���ʽÿ�.

	dc.BitBlt(0, 0, m_Rect.Width(),m_Rect.Height(), &m_dcGraph, 0, 0, SRCCOPY);
}

void CGraphCtrl::PreSubclassWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	GetClientRect(&m_Rect);

	int Height = m_Rect.Height();
	int Width = m_Rect.Width();

	CClientDC dc(this);
	m_dcGraph.CreateCompatibleDC(&dc);
	m_Bitmap.CreateCompatibleBitmap(&dc, Width, Height);
	m_pOldBitmap = m_dcGraph.SelectObject(&m_Bitmap);

	CStatic::PreSubclassWindow();
}


void CGraphCtrl::DisplayToDC( void )
{
	int			RectangleWidth = GetRectangleWidth();
	int			X,Y;
	int			Count = 0;

	CPen Pen;
	Pen.CreatePen(PS_SOLID,1,RGB(0,255,255));
	CBrush	Brush;
	Brush.CreateSolidBrush(RGB(255,255,255));
	CBrush LeftSelectedBrush;
	LeftSelectedBrush.CreateSolidBrush(RGB(255,0,0));
	CBrush RightSelectedBrush;
	RightSelectedBrush.CreateSolidBrush(RGB(0,0,255));


	CPen* pOldPen = m_dcGraph.SelectObject(&Pen);
	CBrush* pOldBrush = m_dcGraph.SelectObject(&Brush);

	m_dcGraph.FillSolidRect(&m_Rect, RGB(255,255,255));

	std::list<CRectangle*>::iterator iter;
	for( iter = m_ListOfRect.begin() ; iter != m_ListOfRect.end() ; iter++ )
	{
		X = RectangleWidth * Count;
		Y = m_Rect.Height();

		CRectangle* pRectangle = (*iter);

		if( true == pRectangle->m_bLeftSelected )
		{
			m_dcGraph.SelectObject(&LeftSelectedBrush);
			m_dcGraph.Rectangle(X,Y- (int)(pRectangle->m_Height * m_YScale),X+RectangleWidth-1, Y);
			m_dcGraph.SelectObject(&Brush);
		}
		else if( true == pRectangle->m_bRightSelected )
		{
			m_dcGraph.SelectObject(&RightSelectedBrush);
			m_dcGraph.Rectangle(X,Y- (int)(pRectangle->m_Height * m_YScale),X+RectangleWidth-1, Y);
			m_dcGraph.SelectObject(&Brush);
		}
		else
		{
			m_dcGraph.Rectangle(X,Y- (int)(pRectangle->m_Height * m_YScale),X+RectangleWidth-1, Y);
		}
		Count++;
	}
	RedrawWindow();

	m_dcGraph.SelectObject(pOldPen);
	m_dcGraph.SelectObject(pOldBrush);

}

int CGraphCtrl::GetRectangleWidth(void)
{
	int RectangleWidth = m_Rect.Width() / m_RectNumLimit;
	if( RectangleWidth <= 0 ) RectangleWidth = 1;
	return RectangleWidth;
}


void CGraphCtrl::AddItem( CProfileManager* pProfileManager )
{
	CRectangle* pRectangle = new CRectangle(pProfileManager->Get_Time_Since_ResetCurrentInfoCall(),pProfileManager);
	pRectangle->m_bInGraphCtrl = true;

	m_ListOfRect.push_back(pRectangle);	

	ReSizeItem(m_RectNumLimit);	// m_RectNumLimit�� ȭ�鿡 ǥ�õǴ� ������ �̸�ŭ�� ���ܵΰ� �������� �����.
	DisplayToDC();
}

bool CGraphCtrl::DeleteItem( CRectangle* pRectangle )
{
	if(  pRectangle						&&
		!pRectangle->m_bLeftSelected	&&
		!pRectangle->m_bRightSelected	&&
		!pRectangle->m_bInGraphCtrl )
	{
		delete pRectangle->m_pProfileManager;
		delete pRectangle;
		return true;
	}
	return false;
}

void CGraphCtrl::ReSizeItem(int ReservedSize)
{
	int ShouldBeRemoved = m_ListOfRect.size() - ReservedSize;
	while(ShouldBeRemoved > 0)
	{
		std::list<CRectangle*>::iterator iter = m_ListOfRect.begin();
		CRectangle* pRectangle = (*iter);
		pRectangle->m_bInGraphCtrl = false;

		DeleteItem(pRectangle);

		m_ListOfRect.pop_front();
		ShouldBeRemoved--;
	}
}

void CGraphCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	int			RectangleWidth = GetRectangleWidth();
	int			X,Y;
	int			Count = 0;

	std::list<CRectangle*>::iterator iter;
	for( iter = m_ListOfRect.begin() ; iter != m_ListOfRect.end() ; iter++ )
	{
		X = RectangleWidth * Count;
		Y = m_Rect.Height();

		CRectangle* pRectangle = (*iter);
		pRectangle->m_bLeftSelected = false;

		CRect Rect(X,0,X+RectangleWidth-1, Y);
		if( TRUE == Rect.PtInRect(point) )
		{
			if( NULL != m_pProfileDialog ) 
			{
				if( m_LeftSelectedRectangle )
					m_LeftSelectedRectangle->m_bLeftSelected = false;
				pRectangle->m_bLeftSelected = true;
				if( DeleteItem(m_LeftSelectedRectangle) )
					m_LeftSelectedRectangle = NULL;
				m_pProfileDialog->SetLeftProfileManager(pRectangle->m_pProfileManager);
				m_LeftSelectedRectangle = pRectangle;
			}
		}
		Count++;
	}

	DisplayToDC();

	CStatic::OnLButtonDown(nFlags, point);
}

void CGraphCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	int			RectangleWidth = GetRectangleWidth();
	int			X,Y;
	int			Count = 0;

	std::list<CRectangle*>::iterator iter;
	for( iter = m_ListOfRect.begin() ; iter != m_ListOfRect.end() ; iter++ )
	{
		X = RectangleWidth * Count;
		Y = m_Rect.Height();

		CRectangle* pRectangle = (*iter);
		pRectangle->m_bRightSelected = false;

		CRect Rect(X,0,X+RectangleWidth-1, Y);
		if( TRUE == Rect.PtInRect(point) )
		{
			if( NULL != m_pProfileDialog ) 
			{
				if( m_RightSelectedRectangle )
					m_RightSelectedRectangle->m_bRightSelected = false;
				pRectangle->m_bRightSelected = true;
				if( DeleteItem(m_RightSelectedRectangle) )
					m_RightSelectedRectangle = NULL;
				m_pProfileDialog->SetRightProfileManager(pRectangle->m_pProfileManager);
				m_RightSelectedRectangle = pRectangle;
			}
		}
		Count++;
	}

	DisplayToDC();

	CStatic::OnRButtonDown(nFlags, point);
}
