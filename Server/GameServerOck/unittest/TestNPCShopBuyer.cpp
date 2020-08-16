#include "stdafx.h"
#include "GTestForward.h"
#include "GNPCShopBuyer.h"
#include "GNPCShopInfo.h"
#include "CSNPCShopCalculator.h"
#include "CSFactionCalculator.h"
#include "GItemHolder.h"
#include "GFactionInfo.h"


SUITE(NPCShopBuyer)
{
	struct Fixture
	{
		Fixture()
		{
			m_pField = GUTHelper_Field::DefaultMockField();
			m_pPlayer = test.player.NewPlayer(m_pField);
			m_pPlayer->GetPlayerInfo()->SetMoney(MAX_MONEY);

			m_pLink = test.network.NewLink(m_pPlayer);
			m_pNPCShopInfo = test.npcshop.NewNPCShopInfo();	
			m_pItemData = test.item.NewItemData();
			m_pItemData->m_nBaseBuyingPrice = 500;

			m_pLink->ResetCommands();
		}

		MockField*		m_pField;
		GEntityPlayer*	m_pPlayer;
		MockLink*		m_pLink;

		GNPCShopInfo*	m_pNPCShopInfo;
		GItemData*		m_pItemData;

		GNPCShopBuyer	m_NPCShopBuyer;
	};

	// �������� ������ �߸��Ǿ�����
	TEST_FIXTURE(Fixture, CheckItemAmount)
	{
		int nAmount;

		nAmount = 0;
		CHECK_EQUAL(false, m_NPCShopBuyer.Buy(m_pPlayer, m_pNPCShopInfo->m_nID, m_pItemData->m_nID, nAmount));

		nAmount = m_pItemData->m_nStackAmount + 1;
		CHECK_EQUAL(false, m_NPCShopBuyer.Buy(m_pPlayer, m_pNPCShopInfo->m_nID, m_pItemData->m_nID, nAmount));
	}

	// ����� üũ�� ��� ��������
	TEST_FIXTURE(Fixture, CheckShopCondition)
	{		
		GItemData* pUnbuyableItemData = test.npcshop.AddUnbuyableItem(m_pNPCShopInfo);		

		CHECK_EQUAL(false, m_NPCShopBuyer.Buy(m_pPlayer, m_pNPCShopInfo->m_nID, pUnbuyableItemData->m_nID, 1));
	}

	// �����ϰ� �ִ� �ǹ����� ��� �������� ����� ���
	TEST_FIXTURE(Fixture, CheckMoney)
	{
		m_pItemData->m_nBaseBuyingPrice = 100;
		m_pPlayer->GetPlayerInfo()->SetMoney(99);
		CHECK_EQUAL(false, m_NPCShopBuyer.Buy(m_pPlayer, m_pNPCShopInfo->m_nID, m_pItemData->m_nID, 1));
	}

	// �Ѱ��� ���� ������ ������
	TEST_FIXTURE(Fixture, CheckUniqueItem)
	{
		m_pItemData->m_bUnique = true;
		test.item.GiveNewItem(m_pPlayer, m_pItemData->m_nID);
		
		CHECK_EQUAL(false, m_NPCShopBuyer.Buy(m_pPlayer, m_pNPCShopInfo->m_nID, m_pItemData->m_nID, 1));
	}

	// �������� ��ȭ��
	TEST_FIXTURE(Fixture, Buy_Item)
	{
		GItemHolder* pItemHolder = m_pPlayer->GetItemHolder();

		int nBeforeBuyItemQuantity = pItemHolder->GetItemCount(SLOTTYPE_INVENTORY, m_pItemData->m_nID);
		CHECK_EQUAL(true, m_NPCShopBuyer.Buy(m_pPlayer, m_pNPCShopInfo->m_nID, m_pItemData->m_nID, 1));

		CHECK_EQUAL(nBeforeBuyItemQuantity + 1, pItemHolder->GetItemCount(SLOTTYPE_INVENTORY, m_pItemData->m_nID));
	}

	// ��ȣ���� GOOD�� �� ���� ��ȭ��
	TEST_FIXTURE(Fixture, Buy_Money_GoodFaction)
	{
		GFactionInfo* pFactionInfo = test.faction.SetGoodFaction(m_pPlayer);
		m_pNPCShopInfo->m_nFactionID = pFactionInfo->m_nID;	

		int nBeforeMoney = m_pPlayer->GetPlayerInfo()->GetMoney();
		CHECK_EQUAL(true, m_NPCShopBuyer.Buy(m_pPlayer, m_pNPCShopInfo->m_nID, m_pItemData->m_nID, 1));

		int nBuyPrice = CSNPCShopCalculator::CalcBuyPrice(m_pItemData->m_nBaseBuyingPrice, m_pNPCShopInfo->m_fBaseBuyMod, FRT_GOOD);
		CHECK_EQUAL(nBeforeMoney - nBuyPrice, m_pPlayer->GetPlayerInfo()->GetMoney());
	}

	// ��ȣ���� Excellent�� �� ���� ��ȭ��
	TEST_FIXTURE(Fixture, Buy_Money_ExcellentFaction)
	{
		GFactionInfo* pFactionInfo = test.faction.SetExcellentFaction(m_pPlayer);
		m_pNPCShopInfo->m_nFactionID = pFactionInfo->m_nID;	

		int nBeforeMoney = m_pPlayer->GetPlayerInfo()->GetMoney();
		CHECK_EQUAL(true, m_NPCShopBuyer.Buy(m_pPlayer, m_pNPCShopInfo->m_nID, m_pItemData->m_nID, 1));

		int nBuyPrice = CSNPCShopCalculator::CalcBuyPrice(m_pItemData->m_nBaseBuyingPrice, m_pNPCShopInfo->m_fBaseBuyMod, FRT_EXCELLENT);
		CHECK_EQUAL(nBeforeMoney - nBuyPrice, m_pPlayer->GetPlayerInfo()->GetMoney());
	}
}