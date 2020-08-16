#include "stdafx.h"
#include "GTestForward.h"
#include "GTalent.h"
#include "GModuleCombat.h"
#include "FBasePlayer.h"

SUITE(TalentCasting)
{
	struct FTalent : public FBasePlayer
	{
		FTalent()
		{
			m_pField = GUTHelper_Field::DefaultMockField();
			m_pPlayer = NewEntityPlayer(m_pField);
			m_pLinkTester = test.network.NewLink(m_pPlayer);
			swprintf_s(m_pPlayer->GetPlayerInfo()->szName, L"Tester");

		}
		~FTalent()
		{
			m_pField->Destroy();
			test.network.ClearLinks();
		}

		MockField*		m_pField;
		GEntityPlayer*	m_pPlayer;
		MockLink*		m_pLinkTester;
		DECLWRAPPER_FieldInfoMgr;
		DECLWRAPPER_FieldMgr;
		DECLWRAPPER_TalentMgr;
	};

	TEST_FIXTURE(FTalent, InstantActWithoutCastingTime)
	{
		GTalentInfo* pTalentInfo = test.talent.NewTalentInfo();
		pTalentInfo->m_fDurationTime = 10.0f;
		pTalentInfo->Cooking();

		test.talent.UseTalent(m_pPlayer, pTalentInfo, m_pPlayer->GetUID());
		GTalent* pTalent = m_pPlayer->GetModuleCombat()->GetTalent();
		TVALID(pTalent);
 		CHECK_EQUAL(TALENT_PHASE_ACT, pTalent->GetPhase());
	}

	TEST_FIXTURE(FTalent, MaintainPreparePhaseInCastingTime)
	{
		GTalentInfo* pTalentInfo = test.talent.NewTalentInfo();
		pTalentInfo->m_fCastingTime = 5.0f;
		pTalentInfo->m_fDurationTime = 10.0f;
		pTalentInfo->Cooking();

		test.talent.UseTalent(m_pPlayer, pTalentInfo, m_pPlayer->GetUID());
		GTalent* pTalent = m_pPlayer->GetModuleCombat()->GetTalent();
		TVALID(pTalent);
		CHECK_EQUAL(TALENT_PHASE_PREPARE, pTalent->GetPhase());
		pTalent->Update(5.0f);	// ĳ���ýð���ŭ ���
		CHECK_EQUAL(TALENT_PHASE_PREPARE, pTalent->GetPhase());	// ACT ��û�� ���ö����� ���
		pTalent->Update(5.0f);	// ĳ���ýð���ŭ ���
		CHECK_EQUAL(TALENT_PHASE_PREPARE, pTalent->GetPhase());	// ACT ��û�� ���ö����� ���
	}

	TEST_FIXTURE(FTalent, ProceedActPhaseByUserRequest)	// ����ڰ� ACT ��û�� �ؼ� ACT ������� �Ѿ��
	{
		GTalentInfo* pTalentInfo = test.talent.NewTalentInfo();
		pTalentInfo->m_fCastingTime = 5.0f;
		pTalentInfo->m_fDurationTime = 10.0f;
		pTalentInfo->Cooking();

		test.talent.UseTalent(m_pPlayer, pTalentInfo, m_pPlayer->GetUID());
		GTalent* pTalent = m_pPlayer->GetModuleCombat()->GetTalent();
		TVALID(pTalent);
		CHECK_EQUAL(TALENT_PHASE_PREPARE, pTalent->GetPhase());
		pTalent->Update(5.0f);	// ĳ���ýð���ŭ ���
		CHECK_EQUAL(TALENT_PHASE_PREPARE, pTalent->GetPhase());	// ACT ��û�� ���ö����� ���
		pTalent->ActTalent(m_pPlayer->GetUID());	
		CHECK_EQUAL(TALENT_PHASE_ACT, pTalent->GetPhase());	
	}

	TEST_FIXTURE(FTalent, AutoDelayedByEalryUserRequest)	// ����ڰ� ACT ��û�� ĳ���� �Ϸ�Ǳ� ���� ������ ����ߴٰ� ó���ϱ�
	{
		GTalentInfo* pTalentInfo = test.talent.NewTalentInfo();
		pTalentInfo->m_fCastingTime = 5.0f;
		pTalentInfo->m_fDurationTime = 10.0f;
		pTalentInfo->Cooking();

		test.talent.UseTalent(m_pPlayer, pTalentInfo, m_pPlayer->GetUID());
		GTalent* pTalent = m_pPlayer->GetModuleCombat()->GetTalent();
		TVALID(pTalent);
		pTalent->Update(2.0f);	// ĳ���ýð����� ���� ���
		pTalent->ActTalent(m_pPlayer->GetUID());	
		CHECK_EQUAL(TALENT_PHASE_PREPARE, pTalent->GetPhase());	
		pTalent->Update(3.0f);	// ĳ���ýð� ���� ���
		CHECK_EQUAL(TALENT_PHASE_ACT, pTalent->GetPhase());	
	}

	TEST_FIXTURE(FTalent, ShouldBeSamePlaceAtActTalent)	// ����ڰ� ACT ��û�Ҷ��� ��ġ�� USE ��û�Ҷ��� ��ġ�� ���ƾ��Ѵ�.
	{
		GTalentInfo* pTalentInfo = test.talent.NewTalentInfo();
		pTalentInfo->m_fCastingTime = 5.0f;
		pTalentInfo->m_fDurationTime = 10.0f;
		pTalentInfo->Cooking();

		test.talent.UseTalent(m_pPlayer, pTalentInfo, m_pPlayer->GetUID());
		GTalent* pTalent = m_pPlayer->GetModuleCombat()->GetTalent();
		TVALID(pTalent);
		pTalent->Update(5.0f);	// ĳ���ýð���ŭ ���
		m_pPlayer->SetPos(vec3(123,456,789));	// ��ġ�� ������
		pTalent->ActTalent(m_pPlayer->GetUID());	
		CHECK_EQUAL(TALENT_PHASE_CANCELED, pTalent->GetPhase());
	}

	TEST_FIXTURE(FTalent, ActTalentTargetCheck)	// ����ڰ� ACT ��û�Ҷ� ���� Ÿ������ ����� ����Ǿ����� Ȯ��
	{
		GEntityPlayer* pTargetPlayer = NewEntityPlayer(m_pField);

		GTalentInfo* pTalentInfo = test.talent.NewTalentInfo();
		pTalentInfo->m_fCastingTime = 5.0f;
		pTalentInfo->m_fDurationTime = 10.0f;
		pTalentInfo->Cooking();

		test.talent.UseTalent(m_pPlayer, pTalentInfo, m_pPlayer->GetUID());
		GTalent* pTalent = m_pPlayer->GetModuleCombat()->GetTalent();
		TVALID(pTalent);
		CHECK_EQUAL(m_pPlayer->GetUID(), pTalent->GetTarget().uidTarget);
		pTalent->Update(5.0f);	// ĳ���ýð���ŭ ���
		pTalent->ActTalent(pTargetPlayer->GetUID());	
		CHECK_EQUAL(pTargetPlayer->GetUID(), pTalent->GetTarget().uidTarget);	// ����� ActTalent�� ��û ������� �ٲ�
	}

	TEST_FIXTURE(FTalent, CastingUsingRealPacket)	// ���� ��Ŷ���� �׽�Ʈ
	{
		GEntityPlayer* pTargetPlayer = NewEntityPlayer(m_pField);

		GTalentInfo* pTalentInfo = test.talent.NewTalentInfo(777);
		pTalentInfo->m_fCastingTime = 5.0f;
		pTalentInfo->m_fDurationTime = 10.0f;
		pTalentInfo->Cooking();

		// UseTalent
		m_pLinkTester->OnRecv(MC_ACTION_USE_TALENT_REQ,		3, NEW_INT(777), NEW_SVEC(m_pPlayer->GetDir()), NEW_VEC(m_pPlayer->GetPos()));

		GTalent* pTalent = m_pPlayer->GetModuleCombat()->GetTalent();
		TVALID(pTalent);
		CHECK_EQUAL(TALENT_PHASE_PREPARE, pTalent->GetPhase());
		pTalent->Update(5.0f);	// ĳ���ýð���ŭ ���
		CHECK_EQUAL(TALENT_PHASE_PREPARE, pTalent->GetPhase());

		// ActTalent
		TD_TALENT_TARGET_DETAIL infoTarget;
		infoTarget.uidTarget = pTargetPlayer->GetUID();
		m_pLinkTester->OnRecv(MC_ACTION_ACT_TALENT_WITH_HITCAPSULE_REQ,		2, NEW_SVEC(m_pPlayer->GetDir()), NEW_SINGLE_BLOB(&infoTarget, sizeof(TD_TALENT_TARGET_DETAIL)));

		CHECK_EQUAL(pTargetPlayer->GetUID(), pTalent->GetTarget().uidTarget);	// ����� ActTalent�� ��û ������� �ٲ�
		CHECK_EQUAL(TALENT_PHASE_ACT, pTalent->GetPhase());	
	}

	TEST_FIXTURE(FTalent, NPCAdvancePhaseWithoutCastingtime)	// NPC�� ACT���� prepare����� �ѱ�
	{
		GEntityNPC* pNPCCaster = GUTHelper_NPC::SpawnNPC(m_pField);

		GTalentInfo* pTalentInfo = test.talent.NewTalentInfo();
		pTalentInfo->m_fCastingTime = 5.0f;
		pTalentInfo->m_fDurationTime = 10.0f;
		pTalentInfo->Cooking();

		test.talent.UseTalent(pNPCCaster, pTalentInfo, pNPCCaster->GetUID());
		GTalent* pTalent = pNPCCaster->GetModuleCombat()->GetTalent();
		TVALID(pTalent);
		CHECK_EQUAL(TALENT_PHASE_PREPARE, pTalent->GetPhase());
		pTalent->Update(5.0f);	// ĳ���ýð���ŭ ���
		CHECK_EQUAL(TALENT_PHASE_ACT, pTalent->GetPhase());
	}

	TEST_FIXTURE(FTalent, ActMethodCanUseOnlyPreparePhase)	// ACT �޽��� prepare��������� �� �� ����
	{
		GTalentInfo* pTalentInfo = test.talent.NewTalentInfo();
		pTalentInfo->m_fCastingTime = 5.0f;
		pTalentInfo->m_fDurationTime = 10.0f;
		pTalentInfo->Cooking();

		test.talent.UseTalent(m_pPlayer, pTalentInfo, m_pPlayer->GetUID());
		GTalent* pTalent = m_pPlayer->GetModuleCombat()->GetTalent();
		TVALID(pTalent);
		pTalent->Update(5.0f);	// ĳ���ýð���ŭ ���
		pTalent->ActTalent(m_pPlayer->GetUID());	
		CHECK_EQUAL(TALENT_PHASE_ACT, pTalent->GetPhase());	
		pTalent->Update(10.0f);	// ����ð����� ���
		pTalent->UpdateExpired(1.0f);
		CHECK_EQUAL(TALENT_PHASE_FINISH, pTalent->GetPhase());	

		// ����� �ŷ�Ʈ���� ACT���ѵ� �۵����� �ʾƾ���
		pTalent->ActTalent(m_pPlayer->GetUID());	
		CHECK_EQUAL(TALENT_PHASE_FINISH, pTalent->GetPhase());	
	}

	TEST_FIXTURE(FTalent, ActTime)	
	{
		GTalentInfo* pTalentInfo = test.talent.NewTalentInfo();
		pTalentInfo->m_fCastingTime = 5.0f;
		pTalentInfo->m_fDurationTime = 10.0f;
		pTalentInfo->Cooking();

		test.talent.UseTalent(m_pPlayer, pTalentInfo, m_pPlayer->GetUID());
		GTalent* pTalent = m_pPlayer->GetModuleCombat()->GetTalent();
		TVALID(pTalent);
		CHECK_EQUAL(0.0f, pTalent->GetActElapsedTime());	// ���۽� 0���� ����
		pTalent->Update(5.0f);	// ĳ���ýð���ŭ ���
		CHECK_EQUAL(0.0f, pTalent->GetActElapsedTime());	// ��Ʈ ������� �Ѿ�� ����
		pTalent->ActTalent(m_pPlayer->GetUID());		
		CHECK_EQUAL(0.0f, pTalent->GetActElapsedTime());	// ��Ʈ ������� �Ѿ�� ����
		CHECK_EQUAL(TALENT_PHASE_ACT, pTalent->GetPhase());	
		pTalent->Update(3.0f);
		CHECK_EQUAL(3.0f, pTalent->GetActElapsedTime());
		pTalent->Update(2.0f);
		CHECK_EQUAL(5.0f, pTalent->GetActElapsedTime());
	}

	TEST_FIXTURE(FTalent, NotPayCostWhenSelfCanceled)		// �ŷ�Ʈ�� ĳ�����߿� ������ ��ҵǸ� �ŷ�Ʈ ����� ���Ҿ���
	{
		GTalentInfo* pTalentInfo = test.talent.NewTalentInfo();
		pTalentInfo->m_fCastingTime = 5.0f;
		pTalentInfo->m_fDurationTime = 10.0f;
		pTalentInfo->m_nENCost = 10; // �������� 10��ŭ �Ҹ�
		pTalentInfo->Cooking();

		m_pPlayer->SetEN(10);
		CHECK_EQUAL(10, m_pPlayer->GetEN());

		test.talent.UseTalent(m_pPlayer, pTalentInfo, m_pPlayer->GetUID());
		GTalent* pTalent = m_pPlayer->GetModuleCombat()->GetTalent();
		TVALID(pTalent);
		CHECK_EQUAL(0.0f, pTalent->GetActElapsedTime());	// ���۽� 0���� ����
		pTalent->Update(1.0f);	// ĳ���ýð��� ���ڸ��� ���
		CHECK_EQUAL(10, m_pPlayer->GetEN());

		// ������ ���
		m_pPlayer->doCancelTalentForce();

		// �ŷ�Ʈ ��ҵƴ��� Ȯ��
		CHECK_EQUAL(TALENT_PHASE_CANCELED, m_pPlayer->GetModuleCombat()->GetTalent()->GetPhase());
		CHECK_EQUAL(10, m_pPlayer->GetEN());	// �ŷ�Ʈ �ߵ� ��� �Һ� �ȵǰ� ����
	}

	TEST_FIXTURE(FTalent, PayCostWhenHitCanceled)		// �ŷ�Ʈ�� ĳ�����߿� �ǰ����� ��ҵǸ� �ŷ�Ʈ ����� ������
	{
		GTalentInfo* pTalentInfo = test.talent.NewTalentInfo();
		pTalentInfo->m_fCastingTime = 5.0f;
		pTalentInfo->m_fDurationTime = 10.0f;
		pTalentInfo->m_nENCost = 10; // �������� 10��ŭ �Ҹ�
		pTalentInfo->Cooking();

		m_pPlayer->SetEN(10);
		CHECK_EQUAL(10, m_pPlayer->GetEN());

		test.talent.UseTalent(m_pPlayer, pTalentInfo, m_pPlayer->GetUID());
		GTalent* pTalent = m_pPlayer->GetModuleCombat()->GetTalent();
		TVALID(pTalent);
		CHECK_EQUAL(0.0f, pTalent->GetActElapsedTime());	// ���۽� 0���� ����
		pTalent->Update(1.0f);	// ĳ���ýð��� ���ڸ��� ���
		CHECK_EQUAL(10, m_pPlayer->GetEN());

		// ���� ����
		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField);
		GTalentInfo* pAtkTalent = test.talent.MakeDamageTalent(1);
		pAtkTalent->m_MotionFactorGroup.Adjust(MF_BEATEN, 100, 1000);	// ��ư�� �ɸ�
		test.talent.UseTalent(pAttacker, pAtkTalent, m_pPlayer->GetUID());
		m_pField->Update(.0f);

		// �ŷ�Ʈ ��ҵƴ��� Ȯ��
		if (NULL != m_pPlayer->GetModuleCombat()->GetTalent())
		{
			CHECK_EQUAL(TALENT_PHASE_CANCELED, m_pPlayer->GetModuleCombat()->GetTalent()->GetPhase());
		}
		CHECK_EQUAL(0, m_pPlayer->GetEN());	// �ŷ�Ʈ �ߵ� ��� �Һ� �ȵǰ� ����
	}
}