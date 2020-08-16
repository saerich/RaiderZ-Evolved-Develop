#pragma once

#include "MWidget.h"
#include "MLookNFeel.h"

namespace mint3 {

class MMsgBox;
class MButtonGroup;

/// ��ư Ÿ��
enum MButtonType{
	MBT_NORMAL = 0,		///< �Ϲ� ��ư
	MBT_CHECKBTN,		///< üũ ��ư
	MBT_PUSHBTN,		///< Ǫ�� ��ư
};

/// ����Ʈ Ű ���
enum MButtonKeyAssigned{
	MBKA_NONE = 0,		///< �ƹ�Ű�� ��ϵǾ� ���� ���� ��ư
	MBKA_ENTER,			///< Enter Ű ������ ����Ǵ� ��ư
	MBKA_ESC,			///< ESC Ű ������ ����Ǵ� ��ư
};

/// ��ư ����
enum MButtonState
{
	MBT_STATE_NOT = -1,
	MBT_STATE_NORMAL = 0,	// �Ϲ�
	MBT_STATE_OVER,			// ���콺�� ����������
	MBT_STATE_PRESS			// ���콺�� ������ ��
};

#define CHECKBOX_SIZE  (int)(12 * Mint::GetInstance()->GetRatioToStandardScreenHeight())

class MButton;
/// Button�� Draw �ڵ尡 �ִ� Ŭ����, �� Ŭ������ ��ӹ޾Ƽ� Ŀ���ҷ��� ���� �� �ִ�.
class MButtonLook{
protected:
	bool	m_bWireLook;
protected:
	virtual void OnDownDraw(MButton* pButton, MDrawContext* pDC);
	virtual void OnUpDraw(MButton* pButton, MDrawContext* pDC);
	virtual void OnOverDraw(MButton* pButton, MDrawContext* pDC);
	virtual void OnDisableDraw(MButton* pButton, MDrawContext* pDC);
	virtual void OnCheckBoxDraw(MButton* pButton, MDrawContext* pDC, bool bPushed);
public:
	WIDGET_UI_LOOK_TYPE(MWIDGET_MBUTTONLOOK)
	virtual void OnDrawText(MButton* pButton, MRECT& r, MDrawContext* pDC);
	virtual void OnDraw(MButton* pButton, MDrawContext* pDC);
	virtual MRECT GetClientRect(MButton* pButton, MRECT& r);
	void SetWireLook(bool b) {	m_bWireLook = b;	}
	bool GetWireLook() const{	return m_bWireLook; }

public:
	MButtonLook():m_bWireLook(false){}
	virtual ~MButtonLook() {}
};


/// ��ư Ŭ����
class MButton : public MWidget{
protected:
	bool		m_bMouseOver;
	MCOLOR		m_TextColor;
	bool		m_bShowText;
	MAlignmentMode	m_AlignmentMode;
	MButtonType		m_Type;
	bool		m_bChecked;				///< MBT_CHECKBTN�� ���
	bool		m_bComboButton;			///< ComboBox ��ư�� ���
	bool		m_bStretch;
	
	MButtonGroup	*m_pButtonGroup;	///< �����ִ� �׷�
	int				m_nIndexInGroup;	///< ���� �׷쳻������ index

	string			m_strUserData;

public:
	bool		m_bEnableEnter;			///< ��Ŀ�� ������ EnterŰ�� ��ư ������ Ȱ��ȭ ( Default : true )
	bool		m_bHighlight;			///< Highlight(&) ����
	MBitmap*	m_pIcon;				///< ������
	MButtonKeyAssigned	m_nKeyAssigned;	///< Key Assigned
	MMsgBox*	m_pMsgBox;				///< �޼��� �ڽ��� ���� Ȯ�� �޼���

	MPOINT		m_ClickPos;

/*
	MPOINT		m_LDragStartClickPos;
	int			m_LDragVariationX;
	int			m_LDragVariationY;
*/
private:
	//void DrawButton(MDrawContext* pDC);
protected:
	//virtual void OnDraw(MDrawContext* pDC);
	/// ���콺�� ��ư �ȿ� ���ö� �߻��ϴ� �̺�Ʈ �ڵ鷯
	virtual void OnMouseIn(void);
	/// ���콺�� ��ư ������ ���� ������ �߻��ϴ� �̺�Ʈ �ڵ鷯
	virtual void OnMouseOut(void);
	virtual bool OnEvent(MEvent* pEvent, MListener* pListener);

	virtual void OnShow(void);
	virtual void OnHide(void);
	/// üũ�� ����� ��
	virtual void OnCheckChanged(bool bChecked);
	virtual void OnClick(MEvent* pEvent);
public:
	MButton(const char* szName=NULL, MWidget* pParent=NULL, MListener* pListener=NULL);
	virtual ~MButton(void);
	/// ��ư �ؽ�Ʈ �÷� ����
	void SetTextColor(MCOLOR color);
	/// ��ư �ؽ�Ʈ �÷� ���
	MCOLOR GetTextColor(void);
	/// ��ư�� �ؽ�Ʈ �����ֱ�
	void ShowText(bool bShow=true);
	virtual bool DefaultCommand(void);

	/// ��ư�� �ؽ�Ʈ ���� ���
	MAlignmentMode GetAlignment(void);
	/// ��ư�� �ؽ�Ʈ ���� ����
	virtual MAlignmentMode SetAlignment(MAlignmentMode am);

	/// ��ư Ÿ�� ����
	void SetType(MButtonType t);
	/// ��ư Ÿ�� ���
	MButtonType GetType(void);

	/// Push ��ư�� ��� üũ
	void SetCheck(bool bCheck);
	/// Push ��ư�� ��� üũ�� ���
	bool GetCheck(void);

	/// ��ư�� ������ �ִ°�? ( ���콺�� ������ ���¸� �� �� �ִ�. )
	bool IsButtonDown(void);	// Async Button Down ?
	/// ���콺�� ��ư ���� �÷��� �ִ°�?
	bool IsMouseOver(void);

	/// Confirm Message Box
	void SetConfirmMessageBox(const char* szMessage);

	virtual bool OnCommand(MWidget* pWidget, MEventMsg nMsg, const char* szArgs=NULL);

	//int GetLDragVariationX() {
	//	return m_LDragVariationX;
	//}

	//int GetLDragVariationY() {
	//	return m_LDragVariationY;
	//}

	DECLARE_LOOK(MButtonLook)
	DECLARE_LOOK_CLIENT()

	bool IsComboButton()		{ return m_bComboButton;		}

	void SetStretch( bool b) {
		m_bStretch = b;
	}

	bool GetStretch( ) const {
		return m_bStretch;
	}

	void SetButtonGroup(MButtonGroup *pGroup);

	int GetIndexInGroup() { 
		return m_nIndexInGroup; 
	}

	void ReleaseButtonGroup()
	{
		m_pButtonGroup = NULL;
		m_nIndexInGroup = 0;
	}

	void			SetUserData(const char* szUserData);
	const char*		GetUserData();

	const char*		glueGetUserData();
	void			glueSetUserData(const char* szUserData );
	bool			glueIsCheck();
	bool			glueIsButtonDown();
	void			glueSetCheck(bool bCheck);
	void			glueSetConfirmMsg( const char* szMessage );

#define MINT_BUTTON	"Button"
	virtual const char* GetClassName(void){ return MINT_BUTTON; }
};

} // namespace mint3
