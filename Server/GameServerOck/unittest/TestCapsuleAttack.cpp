#include "stdafx.h"
#include "GTestForward.h"
#include "GFieldMgr.h"
#include "GCollision.h"
#include "GModuleCombat.h"
#include "FBasePlayer.h"
#include "GNPCHitcapsuleGroup.h"

SUITE(CapsuleAttack)
{
	struct Fixture : public FBasePlayer
	{
		Fixture()
		{
			m_pField = GUTHelper_Field::DefaultMockField();
			m_pNPC = GUTHelper_NPC::SpawnNPC(m_pField);
			m_pNPC->SetPos(vec3(0,0,0));

			GActorMeshInfo* pNPCMeshInfo = new GActorMeshInfo;
			pNPCMeshInfo->ReserveHitGroupForTest(3);
			CSHitGroup& CapsuleGroup1 = pNPCMeshInfo->GetHitGroup(0);
			CSHitGroup& CapsuleGroup2 = pNPCMeshInfo->GetHitGroup(1);
			CSHitCapsule hitCapsule;
			hitCapsule.m_nHitEffectCapsuleID = 0;
			hitCapsule.m_Capsule = MCapsule(vec3(0.f, 200.f, 0.f), vec3(0.f, 11.f, 0.f), 1000.f, 1000.f);	
			CapsuleGroup1.m_nGroupID = 123;
			CapsuleGroup1.m_vecHitCapsules.push_back(CSHitCapsule(hitCapsule));
			hitCapsule.m_nHitEffectCapsuleID = 1;
			hitCapsule.m_Capsule = MCapsule(vec3(0.f, 100.f, 0.f), vec3(0.f, 11.f, 0.f), 1000.f, 1000.f);	// ���� ��ϵǾ���, ���� ����� ĸ��
			CapsuleGroup1.m_vecHitCapsules.push_back(CSHitCapsule(hitCapsule));
			hitCapsule.m_nHitEffectCapsuleID = 2;
			hitCapsule.m_Capsule = MCapsule(vec3(0.f, 100.f, 0.f), vec3(0.f, 11.f, 0.f), 1000.f, 1000.f);	
			CapsuleGroup1.m_vecHitCapsules.push_back(CSHitCapsule(hitCapsule));
			
			m_pNPCInfo = new GNPCInfo();
			m_pNPCInfo->nID = 1;
			m_pNPCInfo->m_pMeshInfo = pNPCMeshInfo;
			
			m_pNPC->InitFromNPCInfo(m_pNPCInfo, NULL);
			m_pNPC->SetDir(vec3(0,1,0));
		}

		~Fixture() 
		{
			m_pField->Destroy();
			SAFE_DELETE(m_pNPCInfo);
		}

		GNPCInfo*		m_pNPCInfo;
		GEntityNPC*		m_pNPC;
		MockField*		m_pField;
		DECLWRAPPER_FieldInfoMgr;
		DECLWRAPPER_FieldMgr;
		DECLWRAPPER_TalentMgr;
	};

	TEST_FIXTURE(Fixture, ChangeCapsuleGroup_ByTrigger)
	{
		CHECK_EQUAL(0, m_pNPC->GetCapsuleGroupIndex());
		m_pNPC->GetNPCHitcapsuleGroup().ChangeCapsuleGroup_ByTrigger(-2);	// ��ȿ���� ���� ������ �ε���
		CHECK_EQUAL(0, m_pNPC->GetCapsuleGroupIndex());
		m_pNPC->GetNPCHitcapsuleGroup().ChangeCapsuleGroup_ByTrigger(1);
		CHECK_EQUAL(1, m_pNPC->GetCapsuleGroupIndex());
		m_pNPC->GetNPCHitcapsuleGroup().ChangeCapsuleGroup_ByTrigger(-1);
		CHECK_EQUAL(-1, m_pNPC->GetCapsuleGroupIndex());
		m_pNPC->GetNPCHitcapsuleGroup().ChangeCapsuleGroup_ByTrigger(2);
		CHECK_EQUAL(2, m_pNPC->GetCapsuleGroupIndex());
		m_pNPC->GetNPCHitcapsuleGroup().ChangeCapsuleGroup_ByTrigger(3);	// ������ �ִ� ĸ���׷��� �Ѿ �ε���
		CHECK_EQUAL(2, m_pNPC->GetCapsuleGroupIndex());
	}

	TEST_FIXTURE(Fixture, MultiHitCapsule)
	{
		CHECK_EQUAL(0, m_pNPC->GetCapsuleGroupIndex());

		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, vec3(0, 110, 0));	// 1�� ĸ���� ��ġ�� ����
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(10);

		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pNPC->GetUID());

		CHECK_EQUAL(90, m_pNPC->GetHP());	// �ϳ��� ĸ���� �ǰ�ó���Ǿ� ���ظ� 10�� �Ծ���Ѵ�.
	}

	TEST_FIXTURE(Fixture, HitClosest)
	{
		CHECK_EQUAL(0, m_pNPC->GetCapsuleGroupIndex());

		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, vec3(0, 110, 0));	// 1�� ĸ���� ��ġ�� ����
		pAttacker->SetDir(vec3(0,1,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(10);

		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pNPC->GetUID());

		// ���� ����� ĸ���� �ǰ�
		CHECK_EQUAL(0, m_pNPC->GetLastHitInfo().nCapsuleGroup);
		CHECK_EQUAL(1, m_pNPC->GetLastHitInfo().nCapsuleIndex);
	}

	TEST(GetDistanceToLongestPoint)
	{
		GTalentInfo* pTalentInfo = test.talent.NewTalentInfo();
		pTalentInfo->Cooking();
		CHECK_EQUAL(0.0f, pTalentInfo->GetDistanceToLongestPoint());	// �ƹ� ĸ���� ����

		// ���� ���׸�Ʈ �߰�
		{
			CSHitSegment infoHitSegment;
			infoHitSegment.m_vCapsules.push_back(MCapsule(vec3(0.0f,100.0f,0.0f), vec3(0.0f,100.0f,100.0f), 100.0f));
			infoHitSegment.m_vDoHitCheck.push_back(true);
			pTalentInfo->m_HitInfo.m_vSegments.push_back(infoHitSegment);
		}
		
		float DISTANCE_MAGIC_MOD = 0.8f;

		pTalentInfo->Cooking();
		CHECK_EQUAL(200.0f * DISTANCE_MAGIC_MOD, pTalentInfo->GetDistanceToLongestPoint());

		// ���� ���׸�Ʈ �߰� (�� �ָ� ����)
		{
			CSHitSegment infoHitSegment;
			infoHitSegment.m_vCapsules.push_back(MCapsule(vec3(0.0f,200.0f,0.0f), vec3(0.0f,200.0f,100.0f), 100.0f));
			infoHitSegment.m_vDoHitCheck.push_back(true);
			pTalentInfo->m_HitInfo.m_vSegments.push_back(infoHitSegment);
		}

		pTalentInfo->Cooking();
		CHECK_EQUAL(300.0f * DISTANCE_MAGIC_MOD, pTalentInfo->GetDistanceToLongestPoint());

		// ���� ���׸�Ʈ �߰� (���� �ָ� ������ üũ���� ����)
		{
			CSHitSegment infoHitSegment;
			infoHitSegment.m_vCapsules.push_back(MCapsule(vec3(0.0f,300.0f,0.0f), vec3(0.0f,300.0f,100.0f), 100.0f));
			infoHitSegment.m_vDoHitCheck.push_back(false);
			pTalentInfo->m_HitInfo.m_vSegments.push_back(infoHitSegment);
		}

		pTalentInfo->Cooking();
		CHECK_EQUAL(300.0f * DISTANCE_MAGIC_MOD, pTalentInfo->GetDistanceToLongestPoint());
	}

	TEST(TestActorHitInfoRadius)
	{
		// ĸ���� �ϳ��� ��� SimpleCapsule�� ������.
		GActorMeshInfo actorMeshInfo;
		CSMeshInfo meshInfo;

		CSHitGroup hitGroup;
		CSHitCapsule hitCapsule1;
		hitCapsule1.m_Capsule = MCapsule(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 100.0f), 50.0f);

		hitGroup.m_vecHitCapsules.push_back(hitCapsule1);
		meshInfo.m_mapHitGroupInfo.insert(map<int, CSHitGroup>::value_type(0, hitGroup));

		actorMeshInfo.Init(&meshInfo, 1.0f);


		CHECK_EQUAL(true, actorMeshInfo.IsSimple());
		CHECK_CLOSE(50.0f, actorMeshInfo.GetColRadius(), 0.001f);	// -y(ĳ����) ������ �ִ밪�� ���� = max y + radius
		CHECK_CLOSE(50.0f, actorMeshInfo.GetHitRadius(), 0.001f);

		// ĸ���� y��ġ ���� ------
		meshInfo.m_mapHitGroupInfo[0].m_vecHitCapsules[0].m_Capsule = MCapsule(vec3(0.0f, 200.0f, 0.0f), vec3(0.0f, -500.0f, 0.0f), 50.0f);

		actorMeshInfo.Init(&meshInfo, 1.0f);

		CHECK_CLOSE(550.0f, actorMeshInfo.GetColRadius(), 0.001f);
		CHECK_CLOSE(50.0f, actorMeshInfo.GetHitRadius(), 0.001f);


		// ĸ�� � �� �߰� -------

		CSHitCapsule hitCapsule2;
		hitCapsule2.m_Capsule = MCapsule(vec3(0.0f, -500.0f, 0.0f), vec3(0.0f, -1000.0f, 100.0f), 80.0f);
		meshInfo.m_mapHitGroupInfo[0].m_vecHitCapsules.push_back(hitCapsule2);

		CSHitCapsule hitCapsule3;
		hitCapsule3.m_Capsule = MCapsule(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 500.0f, 100.0f), 200.0f);
		meshInfo.m_mapHitGroupInfo[0].m_vecHitCapsules.push_back(hitCapsule3);

		actorMeshInfo.Init(&meshInfo, 1.0f);

		CHECK_CLOSE(1080.0f, actorMeshInfo.GetColRadius(), 0.001f);		// -1000 * -1 + 80

		// Scale ����
		actorMeshInfo.Init(&meshInfo, 2.0f);

		CHECK_CLOSE(2160.0f, actorMeshInfo.GetColRadius(), 0.001f);		// -1000 * -1 + 80
	}

	TEST(TestHitCapsuleActiveTime)
	{
		CSHitCapsule hitCapsule;

		CHECK_EQUAL(true, hitCapsule.IsAlwaysActive());

		hitCapsule.SetActiveTime(0.5f, 0.7f);

		CHECK_EQUAL(false, hitCapsule.IsAlwaysActive());
		CHECK_EQUAL(false, hitCapsule.CheckActiveTime(0.2f));
		CHECK_EQUAL(true, hitCapsule.CheckActiveTime(0.5f));
		CHECK_EQUAL(true, hitCapsule.CheckActiveTime(0.6f));
		CHECK_EQUAL(false, hitCapsule.CheckActiveTime(1.0f));
	}



	TEST_FIXTURE(Fixture, TestHitCapsuleActiveTime_Group)
	{
		CSHitGroup& hitGroup = m_pNPCInfo->m_pMeshInfo->GetHitGroup();
		CHECK_EQUAL(3, hitGroup.m_vecHitCapsules.size());

		MCapsule capHit = MCapsule(m_pNPC->GetPos(), m_pNPC->GetPos() + vec3(0,0,100), 2000.0f);

		int8 nCapsuleGroupIndex = -1;
		int8 nCapsuleIndex = -1;

		bool bHit = GCollision::HitTest2(m_pNPC, capHit, nCapsuleGroupIndex, nCapsuleIndex);

		CHECK_EQUAL(true, bHit);

		CHECK_EQUAL(0, (int)nCapsuleGroupIndex);
		CHECK_EQUAL(1, (int)nCapsuleIndex);			// ���� ����� ĸ���� ������


		// active time ����
		GTalentInfo* pTalent = test.talent.MakeDamageTalent(10, 1000);
		pTalent->m_fDurationTime = 10.0f;
		pTalent->Cooking();

		CHECK_CLOSE(10.0f, pTalent->GetExpiredTime(), 0.01f);

		hitGroup.m_vecHitCapsules[0].SetActiveTime(0.0f, 0.5f);
		hitGroup.m_vecHitCapsules[1].SetActiveTime(1.0f, 2.0f);
		hitGroup.m_vecHitCapsules[2].SetActiveTime(2.0f, 3.0f);

		m_pNPC->GetModuleCombat()->UseTalent(pTalent, TALENT_TARGET_INFO::Invalid(), false);

		m_pField->Update(0.2f);	// �ŷ�Ʈ ������� 0.2�� ������

		bHit = GCollision::HitTest2(m_pNPC, capHit, nCapsuleGroupIndex, nCapsuleIndex);
		CHECK_EQUAL(true, bHit);
		CHECK_EQUAL(0, (int)nCapsuleIndex);			// 0�� ĸ���� ��ȿ�ð��� �´�.

		m_pField->Update(0.6f);	// �ŷ�Ʈ ������� 0.8�� ������
		bHit = GCollision::HitTest2(m_pNPC, capHit, nCapsuleGroupIndex, nCapsuleIndex);
		CHECK_EQUAL(false, bHit);					// ��ȿ�� ���� ĸ���� ����.

		m_pField->Update(0.7f);	// �ŷ�Ʈ ������� 1.5�� ������
		bHit = GCollision::HitTest2(m_pNPC, capHit, nCapsuleGroupIndex, nCapsuleIndex);
		CHECK_EQUAL(true, bHit);
		CHECK_EQUAL(1, (int)nCapsuleIndex);			// 1�� ĸ���� ��ȿ�ð��� �´�.


		m_pField->Update(1.0f);	// �ŷ�Ʈ ������� 2.5�� ������
		bHit = GCollision::HitTest2(m_pNPC, capHit, nCapsuleGroupIndex, nCapsuleIndex);
		CHECK_EQUAL(true, bHit);
		CHECK_EQUAL(2, (int)nCapsuleIndex);			// 2�� ĸ���� ��ȿ�ð��� �´�.

	}

}
