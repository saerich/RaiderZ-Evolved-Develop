#include "stdafx.h"
#include "GUTHelper.h"
#include "GNPCInfo.h"
#include "MockField.h"
#include "MockLink.h"
#include "GTestWrapper.h"
#include "GModuleAI.h"
#include "GModuleCombat.h"
#include "GCommandTable.h"
#include "GTalent.h"
#include "GTalentInfoMgr.h"
#include "GFieldMgr.h"
#include "GEntityNPC.h"
#include "GHateTable.h"
#include "FBaseMockLink.h"
#include "GUTHelper_Field.h"

SUITE(TalentGrapple)
{

struct FTalentGrapple : public FBaseMockLink
{
	FTalentGrapple()
	{
		m_pField = GUTHelper_Field::DefaultMockField();

		m_NPCInfo.nID = 1;

		vec3 pos = vec3(1000, 1000, 0);
		vec3 dir = vec3(0,1,0);

		m_pNPC = m_pField->SpawnTestNPC(&m_NPCInfo, pos, dir);
		m_pPlayer = GUTHelper::NewEntityPlayer(m_pField, pos);
		m_pPlayerLink = NewLink(m_pPlayer);

		m_pPlayerLink->AddIgnoreID(MC_CHAR_ADD_ENEMYNPC);
		m_pPlayerLink->AddIgnoreID(MC_CHAR_DEL_ENEMYNPC);
		m_pPlayerLink->AddIgnoreID(MC_CHAR_ENEMY_INFO);

		m_pGrappleTalentInfo = m_Helper.NewTalentInfo();
		m_pGrappleTalentInfo->m_nSkillType = ST_EXTRA_ACTIVE;
		m_pGrappleTalentInfo->m_nExtraActive = TEAT_NPC_ONLY_GRAPPLE;
		m_pGrappleTalentInfo->m_nTiming = TC_HIT_TALENT;
		m_pGrappleTalentInfo->m_EffectInfo.m_nRelation = CSEffectInfo::RELATION_ENEMY;

		// ���� Ÿ���� AREA
		m_pGrappleTalentInfo->m_EffectInfo.m_fRadius = 1.0f;
		m_pGrappleTalentInfo->m_fDurationTime = 2.0f;		// aitime 2��
		m_pGrappleTalentInfo->m_fExtraPhaseTime = 2.0f;		// ���� �� 2�� �ɸ�
		m_pGrappleTalentInfo->m_nForceMF = MF_GRAPPLED;
		m_pGrappleTalentInfo->m_nForceMFWeight = 2000;

		m_pGrappleTalentInfo->Cooking();

		m_Helper.SetTalentDamage(m_pGrappleTalentInfo, 1);		// ������ 1�� ����
		m_Helper.AddSimpleHitInfo(m_pGrappleTalentInfo, 0.0f);	// �ٷ� �����ϴ� �ŷ�Ʈ��

		m_pNPC->SetThinkable(false);
		m_Helper.SetEntityPlayerStatus(m_pPlayer, 100, 100, 100);

		
	}
	~FTalentGrapple()
	{
		m_pField->Destroy();
	}

	GUTHelper		m_Helper;
	MockField*		m_pField;
	GEntityNPC*		m_pNPC;
	GEntityPlayer*	m_pPlayer;
	MockLink*		m_pPlayerLink;

	GNPCInfo		m_NPCInfo;
	GTalentInfo*	m_pGrappleTalentInfo;

	GTestMgrWrapper<GTalentInfoMgr>	m_TalentInfoMgrWrapper;
};

TEST_FIXTURE(FTalentGrapple, TestGrappledTalentCooking)
{
	CHECK_EQUAL(m_pGrappleTalentInfo->m_nForceMF, MF_GRAPPLED);
	CHECK(m_pGrappleTalentInfo->m_nForceMFWeight >= 2000);	// m_fExtraPhaseTime�� 2���̹Ƿ�
}

TEST_FIXTURE(FTalentGrapple, TestSimpleTalentGrapple)
{
	TALENT_TARGET_INFO target_info;
	target_info.uidTarget = m_pPlayer->GetUID();

	// npc�� ��� �ŷ�Ʈ ���
	m_pNPC->GetHateTable().AddPoint_FoundEnemy(m_pPlayer);

	m_pNPC->GetModuleCombat()->UseTalent(m_pGrappleTalentInfo, target_info, false);
	m_pField->Update(0.1f);

	// �����Ǿ����Ƿ� ������ 1 ��Ҵ�.
	CHECK_EQUAL(99, m_pPlayer->GetHP());
	CHECK_EQUAL(MC_ACTION_TALENT_HIT, m_pPlayerLink->GetCommand(2).GetID());

	// �����Ǹ� NPC �ŷ�Ʈ�� ��� �ŷ�Ʈ�̹Ƿ� EXTRA Phase�� �����Ѵ�.
	CHECK(m_pNPC->GetModuleCombat()->GetTalent() != NULL);
	if (m_pNPC->GetModuleCombat()->GetTalent())
	{
		CHECK_EQUAL(m_pNPC->GetModuleCombat()->GetTalent()->GetPhase() ,TALENT_PHASE_EXTRA);
	}

	CHECK_EQUAL(m_pPlayer->GetCurrentMotionFactor() , MF_GRAPPLED);

	m_pField->Update(2.0f);
	m_pField->Update(2.0f);

	CHECK_EQUAL(m_pPlayer->GetCurrentMotionFactor() , MF_NONE);

	// Link�� ���� Ŀ�ǵ�
	//	[0:SEND] (1443)MC_ACTION_USE_TALENT
	//	[1:SEND] (1449)MC_ACTION_EXTRA_ACT_TALENT
	//	[2:SEND] (1460)MC_ACTION_TALENT_HIT
}


TEST_FIXTURE(FTalentGrapple, TestTalentGrappleHitFailed)
{
	TALENT_TARGET_INFO target_info;
	target_info.uidTarget = m_pPlayer->GetUID();

	// npc�� ��� �ŷ�Ʈ ���
	m_pNPC->GetHateTable().AddPoint_FoundEnemy(m_pPlayer);

	vec3 player_pos = vec3(1000, 10000, 0);
	m_pPlayer->SetPos(player_pos);

	m_pNPC->GetModuleCombat()->UseTalent(m_pGrappleTalentInfo, target_info, false);
	m_pField->Update(0.1f);

	// �ʹ� �ָ��־� �������� �ʾҴ�.

	// ������ �ȵǾ����Ƿ� Act Phase�̴�.
	CHECK(m_pNPC->GetModuleCombat()->GetTalent() != NULL);
	if (m_pNPC->GetModuleCombat()->GetTalent())
	{
		CHECK_EQUAL(m_pNPC->GetModuleCombat()->GetTalent()->GetPhase() ,TALENT_PHASE_ACT);
	}

	CHECK_EQUAL(m_pPlayer->GetCurrentMotionFactor() , MF_NONE);

	m_pField->Update(2.0f);
	m_pField->Update(2.0f);


	// �߰��� NPC�� ���ִ� ���
	GEntityNPC* pNewNPC = m_pField->SpawnTestNPC(&m_NPCInfo, m_pNPC->GetPos(), vec3(0,-1,0));

	m_pNPC->GetModuleCombat()->UseTalent(m_pGrappleTalentInfo, target_info, false);
	m_pField->Update(0.1f);

	// ������ ������ �ȵǾ����Ƿ� Act Phase�̴�.
	CHECK(m_pNPC->GetModuleCombat()->GetTalent() != NULL);
	if (m_pNPC->GetModuleCombat()->GetTalent())
	{
		CHECK_EQUAL(m_pNPC->GetModuleCombat()->GetTalent()->GetPhase() ,TALENT_PHASE_ACT);
	}

}



TEST_FIXTURE(FTalentGrapple, TestTalentGrappleHitFailed2)
{
	TALENT_TARGET_INFO target_info;
	target_info.uidTarget = m_pPlayer->GetUID();

	// npc�� ��� �ŷ�Ʈ ���
	m_pNPC->GetHateTable().AddPoint_FoundEnemy(m_pPlayer);

	vec3 player_pos = m_pNPC->GetPos();
	m_pPlayer->SetPos(player_pos);

	m_pPlayer->doSwimming();

	m_pNPC->GetModuleCombat()->UseTalent(m_pGrappleTalentInfo, target_info, false);
	m_pField->Update(0.1f);
	
	// �������̸� �������� �ʴ´�. -----------------------
	CHECK(m_pNPC->GetModuleCombat()->GetTalent() != NULL);
	if (m_pNPC->GetModuleCombat()->GetTalent())
	{
		CHECK_EQUAL(m_pNPC->GetModuleCombat()->GetTalent()->GetPhase() ,TALENT_PHASE_ACT);
	}

	// ����
	m_pField->Update(2.0f);
	m_pField->Update(2.0f);
	m_pPlayer->doSwimmingRelease();

	MOTION_FACTOR_GROUP mf_group;
	GMFApplyArgs args;
	args.pMotionFactorGroup = &mf_group;
	args.pOpponent = m_pNPC;
	args.nForceMF = MF_STUN;
	args.nForceMFWeight = 2000;

	GMFApplyReturnValue mfRet = m_pPlayer->ApplyMotionFactor(args);

	CHECK_EQUAL(m_pPlayer->GetCurrentMotionFactor(), MF_STUN);

	m_pNPC->GetModuleCombat()->UseTalent(m_pGrappleTalentInfo, target_info, false);
	m_pField->Update(0.1f);

	// �������̸� �������� �ʴ´�. -----------------------
	CHECK(m_pNPC->GetModuleCombat()->GetTalent() != NULL);
	if (m_pNPC->GetModuleCombat()->GetTalent())
	{
		CHECK_EQUAL(m_pNPC->GetModuleCombat()->GetTalent()->GetPhase() ,TALENT_PHASE_ACT);
	}

}




}
