/************************************************************************
MBTextAreaLook

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : TextArea ��Ʈ���� Ŀ���ҷ�(Custom look) Ŭ���� �Դϴ�.
*************************************************************************/


#pragma once

#include "Mint.h"
#include "MTextArea.h"
#include "MBitmapDrawer.h"



// namespace : mint3
namespace mint3
{


/*	MBTextAreaLook
	����Ʈ�ڽ��� Look�� �����ϴ� Ŭ����
*/
class MBTextAreaLook : public MTextAreaLook
{
// Member variables
private:
	// �������� ��Ʈ���� �����ϴ� ���� ����
	MBitmapRep			m_Bitmaps[ 3];			// Normal, Focused, Disabled

	// ��Ʈ�� �׸��� ��Ÿ�� ����
	MDRAWBITMAPSTYLE	m_BitmapStyle;

	// ��Ʈ�� �̹����� Ÿ�������� ����� ���θ� �����ϴ� ����
	bool				m_bTiled;

	// Ŀ���� Look�� ����ϴ��� ���θ� �����ϴ� ����
	bool				m_bCustomLook;

	// ��Ʈ�� �����ϴ� ����
	MFont*				m_pFont;




// Member functions
public:
	// ������
	MBTextAreaLook();

	// �Ҹ���
	virtual ~MBTextAreaLook();



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
	void SetTiled( bool bTiled)								{ m_bTiled = bTiled;					}

	// �ؽ�Ʈ ��Ʈ�� �����ϴ� �Լ�
	void SetFont( MFont* pFont)									{ m_pFont = pFont;						}

	// �ؽ�Ʈ�׸��� ������ ���� ���ϱ� ����
	MBitmapRep*	GetBitMap()		{ return &m_Bitmaps[0];}

protected:
	// �������� �׸��� �Լ�
	virtual void OnFrameDraw( MTextArea* pTextArea, MDrawContext* pDC);

	// Ŭ���̾�Ʈ�� ������ ���ϴ� �Լ�(�ؽ�Ʈ�� �Է��ϴ� ����)
	virtual MRECT GetClientRect( MTextArea* pTextArea, MRECT& r);
};


} // namespace mint3










/***********************************************************************
���⼭���� ���� �ڵ��Դϴ�



#pragma once

#include "MTextArea.h"

namespace mint3 {

class MBTextAreaLook : public MTextAreaLook{
public:
	MFont*		m_pFont;
	MBitmap*	m_pFrameBitmaps[9];

public:
	MBTextAreaLook(void);

	virtual void OnFrameDraw(MTextArea* pTextArea, MDrawContext* pDC);
	virtual MRECT GetClientRect(MTextArea* pTextArea, MRECT& r);
};

} // namespace mint3
*/