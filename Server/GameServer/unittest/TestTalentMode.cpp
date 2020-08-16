#include "stdafx.h"
#include "GTalentInfo.h"
#include "GTalentInfoMgr.h"
#include "GUTHelper.h"
#include "GTestWrapper.h"
#include "MockField.h"
#include "GModuleAI.h"
#include "GItemManager.h"
#include "GTalentHelper.h"
#include "GEntityNPC.h"
#include "FBaseMockLink.h"
#include "GItemEquiper.h"
#include "GUTHelper_Item.h"
#include "GUTHelper_Field.h"
#include "GItemHolder.h"
#include "GEquipmentSlot.h"

SUITE(TalentMode)
{
	struct FTalentMode : public FBaseMockLink
	{
		FTalentMode() 
		{
			gsys.pScriptManager->Init();

			m_pField = GUTHelper_Field::DefaultMockField();
			m_NPCInfo.nID = 1;

			vec3 pos = vec3(1000, 1000, 0);
			vec3 dir = vec3(0,1,0);

			m_pNPC = m_pField->SpawnTestNPC(&m_NPCInfo, pos, dir);
			m_pPlayer = GUTHelper::NewEntityPlayer(m_pField, pos);
			m_pPlayerLink = NewLink(m_pPlayer);

			InitTalentInfo();

			m_pNPC->SetThinkable(false);
			m_Helper.SetEntityPlayerStatus(m_pPlayer, 100, 100, 100);
		}
		~FTalentMode() 
		{
			m_pField->Destroy();

			gsys.pScriptManager->Fini();
		}
		void InitTalentInfo()
		{
			GTalentInfo* pTalentInfo = m_Helper.NewMeleeTalentInfo(1);
			m_Helper.SetTalentDamage(pTalentInfo, 0);		// ������ 0���� ����
			m_Helper.AddSimpleHitInfo(pTalentInfo, 0.0f);	// �ٷ� �����ϴ� �ŷ�Ʈ��

			// 1hb ��� �߰�
			GTalentInfo* pModeTalentInfo = new GTalentInfo();
			pModeTalentInfo->m_nID = 1;
			pTalentInfo->CopyAll(pModeTalentInfo);
			pModeTalentInfo->m_nMode = WEAPON_1H_BLUNT;
			m_Helper.SetTalentDamage(pModeTalentInfo, 1000);		// ������ 1000���� ����
			m_Helper.AddSimpleHitInfo(pModeTalentInfo, 0.0f);	// �ٷ� �����ϴ� �ŷ�Ʈ��

			pTalentInfo->InsertMode(pModeTalentInfo);

			gmgr.pTalentInfoMgr->Cooking();
		}
		void Equip_1hb()
		{
			// �÷��̾� 1hb ����
			GUTHelper_Item::EquipNewWeapon(m_pPlayer, WEAPON_1H_BLUNT);
		}

		GUTHelper		m_Helper;
		MockField*		m_pField;
		GEntityNPC*		m_pNPC;
		GEntityPlayer*	m_pPlayer;
		MockLink*		m_pPlayerLink;
		GNPCInfo		m_NPCInfo;

		GTestMgrWrapper<GTalentInfoMgr>	m_TalentInfoMgrWrapper;
		GTestMgrWrapper<GItemManager>	m_ItemManagerWrapper;
	};

	TEST_FIXTURE(FTalentMode, TestTalentInfoMgr_Find)
	{
		GTalentInfo* pModeTalentInfo = gmgr.pTalentInfoMgr->Find(1, WEAPON_1H_BLUNT);
		CHECK_EQUAL(WEAPON_1H_BLUNT, pModeTalentInfo->m_nMode);
		CHECK_EQUAL(1000, pModeTalentInfo->m_nMinDamage);
	}

	TEST_FIXTURE(FTalentMode, TestTalentHelper_FindTalentInfo)
	{
		Equip_1hb();
		GItemHolder* pItemHolder = m_pPlayer->GetItemHolder();
		CHECK_EQUAL(pItemHolder->GetEquipment().GetWeaponType(), WEAPON_1H_BLUNT);

		GTalentInfo* pTalentInfo = GTalentHelper::FindTalentInfo(1, m_pPlayer);
		CHECK_EQUAL(WEAPON_1H_BLUNT, pTalentInfo->m_nMode);
		CHECK_EQUAL(1000, pTalentInfo->m_nMinDamage);
	}

	TEST_FIXTURE(FTalentMode, TestTalentHelper_UseTalent)
	{
		int nNPC_HP = m_pNPC->GetHP();

		m_pPlayer->doUseTalent(1);
		m_pField->Update(0.1f);

		CHECK_EQUAL(nNPC_HP, m_pNPC->GetHP());

		m_pField->Update(10.0f);

		// 1hb�� ��� ��ü - 1hb �ŷ�Ʈ�� ������� 1000�̴�.
		Equip_1hb();

		nNPC_HP = m_pNPC->GetHP();
		m_pPlayer->doUseTalent(1);
		m_pField->Update(0.1f);

		CHECK(nNPC_HP != m_pNPC->GetHP());

	}

}