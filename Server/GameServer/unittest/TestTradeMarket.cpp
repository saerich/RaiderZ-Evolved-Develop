#include "stdafx.h"
#include "FBaseMockLink.h"
#include "GUTHelper.h"
#include "MockField.h"
#include "GItemManager.h"
#include "GServerResLoader.h"
#include "GUTHelper_Item.h"
#include "GItemHolder.h"
#include "GTradeMarket.h"
#include "CCommandTable.h"
#include "CTradeMarketConstant.h"
#include "CTradeMarketDefine.h"
#include "GUTHelper_Field.h"

SUITE(TradeMarket)
{
	struct FTradeMarket : public FBaseMockLink
	{
		FTradeMarket()
		{
			field = GUTHelper_Field::DefaultMockField();
			player = GUTHelper::NewEntityPlayer(field, vec3(0.0f, 0.0f, 0.0f));
			playerLink = GUTHelper::NewLink(player);
			itemHolder = player->GetItemHolder();
		}

		void PushInventoryTestItem(const int& slot,const int& itemID,const int& quantity)
		{
			GItem* const item = GUTHelper_Item::NewItem();
			item->m_nIUID = itemID;
			itemHolder->PushItem(SLOTTYPE_INVENTORY, slot, item);
			itemHolder->SetAmount(SLOTTYPE_INVENTORY, slot, quantity);
		}

		~FTradeMarket()
		{
			field->Destroy();
			GUTHelper::ClearLinks();
		}

		MockField*		field;
		GEntityPlayer*	player;
		MockLink*		playerLink;
		GItemHolder*	itemHolder;
	};

	TEST_FIXTURE(FTradeMarket, PutOn_Overload)
	{
		const int64	�ŷ������۾��̵�	= 100;
		const int	�����ۿ�������		= 10;
		const int	�����۽��Թ�ȣ		= 1;
		const int	�������			= 50;
		const int	�Ⱓ				= 3;

		PushInventoryTestItem(�����۽��Թ�ȣ, �ŷ������۾��̵�, �����ۿ�������);
		ASSERT_EQ(�����ۿ�������, itemHolder->GetItemCountInSlot(SLOTTYPE_INVENTORY, �����۽��Թ�ȣ));

		// �� ���� �ø� �� ����
		EXPECT_EQ(false, GTradeMarket::PutOn(player->GetUID(), �����۽��Թ�ȣ, �������, �����ۿ������� + 10, �Ⱓ));

		ASSERT_EQ(�����ۿ�������, itemHolder->GetItemCountInSlot(SLOTTYPE_INVENTORY, �����۽��Թ�ȣ));
		ASSERT_EQ(1, playerLink->GetCommandCount());
		{
			MCommand& cmd = playerLink->GetCommand(0);
			EXPECT_EQ(MC_SC_TRADEMARKET_EXCEPTION, cmd.GetID());
			EXPECT_EQ(1, cmd.GetParameterCount());
			UCHAR errorCode = 0;
			EXPECT_EQ(true, cmd.GetParameter(&errorCode, 0, MPT_UCHAR));
			EXPECT_EQ(TRADEMARKET_EXCEPTION_PUTON_INVALID_ITEM_COUNT, errorCode);
			playerLink->ResetCommands();
		}

// 		GTradeMarket::Search(player->GetUID(), �ŷ������۾��̵�);
// 		ASSERT_EQ(0, playerLink->GetCommandCount());
	}

	TEST_FIXTURE(FTradeMarket, PutOn_AllItems)
	{
		const int64	�ŷ������۾��̵�	= 100;
		const int	�����ۿ�������		= 10;
		const int	�����۽��Թ�ȣ		= 1;
		const int	�������			= 50;
		const int	�Ⱓ				= 3;

		PushInventoryTestItem(�����۽��Թ�ȣ, �ŷ������۾��̵�, �����ۿ�������);
		ASSERT_EQ(�����ۿ�������, itemHolder->GetItemCountInSlot(SLOTTYPE_INVENTORY, �����۽��Թ�ȣ));

		EXPECT_EQ(true, GTradeMarket::PutOn(player->GetUID(), �����۽��Թ�ȣ, �������, �����ۿ�������, �Ⱓ));

		ASSERT_EQ(0, itemHolder->GetItemCountInSlot(SLOTTYPE_INVENTORY, �����۽��Թ�ȣ));
		ASSERT_EQ(1, playerLink->GetCommandCount());
		{
			MCommand& cmd = playerLink->GetCommand(0);
			EXPECT_EQ(MC_SC_TRADEMARKET_PUTTED_ON, cmd.GetID());
			EXPECT_EQ(3, cmd.GetParameterCount());
			
			int Ŀ�ǵ忡�����Թ�ȣ = 0;
			EXPECT_EQ(true, cmd.GetParameter(&Ŀ�ǵ忡�����Թ�ȣ, 0, MPT_INT));
			EXPECT_EQ(�����۽��Թ�ȣ, Ŀ�ǵ忡�����Թ�ȣ);

			int Ŀ�ǵ忡�������۰��� = 0;
			EXPECT_EQ(true, cmd.GetParameter(&Ŀ�ǵ忡�������۰���, 1, MPT_INT));
			EXPECT_EQ(�����ۿ�������, Ŀ�ǵ忡�������۰���);

			int64 ��ǰ�ŷ��ҵ���Ϸù�ȣ = 0;
			EXPECT_EQ(true, cmd.GetParameter(&��ǰ�ŷ��ҵ���Ϸù�ȣ, 2, MPT_INT64));
			//EXPECT_TRUE(0 < ��ǰ�ŷ��ҵ���Ϸù�ȣ);

			playerLink->ResetCommands();
		}

		{
// 			GTradeMarket::Search(player->GetUID(), �ŷ������۾��̵�);
// 			ASSERT_EQ(1, playerLink->GetCommandCount());
// 			MCommand& cmd = playerLink->GetCommand(0);
// 			EXPECT_EQ(MC_SC_TRADEMARKET_ITEM_LIST, cmd.GetID());
// 
// 			vector<TRADEMARKET_ITEM> ��ǰ�ŷ��Ҿ����۸�ϰ��;
// 			EXPECT_EQ(true, cmd.GetBlob(��ǰ�ŷ��Ҿ����۸�ϰ��, 0));
// 			ASSERT_EQ(1, ��ǰ�ŷ��Ҿ����۸�ϰ��.size());
// 			const TRADEMARKET_ITEM& ���������(*��ǰ�ŷ��Ҿ����۸�ϰ��.begin());
// 			EXPECT_EQ(�ŷ������۾��̵�, ���������.id);
// 			EXPECT_EQ(�����ۿ�������, ���������.count);
		}
	}

	TEST_FIXTURE(FTradeMarket, PutOn_DivisionsOfItems)
	{
		const int64	�ŷ������۾��̵�	= 100;
		const int	�����ۿ�������		= 10;
		const int	�����۽��Թ�ȣ		= 1;
		const int	��ϰ���			= 4;
		const int	�������			= 50;
		const int	�Ⱓ				= 3;
		const int	������κ��丮�����������ۼ��� = �����ۿ������� - ��ϰ���;

		PushInventoryTestItem(�����۽��Թ�ȣ, �ŷ������۾��̵�, �����ۿ�������);
		ASSERT_EQ(�����ۿ�������, itemHolder->GetItemCountInSlot(SLOTTYPE_INVENTORY, �����۽��Թ�ȣ));

		EXPECT_EQ(true, GTradeMarket::PutOn(player->GetUID(), �����۽��Թ�ȣ, �������, ��ϰ���, �Ⱓ));

		ASSERT_EQ(������κ��丮�����������ۼ���, itemHolder->GetItemCountInSlot(SLOTTYPE_INVENTORY, �����۽��Թ�ȣ));
		ASSERT_EQ(1, playerLink->GetCommandCount());
		{
			MCommand& cmd = playerLink->GetCommand(0);
			EXPECT_EQ(MC_SC_TRADEMARKET_PUTTED_ON, cmd.GetID());
			EXPECT_EQ(3, cmd.GetParameterCount());

			int Ŀ�ǵ忡�����Թ�ȣ = 0;
			EXPECT_EQ(true, cmd.GetParameter(&Ŀ�ǵ忡�����Թ�ȣ, 0, MPT_INT));
			EXPECT_EQ(�����۽��Թ�ȣ, Ŀ�ǵ忡�����Թ�ȣ);

			int Ŀ�ǵ忡�������۰��� = 0;
			EXPECT_EQ(true, cmd.GetParameter(&Ŀ�ǵ忡�������۰���, 1, MPT_INT));
			EXPECT_EQ(��ϰ���, Ŀ�ǵ忡�������۰���);

			int64 ��ǰ�ŷ��ҵ���Ϸù�ȣ = 0;
			EXPECT_EQ(true, cmd.GetParameter(&��ǰ�ŷ��ҵ���Ϸù�ȣ, 2, MPT_INT64));
			//EXPECT_TRUE(0 < ��ǰ�ŷ��ҵ���Ϸù�ȣ);

			playerLink->ResetCommands();
		}
	}
} // SUITE(testTradeMarket)
