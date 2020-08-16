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
#include "Gconst.h"
#include "GTestForward.h"
#include "GNPCSwallow.h"

SUITE(TalentSwallow)
{

struct FTalentSwallow : public FBaseMockLink
{
	FTalentSwallow()
	{
		m_pField = GUTHelper_Field::DefaultMockField();

		m_NPCInfo.nID = 1;
		m_NPCInfo.nEatOwnerBuffID = 560;
		m_NPCInfo.nEatTargetBuffID = 561;
		m_NPCInfo.fEatPeriod = 2.0f;
		m_NPCInfo.nAA = NAAT_ALWAYS;
		
		vec3 pos = vec3(1000, 1000, 0);
		vec3 dir = vec3(0,1,0);

		m_pNPC = m_pField->SpawnTestNPC(&m_NPCInfo, pos, dir);
		m_pPlayer = GUTHelper::NewEntityPlayer(m_pField, pos);
		m_pPlayerLink = NewLink(m_pPlayer);

		m_pPlayerLink->AddIgnoreID(MC_CHAR_ADD_ENEMYNPC);
		m_pPlayerLink->AddIgnoreID(MC_CHAR_DEL_ENEMYNPC);
		m_pPlayerLink->AddIgnoreID(MC_BUFF_GAIN);
		m_pPlayerLink->AddIgnoreID(MC_BUFF_LOST);
		m_pPlayerLink->AddIgnoreID(MC_CHAR_ENEMY_INFO);
		
		m_pSwallowedTalentInfo = m_Helper.NewTalentInfo();
		m_pSwallowedTalentInfo->m_nSkillType = ST_EXTRA_ACTIVE;
		m_pSwallowedTalentInfo->m_nExtraActive = TEAT_NPC_ONLY_EAT;
		m_pSwallowedTalentInfo->m_nTiming = TC_HIT_TALENT;
		m_pSwallowedTalentInfo->m_EffectInfo.m_nRelation = CSEffectInfo::RELATION_ENEMY;
		
		// ���� Ÿ���� AREA
		m_pSwallowedTalentInfo->m_EffectInfo.m_fRadius = 1.0f;
		m_pSwallowedTalentInfo->m_fDurationTime = 2.0f;		// aitime 2��
		m_pSwallowedTalentInfo->m_fExtraPhaseTime = 2.0f;	// �Դ� �ִϸ��̼ǽð�
		m_pSwallowedTalentInfo->m_fExtraPhaseTime2 = 3.0f;	// ��� �ִϸ��̼ǽð�
		m_pSwallowedTalentInfo->m_fExtraPhaseTime3 = 4.0f;	// ��� �ִϸ��̼ǽð� (���ݹ޾� ������ ���)
		m_pSwallowedTalentInfo->m_nExtraActiveParam1 = 10;  // �Դ� �ð�
		m_pSwallowedTalentInfo->m_nForceMF = MF_SWALLOWED;
		m_pSwallowedTalentInfo->m_nForceMFWeight = 2000;

		m_pSwallowedTalentInfo->Cooking();

		m_Helper.SetTalentDamage(m_pSwallowedTalentInfo, 1);		// ������ 1�� ����
		m_Helper.AddSimpleHitInfo(m_pSwallowedTalentInfo, 0.0f);	// �ٷ� �����ϴ� �ŷ�Ʈ��

		m_pNPC->SetThinkable(false);
		m_Helper.SetEntityPlayerStatus(m_pPlayer, 100, 100, 100);

		
	}
	~FTalentSwallow()
	{
		m_pField->Destroy();
	}

	GUTHelper		m_Helper;
	MockField*		m_pField;
	GEntityNPC*		m_pNPC;
	GEntityPlayer*	m_pPlayer;
	MockLink*		m_pPlayerLink;

	GNPCInfo		m_NPCInfo;
	GTalentInfo*	m_pSwallowedTalentInfo;

	GTestMgrWrapper<GTalentInfoMgr>	m_TalentInfoMgrWrapper;
	DECLWRAPPER_BuffMgr;
};

TEST_FIXTURE(FTalentSwallow, TestSwallowedTalentCooking)
{
	CHECK_EQUAL(m_pSwallowedTalentInfo->m_nForceMF, MF_SWALLOWED);
	CHECK(m_pSwallowedTalentInfo->m_nForceMFWeight >= 2000);	// m_fExtraPhaseTime�� 2���̹Ƿ�
}

TEST_FIXTURE(FTalentSwallow, TestSimpleSwallowedTalent)
{
	test.buff.NewBuffInfo(m_NPCInfo.nEatOwnerBuffID);
	test.buff.NewBuffInfo(m_NPCInfo.nEatTargetBuffID);

	TALENT_TARGET_INFO target_info;
	target_info.uidTarget = m_pPlayer->GetUID();

	// npc�� ��� �ŷ�Ʈ ���
	m_pNPC->GetHateTable().AddPoint_FoundEnemy(m_pPlayer);

	CHECK_EQUAL(false, m_pNPC->HasBuff(m_NPCInfo.nEatOwnerBuffID));
	CHECK_EQUAL(false, m_pPlayer->HasBuff(m_NPCInfo.nEatTargetBuffID));

	m_pNPC->GetModuleCombat()->UseTalent(m_pSwallowedTalentInfo, target_info, false);
	m_pField->Update(0.1f);

	// �����Ǿ����Ƿ� ������ 1 ��Ҵ�.
	CHECK_EQUAL(m_pPlayer->GetHP(), 99);
	CHECK_EQUAL(m_pPlayerLink->GetCommand(2).GetID(), MC_ACTION_TALENT_HIT);

	// �����Ǹ� NPC �ŷ�Ʈ�� ��� �ŷ�Ʈ�̹Ƿ� EXTRA Phase�� �����Ѵ�.
	CHECK(m_pNPC->GetModuleCombat()->GetTalent() != NULL);
	if (m_pNPC->GetModuleCombat()->GetTalent())
	{
		CHECK_EQUAL(m_pNPC->GetModuleCombat()->GetTalent()->GetPhase() ,TALENT_PHASE_EXTRA);
	}

	CHECK_EQUAL(m_pPlayer->GetCurrentMotionFactor() , MF_SWALLOWED);
	CHECK_EQUAL(true, m_pNPC->HasBuff(m_NPCInfo.nEatOwnerBuffID));
	CHECK_EQUAL(true, m_pPlayer->HasBuff(m_NPCInfo.nEatTargetBuffID));
	CHECK_EQUAL(false, m_pNPC->GetNPCSwallow().IsSwallowable());

	m_pField->Update(m_pSwallowedTalentInfo->m_fExtraPhaseTime+(float)m_pSwallowedTalentInfo->m_nExtraActiveParam1);
	CHECK_EQUAL(m_pNPC->GetModuleCombat()->GetTalent()->GetPhase() ,TALENT_PHASE_EXTRA2);
	CHECK_EQUAL(m_pPlayerLink->GetCommand(3).GetID(), MC_NPC_SPEWUP);
	m_pField->Update(m_pSwallowedTalentInfo->m_fExtraPhaseTime2);
		
	CHECK_EQUAL(m_pPlayer->GetCurrentMotionFactor() , MF_NONE);
	CHECK_EQUAL(false, m_pNPC->HasBuff(m_NPCInfo.nEatOwnerBuffID));
	CHECK_EQUAL(false, m_pPlayer->HasBuff(m_NPCInfo.nEatTargetBuffID));
	CHECK_EQUAL(true, m_pNPC->GetNPCSwallow().IsSwallowable());

	// Link�� ���� Ŀ�ǵ�
	//	[0:SEND] (1443)MC_ACTION_USE_TALENT
	//	[1:SEND] (1449)MC_ACTION_EXTRA_ACT_TALENT
	//	[2:SEND] (1460)MC_ACTION_TALENT_HIT
	//	[3:SEND] (1617)MC_NPC_SPEWUP
}

TEST_FIXTURE(FTalentSwallow, SpewupDueAttacked)
{
	test.buff.NewBuffInfo(m_NPCInfo.nEatOwnerBuffID);
	test.buff.NewBuffInfo(m_NPCInfo.nEatTargetBuffID);

	TALENT_TARGET_INFO target_info;
	target_info.uidTarget = m_pPlayer->GetUID();

	// npc�� ��� �ŷ�Ʈ ���
	m_pNPC->GetHateTable().AddPoint_FoundEnemy(m_pPlayer);

	CHECK_EQUAL(false, m_pNPC->HasBuff(m_NPCInfo.nEatOwnerBuffID));
	CHECK_EQUAL(false, m_pPlayer->HasBuff(m_NPCInfo.nEatTargetBuffID));

	m_pNPC->GetModuleCombat()->UseTalent(m_pSwallowedTalentInfo, target_info, false);
	m_pField->Update(0.1f);

	// �����Ǿ����Ƿ� ������ 1 ��Ҵ�.
	CHECK_EQUAL(m_pPlayer->GetHP(), 99);
	CHECK_EQUAL(m_pPlayerLink->GetCommand(2).GetID(), MC_ACTION_TALENT_HIT);

	// �����Ǹ� NPC �ŷ�Ʈ�� ��� �ŷ�Ʈ�̹Ƿ� EXTRA Phase�� �����Ѵ�.
	CHECK(m_pNPC->GetModuleCombat()->GetTalent() != NULL);
	if (m_pNPC->GetModuleCombat()->GetTalent())
	{
		CHECK_EQUAL(m_pNPC->GetModuleCombat()->GetTalent()->GetPhase() ,TALENT_PHASE_EXTRA);
	}

	CHECK_EQUAL(m_pPlayer->GetCurrentMotionFactor() , MF_SWALLOWED);
	CHECK_EQUAL(true, m_pNPC->HasBuff(m_NPCInfo.nEatOwnerBuffID));
	CHECK_EQUAL(true, m_pPlayer->HasBuff(m_NPCInfo.nEatTargetBuffID));
	CHECK_EQUAL(false, m_pNPC->GetNPCSwallow().IsSwallowable());

	m_pNPC->GetNPCSwallow().Spewup(true);	// ���ݹ޾� ��

	m_pNPC->GetModuleCombat()->Update(0.0f);
	CHECK_EQUAL(m_pNPC->GetModuleCombat()->GetTalent()->GetPhase() ,TALENT_PHASE_EXTRA3);
	CHECK_EQUAL(m_pPlayerLink->GetCommand(3).GetID(), MC_NPC_SPEWUP_INTERRUPT);
	m_pField->Update(m_pSwallowedTalentInfo->m_fExtraPhaseTime2);
	CHECK_EQUAL(m_pNPC->GetModuleCombat()->GetTalent()->GetPhase() ,TALENT_PHASE_EXTRA3);
	m_pField->Update(1.0f);	// ExtraPhaseTime2 ���� 1�� �� ���

	CHECK_EQUAL(m_pPlayer->GetCurrentMotionFactor() , MF_NONE);
	CHECK_EQUAL(false, m_pNPC->HasBuff(m_NPCInfo.nEatOwnerBuffID));
	CHECK_EQUAL(false, m_pPlayer->HasBuff(m_NPCInfo.nEatTargetBuffID));
	CHECK_EQUAL(true, m_pNPC->GetNPCSwallow().IsSwallowable());

	// Link�� ���� Ŀ�ǵ�
	//	[0:SEND] (1443)MC_ACTION_USE_TALENT
	//	[1:SEND] (1449)MC_ACTION_EXTRA_ACT_TALENT
	//	[2:SEND] (1460)MC_ACTION_TALENT_HIT
	//	[3:SEND] (1617)MC_NPC_SPEWUP_INTERRUPT
}

TEST_FIXTURE(FTalentSwallow, TestTalentSwallowedHitFailed)
{
	TALENT_TARGET_INFO target_info;
	target_info.uidTarget = m_pPlayer->GetUID();

	// npc�� ��� �ŷ�Ʈ ���
	m_pNPC->GetHateTable().AddPoint_FoundEnemy(m_pPlayer);

	vec3 player_pos = vec3(1000, 10000, 0);
	m_pPlayer->SetPos(player_pos);

	m_pNPC->GetModuleCombat()->UseTalent(m_pSwallowedTalentInfo, target_info, false);
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

	m_pNPC->GetModuleCombat()->UseTalent(m_pSwallowedTalentInfo, target_info, false);
	m_pField->Update(0.1f);

	// ������ ������ �ȵǾ����Ƿ� Act Phase�̴�.
	CHECK(m_pNPC->GetModuleCombat()->GetTalent() != NULL);
	if (m_pNPC->GetModuleCombat()->GetTalent())
	{
		CHECK_EQUAL(m_pNPC->GetModuleCombat()->GetTalent()->GetPhase() ,TALENT_PHASE_ACT);
	}

}



TEST_FIXTURE(FTalentSwallow, TestTalentSwallowedHitFailed2)
{
	TALENT_TARGET_INFO target_info;
	target_info.uidTarget = m_pPlayer->GetUID();

	// npc�� ��� �ŷ�Ʈ ���
	m_pNPC->GetHateTable().AddPoint_FoundEnemy(m_pPlayer);

	vec3 player_pos = m_pNPC->GetPos();
	m_pPlayer->SetPos(player_pos);

	m_pPlayer->doSwimming();

	m_pNPC->GetModuleCombat()->UseTalent(m_pSwallowedTalentInfo, target_info, false);
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

	m_pNPC->GetModuleCombat()->UseTalent(m_pSwallowedTalentInfo, target_info, false);
	m_pField->Update(0.1f);

	// �������̸� �������� �ʴ´�. -----------------------
	CHECK(m_pNPC->GetModuleCombat()->GetTalent() != NULL);
	if (m_pNPC->GetModuleCombat()->GetTalent())
	{
		CHECK_EQUAL(m_pNPC->GetModuleCombat()->GetTalent()->GetPhase() ,TALENT_PHASE_ACT);
	}

}

TEST_FIXTURE(FTalentSwallow, SpewupWhenDie)
{
	test.buff.NewBuffInfo(m_NPCInfo.nEatOwnerBuffID);
	test.buff.NewBuffInfo(m_NPCInfo.nEatTargetBuffID);

	TALENT_TARGET_INFO target_info;
	target_info.uidTarget = m_pPlayer->GetUID();

	m_pNPC->GetModuleCombat()->UseTalent(m_pSwallowedTalentInfo, target_info, false);
	m_pField->Update(0.1f);

	CHECK_EQUAL(m_pPlayer->GetCurrentMotionFactor() , MF_SWALLOWED);
	CHECK_EQUAL(true, m_pNPC->HasBuff(m_NPCInfo.nEatOwnerBuffID));
	CHECK_EQUAL(true, m_pPlayer->HasBuff(m_NPCInfo.nEatTargetBuffID));
	CHECK_EQUAL(false, m_pNPC->GetNPCSwallow().IsSwallowable());

	m_pNPC->doDie();

	CHECK_EQUAL(m_pPlayer->GetCurrentMotionFactor() , MF_NONE);
	CHECK_EQUAL(false, m_pNPC->HasBuff(m_NPCInfo.nEatOwnerBuffID));
	CHECK_EQUAL(false, m_pPlayer->HasBuff(m_NPCInfo.nEatTargetBuffID));
	CHECK_EQUAL(true, m_pNPC->GetNPCSwallow().IsSwallowable());
}


}
