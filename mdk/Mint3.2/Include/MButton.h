#pragma once

#include "MWidget.h"
#include "MLookBase.h"


namespace mint3
{

class MButton;
class MButtonGroup;


/// ��ư Ÿ��
enum MButtonType
{
	MBT_NORMAL = 0,		///< �Ϲ� ��ư
	MBT_CHECKBTN,		///< üũ ��ư
	MBT_PUSHBTN,		///< Ǫ�� ��ư
};






/// class : MButtonLook
class MButtonLook : public MLookBase
{
/// Member variables
protected:
	MPOINT				m_ptTextOffset;				///< ���� ��ư�� �ؽ�Ʈ�� �̵��ϴ� ������ ���� �����ϴ� ����
	bool				m_bShowHotKey;				///< ����Ű ǥ��



/// Member functions
public:
	MButtonLook();

	void SetTextOffset( int x, int y)				{ m_ptTextOffset = MPOINT( x, y);	}
	MPOINT GetTextOffset() const					{ return m_ptTextOffset;			}

	void ShowHotKey( bool bShow)					{ m_bShowHotKey = bShow;			}
	bool IsShowHotKey() const						{ return m_bShowHotKey;				}

	virtual void OnDrawBorder( MWidget* pWidget, MDrawContext* pDC) override;
	virtual void OnDraw( MWidget* pWidget, MDrawContext* pDC) override;


protected:
	virtual void OnDrawText( MButton* pButton, MRECT& r, MDrawContext* pDC);
	virtual void OnDrawSelectbar( MButton* pButton, MRECT& r, MDrawContext* pDC);
};









/// class : MButton
class MButton : public MWidget
{
/// Member variables
protected:
	MButtonType			m_Type;					///< Type
	bool				m_bChecked;				///< MBT_CHECKBTN�� ���
	unsigned int		m_nHotKey;				///< HotKey

	MBitmap*			m_pIcon;				///< Icon image
	MSIZE				m_IconSize;				///< Icon size
	
	MButtonGroup		*m_pButtonGroup;		///< �����ִ� �׷�
	int					m_nIndexInGroup;		///< ���� �׷쳻������ index

	bool				m_bButtonDown;			///< Button down

	DWORD				m_dwUserData;			///< User data

	bool				m_bBlanking;			///< Blanking

	bool				m_bEnableEnter;			///< ��Ŀ�� ������ EnterŰ�� ��ư ������ Ȱ��ȭ ( Default : true )



// Member functions
public:
	MButton( const char* szName =NULL, MWidget* pParent =NULL, MListener* pListener =NULL);
	virtual ~MButton();

	// ���� ����
	virtual const char* GetState() override;
	virtual int GetStateNum() override;
	
	/// ��ư Ÿ�� ����
	void SetType( MButtonType nType)					{ m_Type = nType;					}
	MButtonType GetType() const							{ return m_Type;					}

	/// Push ��ư�� ��� üũ
	void SetCheck( bool bCheck);
	bool GetCheck() const								{ return m_bChecked;				}

	/// HotKey ����
	void SetHotKey( unsigned int nHotKey)				{ m_nHotKey = nHotKey;				}
	unsigned int GetHotKey() const						{ return m_nHotKey;					}

	/// Icon ����
	void SetIcon( MBitmap* pIcon, MSIZE& size);
	void SetIcon( MBitmap* pIcon)						{ m_pIcon = pIcon;					}
	MBitmap* GetIcon() const							{ return m_pIcon;					}
	void SetIconSize( MSIZE& size)						{ m_IconSize = size;				}
	MSIZE GetIconSize() const							{ return m_IconSize;				}
	
	/// ��ư�� ������ �ִ°�? ( ���콺�� ������ ���¸� �� �� �ִ�. )
	bool IsButtonDown()									{ return ((m_bButtonDown==true && IsMouseOver()) ? true : false);	}

	// Button group
	void SetButtonGroup( MButtonGroup *pGroup);
	int GetIndexInGroup()								{ return m_nIndexInGroup;			}
	void ReleaseButtonGroup();

	// Set user data
	void SetUserData( DWORD dwUserData)					{ m_dwUserData = dwUserData;		}
	DWORD GetUserData()									{ return m_dwUserData;				}

	// Set blanking
	void SetBlanking( bool bBlanking)					{ m_bBlanking = bBlanking;			}



// Event handler
protected:
	virtual bool OnEvent( MEvent* pEvent, MListener* pListener) override;
	virtual bool OnQueryHotKey( unsigned int nHotKey) override;
	virtual void OnHotKey( unsigned int nHotKey) override;
	virtual bool OnCommand( MWidget* pWidget, MEventMsg nMsg, const char* szArgs =NULL) override;
	virtual bool OnSetCursor( MEvent* pEvent, string* pCursor) override;



// Lua glue functions
public:
	void				glueSetCheck( bool bCheck)					{ SetCheck( bCheck);					}
	bool				glueGetCheck()								{ return GetCheck();					}
	void				glueSetHotKey( int nHotKey)					{ SetHotKey( nHotKey);					}
	int					glueGetHotKey()								{ return GetHotKey();					}
	void				glueSetIcon( const char* szIcon);
	const char*			glueGetIcon();
	void				glueSetIconSize( int w, int h)				{ SetIconSize( MSIZE( w, h));			}
	MWLua::ret_int2		glueGetIconSize() const						{ return MWLua::ret_int2( m_IconSize.w, m_IconSize.h);	}
	void				glueSetUserData( DWORD dwUserData)			{ SetUserData( dwUserData);				}
	DWORD				glueGetUserData()							{ return m_dwUserData;					}



#define MINT_BUTTON	"Button"
	virtual const char* GetClassName() override			{ return MINT_BUTTON;				}
};



} // namespace mint3
