/************************************************************************
MBGroupLook

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : Group ��Ʈ���� Ŀ���ҷ�(Custom look) Ŭ���� �Դϴ�.
*************************************************************************/


#pragma once

#include "Mint.h"
#include "MGroup.h"
#include "MBitmapDrawer.h"


// namespace : mint3
namespace mint3
{


/*	MBGroupLook
�������� Look�� �����ϴ� Ŭ����
*/
class MBGroupLook : public MGroupLook
{
	// Member variables
private:
	// �������� ��Ʈ���� �����ϴ� ���� ����
	MBitmapRep			m_Bitmaps[ 2];

	// ��Ʈ�� �׸��� ��Ÿ�� ����
	MDRAWBITMAPSTYLE	m_BitmapStyle;

	// ��Ʈ�� �̹����� Ÿ�������� ����� ���θ� �����ϴ� ����
	bool				m_bTiled;

	// Ŀ���� Look�� ����ϴ��� ���θ� �����ϴ� ����
	bool				m_bCustomLook;

	// ��Ʈ�� �����ϴ� ����
	MFont*				m_pFont;

	// �ؽ�Ʈ ������ �����ϴ� ����
	MCOLOR				m_colorFont[ 2];

	// �ؽ�Ʈ ��ġ ������ ���� �����ϴ� ����
	MPOINT				m_ptTextOffset;




	// Member functions
public:
	// ������
	MBGroupLook();

	// �Ҹ���
	virtual ~MBGroupLook();



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
	void SetTiled( bool bTiled)					{ m_bTiled = bTiled;				}

	// �ؽ�Ʈ ��Ʈ�� �����ϴ� �Լ�
	void SetFont( MFont* pFont)						{ m_pFont = pFont;					}

	// �ؽ�Ʈ ������ �����ϴ� �Լ�
	void SetFontColor( int state, MCOLOR& color)	{ m_colorFont[ state] = color;		}

	// �ؽ�Ʈ ��ġ ������ ���� �����ϴ� �Լ�
	void SetTextOffset( int x, int y)				{ m_ptTextOffset.x = x;  m_ptTextOffset.y = y;	}



protected:
	// �������� �׸��� �Լ�
	virtual void OnDraw( MGroup* pGroup, MDrawContext* pDC);
};


} // namespace mint3
















/************************************************************************************
���⼭���� �����ڵ��Դϴ�


#pragma once

#include "MGroup.h"

namespace mint3 {

#define FRAME_BITMAP_COUNT	9
class MBGroupLook : public MGroupLook{
public:
	MBitmap*	m_pFrameBitmaps[FRAME_BITMAP_COUNT];
	MFont*		m_pFont;
	MCOLOR		m_FontColor;
	MPOINT		m_TitlePosition;
	bool		m_bStretch;

protected:
	virtual void OnDraw(MGroup* pGroup, MDrawContext* pDC);

public:
	MBGroupLook(void);

	virtual MRECT GetClientRect(MGroup* pGroup, MRECT& r);
};

} // namespace mint3
*/
