/************************************************************************
MBEditLook

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : Editbox ��Ʈ���� Ŀ���ҷ�(Custom look) Ŭ���� �Դϴ�.
*************************************************************************/


#pragma once

#include "Mint.h"
#include "MEdit.h"
#include "MBitmapDrawer.h"



// namespace : mint3
namespace mint3
{


/*	MBEditLook
	�������� Look�� �����ϴ� Ŭ����
*/
class MBEditLook : public MEditLook
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

	// �ؽ�Ʈ ������ �����ϴ� ����
	MCOLOR				m_colorFont[ 3];



// Member functions
public:
	// ������
	MBEditLook();

	// �Ҹ���
	virtual ~MBEditLook();



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



protected:
	// �������� �׸��� �Լ�
	virtual void OnFrameDraw( MEdit* pEdit, MDrawContext* pDC);

	// �ؽ�Ʈ�� �׸��� �Լ�
	virtual void OnTextDraw( MEdit* pEdit, MDrawContext* pDC, bool bShowLanguageTab);

	// Ŭ���̾�Ʈ�� ������ ���ϴ� �Լ�(�ؽ�Ʈ�� �Է��ϴ� ����)
	virtual MRECT GetClientRect(MEdit* pEdit, MRECT& r);
};


} // namespace mint3











/*****************************************************************

���⼭���ʹ� ���� �ڵ��Դϴ�


#pragma once

#include "MEdit.h"

namespace mint3 {

class MBEditLook : public MEditLook{
public:
	MBitmap*	m_pFrameBitmaps[9];
	MFont*		m_pFont;
	

public:
	MBEditLook(void);

	virtual void OnFrameDraw(MEdit* pEdit, MDrawContext* pDC);
	virtual MRECT GetClientRect(MEdit* pEdit, MRECT& r);
	virtual void OnDraw(MEdit* pEdit, MDrawContext* pDC);
};

} // namespace mint3
*/