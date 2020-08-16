#pragma once

#include "MWidget.h"
#include "MScrollBar.h"
#include "MLookNFeel.h"
#include <vector>
using namespace std;


namespace mint3
{

#define MTREEINDENT		15
	

class MTreeView;
class MTreeItem;
class MTreeItemEx;



// MTreeHitRgn
enum MTreeHitRgn
{
	MHTNOWHERE		= 0,		// ��Ʈ ���� ����
	MHTEXPAND,					// Ȯ�� ��ư ����
	MHTSTRING,					// ��Ʈ�� ����
	MHTICON						// ������ ����
};


// struct : MTREEHITINFO
struct MTREEHITINFO
{
	MTreeItem*		m_pItem;
	int				m_nHitRegion;
	MPOINT			m_ptHitPos;

	MTREEHITINFO() : m_pItem( NULL), m_nHitRegion( MHTNOWHERE), m_ptHitPos( MPOINT( 0, 0))	{ }
};





// class : MTreeItem
class MTreeItem
{
// Member variables
protected:
	vector< MTreeItem*>		m_ChildList;		// Child list
	string					m_strString;		// String of item
	int						m_nIconIndex;		// Index of icon
	int						m_nData;			// Data of item
	bool					m_bExpand;			// Expand


// Member functions
public:
	MTreeItem();
	MTreeItem( const char* szString, int nIconIndex);
	virtual ~MTreeItem();

	size_t			GetRootItemCount();
	MTreeItem*		GetRootItem( size_t index);
	
	const char*		GetString();
	int				GetIcon() const;
	int				GetUserData() const;
	bool			IsExpanded() const;
	bool			HasChild() const;
	int				GetCount() const;
	virtual MFont*	GetFont()														{ return NULL;			}
	virtual MCOLOR	GetFontColor( MCOLOR def_color =MCOLOR( 255, 255, 255)) const	{ return def_color;		}
	virtual int		GetHeight() const												{ return 0;				}


protected:
	bool			InsertItem( MTreeItem* pItem);
	bool			DeleteItem( MTreeItem* pItem);

	void			SetString( const char* szString);
	void			SetIcon( int index);
	void			SetUserData( int data);
	void			SetExpand( bool bExpand);
	virtual void	SetFont( MFont* pFont)			{ }
	virtual void	SetFontColor( MCOLOR& color)	{ }
	virtual void	SetHeight( int height)			{ }

	int				GetVisibledItemHeight( int nDefaultHeight);
	MTreeItem* 		GetItemPos( int nDefaultHeight, MPOINT& pos, int x, int& y);					// �ش� ��ġ�� �ִ� ������ �����͸� ���Ѵ�
	bool			GetItemPos( MTreeItem* pItem, int nDefaultHeight, MPOINT& pos, int x, int& y);	// �ش� �������� ��ġ ��ǥ�� ���Ѵ�
	MTREEHITINFO	GetSelItem( int nDefaultHeight, MPOINT& pos, int dx, int& dy);					// �ش� ��ǥ�� �ִ� ������ ������ ���Ѵ�


public:
	friend MTreeView;
};



// class : MTreeItemEx (MTreeItem Ȯ����)
class MTreeItemEx : public MTreeItem
{
// Member variables
protected:
	MFont*					m_pFont;			// Font of item string
	MCOLOR					m_nFontColor;		// Color of font
	unsigned char			m_nHeight;			// Height of item column


// Member functions
public:
	MTreeItemEx();
	MTreeItemEx( const char* szString, int nIconIndex =0, const char* szFont =NULL, MCOLOR nFontColor =MCOLOR( 255, 255, 255), unsigned char nHeight =0);
	virtual ~MTreeItemEx();


public:
	virtual MFont*	GetFont();
	virtual MCOLOR	GetFontColor( MCOLOR def_color =MCOLOR( 255, 255, 255)) const;
	virtual int		GetHeight() const;


protected:
	virtual void	SetFont( MFont* pFont);
	virtual void	SetFontColor( MCOLOR& color);
	virtual void	SetHeight( int height);


public:
	friend MTreeView;
};



// class : MTreeViewLook
class MTreeViewLook
{
public:
	MTreeViewLook();

	virtual void OnDraw( MTreeView* pTreeView, MDrawContext* pDC);
	virtual void OnDrawFrame( MTreeView* pTreeView, MDrawContext* pDC, MRECT& client);
	virtual void OnDrawItem( MTreeView* pTreeView, MTreeItem* pTreeItem, MDrawContext* pDC, MRECT& client, int x, int& y);
	virtual void OnDrawSelectbar( MTreeView* pTreeView, MDrawContext* pDC, MRECT& r);
	virtual MRECT GetClientRect( MTreeView* pTreeView, MRECT& client);

protected:
	int DrawColumnRatioBmp( MDrawContext* pDC, MBitmap* bmp, int x, int y, int column, int align);
};


// MNullTreeViewLook
class MNullTreeViewLook : public MTreeViewLook
{
protected:
	bool		m_bCustomLook;

public:
	virtual void OnDrawFrame( MTreeView* pTreeView, MDrawContext* pDC, MRECT& client)		{ }
};





// class : MTreeVeiw
class MTreeView : public MWidget
{
// Member variables
protected:
	MScrollBar*				m_pScrollbar;			// Scrollbar
	int						m_nScrollbarVisible;

	vector< MTreeItem*>		m_vItemList;			// Item list
	MTreeItem*				m_pSelItem;				// Selected item
	int						m_nItemHeight;			// Height of item
	MBitmap*				m_pImage[ 2];			// Collapsed/Expanded Image
	vector< MBitmap*>		m_vIcon;				// Icons
	MAlignmentMode			m_nAlign;				// Align

	bool					m_bScrollUpdate;



// Member functions
public:
	MTreeView( const char* szName =NULL, MWidget* pParent =NULL, MListener* pListener =NULL);
	virtual ~MTreeView();

	// ������ �߰� �� ����
	MTreeItem* InsertItem( const char* szText, int nIconIndex =0, MTreeItem* pParentItem =NULL);
	MTreeItem* InsertItem( MTreeItem* pItem, MTreeItem* pParentItem =NULL);
	MTreeItem* InsertItemEx( const char* szText, int nIconIndex =0, const char* szFont =NULL, MCOLOR nColor =MCOLOR( 255, 255, 255), unsigned char nHeight =0, MTreeItem* pParentItem =NULL);
	bool DeleteItem( MTreeItem* pItem);

	// ��� ������ ����
	void ResetContents();

	// ��Ʈ�� ��ϵ� ������ ������ ����
	size_t GetRootItemCount();

	// ��Ʈ�� ��ϵ� �������� ����
	MTreeItem* GetRootItem( size_t index);


	// ������ Ȯ��
	void Expand( MTreeItem* pItem, bool bExpand);

	// ������ ��Ʈ�� ����
	void SetItemString( MTreeItem* pItem, const char* szString);

	// ������ ������ ����
	void SetItemIcon( MTreeItem* pItem, int index);

	// ������ ������ ����
	void SetUserData( MTreeItem* pItem, int data);

	// ������ ��Ʈ ����
	void SetItemFont( MTreeItem* pItem, MFont* pFont);

	// ������ ��Ʈ ���� ����
	void SetItemFontColor( MTreeItem* pItem, MCOLOR& color);

	// ������ ���� ���� �� ����
	void SetItemHeight( MTreeItem* pItem, int height);
	void SetItemHeight( int height);
	int GetItemHeight();

	// Collapsed, Expand �̹��� ����
	void SetCollapsedImage( const char* szAliasName);
	void SetCollapsedImage( MBitmap* pBitmap);
	MBitmap* GetCollapsedImage();
	void SetExpandedImage( const char* szAliasName);
	void SetExpandedImage( MBitmap* pBitmap);
	MBitmap* GetExpandedImage();

	// ������ ����
	int AddIcon( const char* szAliasName);
	int AddIcon( MBitmap* pIcon);
	MBitmap* GetIcon( size_t index);
	size_t GetIconCount();
	void ClearIcon();

	// �������� �����ϰų� ���õ� �������� ����
	void SelectItem( MTreeItem* pItem);
	MTreeItem* GetSelectedItem();

	// ������ ���� �� ����
	void SetAlign( int align);
	MAlignmentMode GetAlign() const;

	// ������ ������ ����
	int GetCount();

	// ���� ���̴� �����۵��� ���̸� ����
	int GetVisibledItemHeight();


	// Scrollbar
	void ShowScrollbar( int mode);
	void SetScrollbarLook( MScrollBarLook* pLook);
	void SetScrollbarWidth( int nWidth);
	MScrollBar* GetScrollbar();
	void ScrollPage( int delta);

	// �۾� ������ ũ�⸦ ����
	MRECT GetWorkRect();


	// ������ ����
	void SelPrevItem( bool bPostEvent =true);
	void SelNextItem( bool bPostEvent =true);
	void SelPrevPageItem( bool bPostEvent =true);
	void SelNextPageItem( bool bPostEvent =true);
	void SelFirstItem( bool bPostEvent =true);
	void SelLastItem( bool bPostEvent =true);


	// ������ �������� ȭ�� �ȿ� ��ġ�ϵ��� ��
	void EnsureSelItemVisible();


protected:
	void DrawItem( MDrawContext* pDC, MRECT& rect, int x, int& y, MTreeItem* pItem);

	// ��ũ�ѹٸ� ������Ʈ �ϴ� �Լ�
	void UpdateScrollbar();

	// ������ ��ġ�� �ش��ϴ� �������� �����͸� ����
	MTREEHITINFO FindSelItemPos( MPOINT& pPos);



// Message handler
protected:
	virtual bool OnEvent( MEvent* pEvent, MListener* pListener);
	virtual bool OnCommand( MWidget* pWidget, MEventMsg nMsg, const char* szArgs =NULL);
	virtual void OnRun( void);
//	virtual void OnDraw( MDrawContext* pDC);


// Declare look
protected:
	DECLARE_LOOK( MTreeViewLook)
	DECLARE_NULL_LOOK( MNullTreeViewLook)
	DECLARE_LOOK_CLIENT()



#define MINT_TREEVIEW	"TreeView"
	virtual const char* GetClassName( void)		{ return MINT_TREEVIEW;		}
};


} // namespace mint3
