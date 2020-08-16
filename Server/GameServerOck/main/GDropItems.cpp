#include "stdafx.h"
#include "GDropItems.h"
#include "GEntityPlayer.h"
#include "GLootInfo.h"
#include "GGlobal.h"
#include "GConditionsSystem.h"
#include "GEntityNPC.h"
#include "GNPCBParts.h"
#include "GParty.h"
#include "GPartySystem.h"
#include "GPlayerQuests.h"
#include "GQuestInfoMgr.h"
#include "GSTLUtil.h"
#include "GDropItem.h"
#include "GItemManager.h"
#include "GLootInfoMgr.h"
#include "GNPCLoot.h"
#include "GDroplist.h"
#include "GLootSystem.h"
#include "GPlayerObjectManager.h"
#include "GDropItemAuthorizer.h"


GDropItems::GDropItems( GEntityNPC* pOwner )
: m_pOwner(pOwner)
, m_nNowDropItemUID(MUID::ZERO)
{
	m_pDropItemAuthorizer = new GDropItemAuthorizer;
}

GDropItems::~GDropItems()
{
	SAFE_DELETE(m_pDropItemAuthorizer);
	SAFE_DELETE_MAP(m_mapDropItem);
}

void GDropItems::Clear()	
{
	SAFE_DELETE(m_pDropItemAuthorizer);
	SAFE_DELETE_MAP(m_mapDropItem);	
	m_lootingRuleData = LOOTING_RULE_DATA();
	m_nNowDropItemUID.SetZero();
}

const map<MUID, GDropItem*>& GDropItems::GetContainer()
{
	return m_mapDropItem;
}

vector<TD_ITEM_DROP> GDropItems::MakeVisbleTDDropItem(int nCID)
{
	vector<TD_ITEM_DROP> vecTDDropItem;

	for each(const map<MUID, GDropItem*>::value_type& val in m_mapDropItem)
	{
		GDropItem* pDropItem = val.second;

		if (true == pDropItem->IsViewableCID(nCID))
		{
			vecTDDropItem.push_back(pDropItem->MakeTDDropItem());
		}
	}

	return vecTDDropItem;
}

void GDropItems::Remove(const MUID& nDropItemUID)
{
	GDropItem* pDropItem = GetDropItem(nDropItemUID);
	if (NULL == pDropItem) return;

	m_mapDropItem.erase(nDropItemUID);
	SAFE_DELETE(pDropItem);	
}

bool GDropItems::IsEmpty()
{
	return m_mapDropItem.empty();
}

bool GDropItems::IsExist( const MUID& nDropItemUID )
{
	if (NULL ==  GetDropItem(nDropItemUID)) return false;

	return true;
}

bool GDropItems::IsAuthorizedCID(int nCID)
{
	for each(const map<MUID, GDropItem*>::value_type& val in m_mapDropItem)
	{
		GDropItem* pDropItem = val.second;
		if (true == pDropItem->IsAuthorizedCID(nCID)) return true;
	}

	return false;
}

bool GDropItems::IsViewableCID( int nCID )
{
	for each(const map<MUID, GDropItem*>::value_type& val in m_mapDropItem)
	{
		GDropItem* pDropItem = val.second;
		if (true == pDropItem->IsViewableCID(nCID)) return true;
	}

	return false;
}

void GDropItems::GetViewableCID(set<int>& outsetViewableCID)
{
	for each(const map<MUID, GDropItem*>::value_type& val in m_mapDropItem)
	{
		GDropItem* pDropItem = val.second;
		if (NULL == pDropItem) continue;

		pDropItem->GetViewableCID(outsetViewableCID);
	}
}

GDropItem* GDropItems::GetDropItem(const MUID& nDropItemUID)
{
	map<MUID, GDropItem*>::iterator itor = m_mapDropItem.find(nDropItemUID);
	if (m_mapDropItem.end() == itor) return NULL;

	GDropItem* pDropItem = (*itor).second;

	return pDropItem;
}

vector<GDropItem*> GDropItems::CollectDropItemByCID( int nCID )
{
	vector<GDropItem*> vecDropItem;

	for each(const map<MUID, GDropItem*>::value_type& val in m_mapDropItem)
	{
		if (false == val.second->IsAuthorizedCID(nCID)) continue;

		vecDropItem.push_back(val.second);
	}

	return vecDropItem;
}

vector<MUID> GDropItems::CollectDropItemUIDByCID( int nCID )
{
	GVectorMUID vecDropItemUID;

	for each(const map<MUID, GDropItem*>::value_type& val in m_mapDropItem)
	{
		if (false == val.second->IsAuthorizedCID(nCID)) continue;

		vecDropItemUID.Vector().push_back(val.second->GetUID());
	}

	return vecDropItemUID.Vector();
}


int GDropItems::GetSumOfAmount()
{
	int nSumOfAmount = 0;

	for each(const map<MUID, GDropItem*>::value_type& val in m_mapDropItem)
	{
		GDropItem* pDropItem = val.second;
		nSumOfAmount += pDropItem->GetAmount();		
	}

	return nSumOfAmount;
}

void GDropItems::DropByKill(GLootInfo* pLootInfo, int nBeneficiaryCID, MUID nPartyUID, const LOOTING_RULE_DATA& lootingRuleData)
{
	vector<int> vecBeneficiaryCID;
	vecBeneficiaryCID.push_back(nBeneficiaryCID);

	DropByKill(pLootInfo, vecBeneficiaryCID, nPartyUID, lootingRuleData);
}

void GDropItems::DropByKill(GLootInfo* pLootInfo, const vector<int>& vecBeneficiaryCID, MUID nPartyUID, const LOOTING_RULE_DATA& lootingRuleData)
{
	if (true == vecBeneficiaryCID.empty()) return;

	m_lootingRuleData = lootingRuleData;

	int nSumDropQuantity = 0;
	for each(LOOT_ITEM_INFO* pLootItemInfo in pLootInfo->m_vecLootItems)
	{
		if (false == CheckBreakParts(pLootItemInfo)) continue;

		if (false == pLootItemInfo->m_vecQuestID.empty())
		{
			DropQuestItem(vecBeneficiaryCID, pLootItemInfo);
		}
		else if (0 != pLootItemInfo->m_nQuestPVPFieldID && m_pOwner->GetFieldID() == pLootItemInfo->m_nQuestPVPFieldID)
		{
			DropQuestPVPItem(vecBeneficiaryCID, pLootItemInfo);
		}
		else
		{
			int nLimitDropQuantity = CalculLimitDropQuantity(pLootItemInfo, pLootInfo->m_nLimitDropQuantity);
			nSumDropQuantity += DropNormalItem(vecBeneficiaryCID, nPartyUID, pLootItemInfo, nLimitDropQuantity, nSumDropQuantity);
		}
	}
}

void GDropItems::DropByInteract(int nBeneficiaryCID, GLootInfo* pLootInfo)
{
	int nSumDropQuantity = 0;
	for each(LOOT_ITEM_INFO* pLootItemInfo in pLootInfo->m_vecLootItems)
	{
		if (false == CheckBreakParts(pLootItemInfo)) continue;

		int nLimitDropQuantity = CalculLimitDropQuantity(pLootItemInfo, pLootInfo->m_nLimitDropQuantity);
		nSumDropQuantity += DropInteractItem(nBeneficiaryCID, pLootItemInfo, nLimitDropQuantity, nSumDropQuantity);
	}
}

bool GDropItems::CheckBreakParts(LOOT_ITEM_INFO* pLootItemInfo)
{
	if (NULL == m_pOwner) return false;
	if (m_pOwner->GetNPCBParts() == NULL) return true;

	if (0 < pLootItemInfo->m_nBreakPart && false == m_pOwner->GetNPCBParts()->IsBroken(pLootItemInfo->m_nBreakPart))
	{
		return false;
	}

	return true;
}

int GDropItems::CalculLimitDropQuantity( LOOT_ITEM_INFO* pLootItemInfo, int nLimitDropQuantity )
{
	if (0 < pLootItemInfo->m_nBreakPart)
	{
		// bpart�� �ִ� ��� ���� ������ ���� ���� �ʴ´�.
		return INT_MAX;			
	}

	return nLimitDropQuantity;
}

vector<GDropItem*> GDropItems::DropItem(LOOT_ITEM_INFO* pLootItemInfo, int nLimitQuantity)
{
	if (NULL == pLootItemInfo) return vector<GDropItem*>();

	vector<GDropItem*> vecNewDropItem;
	MakeDropItem(pLootItemInfo, vecNewDropItem, nLimitQuantity);
	if (true == vecNewDropItem.empty()) return vector<GDropItem*>();
	StoreDropItem(vecNewDropItem);

	return vecNewDropItem;
}

// �ѹ��� �ֻ����� ���� ����ϰ�, ���÷꿡 ���� ���� �ο�
int GDropItems::DropNormalItem(const vector<int>& vecBeneficiaryCID, MUID nPartyUID, LOOT_ITEM_INFO* pLootItemInfo, int nLimitDropQuantity, int nSumDropQuantity)
{
	if (nLimitDropQuantity <= nSumDropQuantity) return 0;

	vector<GDropItem*> vecNewDropItem = DropItem(pLootItemInfo, nLimitDropQuantity - nSumDropQuantity);
	m_pDropItemAuthorizer->AuthorizeNormalItem(vecNewDropItem, vecBeneficiaryCID, nPartyUID, m_lootingRuleData);

	return vecNewDropItem.size();
}

// ����Ʈ�� ���� �����ŭ �ֻ����� ���� ����ϰ� �ش� ������� ���� �ο�
void GDropItems::DropQuestItem(const vector<int>& vecBeneficiaryCID, LOOT_ITEM_INFO* pLootItemInfo)
{
	if (true == vecBeneficiaryCID.empty()) return;
	if (NULL == pLootItemInfo) return;

	for each (int nBeneficiaryCID in vecBeneficiaryCID)
	{
		GEntityPlayer* pBeneficiary = gmgr.pPlayerObjectManager->GetEntity(nBeneficiaryCID);
		if (NULL == pBeneficiary) continue;

		if (false == pBeneficiary->GetQuests().IsDoingQuest(pLootItemInfo->m_vecQuestID)) continue;

		int nLimitItemQuantity = INT_MAX;
		if (true == gmgr.pQuestInfoMgr->IsCollectItem(pLootItemInfo->m_vecQuestID, pLootItemInfo->m_nID))
		{
			nLimitItemQuantity = pBeneficiary->GetQuests().GetQuestNeedItemQuantity(pLootItemInfo->m_vecQuestID, pLootItemInfo->m_nID);
		}

		vector<GDropItem*> vecNewDropItem = DropItem(pLootItemInfo, nLimitItemQuantity);
		m_pDropItemAuthorizer->AuthorizeQuestItem(vecNewDropItem, pBeneficiary->GetCID());
	}
}

// �����ŭ �ֻ����� ���� ����ϰ�, �ش� ������� ���� �ο�
void GDropItems::DropQuestPVPItem(const vector<int>& vecBeneficiaryCID, LOOT_ITEM_INFO* pLootItemInfo)
{
	if (true == vecBeneficiaryCID.empty()) return;
	if (NULL == pLootItemInfo) return;

	for each (int nBeneficiaryCID in vecBeneficiaryCID)
	{
		GEntityPlayer* pBeneficiary = gmgr.pPlayerObjectManager->GetEntity(nBeneficiaryCID);
		if (NULL == pBeneficiary) continue;

		vector<GDropItem*> vecNewDropItem = DropItem(pLootItemInfo);
		m_pDropItemAuthorizer->AuthorizeQuestPVPItem(vecNewDropItem, pBeneficiary->GetCID());
	}
}

// �ѹ��� �ֻ����� ���� ����ϰ�, ���ͷ����� ������� ���� �ο�
int GDropItems::DropInteractItem(int nBeneficiaryCID, LOOT_ITEM_INFO* pLootItemInfo, int nLimitDropQuantity, int nSumDropQuantity)
{
	if (nLimitDropQuantity <= nSumDropQuantity) return 0;

	vector<GDropItem*> vecNewDropItem = DropItem(pLootItemInfo, nLimitDropQuantity - nSumDropQuantity);
	m_pDropItemAuthorizer->AuthorizeInteractItem(vecNewDropItem, nBeneficiaryCID);

	return vecNewDropItem.size();
}

void GDropItems::MakeDropItem(LOOT_ITEM_INFO* pLootItemInfo, vector<GDropItem*>& outVecDropItem, int nLimitQuantity)
{
	GItemData* pItemData = gmgr.pItemManager->GetItemData(pLootItemInfo->m_nID);
	if (NULL == pItemData) return;

	const float fRandRate = RandomNumber(0.0f, 100.0f);
	if (pLootItemInfo->m_fRate < fRandRate) return;
	int nTotalQuantity = min(RandomNumber(pLootItemInfo->m_nMin, pLootItemInfo->m_nMax), nLimitQuantity);
	if (0 >= nTotalQuantity) return;

	while (0 < nTotalQuantity)
	{
		int nMakeQuantity = 0;
		if (pItemData->m_nStackAmount < nTotalQuantity)
		{
			nMakeQuantity = pItemData->m_nStackAmount;
		}
		else
		{
			nMakeQuantity = nTotalQuantity;
		}

		GDropItem* pNewDropItem = new GDropItem(MakeNewDropItemUID(), pLootItemInfo->m_nID, nMakeQuantity);
		outVecDropItem.push_back(pNewDropItem);

		nTotalQuantity -= pNewDropItem->GetAmount();
	}	
}

void GDropItems::StoreDropItem(const vector<GDropItem*>& vecDropItem)
{
	for each (GDropItem* pDropItem in vecDropItem)
	{
		m_mapDropItem.insert(map<MUID, GDropItem*>::value_type(pDropItem->GetUID(), pDropItem));
	}	
}

MUID GDropItems::MakeNewDropItemUID()
{
	m_nNowDropItemUID.Increase();
	return m_nNowDropItemUID;
}