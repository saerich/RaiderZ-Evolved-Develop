/************************************************************************
MBButtonLook

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : Button ��Ʈ���� Ŀ���ҷ�(Custom look) Ŭ���� �Դϴ�.
*************************************************************************/


#include "stdafx.h"
#include "MBButtonLook.h"


// Define fade factor
const float FADE_FACTOR = 0.0045f;						// ���� Ŭ���� ���̵� �ð��� ª������



// namespace : mint3
namespace mint3
{

/*	MBButtonLook
	������
*/
MBButtonLook::MBButtonLook()
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
	m_colorFont[ MBS_NORMAL]	= MCOLOR( 200, 200, 200);
	m_colorFont[ MBS_MOUSEOVER]	= MCOLOR( 255, 255, 255);
	m_colorFont[ MBS_PRESSED]	= MCOLOR( 255, 255, 255);
	m_colorFont[ MBS_DISABLED]	= MCOLOR( 150, 150, 150);


	// �ؽ�Ʈ �׸��� ���
	m_bTextShadow = false;


	// ���� ��ư�� �ؽ�Ʈ�� �̵��ϴ� ������ ����
	m_ptTextOffset.x = 2;
	m_ptTextOffset.y = 2;


	// ���̵� �ѿ��� �ʱ�ȭ
	m_dwFadeTimer = timeGetTime();
	m_bFadeRollover = false;
}


/*	~MBButtonLook
	�Ҹ���
*/
MBButtonLook::~MBButtonLook()
{
	// Clear map list
	if ( !m_FadeMap.empty())
		m_FadeMap.clear();
}


/*	SetBitmap
	��ư�� ���� �̹����� �����ϴ� �Լ�
*/
void MBButtonLook::SetBitmap( const string strFileName)
{
	for ( int state = 0;  state < MBS_MAX;  state++)
		m_Bitmaps[ state].strBitmapName = strFileName;
}


/*	SetAssignRect
	��ư�� ���� ��Ʈ���� ������ �����ϴ� �Լ�
*/
void MBButtonLook::SetAssignRect( int state, int index, float x1, float y1, float x2, float y2)
{
	// Check valid range
	if ( (state >= MBS_MAX) || (index >= 9))
		return;


	// Set bitmap assignment
	m_Bitmaps[ state].m_rCoord[ index].x = (int)x1;
	m_Bitmaps[ state].m_rCoord[ index].y = (int)y1;
	m_Bitmaps[ state].m_rCoord[ index].w = (int)x2;
	m_Bitmaps[ state].m_rCoord[ index].h = (int)y2;
}


/*	OnUpDraw
	�Ϲ� ������ ��ư�� �׸��� �Լ�
*/
void MBButtonLook::OnUpDraw( MButton* pButton, MDrawContext* pDC)
{
	// ���̵� �ѿ��� ��带 ������� ������ �ش� ��ư�� �׸��� �ٷ� ������
	if ( !m_bFadeRollover)
	{
		DrawButton( pButton, pDC, MBS_NORMAL);
		DrawText( pButton, pDC, MBS_NORMAL);

		return;
	}


	// ���̵� �ѿ��� ����� ����ؼ� ��ư�� �׸���


	// ��ư�� ���̵� ���� �����´�
	float fFade = GetFadeValue( pButton, MBS_NORMAL);


	// ���̵带 ���� �ʾƵ� �Ǵ� ��Ȳ�̸� ��ư�� �׸��� �ٷ� ������
	if ( fFade < 0.0001f)
	{
		// ��ư�� �׸���
		DrawButton( pButton, pDC, MBS_NORMAL);
		DrawText( pButton, pDC, MBS_NORMAL);

		return;
	}


	// ���� ������ �����Ѵ�
	float fOpacity = pDC->GetOpacity();


	// ���콺�� �ö�� ������ ��ư�� �׸���
	DrawButton( pButton, pDC, MBS_MOUSEOVER);


	// �Ϲ� ������ ��ư�� �׸���
	pDC->SetOpacity( fOpacity * (1.0f - fFade));
	DrawButton( pButton, pDC, MBS_NORMAL);


	// ������ �������� �ǵ�����
	pDC->SetOpacity( fOpacity);


	// �ؽ�Ʈ�� �׸���
	DrawText( pButton, pDC, MBS_NORMAL);
}


/*	OnOverDraw
	���콺�� �ö�� ��ư�� �׸��� �Լ�
*/
void MBButtonLook::OnOverDraw( MButton* pButton, MDrawContext* pDC)
{
	// ���̵� �ѿ��� ��带 ������� ������ �ش� ��ư�� �׸��� �ٷ� ������
	if ( !m_bFadeRollover)
	{
		// ��ư�� �׸���
		DrawButton( pButton, pDC, MBS_MOUSEOVER);
		DrawText( pButton, pDC, MBS_MOUSEOVER);

		return;
	}

	
	// ���̵� �ѿ��� ����� ����ؼ� ��ư�� �׸���


	// ��ư�� ���̵� ���� �����´�
	float fFade = GetFadeValue( pButton, MBS_MOUSEOVER);


	// ���̵带 ���� �ʾƵ� �Ǵ� ��Ȳ�̸� ��ư�� �׸��� �ٷ� ������
	if ( fFade > 0.999f)
	{
		DrawButton( pButton, pDC, MBS_MOUSEOVER);
		DrawText( pButton, pDC, MBS_MOUSEOVER);

		return;
	}


	// ���� ������ �����Ѵ�
	float fOpacity = pDC->GetOpacity();


	// �Ϲ� ������ ��ư�� �׸���
	DrawButton( pButton, pDC, MBS_NORMAL);


	// ���콺�� �ö�� ������ ��ư�� �׸���
	pDC->SetOpacity( fOpacity * fFade);
	DrawButton( pButton, pDC, MBS_MOUSEOVER);


	// ������ �������� �ǵ�����
	pDC->SetOpacity( fOpacity);


	// �ؽ�Ʈ�� �׸���
	DrawText( pButton, pDC, MBS_MOUSEOVER);
}


/*	OnDownDraw
	���� ��ư�� �׸��� �Լ�
*/
void MBButtonLook::OnDownDraw( MButton* pButton, MDrawContext* pDC)
{
	// ��ư�� ���̵� ���� �����Ѵ�
	GetFadeValue( pButton, MBS_PRESSED);


	// ��ư�� �׸���
	DrawButton( pButton, pDC, MBS_PRESSED);


	// �ؽ�Ʈ�� �׸���
	DrawText( pButton, pDC, MBS_PRESSED);
}


/*	OnDisableDraw
	����� �� ���� ��ư�� �׸��� �Լ�
*/
void MBButtonLook::OnDisableDraw( MButton* pButton, MDrawContext* pDC)
{
	// ��ư�� ���̵� ���� �����Ѵ�
	GetFadeValue( pButton, MBS_DISABLED);


	// ��ư�� �׸���
	DrawButton( pButton, pDC, MBS_DISABLED);


	// �ؽ�Ʈ�� �׸���
	DrawText( pButton, pDC, MBS_DISABLED);
}


/*	DrawButton
	������ ��ư�� �׸��� �Լ�
*/
void MBButtonLook::DrawButton( MButton* pButton, MDrawContext* pDC, MBS_STATE state)
{
	// ��ư�� �׸� ������ ���մϴ�
	MRECT rect = pButton->GetClientRect();


	// üũ ��ư�� ��쿣 ������ �����մϴ�
	if ( pButton->GetType() == MBT_CHECKBTN)
	{
//		// üũ�ڽ��� ũ��� ��Ʈ�� ���̷� ���մϴ�
//		int height = m_pFont->GetHeight();

		int height = rect.h;


		// ������ �ٽ� �����մϴ�
		rect.x = 0;
		rect.y = (rect.h - height) / 2;
		rect.w = height;
		rect.h = height;
	}


	// ��ư�� �׸��ϴ�
	DrawBitmapByStyle( pDC, rect, MBitmapManager::Get( m_Bitmaps[ state].strBitmapName.c_str()), m_Bitmaps[ state].m_rCoord, m_BitmapStyle, m_bTiled);
}


/*	DrawText
	��ư�� �ؽ�Ʈ�� �׸��� �Լ�
*/
void MBButtonLook::DrawText( MButton* pButton, MDrawContext* pDC, MBS_STATE state)
{
	// �ؽ�Ʈ�� ����� ������ ���մϴ�
	MRECT rect = pButton->GetClientRect();


	// client edge
#ifdef _DEBUG
	// �ؽ�Ʈ�� "show rect"�̸� ������ ������ ǥ���Ѵ�
	if ( strcmp( pButton->GetText(), "show rect") == 0)
	{
		pDC->SetColor( 255, 255, 255, (int)(25.0 * sin( (double)timeGetTime() * 0.005) + 25.0));
		pDC->Rectangle( rect);
	}
#endif


	// ��Ʈ ����
	pDC->SetFont( m_pFont);


	// �ؽ�Ʈ�� �׸��ϴ�
//	if ( pDC->BeginFont())
	{
		// �޺� �ڽ� ��ư�� ��쿣 ���� ���̿��� ���� ��ŭ(��� ��ư ����)�� ���ݴϴ�
		if ( pButton->IsComboButton())
			rect.w -= rect.h;



		// �ؽ�Ʈ ���İ��� ���մϴ�
		MAlignmentMode am = pButton->GetAlignment();

		// �ؽ�Ʈ �������� ���մϴ�
		MCOLOR color = m_colorFont[ state];



		// ��ư ���°� üũ ��ư�� ��쿣...
		if ( pButton->GetType() == MBT_CHECKBTN)
		{
//			// �ؽ�Ʈ�� ��ġ�� üũ�ڽ� ũ�⸸ŭ ���������� �̵���ŵ�ϴ�
//			rect.x += m_pFont->GetHeight() + 3;
			rect.x += rect.h + 2;


			// �ؽ�Ʈ ���� ����
			color = (pButton->IsMouseOver()) ? m_colorFont[ MBS_MOUSEOVER] : m_colorFont[ MBS_NORMAL];


			// �ؽ�Ʈ�� ���� ���Ľ�ŵ�ϴ�
			am = MAM_LEFT;
		}

		// �Ϲ� ��ư�� ��쿡 �������� �����մϴ�
		else if ( state == MBS_PRESSED)
		{
			rect.x += m_ptTextOffset.x;
			rect.y += m_ptTextOffset.y;
		}


		// ��Ʈ�� �ܰ����� ������ �ܰ��� �β���ŭ ���ݴϴ�
		MFont* pFont = pDC->GetFont();
		if ( pFont)
		{
			if ( pFont->IsOutline())
			{
				rect.x -= 1;
				rect.y -= 1;
			}
		}


		// �׸��� �ؽ�Ʈ�� ����մϴ�
		if ( m_bTextShadow)
		{
			rect.x += 1;
			rect.y += 1;

			pDC->SetColor( MCOLOR( 0, 0, 0, 128));
			pDC->Text( rect, pButton->GetText(), am);

			rect.x -= 1;
			rect.y -= 1;
		}


		// �ؽ�Ʈ�� ����մϴ�
		pDC->SetColor( color);
		pDC->Text( rect, pButton->GetText(), am);
//		pDC->EndFont();
	}
}


/*	SetStretchBlt
	��Ʈ���� �÷� ���� �� �ְ� �����ϴ� �Լ�
*/
void MBButtonLook::SetStretchBlt( bool bStretch /*=true*/)
{
	// Set stretch mode
	m_BitmapStyle = bStretch ? DRAWBITMAP_STRETCH : DRAWBITMAP_NORMAL;
}


/*	SetFadeRollover
	���̵� �ѿ��� ȿ���� ����ϵ��� �����ϴ� �Լ�
*/
void MBButtonLook::SetFadeRollover( bool bFade /*=true*/)
{
	m_bFadeRollover = bFade;


	// ���̵� �ѿ����� ������� ������ �� �ʱ�ȭ
	if ( bFade == false)
	{
		if ( !m_FadeMap.empty())
			m_FadeMap.clear();
	}

	// ����ϸ� Ÿ�̸� �ʱ�ȭ
	else
		m_dwFadeTimer = timeGetTime();
}


/*	GetFadeValue
	��ư�� ���̵� ���� ���ϴ� �Լ�
*/
float MBButtonLook::GetFadeValue( MButton* pButton, MBS_STATE state)
{
	// Find button info
	map<MButton*,float>::iterator find = m_FadeMap.find( pButton);

	// If not exist
	if ( find == m_FadeMap.end())
	{
		// Register a new button node
		m_FadeMap.insert( map<MButton*,float>::value_type( pButton, 1.0f));

		find = m_FadeMap.find( pButton);
	}


	// ���̵� ���� ���� ������ ������
	switch ( state)
	{
		// ������ ����� ������.  1.0 ~ 2.0�� ����...
		case MBS_NORMAL :
			if ( (*find).second < 0.0f)
				(*find).second *= -1.0f;
			break;

		// ����� ������ ������.  -2.0 ~ -1.0�� ����...
		case MBS_MOUSEOVER :
			if ( (*find).second > 0.0f)
				(*find).second *= -1.0f;
			break;

		case MBS_PRESSED :
			(*find).second = -2.0f;
			break;

		case MBS_DISABLED :
			(*find).second = 1.0f;
			break;

		default:
			break;
	}


	// Set new fade value
	if ( m_bFadeRollover)
	{
		DWORD curr = timeGetTime();
		DWORD diff = curr - m_dwFadeTimer;

		if ( diff > 10)
		{
			// Get face value
			float fFade = (float)diff * FADE_FACTOR;


			// Set fade to all button
			for ( map<MButton*,float>::iterator itr = m_FadeMap.begin();  itr != m_FadeMap.end();  itr++)
			{
				// ����� ��쿣 Normal�����̹Ƿ�.... (diff�� �ʹ� Ŀ���� ��� �� ������
				// �� �� �����Ƿ� �� ������ �ѷ� ������)
				if ( (*itr).second > 0.0f)
				{
					// fade factor
					(*itr).second -= fFade;

					// Limited range
					if ( (*itr).second < 1.0f)
						(*itr).second = 1.0f;
				}

				// ������ ��쿣 Mouse over �����̹Ƿ�....
				else
				{
					// fade factor
					(*itr).second -= fFade;

					// Limited range
					if ( (*itr).second < -2.0f)
						(*itr).second = -2.0f;
				}
			}


			// Set timer
			m_dwFadeTimer = curr;
		}
	}


	// Get fade value
	float fFade = 0.0f;
	switch ( state)
	{
		case MBS_NORMAL :
			fFade = (*find).second - 1.0f;
			break;

		case MBS_MOUSEOVER :
			fFade = -(*find).second - 1.0f;
			break;

		case MBS_PRESSED :
			fFade = -2.0f;
			break;

		case MBS_DISABLED :
			fFade = 1.0f;
			break;

		default:
			break;
	}


	return fFade;
}


} // namespace mint3








/**************************************************************************************************

   ���⼭���ʹ� ���� �ڵ��Դϴ�.



MBButtonLook::MBButtonLook(void) : MScalableLook()
{
	for(int i=0; i<9; i++){
		m_pUpBitmaps[i] = NULL;
		m_pDownBitmaps[i] = NULL;
		m_pOverBitmaps[i] = NULL;
	}
	for(int i=0; i<4; i++){
		m_pFocusBitmaps[i] = NULL;
	}

	m_pFont = NULL;
	m_FontColor = MColorTable::MBUTTON_TEXT;
	m_FontDownColor = MCOLOR(DEFCOLOR_LIGHT);
	m_FontDisableColor = MCOLOR(MColorTable::DISABLE);
	m_FontHighlightColor = MCOLOR(MColorTable::PEN_HIGHLIGHT);
	m_bStretch = true;
	m_bCustomLook = false;

	m_FontDownOffset = MPOINT(0,0);
}

void MBButtonLook::DrawText(MButton* pButton, MDrawContext* pDC, MRECT& r, const char* szText, MAlignmentMode a)
{
	if(pButton->m_pIcon!=NULL){
		pDC->SetBitmap(pButton->m_pIcon);
		pDC->Draw(r.x, r.y+(r.h-pButton->m_pIcon->GetHeight())/2);

		r.x+=pButton->m_pIcon->GetWidth();
		r.w-=pButton->m_pIcon->GetWidth();
	}

	if(pButton->IsButtonDown())
	{
		r.x+=(int)(m_FontDownOffset.x * GetScale());
		r.y+=(int)(m_FontDownOffset.y * GetScale());
	}


//	if(m_pFont!=NULL)
//		pDC->SetFont(m_pFont);
//
//	if(pButton->IsEnable()==true){
//		if(pButton->IsButtonDown()==true) pDC->SetColor(m_FontDownColor);
//		else if(pButton->IsMouseOver()) pDC->SetColor(m_FontDownColor);
//		else if(pButton->GetType() == MBT_PUSHBTN && pButton->GetCheck()) pDC->SetColor(m_FontDownColor);
//		else pDC->SetColor(m_FontColor);
//	}
//	else pDC->SetColor(m_FontDisableColor);
//	if(pButton->m_bHighlight==true){
//		MCOLOR PrevHCol = pDC->SetHighlightColor(m_FontHighlightColor);
//		pDC->TextWithHighlight(r, szText, a);
//		pDC->SetHighlightColor(PrevHCol);
//	}
//	else{
//		pDC->Text(r, szText, a);
//	}


	// �ӽ÷� �̷��� ����մϴ�
	if (pDC->BeginFont())
	{
		if(pButton->m_bHighlight==true){
			MCOLOR PrevHCol = pDC->SetHighlightColor(MColorTable::PEN_HIGHLIGHT);
			pDC->TextWithHighlight(r, pButton->m_szText, pButton->GetAlignment());
			pDC->SetHighlightColor(PrevHCol);
		}
		else{
			pDC->Text(r, pButton->m_szText, pButton->GetAlignment());
		}
		pDC->EndFont();
	}
}

void MBButtonLook::DrawFocus(MDrawContext* pDC, MRECT& r)
{
	if(m_pFocusBitmaps[0]==NULL) return;

	pDC->SetBitmap(m_pFocusBitmaps[0]);
	pDC->Draw(r.x-2, r.y-2);
	pDC->SetBitmap(m_pFocusBitmaps[1]);
	pDC->Draw(r.x+r.w-m_pFocusBitmaps[1]->GetWidth()+2, r.y-2);
	pDC->SetBitmap(m_pFocusBitmaps[2]);
	pDC->Draw(r.x-2, r.y+r.h-m_pFocusBitmaps[1]->GetHeight()+2);
	pDC->SetBitmap(m_pFocusBitmaps[3]);
	pDC->Draw(r.x+r.w-m_pFocusBitmaps[1]->GetWidth()+2, r.y+r.h-m_pFocusBitmaps[1]->GetHeight()+2);
}

void MBButtonLook::OnDownDraw(MButton* pButton, MDrawContext* pDC)
{
	MRECT r = pButton->GetInitialClientRect();
	if( GetCustomLook() )
	{
//		DrawBitmapButtonCustom1( pDC, r, m_pDownBitmaps, true, m_bStretch );
//		r = pButton->GetClientRect();
//		r.x += 1;
//		r.y += 1;
//		r.w += 1;
//		r.h += 1;

		// madduck [4/5/2006]
		int stat = CONTROL_STATE_PRESSED;
		MDRAWBITMAPSTYLE eDrawStyle = DRAWBITMAP_NORMAL;
		if(m_bStretch)
			eDrawStyle = DRAWBITMAP_STRETCH;
		DrawBitmapByStyle(pDC, pButton->m_Rect, MBitmapManager::Get(m_StateBitmaps[stat].strBitmapName.c_str()),
			m_StateBitmaps[stat].m_rCoord, eDrawStyle);
	}
	else
	{
		DrawBitmapFrame9(pDC, r, m_pDownBitmaps, m_bStretch, GetScale() );
		r = pButton->GetClientRect();
	}
	if(pButton->IsFocus()==true) DrawFocus(pDC, r);
    
	DrawText(pButton, pDC, r, pButton->m_szText, pButton->GetAlignment());
}

void MBButtonLook::OnUpDraw(MButton* pButton, MDrawContext* pDC)
{
	if( pButton->GetType() == MBT_PUSHBTN  && pButton->GetCheck() ) 
	{
		OnDownDraw( pButton, pDC );
		return;
	}

	MRECT r = pButton->GetInitialClientRect();
	if( GetCustomLook() )
	{
//		DrawBitmapButtonCustom1( pDC, r, m_pUpBitmaps, false, m_bStretch );

		// madduck [4/5/2006]
		int stat = CONTROL_STATE_NORMAL;
		MDRAWBITMAPSTYLE eDrawStyle = DRAWBITMAP_NORMAL;
		if(m_bStretch)
			eDrawStyle = DRAWBITMAP_STRETCH;

		
		DrawBitmapByStyle(pDC, pButton->m_Rect, MBitmapManager::Get(m_StateBitmaps[stat].strBitmapName.c_str()),
			m_StateBitmaps[stat].m_rCoord, eDrawStyle);
	}
	else
		DrawBitmapFrame9(pDC, r, m_pUpBitmaps, m_bStretch, GetScale() );
	if(pButton->IsFocus()==true) DrawFocus(pDC, r);

	DrawText(pButton, pDC, pButton->GetClientRect(), pButton->m_szText, pButton->GetAlignment());
}

void MBButtonLook::OnOverDraw(MButton* pButton, MDrawContext* pDC)
{
	if( pButton->GetType() == MBT_PUSHBTN )
	{
		if( pButton->GetCheck() ) OnDownDraw( pButton, pDC );
		else OnUpDraw(pButton, pDC );
		return;
	}
	
	MRECT r = pButton->GetInitialClientRect();
	if( GetCustomLook() )
	{
//		DrawBitmapButtonCustom1( pDC, r, m_pOverBitmaps, false, m_bStretch );

		// madduck [4/5/2006]
		int stat = CONTROL_STATE_MOUSEOVER;
		MDRAWBITMAPSTYLE eDrawStyle = DRAWBITMAP_NORMAL;
		if(m_bStretch)
			eDrawStyle = DRAWBITMAP_STRETCH;

		DrawBitmapByStyle(pDC, pButton->m_Rect, MBitmapManager::Get(m_StateBitmaps[stat].strBitmapName.c_str()),
			m_StateBitmaps[stat].m_rCoord, eDrawStyle);
	}
	else
		DrawBitmapFrame9(pDC, r, m_pOverBitmaps, m_bStretch, GetScale());
	if(pButton->IsFocus()==true) DrawFocus(pDC, r);

	DrawText(pButton, pDC, pButton->GetClientRect(), pButton->m_szText, pButton->GetAlignment());
}

void MBButtonLook::OnDisableDraw(MButton* pButton, MDrawContext* pDC)
{
	OnUpDraw(pButton, pDC);
}

void MBButtonLook::OnDraw( MButton* pButton, MDrawContext* pDC )
{
	if(GetWireLook())		// ������ �����ʿ� �ٴ� ����� ��쿡...
	{
		MRECT rect = pButton->GetInitialClientRect();

		// ��� �׸���...
		pDC->SetColor(19,19,19,255);
		pDC->FillRectangle( rect);


		// ��ư�� �׷��� ũ�⸦ ���Ѵ�.
		MRECT rectButtonBmp;
		rectButtonBmp.x = rect.x + rect.w - rect.h + 1;		// ��ư�� �׷��� ���� ��ġ�� ���Ѵ�.
        rectButtonBmp.y = rect.y + 1;						// �׵θ� �β�(1 pixel)������ 1 pixel �Ʒ��� ������ �׸���.
		rectButtonBmp.w = rect.h - 2;						// ������ ���̸� �������� ���� ���̸� �����.
		rectButtonBmp.h = rect.h - 2;						// ������ ���̸� �������� ���� ���̸� �����.
		float fScale = (float)(rect.h - 2) / (float)m_pDownBitmaps[1]->GetHeight();

		// ��ư�� ��Ʈ���� �׸���
		if( pButton->GetComboDropped() )
		{
			HLineBitmap( pDC, rectButtonBmp.x, rectButtonBmp.y, rectButtonBmp.w, m_pDownBitmaps[2], false, fScale);
		}
		else
		{
			if( pButton->IsFocus() )
				HLineBitmap( pDC, rectButtonBmp.x, rectButtonBmp.y, rectButtonBmp.w, m_pDownBitmaps[0], false, fScale);
			else
				HLineBitmap( pDC, rectButtonBmp.x, rectButtonBmp.y, rectButtonBmp.w, m_pDownBitmaps[1], false, fScale);
		}
		
		MRECT rectText = rect;
		rectText.w -= rect.h;
		pDC->SetColor(205,205,205,255);
	 	pDC->Text(rectText, pButton->m_szText, pButton->GetAlignment() );

		// �׵θ� �׸���...
		pDC->SetColor(205,205,205,255);
		pDC->Rectangle( rect);

	}
	else
		MButtonLook::OnDraw( pButton, pDC );
}

void MBButtonLook::OnCheckBoxDraw( MButton* pButton, MDrawContext* pDC, bool bPushed )
{
	// üũ�ڽ� �׸���
	MRECT r = pButton->GetInitialClientRect();
	int x = r.x + (int)CHECKBOX_SIZE;	// �ణ�� ������
	int y = r.y + (int)(r.h*0.5) -(int)(CHECKBOX_SIZE*0.5);
	pDC->SetColor( 128, 128, 128, 255 );
    pDC->Rectangle( x, y, (int)CHECKBOX_SIZE, (int)CHECKBOX_SIZE );
	//üũ��ư �׸���
    if(bPushed)
	{
		pDC->SetBitmap(m_pUpBitmaps[0]);
		pDC->Draw(x-(int)(CHECKBOX_SIZE*0.5), y-(int)(CHECKBOX_SIZE*0.5));
	}
	// �۾�����
	r = pButton->GetClientRect();
//	r.x += 2*CHECKBOX_SIZE;
	r.x += (int)CHECKBOX_SIZE;			// ��ȯ�̰� ����
	DrawText( pButton, pDC, r, pButton->m_szText, pButton->GetAlignment());
}

MRECT MBButtonLook::GetClientRect(MButton* pButton, MRECT& r)
{
	float fScale = GetScale();

	int al = (int)(fScale * GETWIDTH(m_pUpBitmaps[3]));
	int au = (int)(fScale * GETHEIGHT(m_pUpBitmaps[7]));
	int ar = (int)(fScale * GETWIDTH(m_pUpBitmaps[5]));
	int ab = (int)(fScale * GETHEIGHT(m_pUpBitmaps[1]));
	return MRECT(r.x+al, r.y+au, r.w-(al+ar), r.h-(au+ab));
}
*/
