#include "stdafx.h"
#include "MDrawContext.h"
#include "MColorTable.h"
#include "MWidget.h"
#include <crtdbg.h>
#include "Mint.h"
#include <math.h>
#include "MDebug.h"

namespace mint3 {

// MDrawContex Implementation
/////////////////////////////
MDrawContext::MDrawContext(void)
: m_Color(0, 255, 0), m_HighlightColor(255, 255, 0)
{
#ifdef _DEBUG
	m_nTypeID = MINT_BASE_CLASS_TYPE;
#endif
	m_Color = 0x00000000;
	m_BitmapColor = 0xFFFFFFFF;
	m_ColorKey = MCOLOR(0xFF, 0, 0xFF);
	m_pBitmap = 0;
	m_pFont = 0;
	m_Clip.x = 0;
	m_Clip.y = 0;
	m_Clip.w = 	MGetWorkspaceWidth();
	m_Clip.h = MGetWorkspaceHeight();
	m_Origin.x = 0;
	m_Origin.y = 0;
	m_fOpacity = 1.0f;
	m_Effect = MDE_NORMAL;
}

MDrawContext::~MDrawContext()
{
}

MCOLOR MDrawContext::SetBitmapColor( MCOLOR& color )
{
	m_BitmapColor = color;
	return m_BitmapColor;
}
MCOLOR MDrawContext::SetBitmapColor( unsigned char r, unsigned char g, unsigned char b, unsigned char a/* =255 */ )
{
	return SetBitmapColor( MCOLOR(r,g,b,a));
}

MCOLOR MDrawContext::GetBitmapColor() 
{
	return m_BitmapColor;
}

MCOLOR MDrawContext::SetColor(MCOLOR& color)
{
	MCOLOR temp = m_Color;
	if (m_fOpacity != 1.0f)
	{
		unsigned char a = (unsigned char)(color.a * m_fOpacity);
		m_Color = MCOLOR(color.r, color.g, color.b, a);
	}
	else m_Color = color;
	return temp;
}

MCOLOR MDrawContext::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	if (m_fOpacity != 1.0f) a = (unsigned char)(a * m_fOpacity);
	return SetColor(MCOLOR(r, g, b, a));
}

MCOLOR MDrawContext::GetColor(void)
{
	return m_Color;
}

MCOLOR MDrawContext::SetHighlightColor(MCOLOR& color)
{
	MCOLOR temp = m_HighlightColor;
	if (m_fOpacity != 1.0f)
	{
		unsigned char a = (unsigned char)(color.a * m_fOpacity);
		m_HighlightColor = MCOLOR(color.r, color.g, color.b, a);
	}
	else m_HighlightColor = color;
	return temp;
}

MCOLOR MDrawContext::SetHighlightColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	if (m_fOpacity != 1.0f) a = (unsigned char)(a * m_fOpacity);
	return SetHighlightColor(MCOLOR(r, g, b, a));
}

MCOLOR MDrawContext::GetHighlightColor(void)
{
	return m_HighlightColor;
}

MCOLOR MDrawContext::SetColorKey(MCOLOR& color)
{
	MCOLOR temp = m_ColorKey;
	m_ColorKey = color;
	return temp;
}

MCOLOR MDrawContext::SetColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return SetColorKey(MCOLOR(r, g, b, a));
}

MCOLOR MDrawContext::GetColorKey(void)
{
	return m_ColorKey;
}

MBitmap* MDrawContext::SetBitmap(MBitmap* pBitmap)
{
	if ( m_pBitmap == pBitmap)
		return NULL;

	/*
#ifdef _DEBUG
	// ���� ������ ������Ʈ�̿��� �Ѵ�.
	if(pBitmap!=NULL) _ASSERT(m_nTypeID==pBitmap->m_nTypeID);
#endif
	*/

	MBitmap* temp = m_pBitmap;
	m_pBitmap = pBitmap;
	return temp;
}

MFont* MDrawContext::SetFont(MFont* pFont)
{
	if ( m_pFont == pFont)
		return NULL;

#ifdef _DEBUG
	// ���� ������ ������Ʈ�̿��� �Ѵ�.
	if(pFont!=NULL) _ASSERT(m_nTypeID==pFont->m_nTypeID);
#endif

	MFont* temp = m_pFont;
	m_pFont = pFont;
	if(m_pFont==NULL){
		m_pFont = MFontManager::Get(0);	// Default Font
	}
	return temp;
}

MFont* MDrawContext::SetFont( const char* szFontName )
{
	MFont* pFont = MFontManager::Get( szFontName );
	return SetFont(pFont);
}

void MDrawContext::SetClipRect(int x, int y, int w, int h)
{
	SetClipRect(MRECT(x, y, w, h));
}

void MDrawContext::SetClipRect(MRECT &r)
{
	SetOrigin( r.x, r.y);

	memcpy(&m_Clip, &r, sizeof(MRECT));

	// Screen Coordinate Clipping
//	if(m_Clip.x<0) m_Clip.x = 0;
//	if(m_Clip.y<0) m_Clip.y = 0;
//	if(m_Clip.x+m_Clip.w>MGetWorkspaceWidth()) m_Clip.w = max(MGetWorkspaceWidth()-m_Clip.x, 0);
//	if(m_Clip.y+m_Clip.h>MGetWorkspaceHeight()) m_Clip.h = max(MGetWorkspaceHeight()-m_Clip.y, 0);
}

void MDrawContext::Rectangle(int x, int y, int cx, int cy)
{
	HLine(x, y, cx);
	HLine(x, y+cy, cx);
	VLine(x, y, cy);
	VLine(x+cx, y, cy);
}

void MDrawContext::Rectangle(MRECT& r)
{
	Rectangle(r.x, r.y, r.w, r.h);
}

void MDrawContext::SetOrigin(int x, int y)
{
	m_Origin.x = x;
	m_Origin.y = y;
}

void MDrawContext::SetOrigin(MPOINT& p)
{
	m_Origin = p;
}

MPOINT MDrawContext::GetOrigin(void)
{
	return m_Origin;
}

void MDrawContext::SetEffect(MDrawEffect effect)
{
	m_Effect = effect;
}

void MDrawContext::FillRectangle(MRECT& r)
{
	FillRectangle(r.x, r.y, r.w, r.h);
}

static int comp( const void *a, const void *b)
{
	return *(int*)a - *(int*)b;
}

// �ȼ� ������ ��ٺ��� �� ����
void MDrawContext::FillPoly( MPOINT* pPoly, int nCount)
{
	int x, y, i, j;

	if ( (pPoly == NULL) || (nCount == 0))
		return;

	int sx = pPoly[ 0].x,  sy = pPoly[ 0].y,  ex = pPoly[ 0].x,  ey = pPoly[ 0].y;
	for ( i = 1;  i < nCount;  i++)
	{
		sx = min( pPoly[ i].x, sx);
		sy = min( pPoly[ i].y, sy);
		ex = max( pPoly[ i].x, ex);
		ey = max( pPoly[ i].y, ey);
	}

	int *node_x = new int[ nCount];
	for ( y = sy;  y < ey;  y++)
	{
		int nodes = 0;
		for ( i = 0, j = nCount-1;  i<nCount;  j = i++)
		{
			if ( (pPoly[ i].y < y) && (pPoly[ j].y >= y) || (pPoly[ j].y < y) && (pPoly[ i].y >= y))
			{
				node_x[ nodes++] = (int)(pPoly[ i].x + double( y - pPoly[ i].y) *
					double( pPoly[ j].x - pPoly[ i].x) / double( pPoly[ j].y - pPoly[ i].y) + 0.5f);
			}
		}

		qsort( node_x, nodes, sizeof( int), comp);

		for ( i = 0;  i < nodes;  i += 2)
		{
			if ( (node_x[ i] < sx) || (node_x[ i] >= ex))
				break;

			if ( node_x[ i + 1] > 0)
			{
				if ( node_x[ i] < 0)
					node_x[ i] = 0;
				if ( node_x[ i + 1] > ex)
					node_x[ i + 1] = ex;
				for ( x = node_x[ i];  x < node_x[ i + 1];  x++)
					SetPixel( x + m_Clip.x, y + m_Clip.y, m_Color);
			}
		}
	}

	delete [] node_x;
}

/*
int GetStrLength(MFont* pFont, char* szText, int nSize)
{
	char temp[256];
	memcpy(temp, szText, nSize);
	temp[nSize] = 0;

	return pFont->GetWidth(temp);
}

int GetStrLengthOfWidth(MFont* pFont, int w, char* szText)
{
	int tw = pFont->GetWidth(szText);
	float fw = w / (float)tw;
	int nStrLen = strlen(szText);
	int nRecStrLen = int(nStrLen * fw);

	int nRecWidth = GetStrLength(pFont, szText, nRecStrLen);

	if(nRecWidth>w){	// ��ġ�� ���
		do{
			nRecStrLen--;
			nRecWidth = GetStrLength(pFont, szText, nRecStrLen);
		}while(nRecWidth<w || nRecStrLen==0);
		return nRecStrLen;
	}
	else if(nRecWidth<w){
		do{
			nRecStrLen++;
			nRecWidth = GetStrLength(pFont, szText, nRecStrLen);
		}while(nRecWidth>=w || nRecStrLen==nStrLen);
		return nRecStrLen-1;
	}

	return nRecStrLen;
}
*/

int MDrawContext::Text(int x, int y, const char* szText, int nLength)
{
//	TIMEPROFILE_THIS_FUNCTION;

	SetState(MDS_FONT);

	MFont* pFont = m_pFont;

	if(m_pFont==NULL) 
		pFont = MFontManager::Get(NULL);

	x += m_Origin.x;
	y += m_Origin.y;

	DWORD dwColor = m_Color.GetARGB();
	return pFont->DrawText(x, y, szText, nLength, dwColor);
}

int MDrawContext::TextWithHighlight(int x, int y, const char* szText)
{
	char szFront[MWIDGET_TEXT_LENGTH];// = {0, };
	char szBack[MWIDGET_TEXT_LENGTH];// = {0, };
	char cHighlight;

	int nPos = RemoveAnd(szFront, &cHighlight, szBack, szText);
	if(nPos==-1){	// Highlight(Underline) ���ڰ� ���� ���
		return Text(x, y, szText);
	}
	else{
		if(m_pFont!=NULL){
			char temp[2] = {cHighlight, 0};
			int nFrontWidth = m_pFont->GetWidth(szFront);
			int nHighlightWidth = m_pFont->GetWidth(temp);
			//int nBackWidth = m_pFont->GetWidth(szBack);
			Text(x, y, szFront);
			MCOLOR tmpColor = m_Color;
			SetColor(m_HighlightColor);
			Text(x+nFrontWidth, y, temp);
			SetColor(tmpColor);
			return Text(x+nFrontWidth+nHighlightWidth, y, szBack);
		}
		else{
			return Text(x, y, szText);		// Font�� �ε���� �ʾ��� ��� &�� �ִ� ���� �״�� ǥ��
		}
	}
}

void MDrawContext::GetPositionOfAlignment(MPOINT* p, MRECT& r, const char* szText, MAlignmentMode am, bool bAndInclude)
{
	if(m_pFont!=NULL){
		int w;
		if(bAndInclude==true) w = m_pFont->GetWidth(szText);
		else w = m_pFont->GetWidthWithoutAmpersand(szText);
		int h = m_pFont->GetHeight();

#define DEFAULT_ALIGN_MARGIN	2	// �¿� ���Ľ� �¿쿡 ������ �ּ��� ����
		p->x = r.x;
		p->y = r.y;
		if(am&MAM_HCENTER){
			p->x = r.x + (r.w-w) / 2;
		}
		else if(am&MAM_RIGHT){
			p->x = r.x + (r.w-w-DEFAULT_ALIGN_MARGIN);
			//			p->y = r.y + (r.h-h) / 2;
		}
		else /* if (am&MAM_LEFT) */ {
			p->x = r.x + DEFAULT_ALIGN_MARGIN;
//			p->y = r.y + (r.h-h) / 2;
		}

		if(am&MAM_TOP){
			p->y = r.y + DEFAULT_ALIGN_MARGIN;
		}
		else if(am&MAM_BOTTOM){
			p->y = r.y + (r.h-h-DEFAULT_ALIGN_MARGIN);
		}
		else /* if (am&MAM_VCENTER) */ {
			p->y = r.y + (r.h-h) / 2;
		}

		if(am&MAM_EDIT){
			if(w+DEFAULT_ALIGN_MARGIN<r.w) p->x = r.x + DEFAULT_ALIGN_MARGIN;
			else p->x = r.x + (r.w-w);
			p->y = r.y + (r.h-h) / 2;
		}
	}
}

int MDrawContext::Text(MRECT& r, const char* szText, MAlignmentMode am)
{
	MPOINT p;
	GetPositionOfAlignment(&p, r, szText, am);

	int nLength = GetFont()->GetVisibleLength(r.w,szText);

//	return Text(p.x, p.y, szText);
	return Text(p.x, p.y, szText,nLength);
}

int MDrawContext::TextWithHighlight(MRECT& r, const char* szText, MAlignmentMode am)
{
	char szFront[MWIDGET_TEXT_LENGTH];// = {0, };
	char szBack[MWIDGET_TEXT_LENGTH];// = {0, };
	char cHighlight;

	int nPos = RemoveAnd(szFront, &cHighlight, szBack, szText);
	if(nPos==-1){	// Highlight(Underline) ���ڰ� ���� ���
		TextMC(r, szText, am);
		return 1;
	}
	else{
		MPOINT p;
		char szTextWithoutAnd[MWIDGET_TEXT_LENGTH];
		RemoveAnd(szTextWithoutAnd, szText);
		GetPositionOfAlignment(&p, r, szTextWithoutAnd, am);
		//p.x += m_pFont->GetWidth("-");

		if(m_pFont!=NULL){
			char temp[2] = {cHighlight, 0};
			int nFrontWidth = m_pFont->GetWidth(szFront);
			int nHighlightWidth = m_pFont->GetWidth(temp);
			//int nBackWidth = m_pFont->GetWidth(szBack);
			Text(p.x, p.y, szFront);
			MCOLOR tmpColor = m_Color;
			SetColor(m_HighlightColor);
			Text(p.x+nFrontWidth, p.y, temp);
			SetColor(tmpColor);
			return Text(p.x+nFrontWidth+nHighlightWidth, p.y, szBack);
		}
	}
	return -1;
}

MRECT MDrawContext::GetClipRect(void)
{
	return m_Clip;
}

void MDrawContext::TextMC(MRECT& r, const char* szText, MAlignmentMode am)
{
	MPOINT p;
	char* pPText;

	if(GetFont() == NULL) return;

	if((pPText = GetPureText(szText)) == NULL) return;
	GetPositionOfAlignment(&p, r, pPText, am);
	free(pPText);	// Release duplicated string buffer.

	TextMC(p.x, p.y, szText);
}

unsigned long int MMColorSet[] = {
		0xFF808080,	//0
		0xFFFF0000,	//1
		0xFF00FF00,	//2
		0xFF0000FF,	//3
		0xFFFFFF00,	//4

		0xFF800000,	//5
		0xFF008000,	//6
		0xFF000080,	//7
		0xFF808000,	//8
		0xFFFFFFFF,	//9
	};

static bool TestDigit(int c)
{
	if(c >= 48 && c <= 57){
		return true;
	} else {	
		return false;
	}
}

void MDrawContext::TextMC(int x, int y, const char* szText)
{
	unsigned int nPos = 0, nLen, nOffset = 0;
	const char *pSrc = szText;
	char *pText;

	if(GetFont() == NULL) return;

	while(true){
		nPos = strcspn(pSrc, "^");
		pText = (char *)malloc(nPos+1);
		if(pText){
			strncpy(pText, pSrc, nPos);				// �����ŭ ī��
			pText[nPos] = '\0';						// ���ڿ� �����,
			Text(x+nOffset, y, pText);		// ���
			nOffset += GetFont()->GetWidth(pText);
			free(pText);							// �ؽ�Ʈ ���� ����
		}

		nLen = strlen(pSrc);

		if(nPos + 1 < strlen(pSrc)){
			//�÷� ����
			if(TestDigit(pSrc[nPos+1])){
				SetColor(MCOLOR(MMColorSet[pSrc[nPos+1] - '0']));
				pSrc = pSrc + nPos + 2;
			} else {
				Text(x+nOffset, y, "^");
				nOffset += GetFont()->GetWidth("^");
				pSrc = pSrc + nPos + 1;
			}
		} else {
			if(nPos+1 == nLen && TestDigit(pSrc[nPos]) == false){
				Text(x+nOffset, y, "^");
				nOffset += GetFont()->GetWidth("^");
				//pSrc = pSrc + 1;
			}
			if(nPos == nLen && (pSrc[nPos-1] == '^')){
				Text(x+nOffset, y, "^");
				nOffset += GetFont()->GetWidth("^");
			}
			break;
		}
	}
}

char *MDrawContext::GetPureText(const char *szText)
{
	unsigned int nPos = 0, nLen;
	const char *pSrc = szText;
	char *pText;

	nLen = strlen(szText);
	pText = (char *)malloc(nLen+1);
	memset(pText, 0, nLen+1);

	while(true){
		nPos = strcspn(pSrc, "^");

		strncat(pText, pSrc, nPos);				// �����ŭ ī��
		nLen = strlen(pSrc);

		if(nPos + 1 < strlen(pSrc)){
			if(TestDigit(pSrc[nPos+1])){
				pSrc = pSrc + nPos + 2;
			} else {
				pSrc = pSrc + nPos + 1;
				strcat(pText, "^");
			}
		} else {
			if(nPos+1 == nLen && TestDigit(pSrc[nPos]) == false){
				strcat(pText, "^");
				//pSrc = pSrc + 1;
			}
			if(nPos == nLen && (pSrc[nPos-1] == '^')){
				strcat(pText, "^");				
			}
			break;
		}
	}
	return pText;
}

#define MAX_CHAR_A_LINE		255

// ��°�ٺ��� nIndentation��ŭ �鿩���⸦ �Ѵ�, skipline��ŭ �������� ���� ����Ѵ�.
int MDrawContext::TextMultiLine(MRECT& r, const char* szText,int nLineGap,bool bAutoNextLine,int nIndentation,int nSkipLine, MPOINT* pPositions, bool bSkipColor)
{
	bool bColorSupport=true;

	//MBeginProfile(99,"MDrawContext::TextMultiLine");

	int nLine = 0;
	MFont* pFont = GetFont();

	int nLength = strlen(szText);

	int y = r.y;
	const char* szCurrent=szText;
	MPOINT* pCurrentPos = pPositions;
	do {
		int nX = nLine==0 ? 0 : nIndentation;

		int nOriginalCharCount = MMGetNextLinePos(pFont,szCurrent,r.w-nX,bAutoNextLine,true);
		
		if(nSkipLine<=nLine) 
		{
			int nCharCount = min(nOriginalCharCount,MAX_CHAR_A_LINE);
			char buffer[256];
			if(bColorSupport) {

// Text�� �׷����� ������ ������ ä�� �ִ´�.
#define FLUSHPOS(_Pos)		if(pCurrentPos!=NULL){	\
								for(int i=0; buffer[i]!=NULL; i++){	\
									pCurrentPos[i+szCurrent-szText].x = _Pos+pFont->GetWidth(buffer, i);	\
									pCurrentPos[i+szCurrent-szText].y = y;	\
								}	\
							}

#define FLUSH				if(buffer[0]) { Text(r.x+nLastX, y, buffer); FLUSHPOS(r.x+nLastX); nLastX=nX; buffer[0]=0;pcurbuf=buffer; }

				int nLastX=nX;

				buffer[0]=0;
				char *pcurbuf=buffer;
				bool bLink = false;
				for(int i=0; i<nCharCount; i++){

					unsigned char c  = szCurrent[i], cc  = szCurrent[i+1];

					if(c=='^' && ('0'<=cc) && (cc<='9'))
					{
						FLUSH;

						if(bSkipColor == false)
							SetColor(MCOLOR(MMColorSet[cc - '0']));

						i++;
						continue;
					}

					else if ( (c == '%') && (cc == '%'))
					{
						if ( bLink)
						{
							MCOLOR col = GetColor();

// 							EndFont();

//							BeginDraw();

							SetColor( 0, 0, 255);
							FillRectangle( r.x + nLastX - 1, y - 1,
											pFont->GetWidth( buffer, (int)strlen( buffer)) + 2, pFont->GetHeight() + 2);

//							EndDraw();

// 							BeginFont();

							SetColor( col);
						}


						FLUSH;

						bLink = !bLink;

						i++;
						continue;
					}

					int w;

					*(pcurbuf++)=c;
					if(c>127 && i<nCharCount){
						*(pcurbuf++)=cc;
						w = pFont->GetWidth(szCurrent+i,2);
						i++;
					}
					else w = pFont->GetWidth(szCurrent+i,1);

					*pcurbuf=0;

					nX += w;
				}

				FLUSH;

			}else
			{
				strncpy(buffer,szCurrent,nCharCount);
				buffer[nCharCount]=0;

//				if (BeginFont())
				{
					Text(r.x+nX, y,buffer);
//					EndFont();
				}
				FLUSHPOS(r.x+nX);
			}
			y+=pFont->GetHeight()+nLineGap;
		}

		szCurrent+=nOriginalCharCount;
		nLine++;
		if(y>=r.y+r.h) break;
	} while(szCurrent<szText+nLength);

	//MEndProfile(99);
	return nLine-nSkipLine;
}

// TODO: TextMultiLine2�� ���ְ� align�� TextMultiLine�� �����Ѵ�
int MDrawContext::TextMultiLine2( MRECT& r, const char* szText,int nLineGap,bool bAutoNextLine,MAlignmentMode am, bool bSkipColor )
{
	MFont* pFont = GetFont();
	int nHeight = pFont->GetHeight()+nLineGap;
	int nWidth = pFont->GetWidth(szText);

	int nLine = MMGetLineCount( pFont, szText, r.w, bAutoNextLine );
	int nStrLen = strlen(szText);
	int nX = 0;
	int nCurrLine = 0;
	int IncY = ( r.h	/ nLine ) ; // ������ ����

#define		MAX_WIDGET_LINE_LENGTH	1024
	int clip = 0;
	char	TempStr[MAX_WIDGET_LINE_LENGTH];

	MRECT rTemp;

	int for_index = 0;
	for(int i=0; i<nStrLen; i++)
	{
		for_index = i;
		char temp[3] = {0, 0, 0};
		temp[0] = szText[i];
		unsigned char c = temp[0];

		if(c=='\n'){	// Carriage Return
			nX = 0;
				
			rTemp.x	= r.x;
			rTemp.y	= r.y + IncY * nCurrLine;
			rTemp.w	= r.w;
			rTemp.h	= IncY;

            strncpy( TempStr, szText + clip, i - clip );
			TempStr[i-clip] = '\0';
			Text( rTemp, TempStr, am );

			clip	= i+1;
			++nCurrLine;

			continue;
		}

		if(c=='^' && ('0'<=szText[i+1]) && (szText[i+1]<='9'))
		{
			if(bSkipColor == false)
			{
				SetColor(MCOLOR(MMColorSet[szText[i+1] - '0']));
			}

			i++;
			continue;
		}

		if ( (c == '%') && (szText[i+1] == '%'))
		{
			i++;
			continue;
		}

		if(c>127)
		{
			i++;
			if(i<nStrLen) 
				temp[1] = szText[i];

			for_index = i;				// �ѱۿ��� ������ ���� �ϳ��� ���� ���ϴ� ���� ����
		}

		int w = pFont->GetWidth(temp);
		if( nX +w > r.w && bAutoNextLine )
		{
			nX = 0;

			rTemp.x	= r.x;
			rTemp.y	= r.y + IncY * nCurrLine;
			rTemp.w	= r.w;
			rTemp.h	= IncY;
			++nCurrLine;

 			strncpy( TempStr, szText + clip, i - clip - 1 );
			TempStr[i-clip-1] = '\0';
			Text( rTemp, TempStr, am );
 			clip	= i - 1;
			continue;
		}
		nX += w;
	}

	rTemp.x	= r.x;
	rTemp.y	= r.y + IncY * nCurrLine;
	rTemp.w	= r.w;
	rTemp.h	= IncY;

	strncpy( TempStr, szText + clip, for_index - clip +1 );
	TempStr[for_index-clip + 1] = '\0';
	Text( rTemp, TempStr, am );

	return nLine;
}

// isnara 2007-04-02
// ������ Width ���� ��ŭ ���ڿ� �ڸ���
int MDrawContext::GetTextMultiLine(vector<string> *out_pvecStr, const char* szText, int nStrWidth, bool bSkipColor)
{
	MFont* pFont			= GetFont();
	int nWidth				= pFont->GetWidth(szText);
	int nLine				= 0;

	// nStrWidth���� ������...
	if(nStrWidth > nWidth)
	{
		// 1���̸�...
		if(out_pvecStr)
			out_pvecStr->push_back(szText);

		return 1;
	}

	char chPreColor[3]		= {0, };							// ���� �� ����
	char strTemp[1024]		= {0, };							// �ӽ� �����
	char* pstrTemp			= strTemp;
	int nstrCount			= strlen(szText);					// ���ڿ� ����
	int nPreIndex			= 0;								// ���� ���ڿ� �ε���, ������� ����ϴ� ����... for�� �������� ���̰� �Ǿ���.

	for(int i = 0; i < nstrCount; i++)
	{
		char temp[3]		= {0, };
		temp[0]				= szText[i];
		*pstrTemp			= szText[i];
		pstrTemp++;
		unsigned char c		= temp[0];

		// ���� ����
		if(c == '\n')											// Carriage Return
		{	
			*pstrTemp = '\0';

			if(out_pvecStr)
				out_pvecStr->push_back(strTemp);

			nLine++;

			// �ʱ�ȭ
			memset(strTemp, 0x00, 1024);
			pstrTemp = strTemp;

			// ���� �Է�
			if(strlen(chPreColor))
			{
				strncpy(strTemp, chPreColor, 2);
				pstrTemp += 2;
			}
			continue;
		}


		// ����
		if(c=='^' && ('0' <= szText[i+1]) && (szText[i+1] <= '9'))
		{
			i++;

			// ���� �����̸�...
			if(bSkipColor == false)
			{
				// ���� ����
				strncpy(chPreColor, szText + i - 1, 2);
				*pstrTemp = szText[i];
				pstrTemp++;
			}
			continue;
		}

		// ��ũ
		if ( (c == '%') && (szText[i+1] == '%'))
		{
			i++;
			continue;
		}


		// 2����Ʈ �����ΰ�...
		if(c > 127)
		{
			i++;
			*pstrTemp = szText[i];
			pstrTemp++;
		}

		// Width�� �Ѿ�ٸ�...
		int nCurrentWidth = pFont->GetWidth(strTemp);
		if(nStrWidth < nCurrentWidth)
		{
			// nPreIndex���� 1�� ���°�... ���� ���ڰ� ������ �����Ƿ�
			// ���� ���ڱ����� �������� ���Ѱ��̴�.
			for(int nRemove = i - nPreIndex; nRemove > 0; nRemove--)
			{
				pstrTemp--;
				*pstrTemp = '\0';
			}

			if(out_pvecStr)
				out_pvecStr->push_back(strTemp);

			nLine++;

			i = nPreIndex;										// ���� �ε����� ������.
			
			// �ʱ�ȭ
			memset(strTemp, 0x00, 1024);
			pstrTemp = strTemp;

			// ���� �Է�
			if(strlen(chPreColor))
			{
				strncpy(strTemp, chPreColor, 2);
				pstrTemp += 2;
			}
		}
		else
		{
			nPreIndex = i;
		}
	}

	// ������ ����
	if(strlen(strTemp))
	{
		if(out_pvecStr)
			out_pvecStr->push_back(strTemp);

		nLine++;
	}

	return nLine;
}

float MDrawContext::SetOpacity(float fOpacity)
{
	float fPrevOpacity = m_fOpacity;
	m_fOpacity = fOpacity;
	return fPrevOpacity;
}

float MDrawContext::GetOpacity()
{
	return m_fOpacity;
}

void MDrawContext::Draw(int x, int y)
{
	if(m_pBitmap==NULL) return;
	int w = m_pBitmap->GetWidth();
	int h = m_pBitmap->GetHeight();
	Draw(x, y, w, h, 0, 0, w, h);
}

void MDrawContext::Draw(int x, int y, int w, int h)
{
	if(m_pBitmap==NULL) return;
	int bw = m_pBitmap->GetWidth();
	int bh = m_pBitmap->GetHeight();
	Draw(x, y, w, h, 0, 0, bw, bh);
}

void MDrawContext::DrawInverse(  int x, int y, int w, int h  )
{
	if(m_pBitmap==NULL) return;
	int bw = m_pBitmap->GetWidth();
	int bh = m_pBitmap->GetHeight();
	DrawInverse( x,y,w,h,0,0,bw,bh );
}

void MDrawContext::Draw(int x, int y, int sx, int sy, int sw, int sh)
{
	if(m_pBitmap==NULL) return;
	int w = m_pBitmap->GetWidth();
	int h = m_pBitmap->GetHeight();
	Draw(x, y, w, h, sx, sy, sw, sh);
}

void MDrawContext::Draw(MPOINT &p)
{
	Draw(p.x, p.y);
}

void MDrawContext::Draw(MRECT& r)
{
	Draw(r.x, r.y, r.w, r.h);
}

void MDrawContext::Draw(MRECT& d, MRECT& s)
{
	Draw(d.x, d.y, d.w, d.h, s.x, s.y, s.w, s.h);
}

void MDrawContext::Draw(int x, int y, MRECT& s)
{
	Draw(x, y, s.x, s.y, s.w, s.h);
}

void MDrawContext::DrawEx(int tx1, int ty1, int tx2, int ty2, 
						  int tx3, int ty3, int tx4, int ty4)
{

}

void MDrawContext::Draw(int x, int y, int w, int h, int sx, int sy, int sw, int sh)
{
	if(m_pBitmap==NULL) return;
	Draw(m_pBitmap,x,y,w,h,sx,sy,sw,sh);
}

void MDrawContext::DrawRotate( MVector3& pos, MVector3& dir, int x, int y, int w, int h, int sx, int sy, int sw, int sh)
{
	if(m_pBitmap==NULL) return;
	DrawRotate(m_pBitmap, pos, dir, x,y,w,h,sx,sy,sw,sh);
}

/*	DrawRotate

	x, y, w, h = �̹����� ����� Ÿ�� ��ġ �� ������
	rx, ry = �̹����� ȸ����Ű�� ȸ�� �� ��ǥ
	ra = ȸ�� ����(radian)
	sx, sy, sw, sh = �̹����� ������ �ҽ� ��ġ �� ������
*/
void MDrawContext::DrawRotate( int tx, int ty, int tw, int th, int rx, int ry, float ra, int sx, int sy, int sw, int sh)
{
	if ( m_pBitmap == NULL)
		return;

	DrawRotate( m_pBitmap, tx, ty, tw, th, rx, ry, ra, sx, sy, sw, sh);
}

void MDrawContext::DrawTiled(int x, int y, int w, int h, int sx, int sy, int sw, int sh)
{
	if(m_pBitmap==NULL) return;
	DrawTiled(m_pBitmap,x,y,w,h,sx,sy,sw,sh);
}

int MMGetLinePos(MFont *pFont,const char* szText, int nWidth, bool bAutoNextLine, bool bColorSupport, int nLine,int nIndentation)
{
	int nInd = nIndentation >= nWidth ? 0 : nIndentation;

	int nStrLen = strlen(szText);
	if(nStrLen==0) return 0;

	int nX = 0;
	int nCurLine = 0;

	int nThisChar=0;	// ���� ������ ���� �ε���

	for(int i=0; i<nStrLen; i++){

		if(nCurLine==nLine) 
			return nThisChar;

		nThisChar = i;	// ���� ������ ���� �ε���

		char temp[3] = {0, 0, 0};
		temp[0] = szText[i];
		unsigned char c = temp[0];

		if(c=='\n'){	// Carriage Return
			nCurLine++;
			nThisChar++;
			continue;
		}

		if(bColorSupport)
		{
			if(c=='^' && ('0'<=szText[i+1]) && (szText[i+1]<='9'))
			{
				i++;
				continue;
			}
		}

		if ( (c == '%') && (szText[i+1] == '%'))
		{
			i++;
			continue;
		}

		if(c>127){
			i++;
			if(i<nStrLen) temp[1] = szText[i];
		}

		if(bAutoNextLine){
			int w = pFont->GetWidth(temp);
			if(nX+w>=nWidth){
				nCurLine++;
				nX = nIndentation;	// ��°�ٺ��ʹ� ������ġ�� �ٸ���.
			}
			nX += w;
		}
	}

	if(nCurLine==nLine) 
		return nThisChar;

	return nStrLen;
}

// ���� ���� ���� ��ġ�� �����Ѵ�
int MMGetNextLinePos(MFont *pFont,const char* szText, int nWidth, bool bAutoNextLine, bool bColorSupport)
{
	return MMGetLinePos(pFont,szText,nWidth,bAutoNextLine,bColorSupport);
}

int MMGetWidth(MFont *pFont,const char *szText,int nSize,bool bColorSupport)
{
	int nStrLen = min((int)strlen(szText),nSize);

	int nX = 0;

	for(int i=0; i<nStrLen; i++){

		char temp[3] = {0, 0, 0};
		temp[0] = szText[i];
		unsigned char c = temp[0];

		if(c=='\n'){	// Carriage Return
			return nX;
		}

		if(bColorSupport)
		{
			if(c=='^' && ('0'<=szText[i+1]) && (szText[i+1]<='9'))
			{
				i++;
				continue;
			}
		}

		if ( (c =='%') && (szText[i+1] == '%'))
		{
			i++;
			continue;
		}

		if(c>127){
			i++;
			if(i<nStrLen) temp[1] = szText[i];
		}

		int w = pFont->GetWidth(temp);
		nX += w;
	}

	return nX;
}

int MMGetLineCount(MFont *pFont,const char* szText, int nWidth, bool bAutoNextLine, bool bColorSupport,int nIndentation)
{
	int nLine = 0;
	int nLength = strlen(szText);

	int nCurPos=0;
	do {
		int nRealWidth = (nLine==0 ? nWidth : nWidth-nIndentation);
		int nOriginalCharCount = MMGetNextLinePos(pFont,szText+nCurPos,nRealWidth,bAutoNextLine,bColorSupport);
		if(nOriginalCharCount==0 && szText[nCurPos]!=0) return -1;
		nCurPos+=nOriginalCharCount;
		nLine++;
	} while(nCurPos<nLength);

	return nLine;
}

} // namespace mint3
