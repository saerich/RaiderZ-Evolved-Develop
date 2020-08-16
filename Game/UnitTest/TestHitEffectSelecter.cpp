#include "stdafx.h"
#include "XTalentHitParam.h"
#include "XHitEffectSelecter.h"
#include "XUnitTestUtil.h"
#include "TestGame.h"
#include "XUnitTestHelper.h"
#include "XMyPlayer.h"
#include "XNonPlayer.h"

SUITE(HitEffectSelecter)
{
	class FHitEffectSelecter : public XHitEffectSelecter
	{
	public:
		XMyPlayer* m_pMyPlayer;
		TestGame tGame;

		vector<XTalentHitParam> vecHitParam_sort;
		vector<XTalentHitParam> vecHitParam_order;
	public:
		FHitEffectSelecter()
		{			
			m_pMyPlayer = tGame.GetMyPlayer();

			XUnitTestHelper::NewNPCInfo(1, L"SampleNPC1", 3);
			XUnitTestHelper::NewNPCInfo(2, L"SampleNPC2", 4);

			CreateSortHitParam();
			CreateOrderHitParam();
		}
		~FHitEffectSelecter()
		{
			vecHitParam_sort.clear();
			info.npc->Clear();
		}

		XNonPlayer* CreateNPC(vec3 vPos, int nID)
		{
			XUnitTestHelper helper;
			const MUID uidNPC = global.system->GenerateLocalUID();
			XNonPlayer* pNPC = helper.InNPC(uidNPC, nID, vPos);

			return pNPC;
		}

		void CreateSortHitParam()
		{
			int nTalentID = 10;

			XNonPlayer* pNon1 = CreateNPC(vec3(100, 100, 0), 1);
			XNonPlayer* pNon2 = CreateNPC(vec3(100, 120, 0), 1);
			XNonPlayer* pNon3 = CreateNPC(vec3(100, 140, 0), 1);

			AddHitParam_sort(m_pMyPlayer->GetUIID(), pNon1->GetUIID(), nTalentID);
			AddHitParam_sort(m_pMyPlayer->GetUIID(), pNon2->GetUIID(), nTalentID);
			AddHitParam_sort(m_pMyPlayer->GetUIID(), pNon3->GetUIID(), nTalentID);
		}

		void CreateOrderHitParam()
		{
			int nTalentID = 10;

			XNonPlayer* pNon1 = CreateNPC(vec3(100, 100, 0), 1);
			XNonPlayer* pNon2 = CreateNPC(vec3(100, 120, 0), 1);
			XNonPlayer* pNon3 = CreateNPC(vec3(100, 140, 0), 1);

			AddHitParam_order(m_pMyPlayer->GetUIID(), pNon1->GetUIID(), nTalentID);
			AddHitParam_order(m_pMyPlayer->GetUIID(), pNon2->GetUIID(), nTalentID);
			AddHitParam_order(m_pMyPlayer->GetUIID(), pNon3->GetUIID(), nTalentID);
		}

		void AddHitParam_sort(int nAttaker, int nVictim, int nTalentID)
		{
			XTalentHitParam param;
			param.Build_NoMotionFactor(nAttaker, nVictim, nTalentID);
			vecHitParam_sort.push_back(param);
		}

		void AddHitParam_order(int nAttaker, int nVictim, int nTalentID)
		{
			XTalentHitParam param;
			param.Build_NoMotionFactor(nAttaker, nVictim, nTalentID);
			vecHitParam_order.push_back(param);
		}

		void InitSecondaryEffect()
		{
			for(vector<XTalentHitParam>::iterator it_sort = vecHitParam_sort.begin(); it_sort != vecHitParam_sort.end(); ++it_sort)
			{
				it_sort->bSecondaryEffect = true;
			}

			for(vector<XTalentHitParam>::iterator it_order = vecHitParam_order.begin(); it_order != vecHitParam_order.end(); ++it_order)
			{
				it_order->bSecondaryEffect = true;
			}

		}

		void Init_Sort()
		{
			// ��Ƽ� ó���ϴ� ��Ʈ ����Ʈ
			AddHitInfo(&(vecHitParam_sort[0]), 200);
			AddHitInfo(&(vecHitParam_sort[1]), 200);
			AddHitInfo(&(vecHitParam_sort[2]), 200);

			CHECK_EQUAL(m_vecHitbyTalentInfo.size(), 1);
			CHECK_EQUAL(m_vecHitbyTalentInfo[0].vecHitParam.size(), 3);

			for(int i = 0; i < 3; ++i)
			{
				// ��Ƽ� ó���ϴ°�... �ϴ� Secondary�̴�.
				CHECK_EQUAL(m_vecHitbyTalentInfo[0].vecHitParam[i]->bSecondaryEffect, true);
			}
		}

		void Init_Order()
		{
			// ���������� �ϴ� ��Ʈ ����Ʈ
			// ���������� �ϴ°�... ������ ������ �ʴ´�.
			// �׷��� 1���� ������ First Effect�̴�.
			AddHitInfo(&(vecHitParam_order[0]), 0);
			CHECK_EQUAL(vecHitParam_order[0].bSecondaryEffect, false);

			AddHitInfo(&(vecHitParam_order[1]), 0);
			CHECK_EQUAL(vecHitParam_order[1].bSecondaryEffect, true);

			AddHitInfo(&(vecHitParam_order[2]), 0);
			CHECK_EQUAL(vecHitParam_order[2].bSecondaryEffect, true);

			CHECK_EQUAL(m_vecHitbyTalentInfo.size(), 1);
			CHECK_EQUAL(m_vecHitbyTalentInfo[0].vecHitParam.size(), 0);
		}
	};

	TEST_FIXTURE(FHitEffectSelecter, Test_HitSecondaryEffect_SortAdd)
	{
		Init_Sort();

		InitSecondaryEffect();
	}

	TEST_FIXTURE(FHitEffectSelecter, Test_HitSecondaryEffect_Sort_HitStart)
	{
		Init_Sort();

		// ������ ù��° ó������ ������ �� ����.
		StartHitEffectSelect(&(vecHitParam_sort[1]));
		CHECK_EQUAL(vecHitParam_sort[0].bSecondaryEffect, false);
		CHECK_EQUAL(vecHitParam_sort[1].bSecondaryEffect, true);
		CHECK_EQUAL(vecHitParam_sort[2].bSecondaryEffect, true);

		// �׷��Ƿ� ������ ����. �����ϹǷ�...
		CHECK_EQUAL(m_vecHitbyTalentInfo.size(), 0);

		StartHitEffectSelect(&(vecHitParam_sort[0]));
		CHECK_EQUAL(m_vecHitbyTalentInfo.size(), 0);
		StartHitEffectSelect(&(vecHitParam_sort[2]));
		CHECK_EQUAL(m_vecHitbyTalentInfo.size(), 0);

		InitSecondaryEffect();
	}

	TEST_FIXTURE(FHitEffectSelecter, Test_HitSecondaryEffect_OrderAdd)
	{
		Init_Order();

		InitSecondaryEffect();
	}

	TEST_FIXTURE(FHitEffectSelecter, Test_HitSecondaryEffect_OrderAdd_2)
	{
		AddHitInfo(&(vecHitParam_order[0]), 0);
		CHECK_EQUAL(vecHitParam_order[0].bSecondaryEffect, false);

		AddHitInfo(&(vecHitParam_order[1]), 0);
		CHECK_EQUAL(vecHitParam_order[1].bSecondaryEffect, true);

		
		// �ٸ� ��Ʈ �ŷ�Ʈ ����
		XTalentHitParam param;
		param.Build_NoMotionFactor(vecHitParam_order[0].nAttackerUIID, vecHitParam_order[0].nVictimUIID, 11);
		AddHitInfo(&param, 0);
		CHECK_EQUAL(param.bSecondaryEffect, false);

		// �������� �����ð����Ŀ� ������ ó���Ѵ�.
		CHECK_EQUAL(m_vecHitbyTalentInfo.size(), 2);
		CHECK_EQUAL(m_vecHitbyTalentInfo[0].vecHitParam.size(), 0);
		CHECK_EQUAL(m_vecHitbyTalentInfo[1].vecHitParam.size(), 0);

		InitSecondaryEffect();
	}

	TEST_FIXTURE(FHitEffectSelecter, Test_HitSecondaryEffect_OrderAutoDel)
	{
		Init_Order();

		// ���� �ð� ���� ����...
		Update(1.5f);
		CHECK_EQUAL(m_vecHitbyTalentInfo.size(), 0);

		InitSecondaryEffect();
	}

	TEST_FIXTURE(FHitEffectSelecter, Test_HitSecondaryEffect_ForceFirtstHitEffectByGrade_Sort)
	{
		// ����� 4�̻��� ������ 1����!!!
		XNonPlayer* pNon1 = CreateNPC(vec3(100, 100, 0), 1);
		XNonPlayer* pNon2 = CreateNPC(vec3(100, 120, 0), 2);
		XNonPlayer* pNon3 = CreateNPC(vec3(100, 140, 0), 1);

		XTalentHitParam param1, param2, param3;
		param1.Build_NoMotionFactor(m_pMyPlayer->GetUIID(), pNon1->GetUIID(), 10);
		param2.Build_NoMotionFactor(m_pMyPlayer->GetUIID(), pNon2->GetUIID(), 10);
		param3.Build_NoMotionFactor(m_pMyPlayer->GetUIID(), pNon3->GetUIID(), 10);

		// ��Ƽ� ó���ϴ� ��Ʈ ����Ʈ
		AddHitInfo(&param1, 200);
		AddHitInfo(&param2, 200);
		AddHitInfo(&param3, 200);

		CHECK_EQUAL(param1.bSecondaryEffect, true);
		// ������ 1��. �̹� ���� ���� �����̴�.
		CHECK_EQUAL(param2.bSecondaryEffect, false);
		CHECK_EQUAL(param3.bSecondaryEffect, true);

		CHECK_EQUAL(m_vecHitbyTalentInfo.size(), 1);
		CHECK_EQUAL(m_vecHitbyTalentInfo[0].vecHitParam.size(), 3);


		// ������ �õ��غ���.
		StartHitEffectSelect(&param2);

		CHECK_EQUAL(param1.bSecondaryEffect, false);
		CHECK_EQUAL(param2.bSecondaryEffect, false);
		CHECK_EQUAL(param3.bSecondaryEffect, true);
	}
}