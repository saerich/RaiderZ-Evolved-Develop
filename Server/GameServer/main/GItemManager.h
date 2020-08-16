#ifndef _GITEM_MANAGER_H
#define _GITEM_MANAGER_H

#include "CSItemManager.h"
#include "GItem.h"
#include <map>
using namespace std;

struct TD_PLAYER_GAME_DATA_ITEM_INSTANCE;

/// ������ ������
class GItemManager : public CSItemManager, public MTestMemPool<GItemManager>
{
public:
		typedef map<int, GItemData*>	GItemDataMap;
protected:
		GItemDataMap			m_ItemDataMap;
		MUID					m_uidNowItemID;
		set<int>				m_setNotExistItemIDInXML;	// item.xml���� ���� DB���� �����ϴ� ��������ID

		virtual bool ParseItem(MXmlElement* pElement, CSItemData* pItemData, MXml* pXml) override;
		MUID					GetNewItemID();								///< �� MUID�� ����
public:
		GItemManager();
		virtual ~GItemManager();
		virtual void		Clear();

		GItemData*			GetItemData(int id);								// id�� ������ ����Ÿ�� ����.		
		GItem*				MakeNewItem(int nItemID, int nAmount=1, bool bBind=false);
		GItem*				MakeNewItem(GItem* pItem, int64 nIUID, int nAmount);
		GItem*				MakeNewItem(const TD_PLAYER_GAME_DATA_ITEM_INSTANCE& tdItem);

		virtual GItemData*	NewItemData();
		virtual void		InsertItemData(GItemData* pItemData);
		virtual void		Cooking();

		bool				IsExist(int nID);
		
		GItemDataMap::iterator Begin()		{ return m_ItemDataMap.begin(); }
		GItemDataMap::iterator End()		{ return m_ItemDataMap.end(); }

		void				InsertNotExistItemIDInXML(int nID);
		bool				IsNotExistItemIDInXML(int nID);
};


#endif // _GITEM_MANAGER_H