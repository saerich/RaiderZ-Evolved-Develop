#pragma once

#include "MintTypes.h"
#include "MBitmapDrawer.h"
#include "MResourceManager.h"
#include <string>
using namespace std;


namespace mint3
{


/// class : MLookStateInfo
class MLookStateInfo
{
/// Member variables
protected:
	MRECT*				m_pAssign;			///< ��Ʈ�� assignment�� �����ϴ� ���� ����
	bool*				m_pTiled;			///< ��Ʈ�� ����� Ÿ�� �������� ���� ����
	MCOLOR*				m_pFontColor;		///< �ؽ�Ʈ ������ �����ϴ� ����


/// Member functions
public:
	MLookStateInfo();
	virtual ~MLookStateInfo();

	void SetAssignRect( int index, MRECT& r, bool bTile);
	MRECT* GetAssignRect();

	void SetFontColor( MCOLOR& color);
	MCOLOR GetFontColor() const;

	bool IsValidAssign() const						{ return ( m_pAssign ? true : false);		}
	bool IsValidFontColor() const					{ return ( m_pFontColor ? true : false);	}
};






/// class : MLookPartsInfo
class MLookPartsInfo
{
/// Member variables
protected:
	map< string, MLookStateInfo*>		m_StateInfo;


/// Member functions
public:
	MLookPartsInfo();
	virtual ~MLookPartsInfo();

	MLookStateInfo* FindState( const string& strState);

	void SetAssignRect( const string& strState, int index, MRECT& r, bool bTile);
	MRECT* GetAssignRect( const string& strState);
	bool IsTiledBmp( const string& strState);

	void SetFontColor( const string& strState, MCOLOR& color);
	MCOLOR GetFontColor( const string& strState);

	bool IsValidAssign( const string& strState);
	bool IsValidFontColor( const string& strState);
};






/// class : MLookBase
class MLookBase
{
/// Member variables
protected:
	string								m_strName;				///< �̸�
	map< string, MLookPartsInfo*>		m_PartsInfo;			///< ���� ����
	MBitmap*							m_pBitmap;				///< ��Ʈ��
	MDRAWBITMAPSTYLE					m_BitmapStyle;			///< ��Ʈ�� �׸��� ��Ÿ�� ����
	MFont*								m_pFont;				///< Look���� ����ϴ� ��Ʈ
	int									m_nTextAlign;			///< �ؽ�Ʈ ����
	MRECT								m_WorkRectOffset;		///< �۾� ���� ������
	MRECT								m_BkgrndRectOffset;		///< ��� Look�� ������ ������ ���� ���� ��� ���� ������ ������ ��
	MEventHandler						m_EventHandler;			///< ��ũ��Ʈ �̺�Ʈ �ڵ鷯
	static MWidget*						m_pWindow;				///< ȣ�� ������ ������




/// Member functions
public:
	MLookBase();
	virtual ~MLookBase();

	void SetName( const char* szName)							{ m_strName = szName;										}
	const char* GetName() const									{ return m_strName.c_str();									}

	MLookPartsInfo* FindPart( const string& strPart);

	void SetBitmap( const string& strName)						{ m_pBitmap = MBitmapManager::Get( strName.c_str());		}
	MBitmap* GetBitmap()										{ return m_pBitmap;											}

	void SetAssignRect( const string& strPart, const string& strState, int index, MRECT& r, bool bTile =false);
	MRECT* GetAssignRect( const string& strPart, const string& strState);
	bool IsTiledBmp( const string& strPart, const string& strState);

	void SetStretchBlt( bool bStretch =true)					{ m_BitmapStyle = bStretch ? DRAWBITMAP_STRETCH : DRAWBITMAP_NORMAL;	}
	bool IsStretchBlt() const									{ return ((m_BitmapStyle == DRAWBITMAP_STRETCH) ? true : false);		}

	void SetBitmapStyle( MDRAWBITMAPSTYLE style)				{ m_BitmapStyle = style;									}
	MDRAWBITMAPSTYLE GetBitmapStyle() const						{ return m_BitmapStyle;										}

	void SetFont( MFont* pFont)									{ m_pFont = pFont;											}
	MFont* GetFont()											{ return (m_pFont ? m_pFont : MFontManager::Get( NULL));	}

	void SetFontColor( const string& strPart, const string& strState, MCOLOR& color);
	MCOLOR GetFontColor( const string& strPart, const string& strState, MCOLOR& _default =MCOLOR(255,255,255));

	void SetTextAlign( int align)								{ m_nTextAlign = align;										}
	int GetTextAlign() const									{ return m_nTextAlign;										}

	void SetWorkRectOffset( MRECT& offset)						{ m_WorkRectOffset = offset;								}
	MRECT GetWorkRectOffset() const								{ return m_WorkRectOffset;									}
	MRECT GetWorkRect( MWidget* pWidget) const;

	void SetBkgrndRectOffset( MRECT& offset)					{ m_BkgrndRectOffset = offset;								}
	MRECT GetBkgrndRectOffset() const							{ return m_BkgrndRectOffset;								}
	MRECT GetBkgrndRect( MWidget* pWidget) const;

	void BindScriptEvent( string& strEventName, string& strScript);


	// �ش� ��Ʈ�� ��Ʈ�� assign ������ ��ȿ���� Ȯ���Ѵ�
	bool IsValidAssign( const string& strPart, const string& strState);

	// �ش� ��Ʈ�� ��Ʈ ���� ������ ��ȿ���� Ȯ���Ѵ�
	bool IsValidFontColor( const string& strPart, const string& strState);

	// �ش� ��Ʈ�� �׸���
	bool Draw( MDrawContext* pDC, MRECT& rect, const string& strPart, const string& strState);


/// Event handler
public:
	virtual bool OnEvent( MWidget* pWidget, MEventMsg nEvent);
	virtual void OnDrawBorder( MWidget* pWidget, MDrawContext* pDC);
	virtual void OnDraw( MWidget* pWidget, MDrawContext* pDC);
};




} // namespace mint3

