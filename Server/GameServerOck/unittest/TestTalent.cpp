#include "stdafx.h"
#include "SUnitTest.h"
#include "GDef.h"
#include "GTalentInfo.h"
#include "GTalent.h"
#include "GNPCInfo.h"
#include "GNPCMgr.h"
#include "MockEntityNPC.h"
#include "MockField.h"
#include "GEntityPlayer.h"
#include "GMeleeTalent.h"
#include "GArcheryTalent.h"
#include "GMagicTalent.h"
#include "GPathFinder.h"
#include "GGlobal.h"
#include "GTestWrapper.h"
#include "GModuleCombat.h"
#include "GUTHelper.h"
#include "CSModifier.h"
#include "GCommandTable.h"
#include "GTalentEffector.h"
#include "GModuleBuff.h"
#include "GTalentFactory.h"
#include "GJob_Broker.h"
#include "GModuleAI.h"
#include "GCmdHandler_Action.h"
#include "MockLink.h"
#include "GJobRunner.h"
#include "GPlayerTalent.h"
#include "GTestForward.h"
#include "GFieldMgr.h"
#include "GPlayerObjectManager.h"
#include "GTalentInfoDef.h"
#include "GHateTable.h"
#include "FBaseMockLink.h"
#include "GItemManager.h"
#include "GUTHelper_Player.h"
#include "CSEffectInfo.h"
#include "CCommandResultTable.h"
#include "GActorCooltimeChecker.h"


SUITE(Talent)
{

/// �ŷ�Ʈ �׽�Ʈ�� ���� �ϱ� ���� �Ƚ���
	class TestTalentFixture : public FBaseMockLink
	{
	public:
		TestTalentFixture()
		{
			m_pField = GUTHelper_Field::DefaultMockField();

			m_NPCInfo.nID = 1;
			m_TalentInfo.m_nID = 1;
		}
		virtual ~TestTalentFixture()
		{
			m_pField->Destroy();
		}

		GTalentInfo* MakeDelayedActTalentinfo()
		{
			GTalentInfo* pTalentInfo = test.talent.NewMagicTalentInfo();
			CSTalentEventInfo infoEvent;
			infoEvent.m_nEvent = TE_DELAYED_ACT;
			infoEvent.m_fTime1 = 10.0;
			pTalentInfo->m_Events.m_vecActEvents.push_back(infoEvent);
			test.talent.SetTalentDamage(pTalentInfo, 10);
			pTalentInfo->Cooking();
			return pTalentInfo;
		}

		GUTHelper		m_Helper;
		MockField*		m_pField;
		GNPCInfo		m_NPCInfo;
		CHR_STATUS		m_NPCStatus;
		GTalentInfo		m_TalentInfo;
		GJobRunner		m_JobRunner;
		GTalentEffector m_TalentEffector;
		GTestMgrWrapper<GTalentInfoMgr>	m_TalentInfoMgrWrapper;
	};

TEST3(TestExistArcheryTalent)
{
	GTalentInfo* pTalentInfo = gmgr.pTalentInfoMgr->Find(PLAYER_NORMAL_ATTACK_ARROW_TALENT_ID);
	CHECK(pTalentInfo != NULL);
	if (pTalentInfo == NULL) return;

	CHECK(pTalentInfo->m_nSkillType == ST_ARCHERY);
}

TEST_FIXTURE(TestTalentFixture, TestExtraActiveTalentMustHaveEnumValue)
{
	for (GTalentInfoMgr::iterator itorTalentInfo = gmgr.pTalentInfoMgr->begin(); itorTalentInfo != gmgr.pTalentInfoMgr->end(); itorTalentInfo++)
	{
		GTalentInfo* pTalentInfo = static_cast<GTalentInfo*>((*itorTalentInfo).second);
		if (pTalentInfo->m_nSkillType == ST_EXTRA_ACTIVE)
		{
			CHECK(pTalentInfo->m_nExtraActive != TEAT_NONE);
		}

		if (pTalentInfo->m_nSkillType == ST_EXTRA_PASSIVE)
		{
			CHECK(pTalentInfo->m_nExtraPassive != TEPT_NONE);
		}

		if (pTalentInfo->m_nTalentType == TT_SKILL)
		{
			CHECK(pTalentInfo->m_nSkillType != ST_NONE);
		}
	}
}

TEST_FIXTURE(TestTalentFixture, DelayedActTalent)
{
	DECLWRAPPER_SimpleCombatCalc;

	GEntityPlayer* pAttackerPlayer = GUTHelper::NewEntityPlayer(m_pField);
	GTalentInfo* pTalentInfo = MakeDelayedActTalentinfo();
	pAttackerPlayer->SetHP(100);
	CHECK_EQUAL(100, pAttackerPlayer->GetHP());
	test.talent.UseTalent(pAttackerPlayer, pTalentInfo, pAttackerPlayer->GetUID());
	CHECK_EQUAL(100, pAttackerPlayer->GetHP());
	pAttackerPlayer->GetModuleCombat()->Update(5.0f);
	CHECK_EQUAL(100, pAttackerPlayer->GetHP());
	pAttackerPlayer->GetModuleCombat()->Update(5.0f);
	CHECK_EQUAL(90, pAttackerPlayer->GetHP());
}


TEST(TestProjectile)
{
	GUTHelper helper;

	MockField* pField = GUTHelper_Field::DefaultMockField();
	
	vec3 pos = vec3(1000.0f, 1000.0f, 0.0f);
	
	GEntityPlayer* pAttackerPlayer = GUTHelper::NewEntityPlayer(pField, pos);

	GTalentInfo attack_talent_info;
	helper.SetMeleeTalentInfo(&attack_talent_info, 1);

	pField->Destroy();
}

TEST_FIXTURE(TestTalentFixture, TestTalentArcheryHit)
{
	m_Helper.SetArcheryTalentInfo(&m_TalentInfo, 1);

	MCapsule hit_cap = MCapsule(vec3(0.0f, 0.0f, -5000.0f), vec3(0.0f, 0.0f, 5000.0f), 5000.0f);
	CSHitSegment hit_seg;
	hit_seg.m_vCapsules.push_back(hit_cap);
	hit_seg.m_vDoHitCheck.push_back(true);
	hit_seg.m_fCheckTime = 0.0f;

	m_TalentInfo.m_HitInfo.m_nID = 1;
	m_TalentInfo.m_HitInfo.m_vSegments.push_back(hit_seg);

	vec3 pos = vec3(1000.0f, 1000.0f, 0.0f);
	vec3 dir = vec3(0.0f,1.0f,0.0f);

	GEntityNPC* pNewNPC = m_pField->SpawnTestNPC(&m_NPCInfo, pos, dir);
	if (pNewNPC == NULL) 
	{
		return;
	}

	vec3 player_pos = pos;
	GEntityPlayer* pAttackerPlayer = GUTHelper::NewEntityPlayer(m_pField, player_pos);

	// TODO: ���ݿ� ���� ���� ������Ⱑ ���� �����
	pAttackerPlayer->GetModuleCombat()->UseTalent(&m_TalentInfo, pNewNPC->GetUID(), false);

	GArcheryTalent archeryTalent(pNewNPC, &m_TalentInfo, TALENT_TARGET_INFO(pNewNPC->GetUID()));
	archeryTalent.Start();
	archeryTalent.Update(1.0f);

	CHECK(archeryTalent.GetPhase() == TALENT_PHASE_FINISH);
}

TEST_FIXTURE(TestTalentFixture, TestMeleeTalentPhase)
{
	GTalentInfo talent_info;
	m_Helper.SetMeleeTalentInfo(&talent_info, 1);
	m_Helper.AddSimpleHitInfo(&talent_info);

	GEntityPlayer* pPlayer = GUTHelper::NewEntityPlayer(m_pField, vec3::ZERO);
	GMeleeTalent talent(pPlayer, &talent_info);

	talent.Start();
	talent.Update(1.0f);
	talent.UpdateExpired(1.0f);

	CHECK(talent.GetPhase() == TALENT_PHASE_FINISH);
}

TEST(TestMagicEventCheck)
{
	GUTHelper helper;

	GTalentInfo talent_info;
	talent_info.m_nID = 1;

	helper.AddTalentActEventInfo(&talent_info, TE_LAUNCH_PROJECTILE, 0.42f);

	GTalentProjectile projectile;
	GTalentEventChecker checker;
	CSTalentEventInfo* pOutEventInfo = NULL;
	float fActElapsedTime = 0.0f;

	pOutEventInfo = NULL;
	CHECK(checker.CheckActEvent(&talent_info, fActElapsedTime, &pOutEventInfo) == false);
	CHECK(checker.IsCheckActEventDone(&talent_info) == false);

	fActElapsedTime += 1.0f;

	pOutEventInfo = NULL;
	CHECK(checker.CheckActEvent(&talent_info, fActElapsedTime, &pOutEventInfo) == true);
	CHECK(checker.IsCheckActEventDone(&talent_info) == true);

}

TEST(TestTalentPhaseCheck_WhenCastingTimeIsZero)
{
	GUTHelper helper;

	GTalentInfo talent_info;
	helper.SetMagicTalentInfo(&talent_info, 1);
	talent_info.m_fDurationTime = 1.0f;
	talent_info.m_fCastingTime = 0.0f;
	talent_info.Cooking();

	float fElapsedTime = 0.0f;
	MockField*	pField = GUTHelper_Field::DefaultMockField();
	GEntityNPC*	pNPC = GUTHelper_NPC::SpawnNPC(pField);

	GMagicTalent magic_talent(pNPC, &talent_info, TALENT_TARGET_INFO(pNPC->GetUID()));
	magic_talent.Start();
	magic_talent.Update(0.1f);

	CHECK_EQUAL(magic_talent.GetPhase(), TALENT_PHASE_ACT);

	pField->Destroy();
}

TEST(TestMagicTalentLifeTime)
{
	GUTHelper helper;
	GTalentInfo talent_info;
	helper.SetMagicTalentInfo(&talent_info, 1);
	talent_info.m_fDurationTime = 1.0f;
	talent_info.m_fCastingTime = 0.0f;
	talent_info.Cooking();

	MockField*	pField = GUTHelper_Field::DefaultMockField();
	GEntityNPC*	pNPC = GUTHelper_NPC::SpawnNPC(pField);

	float fElapsedTime = 0.0f;
	
	GMagicTalent magic_talent(pNPC, &talent_info);
	magic_talent.Start();

	fElapsedTime+=0.1f;
	magic_talent.Update(fElapsedTime);

	CHECK_EQUAL(magic_talent.GetPhase(), TALENT_PHASE_ACT);

	fElapsedTime+=1.0f;
	CHECK(magic_talent.Update(fElapsedTime) == true);
	magic_talent.UpdateExpired(fElapsedTime);

	CHECK_EQUAL(magic_talent.GetPhase(), TALENT_PHASE_FINISH);

	fElapsedTime+=0.1f;
	CHECK(magic_talent.Update(fElapsedTime) == false);
	magic_talent.UpdateExpired(fElapsedTime);

	pField->Destroy();
}

TEST(KillByMagicTalent)
{
	GTalentInfo* pMagicTalent = test.talent.NewMagicTalentInfo();
	test.talent.SetTalentDamage(pMagicTalent, 100);
	pMagicTalent->m_fDurationTime = 1.0f;
	pMagicTalent->m_fCastingTime = 0.0f;
	pMagicTalent->m_Resist.m_nDifficulty = 255;	// ���� �Ұ����ϰ� ����
	pMagicTalent->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
	pMagicTalent->m_EffectInfo.m_nRelation = CSEffectInfo::RELATION_ENEMY;
	pMagicTalent->Cooking();
	
	MockField*	pField = GUTHelper_Field::DefaultMockField();
	GEntityPlayer* pAttacker = GUTHelper::NewEntityPlayer(pField);
	GEntityNPC*	pVictim = GUTHelper_NPC::SpawnNPC(pField);

	pVictim->SetHP(1);
	CHECK_EQUAL(false, pVictim->IsDead());
	test.talent.UseTalent(pAttacker, pMagicTalent, pVictim->GetUID());
	CHECK_EQUAL(true, pVictim->IsDead());

	pField->Destroy();
}





TEST_FIXTURE(TestTalentFixture, TestHealTalent)
{

	GTalentInfo* pNewTalentInfo = new GTalentInfo();
	m_Helper.SetMagicTargetTalentInfo(pNewTalentInfo, 1);
	pNewTalentInfo->m_InstantModifier.nHP.nMod = 10;		// hp�� 10 �÷���
	gmgr.pTalentInfoMgr->Insert(pNewTalentInfo);

	GEntityPlayer* pPlayer1 = GUTHelper::NewEntityPlayer(m_pField, vec3(100,100,0));
	GEntityPlayer* pPlayer2 = GUTHelper::NewEntityPlayer(m_pField, vec3(200,100,0));

	PLAYER_INFO* player_info2 = pPlayer2->GetPlayerInfo();
	player_info2->nMaxHP = 100;
	pPlayer2->SetHP(50);

	CHECK_EQUAL(pPlayer2->GetHP(), 50);

	MUID uidTarget = pPlayer2->GetUID();
	pPlayer1->doUseTalent(1, uidTarget);

	pPlayer1->Update(0.1f);
	pPlayer2->Update(0.1f);

	CHECK_EQUAL(pPlayer2->GetHP(), 60);
}

TEST_FIXTURE(TestTalentFixture, TestHealTalent2)
{

	GTalentInfo* pNewTalentInfo = new GTalentInfo();
	m_Helper.SetMagicTargetTalentInfo(pNewTalentInfo, 1);
	pNewTalentInfo->m_nMinHeal = 10;		// hp�� 10 �÷���
	pNewTalentInfo->m_nMaxHeal = 10;		// hp�� 10 �÷���
	gmgr.pTalentInfoMgr->Insert(pNewTalentInfo);

	GEntityPlayer* pPlayer1 = GUTHelper::NewEntityPlayer(m_pField, vec3(100,100,0));
	GEntityPlayer* pPlayer2 = GUTHelper::NewEntityPlayer(m_pField, vec3(200,100,0));

	PLAYER_INFO* player_info2 = pPlayer2->GetPlayerInfo();
	player_info2->nMaxHP = 100;
	pPlayer2->SetHP(50);

	CHECK_EQUAL(pPlayer2->GetHP(), 50);

	MUID uidTarget = pPlayer2->GetUID();
	pPlayer1->doUseTalent(1, uidTarget);

	pPlayer1->Update(0.1f);
	pPlayer2->Update(0.1f);

	CHECK_EQUAL(pPlayer2->GetHP(), 50 + 10);
}

TEST_FIXTURE(TestTalentFixture, TestHealTalent2EffectionSpellPower)
{
	GTalentInfo* pNewTalentInfo = new GTalentInfo();
	m_Helper.SetMagicTargetTalentInfo(pNewTalentInfo, 1);
	pNewTalentInfo->m_nMinHeal = 10;		// hp�� 10 �÷���
	pNewTalentInfo->m_nMaxHeal = 10;		// hp�� 10 �÷���
	gmgr.pTalentInfoMgr->Insert(pNewTalentInfo);

	GUTHelper helper;
	GEntityNPC* pNPC = helper.NewEntityNPC(m_pField, vec3(100,100,0));
	const_cast<GNPCInfo*>(pNPC->GetNPCInfo())->m_fSpellPower = 2.0f;
	GEntityPlayer* pPlayer2 = helper.NewEntityPlayer(m_pField, vec3(200,100,0));

	PLAYER_INFO* player_info2 = pPlayer2->GetPlayerInfo();
	player_info2->nMaxHP = 100;
	pPlayer2->SetHP(50);

	CHECK_EQUAL(pPlayer2->GetHP(), 50);

	MUID uidTarget = pPlayer2->GetUID();
	pNPC->doUseTalent(1, uidTarget);

	pNPC->Update(0.1f);
	pPlayer2->Update(0.1f);

	CHECK_EQUAL(pPlayer2->GetHP(), 50 + 20);
}

TEST_FIXTURE(TestTalentFixture, DeadActorCannotUseTalent)
{
	// �÷��̾ ���� �ŷ�Ʈ
	GTalentInfo* pNewTalentInfo = m_Helper.NewMeleeTalentInfo(1);
	m_Helper.SetTalentDamage(pNewTalentInfo, 10000);		// �ѹ濡 �״� �ŷ�Ʈ

	m_Helper.AddSimpleHitInfo(pNewTalentInfo, 0.0f);	// �ٷ� �����ϴ� �ŷ�Ʈ��

	// NPC�� ���� �ŷ�Ʈ
	GTalentInfo* pNewNPCTalentInfo = m_Helper.NewMeleeTalentInfo(2);
	m_Helper.SetTalentDamage(pNewNPCTalentInfo, 10000);		// �ѹ濡 �״� �ŷ�Ʈ
	
	m_Helper.AddSimpleHitInfo(pNewNPCTalentInfo, 1.0f);	// 1�� �� �����ϴ� �ŷ�Ʈ��

	vec3 pos = vec3(1000.0f, 1000.0f, 0.0f);
	vec3 dir = vec3(0.0f,1.0f,0.0f);

	GEntityNPC* pNPC = m_pField->SpawnTestNPC(&m_NPCInfo, pos, dir);
	if (pNPC == NULL) 
	{
		return;
	}

	GEntityPlayer* pAttackerPlayer = GUTHelper::NewEntityPlayer(m_pField, pos);
	m_Helper.SetEntityPlayerStatus(pAttackerPlayer, 100, 100, 100);

	// pc, npc �Ѵ� hp�� Max
	pAttackerPlayer->SetHP(pAttackerPlayer->GetMaxHP());
	CHECK(pNPC->GetMaxHP() == pNPC->GetHP());
	CHECK(pAttackerPlayer->GetMaxHP() == pAttackerPlayer->GetHP());

	// npc�� 1�� �� �����ϴ� �ŷ�Ʈ�� ���� ����
	pNPC->GetModuleCombat()->UseTalent(pNewNPCTalentInfo, TALENT_TARGET_INFO::Invalid(), false);

	// pc�� �ŷ�Ʈ ����Ͽ� NPC�� ���
	pAttackerPlayer->GetModuleCombat()->UseTalent(pNewTalentInfo, TALENT_TARGET_INFO::Invalid(), false);

	m_pField->Update(0.1f);

	CHECK_EQUAL(pNPC->IsDead(), true);

	m_pField->Update(1.1f);

	CHECK(pAttackerPlayer->GetHP() == pAttackerPlayer->GetMaxHP());		// NPC�� �̹� �׾����Ƿ� PC�� �������� ������ �ȵȴ�.
}

// �ŷ�Ʈ ���� �ð�
TEST_FIXTURE(TestTalentFixture, TestTalentInvincibleTime)
{
	// �÷��̾�1�� ���� �ŷ�Ʈ
	GTalentInfo* pNewTalentInfo = m_Helper.NewMeleeTalentInfo(1);
	m_Helper.SetTalentDamage(pNewTalentInfo, 10);

	m_Helper.AddSimpleHitInfo(pNewTalentInfo, 10.0f);	// 10���Ŀ� ���� (10�ʵ��� �ŷ�Ʈ ����)

	pNewTalentInfo->m_bUseInvincibleTime = true;
	pNewTalentInfo->m_fInvincibleTimeStart = 0.5f;
	pNewTalentInfo->m_fInvincibleTimeEnd = 0.8f;
	pNewTalentInfo->m_fDurationTime = 1.0f;
	pNewTalentInfo->Cooking();

	GEntityPlayer* pPlayer1 = GUTHelper::NewEntityPlayer(m_pField, vec3(100,100,0));
	GEntityPlayer* pPlayer2 = GUTHelper::NewEntityPlayer(m_pField, vec3(200,100,0));


	PLAYER_INFO* player_info2 = pPlayer2->GetPlayerInfo();
	player_info2->nMaxHP = 100;
	pPlayer2->SetHP(100);

	CHECK_EQUAL(pPlayer2->GetHP(), 100);

	pPlayer1->doUseTalent(1, pPlayer2->GetUID());

	m_pField->Update(0.1f);
	GModuleCombat* pmCombat = pPlayer1->GetModuleCombat();
	CHECK_EQUAL(pmCombat->IsNowInvincibleTime(), false);

	m_pField->Update(0.3f);
	CHECK_EQUAL(pmCombat->IsNowInvincibleTime(), false);

	m_pField->Update(0.2f);
	CHECK_EQUAL(pmCombat->IsNowInvincibleTime(), true);

	m_pField->Update(0.3f);
	CHECK_EQUAL(pmCombat->IsNowInvincibleTime(), false);
}

// �ŷ�Ʈ ���۾Ƹ�(������� ��ȭ����) �ð�
TEST_FIXTURE(TestTalentFixture, TestTalentSuperarmorTime)
{
	// �÷��̾�1�� ���� �ŷ�Ʈ
	GTalentInfo* pNewTalentInfo = test.talent.MakeDamageTalent(1);
	pNewTalentInfo->m_bUseSuperarmorTime = true;
	pNewTalentInfo->m_fSuperarmorTimeStart = 0.5f;
	pNewTalentInfo->m_fSuperarmorTimeEnd = 0.8f;
	pNewTalentInfo->m_fDurationTime = 10.0f;
	pNewTalentInfo->m_fDurationTime = 1.0f;	
	pNewTalentInfo->Cooking();

	GEntityPlayer* pPlayer1 = GUTHelper::NewEntityPlayer(m_pField, vec3(100,100,0));
	GEntityPlayer* pPlayer2 = GUTHelper::NewEntityPlayer(m_pField, vec3(110,110,0));
	pPlayer1->SetMFRegenActive(false);
	pPlayer2->SetMFRegenActive(false);

	// ���� �ŷ�Ʈ
	GTalentInfo* pAtkTalent = test.talent.MakeDamageTalent(1);
	pAtkTalent->m_MotionFactorGroup.var[MF_BEATEN].nValue = 20;
	pAtkTalent->m_MotionFactorGroup.var[MF_BEATEN].nWeight = 1000;
	pAtkTalent->m_MotionFactorGroup.var[MF_KNOCKDOWN].nValue = 15;
	pAtkTalent->m_MotionFactorGroup.var[MF_KNOCKDOWN].nWeight = 1000;
	pAtkTalent->m_fDurationTime = 10.0f;
	pAtkTalent->m_fDurationTime = 1.0f;

	// ���۾Ƹ� �Ӽ��� �ִ� �ŷ�Ʈ �����߿� ������Ͱ� �ִ� �ŷ�Ʈ�� ���� ����
	test.talent.UseTalent(pPlayer1, pNewTalentInfo, pPlayer2->GetUID());
	
	m_pField->Update(0.1f);
	test.talent.UseTalent(pPlayer2, pAtkTalent, pPlayer1->GetUID());
	CHECK_EQUAL(pPlayer1->GetMotionfactorForTest().GetFactorsForTest().var[MF_BEATEN].nValue, 20);
	CHECK_EQUAL(pPlayer1->GetMotionfactorForTest().GetFactorsForTest().var[MF_KNOCKDOWN].nValue, 15);
	m_pField->Update(0.3f);
	test.talent.UseTalent(pPlayer2, pAtkTalent, pPlayer1->GetUID());
	CHECK_EQUAL(pPlayer1->GetMotionfactorForTest().GetFactorsForTest().var[MF_BEATEN].nValue, 40);
	CHECK_EQUAL(pPlayer1->GetMotionfactorForTest().GetFactorsForTest().var[MF_KNOCKDOWN].nValue, 30);
	m_pField->Update(0.2f);
	test.talent.UseTalent(pPlayer2, pAtkTalent, pPlayer1->GetUID());
	CHECK_EQUAL(pPlayer1->GetMotionfactorForTest().GetFactorsForTest().var[MF_BEATEN].nValue, 40);	// ���۾Ƹ����̶� ȿ�� ����
	CHECK_EQUAL(pPlayer1->GetMotionfactorForTest().GetFactorsForTest().var[MF_KNOCKDOWN].nValue, 45);
	m_pField->Update(0.3f);
	test.talent.UseTalent(pPlayer2, pAtkTalent, pPlayer1->GetUID());
	CHECK_EQUAL(pPlayer1->GetMotionfactorForTest().GetFactorsForTest().var[MF_BEATEN].nValue, 60);
	CHECK_EQUAL(pPlayer1->GetMotionfactorForTest().GetFactorsForTest().var[MF_KNOCKDOWN].nValue, 60);;
}

// �ŷ�Ʈ ĸ������ ����
TEST_FIXTURE(TestTalentFixture, TestTalentSeparateHit)
{
	GTalentInfo* pNewTalentInfo = m_Helper.NewMeleeTalentInfo(1);
	m_Helper.SetTalentDamage(pNewTalentInfo, 10);

	// ���� ������ 2��
	m_Helper.AddSimpleHitInfo(pNewTalentInfo, 0.0f);
	m_Helper.AddSimpleHitInfo(pNewTalentInfo, 1.0f);

	pNewTalentInfo->m_bSeparateHit = true;		// ĸ������ �����ϵ��� ����
	pNewTalentInfo->m_fDurationTime = 2.0f;
	pNewTalentInfo->Cooking();

	GEntityPlayer* pPlayer1 = GUTHelper::NewEntityPlayer(m_pField, vec3(100,100,0));

	m_NPCInfo.nMaxHP = 100;
	GEntityNPC* pNPC = m_pField->SpawnTestNPC(&m_NPCInfo, vec3(100,100,0), vec3(0,1,0));
	if (pNPC == NULL) 
	{
		return;
	}

	pNPC->SetHP(100);

	CHECK_EQUAL(pNPC->GetHP(), 100);

	pPlayer1->doUseTalent(pNewTalentInfo->m_nID, pNPC->GetUID());

	m_pField->Update(0.05f);			// ������ 100ms ���� �Ͼ�Ƿ� ������Ʈ �ð��� �����Ű�� ������ 2�� ������ �õ��ϰ� �ȴ�


	GActorDamageCalculator damageCalculator;
	GActorDamageCalculator::Desc desc;
	desc.pAttacker = pPlayer1;
	desc.pVictim = pNPC;
	desc.pTalentInfo = pNewTalentInfo;
	desc.DamageInfo = GDamageRangedInfo(pNewTalentInfo->m_nMaxDamage, pNewTalentInfo->m_nMinDamage);
	desc.HealInfo = GHealRangedInfo(0,0);
	const int nDamage = damageCalculator.CalcDamage(desc).nDamage;
	int nHP = 100 - nDamage;

	CHECK_EQUAL(pNPC->GetHP(), nHP);

	m_pField->Update(1.0f);
	nHP = nHP - nDamage;
	CHECK_EQUAL(pNPC->GetHP(), nHP);
}

// �ŷ�Ʈ ���� MP �Һ� Ȯ��
TEST_FIXTURE(TestTalentFixture, TestTalent_CostEN)
{
	// �÷��̾�1�� ���� �ŷ�Ʈ
	GTalentInfo* pNewTalentInfo = m_Helper.NewMeleeTalentInfo(1);
	m_Helper.SetTalentDamage(pNewTalentInfo, 10);
	m_Helper.AddSimpleHitInfo(pNewTalentInfo, 0.0f);
	pNewTalentInfo->m_nENCost = 10;
	pNewTalentInfo->m_fDurationTime = 1.0f;

	GEntityNPC* pNPC = GUTHelper_NPC::SpawnNPC(m_pField);
	CHR_INFO* chr_info = pNPC->GetChrInfo();
	chr_info->nMaxEN = 100;
	pNPC->SetEN(100);
	CHECK_EQUAL(pNPC->GetEN(), 100);
	CHECK_EQUAL(true, pNPC->doUseTalent(1));
	CHECK_EQUAL(pNPC->GetEN(), 90);
	pNPC->SetEN(0);
	CHECK_EQUAL(false, pNPC->doUseTalent(1));	// en�� �����Ƿ� �ŷ�Ʈ�� ������� ���Ѵ�.
	CHECK_EQUAL(pNPC->GetEN(), 0);
}


TEST3(TestRequestAttackMelee_Counter)
{
	GTestMgrWrapper<GPlayerObjectManager>				m_PlayerObjectManager;
	GUTHelper helper;

	MockField* pField = NULL;
	GEntityPlayer* pEntityPlayer = NULL;
	vec3 pos = vec3(1000.0f, 1000.0f, 0.0f);

	MockPlayerObject* pPlayerObject = new MockPlayerObject(SUnitTestUtil::NewUID());
	pPlayerObject->Create();
	gmgr.pPlayerObjectManager->AddPlayer(pPlayerObject);

	pField = GUTHelper_Field::DefaultMockField();
	pEntityPlayer = pPlayerObject->GetEntity();
	pEntityPlayer->GetPlayerInfo()->nCID = SUnitTestUtil::NewCID();
	pEntityPlayer->EnterToField(pField, pos, vec3(0,1,0));


	// ���� ����ֱ�
	helper.EquipSword_ForTS2(pEntityPlayer);

	MCommand command(MC_ACTION_ATTACK_MELEE_REQ, 0, 0);
	TD_REQUEST_ATTACK td_request_attack;
	td_request_attack.nAttackType = NA_COUNTER_ATTACK;
	td_request_attack.vPos = pos;
	td_request_attack.svDir = vec3::AXISY;

	command.AddParameter(new MCommandParameterSingleBlob(&td_request_attack,sizeof(TD_REQUEST_ATTACK)));
	command.SetSenderUID(pEntityPlayer->GetUID());

	GCmdHandler_Action::OnRequestAttackMelee(&command, NULL);

	// ī���� ���� �нú갡 �����Ƿ� �ŷ�Ʈ ��� �����ؾ� ����
	CHECK(pEntityPlayer->GetModuleCombat()->GetUsingTalentID() == 0);

	const int COUNTER_ATTACK_TALENT_ID = 1011;
	if (helper.LearnTalent_ForTS2(pEntityPlayer, COUNTER_ATTACK_TALENT_ID) == false)
	{
		SAFE_DELETE(pField);
		return;
	}

	pEntityPlayer->SetSTA(100);
	GCmdHandler_Action::OnRequestAttackMelee(&command, NULL);
	CHECK(pEntityPlayer->GetModuleCombat()->GetUsingTalentID() != 0);


	gmgr.pPlayerObjectManager->DeletePlayer(pEntityPlayer->GetUID());
	pField->Destroy();
}


TEST_FIXTURE(TestTalentFixture, TestPlayerGuardCostSTA)
{
	// �÷��̾�1�� ���� ��� �ŷ�Ʈ
	GTalentInfo* pNewTalentInfo = m_Helper.NewGuardTalentInfo(1);
	pNewTalentInfo->m_nSTACost = 10;

	// NPC�� ���� ���� �ŷ�Ʈ
	GTalentInfo* pNPCTalentInfo = m_Helper.NewMeleeTalentInfo(2);
	m_Helper.SetTalentDamage(pNPCTalentInfo, 10);
	m_Helper.AddSimpleHitInfo(pNPCTalentInfo);

	vec3 pos = vec3(100,100,0);
	vec3 dir = vec3(0,1,0);

	GEntityPlayer* pPlayer = GUTHelper::NewEntityPlayer(m_pField, pos);

	PLAYER_INFO* player_info = pPlayer->GetPlayerInfo();
	player_info->nLevel = 60;
	m_Helper.SetEntityPlayerStatus(pPlayer, 100, 100, 100);

	CHECK_EQUAL(pPlayer->GetSTA(), 100);

	int nPlayerAP = pPlayer->GetAP();

	// �� �ϸ� ���� ������� �ǳ� Ȯ��
	pPlayer->doGuard(1);
//	CHECK_EQUAL(pPlayer->GetSTA(), 90);	// ��� ���۽� ���׹̳� �Ҹ�
	CHECK(pPlayer->IsGuarding());

	// ������̸� AP�� �ö󰡳� Ȯ��
	CHECK_EQUAL(pPlayer->GetAP(), nPlayerAP * 2);

	m_pField->Update(1.0f);

	pPlayer->doGuardReleased();
	CHECK(pPlayer->IsGuarding() == false);

	m_pField->Update(1.0f);

	//CHECK_EQUAL(pPlayer->GetSTA(), 90);

	m_pField->Update(1.0f);

	pos = pos + pPlayer->GetDir();
	dir = -pPlayer->GetDir();
	GEntityNPC* pEntityNPC = m_pField->SpawnTestNPC(&m_NPCInfo, pos, dir);
	if (pEntityNPC == NULL) 
	{
		return;
	}

	pPlayer->SetHP(pPlayer->GetMaxHP());
	CHECK(pPlayer->GetHP() == pPlayer->GetMaxHP());

	pEntityNPC->doUseTalent(2);
	m_pField->Update(1.0f);
	CHECK(pPlayer->GetHP() != pPlayer->GetMaxHP());
	
	pPlayer->SetHP(pPlayer->GetMaxHP());
	CHECK(pPlayer->GetHP() == pPlayer->GetMaxHP());

	// ����� ������ ������ STA �Һ� Ȯ��
	test.item.EquipNewShield(pPlayer);
	pPlayer->doGuard(1);
	pEntityNPC->doUseTalent(2);
	m_pField->Update(1.0f);
	CHECK(pPlayer->GetHP() == pPlayer->GetMaxHP());
//	CHECK_EQUAL(pPlayer->GetSTA(), 80);
	pPlayer->doGuardReleased();

	m_pField->Update(1.0f);

	//// STA�� ������ ����ϰ� �־ ����� �� ����.
	//pPlayer->SetHP(pPlayer->GetMaxHP());
	//pPlayer->SetSTA(0);

	//pPlayer->doGuard(1);
	//pEntityNPC->doUseTalent(2);
	//m_pField->Update(1.0f);
	//CHECK(pPlayer->GetHP() != pPlayer->GetMaxHP());

}


TEST_FIXTURE(TestTalentFixture, TestPlayersHitClose)
{
	// �÷��̾ ���� ��� �ŷ�Ʈ
	GTalentInfo* pNewTalentInfo = m_Helper.NewGuardTalentInfo(1);
	pNewTalentInfo->m_nSTACost = 10;

	// NPC�� ���� ���� �ŷ�Ʈ
	GTalentInfo* pNPCTalentInfo = m_Helper.NewMeleeTalentInfo(2);
	m_Helper.SetTalentDamage(pNPCTalentInfo, 10);
	m_Helper.AddSimpleHitInfo(pNPCTalentInfo);

	vec3 pos = vec3(100,100,0);
	vec3 dir = vec3(0,1,0);

	const int MAX_PLAYERS = 4;

	GEntityPlayer* ppPlayers[MAX_PLAYERS];

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		ppPlayers[i] = GUTHelper::NewEntityPlayer(m_pField, pos);
		PLAYER_INFO* player_info = ppPlayers[i]->GetPlayerInfo();
		player_info->nLevel = 60;
		m_Helper.SetEntityPlayerStatus(ppPlayers[i], 100, 100, 100);
		ppPlayers[i]->SetDir(dir);
	}

	pos = pos + ppPlayers[0]->GetDir();
	dir = -ppPlayers[0]->GetDir();
	GEntityNPC* pEntityNPC = m_pField->SpawnTestNPC(&m_NPCInfo, pos, dir);
	if (pEntityNPC == NULL) return;


	// NPC�� �����ϸ� 4�� ��� �ǰݵȴ�.
	pEntityNPC->doUseTalent(2);
	m_pField->Update(1.0f);

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		CHECK_EQUAL(93, ppPlayers[i]->GetHP());
	}

	// 1���� �� �ϸ� �� ����� �ǰ��� �ȵǰ� �������� �� �ǰݵǾ�� �Ѵ�.
	test.item.EquipNewShield(ppPlayers[1]);
	ppPlayers[1]->doGuard(1);
	pEntityNPC->doUseTalent(2);
	m_pField->Update(1.0f);

	CHECK_EQUAL(86, ppPlayers[0]->GetHP());
	CHECK_EQUAL(93, ppPlayers[1]->GetHP());	
	CHECK_EQUAL(86, ppPlayers[2]->GetHP());
	CHECK_EQUAL(86, ppPlayers[3]->GetHP());
}
//
// ###			��ƼŸ���� ���				###
//
//TEST_FIXTURE(TestTalentFixture, TestPlayerMagicTalentCostEN)
//{
//	// �÷��̾�1�� ���� ��ƼŸ���� ������ ���� �ŷ�Ʈ
//	GTalentInfo* pNewTalentInfo = new GTalentInfo();
//	m_Helper.SetMagicTargetTalentInfo(pNewTalentInfo, 1);
//	pNewTalentInfo->m_fCastingTime = 1.0f;
//	pNewTalentInfo->m_nTriggerType = TT_MANUAL;
//	pNewTalentInfo->m_nENCost = 10;
//	gmgr.pTalentInfoMgr->Insert(pNewTalentInfo);
//
//
//	vec3 pos = vec3(100,100,0);
//	vec3 dir = vec3(0,1,0);
//
//	GEntityPlayer* pPlayer = GUTHelper::NewEntityPlayer(m_pField, pos);
//
//	PLAYER_INFO* player_info = pPlayer->GetPlayerInfo();
//	m_Helper.SetEntityPlayerStatus(pPlayer, 100, 100, 100);
//
//	CHECK_EQUAL(pPlayer->GetEN(), 100);
//
//	pos = pos + pPlayer->GetDir();
//	dir = -pPlayer->GetDir();
//
//	const int MAX_ENEMY = 4;
//	GEntityNPC* ppEnemies[MAX_ENEMY];
//
//	for (int i = 0; i < MAX_ENEMY; i++)
//	{
//		ppEnemies[i] = m_pField->SpawnTestNPC(&m_NPCInfo, pos, dir);
//		if (ppEnemies[i] == NULL) return;
//	}
//
//	CHECK_EQUAL(pPlayer->GetEN(), 100);
//
//
//	pPlayer->doUseTalent(1);
//
//
//	vector<TALENT_TARGET_INFO> vTargets;
//	for (int i = 0; i < MAX_ENEMY; i++)
//	{
//		TALENT_TARGET_INFO TarInfo;
//		TarInfo.uidTarget = ppEnemies[i]->GetUID();
//		vTargets.push_back(TarInfo);
//	}
//
//	m_pField->Update(1.1f);
//
//	pPlayer->GetModuleCombat()->SetTalentTarget(vTargets);
//	pPlayer->GetModuleCombat()->ActSpell();
//
//	// �� �Ѹ��� 10�� 4��
//	CHECK_EQUAL(pPlayer->GetEN(), 60);
//}

TEST_FIXTURE(TestTalentFixture, TestNPCMotionFactorModifier)
{
	// �÷��̾ ���� �ŷ�Ʈ
	GTalentInfo* pNewTalentInfo = m_Helper.NewMeleeTalentInfo(1);
	m_Helper.SetTalentDamage(pNewTalentInfo, 1);		// ������ 1�� ����
	m_Helper.AddSimpleHitInfo(pNewTalentInfo, 0.0f);	// �ٷ� �����ϴ� �ŷ�Ʈ��

	pNewTalentInfo->m_MotionFactorGroup.var[MF_BEATEN].nValue = 100;
	pNewTalentInfo->m_MotionFactorGroup.var[MF_BEATEN].nWeight = 1000;

	vec3 pos = vec3(1000.0f, 1000.0f, 0.0f);
	vec3 dir = vec3(0.0f,1.0f,0.0f);

	GEntityNPC* pNPC = m_pField->SpawnTestNPC(&m_NPCInfo, pos, dir);
	GEntityPlayer* pAttackerPlayer = GUTHelper::NewEntityPlayer(m_pField, pos);

	m_Helper.SetEntityPlayerStatus(pAttackerPlayer, 100, 100, 100);

	pAttackerPlayer->GetModuleCombat()->UseTalent(pNewTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
	m_pField->Update(0.1f);

	// ������� ������̾ �⺻��(1)�̹Ƿ� beaten ���°� �ȴ�.
	CHECK_EQUAL(pNPC->GetCurrentMotionFactor(), MF_BEATEN);

	m_pField->Update(10.0f);

	const_cast<CHR_STATUS*>(pNPC->GetChrStatus())->ActorModifier.MotionFactorValue[MF_BEATEN].fPer = -0.9f;
	pAttackerPlayer->GetModuleCombat()->UseTalent(pNewTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
	m_pField->Update(0.1f);

	// ������� ������̾ 0.1�̹Ƿ� beaten ���°� �ȵȴ�.
	CHECK_EQUAL(pNPC->GetCurrentMotionFactor(), MF_NONE);
}


TEST_FIXTURE(TestTalentFixture, TestMeleeTalentCancelWhileAct)
{
	// �÷��̾ ���� �ŷ�Ʈ --
	GTalentInfo* pNewTalentInfo = m_Helper.NewMeleeTalentInfo(1);
	m_Helper.SetTalentDamage(pNewTalentInfo, 1);		// ������ 1�� ����
	m_Helper.AddSimpleHitInfo(pNewTalentInfo, 0.0f);	// �ٷ� �����ϴ� �ŷ�Ʈ��
	pNewTalentInfo->m_MotionFactorGroup.var[MF_BEATEN].nValue = 100;
	pNewTalentInfo->m_MotionFactorGroup.var[MF_BEATEN].nWeight = 2000;	// 2�� ��ư
	pNewTalentInfo->Cooking();

	// NPC�� ���� �ŷ�Ʈ --
	GTalentInfo* pNewNPCTalentInfo = m_Helper.NewMeleeTalentInfo(2);
	m_Helper.SetTalentDamage(pNewNPCTalentInfo, 1);
	m_Helper.AddSimpleHitInfo(pNewNPCTalentInfo, 1.0f);	// 1�� �� �����ϴ� �ŷ�Ʈ��
	pNewNPCTalentInfo->Cooking();

	vec3 pos = vec3(1000.0f, 1000.0f, 0.0f);
	vec3 dir = vec3(0.0f,1.0f,0.0f);

	GEntityNPC* pNPC = m_pField->SpawnTestNPC(&m_NPCInfo, pos, dir);
	GEntityPlayer* pAttackerPlayer = GUTHelper::NewEntityPlayer(m_pField, pos);
	m_Helper.SetEntityPlayerStatus(pAttackerPlayer, 100, 100, 100);

	pNPC->GetModuleCombat()->UseTalent(pNewNPCTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
	m_pField->Update(0.1f);

	pAttackerPlayer->GetModuleCombat()->UseTalent(pNewTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
	m_pField->Update(0.1f);

	// �÷��̾ ����� �ŷ�Ʈ�� ������ �ٷ� ���ϱ� NPC �ŷ�Ʈ�� ��ҵȴ�.
	CHECK_EQUAL(pNPC->GetCurrentMotionFactor(), MF_BEATEN);

	m_pField->Update(2.0f);

	// NPC �ŷ�Ʈ�� ��ҵǾ����Ƿ� �������� ����
	pAttackerPlayer->SetHP(pAttackerPlayer->GetMaxHP());
	CHECK_EQUAL(pAttackerPlayer->GetHP(), pAttackerPlayer->GetMaxHP());


	m_pField->Update(10.0f);


	// NPC�� ��� �Ұ����� �ŷ�Ʈ�� ����� ��� ----
	pNewNPCTalentInfo->m_bCancelable = false;

	pNPC->GetModuleCombat()->UseTalent(pNewNPCTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
	m_pField->Update(0.1f);

	pAttackerPlayer->GetModuleCombat()->UseTalent(pNewTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
	m_pField->Update(0.1f);

	// ��� �Ұ����� ���� ��� ������ ������ ���� �ʴ´�.
	CHECK_EQUAL(pNPC->GetCurrentMotionFactor(), MF_NONE);

	m_pField->Update(2.0f);

	CHECK(pAttackerPlayer->GetHP() != pAttackerPlayer->GetMaxHP());

}

TEST_FIXTURE(TestTalentFixture, TestTalentLuaEvents)
{
	gsys.pScriptManager->Init();

	if (!WRUNSTRING(WLUA, 
		"function Talent_Start(Actor, Target)\n"
		"	START = 1\n"
		"end\n"
		"function Talent_Act(Actor, Target)\n"
		"	ACT = 2\n"
		"end\n"
		"function Talent_Finish(Actor, Target)\n"
		"	FINISH = 3\n"
		"end\n"
		"function Talent_Cancel(Actor, Target)\n"
		"	CANCEL = 4\n"
		"end\n"
		))	
	{ 
		CHECK(FALSE && "WRunString Failed.");	 
		return; 
	}

	GTalentInfo attack_talent_info;
	m_Helper.SetMeleeTalentInfo(&attack_talent_info, 1);
	attack_talent_info.m_strLuaOnStart = L"Talent_Start";
	attack_talent_info.m_strLuaOnAct = L"Talent_Act";
	attack_talent_info.m_strLuaOnFinish = L"Talent_Finish";
	attack_talent_info.m_strLuaOnCancel = L"Talent_Cancel";

	vec3 pos = vec3(1000.0f, 1000.0f, 0.0f);
	vec3 dir = vec3(0.0f,1.0f,0.0f);

	{
		GEntityPlayer* pAttackerPlayer = GUTHelper::NewEntityPlayer(m_pField, pos);
		pAttackerPlayer->doUseTalent(&attack_talent_info);

		CHECK_EQUAL(WLUA->GetVar<int>("START"), 1);
		pAttackerPlayer->doCancelTalent();
		pAttackerPlayer->GetModuleCombat()->GetTalent()->Update(0.1f);
		CHECK_EQUAL(WLUA->GetVar<int>("ACT"), 2);
	}
	
	{
		GEntityPlayer* pAttackerPlayer = GUTHelper::NewEntityPlayer(m_pField, pos);
		pAttackerPlayer->doUseTalent(&attack_talent_info);
		pAttackerPlayer->GetModuleCombat()->Update(0.1f);
		pAttackerPlayer->GetModuleCombat()->Update(0.1f);
		CHECK_EQUAL(WLUA->GetVar<int>("FINISH"), 3);
		CHECK_EQUAL(WLUA->GetVar<int>("CANCEL"), 0); // ȣ����� ����
	}

	gsys.pScriptManager->Fini();
}

TEST_FIXTURE(TestTalentFixture, TestTalentLuaEvents_CancelCase)
{
	gsys.pScriptManager->Init();

	if (!WRUNSTRING(WLUA, 
		"function Talent_Start(Actor, Target)\n"
		"	START = 1\n"
		"end\n"
		"function Talent_Act(Actor, Target)\n"
		"	ACT = 2\n"
		"end\n"
		"function Talent_Finish(Actor, Target)\n"
		"	FINISH = 3\n"
		"end\n"
		"function Talent_Cancel(Actor, Target)\n"
		"	CANCEL = 4\n"
		"end\n"
		))	
	{ 
		CHECK(FALSE && "WRunString Failed.");	 
		return; 
	}

	GTalentInfo attack_talent_info;
	m_Helper.SetMeleeTalentInfo(&attack_talent_info, 1);
	attack_talent_info.m_fCastingTime = 1.0f;
	attack_talent_info.m_strLuaOnStart = L"Talent_Start";
	attack_talent_info.m_strLuaOnAct = L"Talent_Act";
	attack_talent_info.m_strLuaOnFinish = L"Talent_Finish";
	attack_talent_info.m_strLuaOnCancel = L"Talent_Cancel";

	vec3 pos = vec3(1000.0f, 1000.0f, 0.0f);
	vec3 dir = vec3(0.0f,1.0f,0.0f);

	{
		GEntityPlayer* pAttackerPlayer = GUTHelper::NewEntityPlayer(m_pField, pos);
		pAttackerPlayer->doUseTalent(&attack_talent_info);

		CHECK_EQUAL(WLUA->GetVar<int>("START"), 1);
		pAttackerPlayer->doCancelTalent();
		pAttackerPlayer->GetModuleCombat()->GetTalent()->Update(0.1f);
		CHECK_EQUAL(WLUA->GetVar<int>("FINISH"), 0); // ȣ����� ����
		CHECK_EQUAL(WLUA->GetVar<int>("CANCEL"), 4);
	}

	gsys.pScriptManager->Fini();
}


TEST_FIXTURE(TestTalentFixture, TestTalentLuaEvents_ForNPC)
{
	gsys.pScriptManager->Init();

	if (!WRUNSTRING(WLUA, 
		"LuaName = {}\n"
		"function LuaName:Talent_Start(Actor, Target)\n"
		"	START = 1\n"
		"end\n"
		"function LuaName:Talent_Act(Actor, Target)\n"
		"	ACT = 2\n"
		"end\n"
		"function LuaName:Talent_Finish(Actor, Target)\n"
		"	FINISH = 3\n"
		"end\n"
		"function LuaName:Talent_Cancel(Actor, Target)\n"
		"	CANCEL = 4\n"
		"end\n"
		))	
	{ 
		CHECK(FALSE && "WRunString Failed.");	 
		return; 
	}

	GTalentInfo attack_talent_info;
	m_Helper.SetMeleeTalentInfo(&attack_talent_info, 1);
	attack_talent_info.m_strLuaOnStart = L"Talent_Start";
	attack_talent_info.m_strLuaOnAct = L"Talent_Act";
	attack_talent_info.m_strLuaOnFinish = L"Talent_Finish";
	attack_talent_info.m_strLuaOnCancel = L"Talent_Cancel";

	vec3 pos = vec3(1000.0f, 1000.0f, 0.0f);
	vec3 dir = vec3(0.0f,1.0f,0.0f);

	{
		m_NPCInfo.strNameInLua = L"LuaName";
		GEntityNPC* pAttackerPlayer = m_pField->SpawnTestNPC(&m_NPCInfo);
		pAttackerPlayer->doUseTalent(&attack_talent_info);

		CHECK_EQUAL(WLUA->GetVar<int>("START"), 1);
		pAttackerPlayer->doCancelTalent();
		pAttackerPlayer->GetModuleCombat()->GetTalent()->Update(0.1f);
		CHECK_EQUAL(WLUA->GetVar<int>("ACT"), 2);
	}

	gsys.pScriptManager->Fini();
}

TEST_FIXTURE(TestTalentFixture, TestTalentPostDelay)
{
	const int TALENT_ID = 1;

	// NPC�� ���� �ŷ�Ʈ --
	GTalentInfo* pNewNPCTalentInfo = m_Helper.NewMeleeTalentInfo(TALENT_ID);
	m_Helper.SetTalentDamage(pNewNPCTalentInfo, 10);
	m_Helper.AddSimpleHitInfo(pNewNPCTalentInfo);

	// �� �ŷ�Ʈ ��� �ð� = 2��
	pNewNPCTalentInfo->m_fPostDelay = 1.0f;	// �ŷ�Ʈ ��� �� 1�� ������
	pNewNPCTalentInfo->m_fDurationTime = 1.0f;	// �ŷ�Ʈ ���ð��� 1��
	pNewNPCTalentInfo->m_setNPCID.insert(m_NPCInfo.nID);

	vec3 pos = vec3(1000.0f, 1000.0f, 0.0f);
	vec3 dir = vec3(0.0f,1.0f,0.0f);

	GEntityNPC* pNPC = m_pField->SpawnTestNPC(&m_NPCInfo, pos, dir);
	GEntityPlayer* pPlayer = GUTHelper::NewEntityPlayer(m_pField, pos);
	m_Helper.SetEntityPlayerStatus(pPlayer, 100, 100, 100);

	m_JobRunner.UseTalent(pNPC, NULL, TALENT_ID);
	pNPC->UpdateAI(0.0f);
	m_pField->Update(0.0f);
	CHECK(pPlayer->GetHP() != 100);

	CHECK_EQUAL(pNPC->GetModuleCombat()->GetUsingTalentID(), TALENT_ID);

	pNPC->UpdateAI(0.5f);
	m_pField->Update(0.5f);	// ���� 0.5��
	CHECK_EQUAL(pNPC->GetModuleCombat()->GetUsingTalentID(), TALENT_ID);

	pNPC->UpdateAI(0.5f);
	m_pField->Update(0.5f);	// ���� 1.0��
	pNPC->UpdateAI(0.5f);
	m_pField->Update(0.5f);	// ���� 1.5��

	pNPC->UpdateAI(0.5f);
	m_pField->Update(0.5f);	// ���� 2.0��
	CHECK(NULL == pNPC->GetModuleCombat()->GetTalent());
}


	class FTestTalent_UseAiming : public TestTalentFixture
	{
	public:
		FTestTalent_UseAiming() : TestTalentFixture()
		{
			InitTalentInfo();
			InitEntities();


		}

		void InitEntities()
		{
			npc_pos = vec3(1000.0f, 1000.0f, 0.0f);
			npc_dir = vec3(0.0f,1.0f,0.0f);
			vec3 player_pos = npc_pos;
			player_pos += (npc_dir * 120.0f);

			m_pNPC = m_pField->SpawnTestNPC(&m_NPCInfo, npc_pos, npc_dir);
			m_pPlayer = GUTHelper::NewEntityPlayer(m_pField, player_pos);

			m_Helper.SetEntityPlayerStatus(m_pPlayer, 100, 100, 100);

			m_pNPC->SetThinkable(false);
			m_pNPC->GetHateTable().AddPoint_FoundEnemy(m_pPlayer);
		}

		void InitTalentInfo()
		{
			TALENT_ID = 1;

			// NPC�� ���� �ŷ�Ʈ --
			m_pNewNPCTalentInfo = m_Helper.NewMagicTalentInfo(TALENT_ID);
			m_Helper.SetMagicAreaTalentInfo(m_pNewNPCTalentInfo, TALENT_ID);
			m_Helper.SetTalentDamage(m_pNewNPCTalentInfo, 10);
			m_pNewNPCTalentInfo->m_nTiming = TC_HIT_TALENT;

			// ĸ���� �ٷ� ��
			MCapsule hit_cap = MCapsule(vec3(0.0f, -100.0f, 0.0f), vec3(0.0f, -100.0f, 200.0f), 50.0f);
			CSHitSegment hit_seg;
			hit_seg.m_vCapsules.push_back(hit_cap);
			hit_seg.m_vDoHitCheck.push_back(true);
			hit_seg.m_fCheckTime = 0.0f;

			int id = (int)m_pNewNPCTalentInfo->m_HitInfo.m_vSegments.size() + 1;
			m_pNewNPCTalentInfo->m_HitInfo.m_nID = id;
			m_pNewNPCTalentInfo->m_HitInfo.m_vSegments.push_back(hit_seg);
			m_pNewNPCTalentInfo->m_Resist.m_nDifficulty = 255;	// ���� �Ұ����ϰ� ����
			m_pNewNPCTalentInfo->m_fCastingTime = 1.0f;
			m_pNewNPCTalentInfo->m_fDurationTime = 2.0f;	// �ŷ�Ʈ ���ð��� 1��
			m_pNewNPCTalentInfo->Cooking();
		}

		void InitAlliedTalentInfo()
		{
			m_pNewNPCTalentInfo->m_HitInfo.m_vSegments.clear();
			m_pNewNPCTalentInfo->m_nTiming = TC_USE_TALENT;
			m_pNewNPCTalentInfo->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
			m_pNewNPCTalentInfo->m_EffectInfo.m_nRelation = CSEffectInfo::RELATION_ALLIED;
			m_pNewNPCTalentInfo->m_EffectInfo.m_fRadius = 100.0f;
			m_pNewNPCTalentInfo->m_EffectInfo.m_nLimit = 10;
			m_pNewNPCTalentInfo->Cooking();
		}


		virtual ~FTestTalent_UseAiming()
		{

		}

		int TALENT_ID;
		GTalentInfo* m_pNewNPCTalentInfo;
		GEntityNPC* m_pNPC;
		GEntityPlayer* m_pPlayer;
		vec3 npc_pos;
		vec3 npc_dir;

	};

	TEST_FIXTURE(FTestTalent_UseAiming, TestTalentUseAiming)
	{
		TALENT_TARGET_INFO target_info;
		target_info.uidTarget = m_pPlayer->GetUID();

		// 1�� �õ� -------------------
		m_pNPC->GetModuleCombat()->UseTalent(m_pNewNPCTalentInfo, target_info, false);
		m_pField->Update(0.1f);
		m_pField->Update(0.1f);

		// Casting������ ���� �ŷ�Ʈ�� ������� �ʾҴ�.
		CHECK_EQUAL(m_pPlayer->GetHP(), 100);

		m_pField->Update(0.8f);
		m_pField->Update(0.2f);

	//	pNPC->GetModuleCombat()->ActTalent(target_info);
	//	m_pField->Update(0.0f); // hitter ó��ƽ

		// ���� ĸ�� ������ �±� ������ �÷��̾� �ǰ���
		CHECK(m_pPlayer->GetHP() != 100);


		// 2�� �õ� -------------------
		m_pField->Update(10.0f);
		m_pField->Update(10.0f);
		m_Helper.SetEntityPlayerStatus(m_pPlayer, 100, 100, 100);
		m_pNPC->SetDir(npc_dir);
		CHECK(m_pPlayer->GetHP() == 100);


		// NPC ������ �ƴ� �ٸ� �ʿ� �÷��̾� ��ġ��
		m_pPlayer->SetPos(npc_pos + (vec3(1.0f, 0.0f, 0.0f) * 120.0f));	

		m_pNPC->GetModuleCombat()->UseTalent(m_pNewNPCTalentInfo, target_info, false);
		m_pField->Update(0.5f);
		m_pField->Update(0.5f);
		m_pField->Update(0.2f);
		m_pField->Update(0.2f);

		// NPC�� Use�� ���� ���� �������� ������ �ٲٱ� ������ �ǰ���
		CHECK(m_pPlayer->GetHP() != 100);

		// 3�� �õ� -------------------
		m_pField->Update(10.0f);
		m_pField->Update(10.0f);
		m_Helper.SetEntityPlayerStatus(m_pPlayer, 100, 100, 100);
		m_pNPC->SetDir(npc_dir);
		CHECK(m_pPlayer->GetHP() == 100);


		// NPC ������ �ƴ� �ٸ� �ʿ� �÷��̾� ��ġ��
		m_pPlayer->SetPos(npc_pos + (vec3(1.0f, 0.0f, 0.0f) * 120.0f));	
		// �ŷ�Ʈ ������ Use�ÿ� ���� �ٲ��� �ʵ��� �ٲ�
		m_pNewNPCTalentInfo->m_bUseAiming = false;

		m_pNPC->GetModuleCombat()->UseTalent(m_pNewNPCTalentInfo, target_info, false);
		m_pField->Update(0.5f);
		m_pField->Update(0.5f);
		m_pField->Update(0.2f);
		m_pField->Update(0.2f);

		// NPC�� Use�� ���� ���� �������� ������ �ٲ��� �ʱ� ������ �ǰݴ����� ����
		CHECK_EQUAL(m_pPlayer->GetHP(), 100);
	}

	/// Allied�� ȿ���� ������ ���� ��ȯ�� ���� �ʴ´�.
	TEST_FIXTURE(FTestTalent_UseAiming, TestTalentUseAiming_TargetAllied)
	{
		InitAlliedTalentInfo();

		TALENT_TARGET_INFO target_info;
		target_info.uidTarget = m_pNPC->GetUID();

		CHECK(npc_dir.x == m_pNPC->GetDir().x);
		CHECK(npc_dir.y == m_pNPC->GetDir().y);
		CHECK(npc_dir.z == m_pNPC->GetDir().z);

		// �ŷ�Ʈ ��� -------------------
		m_pNPC->GetModuleCombat()->UseTalent(m_pNewNPCTalentInfo, target_info, false);
		m_pField->Update(0.2f);

		// Casting������ ���� �ŷ�Ʈ�� ������� �ʾҴ�.
		CHECK_EQUAL(m_pPlayer->GetHP(), 100);

		m_pField->Update(1.0f);

		CHECK(npc_dir.x == m_pNPC->GetDir().x);
		CHECK(npc_dir.y == m_pNPC->GetDir().y);
		CHECK(npc_dir.z == m_pNPC->GetDir().z);

	}

TEST_FIXTURE(TestTalentFixture, TestTalentHitFilterMF)
{
	// �÷��̾ ���� �ŷ�Ʈ
	GTalentInfo* pPlayerTalentInfo = m_Helper.NewMeleeTalentInfo(1);
	m_Helper.SetTalentDamage(pPlayerTalentInfo, 1);		// ������ 1�� ����
	m_Helper.AddSimpleHitInfo(pPlayerTalentInfo, 0.0f);	// �ٷ� �����ϴ� �ŷ�Ʈ��

	pPlayerTalentInfo->m_MotionFactorGroup.var[MF_BEATEN].nValue = 100;
	pPlayerTalentInfo->m_MotionFactorGroup.var[MF_BEATEN].nWeight = 10000;


	GTalentInfo* pPlayerTalentInfo2 = m_Helper.NewMeleeTalentInfo(2);
	m_Helper.SetTalentDamage(pPlayerTalentInfo2, 1);		// ������ 1�� ����
	m_Helper.AddSimpleHitInfo(pPlayerTalentInfo2, 0.0f);	// �ٷ� �����ϴ� �ŷ�Ʈ��

	pPlayerTalentInfo2->m_MotionFactorGroup.var[MF_STUN].nValue = 100;
	pPlayerTalentInfo2->m_MotionFactorGroup.var[MF_STUN].nWeight = 1000;

	// 2��° �ŷ�Ʈ�� BEATEN�� ������ ������ �ȵȴ�.
	pPlayerTalentInfo2->m_HitFilter.bMotionFactor[MF_BEATEN] = false;


	vec3 pos = vec3(1000.0f, 1000.0f, 0.0f);
	vec3 dir = vec3(0.0f,1.0f,0.0f);

	GEntityNPC* pNPC = m_pField->SpawnTestNPC(&m_NPCInfo, pos, dir);
	GEntityPlayer* pAttackerPlayer = GUTHelper::NewEntityPlayer(m_pField, pos);

	m_Helper.SetEntityPlayerStatus(pAttackerPlayer, 100, 100, 100);

	pAttackerPlayer->GetModuleCombat()->UseTalent(pPlayerTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
	m_pField->Update(0.1f);

	CHECK_EQUAL(pNPC->GetCurrentMotionFactor(), MF_BEATEN);

	m_pField->Update(1.0f);
	m_pField->Update(1.0f);

	pAttackerPlayer->GetModuleCombat()->UseTalent(pPlayerTalentInfo2, TALENT_TARGET_INFO::Invalid(), false);
	m_pField->Update(0.1f);

	// ���� ������ ������ �ȵǹǷ� ��� BEATEN�̴�.
	CHECK_EQUAL(pNPC->GetCurrentMotionFactor(), MF_BEATEN);
}


TEST_FIXTURE(TestTalentFixture, TestTalentHitFilterAltitude)
{
	// �÷��̾ ���� �ŷ�Ʈ
	GTalentInfo* pPlayerTalentInfo = m_Helper.NewMeleeTalentInfo(1);
	m_Helper.SetTalentDamage(pPlayerTalentInfo, 1);		// ������ 1�� ����
	m_Helper.AddSimpleHitInfo(pPlayerTalentInfo, 0.0f);	// �ٷ� �����ϴ� �ŷ�Ʈ��

	pPlayerTalentInfo->m_MotionFactorGroup.var[MF_BEATEN].nValue = 100;
	pPlayerTalentInfo->m_MotionFactorGroup.var[MF_BEATEN].nWeight = 10000;

	// npc�� �� �ִ�.
	m_NPCInfo.nAltitude = AT_FLOATING;

	// �� �ŷ�Ʈ�� ���߿� �ִ� NPC���� ������ �ȵȴ�.
	pPlayerTalentInfo->m_HitFilter.bAltitude[AT_FLOATING] = false;

	vec3 pos = vec3(1000.0f, 1000.0f, 0.0f);
	vec3 dir = vec3(0.0f,1.0f,0.0f);

	GEntityNPC* pNPC = m_pField->SpawnTestNPC(&m_NPCInfo, pos, dir);
	GEntityPlayer* pAttackerPlayer = GUTHelper::NewEntityPlayer(m_pField, pos);

	m_Helper.SetEntityPlayerStatus(pAttackerPlayer, 100, 100, 100);

	pAttackerPlayer->GetModuleCombat()->UseTalent(pPlayerTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
	m_pField->Update(0.1f);

	CHECK_EQUAL(pNPC->GetCurrentMotionFactor(), MF_NONE);

}

TEST_FIXTURE(TestTalentFixture, DontRouteGainEffectWhenNoModified)
{
	// �÷��̾ ���� �ŷ�Ʈ
	GTalentInfo* pPlayerTalentInfo = m_Helper.NewMeleeTalentInfo(1);
	pPlayerTalentInfo->m_InstantModifier.nSTA.nMod += 10;
	m_Helper.AddSimpleHitInfo(pPlayerTalentInfo, 0.0f);	// �ٷ� �����ϴ� �ŷ�Ʈ��
	pPlayerTalentInfo->Cooking();

	vec3 pos = vec3(1000.0f, 1000.0f, 0.0f);
	vec3 dir = vec3(0.0f,1.0f,0.0f);

	GEntityNPC* pNPC = m_pField->SpawnTestNPC(&m_NPCInfo, pos, dir);
	GEntityPlayer* pAttackerPlayer = GUTHelper::NewEntityPlayer(m_pField, pos);
	MockLink* pLink1 = NewLink(pAttackerPlayer);

	m_Helper.SetEntityPlayerStatus(pAttackerPlayer, 100, 100, 100);
	CHECK_EQUAL(pNPC->GetHP(), pNPC->GetMaxHP());

	TALENT_TARGET_INFO target_info;
	target_info.uidTarget = pAttackerPlayer->GetUID();
	pNPC->GetModuleCombat()->UseTalent(pPlayerTalentInfo, target_info, true);
	m_pField->Update(0.1f);
	
	CHECK(pLink1->IsExistCommand(MC_TALENT_INSTANT_EFFECT_GAIN) == true);

	// set to no-changed mod
	pPlayerTalentInfo->m_InstantModifier.nSTA.nMod = 0;
	pPlayerTalentInfo->Cooking();

	pLink1->ResetCommands();
	pNPC->GetModuleCombat()->UseTalent(pPlayerTalentInfo, target_info, true);
	m_pField->Update(0.1f);

	CHECK(pLink1->IsExistCommand(MC_TALENT_INSTANT_EFFECT_GAIN) == false);
}


TEST_FIXTURE(TestTalentFixture, TestNPCDirectionWhenUseTalent)
{
	const int TALENT_ID = 1;

	// NPC�� ���� �ŷ�Ʈ --
	GTalentInfo* pNewNPCTalentInfo = m_Helper.NewMagicTalentInfo(TALENT_ID);
	m_Helper.SetMagicAreaTalentInfo(pNewNPCTalentInfo, TALENT_ID);
	m_Helper.SetTalentDamage(pNewNPCTalentInfo, 10);
	pNewNPCTalentInfo->m_setNPCID.insert(m_NPCInfo.nID);


	// ĸ���� �ٷ� ��
	MCapsule hit_cap = MCapsule(vec3(0.0f, -100.0f, 0.0f), vec3(0.0f, -100.0f, 200.0f), 50.0f);
	CSHitSegment hit_seg;
	hit_seg.m_vCapsules.push_back(hit_cap);
	hit_seg.m_vDoHitCheck.push_back(true);
	hit_seg.m_fCheckTime = 0.0f;

	int id = (int)pNewNPCTalentInfo->m_HitInfo.m_vSegments.size() + 1;
	pNewNPCTalentInfo->m_HitInfo.m_nID = id;
	pNewNPCTalentInfo->m_HitInfo.m_vSegments.push_back(hit_seg);
	
	pNewNPCTalentInfo->m_fCastingTime = 1.0f;
	pNewNPCTalentInfo->m_fDurationTime = 2.0f;
	pNewNPCTalentInfo->m_fRange = 100.0f; // �����Ÿ� 1����
	
	vec3 npc_pos = vec3(1000.0f, 1000.0f, 0.0f);
	vec3 npc_dir = vec3(0,-1,0);		// NPC�� ó������ -y ������ ���� ����.
	vec3 player_pos = npc_pos;
	player_pos += (vec3(0,1,0) * 120.0f);	// �׷����� PC�� +y����


	GEntityNPC* pNPC = m_pField->SpawnTestNPC(&m_NPCInfo, npc_pos, npc_dir);
	GEntityPlayer* pPlayer = GUTHelper::NewEntityPlayer(m_pField, player_pos);
	MUID uidPlayer = pPlayer->GetUID();
	MUID uidNPC = pNPC->GetUID();

	m_Helper.SetEntityPlayerStatus(pPlayer, 100, 100, 100);

	TALENT_TARGET_INFO target_info;
	target_info.uidTarget = pPlayer->GetUID();

	pNPC->GetHateTable().AddPoint_FoundEnemy(pPlayer);

	CHECK_CLOSE(pNPC->GetDir().y, -1.0f, 0.0001f);
	CHECK_CLOSE(pNPC->GetFacingDir().y, -1.0f, 0.0001f);

	USETALENT_PARAM param;
	param.bCheckValidate = true;
	m_JobRunner.UseTalent(pNPC, pPlayer, TALENT_ID, param);

	pNPC->UpdateAI(0.1f);
	pNPC->UpdateAI(0.1f);
	
	// �ŷ�Ʈ�� ��������Ƿ� npc�� Dir, FacingDir ��� +y�������� �ٲ�� �־�� �Ѵ�.
	CHECK_CLOSE(pNPC->GetDir().y, 1.0f, 0.0001f);
	CHECK_CLOSE(pNPC->GetFacingDir().y, 1.0f, 0.0001f);
}

TEST_FIXTURE(TestTalentFixture, PreHitCheck)
{
	float fCheckTime = 10.0f;
	float fRealCheckTime = fCheckTime - GConst::PRE_HIT_CHECK_TIME;

	// �÷��̾ ���� �ŷ�Ʈ
	GTalentInfo* pPlayerTalentInfo = m_Helper.NewMeleeTalentInfo(1);
	m_Helper.AddSimpleHitInfo(pPlayerTalentInfo, fCheckTime);	// �ٷ� �����ϴ� �ŷ�Ʈ��
	pPlayerTalentInfo->Cooking();
	m_Helper.SetTalentDamage(pPlayerTalentInfo, 10);

	vec3 pos = vec3(1000.0f, 1000.0f, 0.0f);
	vec3 dir = vec3(0.0f,1.0f,0.0f);

	GEntityNPC* pNPC = m_pField->SpawnTestNPC(&m_NPCInfo, pos, dir);
	GEntityPlayer* pAttackerPlayer = GUTHelper::NewEntityPlayer(m_pField, pos);
	CHECK_EQUAL(pNPC->GetHP(), pNPC->GetMaxHP());

	TALENT_TARGET_INFO target_info;
	target_info.uidTarget = pAttackerPlayer->GetUID();
	pAttackerPlayer->GetModuleCombat()->UseTalent(pPlayerTalentInfo, target_info, true);
	m_pField->Update(0.0f);

	CHECK_EQUAL(pNPC->GetHP(), pNPC->GetMaxHP());
	
	float fRemainCheckTime = fRealCheckTime;
	m_pField->Update(0.5f);

	CHECK_EQUAL(pNPC->GetHP(), pNPC->GetMaxHP());

	m_pField->Update(fRemainCheckTime-0.5f);

	GActorDamageCalculator	damageCalculator;
	GActorDamageCalculator::Desc desc;
	desc.pAttacker = pAttackerPlayer;
	desc.pVictim = pNPC;
	desc.pTalentInfo = pPlayerTalentInfo;
	desc.DamageInfo = GDamageRangedInfo(pPlayerTalentInfo->m_nMaxDamage, pPlayerTalentInfo->m_nMinDamage);
	desc.HealInfo = GHealRangedInfo(0,0);
	const int nDamage = damageCalculator.CalcDamage(desc).nDamage;

	CHECK_EQUAL(pNPC->GetHP(), pNPC->GetMaxHP()-nDamage);
}

TEST_FIXTURE(TestTalentFixture, TestHitPos)
{
	m_NPCInfo.m_pMeshInfo = new GActorMeshInfo();
	m_NPCInfo.m_pMeshInfo->Init(NULL, 100.0f);

	// �÷��̾ ���� �ŷ�Ʈ
	GTalentInfo* pPlayerTalentInfo = m_Helper.NewMeleeTalentInfo(1);
	pPlayerTalentInfo->m_bHitCapsulePosSync = true;	// ���� ��ġ�� ����
	m_Helper.AddHitInfo(pPlayerTalentInfo, vec3(0.0f, -200.0f, 0.0f), vec3(0.0f, 800.0f, 0.0f), 100.0f, 1.0f);		// �÷��̾� 200 �� ����
	pPlayerTalentInfo->m_fDurationTime = 10.0f;
	pPlayerTalentInfo->Cooking();
	m_Helper.SetTalentDamage(pPlayerTalentInfo, 10);

	vec3 npc_pos = vec3(5000.0f, 1000.0f, 0.0f);
	vec3 npc_dir = vec3(0.0f,1.0f,0.0f);
	vec3 player_pos = vec3(1000.0f, 1000.0f, 0.0f);
	vec3 player_dir = npc_pos - player_pos; player_dir.Normalize();

	GEntityNPC* pNPC = m_pField->SpawnTestNPC(&m_NPCInfo, npc_pos, npc_dir);
	GEntityPlayer* pAttackerPlayer = GUTHelper::NewEntityPlayer(m_pField, player_pos);
	pAttackerPlayer->SetDir(player_dir);

	CHECK_EQUAL(pNPC->GetHP(), pNPC->GetMaxHP());

	TALENT_TARGET_INFO target_info;
	target_info.uidTarget = pAttackerPlayer->GetUID();
	pAttackerPlayer->GetModuleCombat()->UseTalent(pPlayerTalentInfo, target_info, true);

	m_pField->Update(0.5f);
	CHECK_EQUAL(pNPC->GetHP(), pNPC->GetMaxHP());

	pAttackerPlayer->SetPos(npc_pos + vec3(-250.0f, 0.0f, 0.0f));
	m_pField->Update(0.6f);

	// ������ ���� �Ǿ����� üũ
	CHECK(pNPC->GetHP() != pNPC->GetMaxHP());
}

TEST(DurationTime)
{
	DECLWRAPPER_FieldInfoMgr;
	DECLWRAPPER_FieldMgr;
	DECLWRAPPER_TalentMgr;

	MockField*		pField = GUTHelper_Field::DefaultMockField();
	GEntityNPC*		pNPC = GUTHelper_NPC::SpawnNPC(pField);

	GTalentInfo* pNewTalentInfo = test.talent.NewTalentInfo(1);
	pNewTalentInfo->m_nSkillType = ST_EXTRA_ACTIVE;
	pNewTalentInfo->m_fDurationTime = 10.0f;
	pNewTalentInfo->Cooking();

	test.talent.UseTalent(pNPC, pNewTalentInfo);
	CHECK_EQUAL(TALENT_PHASE_ACT, pNPC->GetModuleCombat()->GetTalent()->GetPhase());
	pField->Update(5.0f);
	CHECK_EQUAL(TALENT_PHASE_ACT, pNPC->GetModuleCombat()->GetTalent()->GetPhase());
	pField->Update(5.0f);
	CHECK_EQUAL(TALENT_PHASE_FINISH, pNPC->GetModuleCombat()->GetTalent()->GetPhase());

	pField->Destroy();
}

// SEAL(pyo): ���θ�� �۾� �����߿��� �ӽ÷� �ǰ� ��� �ŷ�Ʈ�� ĵ���ǰ� �����մϴ�.
//
//TEST(IgnoreUseTalentForRunningTalent_PlayerCase)
//{
//	DECLWRAPPER_FieldInfoMgr;
//	DECLWRAPPER_FieldMgr;
//	DECLWRAPPER_TalentMgr;
//
//	MockField*		pField = GUTHelper_Field::DefaultMockField();
//	GEntityPlayer*	pPlayer = test.player.NewEntityPlayer(pField);
//
//	GTalentInfo* pTalent1 = test.talent.NewTalentInfo(1151);
//	pTalent1->m_fDurationTime = 10.0f;
//	pTalent1->Cooking();
//	GTalentInfo* pTalent2 = test.talent.NewTalentInfo(1152);
//	pTalent2->m_fDurationTime = 10.0f;
//	pTalent2->Cooking();
//
//	test.talent.UseTalent(pPlayer, pTalent1, pPlayer->GetUID(), false); // �ŷ�Ʈ ����ϱ����� ���� �ŷ�Ʈ�� ������ ������� ����
//	CHECK_EQUAL(1151, pPlayer->GetModuleCombat()->GetTalent()->GetID());
//
//	// �ŷ�Ʈ1�� �������϶� �ŷ�Ʈ2�� ����ص� ���õȴ�.
//	test.talent.UseTalent(pPlayer, pTalent2, pPlayer->GetUID(), false); // �ŷ�Ʈ ����ϱ����� ���� �ŷ�Ʈ�� ������ ������� ����
//	CHECK_EQUAL(1151, pPlayer->GetModuleCombat()->GetTalent()->GetID());
//
//	pField->Destroy();
//}

TEST(ReplaceUseTalentForRunningTalent_NPCCase)
{
	DECLWRAPPER_FieldInfoMgr;
	DECLWRAPPER_FieldMgr;
	DECLWRAPPER_TalentMgr;

	MockField*		pField = GUTHelper_Field::DefaultMockField();
	GEntityNPC*		pNPC = GUTHelper_NPC::SpawnNPC(pField);

	GTalentInfo* pTalent1 = test.talent.NewTalentInfo(1151);
	pTalent1->m_fDurationTime = 10.0f;
	GTalentInfo* pTalent2 = test.talent.NewTalentInfo(1152);
	pTalent2->m_fDurationTime = 10.0f;

	test.talent.UseTalent(pNPC, pTalent1, pNPC->GetUID(), false); // �ŷ�Ʈ ����ϱ����� ���� �ŷ�Ʈ�� ������ ������� ����
	CHECK_EQUAL(1151, pNPC->GetModuleCombat()->GetTalent()->GetID());

	// �ŷ�Ʈ1�� �������϶� �ŷ�Ʈ2�� ����ϸ� ��ü
	test.talent.UseTalent(pNPC, pTalent2, pNPC->GetUID(), false); // �ŷ�Ʈ ����ϱ����� ���� �ŷ�Ʈ�� ������ ������� ����
	CHECK_EQUAL(1152, pNPC->GetModuleCombat()->GetTalent()->GetID());

	pField->Destroy();
}

TEST(TestTalentRangeChecker)
{
	GTalentRangeChecker talentRangeChecker;

	GTalentInfo talentInfo;
	talentInfo.m_nID = 1;

	vec3 vAttackerPos = vec3(0.0f, 1000.0f, 0.0f);
	vec3 vTargetPos = vec3(0.0f, 2000.0f, 0.0f);
	vec3 vAttackerDir = vTargetPos - vAttackerPos;	vAttackerDir.Normalize();

	float fAttackerRadius = 50.0f;
	float fTargetRadius = 50.0f;

	// ST_MOVE�� ������ ������
	talentInfo.m_nSkillType = ST_SOCIAL;
	CHECK_EQUAL(true, talentRangeChecker.Check(&talentInfo, vAttackerPos, vAttackerDir, fAttackerRadius, vTargetPos, fTargetRadius));

	// ���� �ŷ�Ʈ�� Radius�� ���
	talentInfo.m_nSkillType = ST_MAGIC;
	talentInfo.m_fRange = 2000.0f;

	CHECK_EQUAL(true, talentRangeChecker.Check(&talentInfo, vAttackerPos, vAttackerDir, fAttackerRadius, vTargetPos, fTargetRadius));

	talentInfo.m_fRange = 500.0f;
	CHECK_EQUAL(false, talentRangeChecker.Check(&talentInfo, vAttackerPos, vAttackerDir, fAttackerRadius, vTargetPos, fTargetRadius));

	// mele�� �浹 ĸ���� �Ÿ� üũ

	MCapsule hit_cap = MCapsule(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 100.0f), 600.0f);
	CSHitSegment hit_seg;
	hit_seg.m_vCapsules.push_back(hit_cap);
	hit_seg.m_vDoHitCheck.push_back(true);
	hit_seg.m_fCheckTime = 0.0f;

	talentInfo.m_HitInfo.m_nID = 1;
	talentInfo.m_HitInfo.m_vSegments.push_back(hit_seg);
	talentInfo.Cooking();

	// ���� ĸ�� ���� ��
	talentInfo.m_nSkillType = ST_MELEE;
	CHECK_EQUAL(false, talentRangeChecker.Check(&talentInfo, vAttackerPos, vAttackerDir, fAttackerRadius, vTargetPos, fTargetRadius));

	// ���� ĸ�� ���� ��
	vTargetPos = vec3(0.0f, 1250, 0.0f);
	CHECK_EQUAL(true, talentRangeChecker.Check(&talentInfo, vAttackerPos, vAttackerDir, fAttackerRadius, vTargetPos, fTargetRadius));
}

TEST(Talent_DisableAllTalent)
{
	DECLWRAPPER_FieldInfoMgr;
	DECLWRAPPER_FieldMgr;
	DECLWRAPPER_TalentMgr;
	DECLWRAPPER_BuffMgr;

	MockField*		pField = GUTHelper_Field::DefaultMockField();
	GEntityNPC*		pNPC = GUTHelper_NPC::SpawnNPC(pField);

	GTalentInfo* pTalent = test.talent.NewTalentInfo(1151);
	pTalent->m_fDurationTime = 10.0f;

	CHECK_EQUAL(CR_SUCCESS, pNPC->GetModuleCombat()->UseTalent(pTalent, TALENT_TARGET_INFO(pNPC->GetUID()), true));

	// �ӵ��� ���̰� �׽�Ʈ
	int nBuffID = 123;
	GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
	pBuff->m_nPassiveExtraAttrib = BUFPEA_DISABLE_TALENTS;
	pBuff->m_Condition.nEffectConditionType = TC_BUFF_GAIN;
	test.buff.GainBuffSelf(pNPC, pBuff);
	CHECK_EQUAL(true, pNPC->HasBuff(nBuffID));
	CHECK_EQUAL(CR_FAIL_USE_TALENT_DISABLED, pNPC->GetModuleCombat()->UseTalent(pTalent, TALENT_TARGET_INFO(pNPC->GetUID()), true));

	pField->Destroy();
}

TEST(Talent_RequireMoveSpeed)
{
	DECLWRAPPER_FieldInfoMgr;
	DECLWRAPPER_FieldMgr;
	DECLWRAPPER_TalentMgr;

	MockField*		pField = GUTHelper_Field::DefaultMockField();
	GEntityNPC*		pNPC = GUTHelper_NPC::SpawnNPC(pField);

	GTalentInfo* pTalent = test.talent.NewTalentInfo(1151);
	pTalent->m_fRequireMoveSpeed = 0.7f; // 70% �̻��� �ӵ��������� �ŷ�Ʈ ��밡��
	pTalent->m_fDurationTime = 10.0f;

	pNPC->GetChrInfo()->fSpeed = 100.0f;
	CHECK_EQUAL(CR_SUCCESS, pNPC->GetModuleCombat()->UseTalent(pTalent, TALENT_TARGET_INFO(pNPC->GetUID()), true));

	// �ӵ��� ���̰� �׽�Ʈ
	pNPC->GetChrStatus()->ActorModifier.fMoveSpeed = -0.5f; // MoveSpeed = 50%
	CHECK_EQUAL(CR_FAIL_USE_TALENT_NOTENOUGH_SPEED, pNPC->GetModuleCombat()->UseTalent(pTalent, TALENT_TARGET_INFO(pNPC->GetUID()), true));

	pField->Destroy();
}

TEST(TalentOption_NonCombatOnly)
{
	DECLWRAPPER_FieldInfoMgr;
	DECLWRAPPER_FieldMgr;
	DECLWRAPPER_TalentMgr;

	MockField*		pField = GUTHelper_Field::DefaultMockField();
	GEntityNPC*		pNPC = GUTHelper_NPC::SpawnNPC(pField);

	GTalentInfo* pTalent = test.talent.NewTalentInfo(1151);
	pTalent->m_bNonCombatOnly = true;	// �������߿��� ��밡��
	pTalent->m_fDurationTime = 10.0f;

	// �������߿� SUCCESS
	CHECK_EQUAL(CR_SUCCESS, pNPC->GetModuleCombat()->UseTalent(pTalent, TALENT_TARGET_INFO(pNPC->GetUID()), true));

	// �����߿� FAILED
	pNPC->doAttack(pNPC->GetUID(), 1);
	CHECK_EQUAL(CR_FAIL_USE_TALENT_NONCOMBAT_ONLY, pNPC->GetModuleCombat()->UseTalent(pTalent, TALENT_TARGET_INFO(pNPC->GetUID()), true));

	pField->Destroy();
}

TEST(Talent_CheckRootable)
{
	DECLWRAPPER_FieldInfoMgr;
	DECLWRAPPER_FieldMgr;
	DECLWRAPPER_TalentMgr;
	DECLWRAPPER_BuffMgr;

	MockField*		pField = GUTHelper_Field::DefaultMockField();
	GEntityNPC*		pNPC = GUTHelper_NPC::SpawnNPC(pField);

	GTalentInfo* pTalent = test.talent.NewTalentInfo(1151);
	pTalent->m_fRequireMoveSpeed = 1.0f; // 100% �̻��� �ӵ��������� �ŷ�Ʈ ��밡��
	pTalent->m_fDurationTime = 10.0f;

	pNPC->GetChrInfo()->fSpeed = 100.0f;
	CHECK_EQUAL(CR_SUCCESS, pNPC->GetModuleCombat()->UseTalent(pTalent, TALENT_TARGET_INFO(pNPC->GetUID()), true));

	// �ӵ��� ���̰� �׽�Ʈ
	int nBuffID = 123;
	GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
	pBuff->m_nPassiveExtraAttrib = BUFPEA_ROOT;
	pBuff->m_Condition.nEffectConditionType = TC_BUFF_GAIN;
	test.buff.GainBuffSelf(pNPC, pBuff);
	CHECK_EQUAL(true, pNPC->HasBuff(nBuffID));
	CHECK_EQUAL(CR_FAIL_USE_TALENT_DISABLED, pNPC->GetModuleCombat()->UseTalent(pTalent, TALENT_TARGET_INFO(pNPC->GetUID()), true));

	pField->Destroy();
}

TEST(Talent_BackHit)
{
	DECLWRAPPER_FieldInfoMgr;
	DECLWRAPPER_FieldMgr;
	DECLWRAPPER_TalentMgr;
	DECLWRAPPER_BuffMgr;

	GTestHelpers testHelpers;

	MockField*		pField = GUTHelper_Field::DefaultMockField();
	GEntityNPC*		pNPC = GUTHelper_NPC::SpawnNPC(pField);
	GEntityPlayer*	pPlayer = testHelpers.player.NewPlayer(pField);

	GBuffInfo* pBuff1=test.buff.NewBuffInfo();
	GBuffInfo* pBuff2=test.buff.NewBuffInfo();

	GTalentInfo* pBackHitTalent = test.talent.NewMeleeTalentInfo();
	pBackHitTalent->m_Buff1.nID = pBuff1->m_nID;
	pBuff1->m_fDuration = 10.0f;
	pBackHitTalent->m_Buff1.nEnchantEvent = TC_HIT_TALENT;
	pBackHitTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
	pBackHitTalent->m_Buff1.EffectInfo.m_nRelation = CSEffectInfo::RELATION_ENEMY;	

	GTalentInfo* pTalent = test.talent.NewMeleeTalentInfo();	
	pTalent->m_Buff1.nID = pBuff2->m_nID;
	pBuff2->m_fDuration = 5.0f;
	pTalent->m_Buff1.nEnchantEvent = TC_HIT_TALENT;
	pTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
	pTalent->m_Buff1.EffectInfo.m_nRelation = CSEffectInfo::RELATION_ENEMY;
	pTalent->m_nBackHitTalent = pBackHitTalent->m_nID;

	MCapsule hit_cap = MCapsule(vec3(0.0f, 0.0f, -5000.0f), vec3(0.0f, 0.0f, 5000.0f), 5000.0f);
	CSHitSegment hit_seg;
	hit_seg.m_vCapsules.push_back(hit_cap);
	hit_seg.m_vDoHitCheck.push_back(true);
	hit_seg.m_fCheckTime = 0.0f;
	pTalent->m_HitInfo.m_nID = 1;
	pTalent->m_HitInfo.m_vSegments.push_back(hit_seg);
	pTalent->Cooking();


	pNPC->SetPos(vec3(100.0f, 100.0f, 0.0f));
	pPlayer->SetPos(vec3(100.0f, 99.0f, 0.0f));
	pNPC->SetDir(vec3(0.0f, 100.0f, 0.0f));
	pPlayer->SetDir(vec3(0.0f, 100.0f, 0.0f));
	test.talent.UseTalent(pPlayer, pTalent, pNPC->GetUID(), false);
	CHECK_EQUAL(10.0f, pNPC->GetModuleBuff()->GetBuffRemainTime(pBuff1->m_nID));

	pNPC->GetModuleBuff()->LazyLostAll();
	pNPC->GetModuleBuff()->Update(0.0f);
			
	pNPC->SetPos(vec3(100.0f, 100.0f, 0.0f));
	pPlayer->SetPos(vec3(100.0f, 99.0f, 0.0f));
	pNPC->SetDir(vec3(0.0f, -100.0f, 0.0f));
	pPlayer->SetDir(vec3(0.0f, 100.0f, 0.0f));
	test.talent.UseTalent(pPlayer, pTalent, pNPC->GetUID(), false);
	CHECK_EQUAL(5.0f, pNPC->GetModuleBuff()->GetBuffRemainTime(pBuff2->m_nID));
	
	pField->Destroy();
}

TEST(TalentLineIndex)
{
	DECLWRAPPER_TalentMgr;

	{
		GTalentInfo* pTalentInfo = new GTalentInfo();
		pTalentInfo->m_nID = 1;
		pTalentInfo->m_nTalentLine = 111;
		gmgr.pTalentInfoMgr->Insert(pTalentInfo);
	}
	
	{
		GTalentInfo* pTalentInfo = new GTalentInfo();
		pTalentInfo->m_nID = 2;
		pTalentInfo->m_nTalentLine = 111;
		gmgr.pTalentInfoMgr->Insert(pTalentInfo);
	}

	{
		GTalentInfo* pTalentInfo = new GTalentInfo();
		pTalentInfo->m_nID = 3;
		pTalentInfo->m_nTalentLine = 222;
		gmgr.pTalentInfoMgr->Insert(pTalentInfo);
	}

	vector<int> vecTalents;
	vecTalents = gmgr.pTalentInfoMgr->GetLineTalents(444);
	CHECK_EQUAL(0, vecTalents.size());

	vecTalents = gmgr.pTalentInfoMgr->GetLineTalents(111);
	CHECK_EQUAL(2, vecTalents.size());
	CHECK_EQUAL(1, vecTalents[0]);
	CHECK_EQUAL(2, vecTalents[1]);

	vecTalents = gmgr.pTalentInfoMgr->GetLineTalents(222);
	CHECK_EQUAL(1, vecTalents.size());
	CHECK_EQUAL(3, vecTalents[0]);
}

TEST(DoNotCancelAfterFinished)
{
	GUTHelper helper;
	GTalentInfo talent_info;
	helper.SetMagicTalentInfo(&talent_info, 1);
	talent_info.m_fDurationTime = 1.0f;
	talent_info.m_fCastingTime = 0.0f;
	talent_info.Cooking();

	MockField*	pField = GUTHelper_Field::DefaultMockField();
	GEntityNPC*	pNPC = GUTHelper_NPC::SpawnNPC(pField);

	float fElapsedTime = 0.0f;

	GMagicTalent magic_talent(pNPC, &talent_info);
	magic_talent.Start();

	fElapsedTime+=0.1f;
	magic_talent.Update(fElapsedTime);

	CHECK_EQUAL(magic_talent.GetPhase(), TALENT_PHASE_ACT);

	fElapsedTime+=1.0f;
	CHECK(magic_talent.Update(fElapsedTime) == true);
	magic_talent.UpdateExpired(fElapsedTime);

	CHECK_EQUAL(magic_talent.GetPhase(), TALENT_PHASE_FINISH);
	magic_talent.CancelForce(false);
	CHECK_EQUAL(magic_talent.GetPhase(), TALENT_PHASE_FINISH); // �ٲ��� ����

	pField->Destroy();
}

TEST_FIXTURE(TestTalentFixture, UseLimit_BuffRequirement)
{
	GTalentInfo* pNewTalentInfo = new GTalentInfo();
	m_Helper.SetMagicTargetTalentInfo(pNewTalentInfo, 1);
	pNewTalentInfo->m_InstantModifier.nHP.nMod = 10;		// hp�� 10 �÷���
	pNewTalentInfo->m_RequireBuff.vecExcludeBuffID.push_back(111);
	gmgr.pTalentInfoMgr->Insert(pNewTalentInfo);

	GEntityPlayer* pPlayer1 = GUTHelper::NewEntityPlayer(m_pField, vec3(100,100,0));
	GEntityPlayer* pPlayer2 = GUTHelper::NewEntityPlayer(m_pField, vec3(200,100,0));

	PLAYER_INFO* player_info2 = pPlayer2->GetPlayerInfo();
	player_info2->nMaxHP = 100;
	pPlayer2->SetHP(50);

	CHECK_EQUAL(pPlayer2->GetHP(), 50);

	// Exclude ���� ����
	GBuffInfo* pBuff1=test.buff.NewBuffInfo(111);
	pBuff1->m_fDuration = 4.0f; // 4�ʵڿ� Ǯ��
	test.buff.GainBuffSelf(pPlayer1, pBuff1);
	CHECK_EQUAL(true, pPlayer1->HasBuff(111));

	MUID uidTarget = pPlayer2->GetUID();
	pPlayer1->doUseTalent(1, uidTarget);

	pPlayer1->Update(0.1f);
	pPlayer2->Update(0.1f);

	CHECK_EQUAL(pPlayer2->GetHP(), 50);
}

TEST_FIXTURE(TestTalentFixture, ModEffect)
{
	GEntityPlayer* pPlayer1 = GUTHelper::NewEntityPlayer(m_pField, vec3(100,100,0));

	GTalentInfo* pTalentInfo = test.talent.NewTalentInfo(1);
	pTalentInfo->m_ActorModifier.nENMax.nMod = 100;	
	pTalentInfo->m_ActorModifier.nENMax.fPer = 0.5f;	

	CHECK_EQUAL(pPlayer1->GetMaxEN(), 450);

	test.talent.Train(pPlayer1, pTalentInfo);
	CHECK_EQUAL(pPlayer1->GetMaxEN(), 825);

	test.talent.UntrainAll(pPlayer1);
	CHECK_EQUAL(pPlayer1->GetMaxEN(), 450);
}


TEST_FIXTURE(TestTalentFixture, ForOnlyGMTalent)
{
	GTalentInfo* pNewTalentInfo = new GTalentInfo();
	m_Helper.SetMagicTargetTalentInfo(pNewTalentInfo, 1);
	gmgr.pTalentInfoMgr->Insert(pNewTalentInfo);

	// �Ϲ������� ����
	GEntityPlayer* pPlayer = GUTHelper::NewEntityPlayer(m_pField, vec3(100,100,0));
	pPlayer->GetPlayerInfo()->nPlayerGrade = PLAYER_GRADE_NORMAL;

	pNewTalentInfo->m_bUseOnlyGM = false;
	CHECK_EQUAL(true, pPlayer->doUseTalent(pNewTalentInfo));

	pNewTalentInfo->m_bUseOnlyGM = true;
	CHECK_EQUAL(false, pPlayer->doUseTalent(pNewTalentInfo));

	// �ٽ� ������������ �ǵ���
	pPlayer->GetPlayerInfo()->nPlayerGrade = PLAYER_GRADE_DEVELOPER;
}

TEST_FIXTURE(TestTalentFixture, HasHeal_TalentEffect)
{
	GTalentInfo* pTalent = new GTalentInfo();
	pTalent->Cooking();
	CHECK_EQUAL(false, pTalent->HasHeal());
	pTalent->m_nMaxHeal = 1;
	pTalent->Cooking();
	CHECK_EQUAL(true, pTalent->HasHeal());
}

TEST_FIXTURE(TestTalentFixture, HasHeal_InnerBuffEffect)
{
	GBuffInfo* pBuff = test.buff.NewBuffInfo();
	GTalentInfo* pTalent = test.buff.NewBuffTalent(pBuff);
	pTalent->Cooking();
	CHECK_EQUAL(false, pTalent->HasHeal());
	pBuff->m_nMaxHeal = 1;
	pTalent->Cooking();
	CHECK_EQUAL(true, pTalent->HasHeal());
}

TEST_FIXTURE(TestTalentFixture, HasHeal_InnerTriggeredBuffEffect)
{
	GBuffInfo* pBuff = test.buff.NewBuffInfo();
	GBuffInfo* pTriggeredBuff = test.buff.NewBuffInfo();
	pBuff->m_TriggeredBuff.nBuffID = pTriggeredBuff->m_nID;
	GTalentInfo* pTalent = test.buff.NewBuffTalent(pBuff);
	pTalent->Cooking();
	CHECK_EQUAL(false, pTalent->HasHeal());
	pTriggeredBuff->m_nMaxHeal = 1;
	pTalent->Cooking();
	CHECK_EQUAL(true, pTalent->HasHeal());
}

} // SUITE

