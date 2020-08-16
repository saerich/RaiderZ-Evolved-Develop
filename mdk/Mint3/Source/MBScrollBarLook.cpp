/************************************************************************
MBScrollBarLook

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : ScrollBar ��Ʈ���� Ŀ���ҷ�(Custom look) Ŭ���� �Դϴ�.
*************************************************************************/


#include "stdafx.h"
#include "MBScrollBarLook.h"


// namespace : mint3
namespace mint3
{

/*	MBScrollBarLook
	������
*/
MBScrollBarLook::MBScrollBarLook()
{
	// Ŀ���� Look ��� ����
	m_bCustomLook = false;


	// ��Ʈ���� �÷��� ����
	m_BitmapStyle = DRAWBITMAP_STRETCH;


	// ��Ʈ�� �̹����� Ÿ�������� ���� ����
	m_bTiled = false;

}


/*	~MBScrollBarLook
	�Ҹ���
*/
MBScrollBarLook::~MBScrollBarLook()
{
}


/*	SetBitmap
	�����ӿ� ���� �̹����� �����ϴ� �Լ�
*/
void MBScrollBarLook::SetBitmap( const string strFileName)
{
	for ( int i = 0;  i < MSBR_MAX;  i++)
	{
		m_Bitmaps[ i][ 0].strBitmapName = strFileName;
		m_Bitmaps[ i][ 1].strBitmapName = strFileName;
		m_Bitmaps[ i][ 2].strBitmapName = strFileName;
	}
}


/*	SetAssignRect
	�����ӿ� ���� ��Ʈ���� ������ �����ϴ� �Լ�
*/
void MBScrollBarLook::SetAssignRect( int type, int state, int index, float x1, float y1, float x2, float y2)
{
	// Check valid range
	if ( index >= 9)
		return;


	// Set bitmap assignment
	m_Bitmaps[ type][ state].m_rCoord[ index].x = (int)x1;
	m_Bitmaps[ type][ state].m_rCoord[ index].y = (int)y1;
	m_Bitmaps[ type][ state].m_rCoord[ index].w = (int)x2;
	m_Bitmaps[ type][ state].m_rCoord[ index].h = (int)y2;
}


/*	OnDraw
	�������� �׸��� �Լ�
*/
void MBScrollBarLook::OnDraw( MScrollBar* pScrollBar, MDrawContext* pDC)
{
	// ��ũ�ѹ��� ���¸� ���մϴ�
	int state;
	bool bEnable = pScrollBar->IsEnable();


	// ����� �׸��ϴ�
	if ( !bEnable)	state = 2;
	else if ( pScrollBar->GetPushedRgn() == MSBR_BKGRND)	state = 1;
	else			state = 0;
	DrawBitmapByStyle( pDC, pScrollBar->GetBackgroundRgn(), MBitmapManager::Get( m_Bitmaps[ MSBR_BKGRND][ state].strBitmapName.c_str()),
		m_Bitmaps[ MSBR_BKGRND][ state].m_rCoord, m_BitmapStyle, m_bTiled);


	if ( pScrollBar->IsShowArrow())
	{
		if ( pScrollBar->GetType() == MSBT_VERTICAL)
		{
			// �� ��ư�� �׸��ϴ�
			if ( !bEnable)	state = 2;
			else if ( pScrollBar->GetPushedRgn() == MSBR_UPARROW)	state = 1;
			else			state = 0;
			DrawBitmapByStyle( pDC, pScrollBar->GetUpArrowRgn(), MBitmapManager::Get( m_Bitmaps[ MSBR_UPARROW][ state].strBitmapName.c_str()),
				m_Bitmaps[ MSBR_UPARROW][ state].m_rCoord, m_BitmapStyle, m_bTiled);

			// �Ʒ� ��ư�� �׸��ϴ�
			if ( !bEnable)	state = 2;
			else if ( pScrollBar->GetPushedRgn() == MSBR_DOWNARROW)		state = 1;
			else			state = 0;
			DrawBitmapByStyle( pDC, pScrollBar->GetDownArrowRgn(), MBitmapManager::Get( m_Bitmaps[ MSBR_DOWNARROW][ state].strBitmapName.c_str()),
				m_Bitmaps[ MSBR_DOWNARROW][ state].m_rCoord, m_BitmapStyle, m_bTiled);
		}

		else
		{
			// ���� ��ư�� �׸��ϴ�
			if ( !bEnable)	state = 2;
			else if ( pScrollBar->GetPushedRgn() == MSBR_LEFTARROW)		state = 1;
			else			state = 0;
			DrawBitmapByStyle( pDC, pScrollBar->GetLeftArrowRgn(), MBitmapManager::Get( m_Bitmaps[ MSBR_LEFTARROW][ state].strBitmapName.c_str()),
				m_Bitmaps[ MSBR_LEFTARROW][ state].m_rCoord, m_BitmapStyle, m_bTiled);

			// ������ ��ư�� �׸��ϴ�
			if ( !bEnable)	state = 2;
			else if ( pScrollBar->GetPushedRgn() == MSBR_RIGHTARROW)	state = 1;
			else			state = 0;
			DrawBitmapByStyle( pDC, pScrollBar->GetRightArrowRgn(), MBitmapManager::Get( m_Bitmaps[ MSBR_RIGHTARROW][ state].strBitmapName.c_str()),
				m_Bitmaps[ MSBR_RIGHTARROW][ state].m_rCoord, m_BitmapStyle, m_bTiled);
		}

	}


	// Thumb ��ư�� �׸��ϴ�
	if ( !bEnable)	state = 2;
	else if ( pScrollBar->GetPushedRgn() == MSBR_THUMB)		state = 1;
	else			state = 0;
	DrawBitmapByStyle( pDC, pScrollBar->GetThumbRgn(), MBitmapManager::Get( m_Bitmaps[ MSBR_THUMB][ state].strBitmapName.c_str()),
		m_Bitmaps[ MSBR_THUMB][ state].m_rCoord, m_BitmapStyle, m_bTiled);
}


/*	SetStretchBlt
��Ʈ���� �÷� ���� �� �ְ� �����ϴ� �Լ�
*/
void MBScrollBarLook::SetStretchBlt( bool bStretch /*=true*/)
{
	// Set stretch mode
	m_BitmapStyle = bStretch ? DRAWBITMAP_STRETCH : DRAWBITMAP_NORMAL;
}


} // namespace mint3
