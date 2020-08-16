#pragma once

#include <list>
using namespace std;

#include "MWidget.h"
#include "MLookNFeel.h"
#include "MPanel.h"
#include "MButtonGroup.h"

namespace mint3 {


class MTabCtrl;
class MDrawContext;
//class MButton;


/// MTabCtrl�� Draw �ڵ尡 �ִ� Ŭ����, �� Ŭ������ ��ӹ޾Ƽ� Ŀ���ҷ��� ���� �� �ִ�.
class MTabCtrlLook
{
public:
	virtual void OnDraw(MTabCtrl* pTabCtrl, MDrawContext* pDC);
	virtual MRECT GetClientRect(MTabCtrl* pTabCtrl, MRECT& r);
};

class MNullTabCtrlLook : public MTabCtrlLook
{
public:
	virtual void OnDraw(MTabCtrl* pTabCtrl, MDrawContext* pDC) {}
};

//// MTabItem
class MTabItem 
{
public:
	MPanel		*pPanel;
	MButton		*pButton;
	MListener	*pButtonListener;		// ��ư�� ���� ������

	MTabItem(MPanel *pPanel, MButton *pButton, MListener *pListener);
};

class MTabList : public list<MTabItem*> {};


//// MTabCtrl
class MTabCtrl : public MWidget 
{
protected:
	int				m_nCurrentSel;
	MTabList		m_TabList;
	MButtonGroup	m_ButtonGroup;

protected:
	virtual bool OnEvent(MEvent* pEvent, MListener* pListener);

public:
	MTabCtrl(const char* szName, MWidget* pParent=NULL, MListener* pListener=NULL);
	MTabCtrl(MWidget* pParent=NULL, MListener* pListener=NULL);
	virtual ~MTabCtrl(void);
		
	void Add(MButton *pButton, MPanel *pPanel);		// pButton �� ������ pFrame �� ��������
	void RemoveAll();

	int GetCount(void);
	int GetSelIndex(void);
	bool SetSelIndex(int i);

	void UpdateListeners();			// ���� ��ư�� �����ʰ� �ٲ�ٸ� ������� �ҷ��־�� �Ѵ�.

	int		glueGetCount();
	int		glueGetSelIndex();
	bool	glueSetSelIndex(int index);
	bool	glueSetSelTabName(const char* szTabName);

	virtual bool OnCommand(MWidget* pWidget, MEventMsg nMsg, const char* szArgs=NULL);
public:
	DECLARE_LOOK(MTabCtrlLook)
	DECLARE_NULL_LOOK(MNullTabCtrlLook)
	DECLARE_LOOK_CLIENT()
	

	#define MINT_TABCTRL	"TabCtrl"
	virtual const char* GetClassName(void){ return MINT_TABCTRL; }

public:
	
};


} // namespace mint3