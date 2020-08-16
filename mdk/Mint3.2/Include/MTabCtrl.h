#pragma once

#include "MWidget.h"
#include "MPanel.h"
#include "MButtonGroup.h"
#include <list>
using namespace std;


namespace mint3
{



/// MTabItem
class MTabItem 
{
public:
	MButton		*m_pButton;
	MPanel		*m_pPanel;
	MListener	*m_pListener;

public:
	MTabItem()
	{
		m_pButton = NULL;
		m_pPanel = NULL;
		m_pListener = NULL;
	}

	MTabItem( MButton *pButton, MPanel *pPanel, MListener *pListener)
	{
		m_pButton = pButton;
		m_pPanel = pPanel;
		m_pListener = pListener;
	}
};



/// class : MTabList
class MTabList : public list< MTabItem*>	{};






//// MTabCtrl
class MTabCtrl : public MWidget 
{
/// Member variables
protected:
	MTabList		m_TabList;
	int				m_nCurSel;
	MButtonGroup	m_ButtonGroup;


/// Member functions
public:
	MTabCtrl( const char* szName, MWidget* pParent =NULL, MListener* pListener =NULL);
	virtual ~MTabCtrl();
		
	void Add( MButton *pButton, MPanel *pPanel);
	void RemoveAll();
	int GetCount() const						{ return m_TabList.size();		}

	int SetCurSel( int index);
	int GetCurSel() const						{ return m_nCurSel;				}

	void UpdateListeners();						///< ���� ��ư�� �����ʰ� �ٲ�ٸ� ������� �ҷ��־�� �Ѵ�.



/// Event handler
protected:
	virtual bool OnCommand( MWidget* pWidget, MEventMsg nMsg, const char* szArgs =NULL) override;



// LUA script
public:
	int		glueGetCount();
	int		glueGetSelIndex();
	int		glueSetSelIndex(int index);
	bool	glueSetSelTabName(const char* szTabName);
	bool	glueSetTabName( int index, const char* szTabName);

	


#define MINT_TABCTRL	"TabCtrl"
	virtual const char* GetClassName()			{ return MINT_TABCTRL;				}
};


} // namespace mint3

