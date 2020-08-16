

#pragma once

#include "Mint.h"
#include "MPopupBox.h"
#include "MBitmapDrawer.h"


// namespace : mint3
namespace mint3
{


// class : MBPopupBoxLook
class MBPopupBoxLook : public MPopupBoxLook
{
// Member variables
private:
	// �������� ��Ʈ���� �����ϴ� ���� ����
	MBitmapRep			m_Bitmaps;

	// ��Ʈ�� �׸��� ��Ÿ�� ����
	MDRAWBITMAPSTYLE	m_BitmapStyle;

	// ��Ʈ�� �̹����� Ÿ�������� ����� ���θ� �����ϴ� ����
	bool				m_bTiled;

	// Ŀ���� Look�� ����ϴ��� ���θ� �����ϴ� ����
	bool				m_bCustomLook;



// Member functions
public:
	MBPopupBoxLook();
	virtual ~MBPopupBoxLook();


	// Ŀ���� Look�� ����ϴ��� ���θ� �����ϴ� �Լ�
	void SetCustomLook( bool bCustom =true)			{ m_bCustomLook = bCustom;			}

	// Ŀ���� Look�� ����ϴ��� ���θ� ���ϴ� �Լ�
	bool IsCustomLook() const						{ return m_bCustomLook;				}



	/* Interface functions */

	// �����ӿ� ���� �̹����� �����ϴ� �Լ�
	void SetBitmap( const string strFileName);

	// �����ӿ� ���� ��Ʈ���� ������ �����ϴ� �Լ�
	void SetAssignRect( int index, float x1, float y1, float x2, float y2);

	// ��Ʈ���� �÷� ���� �� �ְ� �����ϴ� �Լ�
	void SetStretchBlt( bool bStretch =true);

	// Ÿ�� ��Ʈ���� ����ϴ��� ���θ� �����ϴ� �Լ�
	void SetTiled( bool bTiled)						{ m_bTiled = bTiled;				}


protected:
	// �׸��� �Լ�
	virtual void OnDraw( MWidget* pWidget, MDrawContext* pDC);
};



} // namespace mint3
















/************************************************************************************
���⼭���� �����ڵ��Դϴ�


#pragma once

#include "MGroup.h"

namespace mint3 {

#define FRAME_BITMAP_COUNT	9
class MBPopupBoxLook : public MGroupLook{
public:
	MBitmap*	m_pFrameBitmaps[FRAME_BITMAP_COUNT];
	MFont*		m_pFont;
	MCOLOR		m_FontColor;
	MPOINT		m_TitlePosition;
	bool		m_bStretch;

protected:
	virtual void OnDraw(MGroup* pGroup, MDrawContext* pDC);

public:
	MBPopupBoxLook(void);

	virtual MRECT GetClientRect(MGroup* pGroup, MRECT& r);
};

} // namespace mint3
*/
