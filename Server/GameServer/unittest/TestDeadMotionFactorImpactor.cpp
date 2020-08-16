#include "stdafx.h"
#include "GDeadMotionFactorImpactor.h"
#include "MockEntityPlayer.h"
#include "MockEntityNPC.h"

SUITE(DeadMotionFactorImpactor)
{
	class TestDeadMotionFactorImpactor : public GDeadMotionFactorImpactor
	{
	protected:
		virtual bool Dice() { return true; }
	public:
		TestDeadMotionFactorImpactor() {}
	};

	class FDeadMotionFactorImpactor
	{
	public:
		FDeadMotionFactorImpactor()
		{
			m_EntityNPC.GetNPCInfo()->MFModifier.Value[MF_KNOCKBACK].fPer = 1.0f;
			m_EntityNPC.GetNPCInfo()->MFModifier.Value[MF_THROWUP].fPer = 1.0f;

			args.nForceMF = MF_NONE;
			args.nForceMFWeight = 0;

			m_EntityNPC.GetChrStatus()->nHP = 100;

			m_TalentInfo.m_FinishEffect.m_nFinishType = TALENT_FINISH_KNOCK_BACK;
		}
		~FDeadMotionFactorImpactor()
		{

		}
		bool CheckUp(GEntityActor* pTarget, int nDamage)
		{
			return m_Impactor.CheckUp(args, pTarget, &m_TalentInfo, nDamage);
		}

		GTalentInfo			m_TalentInfo;
		MockEntityNPC		m_EntityNPC;
		MockEntityPlayer	m_EntityPlayer;

		TestDeadMotionFactorImpactor	m_Impactor;
		GMFApplyArgs					args;
	};

	TEST_FIXTURE(FDeadMotionFactorImpactor, TestDeadMotionFactorImpactor_CheckUp)
	{
		CHECK_EQUAL(true, CheckUp(&m_EntityNPC, m_EntityNPC.GetHP()));

		CHECK(MF_KNOCKBACK == args.nForceMF || MF_THROWUP == args.nForceMF);
		CHECK(args.nForceMFWeight > 0);
	}

	// �׾��� ������ ȿ�� ����Ǿ�� �Ѵ�.
	TEST_FIXTURE(FDeadMotionFactorImpactor, TestDeadMotionFactorImpactor_Dead)
	{
		int nHP = m_EntityNPC.GetHP();
		CHECK(nHP > 0);

		CHECK_EQUAL(true, CheckUp(&m_EntityNPC, nHP));
		CHECK_EQUAL(true, CheckUp(&m_EntityNPC, nHP + 1));
		CHECK_EQUAL(true, CheckUp(&m_EntityNPC, 99999));
	}

	TEST_FIXTURE(FDeadMotionFactorImpactor, TestDeadMotionFactorImpactor_Dead_Negative)
	{
		int nHP = m_EntityNPC.GetHP();

		CHECK_EQUAL(false, CheckUp(&m_EntityNPC, nHP - 1));
		CHECK_EQUAL(false, CheckUp(&m_EntityNPC, 1));
	}

	// NPC�� ������ ȿ�� ����Ǿ�� �Ѵ�.
	TEST_FIXTURE(FDeadMotionFactorImpactor, TestDeadMotionFactorImpactor_NPC_Negative)
	{
		CHECK_EQUAL(false, CheckUp(&m_EntityPlayer, m_EntityPlayer.GetHP()));
	}

	// �ŷ�Ʈ ������ �̹� ForceMF���� ������ ������� �ʴ´�. (��� ��)
	TEST_FIXTURE(FDeadMotionFactorImpactor, TestDeadMotionFactorImpactor_TalentInfo_ForceMF_Negative)
	{
		m_TalentInfo.m_nForceMF = MF_GRAPPLED;

		CHECK_EQUAL(false, CheckUp(&m_EntityNPC, m_EntityNPC.GetHP()));
	}


	// ���� �� KnockBack�� �ȵǴ� ���ʹ� �����Ѵ�.
	TEST_FIXTURE(FDeadMotionFactorImpactor, TestDeadMotionFactorImpactor_Boss_Negative)
	{
		m_EntityNPC.GetNPCInfo()->MFModifier.Value[MF_KNOCKBACK].fPer = 0.01f;
		CHECK_EQUAL(false, CheckUp(&m_EntityNPC, m_EntityNPC.GetHP()));
	}

}