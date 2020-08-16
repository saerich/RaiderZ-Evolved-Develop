#include "stdafx.h"
#include "GEntityActor.h"
#include "GUTHelper.h"
#include "MockField.h"
#include "GNPCInfo.h"
#include "GTalentInfo.h"
#include "GTestWrapper.h"
#include "GEntityPlayer.h"
#include "GEntityNPC.h"
#include "GModuleCombat.h"
#include "GCommandTable.h"
#include "TestCombatCalculator.h"
#include "GTestForward.h"
#include "GFieldMgr.h"
#include "FBaseMockLink.h"
#include "GMotionFactorHelper.h"

SUITE(MotionFactor)
{
	class GTestMotionFactor : public GMotionFactor
	{
	public:
		MOTION_FACTOR_VAR& GetVars(MF_STATE nState) { return m_Factors.var[nState]; }

		using GMotionFactor::CheckOverridable;
		using GMotionFactor::CheckFakeMF;
	};

	class FMotionFactor
	{
	public:
		FMotionFactor()
		{
			mf.ClearVars();
		}
		~FMotionFactor()
		{

		}
		bool Apply(MOTION_FACTOR_GROUP& mfg)
		{
			return mf.Apply(mfg, m_fValueModifiers, m_fWeightModifiers);
		}

		GTestMotionFactor mf;

		MOTION_FACTOR_GROUP	mfg;
		PerMulVar<short> m_fValueModifiers[MF_PRIORITY_SIZE];
		PerMulVar<short> m_fWeightModifiers[MF_PRIORITY_SIZE];
	};

	TEST(TestMotionFactorDefine)
	{
		CHECK_EQUAL(MF_PRIORITY_SIZE, 9);	// �켱 ������ �ִ� ��� ���ʹ� �� 9��
		CHECK_EQUAL(MF_VALUABLE_SIZE, 7);	// Value ����� �ϴ� ��� ���ʹ� �� 7��
		CHECK_EQUAL(MF_SIZE, 12);			// ��� ���ʹ� �� 10��

		CHECK(MF_SIZE >= MF_PRIORITY_SIZE);
		CHECK(MF_PRIORITY_SIZE >= MF_VALUABLE_SIZE);
	}

	TEST_FIXTURE(FMotionFactor, TestMotionFactorSimple)
	{
		mfg.var[MF_BEATEN].nValue = 100;
		mfg.var[MF_BEATEN].nWeight = 1000;

		CHECK(Apply(mfg) == true);

		CHECK_EQUAL(mf.GetCurrMF(), MF_BEATEN);
		CHECK_EQUAL(mf.GetCurrWeight(), 1000);
	}

	TEST_FIXTURE(FMotionFactor, TestMotionFactorSimple_Negative)
	{
		CHECK(Apply(mfg) == false);
	}

	TEST_FIXTURE(FMotionFactor, TestMotionFactorSimpleUpdate)
	{
		MF_STATE noutMF = MF_NONE;
		int noutWeight = 0;

		mfg.var[MF_STUN].nValue = 90;
		mfg.var[MF_STUN].nWeight = 1000;

		CHECK(Apply(mfg) == false);
		noutMF = mf.GetCurrMF();
		noutWeight = mf.GetCurrWeight();

		mf.Update(1.5f);		// ��� ���� ƽ���� value�� 10�� ��������.

		CHECK_EQUAL(mf.GetVars(MF_STUN).nValue, 80);

		MOTION_FACTOR_GROUP	mfg2;
		mfg2.var[MF_BEATEN].nValue = 50;
		mfg2.var[MF_BEATEN].nWeight = 1000;

		CHECK(Apply(mfg2) == false);
		noutMF = mf.GetCurrMF();
		noutWeight = mf.GetCurrWeight();

		mf.Update(1.5f);

		CHECK_EQUAL(mf.GetVars(MF_STUN).nValue, 70);

		MOTION_FACTOR_GROUP	mfg3;
		mfg3.var[MF_STUN].nValue = 50;
		mfg3.var[MF_STUN].nWeight = 500;

		CHECK(Apply(mfg3) == true);

		noutMF = mf.GetCurrMF();
		noutWeight = mf.GetCurrWeight();

		CHECK_EQUAL(noutMF, MF_STUN);
		CHECK_EQUAL(noutWeight, 500);

	}


	TEST_FIXTURE(FMotionFactor, TestMotionFactorModifier)
	{
		for (int i = 0; i < MF_PRIORITY_SIZE; i++)
		{
			m_fValueModifiers[i].fPer = 0.5f;
		}

		MF_STATE noutMF = MF_NONE;
		int noutWeight = 0;

		mfg.var[MF_STUN].nValue = 80;
		mfg.var[MF_STUN].nWeight = 1000;

		CHECK_EQUAL(false, Apply(mfg));
		noutMF = mf.GetCurrMF();
		noutWeight = mf.GetCurrWeight();

		CHECK_EQUAL(mf.GetVars(MF_STUN).nValue, 40);

	}

	TEST_FIXTURE(FMotionFactor, TestMotionFactorWeightModifier)
	{
		for (int i = 0; i < MF_PRIORITY_SIZE; i++)
		{
			m_fWeightModifiers[i].fPer = 0.5f;
		}

		MF_STATE noutMF = MF_NONE;
		int noutWeight = 0;

		mf.ClearVars();

		mfg.var[MF_STUN].nValue = 80;
		mfg.var[MF_STUN].nWeight = 1000;

		CHECK_EQUAL(false, Apply(mfg));
		noutMF = mf.GetCurrMF();
		noutWeight = mf.GetCurrWeight();

		CHECK_EQUAL(mf.GetVars(MF_STUN).nValue, 80);
		CHECK_EQUAL(mf.GetVars(MF_STUN).nWeight, 500);

	}

	TEST_FIXTURE(FMotionFactor, TestPiledMotionFactorSimple)
	{
		mfg.var[MF_BEATEN].nValue = 100;
		mfg.var[MF_BEATEN].nWeight = 2000;

		CHECK(Apply(mfg) == true);

		CHECK_EQUAL(mf.GetCurrMF(), MF_BEATEN);
		CHECK_EQUAL(mf.GetCurrWeight(), 2000);

		mf.Update(1.5f);

		mfg.Clear();
		mfg.var[MF_STUN].nValue = 100;
		mfg.var[MF_STUN].nWeight = 2000;

		CHECK(Apply(mfg) == true);

		CHECK_EQUAL(mf.GetCurrMF(), MF_STUN);
		CHECK_EQUAL(mf.GetCurrWeight(), 2000);
	}

	TEST_FIXTURE(FMotionFactor, TestMotionFactorSimple_WithFake)
	{
		mfg.var[MF_BEATEN].nValue = 100;
		mfg.var[MF_BEATEN].nWeight = 1000;

		mfg.var[MF_FAKE_BEATEN].nValue = 50;

		CHECK(Apply(mfg) == true);

		CHECK_EQUAL(mf.GetCurrMF(), MF_BEATEN);
		CHECK_EQUAL(mf.GetCurrWeight(), 1000);
	}

	TEST_FIXTURE(FMotionFactor, TestMotionFactorSimple_FakeBeaten)
	{
		mfg.var[MF_FAKE_BEATEN].nWeight = 100;

		CHECK(Apply(mfg) == true);

		CHECK_EQUAL(mf.GetCurrMF(), MF_FAKE_BEATEN);
		CHECK_EQUAL(mf.GetCurrWeight(), 100);
	}

	TEST_FIXTURE(FMotionFactor, TestMotionFactor_CheckFakeMF)
	{
		for (int i = 0; i< MF_SIZE; i++)
		{
			mf.ClearVars();
			mfg.var[i].nWeight = 1000;

			Apply(mfg);

			if (CSMotionFactorHelper::IsFakeMotionFactor(MF_STATE(i)))
			{
				MF_STATE outState;
				int outWeight = 0;
				CHECK_EQUAL(true, mf.CheckFakeMF(outState, outWeight, false));
				CHECK_EQUAL(1000, outWeight);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	/// �ŷ�Ʈ ����� ���� �ϱ� ���� �Ƚ���
	class FMotionFactor2 : public FBaseMockLink
	{
	public:
		FMotionFactor2()
		{
			m_pField = GUTHelper_Field::DefaultMockField();

			// �ŷ�Ʈ ���
			m_pTalentInfo = m_Helper.NewMeleeTalentInfo(1);
			m_Helper.SetTalentDamage(m_pTalentInfo, 10);		// ������ 1�� ����
			m_Helper.AddSimpleHitInfo(m_pTalentInfo, 0.0f);	// �ٷ� �����ϴ� �ŷ�Ʈ��

			vec3 pos = vec3(1000.0f, 1000.0f, 0.0f);
			vec3 dir = vec3(0.0f,1.0f,0.0f);

			m_NPCInfo.nID = 1;
			m_pNPC = m_pField->SpawnTestNPC(&m_NPCInfo, pos, dir);
			m_pPlayer = GUTHelper::NewEntityPlayer(m_pField, pos);

			m_Helper.SetEntityPlayerStatus(m_pPlayer, 100, 100, 100);
		}
		~FMotionFactor2()
		{
			m_pField->Destroy();
		}

		GTalentInfo* MakeTalent(int nDamage, int nTalentID, MF_STATE nMF, int nMFWeight)
		{
			GTalentInfo* pTalentInfo = m_Helper.NewMeleeTalentInfo(nTalentID);	
			m_Helper.AddSimpleHitInfo(pTalentInfo);	// �ٷ� �����ϴ� �ŷ�Ʈ��
			pTalentInfo->m_nMinDamage = nDamage;
			pTalentInfo->m_nMaxDamage = nDamage;
			if (nMF >= 0)
			{
				pTalentInfo->m_MotionFactorGroup.var[nMF].nValue = 100;
				pTalentInfo->m_MotionFactorGroup.var[nMF].nWeight = nMFWeight;
			}
			pTalentInfo->Cooking();
			return pTalentInfo;
		}

		GUTHelper		m_Helper;
		MockField*		m_pField;
		GNPCInfo		m_NPCInfo;
		CHR_STATUS		m_NPCStatus;
		GTestMgrWrapper<GTalentInfoMgr>	m_TalentInfoMgrWrapper;
		GTestSysWrapper2<GCombatCalculator, SimpleCombatCalculator>				m_CombatCalculator;
		GTestSysWrapper2<GAttackDamageCalculator, SimpleAttackDamageCalculator>	m_AttackDamageCalculator;
		GEntityPlayer*	m_pPlayer;
		GEntityNPC*		m_pNPC;
		GTalentInfo*	m_pTalentInfo;
	};


	TEST_FIXTURE(FMotionFactor2, TestMotionFactorClear)
	{
		GMotionFactor& mf = m_pNPC->GetMotionFactorForTest();
		MOTION_FACTOR_GROUP& mf_factors = mf.GetFactorsForTest();

		// ��� ���Ͱ� ����ǰ� �ش� ��� ���Ͱ� ������ �ٸ� ��� ���Ͱ��� ����� ��� ���Ϳ� ����
		// Ŭ����� ���� �ְ� Ŭ�������� ���� ���� �ִ�.

		// ��� ���Ͱ� ���
		m_pTalentInfo->m_MotionFactorGroup.var[MF_BEATEN].nValue = 100;
		m_pTalentInfo->m_MotionFactorGroup.var[MF_BEATEN].nWeight = 1000;
		m_pTalentInfo->m_MotionFactorGroup.var[MF_STUN].nValue = 50;
		m_pTalentInfo->m_MotionFactorGroup.var[MF_STUN].nWeight = 2000;
		m_pTalentInfo->Cooking();


		m_pPlayer->GetModuleCombat()->UseTalent(m_pTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
		m_pField->Update(0.1f);

		CHECK_EQUAL(m_pNPC->GetCurrentMotionFactor(), MF_BEATEN);
		m_pField->Update(1.5f);

		// BEATEN ����Ǿ STUN ���� Ŭ�������� �ʴ´�.
		CHECK(mf_factors.var[MF_STUN].nValue > 0);
		
		m_pField->Update(10.0f);

		CHECK_EQUAL(m_pNPC->GetCurrentMotionFactor(), MF_NONE);


		m_pTalentInfo->m_MotionFactorGroup.var[MF_THROWUP].nValue = 100;
		m_pTalentInfo->m_MotionFactorGroup.var[MF_THROWUP].nWeight = 1000;

		m_pPlayer->GetModuleCombat()->UseTalent(m_pTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
		m_pField->Update(0.1f);

		CHECK_EQUAL(m_pNPC->GetCurrentMotionFactor(), MF_THROWUP);
		m_pField->Update(m_pNPC->GetStandupTime() + (m_pTalentInfo->m_MotionFactorGroup.var[MF_THROWUP].nWeight/1000.0f) + FORCED_RELEASE_TIME_DELAY);

		// THROWUP�� ����Ǹ� ���� �� ��� ������Ͱ��� Ŭ�����Ѵ�.
		CHECK(mf_factors.var[MF_STUN].nValue == 0);

	}

	TEST_FIXTURE(FMotionFactor2, TestPiledMotionFactor)
	{
		GMotionFactor& mf = m_pNPC->GetMotionFactorForTest();
		MOTION_FACTOR_GROUP& mf_factors = mf.GetFactorsForTest();

		// ��ø������ ��� ���Ͱ� �ְ� ��ø �������� ���� ��� ���Ͱ� �ִ�.

		// ��� ���Ͱ� ���
		m_pTalentInfo->m_MotionFactorGroup.var[MF_UPPERED].nValue = 100;
		m_pTalentInfo->m_MotionFactorGroup.var[MF_UPPERED].nWeight = 1000;
		m_pTalentInfo->m_HitFilter.bMotionFactor[MF_UPPERED] = true;
		m_pTalentInfo->Cooking();

		m_pPlayer->GetModuleCombat()->UseTalent(m_pTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
		m_pField->Update(0.1f);

		CHECK_EQUAL(m_pNPC->GetCurrentMotionFactor(), MF_UPPERED);
		m_pField->Update(0.2f);


		m_pTalentInfo->m_MotionFactorGroup.var[MF_KNOCKBACK].nValue = 100;
		m_pTalentInfo->m_MotionFactorGroup.var[MF_KNOCKBACK].nWeight = 5000;

		m_pPlayer->GetModuleCombat()->UseTalent(m_pTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
		m_pField->Update(0.1f);

		CHECK_EQUAL(m_pNPC->GetCurrentMotionFactor(), MF_KNOCKBACK);
	}

	TEST_FIXTURE(FMotionFactor2, TestPiledMotionFactor2)
	{
		GMotionFactor& mf = m_pNPC->GetMotionFactorForTest();
		MOTION_FACTOR_GROUP& mf_factors = mf.GetFactorsForTest();

		// ��ø������ ��� ���Ͱ� �ְ� ��ø �������� ���� ��� ���Ͱ� �ִ�.

		// ��� ���Ͱ� ���
		m_pTalentInfo->m_MotionFactorGroup.var[MF_UPPERED].nValue = 100;
		m_pTalentInfo->m_MotionFactorGroup.var[MF_UPPERED].nWeight = 1000;
		m_pTalentInfo->m_HitFilter.bMotionFactor[MF_UPPERED] = true;
		m_pTalentInfo->Cooking();


		m_pPlayer->GetModuleCombat()->UseTalent(m_pTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
		m_pField->Update(0.1f);

		CHECK_EQUAL(m_pNPC->GetCurrentMotionFactor(), MF_UPPERED);
		m_pField->Update(0.2f);


		m_pTalentInfo->m_MotionFactorGroup.var[MF_KNOCKDOWN].nValue = 100;
		m_pTalentInfo->m_MotionFactorGroup.var[MF_KNOCKDOWN].nWeight = 5000;

		m_pPlayer->GetModuleCombat()->UseTalent(m_pTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
		m_pField->Update(0.1f);

		CHECK_EQUAL(m_pNPC->GetCurrentMotionFactor(), MF_KNOCKDOWN);
	}

	TEST_FIXTURE(FMotionFactor2, MFStunIsNotPiled)
	{
		MockLink* pPlayerLink = NewLink(m_pPlayer);
		pPlayerLink->AddIgnoreID(MC_CHAR_ENEMY_INFO);
		pPlayerLink->AddIgnoreID(MC_CHAR_ENEMY_INFO_CLEAR);
		pPlayerLink->AddIgnoreID(MC_ACTION_USE_TALENT);
		pPlayerLink->AddIgnoreID(MC_ACTION_TALENT_HIT_NO_MF);
		
		GMotionFactor& mf = m_pNPC->GetMotionFactorForTest();
		MOTION_FACTOR_GROUP& mf_factors = mf.GetFactorsForTest();

		// ��ø������ ��� ���Ͱ� �ְ� ��ø �������� ���� ��� ���Ͱ� �ִ�.

		// ��� ���Ͱ� ���
		m_Helper.SetTalentDamage(m_pTalentInfo, 0);
		m_pTalentInfo->m_MotionFactorGroup.var[MF_STUN].nValue = 100;
		m_pTalentInfo->m_MotionFactorGroup.var[MF_STUN].nWeight = 1000;
		m_pTalentInfo->m_HitFilter.bMotionFactor[MF_STUN] = true;
		m_pTalentInfo->Cooking();

		m_pPlayer->GetModuleCombat()->UseTalent(m_pTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
		m_pField->Update(0.1f);

		CHECK_EQUAL(m_pNPC->GetCurrentMotionFactor(), MF_STUN);
		m_pField->Update(0.2f);


		m_pTalentInfo->m_MotionFactorGroup.var[MF_BEATEN].nValue = 100;
		m_pTalentInfo->m_MotionFactorGroup.var[MF_BEATEN].nWeight = 5000;

		pPlayerLink->ResetCommands();

		m_pPlayer->GetModuleCombat()->UseTalent(m_pTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
		m_pField->Update(0.1f);

		CHECK_EQUAL(m_pNPC->GetCurrentMotionFactor(), MF_STUN);

		// �ι�° ������ �׳� ��� STUN�̹Ƿ� ��Ŷ�� ������ ����
		CHECK_EQUAL(0, pPlayerLink->GetCommand(0).GetID());

	}


	TEST_FIXTURE(FMotionFactor2, TestKnockbackMotionFactorPiled)
	{
		GMotionFactor& mf = m_pNPC->GetMotionFactorForTest();
		MOTION_FACTOR_GROUP& mf_factors = mf.GetFactorsForTest();

		// KnockBack ��� ���� ������ �� Beaten ��� ���Ϳ� �ɷ��� ��� Knockback�̾�� �Ѵ�.

		// ��� ���Ͱ� ���
		m_pTalentInfo->m_MotionFactorGroup.var[MF_KNOCKBACK].nValue = 100;
		m_pTalentInfo->m_MotionFactorGroup.var[MF_KNOCKBACK].nWeight = 1000;
		m_pTalentInfo->m_HitFilter.bMotionFactor[MF_KNOCKBACK] = true;
		m_pTalentInfo->Cooking();

		m_pPlayer->GetModuleCombat()->UseTalent(m_pTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
		m_pField->Update(0.1f);

		CHECK_EQUAL(m_pNPC->GetCurrentMotionFactor(), MF_KNOCKBACK);
		m_pField->Update(0.2f);

		m_pTalentInfo->m_MotionFactorGroup.var[MF_KNOCKBACK].nValue = 0;
		m_pTalentInfo->m_MotionFactorGroup.var[MF_KNOCKBACK].nWeight = 0;
		m_pTalentInfo->m_MotionFactorGroup.var[MF_BEATEN].nValue = 100;
		m_pTalentInfo->m_MotionFactorGroup.var[MF_BEATEN].nWeight = 5000;
		m_pTalentInfo->m_HitFilter.bMotionFactor[MF_BEATEN] = true;

		m_pPlayer->GetModuleCombat()->UseTalent(m_pTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
		m_pField->Update(0.1f);

		CHECK_EQUAL(m_pNPC->GetCurrentMotionFactor(), MF_KNOCKBACK);
	}


	TEST_FIXTURE(FMotionFactor2, ForceMF)
	{
		GMotionFactor& mf = m_pNPC->GetMotionFactorForTest();
		MOTION_FACTOR_GROUP& mf_factors = mf.GetFactorsForTest();

		// ��� ���Ͱ� ���
		m_pTalentInfo->m_nForceMF = MF_BEATEN;
		m_pTalentInfo->m_nForceMFWeight = 1000;
		m_pTalentInfo->Cooking();

		m_pNPC->GetModuleCombat()->UseTalent(m_pTalentInfo, TALENT_TARGET_INFO::Invalid(), false);
		m_pField->Update(0.1f);

		CHECK_EQUAL(m_pPlayer->GetCurrentMotionFactor(), MF_BEATEN);
		m_pField->Update(0.2f);
	}

	TEST_FIXTURE(FMotionFactor2, MovingMotionfactor)
	{
		int nKnockbackTalentID = 7891;
		GTalentInfo* pKnockbackTalentInfo = MakeTalent(1, nKnockbackTalentID, MF_KNOCKBACK, 50);
		GEntityPlayer* m_pPlayer = GUTHelper::NewEntityPlayer(m_pField, vec3(100,100,0));
		MockLink* m_pLinkTester = NewLink(m_pPlayer);

		GEntityPlayer* pDefender = GUTHelper::NewEntityPlayer(
			m_pField, 
			vec3(m_pPlayer->GetPos()+vec3(10,0,0)));

		vec3 vTesterOldPos = pDefender->GetPos();

		// �̵����� ������� ����
		m_pLinkTester->OnRecv(MC_ACTION_USE_TALENT_REQ, 3, 
			NEW_INT(nKnockbackTalentID),
			NEW_SVEC(vec3(0,1,0)),
			NEW_VEC(m_pPlayer->GetPos()));

		m_pField->Update(0.0f);	// �ŷ�Ʈ�� ���� ���� ƽ
		m_pField->Update(0.0f);	// �˹��� ���� �̵��� ���� ƽ

		// ������� üũ
		CHECK_EQUAL(false, vTesterOldPos.IsEqual(pDefender->GetPos()));	// �ڷ� �з����°�?
		CHECK_EQUAL(true, pDefender->HasMovingMotionfactor());

		// ������Ͱ� ����Ǿ���
		m_pField->Update(100.0f);	
		CHECK_EQUAL(false, pDefender->HasMovingMotionfactor());
	}

	TEST_FIXTURE(FMotionFactor2, DontMoveForMovingMotionfactor)
	{
		int nKnockbackTalentID = 7891;
		GTalentInfo* pKnockbackTalentInfo = MakeTalent(1, nKnockbackTalentID, MF_KNOCKBACK, 50);
		GEntityPlayer* m_pPlayer = GUTHelper::NewEntityPlayer(m_pField, vec3(100,100,0));
		MockLink* m_pLinkTester = NewLink(m_pPlayer);

		GEntityPlayer* pDefender = GUTHelper::NewEntityPlayer(
			m_pField, 
			vec3(m_pPlayer->GetPos()+vec3(10,0,0)));
		MockLink* m_pLinkDefender = NewLink(pDefender);

		// �̵����� ������� ����
		m_pLinkTester->OnRecv(MC_ACTION_USE_TALENT_REQ, 3, 
			NEW_INT(nKnockbackTalentID),
			NEW_SVEC(vec3(0,1,0)),
			NEW_VEC(m_pPlayer->GetPos()));
		m_pField->Update(0.0f);	// �ŷ�Ʈ�� ���� ���� ƽ
		m_pField->Update(0.0f);	// �˹��� ���� �̵��� ���� ƽ

		// ������� �����߿� �ŷ�Ʈ�� ������� ����
		CHECK_EQUAL(true, pDefender->HasMovingMotionfactor());
		CHECK_EQUAL(m_pPlayer->GetHP(), 300);
		m_pLinkDefender->OnRecv(MC_ACTION_USE_TALENT_REQ, 3, 
			NEW_INT(1),	// �Ϲݰ��� �ŷ�Ʈ
			NEW_SVEC(vec3(0,1,0)),
			NEW_VEC(pDefender->GetPos()));
		m_pField->Update(0.0f);	// �ŷ�Ʈ�� ���� ���� ƽ
		CHECK_EQUAL(m_pPlayer->GetHP(), 300);

		// ������Ͱ� ����Ǿ���
		m_pField->Update(100.0f);	
		CHECK_EQUAL(false, pDefender->HasMovingMotionfactor());

		// ������Ͱ� ������Ŀ��� �ŷ�Ʈ�� �ٽ� �����
		m_pLinkDefender->OnRecv(MC_ACTION_USE_TALENT_REQ, 3, 
			NEW_INT(1),	// �Ϲݰ��� �ŷ�Ʈ
			NEW_SVEC(vec3(0,1,0)),
			NEW_VEC(pDefender->GetPos()));
		m_pField->Update(0.0f);	// �ŷ�Ʈ�� ���� ���� ƽ
		CHECK_EQUAL(m_pPlayer->GetHP(), 290);
	}

	TEST_FIXTURE(FMotionFactor2, DontUseTalentForMovingMotionfactor)
	{
		int nKnockbackTalentID = 7891;
		GTalentInfo* pKnockbackTalentInfo = MakeTalent(1, nKnockbackTalentID, MF_KNOCKBACK, 50);
		GEntityPlayer* m_pPlayer = GUTHelper::NewEntityPlayer(m_pField, vec3(100,100,0));
		MockLink* m_pLinkTester = NewLink(m_pPlayer);

		GEntityPlayer* pDefender = GUTHelper::NewEntityPlayer(
			m_pField, 
			vec3(m_pPlayer->GetPos()+vec3(10,0,0)));
		MockLink* m_pLinkDefender = NewLink(pDefender);

		// �̵����� ������� ����
		m_pLinkTester->OnRecv(MC_ACTION_USE_TALENT_REQ, 3, 
			NEW_INT(nKnockbackTalentID),
			NEW_SVEC(vec3(0,1,0)),
			NEW_VEC(m_pPlayer->GetPos()));

		m_pField->Update(0.0f);	// �ŷ�Ʈ�� ���� ���� ƽ
		m_pField->Update(0.0f);	// �˹��� ���� �̵��� ���� ƽ

		// ������� �����߿� �̵��� ������� ����
		vec3 vTesterOldPos = pDefender->GetPos();
		CHECK_EQUAL(true, pDefender->HasMovingMotionfactor());
		TD_PC_MOVE	tdMove;
		tdMove.vTarPos = pDefender->GetPos()+vec3(10, 0, 0);
		m_pLinkDefender->OnRecv(MC_ACTION_MOVE_REQ, 1, NEW_SINGLE_BLOB(&tdMove, sizeof(TD_PC_MOVE)));
		CHECK_EQUAL(true, vTesterOldPos.IsEqual(pDefender->GetPos()));	// �̵��� �ȵƴ°�?

		// ������Ͱ� ����Ǿ���
		m_pField->Update(100.0f);	
		CHECK_EQUAL(false, pDefender->HasMovingMotionfactor());

		// ������Ͱ� ������Ŀ��� �̵��� �ٽ� �����
		vTesterOldPos = pDefender->GetPos();
		tdMove.vTarPos = pDefender->GetPos()+vec3(10, 0, 0);
		m_pLinkDefender->OnRecv(MC_ACTION_MOVE_REQ, 1, NEW_SINGLE_BLOB(&tdMove, sizeof(TD_PC_MOVE)));
		CHECK_EQUAL(false, vTesterOldPos.IsEqual(pDefender->GetPos()));	// �̵��� �ȵƴ°�?
	}

	TEST_FIXTURE(FMotionFactor2, ReleaseGuardInMovingMotionfactor)
	{
		int nKnockbackTalentID = 7891;
		GTalentInfo* pKnockbackTalentInfo = MakeTalent(1, nKnockbackTalentID, MF_KNOCKBACK, 50);
		pKnockbackTalentInfo->m_nGuardCrashLevel = GCL_ALL;
		GEntityPlayer* m_pPlayer = GUTHelper::NewEntityPlayer(m_pField, vec3(100,100,0));
		MockLink* m_pLinkTester = NewLink(m_pPlayer);

		GEntityPlayer* pDefender = GUTHelper::NewEntityPlayer(
			m_pField, 
			vec3(m_pPlayer->GetPos()+vec3(10,0,0)));
		MockLink* m_pLinkDefender = NewLink(pDefender);

		// �̵����� ������� ����
		m_pLinkTester->OnRecv(MC_ACTION_USE_TALENT_REQ, 3, 
			NEW_INT(nKnockbackTalentID),
			NEW_SVEC(vec3(0,1,0)),
			NEW_VEC(m_pPlayer->GetPos()));
		test.talent.Guard(pDefender);
		CHECK_EQUAL(true, pDefender->IsGuarding());

		m_pField->Update(0.0f);	// �ŷ�Ʈ�� ���� ���� ƽ
		m_pField->Update(0.0f);	// �˹��� ���� �̵��� ���� ƽ

		// ������� �����߿� �ŷ�Ʈ�� ������� ����
		CHECK_EQUAL(false, pDefender->IsGuarding());
		CHECK_EQUAL(true, pDefender->HasMovingMotionfactor());
		CHECK_EQUAL(m_pPlayer->GetHP(), 300);
		m_pLinkDefender->OnRecv(MC_ACTION_USE_TALENT_REQ, 3, 
			NEW_INT(1),	// �Ϲݰ��� �ŷ�Ʈ
			NEW_SVEC(vec3(0,1,0)),
			NEW_VEC(pDefender->GetPos()));
		m_pField->Update(0.0f);	// �ŷ�Ʈ�� ���� ���� ƽ
		CHECK_EQUAL(m_pPlayer->GetHP(), 300);

		// ������Ͱ� ����Ǿ���
		m_pField->Update(100.0f);	
		CHECK_EQUAL(false, pDefender->HasMovingMotionfactor());

		// ������Ͱ� ������Ŀ��� �ŷ�Ʈ�� �ٽ� �����
		m_pLinkDefender->OnRecv(MC_ACTION_USE_TALENT_REQ, 3, 
			NEW_INT(1),	// �Ϲݰ��� �ŷ�Ʈ
			NEW_SVEC(vec3(0,1,0)),
			NEW_VEC(pDefender->GetPos()));
		m_pField->Update(0.0f);	// �ŷ�Ʈ�� ���� ���� ƽ
		CHECK_EQUAL(m_pPlayer->GetHP(), 290);
	}

	TEST_FIXTURE(FMotionFactor2, TestMotionFactorOverridable)
	{
		MockLink* m_pLink = NewLink(m_pPlayer);
		m_pLink->IgnoreAll();
		m_pLink->AddAllowedID(MC_ACTION_TALENT_HIT);
		m_pLink->AddAllowedID(MC_ACTION_TALENT_HIT_NO_MF);

		GTalentInfo* pKnockbackTalentInfo = MakeTalent(1, 1001, MF_KNOCKBACK, 1000);
		GTalentInfo* pKnockdownTalentInfo = MakeTalent(1, 1002, MF_KNOCKDOWN, 1000);
		pKnockdownTalentInfo->m_HitFilter.bMotionFactor[MF_KNOCKDOWN] = true;

		test.talent.UseTalent(m_pPlayer, pKnockbackTalentInfo, m_pNPC->GetUID());
		m_pField->Update(0.01f);

		CHECK_EQUAL(m_pNPC->GetCurrMF(), MF_KNOCKBACK);

		// knockdown�� ��ø �����ϴ�
		test.talent.UseTalent(m_pPlayer, pKnockdownTalentInfo, m_pNPC->GetUID());
		m_pField->Update(0.1f);

		CHECK_EQUAL(m_pNPC->GetCurrMF(), MF_KNOCKDOWN);


		m_pField->Update(0.4f);
		m_pLink->ResetCommands();

		// knockdown�� �� �Ǵٽ� knockdown�� ��ø�� �� ����.
		test.talent.UseTalent(m_pPlayer, pKnockdownTalentInfo, m_pNPC->GetUID());

		m_pField->Update(0.1f);
		CHECK_EQUAL(m_pNPC->GetCurrMF(), MF_KNOCKDOWN);

		m_pField->Update(1.0f + FORCED_RELEASE_TIME_DELAY);
		CHECK_EQUAL(m_pNPC->GetCurrMF(), MF_NONE);


		CHECK(m_pLink->GetCommandCount() > 0);
		CHECK_EQUAL(MC_ACTION_TALENT_HIT, m_pLink->GetCommand(0).GetID());

		MCommandParameter* pParam = m_pLink->GetCommand(0).GetParameter(0);

		TD_TALENT_HIT* pHitInfo = (TD_TALENT_HIT*)pParam->GetPointer();
		CHECK_EQUAL(MF_NONE, pHitInfo->nMFState);

	}

	TEST(TestMotionFactorOverridable2)
	{
		GTestMotionFactor mf;

		CHECK_EQUAL(false, mf.CheckOverridable(MF_THROWUP, MF_THROWUP));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_THROWUP, MF_KNOCKDOWN));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_THROWUP, MF_KNOCKBACK));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_THROWUP, MF_STUN));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_THROWUP, MF_UPPERED));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_THROWUP, MF_DEFLECT));		
		CHECK_EQUAL(false, mf.CheckOverridable(MF_THROWUP, MF_BEATEN));

		CHECK_EQUAL(false, mf.CheckOverridable(MF_KNOCKDOWN, MF_THROWUP));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_KNOCKDOWN, MF_KNOCKDOWN));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_KNOCKDOWN, MF_KNOCKBACK));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_KNOCKDOWN, MF_STUN));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_KNOCKDOWN, MF_UPPERED));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_KNOCKDOWN, MF_DEFLECT));		
		CHECK_EQUAL(false, mf.CheckOverridable(MF_KNOCKDOWN, MF_BEATEN));


		CHECK_EQUAL(true, mf.CheckOverridable(MF_KNOCKBACK, MF_THROWUP));
		CHECK_EQUAL(true, mf.CheckOverridable(MF_KNOCKBACK, MF_KNOCKDOWN));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_KNOCKBACK, MF_KNOCKBACK));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_KNOCKBACK, MF_STUN));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_KNOCKBACK, MF_UPPERED));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_KNOCKBACK, MF_DEFLECT));		
		CHECK_EQUAL(false, mf.CheckOverridable(MF_KNOCKBACK, MF_BEATEN));

		CHECK_EQUAL(true, mf.CheckOverridable(MF_STUN, MF_THROWUP));
		CHECK_EQUAL(true, mf.CheckOverridable(MF_STUN, MF_KNOCKDOWN));
		CHECK_EQUAL(true, mf.CheckOverridable(MF_STUN, MF_KNOCKBACK));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_STUN, MF_STUN));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_STUN, MF_UPPERED));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_STUN, MF_DEFLECT));		
		CHECK_EQUAL(false, mf.CheckOverridable(MF_STUN, MF_BEATEN));

		CHECK_EQUAL(true, mf.CheckOverridable(MF_UPPERED, MF_THROWUP));
		CHECK_EQUAL(true, mf.CheckOverridable(MF_UPPERED, MF_KNOCKDOWN));
		CHECK_EQUAL(true, mf.CheckOverridable(MF_UPPERED, MF_KNOCKBACK));
		CHECK_EQUAL(true, mf.CheckOverridable(MF_UPPERED, MF_STUN));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_UPPERED, MF_UPPERED));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_UPPERED, MF_DEFLECT));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_UPPERED, MF_BEATEN));

		CHECK_EQUAL(true, mf.CheckOverridable(MF_DEFLECT, MF_THROWUP));
		CHECK_EQUAL(true, mf.CheckOverridable(MF_DEFLECT, MF_KNOCKDOWN));
		CHECK_EQUAL(true, mf.CheckOverridable(MF_DEFLECT, MF_KNOCKBACK));
		CHECK_EQUAL(true, mf.CheckOverridable(MF_DEFLECT, MF_STUN));
		CHECK_EQUAL(true, mf.CheckOverridable(MF_DEFLECT, MF_UPPERED));
		CHECK_EQUAL(false, mf.CheckOverridable(MF_DEFLECT, MF_DEFLECT));
		CHECK_EQUAL(true, mf.CheckOverridable(MF_DEFLECT, MF_BEATEN));

		CHECK_EQUAL(true, mf.CheckOverridable(MF_BEATEN, MF_THROWUP));
		CHECK_EQUAL(true, mf.CheckOverridable(MF_BEATEN, MF_KNOCKDOWN));
		CHECK_EQUAL(true, mf.CheckOverridable(MF_BEATEN, MF_KNOCKBACK));
		CHECK_EQUAL(true, mf.CheckOverridable(MF_BEATEN, MF_STUN));
		CHECK_EQUAL(true, mf.CheckOverridable(MF_BEATEN, MF_UPPERED));
		CHECK_EQUAL(true, mf.CheckOverridable(MF_BEATEN, MF_DEFLECT));		
		CHECK_EQUAL(true, mf.CheckOverridable(MF_BEATEN, MF_BEATEN));

	}
} // SUITE
