/************************************************************************
MBListBoxLook

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : ListBox ��Ʈ���� Ŀ���ҷ�(Custom look) Ŭ���� �Դϴ�.
*************************************************************************/


#pragma once

#include "Mint.h"
#include "MListBox.h"
#include "MBitmapDrawer.h"



// namespace : mint3
namespace mint3
{

// ����Ʈ�ڽ� ����
enum MLISTBOX_REGION
{
	MLBR_NONE			= -1,		// ����
	MLBR_LISTBOX		= 0,		// ����Ʈ �ڽ�
	MLBR_HEADER,					// ���
	MLBR_SELECTBAR,					// ���� ����
	MLBR_MAX
};


/*	MBListBoxLook
	�������� Look�� �����ϴ� Ŭ����
*/
class MBListBoxLook : public MListBoxLook
{
// Member variables
private:
	// �������� ��Ʈ���� �����ϴ� ���� ����
	MBitmapRep			m_Bitmaps[ MLBR_MAX][ 3];		// [ListBox, Header, Select bar] [Normal, Focused, Disabled]

	// ��Ʈ�� �׸��� ��Ÿ�� ����
	MDRAWBITMAPSTYLE	m_BitmapStyle;

	// ��Ʈ�� �̹����� Ÿ�������� ����� ���θ� �����ϴ� ����
	bool				m_bTiled;

	// Ŀ���� Look�� ����ϴ��� ���θ� �����ϴ� ����
	bool				m_bCustomLook;

	// ��Ʈ�� �����ϴ� ����
	MFont*				m_pFont;

	// �ؽ�Ʈ ������ �����ϴ� ����
	MCOLOR				m_colorFont[ MLBR_MAX][ 3];		// [ListBox, Header, Select bar] [Normal, Focused, Disabled]




// Member functions
public:
	// ������
	MBListBoxLook();

	// �Ҹ���
	virtual ~MBListBoxLook();



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
	void SetTiled( bool bTiled)								{ m_bTiled = bTiled;					}

	// �ؽ�Ʈ ��Ʈ�� �����ϴ� �Լ�
	void SetFont( MFont* pFont)									{ m_pFont = pFont;						}

	// �ؽ�Ʈ ������ �����ϴ� �Լ�
	void SetFontColor( int type, int state, MCOLOR& color)		{ m_colorFont[ type][ state] = color;	}



protected:
	// ����Ʈ �ڽ��� ����� �׸��� �Լ�
	virtual void OnHeaderDraw( MListBox* pListBox, MDrawContext* pDC, MRECT& r, const char* szText);

	// ����Ʈ �ڽ��� ���� ���븦 �׸��� �Լ�
	virtual void OnDrawSelectBar( MListBox* pListBox, MDrawContext* pDC, MRECT& r, bool bSelected, bool bEnabled, bool bFocus);
		
	// ����Ʈ �ڽ��� �������� �׸��� �Լ�
	virtual int OnItemDraw( MListBox* pListBox, MDrawContext* pDC, MRECT& r, const char* szText, MCOLOR color, bool bSelected, bool bEnabled, bool bFocus, int nAdjustWidth = 0);
	virtual int OnItemDraw( MListBox* pListBox, MDrawContext* pDC, MRECT& r, MBitmap* pBitmap, bool bSelected, bool bEnabled, bool bFocus, int nAdjustWidth = 0);

	// �������� �׸��� �Լ�
	virtual void OnFrameDraw( MListBox* pListBox, MDrawContext* pDC);

	// Ŭ���̾�Ʈ�� ������ ���ϴ� �Լ�(�ؽ�Ʈ�� �Է��ϴ� ����)
	virtual MRECT GetClientRect( MListBox* pListBox, MRECT& r);
};


} // namespace mint3










/************************************************************************
���⼭���ʹ� ���� �ڵ��Դϴ�


#pragma once

#include "MListBox.h"

namespace mint3 {

class MBListBoxLook : public MListBoxLook{
public:
	MBitmap*	m_pFrameBitmaps[9];
	MFont*		m_pFont;

protected:
	virtual void OnFrameDraw(MListBox* pListBox, MDrawContext* pDC);

public:
	MBListBoxLook(void);

	virtual MRECT GetClientRect(MListBox* pListBox, MRECT& r);
	virtual void OnDraw(MListBox* pListBox, MDrawContext* pDC);
};

} // namespace mint3
*/