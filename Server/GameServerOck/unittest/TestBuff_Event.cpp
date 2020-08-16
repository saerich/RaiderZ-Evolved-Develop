#include "stdafx.h"
#include "GTestForward.h"
#include "GModuleBuff.h"
#include "GModuleCombat.h"
#include "GServer.h"
#include "GFieldMgr.h"
#include "FBasePlayer.h"

SUITE(Buff_Event)
{
	struct FBuff : public FBasePlayer
	{
		FBuff()
		{
			m_pField = GUTHelper_Field::DefaultMockField();
			m_pPlayer = NewEntityPlayer(m_pField, vec3(100,100,0));
			m_pLinkTester = test.network.NewLink(m_pPlayer);
			swprintf_s(m_pPlayer->GetPlayerInfo()->szName, L"Tester");

			gsys.pServer->FrameCounterClear();
			SetIgnoreCommonCommand(m_pLinkTester);
		}

		~FBuff()
		{
			m_pField->Destroy();
			test.network.ClearLinks();
			m_pField->Destroy();
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


		MockField*		m_pField;
		GEntityPlayer*	m_pPlayer;
		MockLink*		m_pLinkTester;

		DECLWRAPPER_FieldInfoMgr;
		DECLWRAPPER_FieldMgr;
		DECLWRAPPER_SimpleCombatCalc;
		DECLWRAPPER_BuffMgr;
	};

	TEST_FIXTURE(FBuff, OnHit)
	{
		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nEffectConditionType = TC_HIT_ME;	// �ڽ��� �¾�����
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;		// �����ο���
		pBuff->m_InstantModifier.nHP.nMod = 10;			// HP�� 10ȸ��
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;;

		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// ���� �̺�Ʈ
		m_pPlayer->SetHP(10);
		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, m_pPlayer->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(m_pPlayer->GetHP(), 10-1+10);	// 1���� �԰�, 10ȸ����
	}

	TEST_FIXTURE(FBuff, OnHitEnemy)
	{
		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nEffectConditionType = TC_HIT_ENEMY;	// �ڽ��� ����������		
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;	// �����ο���
		pBuff->m_InstantModifier.nHP.nMod = 10;			// HP�� 10ȸ��
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;;

		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// ���� �̺�Ʈ
		m_pPlayer->SetHP(10);
		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, m_pPlayer->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		test.talent.UseTalent(m_pPlayer, p1DmgTalent, pDefender->GetUID());
		CHECK_EQUAL(m_pPlayer->GetHP(), 10+10);	// 10ȸ����
	}

	TEST_FIXTURE(FBuff, OnCriHit)
	{
		DECLWRAPPER_CriticalCombatCalc;

		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nEffectConditionType = TC_CRITICAL_HIT_ME;	// �ڽ��� ġ��Ÿ�� �¾�����
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;		// �����ο���
		pBuff->m_InstantModifier.nHP.nMod = 10;			// HP�� 10ȸ��
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;;

		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// ���� �̺�Ʈ
		m_pPlayer->SetHP(10);
		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, m_pPlayer->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(m_pPlayer->GetHP(), 10-1+10);	// 1���� �԰�, 10ȸ����
	}

	TEST_FIXTURE(FBuff, OnCriHitEnemy)
	{
		DECLWRAPPER_CriticalCombatCalc;

		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nEffectConditionType = TC_CRITICAL_HIT_ENEMY;	// ������ ġ��Ÿ�� ������
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;;		// �����ο���
		pBuff->m_InstantModifier.nHP.nMod = 10;			// HP�� 10ȸ��
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;;

		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// ���� �̺�Ʈ
		m_pPlayer->SetHP(10);
		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, m_pPlayer->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		test.talent.UseTalent(m_pPlayer, p1DmgTalent, pDefender->GetUID());
		CHECK_EQUAL(m_pPlayer->GetHP(), 10+10);	// 10ȸ����
	}

	TEST_FIXTURE(FBuff, OnAvoid)
	{
		DECLWRAPPER_MissCombatCalc;

		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nEffectConditionType = TC_MISS_ME;	// ���� ��������
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;;		// �����ο���
		pBuff->m_InstantModifier.nHP.nMod = 10;			// HP�� 10ȸ��
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;;

		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// ���� �̺�Ʈ
		m_pPlayer->SetHP(10);
		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, m_pPlayer->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(m_pPlayer->GetHP(), 10+10);
	}

	TEST_FIXTURE(FBuff, OnAvoidEnemy)
	{
		DECLWRAPPER_MissCombatCalc;

		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nEffectConditionType = TC_MISS_ENEMY;	// ��밡 ��������
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;;		// �����ο���
		pBuff->m_InstantModifier.nHP.nMod = 10;			// HP�� 10ȸ��
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;;

		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// ���� �̺�Ʈ
		m_pPlayer->SetHP(10);
		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, m_pPlayer->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		test.talent.UseTalent(m_pPlayer, p1DmgTalent, pDefender->GetUID());
		CHECK_EQUAL(m_pPlayer->GetHP(), 10+10);	// 10ȸ����
	}

	TEST_FIXTURE(FBuff, OnGuard)
	{
		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nEffectConditionType = TC_GUARD_ME;	// ���� ��������
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;;		// �����ο���
		pBuff->m_InstantModifier.nHP.nMod = 10;			// HP�� 10ȸ��
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;;

		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		m_pPlayer->GetModuleCombat()->CancelTalent(); // �ŷ�Ʈ �Ϸ�� �� �����ϱ�

		// ���� �̺�Ʈ
		m_pPlayer->SetHP(10);
		GEntityPlayer* pAttacker = NewEntityPlayer(m_pField, m_pPlayer->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		test.item.EquipNewShield(m_pPlayer);
		m_pPlayer->SetLevel(100);
		test.talent.Guard(m_pPlayer);
		test.talent.UseTalent(pAttacker, p1DmgTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(m_pPlayer->GetHP(), 10+10);	// 10ȸ����
	}

	TEST_FIXTURE(FBuff, OnGuardEnemy)
	{
		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.nEffectConditionType = TC_GUARD_ENEMY;	// ��밡 ��������
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;;		// �����ο���
		pBuff->m_InstantModifier.nHP.nMod = 10;			// HP�� 10ȸ��
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;;

		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// ���� �̺�Ʈ
		m_pPlayer->SetHP(10);
		GEntityPlayer* pDefender = NewEntityPlayer(m_pField, m_pPlayer->GetPos()+vec3(10,10,0));
		GTalentInfo* p1DmgTalent = test.talent.MakeDamageTalent(1);
		test.item.EquipNewShield(pDefender);
		pDefender->SetLevel(100);
		test.talent.Guard(pDefender);
		test.talent.UseTalent(m_pPlayer, p1DmgTalent, pDefender->GetUID());
		CHECK_EQUAL(m_pPlayer->GetHP(), 10+10);	// 10ȸ����
	}

	TEST_FIXTURE(FBuff, OnDispelled)
	{
		int nBuffID = 1123;	// ���� ��å �������� �Ϲ� ����ID�� 1000�� ���İ� �Ǿ��մϴ�.
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_nType = BUFT_BUFF;
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_DISPELLED;
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;;		// �����ο���
		pBuff->m_InstantModifier.nHP.nMod = 10;			// HP�� 10ȸ��
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;;

		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		m_pPlayer->SetHP(10);
		CHECK_EQUAL(m_pPlayer->GetHP(), 10);
		// ����
		GTalentInfo* pActiveTalent = test.talent.NewTalentInfo();
		test.talent.SetExtraActiveTalentInfo(pActiveTalent, 1, TEAT_DISPEL_BUFF);		
		pActiveTalent->m_nExtraActiveParam1 = 10;	// �����Ŀ� 10�������� ����
		pActiveTalent->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
		pActiveTalent->m_EffectInfo.m_nRelation = CSEffectInfo::RELATION_ALL;
		test.talent.UseTalent(m_pPlayer, pActiveTalent, m_pPlayer->GetUID());
		m_pPlayer->GetModuleBuff()->Update(0.1f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetHP(), 10+10);	// 10ȸ����
	}

	TEST_FIXTURE(FBuff, OnExpired)
	{
		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_nType = BUFT_BUFF;
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_EXPIRED;
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;;		// �����ο���
		pBuff->m_InstantModifier.nHP.nMod = 10;			// HP�� 10ȸ��
		pBuff->m_fDuration = 10.0f;
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;;
		
		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		m_pPlayer->SetHP(10);
		CHECK_EQUAL(m_pPlayer->GetHP(), 10);
		m_pPlayer->GetModuleBuff()->Update(pBuff->m_fDuration);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetHP(), 10+10);	// 10ȸ����
	}

	TEST_FIXTURE(FBuff, OnGone)
	{
		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_nType = BUFT_BUFF;
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_GONE;
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;;		// �����ο���
		pBuff->m_InstantModifier.nHP.nMod = 10;			// HP�� 10ȸ��
		pBuff->m_fDuration = 10.0f;
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;;

		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		m_pPlayer->SetHP(10);
		CHECK_EQUAL(m_pPlayer->GetHP(), 10);
		m_pPlayer->GetModuleBuff()->Update(pBuff->m_fDuration);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetHP(), 10+10);	// 10ȸ����
	}

	TEST_FIXTURE(FBuff, OnGainNPeriod)
	{
		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_nType = BUFT_BUFF;
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_GAIN_N_PERIOD;
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;;		// �����ο���
		pBuff->m_InstantModifier.nHP.nMod = 10;			// HP�� 10ȸ��
		pBuff->m_fDuration = 10.0f;
		pBuff->m_fPeriod = 2.0f;
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;;

		m_pPlayer->SetHP(10);
		CHECK_EQUAL(m_pPlayer->GetHP(), 10);
		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		m_pPlayer->GetModuleBuff()->Update(pBuff->m_fDuration);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetHP(), 10+50+10);	// �ʱ� ��ġ + �ݺ� ȿ�� + �ʱ� ���� ȿ��
	}

	TEST_FIXTURE(FBuff, OnPeriod)
	{
		int nBuffID = 123;
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_nType = BUFT_BUFF;
		pBuff->m_Condition.nEffectConditionType = TC_BUFF_PERIOD;
		pBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;;		// �����ο���
		pBuff->m_InstantModifier.nHP.nMod = 10;			// HP�� 10ȸ��
		pBuff->m_fDuration = 10.0f;
		pBuff->m_fPeriod = 2.0f;
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;;

		m_pPlayer->SetHP(10);
		CHECK_EQUAL(m_pPlayer->GetHP(), 10);
		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		m_pPlayer->GetModuleBuff()->Update(pBuff->m_fDuration);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		CHECK_EQUAL(m_pPlayer->GetHP(), 10+50);	// �ʱ� ��ġ + �ݺ� ȿ��
	}
}
