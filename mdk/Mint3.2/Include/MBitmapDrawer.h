#pragma once


namespace mint3
{

struct MRECT;
class MDrawContext;
class MBitmap;


enum MDRAWBITMAPSTYLE
{
	DRAWBITMAP_NORMAL		= 1,	// �⺻���� Draw ��Ÿ��
	DRAWBITMAP_STRETCH,				// �̹��� ���̱�
};


void DrawBitmapByStyle( MDrawContext* pDC, MRECT& r, MBitmap* Bms, MRECT* rCoord, MDRAWBITMAPSTYLE byStyle = DRAWBITMAP_NORMAL, bool bTiled =false);

}

