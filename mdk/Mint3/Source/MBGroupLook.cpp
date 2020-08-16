/************************************************************************
MBGroupLook

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : Group ��Ʈ���� Ŀ���ҷ�(Custom look) Ŭ���� �Դϴ�.
*************************************************************************/


#include "stdafx.h"
#include "MBGroupLook.h"


// namespace : mint3
namespace mint3
{

/*	MBGroupLook
	������
*/
MBGroupLook::MBGroupLook()
{
	// Ŀ���� Look ��� ����
	m_bCustomLook = false;


	// ��Ʈ���� �÷��� ����
	m_BitmapStyle = DRAWBITMAP_STRETCH;


	// ��Ʈ�� �̹����� Ÿ�������� ���� ����
	m_bTiled = false;


	// ��Ʈ �ʱ�ȭ
	m_pFont = NULL;


	// �ؽ�Ʈ ���� ����
	m_colorFont[ 0]	= MCOLOR( 200, 200, 200);		// Normal
	m_colorFont[ 1]	= MCOLOR( 255, 255, 255);		// Focused


	// �ؽ�Ʈ ��ġ ������ ����
	m_ptTextOffset.x = 0;
	m_ptTextOffset.y = 0;
}


/*	~MBGroupLook
	�Ҹ���
*/
MBGroupLook::~MBGroupLook()
{
}


/*	SetBitmap
	�����ӿ� ���� �̹����� �����ϴ� �Լ�
*/
void MBGroupLook::SetBitmap( const string strFileName)
{
	m_Bitmaps[ 0].strBitmapName = strFileName;
	m_Bitmaps[ 1].strBitmapName = strFileName;
}


/*	SetAssignRect
	�����ӿ� ���� ��Ʈ���� ������ �����ϴ� �Լ�
*/
void MBGroupLook::SetAssignRect( int state, int index, float x1, float y1, float x2, float y2)
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


/*	OnDraw
	�������� �׸��� �Լ�
*/
void MBGroupLook::OnDraw( MGroup* pGroup, MDrawContext* pDC)
{
	// �������� ���¸� ���մϴ�
	int state = pGroup->IsEnable()  ?  0 : 1;


	// �������� �׸��ϴ�
	MRECT rect = pGroup->GetClientRect();
	rect.y += m_ptTextOffset.y;
	rect.h -= m_ptTextOffset.y;
	DrawBitmapByStyle( pDC, rect, MBitmapManager::Get( m_Bitmaps[ state].strBitmapName.c_str()), m_Bitmaps[ state].m_rCoord, m_BitmapStyle, m_bTiled);


	// �ؽ�Ʈ ���

	// ��Ʈ ����
	pDC->SetFont( m_pFont);


	// �ؽ�Ʈ�� �׸��ϴ�
//	if ( pDC->BeginFont())
	{
		// �ؽ�Ʈ�� ����մϴ�
		pDC->SetColor( m_colorFont[ state]);
		pDC->Text( m_ptTextOffset.x, 0, pGroup->GetText());

//		pDC->EndFont();
	}
}


/*	SetStretchBlt
	��Ʈ���� �÷� ���� �� �ְ� �����ϴ� �Լ�
*/
void MBGroupLook::SetStretchBlt( bool bStretch /*=true*/)
{
	// Set stretch mode
	m_BitmapStyle = bStretch ? DRAWBITMAP_STRETCH : DRAWBITMAP_NORMAL;
}


} // namespace mint3




















/****************************************************************************
���⼭���ʹ� �����ڵ� �Դϴ�

#include "stdafx.h"
#include "MBGroupLook.h"
#include "MBitmapDrawer.h"

namespace mint3 {

void MBGroupLook::OnDraw(MGroup* pGroup, MDrawContext* pDC)
{
	MRECT r = pGroup->GetInitialClientRect();
	
	//DrawBitmapFrame9(pDC, r, m_pFrameBitmaps, false, m_bStretch);

	DrawBitmapFrameCustom1( pDC, r, m_pFrameBitmaps, m_bStretch );

	if(m_pFont!=NULL) pDC->SetFont(m_pFont);
	pDC->SetColor(m_FontColor);
	pDC->Text(r.x+m_TitlePosition.x, r.y+m_TitlePosition.y, pGroup->m_szText);
}

MBGroupLook::MBGroupLook(void)
{
	for(int i=0; i<FRAME_BITMAP_COUNT; i++){
		m_pFrameBitmaps[i] = NULL;
	}
	m_pFont=NULL;
	m_bStretch = true;
	m_TitlePosition = MPOINT(8, 4);
}

MRECT MBGroupLook::GetClientRect(MGroup* pGroup, MRECT& r)
{
//	int al = GETWIDTH(m_pFrameBitmaps[3]);
//	int au = GETHEIGHT(m_pFrameBitmaps[6]);
//	int ar = GETWIDTH(m_pFrameBitmaps[5]);
//	int ab = GETHEIGHT(m_pFrameBitmaps[0]);
//	return MRECT(r.x+al, r.y+au, r.w-(al+ar), r.h-(au+ab));
}

} // namespace mint3
*/
