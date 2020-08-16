#include "stdafx.h"
#include "MTabCtrl.h"
#include "MColorTable.h"
#include "MButton.h"

namespace mint3 {

//// MTabCtrlLook ///////////////////////////////////////////////////
IMPLEMENT_LOOK(MTabCtrl, MTabCtrlLook)
IMPLEMENT_NULL_LOOK(MTabCtrl, MNullTabCtrlLook)

void MTabCtrlLook::OnDraw(MTabCtrl* pTabCtrl, MDrawContext* pDC)
{
	MRECT r					= pTabCtrl->GetInitialClientRect();
	const int nButtonHeight = 24;

	r.y						+= nButtonHeight;
	r.h						-= nButtonHeight;

	// client edge
#ifdef _DEBUG
	if ( string( pTabCtrl->GetText()) == string( "show rect"))
	{
		pDC->SetColor( 255, 255, 255, (int)(25.0 * sin( (double)timeGetTime() * 0.005) + 25.0));
		pDC->Rectangle( 0, 0, pTabCtrl->GetRect().w, pTabCtrl->GetRect().h);
	}
#endif
}

MRECT MTabCtrlLook::GetClientRect(MTabCtrl* pTabCtrl, MRECT& r)
{
	MRECT t = r;
	t.EnLarge(-1);
	return t;
}


//// MTabItem ///////////////////////////////////////////////////////
MTabItem::MTabItem(MPanel *pPanel, MButton *pButton, MListener *pListener)
{
	MTabItem::pPanel = pPanel;
	MTabItem::pButton = pButton;
	MTabItem::pButtonListener= pListener;
}


//// MTabCtrl ///////////////////////////////////////////////////////
MTabCtrl::MTabCtrl(const char* szName, MWidget* pParent, MListener* pListener) : MWidget(szName, pParent, pListener)
{
	LOOK_IN_CONSTRUCTOR()
}

MTabCtrl::MTabCtrl(MWidget* pParent, MListener* pListener) : MWidget("TabCtrl", pParent, pListener)
{
	LOOK_IN_CONSTRUCTOR()
}

MTabCtrl::~MTabCtrl(void)
{
	RemoveAll();
}

void MTabCtrl::Add(MButton *pButton, MPanel *pPanel)		// pButton �� ������ pFrame �� ��������
{
	MTabItem *pItem = new MTabItem(pPanel, pButton, pButton->GetListener());
	pButton->SetListener(this);
	pButton->SetButtonGroup(&m_ButtonGroup);
	m_TabList.push_back(pItem);

	// �׻� �Ѱ��� ���� �־�� �Ѵ�.
	//MTabList::iterator iTabList = m_TabList.begin();
	//MTabItem *pFistItem = *iTabList;
	//pFistItem->pButton->SetCheck(true);
	SetSelIndex(0);
}

void MTabCtrl::RemoveAll(void)
{
	while(m_TabList.size() > 0) 
	{
		MTabItem* pItem = *m_TabList.begin();
		//pItem->pButton->ReleaseButtonGroup();
		delete pItem;
		m_TabList.pop_front();
	}
}

int MTabCtrl::GetCount(void)
{
	return m_TabList.size();
}

int MTabCtrl::GetSelIndex(void)
{
	return m_nCurrentSel;
}

bool MTabCtrl::SetSelIndex(int nIndex)
{
	MTabItem *pSelItem = NULL;
	int nCurIndex = 0;
	for(MTabList::iterator i=m_TabList.begin();i!=m_TabList.end();i++)
	{
		MTabItem *pItem = *i;
		if(nIndex==nCurIndex) 
		{
			pSelItem = pItem;
			m_nCurrentSel = nCurIndex;
		}
		else 
		{
			pItem->pButton->SetCheck(false);
			if(pItem->pPanel)
				pItem->pPanel->Show(false);
		}
		nCurIndex++;
	}

	if(pSelItem)
	{
		pSelItem->pButton->SetCheck(true); 
		if(pSelItem->pPanel)
			pSelItem->pPanel->Show(true);
	}

	m_EventHandler.OnCommand(this, MEVENT_TABCTRL_TAB_SELECTED);
	return true;
}

bool MTabCtrl::OnCommand(MWidget* pWidget, MEventMsg nMsg, const char* szArgs)
{
	if(nMsg == MEVENT_BUTTON_CLICK) 
	{
		int nIndex = 0;
		for(MTabList::iterator i=m_TabList.begin();i!=m_TabList.end();i++)
		{
			MTabItem *pItem = *i;
			if(pItem->pButton == pWidget) 
			{
				SetSelIndex(nIndex);

				if(pItem->pButtonListener)
				{
					return pItem->pButtonListener->OnCommand(pWidget,nMsg);
				}

				break;
			}
			nIndex++;
		}
		// return �� true�̸� Event(Lua)�� ���� �� �� ���� �ȴ�.
		// �׷��� �켱 break;�� �׽�Ʈ �� ���� ����(��ȯ���� false�̾�� �Ѵ�.)
		return true;
	}

	return false;
}

void MTabCtrl::UpdateListeners()
{
	int nIndex = 0;
	for(MTabList::iterator i=m_TabList.begin();i!=m_TabList.end();i++)
	{
		MTabItem *pItem = *i;
		if(pItem->pButton->GetListener()!=this)	// �ٲ���� �������ش�
		{
			pItem->pButtonListener = pItem->pButton->GetListener();
			pItem->pButton->SetListener(this);
		}
	}
}

bool MTabCtrl::OnEvent(MEvent* pEvent, MListener* pListener)
{
	MWidget::OnEvent(pEvent, pListener);

	//////////////////////////////////////////////////////////////////////////
	MRECT WidgetRect(0, 0, m_Rect.w, m_Rect.h);

	switch(pEvent->nMessage)
	{
	case MWM_LBUTTONDOWN:
		if(WidgetRect.InPoint(pEvent->Pos)==true)
		{
			return true;
		}
		break;
	}

	return false;
}

int MTabCtrl::glueGetCount()
{
	return GetCount();
}

int MTabCtrl::glueGetSelIndex()
{
	return GetSelIndex();
}

bool MTabCtrl::glueSetSelIndex( int index )
{
	return SetSelIndex(index);
}

bool MTabCtrl::glueSetSelTabName(const char* szTabName)
{
	if( szTabName ==NULL) return false;

	MTabItem *pSelItem = NULL;
	int nCurIndex = 0;
	for(MTabList::iterator i=m_TabList.begin();i!=m_TabList.end();i++)
	{
		MTabItem *pItem = *i;
		if( !strcmp(pItem->pButton->GetName().c_str(), szTabName))
		{
			pSelItem = pItem;
			m_nCurrentSel = nCurIndex;
		}
		else 
		{
			pItem->pButton->SetCheck(false);
			if(pItem->pPanel)
				pItem->pPanel->Show(false);
		}
		nCurIndex++;
	}

	if(pSelItem)
	{
		//�ӽ�
		pSelItem->pButton->SetCheck(false);
		if(pSelItem->pPanel)
			pSelItem->pPanel->Show(true);
	}

	m_EventHandler.OnCommand(this, MEVENT_TABCTRL_TAB_SELECTED);
	return true;
}

} // namespace mint3