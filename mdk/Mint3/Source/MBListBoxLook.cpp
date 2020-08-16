/************************************************************************
MBListBoxLook

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : ListBox ��Ʈ���� Ŀ���ҷ�(Custom look) Ŭ���� �Դϴ�.
*************************************************************************/


#include "stdafx.h"
#include "MBListBoxLook.h"


// namespace : mint3
namespace mint3
{

/*	MBListBoxLook
	������
*/
MBListBoxLook::MBListBoxLook()
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
	for ( int i = 0;  i < MLBR_MAX;  i++)
	{
		m_colorFont[ i][ 0]	= MCOLOR( 200, 200, 200);		// Normal
		m_colorFont[ i][ 1]	= MCOLOR( 255, 255, 255);		// Focused
		m_colorFont[ i][ 2]	= MCOLOR( 150, 150, 150);		// Disabled
	}
}


/*	~MBListBoxLook
	�Ҹ���
*/
MBListBoxLook::~MBListBoxLook()
{
}


/*	SetBitmap
	�����ӿ� ���� �̹����� �����ϴ� �Լ�
*/
void MBListBoxLook::SetBitmap( const string strFileName)
{
	for ( int i = 0;  i < MLBR_MAX;  i++)
	{
		m_Bitmaps[ i][ 0].strBitmapName = strFileName;
		m_Bitmaps[ i][ 1].strBitmapName = strFileName;
		m_Bitmaps[ i][ 2].strBitmapName = strFileName;
	}
}


/*	SetAssignRect
	�����ӿ� ���� ��Ʈ���� ������ �����ϴ� �Լ�
*/
void MBListBoxLook::SetAssignRect( int type, int state, int index, float x1, float y1, float x2, float y2)
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


/*	OnHeaderDraw
	����Ʈ �ڽ��� ����� �׸��� �Լ�
*/
void MBListBoxLook::OnHeaderDraw( MListBox* pListBox, MDrawContext* pDC, MRECT& r, const char* szText)
{
	// �ؽ�Ʈ�� ������ �׳� ����
	if ( szText == NULL)
		return;


	// ����Ʈ �ڽ��� ���¸� ���Ѵ�
	int state;
	if ( !pListBox->IsEnable())			state = 2;
	else if ( pListBox->IsFocus())		state = 1;
	else								state = 0;

	// ��� �� ����� �׸���
	DrawBitmapByStyle( pDC, r, MBitmapManager::Get( m_Bitmaps[ MLBR_HEADER][ state].strBitmapName.c_str()),
					m_Bitmaps[ MLBR_HEADER][ state].m_rCoord, DRAWBITMAP_STRETCH);


	// �ؽ�Ʈ�� ����Ѵ�
	pDC->SetFont( m_pFont);
	pDC->SetColor( m_colorFont[ MLBR_HEADER][ state]);
	pDC->Text( r, szText);
}


/*	OnItemDraw
	����Ʈ �ڽ��� ���� ���븦 �׸��� �Լ�
*/
void MBListBoxLook::OnDrawSelectBar( MListBox* pListBox, MDrawContext* pDC, MRECT& r, bool bSelected, bool bEnabled, bool bFocus)
{
	// ����Ʈ �ڽ��� ���¸� ���Ѵ�
	int state;
	if ( bEnabled == false)				state = 2;
	else if ( !pListBox->IsEnable())	state = 2;
	else if ( pListBox->IsFocus())		state = 1;
	else								state = 0;


	// ���õ� �������� ��쿣 ���ø��븦 �׸���
	if ( bSelected && bEnabled)
		DrawBitmapByStyle( pDC, r, MBitmapManager::Get( m_Bitmaps[ MLBR_SELECTBAR][ state].strBitmapName.c_str()),
		m_Bitmaps[ MLBR_SELECTBAR][ state].m_rCoord, DRAWBITMAP_STRETCH);
}

							  
/*	OnItemDraw
	����Ʈ �ڽ��� �������� �׸��� �Լ�
*/
int MBListBoxLook::OnItemDraw( MListBox* pListBox, MDrawContext* pDC, MRECT& r, const char* szText, MCOLOR color, bool bSelected, bool bEnabled, bool bFocus, int nAdjustWidth)
{
	// �ؽ�Ʈ�� ������ �ٷ� ����
	if ( szText == NULL)
		return 0;


	// ����Ʈ �ڽ��� ���¸� ���Ѵ�
	int state;
	if ( bEnabled == false)				state = 2;
	else if ( !pListBox->IsEnable())	state = 2;
	else if ( pListBox->IsFocus())		state = 1;
	else								state = 0;


	// ������ �̹����� �׸���
	if( m_pItemSlotBitmap != NULL )
	{
		pDC->SetBitmap( m_pItemSlotBitmap );

		if ( bEnabled == false)
		{
			pDC->SetBitmapColor( 128, 128, 128);
			pDC->Draw(r.x, r.y, r.w, r.h );
			pDC->SetBitmapColor( 255, 255, 255);
		}
		else
			pDC->Draw(r.x, r.y, r.w, r.h );
	}


	// �ؽ�Ʈ ���
	else if ( bSelected && bEnabled)
		pDC->SetColor( m_colorFont[ MLBR_SELECTBAR][ state]);
	else
		pDC->SetColor( m_colorFont[ MLBR_LISTBOX][ state]);

	pDC->SetFont( m_pFont);
	pDC->Text( r, szText, MAM_LEFT | MAM_VCENTER);


	return 1;
}

int MBListBoxLook::OnItemDraw( MListBox* pListBox, MDrawContext* pDC, MRECT& r, MBitmap* pBitmap, bool bSelected, bool bEnabled, bool bFocus, int nAdjustWidth)
{
	// ����Ʈ �ڽ��� ���¸� ���Ѵ�
	int state;
	if ( bEnabled == false)				state = 2;
	else if ( !pListBox->IsEnable())	state = 2;
	else if ( pListBox->IsFocus())		state = 1;
	else								state = 0;


	pDC->SetBitmap( pBitmap);

	if ( bEnabled == false)
	{
		pDC->SetBitmapColor( 128, 128, 128);
		pDC->Draw(r.x, r.y, r.w, r.h );
		pDC->SetBitmapColor( 255, 255, 255);
	}
	else
		pDC->Draw(r.x, r.y, r.w, r.h );


	return 1;
}


/*	OnFrameDraw
	�������� �׸��� �Լ�
*/
void MBListBoxLook::OnFrameDraw( MListBox* pListBox, MDrawContext* pDC)
{
	// ���� Ŭ���̾�Ʈ�� ������ ���մϴ�
	MRECT rect = pListBox->GetRect();
	rect.x = rect.y = 0;


	// ����Ʈ�ڽ��� ���¸� ���մϴ�
	int state;
	if ( !pListBox->IsEnable())			state = 2;
	else if ( pListBox->IsFocus())		state = 1;
	else								state = 0;


	// �������� �׸��ϴ�
	DrawBitmapByStyle( pDC, rect, MBitmapManager::Get( m_Bitmaps[ MLBR_LISTBOX][ state].strBitmapName.c_str()),
					m_Bitmaps[ MLBR_LISTBOX][ state].m_rCoord, m_BitmapStyle, m_bTiled);
}


/*	SetStretchBlt
	��Ʈ���� �÷� ���� �� �ְ� �����ϴ� �Լ�
*/
void MBListBoxLook::SetStretchBlt( bool bStretch /*=true*/)
{
	// Set stretch mode
	m_BitmapStyle = bStretch ? DRAWBITMAP_STRETCH : DRAWBITMAP_NORMAL;
}


/*	GetClientRect
	Ŭ���̾�Ʈ�� ������ ���ϴ� �Լ�(�ؽ�Ʈ�� �Է��ϴ� ����)
*/
MRECT MBListBoxLook::GetClientRect( MListBox* pListBox, MRECT& r)
{
	if ( m_bCustomLook)
		return MRECT( r.x + 1,  r.y + 1,  r.w - 2,  r.h - 2);


	MRECT rect = r;
	rect.x += m_Bitmaps[ MLBR_LISTBOX][ 0].m_rCoord[ 1].w;
	rect.y += m_Bitmaps[ MLBR_LISTBOX][ 0].m_rCoord[ 1].h;
	rect.w -= m_Bitmaps[ MLBR_LISTBOX][ 0].m_rCoord[ 1].w + m_Bitmaps[ MLBR_LISTBOX][ 0].m_rCoord[ 3].w;
	rect.h -= m_Bitmaps[ MLBR_LISTBOX][ 0].m_rCoord[ 1].h + m_Bitmaps[ MLBR_LISTBOX][ 0].m_rCoord[ 6].h;


	return rect;
}


} // namespace mint3








/************************************************************************
���⼭���ʹ� ���� �ڵ��Դϴ�


#include "stdafx.h"
#include "MBListBoxLook.h"
#include "MBitmapDrawer.h"

namespace mint3 {

void MBListBoxLook::OnFrameDraw(MListBox* pListBox, MDrawContext* pDC)
{
	MRECT r = pListBox->GetInitialClientRect();
	//DrawBitmapFrame9(pDC, r, m_pFrameBitmaps);
	pDC->SetColor(19,19,19,255);
	pDC->FillRectangle(r);
	pDC->SetColor(128,128,128,255);
	pDC->Rectangle(r);
}

MBListBoxLook::MBListBoxLook(void)
{
	for(int i=0; i<9; i++){
		m_pFrameBitmaps[i] = NULL;
	}
	m_pFont=NULL;
}

MRECT MBListBoxLook::GetClientRect(MListBox* pListBox, MRECT& r)
{
	//int lw = GETWIDTH(m_pFrameBitmaps[3]);
	//int rw = GETWIDTH(m_pFrameBitmaps[5]);
	//int uh = GETHEIGHT(m_pFrameBitmaps[7]);
	//int bh = GETHEIGHT(m_pFrameBitmaps[1]);
	//return MRECT(r.x+lw, r.y+uh, r.w-(lw+rw)-((pListBox->GetScrollBar()->IsVisible()==true)?(pListBox->GetScrollBar()->GetClientRect().w):0), r.h-(uh+bh));
	return MRECT(r.x+3, r.y+3, r.w-(6)-((pListBox->GetScrollBar()->IsVisible()==true)?(pListBox->GetScrollBar()->GetClientRect().w):0), r.h-(6));
}

void MBListBoxLook::OnDraw(MListBox* pListBox, MDrawContext* pDC) 
{
	if(m_pFont!=NULL) pDC->SetFont(m_pFont);

	MListBoxLook::OnDraw(pListBox,pDC);
}

} // namespace mint3
*/