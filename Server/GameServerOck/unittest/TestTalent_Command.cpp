#include "stdafx.h"
#include "GTestForward.h"
#include "GTalent.h"
#include "GModuleCombat.h"
#include "FBasePlayer.h"

SUITE(TalentCommand)
{
	struct FTalent : public FBasePlayer
	{
		FTalent()
		{
			GNPCInfo* pNPCInfo = GUTHelper_NPC::NewNPCInfo(1);
			pNPCInfo->nMaxHP = 100;

			m_pField = GUTHelper_Field::DefaultMockField();
			m_pNPC = GUTHelper_NPC::SpawnNPC(m_pField, 1);
			m_pNPC->SetDir(vec3(0,1,0));

			m_pPlayer = NewEntityPlayer(m_pField);
			m_pLinkTester = test.network.NewLink(m_pPlayer);
		}
		~FTalent()
		{
			m_pField->Destroy();
			test.network.ClearLinks();
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
		void OnRecv__MC_ACTION_ACT_TALENT_REQ(MUID uidTarget)
		{
			TD_TALENT_TARGET_DETAIL infoTarget;
			infoTarget.uidTarget = uidTarget;
			m_pLinkTester->OnRecv(MC_ACTION_ACT_TALENT_WITH_HITCAPSULE_REQ,		2, 
				NEW_SVEC(m_pPlayer->GetDir()), 
				NEW_SINGLE_BLOB(&infoTarget, sizeof(TD_TALENT_TARGET_DETAIL)));
		}

		MockField*		m_pField;
		GEntityNPC*		m_pNPC;
		GEntityPlayer*	m_pPlayer;
		MockLink*		m_pLinkTester;
		GUTHelper		m_Helper;
		DECLWRAPPER_FieldInfoMgr;
		DECLWRAPPER_FieldMgr;
		DECLWRAPPER_TalentMgr;
		DECLWRAPPER_NPCInfoMgr;
	};

	// ��� �ߵ� �ŷ�Ʈ�� UseTalent ��Ŷ�� ��ȯ
	TEST_FIXTURE(FTalent, SendUseTalent_CasemmediateTalent)
	{
		int nTalentID = 111;
		GTalentInfo* pTalentInfo = test.talent.NewMeleeTalentInfo(nTalentID);	
		
		m_pLinkTester->OnRecv(MC_ACTION_USE_TALENT_REQ, 3, 
			NEW_INT(nTalentID),
			NEW_SVEC(vec3(0,1,0)),
			NEW_VEC(m_pPlayer->GetPos()));

		// ��Ŷ üũ
		TVALID_EQ(1, m_pLinkTester->GetCommandCount());
		const minet::MCommand& Command = m_pLinkTester->GetCommand(0);
		CHECK_EQUAL(Command.GetID(), MC_ACTION_USE_TALENT);
		TVALID_EQ(4, Command.GetParameterCount());
		UIID nEntityUIID;
		int nRecvTalentID;
		vec3 vecDir;
		svec3 svecDir;
		vec3 vecPos;
		TVALID(Command.GetParameter(&nEntityUIID,	0, MPT_USHORT));
		TVALID(Command.GetParameter(&nRecvTalentID,	1, MPT_INT));
		TVALID(Command.GetParameter(&svecDir,	2, MPT_SVEC));
		TVALID(Command.GetParameter(&vecPos,	3, MPT_VEC));

		vecDir = svecDir;

		CHECK_EQUAL(nEntityUIID, m_pPlayer->GetUIID());
		CHECK_EQUAL(nTalentID, nRecvTalentID);
		CHECK_CLOSE(vecDir.x, m_pPlayer->GetDir().x, 0.001f);
		CHECK_CLOSE(vecDir.y, m_pPlayer->GetDir().y, 0.001f);
		CHECK_CLOSE(vecDir.z, m_pPlayer->GetDir().z, 0.001f);
		CHECK_EQUAL(vecPos, m_pPlayer->GetPos());
	}

	// ĳ�����ִ� �ŷ�Ʈ�� UseTalent ��Ŷ�� ��ȯ
	TEST_FIXTURE(FTalent, SendUseTalent_CaseCastingTalent)
	{
		int nTalentID = 111;
		GTalentInfo* pTalentInfo = test.talent.NewMeleeTalentInfo(nTalentID);	
		pTalentInfo->m_fCastingTime = 10.0f;	// ĳ���� �ð��� ������

		m_pLinkTester->OnRecv(MC_ACTION_USE_TALENT_REQ, 3, 
			NEW_INT(nTalentID),
			NEW_SVEC(vec3(0,1,0)),
			NEW_VEC(m_pPlayer->GetPos()));

		// ��Ŷ üũ
		TVALID_EQ(1, m_pLinkTester->GetCommandCount());
		const minet::MCommand& Command = m_pLinkTester->GetCommand(0);
		CHECK_EQUAL(Command.GetID(), MC_ACTION_USE_TALENT);
		TVALID_EQ(4, Command.GetParameterCount());
		UIID nEntityUIID;
		int nRecvTalentID;
		vec3 vecDir;
		vec3 vecPos;
		svec3 svecDir;
		TVALID(Command.GetParameter(&nEntityUIID,	0, MPT_USHORT));
		TVALID(Command.GetParameter(&nRecvTalentID,	1, MPT_INT));
		TVALID(Command.GetParameter(&svecDir,	2, MPT_SVEC));
		TVALID(Command.GetParameter(&vecPos,	3, MPT_VEC));

		vecDir = svecDir;

		CHECK_EQUAL(nEntityUIID, m_pPlayer->GetUIID());
		CHECK_EQUAL(nTalentID, nRecvTalentID);
		CHECK_CLOSE(vecDir.x, m_pPlayer->GetDir().x, 0.001f);
		CHECK_CLOSE(vecDir.y, m_pPlayer->GetDir().y, 0.001f);
		CHECK_CLOSE(vecDir.z, m_pPlayer->GetDir().z, 0.001f);
		CHECK_EQUAL(vecPos, m_pPlayer->GetPos());
	}

	// ĳ�����ִ� �ŷ�Ʈ�� ĳ���� ����� ACT ��Ŷ�� ���� ��� ������
	TEST_FIXTURE(FTalent, SendActTalent_CaseCastingTalent)
	{
		int nTalentID = 111;
		GTalentInfo* pTalentInfo = test.talent.NewMeleeTalentInfo(nTalentID);	
		pTalentInfo->m_fCastingTime = 10.0f;	// ĳ���� �ð��� ������

		m_pPlayer->doUseTalent(pTalentInfo);
		// ĳ���ýð� ���
		m_pPlayer->GetModuleCombat()->Update(pTalentInfo->m_fCastingTime);
		
		// ACT ��Ŷ ����
		m_pLinkTester->ResetCommands();
		TD_TALENT_TARGET_DETAIL infoTarget;
		infoTarget.uidTarget = m_pNPC->GetUID();
		m_pLinkTester->OnRecv(MC_ACTION_ACT_TALENT_WITH_HITCAPSULE_REQ,		2, 
			NEW_SVEC(m_pPlayer->GetDir()), 
			NEW_SINGLE_BLOB(&infoTarget, sizeof(TD_TALENT_TARGET_DETAIL)));

		// ��Ŷ üũ
		TVALID_EQ(1, m_pLinkTester->GetCommandCount());
		const minet::MCommand& Command = m_pLinkTester->GetCommand(0);
		CHECK_EQUAL(Command.GetID(), MC_ACTION_ACT_TALENT);
		TVALID_EQ(3, Command.GetParameterCount());
		UIID nEntityUIID;
		int nRecvTalentID;
		svec3 svDir;
		vec3 vecDir;
		TVALID(Command.GetParameter(&nEntityUIID,	0, MPT_USHORT));
		TVALID(Command.GetParameter(&nRecvTalentID,	1, MPT_INT));
		TVALID(Command.GetParameter(&svDir,	2, MPT_SVEC));

		CHECK_EQUAL(nEntityUIID, m_pPlayer->GetUIID());
		CHECK_EQUAL(nTalentID, nRecvTalentID);

		vecDir = svDir;
		CHECK_CLOSE(vecDir.x, m_pPlayer->GetDir().x, 0.001f);
		CHECK_CLOSE(vecDir.y, m_pPlayer->GetDir().y, 0.001f);
		CHECK_CLOSE(vecDir.z, m_pPlayer->GetDir().z, 0.001f);
	}

	// ĳ�����ִ� �ŷ�Ʈ�� ĳ���� ����� ACT ��Ŷ�� ���� ��� ������
	TEST_FIXTURE(FTalent, SendProjectileAct_CaseCastingProjectileTalent)
	{
		float fCastingTime = 5.0f;
		float fDurationTime = 10.0f;
		float fEventTime = 3.0f;
		float fProjectileSpeed = 200.0f;
		GTalentInfo* pTalentInfo = MakeMissileMagicTalent(fCastingTime, fDurationTime, fEventTime, fProjectileSpeed);


		m_pNPC->SetPos(vec3(5000, 5000, 0));
		m_pPlayer->doUseTalent(pTalentInfo, m_pNPC->GetUID());
		// ĳ���ýð� ���
		m_pPlayer->GetModuleCombat()->Update(fCastingTime);

		// ACT ��Ŷ ����
		m_pLinkTester->ResetCommands();
		TD_TALENT_TARGET_DETAIL infoTarget;
		infoTarget.uidTarget = m_pNPC->GetUID();
		infoTarget.nCapsuleGroupIndex = 1;
		infoTarget.nCapsuleIndex = 2;
		m_pLinkTester->OnRecv(MC_ACTION_ACT_TALENT_WITH_HITCAPSULE_REQ,		2, 
			NEW_SVEC(m_pPlayer->GetDir()), 
			NEW_SINGLE_BLOB(&infoTarget, sizeof(TD_TALENT_TARGET_DETAIL)));

		vec3 vecExpectedTargetPos = m_pNPC->GetPos();
		//vecExpectedTargetPos.z += 120.0f;	// ���Ƿ� ����ü��ŭ �ø� ��

		// ��Ŷ üũ
		TVALID_EQ(1, m_pLinkTester->GetCommandCount());
		const minet::MCommand& Command = m_pLinkTester->GetCommand(0);
		CHECK_EQUAL(Command.GetID(), MC_ACTION_ACT_TALENT_PROJECTILE);
		TVALID_EQ(7, Command.GetParameterCount());
		MUID uidEntity;
		int nRecvTalentID;
		vec3 vecDir;
		vec3 vecPos;
		MUID uidTarget;
		int8 nCapsuleGroupIndex;
		int8 nCapsuleIndex;
		TVALID(Command.GetParameter(&uidEntity,	0, MPT_UID));
		TVALID(Command.GetParameter(&nRecvTalentID,	1, MPT_INT));
		TVALID(Command.GetParameter(&vecDir,	2, MPT_VEC));
		TVALID(Command.GetParameter(&vecPos,	3, MPT_VEC));
		TVALID(Command.GetParameter(&uidTarget,	4, MPT_UID));
		TVALID(Command.GetParameter(&nCapsuleGroupIndex,	5, MPT_CHAR));
		TVALID(Command.GetParameter(&nCapsuleIndex,	6, MPT_CHAR));
		CHECK_EQUAL(uidEntity, m_pLinkTester->GetUID());
		CHECK_EQUAL(pTalentInfo->m_nID, nRecvTalentID);
		CHECK_EQUAL(true, vecDir.IsEqual(m_pPlayer->GetDir()));
		CHECK_EQUAL(true, vecPos.IsEqual(vecExpectedTargetPos));
		CHECK_EQUAL(uidTarget, infoTarget.uidTarget);
		CHECK_EQUAL(nCapsuleGroupIndex, infoTarget.nCapsuleGroupIndex);
		CHECK_EQUAL(nCapsuleIndex, infoTarget.nCapsuleIndex);
	}


	// Hate���� �ִ� �ŷ�Ʈ �ǰ� Ŀ�ǵ� �׽�Ʈ
	TEST_FIXTURE(FTalent, TestHateobjTalent_Command)
	{
		m_pLinkTester->IgnoreAll();
		m_pLinkTester->AddAllowedID(MC_ACTION_TALENT_HIT_NO_MF);
		m_pLinkTester->AddAllowedID(MC_ACTION_TALENT_HIT);
		m_pLinkTester->AddAllowedID(MC_TALENT_INSTANT_EFFECT_GAIN);

		int nTalentID = 111;
		GTalentInfo* pTalentInfo = test.talent.NewMeleeTalentInfo(nTalentID);	
		m_Helper.SetTalentDamage(pTalentInfo, 0);
		m_Helper.AddSimpleHitInfo(pTalentInfo, 0.0f);

		m_pNPC->SetPos(vec3(5000, 5000, 0));
		m_pPlayer->SetPos(m_pNPC->GetPos());

		// damage�� 0�̰� ModValue�� ���� HateAdj���� 0�̹Ƿ� ���� Ŀ�ǵ带 ������ �ʴ´�.
		m_pPlayer->doUseTalent(pTalentInfo, m_pNPC->GetUID());

		m_pField->Update(0.1f);

		CHECK_EQUAL(0, m_pLinkTester->GetCommandCount());


		//
		m_pField->Update(10.0f);	m_pField->Update(10.0f);

		pTalentInfo->m_nHateAdj = 1000;

		// hate���� �����Ƿ� Ŀ�ǵ带 ������.
		m_pPlayer->doUseTalent(pTalentInfo, m_pNPC->GetUID());

		m_pField->Update(0.1f);

		CHECK_EQUAL(1, m_pLinkTester->GetCommandCount());
		CHECK_EQUAL(MC_TALENT_INSTANT_EFFECT_GAIN, m_pLinkTester->GetCommand(0).GetID());
	}

	TEST_FIXTURE(FTalent, TestTalent_MC_ACTION_ACT_TALENT_REQ)
	{
		int nTalentID = 111;
		GTalentInfo* pTalentInfo = test.talent.NewMagicTalentInfo(nTalentID);	
		pTalentInfo->m_fCastingTime = 10.0f;	// ĳ���� �ð��� ������

		m_pPlayer->doUseTalent(nTalentID);

		m_pField->Update(pTalentInfo->m_fCastingTime - 0.1f);

		// Ŭ���̾�Ʈ�κ��� ACT ��Ŷ ����
		OnRecv__MC_ACTION_ACT_TALENT_REQ(m_pNPC->GetUID());

		m_pField->Update(0.1f);

		// ��Ŷ üũ
		CHECK_EQUAL(true, m_pLinkTester->IsExistCommand(MC_ACTION_ACT_TALENT));
	}

	TEST_FIXTURE(FTalent, TestTalent_MC_ACTION_ACT_TALENT_REQ__Exception)
	{
		int nTalentID = 111;
		GTalentInfo* pTalentInfo = test.talent.NewMagicTalentInfo(nTalentID);	
		pTalentInfo->m_fCastingTime = 10.0f;	// ĳ���� �ð��� ������

		int nNPCTalentID = 112;
		GTalentInfo* pNPCTalentInfo = test.talent.NewMeleeTalentInfo(nNPCTalentID);	
		test.talent.MakeDamageTalent(10, nNPCTalentID);
		pNPCTalentInfo->m_MotionFactorGroup.var[MF_BEATEN].nValue = 100;
		pNPCTalentInfo->m_MotionFactorGroup.var[MF_BEATEN].nWeight = 1000;

		m_pPlayer->SetPos(vec3(5000, 5000, 0));
		m_pNPC->SetPos(vec3(5000, 5000, 0));

		m_pPlayer->doUseTalent(nTalentID);

		m_pField->Update(pTalentInfo->m_fCastingTime - 0.1f);

		// act req�� �ޱ� ���� �ǰݴ��ߴ�.


		// ACT ��Ŷ ����
		OnRecv__MC_ACTION_ACT_TALENT_REQ(m_pNPC->GetUID());


		m_pNPC->doUseTalent(nNPCTalentID);
		m_pField->Update(0.1f);

		//// ��Ŷ üũ
//		m_pLinkTester->IsExistCommand()
	}
}
