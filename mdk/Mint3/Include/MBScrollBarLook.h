/************************************************************************
MBScrollBarLook

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : Scrollbar ��Ʈ���� Ŀ���ҷ�(Custom look) Ŭ���� �Դϴ�.
*************************************************************************/


#pragma once

#include "Mint.h"
#include "MScrollBar.h"
#include "MBitmapDrawer.h"


// namespace : mint3
namespace mint3
{


/*	MBScrollBarLook
	��ũ�ѹ��� Look�� �����ϴ� Ŭ����
*/
class MBScrollBarLook : public MScrollBarLook
{
	// Member variables
private:
	// ��Ʈ���� �����ϴ� ���� ����
	MBitmapRep			m_Bitmaps[ MSBR_MAX][ 3];	// [type] [normal, mouse down, disabled]

	// ��Ʈ�� �׸��� ��Ÿ�� ����
	MDRAWBITMAPSTYLE	m_BitmapStyle;

	// ��Ʈ�� �̹����� Ÿ�������� ����� ���θ� �����ϴ� ����
	bool				m_bTiled;

	// Ŀ���� Look�� ����ϴ��� ���θ� �����ϴ� ����
	bool				m_bCustomLook;



// Member functions
public:
	// ������
	MBScrollBarLook();

	// �Ҹ���
	virtual ~MBScrollBarLook();



	// Ŀ���� Look�� ����ϴ��� ���θ� �����ϴ� �Լ�
	void SetCustomLook( bool bCustom =true)		{ m_bCustomLook = bCustom;			}

	// Ŀ���� Look�� ����ϴ��� ���θ� ���ϴ� �Լ�
	bool IsCustomLook() const						{ return m_bCustomLook;				}



	/* Interface functions */

	// �����ӿ� ���� �̹����� �����ϴ� �Լ�
	void SetBitmap( const string strFileName);

	// �����ӿ� ���� ��Ʈ���� ������ �����ϴ� �Լ�
	void SetAssignRect( int type, int state, int index, float x1, float y1, float x2, float y2);

	// ��Ʈ���� �÷� ���� �� �ְ� �����ϴ� �Լ�
	void SetStretchBlt( bool bStretch =true);

	// Ÿ�� ��Ʈ���� ����ϴ��� ���θ� �����ϴ� �Լ�
	void SetTiled( bool bTiled)					{ m_bTiled = bTiled;				}



protected:
	// ��ũ�ѹٸ� �׸��� �Լ�
	virtual void OnDraw( MScrollBar* pScrollBar, MDrawContext* pDC);
};

} // namespace mint3