#include "stdafx.h"
#include "TestModule.h"
#include "XMyPlayer.h"
#include "XModuleMyControl.h"
#include "XModuleMovable.h"
#include "MockItemInfo.h"
#include "XMyActionNormalAttack.h"
#include "XMyActionGuard.h"
#include "XPost_Loot.h"
#include "MockBuffInfoMgr.h"
#include "RMesh.h"
#include "XGlobal.h"
#include "FBaseNetClient.h"

SUITE(MyPlayerAction)
{
	struct FMyPlayerActionFixture : public FBaseNetClient
	{
		FMyPlayerActionFixture()
		{
			m_pController = tGame.GetController();
			m_pMyPlayer = tGame.GetMyPlayer();
			m_pModuleMyControl = m_pMyPlayer->GetModuleMyControl();

			// npc ���� �߰�
			XUnitTestHelper::NewNPCInfo(1, L"sample", 3, L"hm_adult", L"NPC/hm_adult", L"monster");
		}
		~FMyPlayerActionFixture()
		{

		}

		void CreateNormalAttackTalents()
		{
			MockTalentInfoMgr* pTalentInfoMgr = static_cast<MockTalentInfoMgr*>(info.talent);

			// �⺻ ����
			pTalentInfoMgr->CreateTalent(L"<TALENT id='101001' name='1hs �Ϲݰ��� 1Ÿ' TalentType='skill' Category='melee' SkillType='melee' Stance='combat' UseAni='PS_atk1;PS_atk1_r' />");
			pTalentInfoMgr->CreateTalent(L"<TALENT id='101002' name='1hs �Ϲݰ��� 2Ÿ' TalentType='skill' Category='melee' SkillType='melee' Stance='combat' UseAni='PS_atk1;PS_atk1_r' />");
			pTalentInfoMgr->CreateTalent(L"<TALENT id='101003' name='1hs �Ϲݰ��� 3Ÿ' TalentType='skill' Category='melee' SkillType='melee' Stance='combat' UseAni='PS_atk1;PS_atk1_r' />");
			pTalentInfoMgr->CreateTalent(L"<TALENT id='101004' name='1hs �Ϲݰ��� 4Ÿ' TalentType='skill' Category='melee' SkillType='melee' Stance='combat' UseAni='PS_atk1;PS_atk1_r' />");
			pTalentInfoMgr->CreateTalent(L"<TALENT id='101005' name='1hs �Ϲݰ��� 5Ÿ' TalentType='skill' Category='melee' SkillType='melee' Stance='combat' UseAni='PS_atk1;PS_atk1_r' />");

			// ��Ƽ�� �ŷ�Ʈ
			pTalentInfoMgr->CreateTalent(L"<TALENT id='20501' name='������ũ ����Ʈ Rank 1' TalentLine='20501' Rank='1' TalentType='skill' Category='melee' SkillType='melee' Stance='combat' UseAni='PS_atk1;PS_atk1_r' />");

			info.motion->AddMotionForTest(L"<MOTION name='PS_atk1' loop='false' cameraLook='false'><Default><Sequence ani='atk1'/></Default></MOTION>");
			info.motion->AddMotionForTest(L"<MOTION name='PS_atk1_r' loop='false' cameraLook='false'><Default><Sequence ani='atk1_r'/></Default></MOTION>");


			// ��� �ŷ�Ʈ
			XUnitTestHelper::CreateGuardTalentInfo(CSTalentInfoHelper::NormalGuardTalentID(WEAPON_1H_SLASH, 0, false));
			XUnitTestHelper::CreateGuardTalentInfo(CSTalentInfoHelper::NormalGuardTalentID(WEAPON_1H_SLASH, 1, false));

			info.motion->AddMotionForTest(L"<MOTION name='idle' stance='all' loop='true' cancelEnabled='true' cameraLook='true'><Default><Sequence ani='idle'/></Default></MOTION>");
			info.motion->AddMotionForTest(L"<MOTION name='guard' stance='battle' loop='true' cancelEnabled='true' cameraLook='true'><Default><Sequence ani='guard_idle'/></Default></MOTION>");
			info.motion->AddMotionForTest(L"<MOTION name='guard_r' stance='battle' loop='false' cancelEnabled='true' cameraLook='true'><Default><Sequence ani='guard_end'/></Default></MOTION>");

			// ���� ����
			MockBuffInfoMgr* pBuffInfoMgr = static_cast<MockBuffInfoMgr*>(info.buff);
			pBuffInfoMgr->CreateBuffInfo( L"<BUFF ID='200101' Name='����' Type='BUFF' Cancelable='False' EffectPoint='self' EffectTarget='allied' PassiveExtraAttrib='DISABLE_TALENTS' />");
			pBuffInfoMgr->CreateBuffInfo( L"<BUFF ID='200102' Name='��Ʈ' Type='BUFF' Cancelable='False' EffectPoint='self' EffectTarget='allied' PassiveExtraAttrib='ROOT' />");
		}
		
		void LearnedTalent()
		{
			XTalentInfo* pTalentInfo = info.talent->Get(20501);
			gvar.MyInfo.Talent.Train(pTalentInfo);

			pTalentInfo = info.talent->Get(50501);
			gvar.MyInfo.Talent.Train(pTalentInfo);

			pTalentInfo = info.talent->Get(CSTalentInfoHelper::NormalGuardTalentID(WEAPON_1H_SLASH, 0, false));
			gvar.MyInfo.Talent.Train(pTalentInfo);			
		}

		void CreateProjectileTalents()
		{
			MockTalentInfoMgr* pTalentInfoMgr = static_cast<MockTalentInfoMgr*>(info.talent);
	
			// ���� �ŷ�Ʈ
			pTalentInfoMgr->CreateTalent(L"<TALENT id='50501' name='���̾ Rank 1' TalentLine='50501' Rank='1' TalentType='skill' Category='magic' SkillType='magic'  Style='sorcerer' EffectTiming='hit_this_talent' ENCost='8' CoolTime='5' Stance='all' CastingAni='spell_area_cast' CastingLoopAni='spell_area_idle' CastingTime='1.6' UseAni='spell_area_fire' MF_BEATEN='100' MF_WEIGHT_BEATEN='100' MinDamage='80' MaxDamage='90' ResistMethod='partial' ResistType='FR' ResistDifficulty='190' CriticalEnable='True' DamageAttrib='fire' Guardable='False' Cancelable='True' HateAdj='50' ProjectileSpeed='2000' Range='30' ProjectileHitRadius='30' EffectPoint='target' EffectRadius='5' EffectTarget='enemy' ProjectileType='missile' />");

		}

		void CreateWeaponAndChangeToBattleStance()
		{
			// ���� ����
			XUnitTestHelper::CreateWeaponItemData(1, WEAPON_1H_SLASH);
			XUnitTestHelper::CreateItemAndEquip(m_pMyPlayer, 1, ITEMSLOT_RWEAPON);

			tGame.Update(0.1f);

			// ���� ���Ľ��� ��ȯ
			m_pMyPlayer->ChangeStance(CS_BATTLE, true);
		}

		void SendActSpellAction()
		{
			XEvent msg(XEVTL_VIRTUAL_KEY_PRESSED);
			XVirtualKey key = VKEY_ACTION;
			msg.m_pData = &key;
			m_pMyPlayer->Event(msg);
		}

		XTestInfoWrapper2<XTalentInfoMgr, MockTalentInfoMgr>	m_TalentInfoWrapper;
		XTestInfoWrapper2<XItemManager, MockItemInfoMgr>	m_ItemInfoWrapper;
		XTestInfoWrapper2<XNPCInfoMgr, MockNPCInfoMgr>		m_NPCInfoWrapper;
		XTestInfoWrapper2<XBuffInfoMgr, MockBuffInfoMgr>	m_BuffInfoWrapper;

		TestGame tGame;
		XMyPlayer* m_pMyPlayer;
		XModuleMyControl* m_pModuleMyControl;
		MockController* m_pController;
		XUnitTestHelper	helper;
	};

	TEST_FIXTURE(FMyPlayerActionFixture, TestMyActionTransToSwitchingWeaponState_PreCheck)
	{
		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);
		CHECK_EQUAL(m_pMyPlayer->IsCurrStanceBattle(), false);
	}

	TEST_FIXTURE(FMyPlayerActionFixture, TestMyActionTransToSwitchingWeaponState_Negative)
	{
		XEvent msg(XEVTL_VIRTUAL_KEY_PRESSED);
		XVirtualKey key = VKEY_CHANGE_WEAPON;
		msg.m_pData = &key;

		m_pMyPlayer->Event(msg);

		CHECK_EQUAL(m_pMyPlayer->GetWeaponSet(), 0);
		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);	// normal ���Ľ��̹Ƿ� �ٷ� idle
		CHECK_EQUAL(m_pNetAgent->GetSendCommandCount(), 0);		// ����2�� ���Ǿ� ���� �����Ƿ� Ŀ�ǵ� �Ⱥ���.
	}

	TEST_FIXTURE(FMyPlayerActionFixture, TestMyActionTransToSwitchingWeaponState)
	{
		// ���� ����
		XUnitTestHelper::CreateWeaponItemData(1, WEAPON_1H_SLASH);
		XUnitTestHelper::CreateWeaponItemData(2, WEAPON_TWO_HANDED);

		XUnitTestHelper::CreateItemAndEquip(m_pMyPlayer, 1, ITEMSLOT_RWEAPON);
		XUnitTestHelper::CreateItemAndEquip(m_pMyPlayer, 2, ITEMSLOT_RWEAPON2);

		XEvent msg(XEVTL_VIRTUAL_KEY_PRESSED);
		XVirtualKey key = VKEY_CHANGE_WEAPON;
		msg.m_pData = &key;

		m_pMyPlayer->Event(msg);

		CHECK_EQUAL(m_pMyPlayer->GetWeaponSet(), 0);		// ���� ������ 0. �ٲ㵵 �ȴٴ� Ŀ�ǵ尡 ����;� ��μ� 1
		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);	// normal ���Ľ��̹Ƿ� �ٷ� idle
		CHECK_EQUAL(m_pNetAgent->GetSendCommandCount(), 1);
		CHECK_EQUAL(m_pNetAgent->GetSendCommandID(0), MC_ACTION_SWITCHING_WEAPON_REQ);
		CHECK_EQUAL(m_pNetAgent->GetSendParam<int8>(0, 0), 1);

		// �������� ����ٲ� Ŀ�ǵ尡 ������� ��μ� ������� �ٲ�
		m_pNetAgent->OnRecv(MC_ACTION_SWITCHING_WEAPON, 2, NEW_USHORT(m_pMyPlayer->GetUIID()), NEW_CHAR(1));
		CHECK_EQUAL(m_pMyPlayer->GetWeaponSet(), 1);
	}


	TEST_FIXTURE(FMyPlayerActionFixture, TestMyActionRunningMotion)
	{
		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);

		tGame.Update(0.1f);
		CHECK(m_pMyPlayer->GetCurrentAnimationName() == wstring(L"none_idle"));

		m_pController->TestKeyPress(VKEY_FORWARD);

		tGame.Update(0.1f);

		CHECK(m_pMyPlayer->GetCurrentAnimationName() == wstring(L"none_run_f"));


		m_pController->TestKeyRelease(VKEY_FORWARD);


		CHECK(m_pMyPlayer->GetCurrentAnimationName() == wstring(L"none_idle"));
	}

	TEST_FIXTURE(FMyPlayerActionFixture, TestMyActionUseTalentComboAfterNormalAttack)
	{
		XTestInfoWrapper<XMotionMgr>	m_MotionInfoWrapper;
		CreateNormalAttackTalents();
		LearnedTalent();

		// ���� ����, ���Ľ� ��ȯ
		CreateWeaponAndChangeToBattleStance();

		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);
		CHECK_EQUAL(m_pMyPlayer->IsCurrStanceBattle(), true);

		TestAniDataSet& testAniDataSet = tGame.GetPlayerAniDataSet();
		CHECK(testAniDataSet.GetAniData(L"1hs_atk1") != NULL);
		CHECK(testAniDataSet.GetAniData(L"1hs_atk1_r") != NULL);

		// ���� Ű�� ������ 1Ÿ ����
		m_pController->TestKeyPress(VKEY_ACTION, 0.1f);
		tGame.Update(0.1f);

		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_NORMAL_ATTACK);

		tGame.Update(0.1f);
		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_NORMAL_ATTACK);
		CHECK(m_pMyPlayer->GetCurrentAnimationName() == wstring(L"1hs_atk1"));

		// �ŷ�Ʈ ���
		int nTalentID = 20501;
		m_pModuleMyControl->DoActionUseTalent(nTalentID);

		tGame.Update(0.1f);
		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_NORMAL_ATTACK);

		tGame.Update(0.2f);
		m_pModuleMyControl->DoActionUseTalent(nTalentID);	// �ŷ�Ʈ ��� ����

		// atk1�� 0.8�ʷ� ���õǾ� ����
		tGame.Update(0.4f);
		tGame.Update(0.1f); // ������Ʈ ���������� �ι� ������Ʈ �ؾ��Ѵ�.
		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_USE_TALENT);

	}

	TEST_FIXTURE(FMyPlayerActionFixture, TestComboOverlapped)
	{
		XTestInfoWrapper<XMotionMgr>	m_MotionInfoWrapper;
		CreateNormalAttackTalents();
		LearnedTalent();

		// ���� ����
		XUnitTestHelper::CreateWeaponItemData(1, WEAPON_1H_SLASH);
		XUnitTestHelper::CreateItemAndEquip(m_pMyPlayer, 1, ITEMSLOT_RWEAPON);

		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);

		tGame.Update(0.1f);

		// ���� ���Ľ��� ��ȯ
		m_pMyPlayer->ChangeStance(CS_BATTLE, true);
		CHECK_EQUAL(m_pMyPlayer->IsCurrStanceBattle(), true);

		// ���� Ű�� ������ 1Ÿ ����
		m_pController->TestKeyPress(VKEY_ACTION, 0.1f);
		tGame.Update(0.3f);

		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_NORMAL_ATTACK);


		// ���� Ű ����
		m_pController->TestKeyPress(VKEY_ACTION, 0.1f);
		tGame.Update(0.1f);

		// �ٽ� �ŷ�Ʈ ��� ����
		int nTalent = 20501;
		m_pModuleMyControl->DoActionUseTalent(nTalent);

		tGame.Update(0.1f);
		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_NORMAL_ATTACK);

		// atk1�� 0.8�ʷ� ���õǾ� ����
		tGame.Update(0.4f);
		tGame.Update(0.1f); // ������Ʈ ���������� �ι� ������Ʈ �ؾ��Ѵ�.
		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_USE_TALENT);

	}


	class TestMyActionNormalAttack : public XMyActionNormalAttack
	{
	public:
		using XMyActionNormalAttack::doAttack;
		using XMyActionNormalAttack::CheckCanReserveCombo;
		using XMyActionNormalAttack::GetTalentID;

		TestMyActionNormalAttack(XModuleMyControl* pOwner) : XMyActionNormalAttack(pOwner) {}
	};

	// ���� ���� �׽�Ʈ
	TEST_FIXTURE(FMyPlayerActionFixture, TestMyActionLoot_Begin)
	{
		CHECK_EQUAL(m_pNetAgent->GetSendCommandCount(), 0);

		TestModuleContext	testContext(&tGame, m_pNetAgent);

		XNonPlayer* pNPC = testContext.CreateNPCAndEnterToGame(1, vec3(1000.0f, 1000.0f, 0.0f));
		CHECK(pNPC != NULL);
		if (pNPC == NULL) return;

		MUID uidNPC = pNPC->GetUID();

		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);
		CHECK_EQUAL(m_pMyPlayer->IsCurrStanceBattle(), false);

		XPostLoot_Begin(uidNPC);
		CHECK_EQUAL(m_pNetAgent->GetSendCommandCount(), 1);
		CHECK_EQUAL(m_pNetAgent->GetSendCommand(0).GetID(), MC_LOOT_BEGIN_REQ);
	}
	
	TEST_FIXTURE(FMyPlayerActionFixture, TestMyActionLoot_SeeItem_normal)
	{
		CHECK_EQUAL(m_pNetAgent->GetSendCommandCount(), 0);

		TestModuleContext	testContext(&tGame, m_pNetAgent);

		XNonPlayer* pNPC = testContext.CreateNPCAndEnterToGame(1, vec3(1000.0f, 1000.0f, 0.0f));
		CHECK(pNPC != NULL);
		if (pNPC == NULL) return;

		//���ð�������
		pNPC->SetLootable(true);
		MUID uidNPC = pNPC->GetUID();

		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);
		CHECK_EQUAL(m_pMyPlayer->IsCurrStanceBattle(), false);

		m_pNetAgent->OnRecv(MC_LOOT_BEGIN, 3, NEW_UID(m_pMyPlayer->GetUID()), NEW_UID(uidNPC), NEW_FLOAT(.0f));
		CHECK_EQUAL(m_pNetAgent->GetSendCommandCount(), 1); //Ŀ�ǵ���� �ٸ��� ������ ����� �ȵǴ°�
		CHECK_EQUAL(m_pNetAgent->GetSendCommand(0).GetID(), MC_LOOT_SEE_ITEM_REQ);
	}

	TEST_FIXTURE(FMyPlayerActionFixture, TestMyActionLoot_SeeItem_battle)
	{
		CHECK_EQUAL(m_pNetAgent->GetSendCommandCount(), 0);

		TestModuleContext	testContext(&tGame, m_pNetAgent);

		XNonPlayer* pNPC = testContext.CreateNPCAndEnterToGame(1, vec3(1000.0f, 1000.0f, 0.0f));
		CHECK(pNPC != NULL);
		if (pNPC == NULL) return;


		//���ð�������
		pNPC->SetLootable(true);
		MUID uidNPC = pNPC->GetUID();

		// ���� ����, ���Ľ� ��ȯ
		CreateWeaponAndChangeToBattleStance();

		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);
		CHECK_EQUAL(m_pMyPlayer->IsCurrStanceBattle(), true);

		m_pNetAgent->OnRecv(MC_LOOT_BEGIN, 3, NEW_UID(m_pMyPlayer->GetUID()), NEW_UID(uidNPC), NEW_FLOAT(.0f));
		CHECK_EQUAL(m_pNetAgent->GetSendCommandCount(), 1);
		CHECK_EQUAL(m_pNetAgent->GetSendCommand(0).GetID(), MC_LOOT_SEE_ITEM_REQ);
	}

	TEST_FIXTURE(FMyPlayerActionFixture, TestMyActionLoot_GetItem)
	{
		XUnitTestHelper::CreateItemSwordInfo(200);

		vector<TD_ITEM_DROP> vecTDItem;
		TD_ITEM_DROP tdItem;
		tdItem.m_nDropItemUID = global.system->GenerateLocalUID();
		tdItem.m_nDropItemType = DIT_GETTABLE;
		tdItem.m_nItemID = 200;		
		tdItem.m_nAmount= 1;		
		vecTDItem.push_back(tdItem);

		m_pNetAgent->OnRecv(MC_LOOT_SEE_ITEM, 1, NEW_BLOB(vecTDItem));

		// ȭ�� ��
		// TODO : MockLua ���� ó���ϼ���.

		//	"1, 1"

		//////////////////////////////////////////////////////////////////////////
	}

	TEST_FIXTURE(FMyPlayerActionFixture, TestMyActionLoot_LootItem)
	{
		CHECK_EQUAL(m_pNetAgent->GetSendCommandCount(), 0);

		TestModuleContext	testContext(&tGame, m_pNetAgent);

		XNonPlayer* pNPC = testContext.CreateNPCAndEnterToGame(1, vec3(1000.0f, 1000.0f, 0.0f));
		CHECK(pNPC != NULL);
		if (pNPC == NULL) return;

		MUID uidNPC = pNPC->GetUID();

		vector<MUID> vecDropItemUID;
		vecDropItemUID.push_back(MUID());

		XPostLoot_LootItem(uidNPC, vecDropItemUID);
		CHECK_EQUAL(m_pNetAgent->GetSendCommandCount(), 1);
		CHECK_EQUAL(m_pNetAgent->GetSendCommand(0).GetID(), MC_LOOT_LOOT_ITEM_REQ);
	}

	class TestMyActionGuard : public XMyActionGuard
	{
	public:
		using class XMyActionGuard::XGuardReserver;
		TestMyActionGuard(XModuleMyControl* pOwner) : XMyActionGuard(pOwner) {}
	};

	TEST(TestMyActionGuardReserver)
	{
		TestMyActionGuard::XGuardReserver guardReserver;

		CHECK_EQUAL(guardReserver.IsReserved(), false);

		guardReserver.Reserve(NA_NORMAL1);
		CHECK_EQUAL(guardReserver.GetReservedAttackType(), NA_NORMAL1);
		CHECK_EQUAL(guardReserver.IsReserved(), true);

		guardReserver.Release();
		CHECK_EQUAL(guardReserver.IsReserved(), false);

	}

	TEST_FIXTURE(FMyPlayerActionFixture, TestMyActionUsetTalent_Projectile)
	{
		XTestInfoWrapper<XMotionMgr>	m_MotionInfoWrapper;
		CreateProjectileTalents();
		LearnedTalent();

		//-----------------------------------------------------------------------------------------------
		// NPC ����

		MCapsule NPCCapsule(vec3(0.0f, 0.0f, 40.0f), vec3(0.0f, 0.0f, 140.0f), 40.0f);

		TestModuleContext	testContext(&tGame, m_pNetAgent);

		XModuleEntity* pPlayerModuleEntity = testContext.GetModule<XModuleEntity>(m_pMyPlayer);
		CHECK(pPlayerModuleEntity);

		XNonPlayer* pNPC = testContext.CreateNPCAndEnterToGame(1, vec3(1000.0f, 1000.0f, 0.0f), vec3::AXISX, &NPCCapsule, L"testNPC");
		CHECK(pNPC != NULL);
		if (pNPC == NULL) return;


		XUnitTestHelper::NewNPCInfo(107001, L"�׽�ƮNPC", 3, L"testNPC", L"Monster/testNPC", L"monster");

		helper.SetMeshHitCapsule(L"testNPC", &NPCCapsule);
		//-----------------------------------------------------------------------------------------------

		// �� ��ġ ����
		m_pMyPlayer->SetPosition(vec3(900.0f, 1000.0f, 0.0f));

		// ���� My State üũ
		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);

		// �ŷ�Ʈ ���
		int TalentID = 50501;
		m_pModuleMyControl->DoActionUseTalent(TalentID);

		// �� ���� ����
		vec3 vMyDir = (vec3(1000.0f, 1000.0f, 0.0f) - m_pMyPlayer->GetPosition()).Normalize();

		m_pController->TestSetCameraDir(vMyDir);
		m_pModuleMyControl->SyncPlayerDirToCameraDir();

		tGame.Update(0.1f);


		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_USE_TALENT);

		// Use �ŷ�Ʈ ������.
		m_pNetAgent->OnRecv(MC_ACTION_USE_TALENT, 4, 
			NEW_USHORT( m_pMyPlayer->GetUIID() ), 
			NEW_INT( TalentID ), 
			NEW_SVEC(m_pMyPlayer->GetDirection() ),	
			NEW_VEC(m_pMyPlayer->GetPosition()));

		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_USE_TALENT);

		tGame.Update(1.6f);
		SendActSpellAction();

		CHECK_EQUAL(m_pNetAgent->GetSendCommand(3).GetID(), MC_ACTION_ACT_TALENT_WITH_HITCAPSULE_REQ);

		m_pNetAgent->OutputLog();

		info.npc->Clear();
	}

	TEST_FIXTURE(FMyPlayerActionFixture, TestMyActionUsetTalent_Projectile_Cancel)
	{
		XTestInfoWrapper<XMotionMgr>	m_MotionInfoWrapper;
		CreateProjectileTalents();
		LearnedTalent();

		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);

		// �ŷ�Ʈ ���
		int nTalentID = 50501;
		m_pModuleMyControl->DoActionUseTalent(nTalentID);

		tGame.Update(0.1f);
		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_USE_TALENT);

		// Use �ŷ�Ʈ ������.
		m_pNetAgent->OnRecv(MC_ACTION_USE_TALENT, 4, 
								NEW_USHORT( m_pMyPlayer->GetUIID() ), 
								NEW_INT( nTalentID ), 
								NEW_SVEC(m_pMyPlayer->GetDirection() ),	
								NEW_VEC(m_pMyPlayer->GetPosition()));

		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_USE_TALENT);

		tGame.Update(1.6f);
		SendActSpellAction();

		CHECK_EQUAL(m_pNetAgent->GetSendCommand(2).GetID(), MC_ACTION_CANCEL_TALENT_REQ);

		// Cancle�� �غ���
		m_pNetAgent->OnRecv(MC_ACTION_CANCEL_TALENT, 2, 
							NEW_USHORT( m_pMyPlayer->GetUIID() ), 
							NEW_INT( nTalentID ));
		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);
	}

	TEST_FIXTURE(FMyPlayerActionFixture, TestMyActionAttackOnDisabledBuff)
	{
		XTestInfoWrapper<XMotionMgr>	m_MotionInfoWrapper;
		CreateNormalAttackTalents();
		CreateWeaponAndChangeToBattleStance();

		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);

		// ���� ������ �ɸ�
		m_pNetAgent->OnRecv( MC_BUFF_GAIN, 3, NEW_USHORT( m_pMyPlayer->GetUIID()), NEW_INT( 200101), NEW_FLOAT( 1.0f));
		tGame.Update(0.1f);

		// ���� �õ�
		m_pController->TestKeyPress( VKEY_ACTION, 0.1f);
		tGame.Update( 0.1f);

		// ���ݵ��� �ʾ����� OK
		CHECK_EQUAL( m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);
	}

	TEST_FIXTURE(FMyPlayerActionFixture, TestMyActionUseTalentOnDisabledBuff)
	{
		XTestInfoWrapper<XMotionMgr>	m_MotionInfoWrapper;
		CreateNormalAttackTalents();
		CreateWeaponAndChangeToBattleStance();

		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);

		// ���� ������ �ɸ�
		m_pNetAgent->OnRecv( MC_BUFF_GAIN, 3, NEW_USHORT( m_pMyPlayer->GetUIID()), NEW_INT( 200101), NEW_FLOAT( 1.0f));
		tGame.Update( 0.1f);

		// �ŷ�Ʈ ���
		m_pModuleMyControl->DoActionUseTalent(101001);
		tGame.Update( 0.1f);

		// Ż��Ʈ�� ������ �ʾ����� OK
		CHECK_EQUAL( m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);
	}

	TEST_FIXTURE(FMyPlayerActionFixture, TestMyActionAttackOnRootBuff)
	{
		XTestInfoWrapper<XMotionMgr>	m_MotionInfoWrapper;
		CreateNormalAttackTalents();
		CreateWeaponAndChangeToBattleStance();

		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);

		// ��Ʈ ������ �ɸ�
		m_pNetAgent->OnRecv( MC_BUFF_GAIN, 3, NEW_USHORT( m_pMyPlayer->GetUIID()), NEW_INT( 200102), NEW_FLOAT( 1.0f));
		tGame.Update( 0.1f);

		// ���� �õ�
		m_pController->TestKeyPress( VKEY_ACTION, 0.1f);
		tGame.Update( 0.1f);

		// ���ݵ��� �ʾ����� OK
		CHECK_EQUAL( m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_DEACTIVATIVE);
	}

	TEST_FIXTURE(FMyPlayerActionFixture, TestMyActionUseTalentOnRootBuff)
	{
		XTestInfoWrapper<XMotionMgr>	m_MotionInfoWrapper;
		CreateNormalAttackTalents();
		CreateWeaponAndChangeToBattleStance();

		CHECK_EQUAL(m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);

		// ��Ʈ ������ �ɸ�
		m_pNetAgent->OnRecv( MC_BUFF_GAIN, 3, NEW_USHORT( m_pMyPlayer->GetUIID()), NEW_INT( 200102), NEW_FLOAT( 1.0f));
		tGame.Update( 0.1f);

		// �ŷ�Ʈ ���
		m_pModuleMyControl->DoActionUseTalent(101001);
		tGame.Update( 0.1f);

		// Ż��Ʈ�� ������ �ʾ����� OK
		CHECK_EQUAL( m_pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_DEACTIVATIVE);
	}

	TEST_FIXTURE(FMyPlayerActionFixture, TestMyActionMoveOnRootBuff)
	{
		XTestInfoWrapper<XMotionMgr>	m_MotionInfoWrapper;
		info.motion->AddMotionForTest(
			L"<MOTION name='run' stance='all' loop='true' cancelEnabled='true' cameraLook='true'>	\
				<Default>	<Sequence ani='run_f'/> </Default>	\
				<Forward>	<Sequence ani='run_f'/> </Forward>	\
				<Backward>	<Sequence ani='run_b'/> </Backward>	\
				<Left>		<Sequence ani='run_l'/> </Left>		\
				<Right>		<Sequence ani='run_r'/> </Right>	\
			</MOTION>");

		CreateNormalAttackTalents();
		tGame.Update( 0.1f);
		
		// ��Ʈ ������ �ɸ�
		m_pNetAgent->OnRecv( MC_BUFF_GAIN, 3, NEW_USHORT( m_pMyPlayer->GetUIID()), NEW_INT( 200102), NEW_FLOAT( 1.0f));
		tGame.Update( 0.1f);

		// ������ ����
		m_pController->TestKeyPress( VKEY_FORWARD);
		tGame.Update( 0.1f);

		// �̵����� �ʾ����� OK
		CHECK( wstring(L"none_idle") == m_pMyPlayer->GetCurrentAnimationName() );
	}

	// ���� ������ �ִϸ��̼��� �� ������ ���� ������Ʈ���� ���������� �Ѵ�.
	TEST_FIXTURE(FMyPlayerActionFixture, TestMyAction_GuardState_OnRelease)
	{
		XTestInfoWrapper<XMotionMgr>	m_MotionInfoWrapper;
		CreateNormalAttackTalents();
		LearnedTalent();

		// ���� ����, ���Ľ� ��ȯ
		CreateWeaponAndChangeToBattleStance();


		// ��� Ű�� ����
		m_pController->TestKeyPress(VKEY_GUARD);
		tGame.Update(0.1f);

		CHECK_EQUAL(ACTION_STATE_GUARD, m_pModuleMyControl->GetCurrentActionStateID());

		tGame.Update(0.1f);
		CHECK(wstring(L"1hs_guard_idle") == m_pMyPlayer->GetCurrentAnimationName());

		m_pController->TestKeyRelease(VKEY_GUARD);
		tGame.Update(0.1f);

		// Ű�� ���� �ٷ� IDLE ���°� �ƴ϶� ������ ���ƿ��� �ִϸ��̼��� ��� ������ IDLE�� ���ƿ´�.
		CHECK_EQUAL(ACTION_STATE_GUARD, m_pModuleMyControl->GetCurrentActionStateID());
		CHECK( wstring(L"1hs_guard_end") == m_pMyPlayer->GetCurrentAnimationName());

		tGame.Update(0.1f);
		tGame.Update(0.1f);
		tGame.Update(0.1f);
		tGame.Update(0.1f);

		CHECK_EQUAL(ACTION_STATE_IDLE, m_pModuleMyControl->GetCurrentActionStateID());
		CHECK( wstring(L"1hs_idle") == m_pMyPlayer->GetCurrentAnimationName());
	}
}