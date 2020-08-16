
#pragma once

#include "Mint.h"
#include "MTreeView.h"
#include "MBitmapDrawer.h"



// namespace : mint3
namespace mint3
{



// MTREEVIEWREGION
enum MTREEVIEWREGION
{
	MTVR_NONE			= -1,		// none
	MTVR_FRAMEBOX		= 0,		// Frame box
	MTVR_SELECTBAR,					// Select bar
	MTVR_MAX
};



// MBTreeViewLook
class MBTreeViewLook : public MTreeViewLook
{
private:
	// �������� ��Ʈ���� �����ϴ� ���� ����
	MBitmapRep			m_Bitmaps[ MTVR_MAX][ 3];		// [Frame, Select bar] [Normal, Focused, Disabled]

	// ��Ʈ�� �׸��� ��Ÿ�� ����
	MDRAWBITMAPSTYLE	m_BitmapStyle;

	// ��Ʈ�� �̹����� Ÿ�������� ����� ���θ� �����ϴ� ����
	bool				m_bTiled;

	// Ŀ���� Look�� ����ϴ��� ���θ� �����ϴ� ����
	bool				m_bCustomLook;

	// ��Ʈ�� �����ϴ� ����
	MFont*				m_pFont;

	// �ؽ�Ʈ ������ �����ϴ� ����
	MCOLOR				m_nFontColor[ MTVR_MAX][ 3];	// [Frame, Select bar] [Normal, Focused, Disabled]




// Member functions
public:
	MBTreeViewLook();
	virtual ~MBTreeViewLook();


	// Ŀ���� Look�� ����ϴ��� ���θ� �����ϴ� �Լ�
	void SetCustomLook( bool bCustom =true)			{ m_bCustomLook = bCustom;			}

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
	void SetTiled( bool bTiled)									{ m_bTiled = bTiled;						}

	// �ؽ�Ʈ ��Ʈ�� �����ϴ� �Լ�
	void SetFont( MFont* pFont)									{ m_pFont = pFont;							}

	// �ؽ�Ʈ ������ �����ϴ� �Լ�
	void SetFontColor( int type, int state, MCOLOR& color)		{ m_nFontColor[ type][ state] = color;		}



protected:
	virtual void OnDrawFrame( MTreeView* pTreeView, MDrawContext* pDC, MRECT& client);
	virtual void OnDrawItem( MTreeView* pTreeView, MTreeItem* pTreeItem, MDrawContext* pDC, MRECT& client, int x, int& y);
	virtual void OnDrawSelectbar( MTreeView* pTreeView, MDrawContext* pDC, MRECT& r);
	virtual MRECT GetClientRect( MTreeView* pTreeView, MRECT& client);
};


} // namespace mint3


