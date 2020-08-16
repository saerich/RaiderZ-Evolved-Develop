#include "stdafx.h"
#include "MBitmapDrawer.h"
#include "MDrawContext.h"
#include "MintTypes.h"


namespace mint3
{

void BitmapStretch(int& nOutW, int& nOutH, const MRECT& r, const MRECT* rCoord)
{
	nOutW = r.w - (rCoord[4].w + rCoord[5].w);
	nOutH = r.h - (rCoord[2].h + rCoord[7].h);
}


void DrawBitmapByStyle(MDrawContext* pDC, MRECT& r, MBitmap* Bms, MRECT* rCoord, MDRAWBITMAPSTYLE byStyle, bool bTiled)
{
	pDC->SetBitmap(Bms);

	// ����ȯ�� ���� ��ġ ��� ���� ����,�� �������� ������ �ʴ� ������� ����
	// �� ������ ���� ��ǥ ������ �ٽ� �ؾ� �Ѵ�.
	int nLongWid = 0;
	int nLongHei = 0;

	nLongWid = rCoord[0].w;
	nLongHei = rCoord[0].h;


	// �̹����� �Ϲ����� ��´�
	if ( byStyle == DRAWBITMAP_STRETCH || bTiled)
		BitmapStretch( nLongWid, nLongHei, r, rCoord);


	// �־����� ������ ������ ���� �κ�
	if(nLongWid <= 0)	nLongWid = 0;
	if(nLongHei <= 0)	nLongHei = 0;

	MRECT stThinRect[9];
	memcpy(stThinRect, rCoord, sizeof(MRECT) * 9);

	stThinRect[1].x = stThinRect[4].x = stThinRect[6].x = 0;
	stThinRect[1].y = stThinRect[2].y = stThinRect[3].y = 0;

	stThinRect[0].x	= rCoord[4].w;
	stThinRect[2].x = rCoord[1].w;
	stThinRect[7].x = rCoord[6].w;

	stThinRect[0].y = stThinRect[4].y = stThinRect[5].y = rCoord[2].h;
	stThinRect[6].y = stThinRect[7].y = stThinRect[8].y = rCoord[2].h + nLongHei;

	stThinRect[3].x = r.w - rCoord[3].w;
	stThinRect[5].x = rCoord[4].w + nLongWid;
	stThinRect[8].x = r.w - rCoord[8].w;


	stThinRect[0].h = stThinRect[4].h = stThinRect[5].h = nLongHei;

	stThinRect[2].w = r.w - rCoord[1].w - rCoord[3].w;
	stThinRect[0].w = r.w - rCoord[4].w - rCoord[5].w;
	stThinRect[7].w = r.w - rCoord[6].w - rCoord[8].w;

	for ( int i = 0; i < 9; i++)
	{
		// ������ ���� ���� �־�� Draw�� �Ѵ�.
		if( rCoord[i].w || rCoord[i].h)
		{
			pDC->Draw( stThinRect[i].x + r.x, stThinRect[i].y + r.y, stThinRect[i].w, stThinRect[i].h,
				rCoord[i].x, rCoord[i].y, rCoord[i].w, rCoord[i].h );
		}
	}
}


}
