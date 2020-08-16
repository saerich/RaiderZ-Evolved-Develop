//
// MINT ( MAIET In-house wiNdows sysTem )
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "MDrawContext.h"
#include "MintTypes.h"
#include "MEvent.h"
#include "MEventHandler.h"
#include "MWLua.h"
#include <string>
#include <list>

using namespace std;

namespace mint3 {


class MWidget;
class MResourceMap;

/// ������ Z ����
enum MZOrder{
	MZ_TOP = 0,				///< �� ������
	MZ_BOTTOM = 100,		///< �� �ڷ�
};

// ���� Ÿ��
enum MWidgetLookType
{
	MWIDGET_MBUTTONLOOK,			// ��ư Look
	MWIDGET_MFRAMELOOK,				// ������ Look
	MWIDGET_MBBUTTONLOOK			// �̹��� ��ư Look
};

struct MAnchorStyle
{
	bool bLeft;
	bool bRight;
	bool bTop;
	bool bBottom;
	MAnchorStyle(void)	{ bLeft = true; bRight = false; bTop = true; bBottom = false; }
	MAnchorStyle(bool left, bool right, bool top, bool bottom)
	{ bLeft = left; bRight = right; bTop = top; bBottom = bottom; }
};



#define MWIDGET_TEXT_LENGTH			256			///< ���� �ؽ�Ʈ �ִ� ����
#define MWIDGET_WINDOW_MIN_SIZE_W	50			// ���� â �ּ� ������(Width)
#define MWIDGET_WINDOW_MIN_SIZE_H	50			// ���� â �ּ� ������(Height)

#define WIDGET_UI_LOOK_TYPE(TY)			virtual MWidgetLookType GetUILookType()	{ return TY; }

/// �⺻ ���� Ŭ����
class MWidget  : public MListener 
{
private:
	friend class MEventHandler;

	bool				m_bEnable;			///< Enable
	bool				m_bFocusEnable;		///< Focus Enable

//	MListener*			m_pListener;		///< Listener
	string				m_strTooltip;		///< ���� �޽���
	string				m_strHelpMsg;		///< ���� �޽���(Mint�� �������� �������� ������ ������ �� �ִ� �޽���)

	int					m_nAndPos;
	int					m_nAccelerator;		///< Accelerator Key
protected:
	bool				m_bVisible;			///< Visible Flag

	list<MWidget*>		m_Children;			///< Children Widgets

	MWidget*			m_pParent;			///< Parent Widget
	list<MWidget*>		m_Exclusive;		///< Exclusive Child Widget Stack ( for Modal Support )
	MEventHandler		m_EventHandler;
	bool				m_bDeclToScript;


	MCursor*			m_pCursor;			///< Default Cursor of this Widget
	MFont*				m_pFont;			///< Default Font of this Widget

	static MWidget*		m_pCapturedWidget;	///< SetCapture(), ReleaseCapture()
	static MWidget*		m_pFocusedWidget;	///< SetFocus()

	bool				m_bZOrderChangable;	///< Z Order Change by L-Button Click
	bool				m_bResizable;		///< Resizable?

	int					m_nResizeSide;		///< 0: N/A   1: Up  2: Right  4: Down  8: Left(BitFlag)

	float				m_fOpacity;			///< ������(0 : ��������, 1 : ����������)

	bool				m_bClipByParent;	///< Parent Widget�� ���� Clip�Ǵ°�?

	MAlignmentMode		m_BoundsAlignment;	///< m_Rect�� ���� Alignment
	MAnchorStyle		m_Anchor;			///< Parent Widget�� ����ؼ� ��ġ�ϴ°�?

	bool				m_bLButtonDown;
	bool				m_bRButtonDown;

// 	bool				m_bUseRatioToStandardScreenSize;	///< ȭ�� ���� ��� ��(true)/��(flase, Default)
// 	float				m_fRatioToStandardScreenWidth;	///< ȭ�� ���� ����
// 	float				m_fRatioToStandardScreenHeight; ///< ȭ�� ���� ����

	bool				m_bIterating;		///< ������Ʈ ���ͷ����� ���ΰ�
	
	struct ZORDERRESERVED {
		MWidget* pWidget;
		MZOrder zOrder;
	};
	list< ZORDERRESERVED > m_reservedZOrder;

public:

	string				m_strName;			///< �ν��Ͻ� Name
//	char				m_szText[MWIDGET_TEXT_LENGTH];	///< Text
	string				m_strText;			///< Text

	MRECT				m_Rect;				///< Rect in Parent Widget
	
	int					m_nMinWidth, m_nMinHeight;	///< ������ �ּ� ũ��

	bool				m_bEventAcceleratorCall;///< Ű�Է��̺�Ʈ�� �ҷȴ��� ����
protected:
	// Only for Designer Mode
	bool				m_bEnableDesignerMode;	///< Designer Mode Ȱ��ȭ �÷���
	int					m_nDMDragWidget;		///< ���� Move �Ǵ� Resize �÷���
	MPOINT				m_DMDragPoint;			///< ���� Move �Ǵ� Resize ������
	bool				m_bModifiedByDesigner;	///< Designer�� ���� ��ġ,ũ�Ⱑ ������ ���
	bool				m_bAddedByDesigner;		///< Designer�� ���� �߰��� ���
	int					m_nID;					///< ������ ID

private:
	/// Local Coordinate�� �̿��� Local Event�� ��ȯ
	void MakeLocalEvent(MEvent* pLoalEvent, const MEvent* pEvent);
	/// Event���� Resize�κ� ó��
	bool EventResize(MEvent* pEvent);
	/// ��ũ��Ʈ���� ����
	void ReleaseFromScript();

	/// ������Ʈ �߿��� zorder�� ���� �ٲٸ� �ȵǰ� ������ �ؾ��մϴ�.
	void ReserveZOrder(MZOrder z);
	/// ����� zorder �� ����
	void ExecuteReservedZOrder();

protected:
	/// �̺�Ʈ �ڵ鷯 -----------------------------------------------
	virtual void OnEnabled(bool bEnabled);
	virtual void OnShow();
	virtual void OnHide();
	virtual void OnClick(MEvent* pEvent);
	virtual void OnRClick(MEvent* pEvent);
	virtual void OnDoubleClick(MEvent* pEvent);
	virtual void OnMouseDown(MEvent* pEvent);
	virtual void OnMouseUp(MEvent* pEvent);
	virtual void OnKeyDown(MEvent* pEvent);
	virtual void OnKeyUp(MEvent* pEvent);
	virtual void OnMouseWheel(MEvent* pEvent);
	virtual void OnItemSelected(MEvent* pEvent);
protected:
	/// Child Widget �� �տ� �߰�
	void InsertChild(MWidget* pWidget);
	/// Child Widget �߰�
	void AddChild(MWidget* pWidget);
	/// Child Widget ����
	void RemoveChild(MWidget* pWidget);
	/// Parent�� ���� ����
	void RemoveFromParent();

	// Accelerator�� szText�� &�ڿ� ������ ���ڷ� �Ѵ�.
	//void UseAcceleractorAndCharacter(char* szText);

	/// Exclusive Stack�� �߰��Ѵ�.
	void AddExclusive(MWidget* pWidget);
	/// Exclusive Stack���� �����Ѵ�.
	bool RemoveExclusive(MWidget* pWidget);

	/// Runtime Loop
	virtual void OnRun(void);
	/// Draw Handler
	virtual void OnDraw(MDrawContext* pDC);
	/*
	/// Draw Handler
	/// @deprecated	3D Rendering Pass�� ���� ó���ϱ� ���� �ڵ�� ���̻� ������� �ʴ´�.
	virtual void OnDraw3D(MDrawContext3D* pDC);
	*/
	/// Event Handler. Event�� ó���Ǹ� return true, �ƴϸ� return false.
	virtual bool OnEvent(MEvent* pEvent, MListener* pListener);

	/// ��Ű�� �������� �ҷ����� �̺�Ʈ �ڵ鷯
	virtual bool OnHotKey(int nID){ return false; }


	/// ��Ŀ�� �ڵ鷯
	virtual void OnSetFocus(void);
	virtual void OnReleaseFocus(void);

	/// SetSize(), SetBounds()ȣ�⿡ ���� Event Handler
	virtual void OnSize(int w, int h);

	/// Tab Key
	virtual bool OnTab(bool bForward=true);

	/// Drop&Drop�� ���� �������� �ڵ鷯
	virtual bool OnDrop(MWidget* pSender, MBitmap* pBitmap, const char* szString, const char* szItemString);

	/// ����� ���� �Ѱ��
	virtual bool OnUnDrop(MWidget* pSender, MBitmap* pBitmap, const char* szString, const char* szItemString);

	/// Listener Event Handler ( ������ �޼����� ���������� return true�� �Ѵ�. )
	virtual bool OnCommand(MWidget* pWidget, MEventMsg nMsg, const char* szArgs=NULL){ return false; }

	/// Anchor ������ ���� Children Resize
	/// @param w	new width
	/// @param h	new height
	void ResizeChildrenByAnchor(int w, int h);

	/// Bounds Alignment�� ���� ��ġ��
	void GetBoundsAlignmentPosition(MPOINT* p, MAlignmentMode am, int w=-1, int h=-1);
public:
	/// Workspace�� ����ɶ� ȣ��Ǵ� �Լ�
	//virtual void OnReSize();
	static MEventArgs		m_EventArgs;		///< ��ƿ��� ����ϴ� �̺�Ʈ ���ڰ�
	static MDragEventArgs	m_DragEventArgs;
	static MLinkedTextArgs	m_LinkedTextArgs;
public:
	/// @param szName	���� �̸�
	/// @param pParent	�θ� ����
	/// @param szName	�޼����� ���޹��� ������, ������ �θ� ������ �ش�ȴ�.
	MWidget(const char* szName=NULL, MWidget* pParent=NULL, MListener* pListener=NULL);
	virtual ~MWidget(void);

	/// Loop�� �������� Widget System���� ó���� ���� ������ �����Ѵ�.
	void Run(void);
	/// �� Widget�� ����� Child Widget�� �׸���.
	void Draw(MDrawContext* pDC);
	/// ���� �ٽ� �׸���.
	void Redraw(void);
	/// �̺�Ʈ�� ó���� �ڵ鷯 ȣ��
	bool Event(MEvent* pEvent);
	/// Ű���� Accelerator �̺�Ʈ�� ó��
	bool EventAccelerator(MEvent* pEvent);
	/// ����Ʈ Ű(Enter, ESC) �ڵ鷯 ȣ��
	bool EventDefaultKey(MEvent* pEvent);

	/// ������ �����ش�.
	/// @param bVisible	Visible Flag
	/// @param bModal	Modal Dialog Flag
	virtual void Show(bool bVisible=true, bool bModal=false);
	/// ������ �����.
	void Hide(void){ Show(false); }
	/// ������ Ȱ��ȭ ��Ų��.
	/// @param bEnable	Enable Flag
	void Enable(bool bEnable=true);
	/// ������ ���̴°�?
	bool IsVisible(void);
	/// ������ Ȱ��ȭ �Ǿ��°�?
	bool IsEnable(void);

	void SetResizable(bool bEnable);
	bool IsResizable(void);

	/// ������ ����
	/// @param pListener	������ ������
	virtual void SetListener(MListener* pListener);
	/// ���� ������ ���
	/// @return ���� ������
	virtual MListener* GetListener(void);


	/// ���� ID ���
	int GetID(void);
	/// ���� ID �����ϱ�
	void SetID(int nID);

	/// ���� �ν��Ͻ� �̸� ���
	string& GetName();

	/// ���� �ؽ�Ʈ(Name) �����ϱ�
	virtual void SetText(const char* szText);
	/// ���� �ؽ�Ʈ(Name) ���
	virtual const char* GetText(void);

	/// ���� ������ ��� ���콺 �޼����� ������ ĸ��
	void SetCapture(void);
	/// ���� ĸ�ĵ� ������ ����
	void ReleaseCapture(void);

	void SetFocusEnable(bool bEnable);
	bool IsFocusEnable(void);

	/// ������ ��Ŀ���� ��ġ��Ų��. ��Ŀ���� ������ Ű���� �޼����� �켱������ ����.
	void SetFocus(void);
	/// ������ ��Ŀ���� ������Ų��.
	void ReleaseFocus(void);
	/// ������ ��Ŀ���� ������ �ִ°�?
	virtual bool IsFocus(void);

	/// �θ� ������ �����Ѵ�.
	void SetParent(MWidget* pParent);
	/// �θ� ������ ����.
	MWidget* GetParent(void);
	/// �ڽ� ���� ������ ����.
	int GetChildCount(void);
	/// �ڽ� ������ ����.
	/// @param i	�ڽ� ���� �ε���
	MWidget* GetChild(int i);
	/// �ڽ� ������ �ε����� ����.
	/// @param	pWidget	�ڽ� ����
	/// @return	�ڽ� ������ �ε���
	int GetChildIndex(MWidget* pWidget);
	/// Child Widget ���� �� �޸𸮿����� ����
	void DeleteChildren();
	bool IsEmptyChildWidgets() { return ((m_Children.empty()) ? true : false); }

	/// Exculsive ���(Modal)�� ��ȯ�Ѵ�. Exclusive ���� �����Ǿ� �ִ� ������ ��� �޼����� �����Ѵ�.
	void SetExclusive(void);
	/// Exclusive ��带 �����ϳ�.
	void ReleaseExclusive(void);
	/// Exclusive Stack�� ���� ������ ������ ����.
	MWidget* GetLatestExclusive(void);
	/// Child Widget�� Exclusive���� �׽�Ʈ
	bool IsExclusive(MWidget* pWidget);

	/// ������ Ŀ���� �����Ѵ�.
	/// @param pCursor	������ ������ Ŀ��
	/// @return			������ �����Ǿ� �ִ� Ŀ��
	MCursor* SetCursor(MCursor* pCursor);
	/// ���� Ŀ���� ����.
	MCursor* GetCursor(void);

	/// ������ ��Ʈ�� �����Ѵ�.
	MFont* SetFont(MFont* pFont);
	/// ���� ��Ʈ�� ����.
	MFont* GetFont(void);

	/// Size �����ϱ�
	/// @param w, h		���� ���� ũ��
	void SetSize(int w, int h);
	/// Size �����ϱ�
	/// @param s		������
	void SetSize(MSIZE& s);

	/// Parent Widget�� Local ��ǥ�迡�� Poition �����ϱ�
	/// @param x, y		��ġ
	void SetPosition(int x, int y);
	/// Parent Widget�� Local ��ǥ�迡�� Poition �����ϱ�
	/// @param p		��ġ
	void SetPosition(MPOINT& p);
	/// Parent Widget�� Local ��ǥ�迡�� Initial Rect �����ϱ�
	void SetInitialBounds(MRECT& r);
	/// Parent Widget�� Local ��ǥ�迡�� Rect �����ϱ�
	/// @param r		Rectangle
	void SetBounds(MRECT& r);
	/// Parent Widget�� Local ��ǥ�迡�� Rect �����ϱ�
	/// @param x,y,w,h		Rectangle
	void SetBounds(int x, int y, int w, int h);
	/// Parent Widget�� Local ��ǥ�迡�� Position ����
	MPOINT GetPosition(void);
	/// Parent Widget�� Local ��ǥ�迡�� Rect ����
	MRECT GetRect(void);
	/// Anchor ���� ���
	MAnchorStyle& GetAnchor()			{ return m_Anchor;		}
	/// Anchor ���� �����ϱ�
	void SetAnchor(MAnchorStyle& anchor);
	/// ��ġ������ ����θ�Ʈ ����
	void SetBoundsAlignment(MAlignmentMode am, int w, int h);
	/// ��ġ������ ����θ�Ʈ ���
	MAlignmentMode GetBoundsAlignment(void);

	void SetOpacity(float fOpacity);
	float GetOpacity();

	/// Screen ��ǥ�迡�� Rect ����
	/// @deprecated		���� ��ǥ��� ȥ���� �������Ƿ� ���� ��ǥ�� �Լ���� ��ü, ���� ��ǥ�� �˰� ������ MClientToScreen(),MScreenToClient()�� �̿�
	MRECT GetScreenRect(void);

	/// ���� �޽��� ���� �� ����
	void SetTooltipMsg( const char* szToolTipMsg =NULL);
	const char* GetTooltipMsg();

	/// ���� �޽��� ���� �� ����
	void SetHelpMsg( const char* szHelpMsg =NULL);
	const char* GetHelpMsg();

	/// Accelerator ����
	void SetAccelerator(int a);
	/// Label���� &������ ������ ���ڷ� Accelerator ����
	void SetLabelAccelerator(void);
	/// ���� Accelerator ���
	char GetLabelAccelerator(void);

	/// Widget�� Ŀ���͸������ Client Rect
	virtual MRECT GetClientRect(void);
	/// Widget�� ���� Client Rect(0, 0, width, height)
	MRECT GetInitialClientRect(void);

	/// Child Widget ������ �ٲ� �׸��� ������ �����Ѵ�.
	void SetZOrder(MZOrder z);

	/// �ڼ� Widget�� ��ó�� �߰ߵ� Exclusive Widget ����
	MWidget* FindExclusiveDescendant(void);

	/// ��ġ�� ���� �˻�
	MWidget* Find(int x, int y){ return Find(MPOINT(x, y)); }
	/// ��ġ�� ���� �˻�
	MWidget* Find(MPOINT& p);
	MWidget* FindDropAble(MPOINT& p);

	/// �ش� ID�� Drag Object�� Drop�����Ѱ�?
	virtual bool IsDropable(MWidget* pSender){ return false; }
	bool Drop(MWidget* pSender, MBitmap* pBitmap, const char* szString, const char* szItemString);
	bool UnDrop(MWidget* pSender, MBitmap* pBitmap, const char* szString, const char* szItemString);

	bool GetVisible() { return m_bVisible; }

	/// ���� ������ ������ �̸� ���� ( ��: Parent/Child )
	void GetHierarchicalName(char* szName);
	/// ���� ������ ������ �̸����� ���� ã��
	MWidget* FindWidgetByHierarchicalName(const char* szName);

	/// ���� �������� Accelerator� ���� ����� �� �ִ� Default Command�� ���� ( �������̵��� ��� return true�� �Ѵ�. )
	virtual bool DefaultCommand(void){ return false; }

	/// �޼����� ��ġ�ϴ°�?
	static bool IsMsg(const char* szMsg1, const char* szMsg2);

	/// Query Anything
	virtual void* Query(const char* szQuery);

	void SetClipByParent(bool b) {
		m_bClipByParent = b;
	}

	/// �ּ� ������ ���� ������ �ּ� ������� ����
	void ResetMinSize(MRECT& rt, const MRECT& rtOrg);

	virtual void SetTextColor(MCOLOR color) {}

	void MakeLocalPos(MEvent* pLoalEvent, const MEvent* pEvent)
	{
		MakeLocalEvent(pLoalEvent, pEvent);
	}

	// ���콺 ���� �ʱ�ȭ
	void InitMouseState();

#define MINT_WIDGET	"Widget"
	/// Ŭ���� �̸� ���
	virtual const char* GetClassName(void){ return MINT_WIDGET; }

	/// ���� ����
	virtual MAlignmentMode SetAlignment(MAlignmentMode am) { return MAM_NOTALIGN; }

	/// Designer Mode�� ���Ǵ� Widget?
	bool IsEnableDesignerMode(void);
	/// Designer Mode�� ���
	void EnableDesignerMode(bool bEnable);
	/// Designer Mode�� ���� �����Ǿ�����?
	bool IsModifiedByDesigner(void);
	/// AddedByDesignerFlag�� ���õǾ� �ִ°�?
	bool IsAddedByDesignerFlag(void);

public:
	void LinkScriptEvent(string& strEventName, string& strScript);
	void DeclToSctipt();

	// Lua���� ����ϴ� �Լ�(glue�� �������� lua���� ����� �� �ִ�.)
	void					glueShow() { Show(true); }
	void					glueHide() { Show(false); }
	void					glueShowModal() { Show(true, true); }
	void					glueSetOpacity(float fOpacity);
	void					glueSetSize(int nWidth, int nHeight);
	void					glueSetText(const char* szText);
	bool					glueIsVisible();

	void					glueSetWidth(int nWidth);

	// madduck [5/11/2006]
	bool					glueIsFocus();
	void					glueSetFocus();

	int						glueGetWidth();
	int						glueGetHeight();
	const char*				glueGetName();
	const char*				glueGetText();
	float					glueGetOpacity();
	MWidget*				glueGetParent();
	bool					glueIsEnabled()	{ return IsEnable(); }
	void					glueSetEnabled(bool enabled)	{ Enable(enabled); }
	MWLua::ret_int2			glueGetPosition();
	void					glueSetPosition(int x, int y);
	void					glueClick()	{ OnClick(NULL); }
	int						glueGetPositionX();
	int						glueGetPositionY();
	void					glueBringToTop();
};


// Help Functions
/// &�� �ִ� ��ġ�� �˷��ش�. szText�� NULL�̸� m_szText���� &�� ��ġ�� ã�´�.
int GetAndPos(const char* szText);
/// & �ڿ� �پ��ִ� ���ڸ� ����.
char GetAndChar(const char* szText);
/// &���� ����
int RemoveAnd(char* szText);
/// &���� ����
/// @param szRemovedText	[out]	���ŵ� ���ڿ�
/// @param szText			[in]	�ҽ� ���ڿ�
int RemoveAnd(char* szRemovedText, const char* szText);
/// &���� ����
/// @param szRemovedFrontText	[out]	&���� ���ڿ�
/// @param cUnderLineChar		[out]	&�� ���� ������ ���� ���ڿ�
/// @param szRemovedBackText	[out]	&���� ���ڿ�
/// @param szText				[in]	�ҽ� ���ڿ�
int RemoveAnd(char* szRemovedFrontText, char* cUnderLineChar, char* szRemovedBackText, const char* szText);

/// ���� ��ġ�� ���� ��ġ�� ��ȯ
MPOINT MClientToScreen(MWidget* pWidget, MPOINT& p);
/// ���� ��ġ�� ���� ��ġ�� ��ȯ
MPOINT MScreenToClient(MWidget* pWidget, MPOINT& p);
/// ���� ������ ���� �������� ��ȯ
MRECT MClientToScreen(MWidget* pWidget, MRECT& p);
/// ���� ������ ���� �������� ��ȯ
MRECT MScreenToClient(MWidget* pWidget, MRECT& p);

/// ���� Ŀ�� ��ġ ���
MPOINT GetCursorPosition(void);

/// ���� ���� Ŭ�������� �˻�
bool MIsSameWidgetClass(MWidget* pWidget, const char* szClassName);
/// ���� ���� Ŭ�������� �˻�
bool MIsSameWidgetClass(MWidget* pWidgetA, MWidget* pWidgetB);


} // namespace mint3