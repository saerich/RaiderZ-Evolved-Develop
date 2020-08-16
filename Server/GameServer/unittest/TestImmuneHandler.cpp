#include "stdafx.h"
#include "GTestForward.h"
#include "GGlobal.h"
#include "GServer.h"
#include "FBasePlayer.h"

SUITE(ImmuneHandler)
{
	struct FImmuneHandler : public FBasePlayer
	{
		FImmuneHandler()
		{
			m_pField = GUTHelper_Field::DefaultMockField();

			// �÷��̾� ����
			MakePlayer(m_pAttacker, L"Attacker");
			MakePlayer(m_pTester, L"Tester");
		}

		~FImmuneHandler()
		{
			m_pField->Destroy();
			test.network.ClearLinks();
			m_pField->Destroy();
		}

		void MakePlayer(GEntityPlayer*& pPlayer, std::wstring strName)
		{
			pPlayer = NewEntityPlayer(m_pField, vec3(100,100,0));
			swprintf_s(pPlayer->GetPlayerInfo()->szName, strName.c_str());
		}

		MockField*		m_pField;
		GEntityPlayer*	m_pAttacker;
		GEntityPlayer*	m_pTester;

		DECLWRAPPER_FieldInfoMgr;
		DECLWRAPPER_FieldMgr;
		DECLWRAPPER_BuffMgr;
		DECLWRAPPER_SimpleCombatCalc;
	};

	TEST_FIXTURE(FImmuneHandler, GainExtraAttribBuff)
	{
		// ��� ���� Ư��ȿ���� �ɸ�
		for (int i=0; i<BUFPEA_MAX; i++)
		{
			int nBuffID = 1000+i;
			float fDuration = 100.0f;
			GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
			pBuff->m_nPassiveExtraAttrib = (BUFF_PASSIVE_EXTRA_ATTRIB_)i;
			test.buff.GainBuffSelf(m_pTester, pBuff, fDuration);
			CHECK_EQUAL(true, m_pTester->HasBuff(nBuffID));
			m_pField->Update(fDuration);
			CHECK_EQUAL(false, m_pTester->HasBuff(nBuffID));
		}
	}

	TEST_FIXTURE(FImmuneHandler, ImmuneBuff)
	{
		for (int i=0; i<BUFPEA_MAX; i++)
		{
			// �鿪 ����
			immune::INFO info;
			SetBitSet(info.nImmuneBuff, (BUFF_PASSIVE_EXTRA_ATTRIB_)i);
			m_pTester->GetImmuneHandler().Init(info);

			// ���� �Ȱɸ����� Ȯ��
			int nBuffID = 1000+i;
			GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
			pBuff->m_nPassiveExtraAttrib = (BUFF_PASSIVE_EXTRA_ATTRIB_)i;
			test.buff.GainBuffSelf(m_pTester, pBuff);
			CHECK_EQUAL(false, m_pTester->HasBuff(nBuffID));
		}
	}

	TEST_FIXTURE(FImmuneHandler, TakeDamageType)
	{
		// ��� ����Ÿ���� ���ظ� ����
		for (int i=0; i<DA_MAX; i++)
		{
			GTalentInfo* pAttackTalent = test.talent.NewTalentInfo();
			test.talent.SetTalentDamage(pAttackTalent, 10);
			pAttackTalent->m_nDamageAttrib = (DAMAGE_ATTRIB)i;

			m_pTester->SetHP(100);
			test.talent.UseTalent(m_pAttacker, pAttackTalent, m_pTester->GetUID());
			CHECK_EQUAL(90, m_pTester->GetHP());
		}
	}

	TEST_FIXTURE(FImmuneHandler, ImmuneSkillType)
	{
		// ��� ��ųŸ���� ���ظ� ����
		for (int i=0; i<ST_MAX; i++)
		{
			// �鿪 ����
			immune::INFO info;
			SetBitSet(info.nImmuneSkillType, (TALENT_SKILL_TYPE)i);
			m_pTester->GetImmuneHandler().Init(info);

			// ���� ���Դ��� Ȯ��
			GTalentInfo* pAttackTalent = test.talent.NewTalentInfo();
			test.talent.SetTalentDamage(pAttackTalent, 10);
			pAttackTalent->m_nSkillType = (TALENT_SKILL_TYPE)i;

			m_pTester->SetHP(100);
			test.duel.NewMockDuel_Fighting(m_pAttacker, m_pTester);
			test.talent.UseTalent(m_pAttacker, pAttackTalent, m_pTester->GetUID());
			CHECK_EQUAL(100, m_pTester->GetHP());
		}
	}

	TEST_FIXTURE(FImmuneHandler, ImmuneDrain)
	{
		// �鿪 ����
		immune::INFO info;
		SetBitSet(info.nImmuneCustomType, immune::IMMUNE_DRAIN);
		m_pTester->GetImmuneHandler().Init(info);

		// ���� ���Դ��� Ȯ��
		GTalentInfo* pAttackTalent = test.talent.NewTalentInfo();
		test.talent.SetTalentDamage(pAttackTalent, 10);
		pAttackTalent->m_Drain.m_nType = TALENT_DRAIN_HP;
		pAttackTalent->m_Drain.m_fRate = 100.0f;// 100% Ȯ��

		m_pTester->SetHP(100);
		test.duel.NewMockDuel_Fighting(m_pAttacker, m_pTester);
		test.talent.UseTalent(m_pAttacker, pAttackTalent, m_pTester->GetUID());
		CHECK_EQUAL(100, m_pTester->GetHP());
	}

	TEST_FIXTURE(FImmuneHandler, ImmuneDispel)
	{
		// �鿪 ����
		immune::INFO info;
		SetBitSet(info.nImmuneCustomType, immune::IMMUNE_DISPEL);
		m_pTester->GetImmuneHandler().Init(info);

		// ���� ����
		int nBuffID = 123;
		CHECK_EQUAL(false, m_pTester->HasBuff(nBuffID));
		GBuffInfo* pBuff=test.buff.NewBuffInfo(nBuffID);
		pBuff->m_nType = BUFT_BUFF;
		test.buff.GainBuffSelf(m_pTester, pBuff);
		CHECK_EQUAL(true, m_pTester->HasBuff(nBuffID));

		// ���翡 �鿪
		GTalentInfo* pActiveTalent = test.talent.NewTalentInfo();
		test.talent.SetExtraActiveTalentInfo(pActiveTalent, 1, TEAT_DISPEL_BUFF);
		pActiveTalent->m_nExtraActiveParam1 = 1;	// 1���� ����
		pActiveTalent->m_nExtraActiveParam2 = 10;	// �����Ŀ� 10�������� ����
		pActiveTalent->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_TARGET;
		pActiveTalent->m_EffectInfo.m_nRelation = CSEffectInfo::RELATION_ALL;
		test.talent.UseTalent(m_pAttacker, pActiveTalent, m_pTester->GetUID());
		CHECK_EQUAL(true, m_pTester->HasBuff(nBuffID));	// �Ȼ����
	}

	TEST(WeaponDamageAttrib)
	{
		MockField* m_pField = GUTHelper_Field::DefaultMockField();
		GEntityPlayer* m_pAttacker = GUTHelper::NewEntityPlayer(m_pField, vec3(100,100,0));
		GEntityNPC* pTargetNPC = GUTHelper_NPC::SpawnNPC(m_pField);

		GTalentInfo* pAttackTalent = test.talent.NewTalentInfo();
		test.talent.SetTalentDamage(pAttackTalent, 10);
		pAttackTalent->m_WeaponReference = WR_RIGHT;	// ������ ����Ÿ�Կ� ����
		GItem* pWeapon = test.item.EquipNewWeapon(m_pAttacker, WEAPON_1H_SLASH);

		pWeapon->m_pItemData->m_nDamageAttrib = DA_NONE;	// ���� Ÿ�� ����
		test.talent.UseTalent(m_pAttacker, pAttackTalent, pTargetNPC->GetUID());
		CHECK_EQUAL(DA_NONE, pTargetNPC->GetLastDamagedInfo().nType);

		pWeapon->m_pItemData->m_nDamageAttrib = DA_SLASH;	// ���� Ÿ���� ������
		test.talent.UseTalent(m_pAttacker, pAttackTalent, pTargetNPC->GetUID());
		CHECK_EQUAL(DA_SLASH, pTargetNPC->GetLastDamagedInfo().nType);
	}
}