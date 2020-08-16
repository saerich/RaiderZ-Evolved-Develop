#include "StdAfx.h"
#include "GTestForward.h"
#include "GFieldMgr.h"
#include "CSHitCapsuleIndexMgr.h"
#include "GModuleCombat.h"
#include "GTalent.h"
#include "GTalentInfoMgr.h"
#include "GHateTable.h"
#include "GNPCSwallow.h"

SUITE(HitCapsuleIndexMgr)
{
	struct FHitCapsuleIndexMgr
	{
		FHitCapsuleIndexMgr()
		{
			m_pField = GUTHelper_Field::DefaultMockField();

			m_pNPC = GUTHelper_NPC::SpawnNPC(m_pField);

			// ĸ�� ���� �߰�
			m_pNPCInfo = new GNPCInfo;
			m_pNPCInfo_Mode1 = new GNPCInfo;
			m_pNPCInfo->vecModes.push_back(m_pNPCInfo_Mode1);
			m_pNPCInfo->m_pMeshInfo = new GActorMeshInfo();
			m_pNPCInfo->m_pMeshInfo->ReserveHitGroupForTest(5);
			m_pNPCInfo->nAA = NAAT_ALWAYS;
			m_pNPCInfo->nMode = NPC_MODE_DEFAULT;
			m_pNPCInfo->nHitTestIndex = DEFAULT_HIT_INDEX_MODE0;			// 0�� ����� �⺻ ��Ʈ�׷���̵�
			m_pNPCInfo->nEatOwnerBuffID = 560;
			m_pNPCInfo->nEatTargetBuffID = 561;
			m_pNPCInfo->fEatPeriod = 2.0f;
			m_pNPCInfo_Mode1->nMode = NPC_MODE_1;
			m_pNPCInfo_Mode1->nHitTestIndex = 21;	// 1�� ����� �⺻ ��Ʈ�׷���̵�

			test.buff.NewBuffInfo(m_pNPCInfo->nEatOwnerBuffID);
			test.buff.NewBuffInfo(m_pNPCInfo->nEatTargetBuffID);

			m_pNPC->SetNPCInfo(m_pNPCInfo);
		}

		~FHitCapsuleIndexMgr()
		{
			m_pField->Destroy();
			SAFE_DELETE(m_pNPCInfo->m_pMeshInfo);
			SAFE_DELETE(m_pNPCInfo);			
		}

		static const int DEFAULT_HIT_INDEX_MODE0 = 77;

		MockField*		m_pField;
		GEntityNPC*		m_pNPC;
		GNPCInfo*		m_pNPCInfo;
		GNPCInfo*		m_pNPCInfo_Mode1;
		DECLWRAPPER_FieldInfoMgr;
		DECLWRAPPER_FieldMgr;
		DECLWRAPPER_TalentMgr;
		DECLWRAPPER_BuffMgr;
	};

	TEST(Default)
	{
		CSHitCapsuleIndexMgr mgrHitIndex;
		CHECK_EQUAL(INVALID_HITCAPSULEGROUP_ID, mgrHitIndex.GetGroupIndex());
	}

	TEST(ModeChange)
	{
		CSHitCapsuleIndexMgr mgrHitIndex;
		mgrHitIndex.InsertNPCModeGroupIndex(0, 77);
		mgrHitIndex.InsertNPCModeGroupIndex(1, 21);
		CHECK_EQUAL(77, (int)mgrHitIndex.GetGroupIndex());
		mgrHitIndex.ChangeNPCMode(1);
		CHECK_EQUAL(21, (int)mgrHitIndex.GetGroupIndex());
	}

	TEST(UseTalent)
	{
		CSHitCapsuleIndexMgr mgrHitIndex;
		mgrHitIndex.InsertNPCModeGroupIndex(0, 77);
		CHECK_EQUAL(77, (int)mgrHitIndex.GetGroupIndex());
		mgrHitIndex.Change_ByTalent(55);							// �ŷ�Ʈ ����
		CHECK_EQUAL(55, (int)mgrHitIndex.GetGroupIndex());		// �ŷ�Ʈ�� ���� ��ȭ�� �켱������ �� ����
		mgrHitIndex.Change_ByTalentComplete();	// �ŷ�Ʈ ��
		CHECK_EQUAL(77, (int)mgrHitIndex.GetGroupIndex());
	}

	TEST(UseTalent_Invalid)
	{
		CSHitCapsuleIndexMgr mgrHitIndex;
		mgrHitIndex.InsertNPCModeGroupIndex(0, 77);
		CHECK_EQUAL(77, (int)mgrHitIndex.GetGroupIndex());
		mgrHitIndex.Change_ByTalent(INVALID_HITCAPSULEGROUP_ID);							// �ŷ�Ʈ ����
		CHECK_EQUAL(INVALID_HITCAPSULEGROUP_ID, (int)mgrHitIndex.GetGroupIndex());		// �ŷ�Ʈ�� ���� ��ȭ�� �켱������ �� ����
		mgrHitIndex.Change_ByTalentComplete();	// �ŷ�Ʈ ��
		CHECK_EQUAL(77, (int)mgrHitIndex.GetGroupIndex());
	}

	TEST(Trigger)
	{
		CSHitCapsuleIndexMgr mgrHitIndex;
		mgrHitIndex.InsertNPCModeGroupIndex(0, 77);
		mgrHitIndex.InsertNPCModeGroupIndex(1, 21);
		CHECK_EQUAL(77, (int)mgrHitIndex.GetGroupIndex());
		mgrHitIndex.Change_ByTrigger(12);					
		CHECK_EQUAL(12, (int)mgrHitIndex.GetGroupIndex());		// Ʈ���ſ� ���� ��尡 ������ �ٲ�
		mgrHitIndex.ChangeNPCMode(1);	
		CHECK_EQUAL(21, (int)mgrHitIndex.GetGroupIndex());
		mgrHitIndex.ChangeNPCMode(0);	
		CHECK_EQUAL(12, (int)mgrHitIndex.GetGroupIndex());		// ��尡 �ǵ��ư��� ��ȭ�� �����Ǿ�����
	}

	TEST_FIXTURE(FHitCapsuleIndexMgr, NPC_ModeChange)
	{
		CHECK_EQUAL(DEFAULT_HIT_INDEX_MODE0, (int)m_pNPC->GetCapsuleGroupIndex());
		m_pNPC->ChangeMode(NPC_MODE_1);
		CHECK_EQUAL(21, (int)m_pNPC->GetCapsuleGroupIndex());
	}

	TEST_FIXTURE(FHitCapsuleIndexMgr, NPC_UseTalent_Change)
	{
		// �ʱ� ĸ�� ID�� 0������� ���� ���
		CHECK_EQUAL(DEFAULT_HIT_INDEX_MODE0, (int)m_pNPC->GetCapsuleGroupIndex());

		// �ŷ�Ʈ ����ϱ�
		GTalentInfo* pTestTalent = test.talent.MakeDamageTalent(1);
		pTestTalent->m_nHitCapsuleGroupIndex = 1;		// �ŷ�Ʈ ���Ǵ� 1�� ���� �ٲ�
		TALENT_TARGET_INFO target_info;
		target_info.uidTarget = m_pNPC->GetUID();
		m_pNPC->GetModuleCombat()->UseTalent(pTestTalent, target_info, true);
		GTalent* pTaelnt = m_pNPC->GetModuleCombat()->GetTalent();
		TVALID(pTaelnt);
		CHECK_EQUAL(TALENT_PHASE_ACT, pTaelnt->GetPhase());
		CHECK_EQUAL(1, (int)m_pNPC->GetCapsuleGroupIndex());	// �ŷ�Ʈ ������̶� 1�� ���
		m_pNPC->GetField()->Update(0.0f);
		CHECK_EQUAL(TALENT_PHASE_FINISH, pTaelnt->GetPhase());
		CHECK_EQUAL(DEFAULT_HIT_INDEX_MODE0, (int)m_pNPC->GetCapsuleGroupIndex());
	}

	TEST_FIXTURE(FHitCapsuleIndexMgr, NPC_UseSwallowTalent_SuccessChange)
	{
		// �ʱ� ĸ�� ID�� 0������� ���� ���
		CHECK_EQUAL(DEFAULT_HIT_INDEX_MODE0, (int)m_pNPC->GetCapsuleGroupIndex());

		// �ŷ�Ʈ ����
		GUTHelper		m_Helper;
		GTalentInfo*	pTestTalent;
		pTestTalent = m_Helper.NewTalentInfo();
		pTestTalent->m_nSkillType = ST_EXTRA_ACTIVE;
		pTestTalent->m_nExtraActive = TEAT_NPC_ONLY_EAT;
		pTestTalent->m_nTiming = TC_HIT_TALENT;
		pTestTalent->m_EffectInfo.m_nRelation = CSEffectInfo::RELATION_ENEMY;
		m_Helper.SetTalentDamage(pTestTalent, 1);		// ������ 1�� ����
		m_Helper.AddSimpleHitInfo(pTestTalent, 0.0f);	// �ٷ� �����ϴ� �ŷ�Ʈ��

		// ���� Ÿ���� AREA
		pTestTalent->m_EffectInfo.m_fRadius = 1.0f;
		pTestTalent->m_fDurationTime = 2.0f;		// aitime 2��
		pTestTalent->m_fExtraPhaseTime = 2.0f;	// �Դ� �ִϸ��̼ǽð�
		pTestTalent->m_fExtraPhaseTime2 = 3.0f;	// ��� �ִϸ��̼ǽð�
		pTestTalent->m_fExtraPhaseTime3 = 4.0f;	// ��� �ִϸ��̼ǽð� (���ݹ޾� ������ ���)
		pTestTalent->m_nExtraActiveParam1 = 10;  // �Դ� �ð�

		// ��Ʈĸ���׷� ��������
		pTestTalent->m_nHitCapsuleGroupIndex = 1;		// �ŷ�Ʈ ���Ǵ� 1�� ���� �ٲ�
		pTestTalent->m_nExtraHitCapsuleGroup = 2;
		pTestTalent->m_nExtraHitCapsuleGroup2 = 3;
		pTestTalent->m_nExtraHitCapsuleGroup3 = 4;

		pTestTalent->Cooking();

		// �� ����
		GEntityPlayer*	m_pPlayer = GUTHelper::NewEntityPlayer(m_pField);
	

		// �ŷ�Ʈ ����ϱ�
		TALENT_TARGET_INFO target_info;
		target_info.uidTarget = m_pNPC->GetUID();
		m_pNPC->GetHateTable().AddPoint_FoundEnemy(m_pNPC);
		m_pNPC->GetModuleCombat()->UseTalent(pTestTalent, target_info, true);
		GTalent* pTaelnt = m_pNPC->GetModuleCombat()->GetTalent();
		TVALID(pTaelnt);
		CHECK_EQUAL(TALENT_PHASE_ACT, pTaelnt->GetPhase());
		CHECK_EQUAL(1, (int)m_pNPC->GetCapsuleGroupIndex());	// �ŷ�Ʈ ������̶� 1�� ���
		m_pNPC->GetField()->Update(0.0f);
		CHECK_EQUAL(TALENT_PHASE_EXTRA, pTaelnt->GetPhase());
		CHECK_EQUAL(2, (int)m_pNPC->GetCapsuleGroupIndex());
		m_pField->Update(pTestTalent->m_fExtraPhaseTime+(float)pTestTalent->m_nExtraActiveParam1);
		CHECK_EQUAL(TALENT_PHASE_EXTRA2, pTaelnt->GetPhase());
		CHECK_EQUAL(3, (int)m_pNPC->GetCapsuleGroupIndex());
		m_pField->Update(pTestTalent->m_fExtraPhaseTime2);
		CHECK_EQUAL(TALENT_PHASE_FINISH, pTaelnt->GetPhase());
		CHECK_EQUAL(DEFAULT_HIT_INDEX_MODE0, (int)m_pNPC->GetCapsuleGroupIndex());
	}

	TEST_FIXTURE(FHitCapsuleIndexMgr, NPC_UseSwallowTalent_FailedChange)
	{
		// �ʱ� ĸ�� ID�� 0������� ���� ���
		CHECK_EQUAL(DEFAULT_HIT_INDEX_MODE0, (int)m_pNPC->GetCapsuleGroupIndex());

		// �ŷ�Ʈ ����
		GUTHelper		m_Helper;
		GTalentInfo*	pTestTalent;
		pTestTalent = m_Helper.NewTalentInfo();
		pTestTalent->m_nSkillType = ST_EXTRA_ACTIVE;
		pTestTalent->m_nExtraActive = TEAT_NPC_ONLY_EAT;
		pTestTalent->m_nTiming = TC_HIT_TALENT;
		pTestTalent->m_EffectInfo.m_nRelation = CSEffectInfo::RELATION_ENEMY;
		m_Helper.SetTalentDamage(pTestTalent, 1);		// ������ 1�� ����
		m_Helper.AddSimpleHitInfo(pTestTalent, 0.0f);	// �ٷ� �����ϴ� �ŷ�Ʈ��

		// ���� Ÿ���� AREA
		pTestTalent->m_EffectInfo.m_fRadius = 1.0f;
		pTestTalent->m_fDurationTime = 2.0f;		// aitime 2��
		pTestTalent->m_fExtraPhaseTime = 2.0f;	// �Դ� �ִϸ��̼ǽð�
		pTestTalent->m_fExtraPhaseTime2 = 3.0f;	// ��� �ִϸ��̼ǽð�
		pTestTalent->m_fExtraPhaseTime3 = 4.0f;	// ��� �ִϸ��̼ǽð� (���ݹ޾� ������ ���)
		pTestTalent->m_nExtraActiveParam1 = 10;  // �Դ� �ð�

		// ��Ʈĸ���׷� ��������
		pTestTalent->m_nHitCapsuleGroupIndex = 1;		// �ŷ�Ʈ ���Ǵ� 1�� ���� �ٲ�
		pTestTalent->m_nExtraHitCapsuleGroup = 2;
		pTestTalent->m_nExtraHitCapsuleGroup2 = 3;
		pTestTalent->m_nExtraHitCapsuleGroup3 = 4;

		pTestTalent->Cooking();

		// �� ����
		GEntityPlayer*	m_pPlayer = GUTHelper::NewEntityPlayer(m_pField);

		// �ŷ�Ʈ ����ϱ�
		TALENT_TARGET_INFO target_info;
		target_info.uidTarget = m_pNPC->GetUID();
		m_pNPC->GetHateTable().AddPoint_FoundEnemy(m_pNPC);
		m_pNPC->GetModuleCombat()->UseTalent(pTestTalent, target_info, true);
		GTalent* pTaelnt = m_pNPC->GetModuleCombat()->GetTalent();
		TVALID(pTaelnt);
		CHECK_EQUAL(TALENT_PHASE_ACT, pTaelnt->GetPhase());
		CHECK_EQUAL(1, (int)m_pNPC->GetCapsuleGroupIndex());	// �ŷ�Ʈ ������̶� 1�� ���
		m_pNPC->GetField()->Update(0.0f);
		CHECK_EQUAL(TALENT_PHASE_EXTRA, pTaelnt->GetPhase());
		CHECK_EQUAL(2, (int)m_pNPC->GetCapsuleGroupIndex());
		m_pField->Update(0.0f);
		m_pNPC->GetNPCSwallow().Spewup(true);	// ���ݹ޾� ��
		m_pNPC->GetModuleCombat()->Update(0.0f);
		CHECK_EQUAL(TALENT_PHASE_EXTRA3, pTaelnt->GetPhase());
		CHECK_EQUAL(4, (int)m_pNPC->GetCapsuleGroupIndex());
		m_pField->Update(pTestTalent->m_fExtraPhaseTime3);
		CHECK_EQUAL(TALENT_PHASE_FINISH, pTaelnt->GetPhase());
		CHECK_EQUAL(DEFAULT_HIT_INDEX_MODE0, (int)m_pNPC->GetCapsuleGroupIndex());
	}

}