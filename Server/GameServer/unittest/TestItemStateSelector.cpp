#include "stdafx.h"
#include "GTestForward.h"
#include "FBaseGame.h"
#include "GItemManager.h"
#include "GItemStateSelector.h"
#include "GPlayerDoing.h"
#include "GTradeSystem.h"
#include "GTradeRequester.h"
#include "GTradeAcceptor.h"
#include "GTradeItemPutUper.h"

SUITE(ItemStateSelector)
{
	struct Fixture : public FBaseGame
	{
		DECLWRAPPER_ItemMgr;
		GItemStateSelector	m_ItemStateSelector;
	};


	// �κ��丮 ������
	TEST_FIXTURE(Fixture, Select_InventoryItem)
	{
		GItem* pItem = test.item.GiveNewItem(m_pPlayer);

		CHECK_EQUAL(IST_INVENTORY, m_ItemStateSelector.Select(m_pPlayer, pItem));
	}

	// â�� ������
	TEST_FIXTURE(Fixture, Select_StorageItem)
	{
		// TODO : â������ üũ
	}

	// ������ ������
	TEST_FIXTURE(Fixture, Select_PostItem)
	{
		// TODO : ������ ����� üũ
	}

	// ������ ������
	TEST_FIXTURE(Fixture, Select_EquipedItem)
	{
		GItem* pItem = test.item.EquipNewItem(m_pPlayer);

		CHECK_EQUAL(IST_EQUIPED, m_ItemStateSelector.Select(m_pPlayer, pItem));
	}

	// ��ġ�� ���� ������
	TEST_FIXTURE(Fixture, Select_ArrangedItem)
	{
		GItem* pItem = test.item.EquipNewItem(m_pPlayer, INVALID_ID, ITEMSLOT_INN_BED);
		pItem->m_pItemData->m_ItemType = ITEMTYPE_HOUSING;

		CHECK_EQUAL(IST_ARRANGED, m_ItemStateSelector.Select(m_pPlayer, pItem));
	}

	// �ŷ����� ������
	TEST_FIXTURE(Fixture, Select_TradeItem)
	{
		GItem* pItem = test.item.GiveNewItem(m_pPlayer);
		GEntityPlayer* pOther = test.player.NewPlayer(m_pField);
		gsys.pTradeSystem->GetRequester().Request(m_pPlayer, pOther);
		gsys.pTradeSystem->GetAcceptor().Accept(pOther, m_pPlayer, true);
		gsys.pTradeSystem->GetItemPutUper().PutUp(m_pPlayer, pOther, pItem->m_nSlotID, pItem->GetAmount());

		CHECK_EQUAL(IST_TRADING, m_ItemStateSelector.Select(m_pPlayer, pItem));
	}
}