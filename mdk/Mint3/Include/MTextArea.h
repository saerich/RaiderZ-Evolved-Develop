#pragma once

#pragma warning(disable:4786)

#include <list>
#include <string>
#include "MWidget.h"
#include "MLookNFeel.h"
#include "MScrollBar.h"

using namespace std;

namespace mint3 {

class MTextArea;
class MScrollBar;

#define MTEXTAREA_DEFAULT_TEXT_COLOR		MCOLOR(224,224,224)

class MTextAreaLook{
public:
	virtual void OnDraw(MTextArea* pTextArea, MDrawContext* pDC);
	virtual MRECT GetClientRect(MTextArea* pTextArea, MRECT& r);

private:
	virtual void OnFrameDraw(MTextArea* pTextArea, MDrawContext* pDC);
	virtual void OnTextDraw(MTextArea* pTextArea, MDrawContext* pDC);
	virtual void OnTextDraw_WordWrap(MTextArea* pTextArea, MDrawContext* pDC);
};

// �۾��� �������� TextArea
class MNullTextAreaLook : public MTextAreaLook {
public:
private:
	virtual void OnFrameDraw(MTextArea* pTextArea, MDrawContext* pDC) {}
};


struct MLineItem {
	MLineItem(MCOLOR _color,string &_text) {
		color=_color;
		text=_text;
	}
	MLineItem(string &_text) {
		color=MTEXTAREA_DEFAULT_TEXT_COLOR;
		text=_text;
	}
	MCOLOR color;
	string text;
};

typedef vector<MLineItem> MLINELIST;
typedef vector<MLineItem>::iterator MLINELISTITERATOR;


class MTextArea : public MWidget{
	friend MTextAreaLook;
protected:
	bool		m_bScrollBarEnable;
	int			m_nIndentation;		// ���̹ٲ�� �鿩���� �ȼ���
	bool		m_bWordWrap;
	bool		m_bColorSupport;
	MPOINT		m_TextOffset;		// �̰� ����� ?  ���µ��� ����̴µ�...
	bool		m_bMouseOver;
	MCOLOR		m_TextColor;
	int			m_nMaxLen;
	char		m_szIMECompositionString[MIMECOMPOSITIONSTRING_LENGTH];
	bool		m_bEditable;
//	char*		m_pBuffer;			//Text Buffer
	int			m_nStartLine;
	int			m_nStartLineSkipLine;
	
	int			m_nCurrentSize;
	bool		m_bVerticalMoving;
	int			m_nVerticalMoveAxis;

	MPOINT		m_CaretPos;		// ��ü���� ĳ�� ��ġ. y��° ���� x��° ����
//	MPOINT		m_LineCaretPos;	// �Ѷ��� �ȿ����� ĳ�� ��ġ
	bool		m_bCaretFirst;	// �Ѷ��� �ȿ��� �ٹٲ� ĳ���� ������ ������ ������ �÷���

	MLINELIST			m_Lines;
	MLINELISTITERATOR	m_CurrentLine;

	MScrollBar*		m_pScrollBar;

	// Look & Feel
	DECLARE_LOOK(MTextAreaLook)
	DECLARE_NULL_LOOK(MNullTextAreaLook)
	DECLARE_LOOK_CLIENT()

	// ��ũ�ѹ� Look ����
	void SetScrollBarLook( MScrollBarLook* pLook)			{ m_pScrollBar->ChangeCustomLook( pLook);	}

	// ��ũ�ѹ� �β� ����
	void SetScrollBarWidth( int nWidth)					{ m_pScrollBar->SetWidth( nWidth);			}


protected:
	virtual void OnSize(int w, int h);
	virtual bool OnCommand(MWidget* pWidget, MEventMsg nMsg, const char* szArgs=NULL);
	virtual bool OnEvent(MEvent* pEvent, MListener* pListener);
	virtual void OnSetFocus(void);
	virtual void OnReleaseFocus(void);
	
	// Ű �Է��� ������ �� Ű���� ���� ó���� �� �ִ�.
	// true�� �����ϸ� InputFilter���� ó�������� �ǹ��Ѵ�.
	// false�� �����ϸ� Edit�� ��Ʈ���� �߰����� �ǹ��Ѵ�.
	virtual bool InputFilterKey(int nKey,bool bCtrl);	// MWM_KEYDOWN
	virtual bool InputFilterChar(int nKey);	// MWM_CHAR

	bool OnLButtonDown(MPOINT pos);
	void OnScrollBarChanged(int nPos);

	bool MoveLeft(bool bBackspace=false);
	void MoveRight();
	void MoveDown();
	void MoveUp();
	void DeleteCurrent();
	void ScrollDown();
	void ScrollUp();
	void MoveFirst();
	void MoveLast();
	void OnHome();
	void OnEnd();

// �ȿ��� �ʿ��� �Լ���
	
	// nPos��° ���ڰ� ��� ��ġ�� �׷����� �ϴ��� pOut���� �������ش�
	// x�� �ȼ��� , y �� ���μ�,bFirst�� ĳ���ϰ�� ��/���÷���
	bool GetCaretPosition(MPOINT *pOut,int nSize,const char* szText,int nPos,bool bFirst);

	// MDrawTextMultiline���� �׸��� nLine �࿡ nX�ȼ� ��ġ�� ���° ���ڰ� ������ �˷��ִ� ���
	int GetCharPosition(const char* szText,int nX,int nLine);

	// ���� ĳ���� ��ġ�� �� ���ξȿ��� �����ٷ� wrap������ �Ѿ���
	// ���� �������̳� ������ ù��°�� ��ǻ� ���� ��ġ�̴�. �̷�������� �˾ƺ���
	bool IsDoubleCaretPos();	

	void UpdateScrollBar(bool bAdjustStart=false);

	MLINELISTITERATOR GetIterator(int nLine);

public:
	MTextArea(int nMaxLen = 120, const char* szName=NULL, MWidget* pParent=NULL, MListener* pListener=NULL);
	virtual ~MTextArea();

#define MINT_TEXTAREA	"TextArea"
	virtual const char* GetClassName(void){ return MINT_TEXTAREA; }

	MPOINT GetCaretPos() { return m_CaretPos; }
	int	GetStartLine() { return m_nStartLine; }

	bool IsScrollBarVisible() { return m_pScrollBar->IsVisible();	}
	int GetScrollBarWidth() { return m_pScrollBar->GetRect().w;	}

	int GetClientWidth();

	int GetLength() { return (int)(m_nCurrentSize+m_Lines.size()); }
	int GetLineCount() { return (int)m_Lines.size(); }

	bool GetText(char *pBuffer,int nBufferSize);
	const char* GetTextLine(int nLine);

	void SetMaxLen(int nMaxLen);
	int	GetMaxLen() { return m_nMaxLen; }

	const char* GetCompositionString(void);

	void SetEditable(bool editable){ m_bEditable = editable; }
	bool GetEditable() { return m_bEditable; }

	void SetWordWrap(bool bEnabled){ m_bWordWrap = bEnabled; }
	bool GetWordWrap() { return m_bWordWrap; }

	void SetScrollBarEnable(bool bEnable) { m_bScrollBarEnable = bEnable; }
	bool GetScrollBarEnable() { return m_bScrollBarEnable; }
	
	void SetTextOffset(MPOINT p);

	void SetIndentation(int nIndentation) { m_nIndentation = nIndentation; }
	
	void SetTextColor(MCOLOR color);
	MCOLOR GetTextColor(void);

	// ��� �ؽ�Ʈ ������ �����.
	void Clear();

	virtual void SetText(const char *szText);

	void AddText(const char *szText,MCOLOR color);
	void AddText(const char *szText);
	
	// ù���� �����
	void DeleteFirstLine();

	int GetLineHeight( void);

	// glue �Լ�
	void glueClear();
	void glueAddText(const char* szText);
	void glueAddColorText(const char *szText, unsigned int color);
	int glueGetMaxHeight();
	void	glueMoveFirst()	{ MoveFirst(); }
};

} // namespace mint3