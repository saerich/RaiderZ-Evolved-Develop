#include "stdafx.h"
#include "GTestForward.h"
#include "GModuleCombat.h"
#include "GFieldMgr.h"
#include "FBasePlayer.h"
#include "GActorCooltimeChecker.h"
#include "CCommandResultTable.h"

SUITE(PlayerCoolTimeChecker)
{
	struct FPlayerCoolTimeChecker : public FBasePlayer
	{
		FPlayerCoolTimeChecker()
		{
			m_pField = GUTHelper_Field::DefaultMockField();
			m_pPlayer = NewEntityPlayer(m_pField);
		}

		virtual ~FPlayerCoolTimeChecker()
		{
			m_pField->Destroy();
			test.network.ClearLinks();
		}
		
		MockField*		m_pField;
		GEntityPlayer*	m_pPlayer;		

		DECLWRAPPER_FieldInfoMgr;
		DECLWRAPPER_FieldMgr;
		DECLWRAPPER_TalentMgr;
	};
	

	// ���� �ŷ�Ʈ ��Ÿ�� �������� To DB
	TEST_FIXTURE(FPlayerCoolTimeChecker, GetTalentRemainCoolTime)
	{
		GTalentInfo* pTalentInfo = test.talent.NewTalentInfo();
		pTalentInfo->m_fCoolTime = 5.0f;
		test.talent.UseTalent(m_pPlayer, pTalentInfo);

		float fElapsedCoolTime = 1.0f;
		m_pField->Update(fElapsedCoolTime);

		vector<pair<int,float>> vecTalentRemainCoolTime;
		m_pPlayer->GetActorCooltimeChecker().GetTalentRemainCoolTime(vecTalentRemainCoolTime);

		for each (pair<int,float> pair in vecTalentRemainCoolTime)
		{
			int nTalentID = pair.first;
			float fRemainCoolTime = pair.second;

			if (pTalentInfo->m_nID == nTalentID)
			{
				CHECK_EQUAL(pTalentInfo->m_fCoolTime - fElapsedCoolTime, fRemainCoolTime);
			}
		}
	}

	// ���� �ŷ�Ʈ ��Ÿ�� �ֱ� From DB
	TEST_FIXTURE(FPlayerCoolTimeChecker, InsertTalentRemainCoolTime)
	{
		GTalentInfo* pTalentInfo = test.talent.NewTalentInfo();
		pTalentInfo->m_fCoolTime = 5.0f;

		float fElapsedCoolTime = 1.0f;
		float fRemainCoolTime = pTalentInfo->m_fCoolTime - fElapsedCoolTime;

		vector<pair<int,float>> vecTalentRemainCoolTime;		
		vecTalentRemainCoolTime.push_back(vector<pair<int,float>>::value_type(pTalentInfo->m_nID, fRemainCoolTime));
		m_pPlayer->GetActorCooltimeChecker().InsertTalentRemainCoolTime(vecTalentRemainCoolTime);

		TALENT_TARGET_INFO targetInfo;
		targetInfo.uidTarget = m_pPlayer->GetUID();		
		CHECK_EQUAL(CR_FAIL_USE_TALENT_NOTENOUGH_COOLDOWN, m_pPlayer->GetModuleCombat()->UseTalent(pTalentInfo, targetInfo, true));

		m_pField->Update(fRemainCoolTime);

		CHECK_EQUAL(CR_SUCCESS, m_pPlayer->GetModuleCombat()->UseTalent(pTalentInfo, targetInfo, true));
	}

	TEST_FIXTURE(FPlayerCoolTimeChecker, ToggleTalent)
	{
		// ��Ŀ���� Ȱ��ȭ�� �� �ִ� ���⸦ ����
		test.focus.GainItemForSneak(m_pPlayer);

		// ��Ŀ�� ����
		test.buff.NewBuffInfo(BUFF_FOCUS_ID[TFT_SNEAK]);
		GTalentInfo* pActiveTalent = test.talent.NewTalentInfo();
		pActiveTalent->m_fCoolTime = 10.0f; // 10���Ŀ� ��Ÿ�� Ǯ��
		test.talent.SetExtraActiveTalentInfo(pActiveTalent, pActiveTalent->m_nID, TEAT_TOGGLE_SNEAK);
		CHECK_EQUAL(false, m_pPlayer->HasBuff(BUFF_FOCUS_ID[TFT_SNEAK]));
		CHECK_EQUAL(false, m_pPlayer->GetActorCooltimeChecker().IsCooldown(pActiveTalent->m_nID));
		test.talent.UseTalent(m_pPlayer, pActiveTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->HasBuff(BUFF_FOCUS_ID[TFT_SNEAK]));
		CHECK_EQUAL(false, m_pPlayer->GetActorCooltimeChecker().IsCooldown(pActiveTalent->m_nID));
		m_pPlayer->Update(10.0f); // ��Ÿ�� �ð� ���
		CHECK_EQUAL(false, m_pPlayer->GetActorCooltimeChecker().IsCooldown(pActiveTalent->m_nID));
		m_pPlayer->Update(10.0f); // ��Ÿ�� �ð� ��� ���
		m_pPlayer->Update(10.0f); // ��Ÿ�� �ð� ��� ���
		CHECK_EQUAL(false, m_pPlayer->GetActorCooltimeChecker().IsCooldown(pActiveTalent->m_nID));

		// ��� Ǯ��
		test.talent.UseTalent(m_pPlayer, pActiveTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(false, m_pPlayer->HasBuff(BUFF_FOCUS_ID[TFT_SNEAK]));
		m_pPlayer->Update(5.0f); // ��Ÿ�� �ð� ���� ���
		// ��Ŭ Ǯ���� ��Ÿ�� ����
		CHECK_EQUAL(true, m_pPlayer->GetActorCooltimeChecker().IsCooldown(pActiveTalent->m_nID));
		m_pPlayer->Update(5.0f); // ��Ÿ�� �ð� ���� ���� ���
		// ��Ÿ�� �Ϸ�
		CHECK_EQUAL(false, m_pPlayer->GetActorCooltimeChecker().IsCooldown(pActiveTalent->m_nID));
	}

	TEST_FIXTURE(FPlayerCoolTimeChecker, Cooltime)
	{
		GTalentInfo* pActiveTalent = test.talent.NewTalentInfo();
		pActiveTalent->m_fCoolTime = 10.0f; // 10���Ŀ� ��Ÿ�� Ǯ��
		CHECK_EQUAL(false, m_pPlayer->GetActorCooltimeChecker().IsCooldown(pActiveTalent->m_nID));
		test.talent.UseTalent(m_pPlayer, pActiveTalent, m_pPlayer->GetUID());
		CHECK_EQUAL(true, m_pPlayer->GetActorCooltimeChecker().IsCooldown(pActiveTalent->m_nID));
		m_pPlayer->Update(10.0f); // ��Ÿ�� �ð� ���
		CHECK_EQUAL(false, m_pPlayer->GetActorCooltimeChecker().IsCooldown(pActiveTalent->m_nID));
	}

	TEST_FIXTURE(FPlayerCoolTimeChecker, Cooltime_ForNPC)
	{
		GEntityNPC* pNPC = GUTHelper_NPC::SpawnNPC(m_pField);
		GTalentInfo* pActiveTalent = test.talent.NewTalentInfo();
		pActiveTalent->m_fCoolTime = 10.0f; // 10���Ŀ� ��Ÿ�� Ǯ��
		CHECK_EQUAL(false, pNPC->GetActorCooltimeChecker().IsCooldown(pActiveTalent->m_nID));
		test.talent.UseTalent(pNPC, pActiveTalent, pNPC->GetUID());
		CHECK_EQUAL(true, pNPC->GetActorCooltimeChecker().IsCooldown(pActiveTalent->m_nID));
		pNPC->Update(10.0f); // ��Ÿ�� �ð� ���
		CHECK_EQUAL(false, pNPC->GetActorCooltimeChecker().IsCooldown(pActiveTalent->m_nID));
	}

	// ���� �ŷ�Ʈ ������ �ŷ�Ʈ���� ��Ÿ�� ����
	TEST_FIXTURE(FPlayerCoolTimeChecker, Cooltime_SharingWithSameLineTalents)
	{
		GTalentInfo* pActiveTalentA = test.talent.NewTalentInfo(1, 111);
		pActiveTalentA->m_fCoolTime = 10.0f;
		GTalentInfo* pActiveTalentB = test.talent.NewTalentInfo(2, 111);
		pActiveTalentB->m_fCoolTime = 5.0f; 
		CHECK_EQUAL(false, m_pPlayer->GetActorCooltimeChecker().IsCooldown(pActiveTalentA->m_nID));
		CHECK_EQUAL(false, m_pPlayer->GetActorCooltimeChecker().IsCooldown(pActiveTalentB->m_nID));
		test.talent.UseTalent(m_pPlayer, pActiveTalentA, m_pPlayer->GetUID()); // A�ŷ�Ʈ ���
		CHECK_EQUAL(true, m_pPlayer->GetActorCooltimeChecker().IsCooldown(pActiveTalentA->m_nID));
		CHECK_EQUAL(true, m_pPlayer->GetActorCooltimeChecker().IsCooldown(pActiveTalentB->m_nID));
		m_pPlayer->Update(5.0f); // ��Ÿ�� �ð� ���� ���
		CHECK_EQUAL(true, m_pPlayer->GetActorCooltimeChecker().IsCooldown(pActiveTalentA->m_nID));
		CHECK_EQUAL(true, m_pPlayer->GetActorCooltimeChecker().IsCooldown(pActiveTalentB->m_nID));
		m_pPlayer->Update(5.0f); // ��Ÿ�� �ð� ���
		CHECK_EQUAL(false, m_pPlayer->GetActorCooltimeChecker().IsCooldown(pActiveTalentA->m_nID));
		CHECK_EQUAL(false, m_pPlayer->GetActorCooltimeChecker().IsCooldown(pActiveTalentB->m_nID));
	}
}