/************************************************************************
MBFrameLook

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : Frame ��Ʈ���� Ŀ���ҷ�(Custom look) Ŭ���� �Դϴ�.
*************************************************************************/


#include "stdafx.h"
#include "MBFrameLook.h"


// namespace : mint3
namespace mint3
{

/*	MBFrameLook
	������
*/
MBFrameLook::MBFrameLook()
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
	m_colorFont[ 2]	= MCOLOR( 150, 150, 150);		// Disabled


	// Ÿ��Ʋ �׸��� ���
	m_bTitleShadow = true;


	// Ÿ��Ʋ ������ �⺻�� ����
	m_ptTitleOffset.x = 8;
	m_ptTitleOffset.y = 8;
}


/*	~MBFrameLook
	�Ҹ���
*/
MBFrameLook::~MBFrameLook()
{
}


/*	SetBitmap
	�����ӿ� ���� �̹����� �����ϴ� �Լ�
*/
void MBFrameLook::SetBitmap( const string strFileName)
{
	m_Bitmaps[ 0].strBitmapName = strFileName;
	m_Bitmaps[ 1].strBitmapName = strFileName;
	m_Bitmaps[ 2].strBitmapName = strFileName;
}


/*	SetAssignRect
	�����ӿ� ���� ��Ʈ���� ������ �����ϴ� �Լ�
*/
void MBFrameLook::SetAssignRect( int state, int index, float x1, float y1, float x2, float y2)
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
void MBFrameLook::OnDraw( MFrame* pFrame, MDrawContext* pDC)
{
	// �������� ���¸� ���մϴ�
	int state;
	if ( !pFrame->IsEnable())		state = 2;
	else if ( pFrame->IsFocus())	state = 1;
	else							state = 0;


	// �������� �׸��ϴ�
	MRECT cr = pFrame->GetClientRect();
	cr.x += m_WinRectOffset.x;
	cr.y += m_WinRectOffset.y;
	cr.w += m_WinRectOffset.w;
	cr.h += m_WinRectOffset.h;
	DrawBitmapByStyle( pDC, cr, MBitmapManager::Get( m_Bitmaps[ state].strBitmapName.c_str()), m_Bitmaps[ state].m_rCoord, m_BitmapStyle, m_bTiled);


	// Ÿ��Ʋ�� �ؽ�Ʈ�� �׸��ϴ�
	if ( pFrame->m_bTitleBar)
		DrawText( pFrame, pDC);
}


/*	DrawText
	�������� �ؽ�Ʈ�� �׸��� �Լ�
*/
void MBFrameLook::DrawText( MFrame* pFrame, MDrawContext* pDC)
{
	// ��Ʈ ����
	pDC->SetFont( m_pFont);


	// �ؽ�Ʈ�� �׸��ϴ�
//	if ( pDC->BeginFont())
	{
		// �׸��� �ؽ�Ʈ�� ����մϴ�
		if ( m_bTitleShadow)
		{
			pDC->SetColor( MCOLOR( 0, 0, 0, 128));
			pDC->Text( m_ptTitleOffset.x + 1, m_ptTitleOffset.y + 1, pFrame->GetText());
		}

		
		// �������� ���¸� ���մϴ�
		int state;
		if ( !pFrame->IsEnable())		state = 2;
		else if ( pFrame->IsFocus())	state = 1;
		else							state = 0;


		// �ؽ�Ʈ�� ����մϴ�
		pDC->SetColor( m_colorFont[ state]);
		pDC->Text( m_ptTitleOffset.x, m_ptTitleOffset.y, pFrame->GetText());
//		pDC->EndFont();
	}
}


/*	SetStretchBlt
	��Ʈ���� �÷� ���� �� �ְ� �����ϴ� �Լ�
*/
void MBFrameLook::SetStretchBlt( bool bStretch /*=true*/)
{
	// Set stretch mode
	m_BitmapStyle = bStretch ? DRAWBITMAP_STRETCH : DRAWBITMAP_NORMAL;
}


} // namespace mint3









/************************************************************************
 
 ���� �ڵ��Դϴ�


#include "stdafx.h"
#include "Mint.h"
#include "MBFrameLook.h"
#include "MBitmapDrawer.h"

namespace mint3 {

void MBFrameLook::OnDraw(MFrame* pFrame, MDrawContext* pDC)
{
	MRECT r = pFrame->GetInitialClientRect();

	// ��.. ���â�� tooltip frame -
	// DrawBitmapFrame2 �� 2���� �̹����� ����ؼ� �׸�..

	if(GetCustomLook()==3) { 

		if(pFrame->GetVisible()==false)
			return;

		MRECT rr = r; // �����̶� �׸��� ������ ���� ����..

		rr.x -= 10;
		rr.y -= 10;
		rr.w += 20;
		rr.h += 20;

		// ������ ���� ������� ���⶧���� cliprect �� ����..

		MRECT cliprect = MRECT(0,0,MGetWorkspaceWidth(),MGetWorkspaceHeight());

		DrawBitmapFrame2(pDC,rr,cliprect,m_pFrameBitmaps);

		return;
	}

	MCOLOR color = MCOLOR(0xFF, 0xFF, 0xFF, (unsigned char)(255 * pFrame->GetOpacity()));

	if( GetCustomLook() == 1 )
		DrawBitmapFrameCustom1(pDC, r, m_pFrameBitmaps, m_bStretch);
	else if( GetCustomLook() == 2)
		DrawBitmapFrameCustom2(pDC, r, m_pFrameBitmaps, m_BGColor, m_bStretch );
	else if( GetCustomLook() == 3 )
	{
		pDC->SetColor( 128, 128, 128 );
		//r = pFrame->GetRect();
		pDC->Rectangle( r );
	}
	else
	{
		// �ؿ� �Լ��� ���̴� �� ���� �ʴ�.
//		DrawBitmapFrame9(pDC, r, m_pFrameBitmaps, m_bStretch, GetScale());

		MDRAWBITMAPSTYLE eDrawStyle = DRAWBITMAP_NORMAL;
		if(m_bStretch)
			eDrawStyle = DRAWBITMAP_STRETCH;

		DrawBitmapByStyle(pDC, r, MBitmapManager::Get(m_sBitmaps.strBitmapName.c_str()), m_sBitmaps.m_rCoord, eDrawStyle);
	}

	if(pFrame->m_bTitleBar==true){
		if (pFrame->GetCloseButton()->m_bVisible)
		{
			MBitmap* pBitmap;
			if (pFrame->GetCloseButton()->IsButtonDown() == false)
			{
				pBitmap = m_pCloseButtonBitmaps[0];
			}
			else
			{
				pBitmap = m_pCloseButtonBitmaps[1];
				if (pBitmap == NULL) pBitmap = m_pCloseButtonBitmaps[0];
			}

			if (pBitmap!=NULL)
			{
				int x, y;
				x = pFrame->GetCloseButton()->m_Rect.x;
				y = pFrame->GetCloseButton()->m_Rect.y;

				pDC->SetBitmap(pBitmap);
				pDC->Draw(x, y);
			}

		}
		if (pFrame->GetMinimizeButton()->m_bVisible)
		{
			MBitmap* pBitmap;
			if (pFrame->GetMinimizeButton()->IsButtonDown() == false)
			{
				pBitmap = m_pMinimizeButtonBitmaps[0];
			}
			else
			{
				pBitmap = m_pMinimizeButtonBitmaps[1];
				if (pBitmap == NULL) pBitmap = m_pMinimizeButtonBitmaps[0];
			}

			if (pBitmap!=NULL)
			{
				int x, y;
				x = pFrame->GetMinimizeButton()->m_Rect.x;
				y = pFrame->GetMinimizeButton()->m_Rect.y;

				pDC->SetBitmap(pBitmap);
				pDC->Draw(x, y);
			}

		}

		if(m_pFont!=NULL) pDC->SetFont(m_pFont);
		pDC->SetColor(MCOLOR(0x0));
		if( GetCustomLook() == 0 ) pDC->Text(r.x+16, r.y+12, pFrame->m_szText);
//		pDC->Text(r.x+12, r.y+8, pFrame->m_szText);
//		pDC->Text(r.x+16, r.y+8, pFrame->m_szText);
//		pDC->Text(r.x+12, r.y+12, pFrame->m_szText);
		pDC->SetColor(m_FontColor);

		// ���߿� align����ؼ� �ٽ� �����ؾ� ��
		
		int y = int(m_TitlePosition.y*GetScale());

		if(m_pFrameBitmaps[7])
		{
			int au = (int)(GetScale() * m_pFrameBitmaps[7]->GetHeight());
			y = (au - m_pFont->GetHeight())/2;
		}

		pDC->Text(int(r.x+m_TitlePosition.x*GetScale()), r.y+y, pFrame->m_szText);
//		pDC->Text(r.x+m_TitlePosition.x*GetScale(), r.y+m_TitlePosition.y*GetScale(), pFrame->m_szText);

	//	pDC->Text(r.x+14, r.y+10, pFrame->m_szText);
	}
}

MBFrameLook::MBFrameLook(void)	: MScalableLook()
{
	m_szDefaultTitle[0]=0;
	m_TitlePosition = MPOINT(14, 10);
	m_pFont		= NULL;
	m_FontColor = MCOLOR(255,255,255,255);
	m_bStretch = true;

	for(int i=0; i<FRAME_BITMAP_COUNT; i++){
		m_pFrameBitmaps[i] = NULL;
	}
	for (int i=0; i<FRAME_BUTTON_BITMAP_COUNT; i++)
	{
		m_pCloseButtonBitmaps[i] = NULL;
		m_pMinimizeButtonBitmaps[i] = NULL;
	}
	m_iCustomLook = 0;
}

#define RECT_DEFAULT_SPACE 12
MRECT MBFrameLook::GetClientRect(MFrame* pFrame, MRECT& r)
{
	if(GetCustomLook() == 1)
	{
 		int al = FRAME_OUTLINE_WIDTH+FRAME_WIDTH+FRAME_INLINE_WIDTH+RECT_DEFAULT_SPACE;
		int au = m_pFrameBitmaps[7]->GetHeight() + FRAME_OUTLINE_WIDTH + FRAME_INLINE_WIDTH+RECT_DEFAULT_SPACE;
		return MRECT(r.x+al, r.y+au, r.w-(al*2), r.h-(al+au));
	}
	if( GetCustomLook() == 2 )
	{
#define CUSTOM2_FRAME_OFFSET_WIDTH 2
#define CUSTOM2_FRAME_OFFSET_HEIGHT 10
		return MRECT(r.x + CUSTOM2_FRAME_OFFSET_WIDTH, r.y + CUSTOM2_FRAME_OFFSET_HEIGHT , r.w - 2 * CUSTOM2_FRAME_OFFSET_WIDTH, r.h - 2 * CUSTOM2_FRAME_OFFSET_HEIGHT);
	}
	else
	{
		if(IsNull(m_pFrameBitmaps, FRAME_BITMAP_COUNT)==true) return r;

		float fScale = GetScale();
		int al = (int)(fScale * m_pFrameBitmaps[3]->GetWidth());
		int au = (int)(fScale * m_pFrameBitmaps[7]->GetHeight());
		int ar = (int)(fScale * m_pFrameBitmaps[5]->GetWidth());
		int ab = (int)(fScale * m_pFrameBitmaps[1]->GetHeight());
		return MRECT(r.x+al, r.y+au, r.w-(al+ar), r.h-(au+ab));
	}
}

} // namespace mint3
*/