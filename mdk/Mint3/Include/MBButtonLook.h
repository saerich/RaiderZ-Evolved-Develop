/************************************************************************
MBButtonLook

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : Button ��Ʈ���� Ŀ���ҷ�(Custom look) Ŭ���� �Դϴ�.
*************************************************************************/


#pragma once

#include "Mint.h"
#include "MButton.h"
#include "MBitmapDrawer.h"
#include <map>


// namespace : mint3
namespace mint3
{

/*	MBS_STATE
	��ư�� ���¸� �����մϴ�
*/
enum MBS_STATE
{
	MBS_NORMAL = 0,					// �Ϲ� ����
	MBS_MOUSEOVER,					// ���콺�� �ö�� �ִ� ����
	MBS_PRESSED,						// ���� ����
	MBS_DISABLED,					// ��� �Ұ� ����
	MBS_MAX							// �ִ� ���� ����
};



/*	MBButtonLook
	��ư�� Look�� �����ϴ� Ŭ����
*/
class MBButtonLook : public MButtonLook
{
// Member variables
private:
	// ��ư�� ���¿� ���� ��Ʈ���� �����ϴ� ���� ����
	MBitmapRep			m_Bitmaps[ MBS_MAX];

	// ��Ʈ�� �׸��� ��Ÿ�� ����
	MDRAWBITMAPSTYLE	m_BitmapStyle;

	// ��Ʈ�� �̹����� Ÿ�������� ����� ���θ� �����ϴ� ����
	bool				m_bTiled;

	// Ŀ���� Look�� ����ϴ��� ���θ� �����ϴ� ����
	bool				m_bCustomLook;

	// ��Ʈ�� �����ϴ� ����
	MFont*				m_pFont;

	// �ؽ�Ʈ ������ �����ϴ� ����
	MCOLOR				m_colorFont[ MBS_MAX];

	// �ؽ�Ʈ�� �׸��ڸ� ����ϴ��� ���θ� �����ϴ� ����
	bool				m_bTextShadow;

	// ���� ��ư�� �ؽ�Ʈ�� �̵��ϴ� ������ ���� �����ϴ� ����
	MPOINT				m_ptTextOffset;

	// ���̵� �ѿ���(���콺�� ���ٴ�� ������ ���ϴ� ȿ��)�� ����ϴ��� ���θ� �����ϴ� ����
	bool				m_bFadeRollover;

	// ���̵� �ѿ����� Ÿ�̸�
	DWORD				m_dwFadeTimer;

	// ���̵� ���� �����ϴ� ����
	map<MButton*,float>		m_FadeMap;



// Member functions
public:
	// ������
	MBButtonLook();

	// �Ҹ���
	virtual ~MBButtonLook();



	// Ŀ���� Look�� ����ϴ��� ���θ� �����ϴ� �Լ�
	void SetCustomLook( bool bCustom =true)		{ m_bCustomLook = bCustom;			}

	// Ŀ���� Look�� ����ϴ��� ���θ� ���ϴ� �Լ�
	bool IsCustomLook() const						{ return m_bCustomLook;				}



	/* Interface functions */

	// ��ư�� ���� �̹����� �����ϴ� �Լ�
	void SetBitmap( const string strFileName);

	// ��ư�� ���� ��Ʈ���� ������ �����ϴ� �Լ�
	void SetAssignRect( int state, int index, float x1, float y1, float x2, float y2);

	// �Ϲ� ������ ��ư�� �׸��� �Լ�
	virtual void OnUpDraw( MButton* pButton, MDrawContext* pDC);

	// ���콺�� �ö�� ��ư�� �׸��� �Լ�
	virtual void OnOverDraw( MButton* pButton, MDrawContext* pDC);

	// ���� ��ư�� �׸��� �Լ�
	virtual void OnDownDraw( MButton* pButton, MDrawContext* pDC);

	// ����� �� ���� ��ư�� �׸��� �Լ�
	virtual void OnDisableDraw( MButton* pButton, MDrawContext* pDC);

	// ��Ʈ���� �÷� ���� �� �ְ� �����ϴ� �Լ�
	void SetStretchBlt( bool bStretch =true);

	// Ÿ�� ��Ʈ���� ����ϴ��� ���θ� �����ϴ� �Լ�
	void SetTiled( bool bTiled)					{ m_bTiled = bTiled;				}

	// �ؽ�Ʈ ��Ʈ�� �����ϴ� �Լ�
	void SetFont( MFont* pFont)						{ m_pFont = pFont;					}

	// �ؽ�Ʈ ������ �����ϴ� �Լ�
	void SetFontColor( int state, MCOLOR& color)	{ m_colorFont[ state] = color;		}

	// �ؽ�Ʈ �׸��ڸ� ����ϵ��� �����ϴ� �Լ�
	void SetTextShadow( bool bShadow =true)		{ m_bTextShadow = bShadow;			}

	// �ؽ�Ʈ ��ġ ������ ���� �����ϴ� �Լ�
	void SetTextOffset( int x, int y)				{ m_ptTextOffset.x = x;  m_ptTextOffset.y = y;	}

	// ���̵� �ѿ��� ȿ���� ����ϵ��� �����ϴ� �Լ�
	void SetFadeRollover( bool bFade =true);


protected:
	// ������ ��ư�� �׸��� �Լ�
	void DrawButton( MButton* pButton, MDrawContext* pDC, MBS_STATE state);

	// ��ư�� �ؽ�Ʈ�� �׸��� �Լ�
	void DrawText( MButton* pButton, MDrawContext* pDC, MBS_STATE state);

	// ��ư�� ���̵� ���� ���ϴ� �Լ�
	float GetFadeValue( MButton* pButton, MBS_STATE state);


	WIDGET_UI_LOOK_TYPE( MWIDGET_MBBUTTONLOOK);
};


} // namespace mint3








/**************************************************************************************************

   ���⼭���ʹ� ���� �ڵ��Դϴ�.



	void SetAssignRect(int stat, int index, float x1, float y1, float x2, float y2)
	{
		if( stat >= BS_MAX) return;
		if( index >= 9) return;

		m_StateBitmaps[stat].m_rCoord[index].x = (int)x1;
		m_StateBitmaps[stat].m_rCoord[index].y = (int)y1;
		m_StateBitmaps[stat].m_rCoord[index].w = (int)x2;
		m_StateBitmaps[stat].m_rCoord[index].h = (int)y2;
	}

	MBitmap*	m_pUpBitmaps[9];
	MBitmap*	m_pDownBitmaps[9];
	MBitmap*	m_pOverBitmaps[9];
	MBitmap*	m_pFocusBitmaps[4];

	MFont*		m_pFont;
	MCOLOR		m_FontColor;
	MCOLOR		m_FontDisableColor;
	MCOLOR		m_FontDownColor;
	MCOLOR		m_FontHighlightColor;
	bool		m_bStretch;
	bool		m_bCustomLook;
	MPOINT		m_FontDownOffset;

	bool		m_bEnableFade;
	float		m_fFade;
	
protected:
	virtual void DrawText(MButton* pButton, MDrawContext* pDC, MRECT& r, const char* szText, MAlignmentMode a);
	virtual void DrawFocus(MDrawContext* pDC, MRECT& r);
	virtual void OnDownDraw(MButton* pButton, MDrawContext* pDC);
	virtual void OnUpDraw(MButton* pButton, MDrawContext* pDC);
	virtual void OnOverDraw(MButton* pButton, MDrawContext* pDC);
	virtual void OnDisableDraw(MButton* pButton, MDrawContext* pDC);
	virtual void OnCheckBoxDraw(MButton* pButton, MDrawContext* pDC, bool bPushed);
	virtual void OnDraw(	MButton* pButton, MDrawContext* pDC );

public:
	MBButtonLook(void);
	virtual ~MBButtonLook() {}

	void SetCustomLook(bool b) {
		m_bCustomLook = b;
	}
	bool GetCustomLook() const{
		return m_bCustomLook;
	}

	virtual MRECT GetClientRect(MButton* pButton, MRECT& r);

	void EnableFade( bool bEnable)		{ m_bEnableFade = bEnable;	}
*/
