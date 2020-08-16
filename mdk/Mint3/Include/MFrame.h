#pragma once

#include "MWidget.h"
#include "MLookNFeel.h"
#include "MBmButton.h"

#define MTITLEBAR_HEIGHT		18


namespace mint3 {

class MFrame;
class MFrameLook{
protected:
	MRECT	m_WinRectOffset;			// Look ���� ������ ���� ������ ���� �����ϴ� ����
	int		m_nTitlebarHeight;			// Ÿ��Ʋ�� ����

public:
	MFrameLook() { m_WinRectOffset = MRECT( 0, 0, 0, 0);	m_nTitlebarHeight = MTITLEBAR_HEIGHT;	}
	virtual void OnDraw(MFrame* pFrame, MDrawContext* pDC);
	virtual MRECT GetClientRect(MFrame* pFrame, MRECT& r);

	// Look ���� ������ ���� ������ ���� ���� �� ���ϴ� �Լ�
	void SetWinRectOffset( int x, int y, int w, int h)		{ m_WinRectOffset = MRECT( x, y, w, h);	}
	const MRECT& SetWinRectOffset()							{ return m_WinRectOffset;				}

	// Ÿ��Ʋ���� ���̸� ���� �� ���ϴ� �Լ�
	void SetTitlebarHeight( int height)						{ m_nTitlebarHeight = height;			}
	int GetTitlebarHeight()									{ return m_nTitlebarHeight;				}

};

struct MFrameBtn
{
	bool			m_bVisible;
	bool			m_bMouseOver;
	bool			m_bLButtonDown;
//	bool			m_bDowned;
	MRECT			m_Rect;
	MAnchorStyle	m_Anchor;
	MFrameBtn() { 
		m_bVisible = false; 
		m_bLButtonDown = false; 
		m_bMouseOver = false; 
		m_Rect = MRECT(0,0,0,0);
	}
	bool IsButtonDown() { if ((m_bLButtonDown==true) && (m_bMouseOver==true)) return true; return false; }
};

/// ������ �� �ִ� ���������� ������ ����
class MFrame : public MWidget{
	DECLARE_LOOK(MFrameLook)
	DECLARE_LOOK_CLIENT()
protected:
	bool	m_bDragWidget;		///< Drag Widget
	MPOINT	m_DragPoint;

	bool	m_bMovable;			///< ������ �� �ִ��� ����
	bool	m_bCanShade;		///< Shade �� �� �ִ��� ���� - false�̸� Shade�� �� ����.
	bool	m_bShade;
	MSIZE	m_BeforeShade;		///< Before Shade

	MRECT		m_OldRect;
	MFrameBtn	m_BtnClose;
	MFrameBtn	m_BtnMinimize;

public:
	bool		m_bTitleBar;	///< Use Title Bar

protected:
	virtual bool OnCommand(MWidget* pWidget, MEventMsg nMsg, const char* szArgs=NULL);
	virtual bool OnEvent(MEvent* pEvent, MListener* pListener);
	virtual void OnSize(int w, int h);
	virtual void OnShow(void);

	void ResizeBtnsByAnchors(int w, int h);
	void OnCloseButtonClick();
	void OnMinimizeButtonClick();
public:
	MFrame(const char* szName=NULL, MWidget* pParent=NULL, MListener* pListener=NULL);
	virtual ~MFrame(void);

	void SetShade(bool bShade);
	void SetMovable(bool bValue) { m_bMovable = bValue; }
	MFrameBtn*	GetCloseButton() { return &m_BtnClose; }
	MFrameBtn*	GetMinimizeButton() { return &m_BtnMinimize; }

	virtual bool IsFocus(void);


#define MINT_FRAME	"Frame"
	virtual const char* GetClassName(void){ return MINT_FRAME; }
};

} // namespace mint3