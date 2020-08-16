#include "StdAfx.h"
#include "TTabItem.h"
#include "TItemData.h"

TTabItem::TTabItem(void)
{
	Init();
}

TTabItem::~TTabItem(void)
{
}

void TTabItem::Init()
{
	m_nSelSlot = -1;
}

//�������� ���� �ε������ ����Ʈ�� �����ߵǳ�...
// TODO: �𵨿��� ������ ���� ������ �´� �����۸���Ʈ�� ����ϰ�
// �ε�� ���ص� ��� ���� �ʳ�???
//�������� �з��� �� �ְ� �����ؾ��ҵ�..
void TTabItem::InitItemMap()
{
	TItemManager* pItemMgr = g_pMainApp->GetItemMgr();

	if ( !pItemMgr->LoadFromFile(FILENAME_XITEM_TOOL) )
		assert(!"pItemMgr->LoadFromFile() - ������ ���̺� �ҷ����� ����");

	if ( !pItemMgr->LoadFromFile_SubMtrl(FILENAME_ITEM_CHANGEMTRL_TOOL) )
		mlog("���� : pItemMgr->LoadFromFile() - ������ ���� ���̺� �ҷ����� ����");

	TItemManager::CSItemDataMap& dataMap = pItemMgr->GetDataMap();

	//none �� ������ �߰�.
	for(int i=0;i<ITEMSLOT_MAX;i++)
		m_ItemMap[i].m_Index.push_back(NULL);

	for(TItemManager::CSItemDataMap::iterator itr = dataMap.begin();itr!=dataMap.end();itr++)
	{
		TItemData* data = (TItemData*)itr->second;
		int slot = (int)data->m_nItemSlot;
		m_ItemMap[slot].m_Index.push_back(data);

		// ���ʿ��� ������ �Ǵ°�?
		if(data->m_bLeftWeaponEquipable)
		{
			m_ItemMap[ITEMSLOT_LWEAPON].m_Index.push_back(data);
		}
	}

	pItemMgr->Cooking();
}

SlotList* TTabItem::SelectItem( int nSlot )
{
	ITEMMAP::iterator it = m_ItemMap.find(nSlot);
	if (it != m_ItemMap.end())
	{
		m_nSelSlot = nSlot;
		m_SelSlotList = (*it).second;

		return &m_SelSlotList;
	}

	return NULL;
}

CSItemData* TTabItem::GetItemData( int index )
{
	return m_SelSlotList.m_Index[index];
}

CSItemData* TTabItem::GetItemDataByID(int nID)
{
	for( int i = 1; i < (int)m_SelSlotList.m_Index.size(); ++i )
	{
		if( m_SelSlotList.m_Index[i]->m_nID == nID )
		{
			return m_SelSlotList.m_Index[i];
		}
	}

	return NULL;
}

//void TTabItem::SelectItemEquip( int nSelSlot, int nID )
//{
//	SelectItem(nSelSlot);
//
//	CSItemData* pItemData = GetItemDataByID(nID);
//
//	g_pMainApp->GetActor()->EquipItem(*pItemData);
//}