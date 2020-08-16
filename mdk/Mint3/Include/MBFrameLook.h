/************************************************************************
MBFrameLook

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : Frame ��Ʈ���� Ŀ���ҷ�(Custom look) Ŭ���� �Դϴ�.
*************************************************************************/


#pragma once

#include "Mint.h"
#include "MFrame.h"
#include "MBitmapDrawer.h"


// namespace : mint3
namespace mint3
{


/*	MBFrameLook
	�������� Look�� �����ϴ� Ŭ����
*/
class MBFrameLook : public MFrameLook
{
// Member variables
private:
	// �������� ��Ʈ���� �����ϴ� ���� ����
	MBitmapRep			m_Bitmaps[ 3];

	// ��Ʈ�� �׸��� ��Ÿ�� ����
	MDRAWBITMAPSTYLE	m_BitmapStyle;

	// ��Ʈ�� �̹����� Ÿ�������� ����� ���θ� �����ϴ� ����
	bool				m_bTiled;

	// Ŀ���� Look�� ����ϴ��� ���θ� �����ϴ� ����
	bool				m_bCustomLook;

	// ��Ʈ�� �����ϴ� ����
	MFont*				m_pFont;

	// �ؽ�Ʈ ������ �����ϴ� ����
	MCOLOR				m_colorFont[ 3];

	// Ÿ��Ʋ�� �׸��ڸ� ����ϴ��� ���θ� �����ϴ� ����
	bool				m_bTitleShadow;

	// Ÿ��Ʋ ��ġ ������ ���� �����ϴ� ����
	MPOINT				m_ptTitleOffset;



// Member functions
public:
	// ������
	MBFrameLook();

	// �Ҹ���
	virtual ~MBFrameLook();



	// Ŀ���� Look�� ����ϴ��� ���θ� �����ϴ� �Լ�
	void SetCustomLook( bool bCustom =true)		{ m_bCustomLook = bCustom;			}

	// Ŀ���� Look�� ����ϴ��� ���θ� ���ϴ� �Լ�
	bool IsCustomLook() const						{ return m_bCustomLook;				}



	/* Interface functions */

	// �����ӿ� ���� �̹����� �����ϴ� �Լ�
	void SetBitmap( const string strFileName);

	// �����ӿ� ���� ��Ʈ���� ������ �����ϴ� �Լ�
	void SetAssignRect( int state, int index, float x1, float y1, float x2, float y2);

	// ��Ʈ���� �÷� ���� �� �ְ� �����ϴ� �Լ�
	void SetStretchBlt( bool bStretch =true);

	// Ÿ�� ��Ʈ���� ����ϴ��� ���θ� �����ϴ� �Լ�
	void SetTiled( bool bTiled)						{ m_bTiled = bTiled;				}

	// �ؽ�Ʈ ��Ʈ�� �����ϴ� �Լ�
	void SetFont( MFont* pFont)						{ m_pFont = pFont;					}

	// �ؽ�Ʈ ������ �����ϴ� �Լ�
	void SetFontColor( int state, MCOLOR& color)	{ m_colorFont[ state] = color;		}

	// Ÿ��Ʋ �׸��ڸ� ����ϵ��� �����ϴ� �Լ�
	void SetTitleShadow( bool bShadow =true)		{ m_bTitleShadow = bShadow;			}

	// Ÿ��Ʋ ��ġ ������ ���� �����ϴ� �Լ�
	void SetTitleOffset( int x, int y)				{ m_ptTitleOffset.x = x;  m_ptTitleOffset.y = y;	}


protected:
	// �������� �׸��� �Լ�
	virtual void OnDraw( MFrame* pFrame, MDrawContext* pDC);

	// �������� Ÿ��Ʋ�ٿ� �ؽ�Ʈ�� �׸��� �Լ�
	void DrawText( MFrame* pFrame, MDrawContext* pDC);
};


} // namespace mint3









/**************************************************************

 ���⼭���ʹ� ���� �ڵ��Դϴ�


#pragma once

#include "MFrame.h"
#include "MScalableLook.h"

namespace mint3 {

#define FRAME_BITMAP_COUNT			9
#define FRAME_BUTTON_BITMAP_COUNT	3

class MBFrameLook : public MFrameLook , public MScalableLook {
public:

	// madduck [4/5/2006]
	MBitmapRep	m_sBitmaps;
	void	SetAssignRect(int index, float x1, float y1, float x2, float y2)
	{
		if( index >= 9) return;

		m_sBitmaps.m_rCoord[index].x = (int)x1;
		m_sBitmaps.m_rCoord[index].y = (int)y1;
		m_sBitmaps.m_rCoord[index].w = (int)x2;
		m_sBitmaps.m_rCoord[index].h = (int)y2;
	}

	char		m_szDefaultTitle[256];
	MBitmap*	m_pFrameBitmaps[FRAME_BITMAP_COUNT];
	MFont*		m_pFont;
	MCOLOR		m_FontColor;
	MCOLOR		m_BGColor;
	MPOINT		m_TitlePosition;
	bool		m_bStretch;
	MBitmap*	m_pCloseButtonBitmaps[FRAME_BUTTON_BITMAP_COUNT];	// 0 - up, 1 - down, 2 - disable
	MBitmap*	m_pMinimizeButtonBitmaps[FRAME_BUTTON_BITMAP_COUNT];
protected:
	virtual void	OnDraw(MFrame* pFrame, MDrawContext* pDC);
	int			m_iCustomLook;

public:
	MBFrameLook(void);
	void SetCustomLook( int i) {
		m_iCustomLook = i;
	}
	int GetCustomLook() const{
		return m_iCustomLook;
	}

	virtual MRECT GetClientRect(MFrame* pFrame, MRECT& r);
};

} // namespace mint3
*/
