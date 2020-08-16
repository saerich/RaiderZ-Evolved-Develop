#include "stdafx.h"
#include "GTestForward.h"
#include "FBaseGame.h"
#include "GItemManager.h"
#include "GItemMover.h"
#include "GItemHolder.h"


SUITE(ItemMover)
{
	struct Fixture : public FBaseGame
	{
		Fixture()
		{
			m_pItemData = test.item.NewItemData();
			m_pItemData->m_nStackAmount = 10;
		}

		DECLWRAPPER_ItemMgr;

		GItemMover		m_ItemMover;
		GItemData*		m_pItemData;
	};


	namespace Check
	{
		// �ͼӵ� �������� ��� ���������� �̵��� �� ����
		TEST_FIXTURE(Fixture, CanNotMoveBindItemToGuildStorage)
		{
			GItem* pFromItem = test.item.GiveNewItem(m_pPlayer);
			pFromItem->m_bBind = true;

			CHECK_EQUAL(false, gsys.pItemSystem->GetMover().Move(m_pPlayer, pFromItem->m_nSlotType, pFromItem->m_nSlotID, pFromItem->GetID(), SLOTTYPE_GUILDSTORAGE, 0, 0, pFromItem->GetAmount()));
		}

		// �Ⱓ�� �������� ��� ���������� �̵��� �� ����
		TEST_FIXTURE(Fixture, CanNotMovePeriodItemToGuildStorage)
		{
			GItem* pFromItem = test.item.GiveNewItem(m_pPlayer);
			pFromItem->m_UsagePeriod.Set(true, true, 10, 0);

			CHECK_EQUAL(false, gsys.pItemSystem->GetMover().Move(m_pPlayer, pFromItem->m_nSlotType, pFromItem->m_nSlotID, pFromItem->GetID(), SLOTTYPE_GUILDSTORAGE, 0, 0, pFromItem->GetAmount()));
		}

		// �Ⱓ�� �������� ���������� �̵��� �� ����
		TEST_FIXTURE(Fixture, CanNotMovePeriodItemToStorage)
		{
			GItem* pFromItem = test.item.GiveNewItem(m_pPlayer);
			pFromItem->m_UsagePeriod.Set(true, true, 10, 0);

			CHECK_EQUAL(false, gsys.pItemSystem->GetMover().Move(m_pPlayer, pFromItem->m_nSlotType, pFromItem->m_nSlotID, pFromItem->GetID(), SLOTTYPE_STORAGE, 0, 0, pFromItem->GetAmount()));
		}

		// �Ⱓ�� �������� �ɰ� �� ����
		TEST_FIXTURE(Fixture, CanNotSplitPeriodItem)
		{
			GItem* pFromItem = test.item.GiveNewItem(m_pPlayer, INVALID_ID, 5);
			pFromItem->m_UsagePeriod.Set(true, true, 10, 0);

			CHECK_EQUAL(false, gsys.pItemSystem->GetMover().Move(m_pPlayer, pFromItem->m_nSlotType, pFromItem->m_nSlotID, pFromItem->GetID(), pFromItem->m_nSlotType, 1, 0, 1));
		}

		// �Ⱓ�� �������� ��ĥ �� ����
		TEST_FIXTURE(Fixture, CanNotMergePeriodItem)
		{
			{
				GItem* pFromItem = test.item.GiveNewItem(m_pPlayer, m_pItemData->m_nID, 5);				
				GItem* pToItem = test.item.GiveNewItem(m_pPlayer, m_pItemData->m_nID, 5);
				pFromItem->m_UsagePeriod.Set(true, true, 10, 0);

				CHECK_EQUAL(false, m_ItemMover.Move(m_pPlayer, pFromItem->m_nSlotType, pFromItem->m_nSlotID, pFromItem->GetID(), pToItem->m_nSlotType, pToItem->m_nSlotID, pToItem->GetID(), 5));
			}
			{
				GItem* pFromItem = test.item.GiveNewItem(m_pPlayer, m_pItemData->m_nID, 5);				
				GItem* pToItem = test.item.GiveNewItem(m_pPlayer, m_pItemData->m_nID, 5);
				pToItem->m_UsagePeriod.Set(true, true, 10, 0);

				CHECK_EQUAL(false, m_ItemMover.Move(m_pPlayer, pFromItem->m_nSlotType, pFromItem->m_nSlotID, pFromItem->GetID(), pToItem->m_nSlotType, pToItem->m_nSlotID, pToItem->GetID(), 5));
			}			
		}

		// ���̽��Կ� �������� �ű���� ���
		TEST_FIXTURE(Fixture, CanNotMoveToDummySlot)
		{
			GItem* pFromItem = test.item.GiveNewItem(m_pPlayer);
			int nToDummySlotID = 5;
			m_pPlayer->GetItemHolder()->ReserveDummySlot(pFromItem->m_nSlotType, nToDummySlotID);

			CHECK_EQUAL(false, m_ItemMover.Move(m_pPlayer, pFromItem->m_nSlotType, pFromItem->m_nSlotID, pFromItem->GetID(), pFromItem->m_nSlotType, nToDummySlotID, 0, 1));
		}	

		// ��ġ���� ������ �ִ� ���ð����� �Ѵ� ���
		TEST_FIXTURE(Fixture, CanNotMergeOverMaxStackAmout)
		{
			GItem* pFromItem = test.item.GiveNewItem(m_pPlayer, m_pItemData->m_nID, 5);
			GItem* pToItem = test.item.GiveNewItem(m_pPlayer, m_pItemData->m_nID, m_pItemData->m_nStackAmount - 1);

			CHECK_EQUAL(false, m_ItemMover.Move(m_pPlayer, pFromItem->m_nSlotType, pFromItem->m_nSlotID, pFromItem->GetID(), pToItem->m_nSlotType, pToItem->m_nSlotID, pToItem->GetID(), 5));
		}
	}


	// ��ü�� �ű�� ���
	TEST_FIXTURE(Fixture, MoveAll)
	{
		GItem* pFromItem = test.item.GiveNewItem(m_pPlayer);
		SH_ITEM_SLOT_TYPE nFromSlotType = pFromItem->m_nSlotType;
		int nFromSlotID = pFromItem->m_nSlotID;
		SH_ITEM_SLOT_TYPE nToSlotType = pFromItem->m_nSlotType;
		int nToSlotID = 2;

		CHECK_EQUAL(true, gsys.pItemSystem->GetMover().Move(m_pPlayer, pFromItem->m_nSlotType, pFromItem->m_nSlotID, pFromItem->GetID(), pFromItem->m_nSlotType, nToSlotID, 0, pFromItem->GetAmount()));

		CHECK(pFromItem == m_pPlayer->GetItemHolder()->GetItem(nToSlotType, nToSlotID));
		CHECK_EQUAL(pFromItem->m_nSlotID, nToSlotID);

		CHECK(NULL == m_pPlayer->GetItemHolder()->GetItem(nFromSlotType, nFromSlotID));
	}

	// �ɰ��� ���
	TEST_FIXTURE(Fixture, Split)
	{
		GItem* pFromItem = test.item.GiveNewItem(m_pPlayer, INVALID_ID, 5);
		SH_ITEM_SLOT_TYPE nToSlotType = SLOTTYPE_INVENTORY;
		int nToSlotID = 5;
		int nSplitAmount = 2;
		int nBeforeFromAmount = pFromItem->GetAmount();

		CHECK_EQUAL(true, gsys.pItemSystem->GetMover().Move(m_pPlayer, pFromItem->m_nSlotType, pFromItem->m_nSlotID, pFromItem->GetID(), pFromItem->m_nSlotType, nToSlotID, 0, nSplitAmount));
		CHECK(pFromItem->GetAmount() == nBeforeFromAmount - nSplitAmount);
		GItem* pSplitItem = m_pPlayer->GetItemHolder()->GetItem(nToSlotType, nToSlotID);
		ASSERT_CHECK(NULL != pSplitItem);
		CHECK_EQUAL(pSplitItem->GetID(), pFromItem->GetID());
		CHECK_EQUAL(pSplitItem->GetAmount(), nSplitAmount);
	}
	
	// ��ü�� ���ļ� FromItem�� ���ŵǴ� ���
	TEST_FIXTURE(Fixture, MergeAll)
	{
		GItem* pFromItem = test.item.GiveNewItem(m_pPlayer, m_pItemData->m_nID, 5);
		GItem* pToItem = test.item.GiveNewItem(m_pPlayer, m_pItemData->m_nID, 5);

		SH_ITEM_SLOT_TYPE nFromSlotType = pFromItem->m_nSlotType;
		int nFromSlotID = pFromItem->m_nSlotID;

		CHECK_EQUAL(true, m_ItemMover.Move(m_pPlayer, pFromItem->m_nSlotType, pFromItem->m_nSlotID, pFromItem->GetID(), pToItem->m_nSlotType, pToItem->m_nSlotID, pToItem->GetID(), 5));
		CHECK(NULL == m_pPlayer->GetItemHolder()->GetItem(nFromSlotType, nFromSlotID));
		CHECK_EQUAL(10, pToItem->GetAmount());
	}

	// �Ϻκи� ���ļ� FromItem�� ���� ���
	TEST_FIXTURE(Fixture, MergePart)
	{
		GItem* pFromItem = test.item.GiveNewItem(m_pPlayer, m_pItemData->m_nID, 10);
		GItem* pToItem = test.item.GiveNewItem(m_pPlayer, m_pItemData->m_nID, 5);

		CHECK_EQUAL(true, m_ItemMover.Move(m_pPlayer, pFromItem->m_nSlotType, pFromItem->m_nSlotID, pFromItem->GetID(), pToItem->m_nSlotType, pToItem->m_nSlotID, pToItem->GetID(), 5));
		CHECK_EQUAL(5, pFromItem->GetAmount());
		CHECK_EQUAL(10, pToItem->GetAmount());
	}	

	// ������ ��ġ ��ȯ
	TEST_FIXTURE(Fixture, Swap)
	{
		GItem* pFromItem = test.item.GiveNewItem(m_pPlayer);
		GItem* pToItem = test.item.GiveNewItem(m_pPlayer);

		SH_ITEM_SLOT_TYPE nFromSlotType = pFromItem->m_nSlotType;
		int nFromSlotID = pFromItem->m_nSlotID;
		SH_ITEM_SLOT_TYPE nToSlotType = pToItem->m_nSlotType;
		int nToSlotID = pToItem->m_nSlotID;

		CHECK_EQUAL(true, gsys.pItemSystem->GetMover().Move(m_pPlayer, nFromSlotType, nFromSlotID, pFromItem->GetID(), nToSlotType, nToSlotID, pToItem->GetID(), pFromItem->GetAmount()));

		CHECK(pFromItem == m_pPlayer->GetItemHolder()->GetItem(nToSlotType, nToSlotID));
		CHECK(pToItem == m_pPlayer->GetItemHolder()->GetItem(nFromSlotType, nFromSlotID));

		CHECK_EQUAL(pFromItem->m_nSlotID, nToSlotID);
		CHECK_EQUAL(pToItem->m_nSlotID, nFromSlotID);
	}	
}
