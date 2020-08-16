#include "stdafx.h"
#include "GTestForward.h"
#include "GNPCShopSeller.h"
#include "GNPCShopInfo.h"
#include "GItemHolder.h"
#include "GFactionInfo.h"
#include "CSNPCShopCalculator.h"
#include "CSFactionCalculator.h"


SUITE(NPCShopSeller)
{
	struct Fixture
	{
		Fixture()
		{
			m_pField = GUTHelper_Field::DefaultMockField();
			m_pPlayer = test.player.NewPlayer(m_pField);

			m_pLink = test.network.NewLink(m_pPlayer);
			m_pNPCShopInfo = test.npcshop.NewNPCShopInfo();	

			m_pSellItem = test.item.GiveNewItem(m_pPlayer);
			m_pSellItem->m_pItemData->m_nBaseSellingPrice = 500;

			m_pLink->ResetCommands();
		}

		MockField*		m_pField;
		GEntityPlayer*	m_pPlayer;
		MockLink*		m_pLink;

		GNPCShopInfo*	m_pNPCShopInfo;
		GItem*			m_pSellItem;

		GNPCShopSeller	m_NPCShopSeller;
	};
	

	// �����ϰ� ���� ���� �������� �ȷ��� �ϴ� ���
	TEST_FIXTURE(Fixture, Sell_NotExistItem)
	{
		int nBeforeMoney = m_pPlayer->GetPlayerInfo()->GetMoney();			

		CHECK_EQUAL(false, m_NPCShopSeller.Sell(m_pPlayer, m_pNPCShopInfo->m_nID, INVALID_ID, 1));
		CHECK_EQUAL(nBeforeMoney, m_pPlayer->GetPlayerInfo()->GetMoney());
	}	

	// �������� ������ �߸��Ǿ�����
	TEST_FIXTURE(Fixture, Sell_CheckItemAmount)
	{
		int nAmount;

		nAmount = 0;
		CHECK_EQUAL(false, m_NPCShopSeller.Sell(m_pPlayer, m_pNPCShopInfo->m_nID, m_pSellItem->m_nSlotID, nAmount));

		nAmount = m_pSellItem->GetAmount() + 1;
		CHECK_EQUAL(false, m_NPCShopSeller.Sell(m_pPlayer, m_pNPCShopInfo->m_nID, m_pSellItem->m_nSlotID, nAmount));
	}

	// �Ǹ� �Ұ����� �������� �ȷ��� ���
	TEST_FIXTURE(Fixture, CheckSellabeItem)
	{
		m_pSellItem->m_pItemData->m_bSellable = false;
		CHECK_EQUAL(false, m_NPCShopSeller.Sell(m_pPlayer, m_pNPCShopInfo->m_nID, m_pSellItem->m_nSlotID, 1));

		GItem* pEquipItem = test.item.EquipNewItem(m_pPlayer);
		CHECK_EQUAL(false, m_NPCShopSeller.Sell(m_pPlayer, m_pNPCShopInfo->m_nID, pEquipItem->m_nSlotID, 1));
	}

	// �������� ���� �հ�� ���� �Ӵϸ� �������� MAX_MONEY���� ū ���
	TEST_FIXTURE(Fixture, CheckMoney)
	{	
		GItemHolder* pItemHolder = m_pPlayer->GetItemHolder();

		int nSellAmount = static_cast<int>((MAX_MONEY - m_pPlayer->GetPlayerInfo()->GetMoney()) / m_pNPCShopInfo->m_fBaseSellMod / m_pSellItem->m_pItemData->m_nBaseSellingPrice + 1);
		GItem* pInvenSellItem = pItemHolder->GetItem(SLOTTYPE_INVENTORY, m_pSellItem->m_nSlotID);

		m_pPlayer->GetItemHolder()->IncreaseAmount(SLOTTYPE_INVENTORY, m_pSellItem->m_nSlotID, nSellAmount - pInvenSellItem->GetAmount());

		CHECK_EQUAL(false, m_NPCShopSeller.Sell(m_pPlayer, m_pNPCShopInfo->m_nID, m_pSellItem->m_nSlotID, nSellAmount));
	}

	// �������� ��ȭ��
	TEST_FIXTURE(Fixture, Sell_Item)
	{
		GItemHolder* pItemHolder = m_pPlayer->GetItemHolder();

		int nBeforeAmount = pItemHolder->GetItemCount(SLOTTYPE_INVENTORY, m_pSellItem->m_pItemData->m_nID);
		int nSellItemID = m_pSellItem->m_pItemData->m_nID;
		int nSellAmount = 1;

		CHECK_EQUAL(true, m_NPCShopSeller.Sell(m_pPlayer, m_pNPCShopInfo->m_nID, m_pSellItem->m_nSlotID, nSellAmount));

		CHECK_EQUAL(nBeforeAmount - nSellAmount, pItemHolder->GetItemCount(SLOTTYPE_INVENTORY, nSellItemID));
	}

	// ��ȣ���� GOOD�� �� �������� ��ȭ��
	TEST_FIXTURE(Fixture, Sell_MoneyGoodFaction)
	{
		GFactionInfo* pFactionInfo = test.faction.SetGoodFaction(m_pPlayer);
		m_pNPCShopInfo->m_nFactionID = pFactionInfo->m_nID;

		int nBeforeMoney = m_pPlayer->GetPlayerInfo()->GetMoney();
		CHECK_EQUAL(true, m_NPCShopSeller.Sell(m_pPlayer, m_pNPCShopInfo->m_nID, m_pSellItem->m_nSlotID, 1));

		int nSellPrice = CSNPCShopCalculator::CalcSellPrice(m_pSellItem->m_pItemData->m_nBaseSellingPrice, m_pNPCShopInfo->m_fBaseSellMod, FRT_GOOD);
		CHECK_EQUAL(nBeforeMoney + nSellPrice, m_pPlayer->GetPlayerInfo()->GetMoney());
	}

	// ��ȣ���� EXCELLENT�� �� �������� ��ȭ��
	TEST_FIXTURE(Fixture, Sell_MoneyExcellentFaction)
	{
		GFactionInfo* pFactionInfo = test.faction.SetExcellentFaction(m_pPlayer);
		m_pNPCShopInfo->m_nFactionID = pFactionInfo->m_nID;

		int nBeforeMoney = m_pPlayer->GetPlayerInfo()->GetMoney();
		CHECK_EQUAL(true, m_NPCShopSeller.Sell(m_pPlayer, m_pNPCShopInfo->m_nID, m_pSellItem->m_nSlotID, 1));

		int nSellPrice = CSNPCShopCalculator::CalcSellPrice(m_pSellItem->m_pItemData->m_nBaseSellingPrice, m_pNPCShopInfo->m_fBaseSellMod, FRT_EXCELLENT);
		CHECK_EQUAL(nBeforeMoney + nSellPrice, m_pPlayer->GetPlayerInfo()->GetMoney());
	}

	TEST_FIXTURE(Fixture, SellVeryCommon)
	{
		GItemHolder* pItemHolder = m_pPlayer->GetItemHolder();

		GItem* pVeryCommonItem = test.item.GiveNewItem(m_pPlayer, INVALID_ID, 10);
		pVeryCommonItem->m_pItemData->m_nTier = ITEM_TIER_VERY_COMMON;

		GItem* pCommonItem = test.item.GiveNewItem(m_pPlayer, INVALID_ID, 10);
		pCommonItem->m_pItemData->m_nTier = ITEM_TIER_COMMON;

		int nVeryCommonItemID = pVeryCommonItem->m_pItemData->m_nID;

		int nBeforeVeryCommonItemAmount = pVeryCommonItem->GetAmount();
		int nBeforeCommonItemAmount = pCommonItem->GetAmount();		

		CHECK_EQUAL(true, m_NPCShopSeller.SellVeryCommon(m_pPlayer, m_pNPCShopInfo->m_nID));
		CHECK_EQUAL(false, pItemHolder->IsExist(SLOTTYPE_INVENTORY, nVeryCommonItemID));
		CHECK_EQUAL(nBeforeCommonItemAmount, pCommonItem->GetAmount());
	}
}