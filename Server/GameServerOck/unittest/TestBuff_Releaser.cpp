#include "stdafx.h"
#include "GTestForward.h"
#include "GModuleBuff.h"
#include "GServer.h"
#include "GFieldMgr.h"
#include "GPlayerObjectManager.h"
#include "FBasePlayer.h"

SUITE(Buff_Releaser)
{
	struct FBuff : public FBasePlayer
	{
		FBuff()
		{
			m_pField = GUTHelper_Field::DefaultMockField();
			m_pPlayer = NewEntityPlayer(m_pField, vec3(100,100,0));
			m_pPlayerHelper = NewEntityPlayer(m_pField, m_pPlayer->GetPos()+vec3(10,10,0));

			m_p1DmgTalent = test.talent.MakeDamageTalent(1);
		}

		virtual ~FBuff()
		{
			m_pField->Destroy();
			test.network.ClearLinks();
			m_pField->Destroy();
		}

		GBuffInfo* GainBuff(GEntityActor* pActor, int nBuffID, TALENT_CONDITION eReleaseConditionType, int nReleaseParam = 0, vector<TALENT_CONDITION>& vecTalentCondition = vector<TALENT_CONDITION>())
		{
			CHECK_EQUAL(false, pActor->HasBuff(nBuffID));
			GBuffInfo* pBuff = test.buff.NewBuffInfo(nBuffID);
			pBuff->m_Condition.infoRelease.nType = eReleaseConditionType;
			pBuff->m_Condition.infoRelease.nParam = nReleaseParam;

			for (int i = 0; i < (int)vecTalentCondition.size(); ++i)
			{
				pBuff->m_Condition.infoRelease.vecAdditionType.push_back(vecTalentCondition[i]);
			}

			test.buff.GainBuffSelf(pActor, pBuff);
			CHECK_EQUAL(true, pActor->HasBuff(nBuffID));
			return pBuff;
		}
		void Hit(GEntityActor* pAttacker, GEntityActor* pTarget)
		{
			test.talent.UseTalent(pAttacker, m_p1DmgTalent, pTarget->GetUID());
		}

		MockField*		m_pField;
		GEntityPlayer*	m_pPlayer;
		GEntityPlayer*	m_pPlayerHelper;
		GTalentInfo*	m_p1DmgTalent;

		DECLWRAPPER_FieldInfoMgr;
		DECLWRAPPER_FieldMgr;
		DECLWRAPPER_BuffMgr;
		GTestMgrWrapper<GPlayerObjectManager>	m_PlayerObjectManager;
	};

	TEST_FIXTURE(FBuff, OnHit)
	{
		int nBuffID = 123;
		GainBuff(m_pPlayer, nBuffID, TC_HIT_ME);

		// ���� ����
		Hit(m_pPlayerHelper, m_pPlayer);
		
		// ���� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnHitN)
	{
		int nBuffID = 123;
		int nReleaseParam = 2; // ������ Ǯ�� ������ ���ڰ� : 2�� �¾ƾ� Ǯ��
		GainBuff(m_pPlayer, nBuffID, TC_HIT_ME_N, nReleaseParam);

		// ���� ����
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� ���� �� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// �ѹ� �� ����
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnHitN_ReleaseParamIsZero)
	{
		int nBuffID = 123;
		int nReleaseParam = 0; // ������ Ǯ�� ������ ���ڰ� : 0�� ��� ������ ���õ�
		GainBuff(m_pPlayer, nBuffID, TC_HIT_ME_N, nReleaseParam);

		// ���� ����
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� ���� �� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// �ѹ� �� ����
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� �� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnHitN_OnlyCriticalHit)
	{
		DECLWRAPPER_CriticalCombatCalc;	///< �׻� ũ��Ƽ�� ��Ʈ�� �ߵ��� ��

		int nBuffID = 123;
		int nReleaseParam = 2; // ������ Ǯ�� ������ ���ڰ� : 2�� �¾ƾ� Ǯ��
		GainBuff(m_pPlayer, nBuffID, TC_HIT_ME_N, nReleaseParam);

		// ���� ���� - �� �� ����.
		Hit(m_pPlayerHelper, m_pPlayer);
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnHitEnemy)
	{
		int nBuffID = 123;
		GainBuff(m_pPlayer, nBuffID, TC_HIT_ENEMY);

		// ���� ��
		Hit(m_pPlayer, m_pPlayerHelper);

		// ���� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnCriHit)
	{
		DECLWRAPPER_CriticalCombatCalc;

		int nBuffID = 123;
		GainBuff(m_pPlayer, nBuffID, TC_CRITICAL_HIT_ME);

		// ���� ����
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnCriHitN)
	{
		DECLWRAPPER_CriticalCombatCalc;

		int nBuffID = 123;
		int nReleaseParam = 2; // ������ Ǯ�� ������ ���ڰ� : 2�� �¾ƾ� Ǯ��
		GainBuff(m_pPlayer, nBuffID, TC_CRIHIT_ME_N, nReleaseParam);

		// ���� ����
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� ���� �� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// �ѹ� �� ����
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnCriHitN_ReleaseParamIsZero)
	{
		DECLWRAPPER_CriticalCombatCalc;

		int nBuffID = 123;
		int nReleaseParam = 0; // ������ Ǯ�� ������ ���ڰ� : 0�� ��� ������ ���õ�
		GainBuff(m_pPlayer, nBuffID, TC_CRIHIT_ME_N, nReleaseParam);

		// ���� ����
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� ���� �� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// �ѹ� �� ����
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� �� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnCriHitEnemy)
	{
		DECLWRAPPER_CriticalCombatCalc;

		int nBuffID = 123;
		GainBuff(m_pPlayer, nBuffID, TC_CRITICAL_HIT_ENEMY);

		// ���� ����
		Hit(m_pPlayer, m_pPlayerHelper);

		// ���� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnMiss)
	{
		DECLWRAPPER_MissCombatCalc;

		int nBuffID = 123;
		GainBuff(m_pPlayer, nBuffID, TC_MISS_ME);

		// ���� ����
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnMissEnemy)
	{
		DECLWRAPPER_MissCombatCalc;

		int nBuffID = 123;
		GainBuff(m_pPlayer, nBuffID, TC_MISS_ENEMY);

		// ���� ��
		Hit(m_pPlayer, m_pPlayerHelper);

		// ���� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnGuard)
	{
		int nBuffID = 123;
		GainBuff(m_pPlayer, nBuffID, TC_GUARD_ME);

		// ���� ����
		test.talent.Guard(m_pPlayer);
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnPerfectGuard)
	{
		int nBuffID = 123;
		GainBuff(m_pPlayer, nBuffID, TC_PERFECT_GUARD_ME);

		// ���� ����
		test.talent.Guard(m_pPlayer);
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� ��Ǯ�� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// ���� �����ϰ� Ǯ��
		m_pPlayer->SetLevel(100);
		m_pPlayerHelper->SetLevel(1);
		test.item.EquipNewShield(m_pPlayer);
		test.talent.Guard(m_pPlayer);
		
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� ��Ǯ�� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnGuardEnemy)
	{
		int nBuffID = 123;
		GainBuff(m_pPlayer, nBuffID, TC_GUARD_ENEMY);

		// ���� ��
		test.talent.Guard(m_pPlayerHelper);
		Hit(m_pPlayer, m_pPlayerHelper);

		// ���� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, MultiCondition)
	{
		int nBuffID = 123;
		vector<TALENT_CONDITION> vecTalentDonditions;
		vecTalentDonditions.push_back(TC_GUARD_ME);
		vecTalentDonditions.push_back(TC_MISS_ENEMY);

		GBuffInfo* pBuff = GainBuff(m_pPlayer, nBuffID, TC_HIT_ME, 0, vecTalentDonditions);

		// ���� ����
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));

		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// TC_GUARD_ME �������� ������ --------------------------

		// ���� ����
		test.talent.Guard(m_pPlayer);
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));

		// TC_AVOID_ENEMY �������� ������ --------------------------
		DECLWRAPPER_MissCombatCalc;

		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// ���� ����
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnUseTalent)
	{
		DECLWRAPPER_CriticalCombatCalc;

		int nBuffID = 123;
		GainBuff(m_pPlayer, nBuffID, TC_USE_TALENT);

		// ���� ��
		Hit(m_pPlayer, m_pPlayerHelper);

		// ���� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnFinishTalent)
	{
		DECLWRAPPER_CriticalCombatCalc;

		int nBuffID = 123;
		GainBuff(m_pPlayer, nBuffID, TC_FINISH_TALENT);

		// ���� ��
		Hit(m_pPlayer, m_pPlayerHelper);

		// ���� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnBuffHit)
	{
		int nBuffID = 123;
		GainBuff(m_pPlayer, nBuffID, TC_BUFF_HIT);

		// �ֱ��� ���� ��������
		GBuffInfo* pDmgBuff=test.buff.NewBuffInfo();
		pDmgBuff->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pDmgBuff->m_fDuration = 10.0f;
		pDmgBuff->m_fPeriod = 2.0f;
		GTalentInfo* pBuffTalent=test.buff.NewBuffTalent(pDmgBuff);
		pBuffTalent->m_Buff1.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
		pDmgBuff->m_nMinDamage = 10;
		pDmgBuff->m_nMaxDamage = 10;
		test.talent.UseTalent(m_pPlayer, pBuffTalent, m_pPlayer->GetUID());
		m_pField->Update(0.0f);

		// ���� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}
	
	TEST_FIXTURE(FBuff, OnDamage)
	{
		int nBuffID = 123;
		int nConditionParam = 2; // ������ Ǯ�� ������ ���ڰ� : 2 �������� ������  Ǯ��
		GainBuff(m_pPlayer, nBuffID, TC_DAMAGE, nConditionParam);

		// ���� ���� - ������ 1¥�� ����
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� ���� �� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// �ѹ� �� ���� - ������ 1¥�� ����
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnDamage_ReleaseParamIsZero)
	{
		int nBuffID = 123;
		int nConditionParam = 0; // ������ Ǯ�� ������ ���ڰ� : 0�� ��� ������ ���õ�
		GainBuff(m_pPlayer, nBuffID, TC_DAMAGE, nConditionParam);

		// ���� ���� - ������ 1¥�� ����
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� ���� �� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		// �ѹ� �� ���� - ������ 1¥�� ����
		Hit(m_pPlayerHelper, m_pPlayer);

		// ���� �� Ǯ��
		m_pPlayer->GetModuleBuff()->Update(0.0f);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnAffected)
	{
		int nBuffID = 123;
		int nTriggeredBuffID = 321;

		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.infoRelease.nType = TC_AFFECTED;

		/// Triggered Buff ����
		pBuff->m_TriggeredBuff.nBuffID = nTriggeredBuffID;
		pBuff->m_TriggeredBuff.nTiming = TC_BUFF_GAIN_N_PERIOD;
		pBuff->m_TriggeredBuff.infoEffect.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_TriggeredBuff.infoEffect.m_fRadius = 10*100; // 10m
		pBuff->m_TriggeredBuff.infoEffect.m_nRelation = CSEffectInfo::RELATION_ENEMY;
		pBuff->m_fDuration = 10.0f;
		pBuff->m_fPeriod = 2.0f;

		GBuffInfo* pTriggeredBuff=test.buff.NewBuffInfo(nTriggeredBuffID);
		pTriggeredBuff->m_fDuration = 20.0f;
		pTriggeredBuff->m_fPeriod = 2.0f;

		/// Monster ����
		GEntityNPC* pMonster = test.npc.SpawnNPC(m_pField);
		/// Monster�� Tester �� ����� �����. (Radius, 10m �̳���)
		pMonster->SetPos(vec3(100,100,0));
		CHECK(10*100 > m_pPlayer->DistanceTo(pMonster));

		/// Monster �� Triggered Buff�� ������ ���� ����
		CHECK_EQUAL(false, pMonster->HasBuff(nTriggeredBuffID));

		/// Tester ���� Buff�� ��
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		test.buff.GainBuffSelf(m_pPlayer, pBuff);

		/// Tester �� Buff�� ���ڸ��� Monster ���� Affected�� �Ͼ Buff�� ����
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));

		m_pField->Update(0.1f);

		/// Monster �� Triggered Buff�� ����
		CHECK_EQUAL(true, pMonster->HasBuff(nTriggeredBuffID));
	}

	TEST_FIXTURE(FBuff, OnAffected_Approach)
	{
		int nBuffID = 123;
		int nTriggeredBuffID = 321;

		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.infoRelease.nType = TC_AFFECTED;

		/// Triggered Buff ����
		pBuff->m_TriggeredBuff.nBuffID = nTriggeredBuffID;
		pBuff->m_TriggeredBuff.nTiming = TC_BUFF_GAIN_N_PERIOD;
		pBuff->m_TriggeredBuff.infoEffect.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_TriggeredBuff.infoEffect.m_fRadius = 10*100; // 10m
		pBuff->m_TriggeredBuff.infoEffect.m_nRelation = CSEffectInfo::RELATION_ENEMY;
		pBuff->m_fDuration = 10.0f;
		pBuff->m_fPeriod = 2.0f;

		GBuffInfo* pTriggeredBuff=test.buff.NewBuffInfo(nTriggeredBuffID);
		pTriggeredBuff->m_fDuration = 20.0f;
		pTriggeredBuff->m_fPeriod = 2.0f;

		/// Monster ����
		GEntityNPC* pMonster = test.npc.SpawnNPC(m_pField);

		/// Monster�� Tester �� 10m �̻� ����������
		pMonster->SetPos(vec3(20000,20000,0));
		CHECK(10*100 < m_pPlayer->DistanceTo(pMonster));

		/// Tester ���� Buff ��
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		test.buff.GainBuffSelf(m_pPlayer, pBuff);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		m_pField->Update(pBuff->m_fPeriod);

		/// Monster �� Triggered Buff�� ������ ���� ����
		CHECK_EQUAL(false, pMonster->HasBuff(nTriggeredBuffID));

		/// Monster�� Tester �� �������. (Radius, 10m �̳���)
		pMonster->SetPos(vec3(100,100,0));
		CHECK(10*100 > m_pPlayer->DistanceTo(pMonster));

		m_pField->Update(pBuff->m_fPeriod);
		m_pField->Update(0.1f);

		/// Monster �� Triggered Buff�� ����
		CHECK_EQUAL(true, pMonster->HasBuff(nTriggeredBuffID));

		/// Tester �� Buff�� ����
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnAffected_NotRelease_NoTarget)
	{
		int nBuffID = 123;
		int nTriggeredBuffID = 321;

		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_Condition.infoRelease.nType = TC_AFFECTED;

		/// Triggered Buff ����
		pBuff->m_TriggeredBuff.nBuffID = nTriggeredBuffID;
		pBuff->m_TriggeredBuff.nTiming = TC_BUFF_GAIN_N_PERIOD;
		pBuff->m_TriggeredBuff.infoEffect.m_nPoint = CSEffectInfo::POINT_SELF;
		pBuff->m_TriggeredBuff.infoEffect.m_fRadius = 100; // 1m
		pBuff->m_TriggeredBuff.infoEffect.m_nRelation = CSEffectInfo::RELATION_ENEMY;
		pBuff->m_fDuration = 10.0f;
		pBuff->m_fPeriod = 2.0f;

		GBuffInfo* pTriggeredBuff=test.buff.NewBuffInfo(nTriggeredBuffID);
		pTriggeredBuff->m_fDuration = 20.0f;
		pTriggeredBuff->m_fPeriod = 2.0f;
		
		/// Monster ����
		GEntityNPC* pMonster = test.npc.SpawnNPC(m_pField);

		/// Tester ���� Buff ��
		test.buff.GainBuffSelf(m_pPlayer, pBuff);

		/// Monster�� Helper �� 10m �̻� ����������
		pMonster->SetPos(vec3(20000,20000,0));
		CHECK(10*100 < m_pPlayer->DistanceTo(pMonster));
		
		/// Monster �� Triggered Buff�� ������ ���� ����
		CHECK_EQUAL(false, pMonster->HasBuff(nTriggeredBuffID));

		m_pField->Update(pBuff->m_fPeriod);

		/// Monster �� Triggered Buff�� ���� ����. (�ʹ� ��)
		CHECK_EQUAL(false, pMonster->HasBuff(nTriggeredBuffID));

		/// Tester �� Buff�� ���� ����
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		m_pField->Update(pBuff->m_fPeriod);
		m_pField->Update(pBuff->m_fPeriod);
		m_pField->Update(pBuff->m_fPeriod);

		/// ���� Tester �� Buff�� ���� ����
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));

		m_pField->Update(pBuff->m_fPeriod);

		/// ���� Tester �� Buff�� ���� ����
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

	TEST_FIXTURE(FBuff, OnMaxStacked)
	{
		int nBuffID = 123;
		GBuffInfo* pInfo=test.buff.NewBuffInfo(nBuffID);
		pInfo->m_nStackType = BUFFSTACK_STACK_UPDATE;
		pInfo->m_nStackMaxCount = 3;
		pInfo->m_Condition.infoRelease.nType = TC_BUFF_MAX_STACKED;

		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
		test.buff.GainBuffSelf(m_pPlayer, pInfo);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		test.buff.GainBuffSelf(m_pPlayer, pInfo);
		CHECK_EQUAL(true, m_pPlayer->HasBuff(nBuffID));
		test.buff.GainBuffSelf(m_pPlayer, pInfo);
		m_pPlayer->GetModuleBuff()->Update(0.1f);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(nBuffID));
	}

}