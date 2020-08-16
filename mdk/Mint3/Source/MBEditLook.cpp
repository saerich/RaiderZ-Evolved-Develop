/************************************************************************
MBEditLook

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : Editbox ��Ʈ���� Ŀ���ҷ�(Custom look) Ŭ���� �Դϴ�.
*************************************************************************/


#include "stdafx.h"
#include "MBEditLook.h"


// namespace : mint3
namespace mint3
{

/*	MBEditLook
	������
*/
MBEditLook::MBEditLook()
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
}


/*	~MBEditLook
	�Ҹ���
*/
MBEditLook::~MBEditLook()
{
}


/*	SetBitmap
	�����ӿ� ���� �̹����� �����ϴ� �Լ�
*/
void MBEditLook::SetBitmap( const string strFileName)
{
	m_Bitmaps[ 0].strBitmapName = strFileName;
	m_Bitmaps[ 1].strBitmapName = strFileName;
	m_Bitmaps[ 2].strBitmapName = strFileName;
}


/*	SetAssignRect
	�����ӿ� ���� ��Ʈ���� ������ �����ϴ� �Լ�
*/
void MBEditLook::SetAssignRect( int state, int index, float x1, float y1, float x2, float y2)
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
void MBEditLook::OnFrameDraw(MEdit* pEdit, MDrawContext* pDC)
{
	// ���� Ŭ���̾�Ʈ�� ������ ���մϴ�
	MRECT rect = pEdit->GetRect();
	rect.x = rect.y = 0;


	// ����Ʈ�ڽ��� ���¸� ���մϴ�
	int state;
	if ( !pEdit->IsEnable())		state = 2;
	else if ( pEdit->IsFocus())		state = 1;
	else							state = 0;


	// �������� �׸��ϴ�
	DrawBitmapByStyle( pDC, rect, MBitmapManager::Get( m_Bitmaps[ state].strBitmapName.c_str()), m_Bitmaps[ state].m_rCoord, m_BitmapStyle, m_bTiled);
}


/*	OnTextDraw
	�ؽ�Ʈ�� �׸��� �Լ�
*/
void MBEditLook::OnTextDraw( MEdit* pEdit, MDrawContext* pDC, bool bShowLanguageTab)
{
#define BUFFERSIZE	1024

	char szBuffer[BUFFERSIZE];
	_ASSERT(sizeof(szBuffer)>pEdit->GetMaxLength()+2);
	if(pEdit->GetMaxLength()+2>BUFFERSIZE) return;

	if(pEdit->IsPasswordField()==false){
		wsprintf(szBuffer, "%s", pEdit->GetText());
	}
	else{
		memset(szBuffer, '*', strlen(pEdit->GetText()));
		szBuffer[strlen(pEdit->GetText())] = '\0';
	}

	// IME Composition String
	if(pEdit->IsFocus()==true && pEdit->GetCompositionString()[0]!=NULL){
		InsertString(szBuffer, pEdit->GetCompositionString(), pEdit->GetCarretPos());
	}

	// �ؽ�Ʈ ������ ����
	int state;
	if ( !pEdit->IsEnable())		state = 2;
	else if ( pEdit->IsFocus())		state = 1;
	else							state = 0;
	pDC->SetColor( m_colorFont[ state]);

	
	// ��Ʈ ����
	pDC->SetFont( m_pFont);


	MRECT r = pEdit->GetClientRect();
	r.x-=2;

	if( bShowLanguageTab )
		r.w-=m_pFont->GetHeight();
	//	r.w-=10;

	if(pEdit->GetCarretPos()<pEdit->GetStartPos())
		pEdit->SetStartPos(pEdit->GetCarretPos());

	_ASSERT(pEdit->GetCarretPos()>=pEdit->GetStartPos());

	int nCompositionStringLength = strlen(pEdit->GetCompositionString());


	// �ؽ�Ʈ�� ���� ���ڿ� ��ġ ����
	char* szTemp = NULL;
	szTemp = szBuffer+pEdit->GetStartPos();
	// pEdit->GetCompositionString()�� ���̵� �����ؾ� StartPos ���ÿ� ���������� ����ȴ�.
	int nTestLen = pEdit->GetCarretPos()-pEdit->GetStartPos()+nCompositionStringLength;

	// Composition ���ڿ��� Edit �ʺ� �Ѿ���� StartPos�� Composition ���ڿ� ������ ���;� �Ѵ�.
	// �̷� ��� ESC ������ StartPos�� Composition ���ڿ��� ������ ���� ���ڿ��� ���ƿ��� ���������� ����Ƿ�,
	// StartPos�� ������ Composition ���ڿ��� ������ ���ڿ��� �����Ѵ�.
#define INDICATOR_WIDTH	10
	while(szTemp[0]!=NULL && r.w-INDICATOR_WIDTH < m_pFont->GetWidth(szTemp, nTestLen))
	{	// -INDICATOR_WIDTH�� IME Indicator ��ġ ����
		int nStartPos = NextPos(pEdit->GetText(), pEdit->GetStartPos());
		if(pEdit->SetStartPos(nStartPos)==false) break;
		szTemp = szBuffer+pEdit->GetStartPos();
		nTestLen = pEdit->GetCarretPos()-pEdit->GetStartPos()+nCompositionStringLength;
	}


	char* szStartText = szBuffer+pEdit->GetStartPos();	// ���� â�� ǥ�õǴ� ���ڿ��� ����

	if(pEdit->IsFocus()==true)
	{
		int nFontHeight = m_pFont->GetHeight();
		Mint* pMint = Mint::GetInstance();

		int nInsertPosInWidget = m_pFont->GetWidth(szStartText, pEdit->GetCarretPos()-pEdit->GetStartPos());
		int nCaretPosInWidget = m_pFont->GetWidth(szStartText, pEdit->GetCarretPos()+pMint->m_nCompositionCaretPosition-pEdit->GetStartPos());

		// ��µ� Candidate List ��ġ �����ϱ�
		MRECT r = pEdit->GetClientRect();
		//int nTextWidth = pFont->GetWidth(pEdit->GetText());
		MPOINT cp = MClientToScreen(pEdit, MPOINT(r.x+nInsertPosInWidget, r.y));
		pMint->SetCandidateListPosition(cp, r.h);


		// Caret
		int nSelStartPos=0;
		int nSelEndPos=0;
		// Caret
		long nCurrTime = timeGetTime();
		if((nCurrTime%(MEDIT_BLINK_TIME*2))>MEDIT_BLINK_TIME){
			//pDC->Text(r.x+nWidth-1, r.y, "|");
			//				r.x+=nCaretPosInWidget-(int)((float)pFont->GetHeight()*0.3f);
			r.x+=nCaretPosInWidget;//-(int)((float)pFont->GetHeight());

//			if (pDC->BeginFont())
			{
				pDC->Text(r, "|", MAM_LEFT);
//				pDC->EndFont();
			}
		}


		r = pEdit->GetClientRect();

		// Composition ����� ���
		MPOINT p;
		pDC->GetPositionOfAlignment(&p, r, szStartText, MAM_LEFT);
		p.x += nInsertPosInWidget;
		pMint->DrawCompositionAttributes(pDC, p, pEdit->GetCompositionString());
	}


	pDC->Text(r, szStartText, MAM_LEFT);


	// �ε������� ǥ��, �������ϰ� �ؽ�Ʈ�� ���ļ� ������ �ʰ� ���ʿ� �׷��ش�.
	if(pEdit->IsFocus()==true && 
		pEdit->GetVisibleIndicator() == true)	// �ε������� ǥ�� �ϴ°�?
	{
		if(bShowLanguageTab){
			Mint* pMint = Mint::GetInstance();
			pMint->DrawIndicator(pDC, pEdit->GetClientRect());
		}
	}
}


/*	SetStretchBlt
	��Ʈ���� �÷� ���� �� �ְ� �����ϴ� �Լ�
*/
void MBEditLook::SetStretchBlt( bool bStretch /*=true*/)
{
	// Set stretch mode
	m_BitmapStyle = bStretch ? DRAWBITMAP_STRETCH : DRAWBITMAP_NORMAL;
}


/*	GetClientRect
	Ŭ���̾�Ʈ�� ������ ���ϴ� �Լ�(�ؽ�Ʈ�� �Է��ϴ� ����)
*/
MRECT MBEditLook::GetClientRect(MEdit* pEdit, MRECT& r)
{
	if ( m_bCustomLook )
		return MRECT( r.x + 1,  r.y + 1,  r.w - 2,  r.h - 2);


	MRECT rect = r;
	rect.x += m_Bitmaps[ 0].m_rCoord[ 1].w;
	rect.y += m_Bitmaps[ 0].m_rCoord[ 1].h;
	rect.w -= m_Bitmaps[ 0].m_rCoord[ 1].w + m_Bitmaps[ 0].m_rCoord[ 3].w;
	rect.h -= m_Bitmaps[ 0].m_rCoord[ 1].h + m_Bitmaps[ 0].m_rCoord[ 6].h;


	return rect;
}


} // namespace mint3








/*****************************************************************

���⼭���ʹ� ���� �ڵ��Դϴ�


#include "stdafx.h"
#include "MBEditLook.h"
#include "MBitmapDrawer.h"

namespace mint3 {

void MBEditLook::OnFrameDraw(MEdit* pEdit, MDrawContext* pDC)
{
	MRECT r = pEdit->GetInitialClientRect();
 	if(GetCustomLook())
	{
		pDC->SetColor(MCOLOR(200,200,200,255));
		pDC->Rectangle(r);
		HLineBitmap( pDC, r.x+1, r.y+1, r.w-2, m_pFrameBitmaps[4], false );
		return;
	}
	DrawBitmapFrame9(pDC, r, m_pFrameBitmaps);
}

MBEditLook::MBEditLook(void)
{
	for(int i=0; i<9; i++){
		m_pFrameBitmaps[i] = NULL;
	}
	m_pFont=NULL;
}

MRECT MBEditLook::GetClientRect(MEdit* pEdit, MRECT& r)
{
	if( m_bCustomLook )
	{
		return MRECT(r.x+1, r.y+1, r.w-2, r.h-2);
	}
	int al = GETWIDTH(m_pFrameBitmaps[3]);
	int au = GETHEIGHT(m_pFrameBitmaps[7]);
	int ar = GETWIDTH(m_pFrameBitmaps[5]);
	int ab = GETHEIGHT(m_pFrameBitmaps[1]);
	return MRECT(r.x+al, r.y+au, r.w-(al+ar), r.h-(au+ab));
}

void MBEditLook::OnDraw(MEdit* pEdit, MDrawContext* pDC) 
{
	if(m_pFont!=NULL) pDC->SetFont(m_pFont);

    MEditLook::OnDraw(pEdit,pDC);
}

} // namespace mint3
*/