/************************************************************************
MBSpinCtrlLook

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : SpinCtrl ��Ʈ���� Ŀ���ҷ�(Custom look) Ŭ���� �Դϴ�.
*************************************************************************/


#include "stdafx.h"
#include "MBSpinCtrlLook.h"


// namespace : mint3
namespace mint3
{

/*	MBSpinCtrlLook
	������
*/
MBSpinCtrlLook::MBSpinCtrlLook()
{
	// Ŀ���� Look ��� ����
	m_bCustomLook = false;


	// ��Ʈ���� �÷��� ����
	m_BitmapStyle = DRAWBITMAP_STRETCH;


	// ��Ʈ�� �̹����� Ÿ�������� ���� ����
	m_bTiled = false;

}


/*	~MBSpinCtrlLook
	�Ҹ���
*/
MBSpinCtrlLook::~MBSpinCtrlLook()
{
}


/*	SetBitmap
	�����ӿ� ���� �̹����� �����ϴ� �Լ�
*/
void MBSpinCtrlLook::SetBitmap( const string strFileName)
{
	for ( int i = 0;  i < MSPR_MAX;  i++)
	{
		m_Bitmaps[ i][ 0].strBitmapName = strFileName;
		m_Bitmaps[ i][ 1].strBitmapName = strFileName;
		m_Bitmaps[ i][ 2].strBitmapName = strFileName;
	}
}


/*	SetAssignRect
	�����ӿ� ���� ��Ʈ���� ������ �����ϴ� �Լ�
*/
void MBSpinCtrlLook::SetAssignRect( int type, int state, int index, float x1, float y1, float x2, float y2)
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
void MBSpinCtrlLook::OnDraw( MSpinCtrl* pSpinCtrl, MDrawContext* pDC)
{
	// ������Ʈ���� ���¸� ���մϴ�
	int state;
	bool bEnable = pSpinCtrl->IsEnable();


	if ( pSpinCtrl->GetType() == MSPT_VERTICAL)
	{
		// �� ��ư�� �׸��ϴ�
		if ( !bEnable)	state = 2;
		else if ( pSpinCtrl->GetPushedRgn() == MSPR_UPARROW)	state = 1;
		else			state = 0;
		DrawBitmapByStyle( pDC, pSpinCtrl->GetUpArrowRgn(), MBitmapManager::Get( m_Bitmaps[ MSPR_UPARROW][ state].strBitmapName.c_str()),
			m_Bitmaps[ MSPR_UPARROW][ state].m_rCoord, m_BitmapStyle, m_bTiled);

		// �Ʒ� ��ư�� �׸��ϴ�
		if ( !bEnable)	state = 2;
		else if ( pSpinCtrl->GetPushedRgn() == MSPR_DOWNARROW)		state = 1;
		else			state = 0;
		DrawBitmapByStyle( pDC, pSpinCtrl->GetDownArrowRgn(), MBitmapManager::Get( m_Bitmaps[ MSPR_DOWNARROW][ state].strBitmapName.c_str()),
			m_Bitmaps[ MSPR_DOWNARROW][ state].m_rCoord, m_BitmapStyle, m_bTiled);
	}

	else
	{
		// ���� ��ư�� �׸��ϴ�
		if ( !bEnable)	state = 2;
		else if ( pSpinCtrl->GetPushedRgn() == MSPR_LEFTARROW)		state = 1;
		else			state = 0;
		DrawBitmapByStyle( pDC, pSpinCtrl->GetLeftArrowRgn(), MBitmapManager::Get( m_Bitmaps[ MSPR_LEFTARROW][ state].strBitmapName.c_str()),
			m_Bitmaps[ MSPR_LEFTARROW][ state].m_rCoord, m_BitmapStyle, m_bTiled);

		// ������ ��ư�� �׸��ϴ�
		if ( !bEnable)	state = 2;
		else if ( pSpinCtrl->GetPushedRgn() == MSPR_RIGHTARROW)	state = 1;
		else			state = 0;
		DrawBitmapByStyle( pDC, pSpinCtrl->GetRightArrowRgn(), MBitmapManager::Get( m_Bitmaps[ MSPR_RIGHTARROW][ state].strBitmapName.c_str()),
			m_Bitmaps[ MSPR_RIGHTARROW][ state].m_rCoord, m_BitmapStyle, m_bTiled);
	}
}


/*	SetStretchBlt
��Ʈ���� �÷� ���� �� �ְ� �����ϴ� �Լ�
*/
void MBSpinCtrlLook::SetStretchBlt( bool bStretch /*=true*/)
{
	// Set stretch mode
	m_BitmapStyle = bStretch ? DRAWBITMAP_STRETCH : DRAWBITMAP_NORMAL;
}


} // namespace mint3
