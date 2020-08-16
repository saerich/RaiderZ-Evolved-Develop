#pragma once

#include <set>
#include <string>
using namespace std;

namespace mint3 {

class MWidget;

/// Listener���� ���޵Ǵ� �޼���
enum MEventMsg
{
	MEVENT_UNKNOWN = 0,

	// Widget
	MEVENT_SHOW,				///< �����. ������ �������� �߻��ϴ� �޼���
	MEVENT_HIDE,				///< �����. ������ �������� �߻��ϴ� �޼���
	MEVENT_CLICK,				///< �����. Ŭ���� �� �߻��ϴ� �޼���
	MEVENT_RCLICK,
	MEVENT_ENTER,				///< �����. ��Ŀ���� �� �߻��ϴ� �޼���
	MEVENT_LEAVE,				///< �����. ��Ŀ���� ����� �� �߻��ϴ� �޼���
	MEVENT_DRAW,				///< �����. �׸��� �߻��ϴ� �ż���
	MEVENT_CHAR,
	MEVENT_DOUBLE_CLICK,
	MEVENT_KEY_DOWN,			///< Ű�� �Է����� �� �߻��ϴ� �޼���
	MEVENT_KEY_UP,				///< Ű�� ���� �߻��ϴ� �޼���
	MEVENT_MOUSE_DOWN,			///< ���콺�� ������ �� �߻��ϴ� �޼���
	MEVENT_MOUSE_UP,			///< ���콺�� �� �� �߻��ϴ� �޼���
	

//	MEVENT_SIZE,

	// Timer
	MEVENT_TIMER_TICK,

	// TabCtrl
	MEVENT_TABCTRL_TAB_SELECTED,	///< �� �������� ���õ�. ���� - ���õ� �������� �ε���
	MEVENT_BUTTON_CHECK_CHANGED,	///< ��ư�� üũ�� ����� �� �߻��ϴ� �޼���

	MEVENT_DROP,
	MEVENT_MOUSE_WHEEL,			///<

	// Button
	MEVENT_BUTTON_CLICK,		///< ��ư�� Ŭ���ɶ� Listener���� �߻��Ǵ� �޼���
	MEVENT_BUTTON_IN,			///< ��ư ���� Ŀ���� �÷� ����
	MEVENT_BUTTON_OUT,			///< ��ư ���� Ŀ���� ������ �÷� ����

	// Edit
	MEVENT_EDIT_KEYDOWN,		///< Ű�� �Է�������
	MEVENT_EDIT_CHAR,			///< Ű�� �Է�������
	MEVENT_EDIT_ENTER,			///< EnterŰ�� �Է�������
	MEVENT_EDIT_ESC,			///< ESCŰ�� �Է�������

	// List
	MEVENT_LIST_VALUE_CHANGED,	///< value�� �ٲ� �������� �޼���



	// ComboBox
	MEVENT_CB_SELECTED_INDEX_CHANGED,		///< ������ �׸��� �ٲ���� ��

	// Frame
	MEVENT_FRAME_CLOSE,
	MEVENT_FRAME_MINIMIZE,

	// ListBox
	MEVENT_LB_ITEM_SEL,
	MEVENT_LB_ITEM_SEL2,
	MEVENT_LB_ITEM_DBLCLK,
	MEVENT_LB_ITEM_SELLOST,
	MEVENT_LB_ITEM_DEL,
	MEVENT_LB_ITEM_START,		///< ���۾������� �ٲ�
	MEVENT_LB_ITEM_CLICKOUT,

	// TextArea
	MEVENT_TEXTAREA_ENTER_VALUE,
	MEVENT_TEXTAREA_ESC_VALUE,
	MEVENT_TEXTAREA_LINKEDTEXT,

	// MsgBox
	MEVENT_MSGBOX_OK,
	MEVENT_MSGBOX_CANCEL,
	MEVENT_MSGBOX_YES,
	MEVENT_MSGBOX_NO,

	// FileDialog
	MEVENT_FILEDIALOG_OK,
	MEVENT_FILEDIALOG_CANCEL,

	

	MEVENT_MAX
};

/// �������� �޼����� ���� �� �ִ� Listener(Pure Virtual Class)
class MListener{
public:
	/// �޼����� �ޱ����� Virtual Function
	/// @param	pWidget		�޼����� ������ ���� ������
	/// @param	szMessage	�޼���(��Ʈ��)
	virtual bool OnCommand(MWidget* pWidget, MEventMsg nMsg, const char* szArgs=NULL) = 0;
};

// ��ƿ��� ����ϴ� �̺�Ʈ ���ڰ� - �̺�Ʈ ���ڴ� ���������� �� ��ü�� ������.
class MEventArgs
{
public:
	MEventArgs()
	{
		m_nKeyValue = 0;
		m_nMouseX = m_nMouseY = m_nMouseDelta = 0;
		m_bCtrl = false;
		m_bShift = false;
	}
	~MEventArgs() {}
	void SetFromEvent(MEvent* pEvent)
	{
		m_nKeyValue		= pEvent->nKey;
		m_nMouseX		= pEvent->Pos.x;
		m_nMouseY		= pEvent->Pos.y;
		m_nMouseDelta	= pEvent->nDelta;
		m_bCtrl			= pEvent->bCtrl;
		m_bShift		= pEvent->bShift;
	}

	unsigned int	m_nKeyValue;		///< KeyDown �Ǵ� KeyUp �̺�Ʈ�� ���� Ű���� ��
	int				m_nMouseX;
	int				m_nMouseY;
	int				m_nMouseDelta;			// Wheel Mouse Delta
	bool			m_bCtrl;
	bool			m_bShift;
};


class MDragEventArgs
{
private:
	char m_szString[256];
	char m_szItemString[256];
public:
	MDragEventArgs()
	{
		m_nX = m_nY = 0;
		m_szString[0] = 0;
		m_szItemString[0] = 0;
	}
	~MDragEventArgs() {}
	void Set(MWidget* pSender, int x, int y, bool bCtrl, const char* szString, const char* szItemString)
	{
		this->m_pSender = pSender;
		this->m_nX = x;
		this->m_nY = y;
		this->m_bCtrl = bCtrl;

		strcpy_s(m_szString, szString);
		strcpy_s(m_szItemString, szItemString);
	}

	MWidget*	m_pSender;
	int			m_nX;
	int			m_nY;
	bool		m_bCtrl;

	const char* GetString()		{ return m_szString; }
	const char* GetItemString()	{ return m_szItemString; }
};


class MLinkedTextArgs
{
private:
	char m_szString[ 256];
	char m_szArgument[ 256];
public:
	MLinkedTextArgs()
	{
		m_szString[ 0] = 0;
		m_szArgument[ 0] = 0;
	}

	void Set( const char* szString, const char* szArgument)
	{
		strcpy_s( m_szString, szString);
		strcpy_s( m_szArgument, szArgument);
	}

	const char* GetString()			{ return m_szString;		}
	const char* GetArgument()		{ return m_szArgument;		}
};


class MEventHandler
{
protected:
	MListener*		m_pListener;
	set<MEventMsg>	m_ScriptEvents;
	void			DoScriptEvent(MWidget* pWidget, MEventMsg nMsg, const char* szArgs);
public:
	MEventHandler() : m_pListener(NULL) {}
	virtual bool OnCommand(MWidget* pWidget, MEventMsg nMsg, const char* szArgs=NULL);
	void SetListener(MListener* pListener) { m_pListener = pListener; }
	MListener* GetListener() { return m_pListener; }
	bool ExistScriptEvent(MEventMsg nMsg);
	void SetFlagScriptEvents(MEventMsg nMsg) { m_ScriptEvents.insert(nMsg); }
};

MEventMsg FindEventMsgFromName(const char* szEventName);

} // namespace mint3