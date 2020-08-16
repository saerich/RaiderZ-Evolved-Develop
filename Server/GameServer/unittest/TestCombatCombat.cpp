#include "stdafx.h"
#include "GUTHelper.h"
#include "MockField.h"
#include "GMessageID.h"
#include "GModuleAI.h"
#include "GJob_Broker.h"
#include "GJob_Talent.h"
#include "GTestForward.h"
#include "GJobMgr.h"
#include "GBehaviorStateMachine.h"
#include "GBehaviorState.h"
#include "GHateTable.h"
#include "FBaseMockLink.h"

SUITE(CombatCombat)
{
	struct FCombatCombat : public FBaseMockLink
	{
		FCombatCombat()
		{
			m_pField = GUTHelper_Field::DefaultMockField();
			m_pNPC = m_Helper.NewEntityNPC(m_pField);
			m_pPlayer = GUTHelper::NewEntityPlayer(m_pField);
		}

		virtual ~FCombatCombat()
		{
			m_pField->Destroy();
		}

		GUTHelper		m_Helper;
		MockField*		m_pField;
		GEntityNPC*		m_pNPC;
		GEntityPlayer*	m_pPlayer;
	};

	// job�� �� ����������, combatcombat ������Ʈ���� �ٸ� ������Ʈ�� �ٲ��� �ʴ´�.
	// ex : NPC�� PC�� �ŷ�Ʈ�� �����ϴ��� PC�� �׾ ����ϴ� �ŷ�Ʈ�� ���� �� ����ϰ� �¸���� ���Ѵ�.
	TEST_FIXTURE(FCombatCombat, NoChangeStateWhenExistJob)
	{
		GTalentInfo* pTalentInfo = m_Helper.NewTalentInfo();
		pTalentInfo->m_nSkillType = ST_MELEE;
		MUID uidEnemy = m_pPlayer->GetUID();
		GMessage message(GMSG_AI_EVT_STARTCOMBAT, &uidEnemy);

		// ���� ����
		m_pNPC->GetModuleAI()->Message(message);
		GBehaviorStateMachine* pFSM = m_pNPC->GetBehaviorFSM();
		CHECK_EQUAL(AI_COMBAT_COMBAT, pFSM->GetCurrentCombatStateID());

		// job�� �� ������ �ʾƼ� combatcomabt ������Ʈ ����
		GJob* pJob =  new GJob_UseTalent(m_pNPC->GetModuleAI(), pTalentInfo, m_pNPC->GetUID());

		GJobMgr* pJobMgr = m_pNPC->GetJobMgr();		

		pJobMgr->Push(pJob);
		m_pNPC->GetModuleAI()->Update(0.0f);

		// ���� ����
		m_pNPC->GetHateTable().Reset();

		// job�� �� ������ �ʾƼ� combatcomabt ������Ʈ ����
		m_pNPC->GetModuleAI()->Update(0.1f);		
		CHECK_EQUAL(AI_COMBAT_COMBAT, pFSM->GetCurrentCombatStateID());

		// job�� �� ������ ������Ʈ ����
		pJobMgr->Clear();
		m_pNPC->GetModuleAI()->Update(0.0f);
		m_pNPC->GetModuleAI()->Update(0.0f);
		CHECK_EQUAL(AI_COMBAT_VICTORY, pFSM->GetCurrentCombatStateID());
	}

	TEST_FIXTURE(FCombatCombat, MissWhenAttackInvincibleTarget)
	{
		GTalentInfo* pTalentInfo = test.talent.MakeDamageTalent(1);
		pTalentInfo->m_nSkillType = ST_MELEE;
		pTalentInfo->m_fDurationTime = 10.0f;
		pTalentInfo->m_bUseInvincibleTime = true;
		pTalentInfo->m_fInvincibleTimeStart = 0.0f;
		pTalentInfo->m_fInvincibleTimeEnd = 5.0f;	// 5�ʵ��� ����
		
		test.talent.UseTalent(m_pNPC, pTalentInfo, m_pPlayer->GetUID());	// �ŷ�Ʈ ����ؼ� ������ �� (5��)

		MockLink* m_pLinkTester = test.network.NewLink(m_pPlayer);
		m_pLinkTester->AddIgnoreID(MC_ACTION_USE_TALENT);
		m_pLinkTester->AddIgnoreID(MC_NPCINTERACTION_ICON);
		m_pLinkTester->AddIgnoreID(MC_CHAR_ENEMY_INFO);
		m_pLinkTester->AddIgnoreID(MC_CHAR_ADD_ENEMYNPC);
				
		test.talent.UseTalent(m_pPlayer, pTalentInfo, m_pNPC->GetUID());
		CHECK_EQUAL(m_pNPC->GetHP(), m_pNPC->GetHP());	// ���� �ȹ���

		// ��Ŷ üũ
		const minet::MCommand& Command = m_pLinkTester->GetCommand(0);
		CHECK_EQUAL(Command.GetID(), MC_ACTION_TALENT_HIT);
		if (Command.GetParameterCount() < 1)						 { CHECK(false); return; }
		MCommandParameter* pParam = Command.GetParameter(0);
		if(pParam->GetType()!=MPT_SINGLE_BLOB)								{ CHECK(false); return; }
		TD_TALENT_HIT* pTD = (TD_TALENT_HIT*)pParam->GetPointer();
		CHECK(CheckBitSet(pTD->nFlags,CTR_IMMUNE));

		test.network.ClearLinks();
	}

	TEST_FIXTURE(FCombatCombat, CheckCombatRadius)
	{
		GTalentInfo* pTalentInfo = m_Helper.NewTalentInfo();
		pTalentInfo->m_nSkillType = ST_MELEE;
		MUID uidEnemy = m_pPlayer->GetUID();
		GMessage message(GMSG_AI_EVT_STARTCOMBAT, &uidEnemy);

		// ���� ����
		m_pNPC->GetModuleAI()->Message(message);
		GBehaviorStateMachine* pFSM = m_pNPC->GetBehaviorFSM();
		CHECK_EQUAL(AI_BEHAVIOR_COMBAT, pFSM->GetCurrentStateID());

		// ���� ���� �Ѿ
		CHECK_EQUAL(GConst::NPC_DEFAULT_COMBAT_RADIUS, m_pNPC->GetCombatRadius());
		m_pNPC->SetPos(m_pNPC->GetPos() + vec3(GConst::NPC_DEFAULT_COMBAT_RADIUS+1, 0, 0));
		pFSM->Update(0.0f);

		// ���� ���� Ȯ��
		CHECK_EQUAL(AI_BEHAVIOR_RETURN, pFSM->GetCurrentStateID());
	}

	TEST_FIXTURE(FCombatCombat, InvincibleWhenReturnState)
	{
		GTalentInfo* pTalentInfo = m_Helper.NewTalentInfo();
		pTalentInfo->m_nSkillType = ST_MELEE;
		MUID uidEnemy = m_pPlayer->GetUID();
		GMessage message(GMSG_AI_EVT_STARTCOMBAT, &uidEnemy);

		// ���� ����
		m_pNPC->GetModuleAI()->Message(message);
		GBehaviorStateMachine* pFSM = m_pNPC->GetBehaviorFSM();
		CHECK_EQUAL(AI_BEHAVIOR_COMBAT, pFSM->GetCurrentStateID());

		// ���� ���� Ȯ��
		CHECK_EQUAL(false, m_pNPC->IsNowInvincibility());
		m_pNPC->SetPos(m_pNPC->GetPos() + vec3(GConst::NPC_DEFAULT_COMBAT_RADIUS+1, 0, 0));
		pFSM->Update(0.0f);
		CHECK_EQUAL(AI_BEHAVIOR_RETURN, pFSM->GetCurrentStateID());
		CHECK_EQUAL(true, m_pNPC->IsNowInvincibility());
		GJobMgr* pJobMgr = m_pNPC->GetJobMgr();		
		pJobMgr->Clear();
		m_pNPC->GetModuleAI()->Update(0.0f);
		m_pNPC->GetModuleAI()->Update(0.0f);
		CHECK_EQUAL(AI_BEHAVIOR_PEACE, pFSM->GetCurrentStateID());
		CHECK_EQUAL(false, m_pNPC->IsNowInvincibility());

	}

}