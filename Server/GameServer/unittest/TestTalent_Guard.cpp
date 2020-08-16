#include "StdAfx.h"
#include "GTestForward.h"
#include "GModuleCombat.h"
#include "GModuleBuff.h"
#include "GTalent.h"
#include "GItemManager.h"
#include "GActorObserver.h"
#include "GServer.h"
#include "GFieldMgr.h"
#include "FBasePlayer.h"
#include "GuardEffector.h"
#include "GDefenseFactorCalculator.h"

class MockActorObserver_ForEvent2 : public GActorObserver, public TestCallCounter
{
public:
	// ��� �̺�Ʈ
	virtual void OnGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo) override { 
		__super::OnGuard(pOwner, pAttacker, pAttackTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
	// ���� ��� �̺�Ʈ
	virtual void OnPerfectGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo) override  { 
		__super::OnPerfectGuard(pOwner, pAttacker, pAttackTalentInfo);
		INCREASE_CALLCOUNTER; 
	}

	// �κ� ��� �̺�Ʈ
	virtual void OnPartialGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo) override  { 
		__super::OnPartialGuard(pOwner, pAttacker, pAttackTalentInfo);
		INCREASE_CALLCOUNTER; 
	}

	// �κ� ��� �̺�Ʈ
	virtual void OnAbsoluteGuard(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo) override  { 
		__super::OnAbsoluteGuard(pOwner, pAttacker, pAttackTalentInfo);
		INCREASE_CALLCOUNTER; 
	}
};


SUITE(Talent_Guard)
{
	struct FTalent : public FBasePlayer
	{
		FTalent()
		{
			m_pField = GUTHelper_Field::DefaultMockField();
			m_pTester = NewEntityPlayer(m_pField, vec3(100,100,0));
			m_pLinkTester = test.network.NewLink(m_pTester);
			swprintf_s(m_pTester->GetPlayerInfo()->szName, L"Tester");

			gsys.pServer->FrameCounterClear();
			SetIgnoreCommonCommand(m_pLinkTester);

			m_pTester->AttachObserver(&m_ActorObserver);
		}

		~FTalent()
		{
			m_pTester->DetachObserver(&m_ActorObserver);
			m_pField->Destroy();
			test.network.ClearLinks();
		}

		void BeginCommandRecord()
		{
			m_pLinkTester->ResetCommands();
		}

		void SetIgnoreCommonCommand(MockLink* pLink)
		{
			pLink->AddIgnoreID(MC_ACTION_USE_TALENT);
			pLink->AddIgnoreID(MC_ACTION_TALENT_HIT);
			pLink->AddIgnoreID(MC_ACTION_TALENT_HIT_NO_MF);
		}

		void GainAbsoluteGuardBuff(GEntityActor* pEffect)
		{
			DCHECK(pEffect);

			// ������ ���� ���
			int nBuffID = 123;
			GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
			pBuff->m_nPassiveExtraAttrib = BUFPEA_ABSOLUTE_GUARD;
			CSBuffEnchantInfo infoEnchant;
			infoEnchant.nID = nBuffID;
			infoEnchant.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
			infoEnchant.EffectInfo.m_nRelation = CSEffectInfo::RELATION_PARTY;
			test.buff.GainBuffSelf(pEffect, pBuff, &infoEnchant);
		}

		GTalentInfo* MakeMissileMagicTalent(float fCastingTime, float fDurationTime, float fEventTime, float fProjectileSpeed)
		{
			GTalentInfo* pTalentInfo = test.talent.NewTalentInfo();
			// �ŷ�Ʈ �ð� --------------------------
			pTalentInfo->m_fCastingTime = fCastingTime;
			pTalentInfo->m_fDurationTime = fDurationTime;
			// �߻�ü -------------------------------
			pTalentInfo->m_nSkillType = ST_MAGIC;
			pTalentInfo->m_nTiming = TC_HIT_ENEMY;
			pTalentInfo->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;

			// �߻�ü �̺�Ʈ �߰�
			CSTalentEventInfo talentEventInfo;
			talentEventInfo.m_nEvent = TE_LAUNCH_PROJECTILE;
			talentEventInfo.m_fTime1 = fEventTime;
			talentEventInfo.m_ProjectileInfo.m_nType = TPT_MISSILE;
			talentEventInfo.m_ProjectileInfo.m_fSpeed = fProjectileSpeed;	// �߻�ü�ӵ�: (x)m/s
			pTalentInfo->m_Events.m_vecActEvents.push_back(talentEventInfo);

			pTalentInfo->Cooking();
			return pTalentInfo;
		}


		MockField*		m_pField;
		GEntityPlayer*	m_pTester;
		MockLink*		m_pLinkTester;
		MockActorObserver_ForEvent2	m_ActorObserver;
		
		DECLWRAPPER_FieldInfoMgr;
		DECLWRAPPER_FieldMgr;
		DECLWRAPPER_TalentMgr;
		DECLWRAPPER_ItemMgr;
		DECLWRAPPER_SimpleCombatCalc;
	};

	TEST_FIXTURE(FTalent, GuardFail_SetWrongDirection)
	{
		GTestMgrWrapper<GNPCInfoMgr>();

		test.buff.NewBuffInfo(BUFF_FOCUS_ID[TFT_COUNTER]);
		test.focus.GainItemForCounter(m_pTester);

		// �����ڰ� ����� ���ʿ� ��ġ
		GEntityPlayer* pAttacker = 
			NewEntityPlayer(m_pField, m_pTester->GetPos()-vec3(-10,0,0));

		// ����ڰ� ������ �ٶ�
		m_pTester->SetDir(vec3(0,1,0));
		int nGuardTalentID = SUnitTestUtil::NewID();
		GTalentInfo* pGuardTalent = test.talent.NewGuardTalentInfo(nGuardTalentID);
		pGuardTalent->m_nGuardDirection = GUARD_DIRECTION_FRONT;
		m_pTester->doGuard(nGuardTalentID);

		// �����ڰ� ����
		m_pTester->SetHP(m_pTester->GetMaxHP());
		GTalentInfo* pAtkTalent = test.talent.MakeDamageTalent(100);	// 100�� ���ظ� �ִ� �ŷ�Ʈ
		test.talent.UseTalent(pAttacker, pAtkTalent, m_pTester->GetUID());

		// ���� Ȯ��
		CHECK_EQUAL(m_pTester->GetHP(), m_pTester->GetMaxHP()-100);	// ���� ����
	}

	TEST_FIXTURE(FTalent, PerfectGuard)
	{
		test.item.EquipNewShield(m_pTester);

		// �����ڰ� ����� ���ʿ� ��ġ
		GEntityPlayer* pAttacker = 
			NewEntityPlayer(m_pField, m_pTester->GetPos()-vec3(-10,0,0));

		// ������/����� ������ ���� (��������=5), ����Ȯ��=100%, ���غ�����=0.0
		m_pTester->SetLevel(100);
		pAttacker->SetLevel(1);

		// ����ڰ� �����ڸ� �ٶ󺸸� ���
		m_pTester->SetDirForwardToActor(pAttacker);
		int nGuardTalentID = SUnitTestUtil::NewID();
		GTalentInfo* pGuardTalent = test.talent.NewGuardTalentInfo(nGuardTalentID);
		pGuardTalent->m_nGuardDirection = GUARD_DIRECTION_FRONT;
		m_pTester->doGuard(nGuardTalentID);

		// �����ڰ� ����
		m_pTester->SetHP(m_pTester->GetMaxHP());
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnPerfectGuard"));
		GTalentInfo* pAtkTalent = test.talent.MakeDamageTalent(100);	// 100�� ���ظ� �ִ� �ŷ�Ʈ
		test.talent.UseTalent(pAttacker, pAtkTalent, m_pTester->GetUID());

		// ���� Ȯ��
		CHECK_EQUAL(m_pTester->GetHP(), m_pTester->GetMaxHP());	// ���� ���� ����
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnPerfectGuard"));
	}


	TEST_FIXTURE(FTalent, PerfectGuardBuff)
	{
		test.item.EquipNewShield(m_pTester);		

		GEntityPlayer* pAttacker = 
			NewEntityPlayer(m_pField, m_pTester->GetPos()-vec3(-10,0,0));

		int nMeleeAttackTalentID = SUnitTestUtil::NewID();
		GTalentInfo* pAttackTalentInfo = test.talent.NewMeleeTalentInfo(nMeleeAttackTalentID);

		pAttackTalentInfo-> m_nTiming = TC_HIT_TALENT;
		pAttackTalentInfo->m_Buff1.nID = 40005;

		pAttacker->OnGuardEnemy(m_pTester, pAttackTalentInfo);
		CHECK(!m_pTester->GetModuleBuff()->IsGained(40005));
	}


	TEST_FIXTURE(FTalent, PartialGuard)
	{
		// �����ڰ� ����� ���ʿ� ��ġ
		GEntityPlayer* pAttacker = 
			NewEntityPlayer(m_pField, m_pTester->GetPos()-vec3(-10,0,0));

		// ����ڰ� ������ �������� 2���� (��������=4), ����Ȯ��=100%, ���غ�����=0.1
		m_pTester->SetLevel(15);
		pAttacker->SetLevel(13);
		m_pTester->SetHP(m_pTester->GetMaxHP());

		// ����ڰ� �����ڸ� �ٶ󺸸� ���
		test.item.EquipNewWeapon(m_pTester, WEAPON_1H_SLASH);
		m_pTester->SetDirForwardToActor(pAttacker);
		int nGuardTalentID = SUnitTestUtil::NewID();
		GTalentInfo* pGuardTalent = test.talent.NewGuardTalentInfo(nGuardTalentID);
		pGuardTalent->m_nGuardDirection = GUARD_DIRECTION_FRONT;
		m_pTester->doGuard(nGuardTalentID);

		// �����ڰ� ����
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnPartialGuard"));
		GTalentInfo* pAtkTalent = test.talent.MakeDamageTalent(100);	// 100�� ���ظ� �ִ� �ŷ�Ʈ
		test.talent.UseTalent(pAttacker, pAtkTalent, m_pTester->GetUID());

		GDefenseFactorCalculator defenseFactorCalc;
		float fDefenseFactor = defenseFactorCalc.CalcDefenseFactor(pAttacker, m_pTester, pAtkTalent);

		// 50%���ظ� ����
		int nDamage = (int)((100 / 2) * fDefenseFactor);

		// ���� Ȯ��
		CHECK_EQUAL(m_pTester->GetHP(), m_pTester->GetMaxHP()-nDamage);	// 50%�� ���ظ� ���� (�Ѽհ����� ���� 50%��)
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnPartialGuard"));
	}

	TEST_FIXTURE(FTalent, PartialGuard_WhenPerfectGuardFailed)
	{
		test.item.EquipNewShield(m_pTester);

		// �����ڰ� ����� ���ʿ� ��ġ
		GEntityNPC* pAttacker =
			GUTHelper_NPC::SpawnNPC(m_pField, INVALID_ID, m_pTester->GetPos()-vec3(-10,0,0));

		// ������/����� ������ ���� (��������=10), ����Ȯ��=0%, ���غ�����=0.4
		m_pTester->SetLevel(1);
		pAttacker->SetLevel(99);
		int8 nOldGrade = pAttacker->GetNPCInfo()->nGrade;
		pAttacker->GetNPCInfo()->nGrade = 100; // �ִ� ���

		// ����ڰ� �����ڸ� �ٶ󺸸� ���
		m_pTester->SetDirForwardToActor(pAttacker);
		int nGuardTalentID = SUnitTestUtil::NewID();
		GTalentInfo* pGuardTalent = test.talent.NewGuardTalentInfo(nGuardTalentID);
		pGuardTalent->m_nGuardDirection = GUARD_DIRECTION_FRONT;
		m_pTester->doGuard(nGuardTalentID);

		// �����ڰ� ����
		GTalentInfo* pAtkTalent = test.talent.MakeDamageTalent(100);	// 100�� ���ظ� �ִ� �ŷ�Ʈ
		test.talent.UseTalent(pAttacker, pAtkTalent, m_pTester->GetUID());

		GuardEffector guard_effector;
		CHECK(GT_PARTIAL == guard_effector.GetGuardLevel(m_pTester, pAttacker, pAtkTalent));

		pAttacker->GetNPCInfo()->nGrade = nOldGrade;
	}

	TEST_FIXTURE(FTalent, NPCGuardType_Partial)
	{
		// �����ڰ� ����� �����ʿ� ��ġ
		GEntityNPC* pDefender =
			GUTHelper_NPC::SpawnNPC(m_pField);

		pDefender->SetPos(m_pTester->GetPos()-vec3(10,0,0));
		pDefender->GetNPCInfo()->nDefaultGuardType = GT_PARTIAL; // �κ� �� ��
		MockActorObserver_ForEvent2	ActorObserver;
		pDefender->AttachObserver(&ActorObserver);

		// ����ڰ� �����ڸ� �ٶ󺸸� ���
		pDefender->SetDirForwardToActor(m_pTester);
		int nGuardTalentID = SUnitTestUtil::NewID();
		GTalentInfo* pGuardTalent = test.talent.NewGuardTalentInfo(nGuardTalentID);
		pGuardTalent->m_nGuardDirection = GUARD_DIRECTION_FRONT;
		pDefender->doGuard(nGuardTalentID);

		// �����ڰ� ����
		CHECK_EQUAL(0, ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnPartialGuard"));
		GTalentInfo* pAtkTalent = test.talent.MakeDamageTalent(100);	// 100�� ���ظ� �ִ� �ŷ�Ʈ
		test.talent.UseTalent(m_pTester, pAtkTalent, pDefender->GetUID());

		// ���� Ȯ��
		CHECK_EQUAL(1, ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnPartialGuard"));
		pDefender->DetachObserver(&ActorObserver);
	}

	TEST_FIXTURE(FTalent, NPCGuardType_Perfect)
	{
		// �����ڰ� ����� �����ʿ� ��ġ
		GEntityNPC* pDefender =
			GUTHelper_NPC::SpawnNPC(m_pField);
		pDefender->SetPos(m_pTester->GetPos()-vec3(10,0,0));
		pDefender->GetNPCInfo()->nDefaultGuardType = GT_PERFECT; // ���� �� ��
		MockActorObserver_ForEvent2	ActorObserver;
		pDefender->AttachObserver(&ActorObserver);

		// ����ڰ� �����ڸ� �ٶ󺸸� ���
		pDefender->SetDirForwardToActor(m_pTester);
		int nGuardTalentID = SUnitTestUtil::NewID();
		GTalentInfo* pGuardTalent = test.talent.NewGuardTalentInfo(nGuardTalentID);
		pGuardTalent->m_nGuardDirection = GUARD_DIRECTION_FRONT;
		pDefender->doGuard(nGuardTalentID);

		// ������/����� ������ ���� (��������=5), ����Ȯ��=100%, ���غ�����=0.0
		pDefender->SetLevel(100);
		m_pTester->SetLevel(1);

		// �����ڰ� ����
		CHECK_EQUAL(0, ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnPerfectGuard"));
		GTalentInfo* pAtkTalent = test.talent.MakeDamageTalent(100);	// 100�� ���ظ� �ִ� �ŷ�Ʈ
		test.talent.UseTalent(m_pTester, pAtkTalent, pDefender->GetUID());

		// ���� Ȯ��
		CHECK_EQUAL(1, ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnPerfectGuard"));
		pDefender->DetachObserver(&ActorObserver);
	}

	TEST_FIXTURE(FTalent, GuardAttr_PerfectGuardOnly)
	{
		// �����ڰ� ����� �����ʿ� ��ġ
		GEntityNPC* pDefender =
			GUTHelper_NPC::SpawnNPC(m_pField);
		pDefender->SetPos(m_pTester->GetPos()-vec3(10,0,0));
		pDefender->GetNPCInfo()->nDefaultGuardType = GT_PARTIAL; // �κ� �� ��
		MockActorObserver_ForEvent2	ActorObserver;
		pDefender->AttachObserver(&ActorObserver);

		// ����ڰ� �����ڸ� �ٶ󺸸� ���
		pDefender->SetDirForwardToActor(m_pTester);
		int nGuardTalentID = SUnitTestUtil::NewID();
		GTalentInfo* pGuardTalent = test.talent.NewGuardTalentInfo(nGuardTalentID);
		pGuardTalent->m_nGuardDirection = GUARD_DIRECTION_FRONT;
		pDefender->doGuard(nGuardTalentID);

		// �����ڰ� ����
		CHECK_EQUAL(0, ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnPerfectGuard"));
		CHECK_EQUAL(0, ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnPartialGuard"));
		GTalentInfo* pAtkTalent = test.talent.MakeDamageTalent(10);	// 10�� ���ظ� �ִ� �ŷ�Ʈ
		pAtkTalent->m_bPerfectGuardOnly = true;
		test.talent.UseTalent(m_pTester, pAtkTalent, pDefender->GetUID());
		CHECK_EQUAL(0, ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnPerfectGuard"));
		CHECK_EQUAL(0, ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnPartialGuard"));

		pDefender->DetachObserver(&ActorObserver);
	}

	TEST_FIXTURE(FTalent, GuardAttr_PartaialGuardHit)
	{
		// �����ڰ� ����� �����ʿ� ��ġ
		GEntityNPC* pDefender =
			GUTHelper_NPC::SpawnNPC(m_pField);
		pDefender->SetPos(m_pTester->GetPos()-vec3(10,0,0));
		pDefender->GetNPCInfo()->nDefaultGuardType = GT_PERFECT; // ���� �� ��
		MockActorObserver_ForEvent2	ActorObserver;
		pDefender->AttachObserver(&ActorObserver);

		// ����ڰ� �����ڸ� �ٶ󺸸� ���
		pDefender->SetDirForwardToActor(m_pTester);
		int nGuardTalentID = SUnitTestUtil::NewID();
		GTalentInfo* pGuardTalent = test.talent.NewGuardTalentInfo(nGuardTalentID);
		pGuardTalent->m_nGuardDirection = GUARD_DIRECTION_FRONT;
		pDefender->doGuard(nGuardTalentID);

		// �����ڰ� ����, �κ� ���� ó����
		CHECK_EQUAL(0, ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnPerfectGuard"));
		CHECK_EQUAL(0, ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnPartialGuard"));
		GTalentInfo* pAtkTalent = test.talent.MakeDamageTalent(10);	// 10�� ���ظ� �ִ� �ŷ�Ʈ
		pAtkTalent->m_bPartialGuardHit = true;
		test.talent.UseTalent(m_pTester, pAtkTalent, pDefender->GetUID());
		CHECK_EQUAL(0, ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnPerfectGuard"));
		CHECK_EQUAL(1, ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnPartialGuard"));

		pDefender->DetachObserver(&ActorObserver);
	}

	TEST_FIXTURE(FTalent, AbsoluteGuard)
	{
		test.item.EquipNewShield(m_pTester);

		// ������ ���� ���
		GainAbsoluteGuardBuff(m_pTester);

		// �����ڰ� ����� ���ʿ� ��ġ
		GEntityPlayer* pAttacker = 
			NewEntityPlayer(m_pField, m_pTester->GetPos()-vec3(-10,0,0));

		// ������/����� ������ ���� (��������=5), ����Ȯ��=100%, ���غ�����=0.0
		m_pTester->SetLevel(1);
		pAttacker->SetLevel(100);

		// ����ڰ� �����ڸ� �ٶ󺸸� ���
		m_pTester->SetDirForwardToActor(pAttacker);
		int nGuardTalentID = SUnitTestUtil::NewID();
		GTalentInfo* pGuardTalent = test.talent.NewGuardTalentInfo(nGuardTalentID);
		pGuardTalent->m_nGuardDirection = GUARD_DIRECTION_FRONT;
		m_pTester->doGuard(nGuardTalentID);

		// �����ڰ� ����
		m_pTester->SetHP(m_pTester->GetMaxHP());
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnAbsoluteGuard"));
		GTalentInfo* pAtkTalent = test.talent.MakeDamageTalent(100);	// 100�� ���ظ� �ִ� �ŷ�Ʈ
		test.talent.UseTalent(pAttacker, pAtkTalent, m_pTester->GetUID());

		// ���� Ȯ��
		CHECK_EQUAL(m_pTester->GetHP(), m_pTester->GetMaxHP());	// ���� ���� ����
		CHECK_EQUAL(1, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnAbsoluteGuard"));
	}

	TEST_FIXTURE(FTalent, AbsoluteGuard_NeedShield)
	{
		// ������ ���� ���
		GainAbsoluteGuardBuff(m_pTester);

		// �����ڰ� ����� ���ʿ� ��ġ
		GEntityPlayer* pAttacker = 
			NewEntityPlayer(m_pField, m_pTester->GetPos()-vec3(-10,0,0));

		// ������/����� ������ ���� (��������=5), ����Ȯ��=100%, ���غ�����=0.0
		m_pTester->SetLevel(1);
		pAttacker->SetLevel(100);

		// ����ڰ� �����ڸ� �ٶ󺸸� ���
		m_pTester->SetDirForwardToActor(pAttacker);
		int nGuardTalentID = SUnitTestUtil::NewID();
		GTalentInfo* pGuardTalent = test.talent.NewGuardTalentInfo(nGuardTalentID);
		pGuardTalent->m_nGuardDirection = GUARD_DIRECTION_FRONT;
		m_pTester->doGuard(nGuardTalentID);

		// �����ڰ� ����
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnAbsoluteGuard"));
		GTalentInfo* pAtkTalent = test.talent.MakeDamageTalent(100);	// 100�� ���ظ� �ִ� �ŷ�Ʈ
		test.talent.UseTalent(pAttacker, pAtkTalent, m_pTester->GetUID());

		// ���� Ȯ��
		CHECK(m_pTester->GetHP() != m_pTester->GetMaxHP());	// ���� ����
		CHECK_EQUAL(0, m_ActorObserver.GetCallCounter(L"MockActorObserver_ForEvent2::OnAbsoluteGuard"));
	}

	TEST_FIXTURE(FTalent, AbsoluteGuard_GuardMagic)
	{
		test.item.EquipNewShield(m_pTester);

		// ������ ���� ���
		GainAbsoluteGuardBuff(m_pTester);

		// �����ڰ� ����� ���ʿ� ��ġ
		GEntityNPC* pAttacker = GUTHelper_NPC::SpawnNPC(m_pField, 777, m_pTester->GetPos()-vec3(-10,0,0));
		pAttacker->GetNPCStatus().nAA = NAAT_FACTION;

		// ������/����� ������ ���� (��������=5), ����Ȯ��=100%, ���غ�����=0.0
		m_pTester->SetLevel(1);
		pAttacker->SetLevel(100);

		// ����ڰ� �����ڸ� �ٶ󺸸� ���
		m_pTester->SetDirForwardToActor(pAttacker);
		int nGuardTalentID = SUnitTestUtil::NewID();
		GTalentInfo* pGuardTalent = test.talent.NewGuardTalentInfo(nGuardTalentID);
		pGuardTalent->m_nGuardDirection = GUARD_DIRECTION_FRONT;
		m_pTester->doGuard(nGuardTalentID);

		// �����ڰ� ����
		m_pTester->SetHP(m_pTester->GetMaxHP());
		float fCastingTime = 5.0f;
		float fDurationTime = 10.0f;
		float fEventTime = 3.0f;
		float fProjectileSpeed = 200.0f;
		GTalentInfo* pTalentInfo = MakeMissileMagicTalent(fCastingTime, fDurationTime, fEventTime, fProjectileSpeed);
		test.talent.SetTalentDamage(pTalentInfo, 100);
		test.talent.UseTalent(pAttacker, pTalentInfo, m_pTester->GetUID());
		GTalent* pTalent = pAttacker->GetModuleCombat()->GetTalent();
		TVALID(pTalent);
		CHECK_EQUAL(TALENT_PHASE_PREPARE, pTalent->GetPhase());
		pTalent->Update(fCastingTime);	// ĳ���ýð���ŭ ���
		pTalent->ActTalent(m_pTester->GetUID());	
		CHECK_EQUAL(TALENT_PHASE_ACT, pTalent->GetPhase());
		pTalent->Update(fEventTime);	// �߻�ü �߻�ð� ���
		pAttacker->GetModuleCombat()->Update(10.0f);	// �߻�ü �浹�ð� ���

		// ���� Ȯ��
		CHECK_EQUAL(m_pTester->GetHP(), m_pTester->GetMaxHP());	// ���� ���� ����
	}
}

