#include "stdafx.h"
#include "GPlayerMailReserver.h"
#include "GEntityPlayer.h"
#include "GItemHolder.h"


GPlayerMailReserver::GPlayerMailReserver(GEntityPlayer* pOwner)
: m_pOwner(pOwner)
{
	VALID(pOwner);		
}

GPlayerMailReserver::~GPlayerMailReserver()
{
	// do nothing
}

bool GPlayerMailReserver::PutUpItem(int nSlotID, int nAmount)
{
	// ���� Ȯ��
	if (!IsValidParameter(nSlotID, nAmount))
	{
		return false;
	}


	// ÷�� ������ ã��
	map_appended::iterator it = m_mapAppendedItem.find(nSlotID);
	
	if (it == m_mapAppendedItem.end())
	{
		// ÷�� ������ ���� ���� ���� Ȯ��
		if (MAX_MAIL_APPENDED_ITEM_COUNT <=m_mapAppendedItem.size())	
		{
			return false;
		}

		// ÷�� ������ �߰�
		m_mapAppendedItem.insert(make_pair(nSlotID, nAmount));
	}
	else
	{
		// ���� ����
		it->second += nAmount;
	}


	return true;
}

bool GPlayerMailReserver::PutDownItem(int nSlotID, int nAmount)
{
	// ���� Ȯ��
	if (!IsValidParameter(nSlotID, nAmount))
	{
		return false;
	}


	// ÷�� ������ ã��
	map_appended::iterator it = m_mapAppendedItem.find(nSlotID);

	if (it == m_mapAppendedItem.end())
	{
		return false;
	}


	// ���� ����
	if (nAmount < it->second)
	{
		it->second -= nAmount;
	}
	else
	{		
		m_mapAppendedItem.erase(it);
	}


	return true;
}

void GPlayerMailReserver::Clear(void)
{
	m_mapAppendedItem.clear();
}

bool GPlayerMailReserver::IsAppended(int nSlotID)
{
	map_appended::iterator it = m_mapAppendedItem.find(nSlotID);
	
	if (it == m_mapAppendedItem.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

int GPlayerMailReserver::GetAmount(int nSlotID)
{
	map_appended::iterator it = m_mapAppendedItem.find(nSlotID);

	if (it == m_mapAppendedItem.end())
	{
		return 0;
	}

	return it->second;
}

int GPlayerMailReserver::GetAppendedSlotCount(void)
{
	int nCount = m_mapAppendedItem.size();
	return nCount;
}

bool GPlayerMailReserver::IsValidParameter(int nSlotID, int nAmount)
{
	const GItemHolder* pItemHolder = m_pOwner->GetItemHolder();
	int nSlotSize = pItemHolder->GetSlotSize(SLOTTYPE_INVENTORY);

	if (nSlotID < 0 || nSlotSize <= nSlotID)
	{
		return false;
	}

	if (nAmount < 0)
	{
		return false;
	}

	return true;
}

int GPlayerMailReserver::GetDefaultItemID(void)
{
	if (m_mapAppendedItem.empty())
	{
		return 0;
	}

	map_appended::iterator it = m_mapAppendedItem.begin();
	
	GItemHolder* pItemHolder = m_pOwner->GetItemHolder();	
	GItem* pItem = pItemHolder->GetItem(SLOTTYPE_INVENTORY, it->first);	
	if (pItem == NULL)
	{
		return 0;
	}

	return pItem->m_pItemData->m_nID;
}


int GPlayerMailReserver::MakeDBT(GDBT_MAIL_APPENDED_ITEM (&itemSlot)[MAX_MAIL_APPENDED_ITEM_COUNT])
{
	int nAppendedCount = 0;
	map_appended::iterator it = m_mapAppendedItem.begin();
	
	for(int i = 0; i < MAX_MAIL_APPENDED_ITEM_COUNT; i++)
	{
		// ���̻� ÷�� �������� ������ ����
		if (it == m_mapAppendedItem.end())
		{
			return nAppendedCount;
		}

		
		// ÷�� ������ Ȯ��
		int nSlotID = it->first;

		GItemHolder* pItemHolder = m_pOwner->GetItemHolder();
		GItem* pItem = pItemHolder->GetItem(SLOTTYPE_INVENTORY, nSlotID);

		if (pItem == NULL)
		{
			return nAppendedCount;
		}


		// ÷�� ���� ����
		int nAmount = it->second;
		int nStackAmount = pItem->GetAmount() - nAmount;

		if (nStackAmount < 0)
		{
			return nAppendedCount;
		}


		// ���� ����
		itemSlot[i].m_nSlotID = nSlotID;
		itemSlot[i].m_nItemID = pItem->m_pItemData->m_nID;
		itemSlot[i].m_nItemUID = pItem->m_nIUID;
		itemSlot[i].m_nStackAmount = nStackAmount;
		itemSlot[i].m_nDeltaStackAmount = nAmount;
		itemSlot[i].m_nDuration = pItem->m_nDurability;
		itemSlot[i].m_nPeriod = pItem->m_UsagePeriod.IsPeriod() || pItem->m_ExpirationPeriod.IsPeriod();	// 0:������, 1:�Ⱓ��

		nAppendedCount++;


		// ���� ÷�� ������ �غ�		
		it++;
	}


	return nAppendedCount;
}
