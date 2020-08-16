/************************************************************************
MBTextAreaLook

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : TextArea ��Ʈ���� Ŀ���ҷ�(Custom look) Ŭ���� �Դϴ�.
*************************************************************************/


#include "stdafx.h"
#include "MBTextAreaLook.h"


// namespace : mint3
namespace mint3
{

/*	MBTextAreaLook
	������
*/
MBTextAreaLook::MBTextAreaLook()
{
	// Ŀ���� Look ��� ����
	m_bCustomLook = false;


	// ��Ʈ���� �÷��� ����
	m_BitmapStyle = DRAWBITMAP_STRETCH;


	// ��Ʈ�� �̹����� Ÿ�������� ���� ����
	m_bTiled = false;


	// ��Ʈ �ʱ�ȭ
	m_pFont = NULL;
}


/*	~MBTextAreaLook
	�Ҹ���
*/
MBTextAreaLook::~MBTextAreaLook()
{
}


/*	SetBitmap
	�����ӿ� ���� �̹����� �����ϴ� �Լ�
*/
void MBTextAreaLook::SetBitmap( const string strFileName)
{
	m_Bitmaps[ 0].strBitmapName = strFileName;
	m_Bitmaps[ 1].strBitmapName = strFileName;
	m_Bitmaps[ 2].strBitmapName = strFileName;
}


/*	SetAssignRect
	�����ӿ� ���� ��Ʈ���� ������ �����ϴ� �Լ�
*/
void MBTextAreaLook::SetAssignRect( int state, int index, float x1, float y1, float x2, float y2)
{
	// Check valid range
	if ( index >= 9)
		return;


	// Set bitmap assignment
	m_Bitmaps[ state].m_rCoord[ index].x = (int)x1;
	m_Bitmaps[ state].m_rCoord[ index].y = (int)y1;
	m_Bitmaps[ state].m_rCoord[ index].w = (int)x2;
	m_Bitmaps[ state].m_rCoord[ index].h = (int)y2;
}


/*	OnFrameDraw
	�������� �׸��� �Լ�
*/
void MBTextAreaLook::OnFrameDraw( MTextArea* pTextArea, MDrawContext* pDC)
{
	// ���� Ŭ���̾�Ʈ�� ������ ���մϴ�
	MRECT rect = pTextArea->GetRect();
	rect.x = rect.y = 0;


	// ����Ʈ�ڽ��� ���¸� ���մϴ�
	int state;
	if ( !pTextArea->IsEnable())		state = 2;
	else if ( pTextArea->IsFocus())		state = 1;
	else								state = 0;


	// �������� �׸��ϴ�
	DrawBitmapByStyle( pDC, rect, MBitmapManager::Get( m_Bitmaps[ state].strBitmapName.c_str()),
		m_Bitmaps[ state].m_rCoord, m_BitmapStyle, m_bTiled);
}


/*	SetStretchBlt
	��Ʈ���� �÷� ���� �� �ְ� �����ϴ� �Լ�
*/
void MBTextAreaLook::SetStretchBlt( bool bStretch /*=true*/)
{
	// Set stretch mode
	m_BitmapStyle = bStretch ? DRAWBITMAP_STRETCH : DRAWBITMAP_NORMAL;
}


/*	GetClientRect
	Ŭ���̾�Ʈ�� ������ ���ϴ� �Լ�(�ؽ�Ʈ�� �Է��ϴ� ����)
*/
MRECT MBTextAreaLook::GetClientRect( MTextArea* pTextArea, MRECT& r)
{
	if ( m_bCustomLook)
		return MRECT( r.x + 1,  r.y + 1,  r.w - 2,  r.h - 2);


	MRECT rect = r;
	rect.x += m_Bitmaps[ 0].m_rCoord[ 1].w;
	rect.y += m_Bitmaps[ 0].m_rCoord[ 1].h;
	rect.w -= m_Bitmaps[ 0].m_rCoord[ 1].w + m_Bitmaps[ 0].m_rCoord[ 3].w;
	rect.h -= m_Bitmaps[ 0].m_rCoord[ 1].h + m_Bitmaps[ 0].m_rCoord[ 6].h;


	return rect;
}


} // namespace mint3








/********************************************************************
���⼭���� ���� �ڵ��Դϴ�

#include "stdafx.h"
#include "MBTextAreaLook.h"
#include "MBitmapDrawer.h"

namespace mint3 {

void MBTextAreaLook::OnFrameDraw(MTextArea* pTextArea, MDrawContext* pDC)
{
//	MRECT r = pTextArea->GetInitialClientRect();
//	DrawBitmapFrame9(pDC, r, m_pFrameBitmaps);

	MRECT r = pTextArea->GetInitialClientRect();
	pDC->SetColor(19,19,19,255);
	pDC->FillRectangle(r);
	pDC->SetColor(128,128,128,255);
	pDC->Rectangle(r);

}

MBTextAreaLook::MBTextAreaLook(void)
{
	m_pFont = NULL;
	for(int i=0; i<9; i++){
		m_pFrameBitmaps[i] = NULL;
	}
}

MRECT MBTextAreaLook::GetClientRect(MTextArea* pTextArea, MRECT& r)
{
	int al = GETWIDTH(m_pFrameBitmaps[3]);
	int au = GETHEIGHT(m_pFrameBitmaps[7]);
	int ar = GETWIDTH(m_pFrameBitmaps[5]);
	int ab = GETHEIGHT(m_pFrameBitmaps[1]);

	int nWidth = r.w-(al+ar);
	if(pTextArea->IsScrollBarVisible()) 
		nWidth-=pTextArea->GetScrollBarWidth();

	const int nMargin = 2;

	return MRECT(r.x+al + nMargin, r.y+au + nMargin, nWidth -nMargin*2, r.h-(au+ab) -nMargin*2);
}

} // namespace mint3
*/