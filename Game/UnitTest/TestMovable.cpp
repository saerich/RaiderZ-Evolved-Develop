#include "stdafx.h"
#include "XModuleTalent.h"
#include "XUnitTestList.h"
#include "XNPCInfo.h"
#include "XUnitTestHelper.h"
#include "XSystem.h"
#include "XCharacterCaption.h"
#include "MockNetClient.h"
#include "XNonPlayer.h"
#include "XModuleEntity.h"
#include "XModuleMotion.h"
#include "MockAnimationController.h"
#include "XUnitTestUtil.h"
#include "XMyPlayer.h"
#include "XConsoleCmds.h"
#include "XModuleEntity.h"
#include "XPost_Training.h"
#include "TestCommonFixture.h"
#include "TestModule.h"
#include "XMovableFactor.h"
#include "XModuleMovable.h"
#include "XModuleAction.h"
#include "RMesh.h"
#include "MockPlayer.h"
#include "XMovableProcObjectChecker.h"
#include "XNetwork.h"

SUITE(Movable)
{
	TEST(ColShoveTest)
	{
		// �б� �׽�Ʈ
		minet::MTestNetAgent	net_agent(global.net->GetClient());
		net_agent.IgnoreCmd(MC_FIELD_LOADING_COMPLETE);

		TestGame		tg;
		TestModuleContext testContext(&tg, &net_agent);
		XUnitTestHelper helper;

		XMyPlayer* pMyPlayer = tg.GetMyPlayer();
		if (pMyPlayer == NULL) return;

		pMyPlayer->SetPosition(vec3(1000.0f, 1000.0f, 0.0f));
		pMyPlayer->SetDirection(vec3(0.0f, 1.0f, 0.0f));

		XModuleCollision* pModuleCollision = testContext.GetModule<XModuleCollision>(pMyPlayer);
		CHECK(pModuleCollision);


		XModuleEntity* pPlayerModuleEntity = testContext.GetModule<XModuleEntity>(pMyPlayer);
		CHECK(pPlayerModuleEntity);

		//////////////////////////////////////////////////////////////////////////
		// npc ���� �߰�
		XUnitTestHelper::NewNPCInfo(107025, L"����Ʈ ����", 3, L"goblin_mage", L"Monster/goblin_mage", L"monster");

		// NPC ĸ�� �Է�
		MCapsule NPCCapsule(vec3(0.0f, 0.0f, 40.0f), vec3(0.0f, 0.0f, 150.0f), 40.0f);

		// NPC ����
		const MUID uidNPC = global.system->GenerateLocalUID();
		helper.InNPC(uidNPC, 107025, vec3(1000.0f, 980.0f, 0.0f), NULL, &NPCCapsule, L"goblin_mage");

		XNonPlayer* pNPC = gg.omgr->FindNPC(uidNPC);
		CHECK(pNPC);


		//////////////////////////////////////////////////////////////////////////
		// ���� ĸ���� ������ �ִ�. �׷��Ƿ� ������Ʈ�� �ϸ� My ĳ���ʹ� �з����� �Ѵ�.

		// ����
		vec3 vShoveDir = (pMyPlayer->GetPosition() - vec3(1000.0f, 980.0f, 0.0f)).Normalize();
		vec3 vMyPlayerShovePos = vec3(1000.0f, 980.0f, 0.0f) + vShoveDir * 80.0f;

		// ������Ʈ
		gg.omgr->Update(0.01f);
		vec3 vNewPos = pMyPlayer->GetPosition();

		CHECK_CLOSE(vMyPlayerShovePos.x, vNewPos.x, 0.001f);
		CHECK_CLOSE(vMyPlayerShovePos.y, vNewPos.y, 0.001f);

		info.npc->Clear();
	}

	TEST(ObjectCol_ColumnCapsule)
	{
		// ���� �浹 �׽�Ʈ
		minet::MTestNetAgent	net_agent(global.net->GetClient());
		net_agent.IgnoreCmd(MC_FIELD_LOADING_COMPLETE);

		TestGame		tg;
		TestModuleContext testContext(&tg, &net_agent);
		XUnitTestHelper helper;

		XMyPlayer* pMyPlayer = tg.GetMyPlayer();
		if (pMyPlayer == NULL) return;

		pMyPlayer->SetPosition(vec3(1000.0f, 1065.0f, 0.0f));
		pMyPlayer->SetDirection(vec3(0.0f, 1.0f, 0.0f));

		XModuleCollision* pModuleCollision = testContext.GetModule<XModuleCollision>(pMyPlayer);
		CHECK(pModuleCollision);

		XModuleEntity* pPlayerModuleEntity = testContext.GetModule<XModuleEntity>(pMyPlayer);
		CHECK(pPlayerModuleEntity);

		//////////////////////////////////////////////////////////////////////////
		// npc ���� �߰�
		XUnitTestHelper::NewNPCInfo(107025, L"����Ʈ ����", 3, L"goblin_mage", L"Monster/goblin_mage", L"monster");

		// NPC ����
		const MUID uidNPC = global.system->GenerateLocalUID();
		helper.InNPC(uidNPC, 107025, vec3(1000.0f, 980.0f, 0.0f));

		XNonPlayer* pNPC = gg.omgr->FindNPC(uidNPC);
		CHECK(pNPC);

		XModuleEntity* pNPCModuleEntity = testContext.GetModule<XModuleEntity>(pNPC);
		CHECK(pNPCModuleEntity);

		// ����
		vec3 vMoveDir = (pMyPlayer->GetPosition() - vec3(1000.0f, 980.0f, 0.0f)).Normalize();
		vec3 vMyPlayerMovePos = vec3(1000.0f, 980.0f, 0.0f) + vMoveDir * 80.0f;

		//////////////////////////////////////////////////////////////////////////
		// �̵��� �غ���
		// ��Ű ����
		MockController* pController = tg.GetController();

		pController->TestSetCameraDir(vec3(0.0f, -1.0f, 0.0f));
		pController->TestKeyPress(VKEY_FORWARD);

		const float fDelta = 0.05f;
		tg.Update(fDelta);

		pController->TestKeyRelease(VKEY_FORWARD);

		vec3 vNewPos = pMyPlayer->GetPosition();

		CHECK_CLOSE(vMyPlayerMovePos.x, vNewPos.x, 0.2f);
		CHECK_CLOSE(vMyPlayerMovePos.y, vNewPos.y, 0.2f);

		info.npc->Clear();
	}

	TEST(ObjectCol_RowCapsule)
	{
		// ���� �浹 �׽�Ʈ
		minet::MTestNetAgent	net_agent(global.net->GetClient());
		net_agent.IgnoreCmd(MC_FIELD_LOADING_COMPLETE);

		TestGame		tg;
		TestModuleContext testContext(&tg, &net_agent);
		XUnitTestHelper helper;

		XMyPlayer* pMyPlayer = tg.GetMyPlayer();
		if (pMyPlayer == NULL) return;

		pMyPlayer->SetPosition(vec3(1000.0f, 1065.0f, 0.0f));
		pMyPlayer->SetDirection(vec3(0.0f, 1.0f, 0.0f));

		XModuleCollision* pModuleCollision = testContext.GetModule<XModuleCollision>(pMyPlayer);
		CHECK(pModuleCollision);

		XModuleEntity* pPlayerModuleEntity = testContext.GetModule<XModuleEntity>(pMyPlayer);
		CHECK(pPlayerModuleEntity);

		// NPC ĸ�� �Է½� ����� �Ѵ�.
		// NPC ������ WorldTransform ������ ���� �����̴�.
		MCapsule NPCCapsule(vec3(0.0f, 0.0f, 40.0f), vec3(0.0f, 0.0f, 150.0f), 40.0f);

		//////////////////////////////////////////////////////////////////////////
		// npc ���� �߰�
		XUnitTestHelper::NewNPCInfo(107001, L"ȸ������", 3, L"wolf", L"Monster/wolf", L"monster");

		// NPC ����
		const MUID uidNPC = global.system->GenerateLocalUID();
		helper.InNPC(uidNPC, 107001, vec3(1000.0f, 980.0f, 0.0f), NULL, &NPCCapsule, L"goblin_mage");


		XNonPlayer* pNPC = gg.omgr->FindNPC(uidNPC);
		CHECK(pNPC);

		// ����
		vec3 vMoveDir = (pMyPlayer->GetPosition() - vec3(1000.0f, 980.0f, 0.0f)).Normalize();
		vec3 vMyPlayerMovePos = vec3(1000.0f, 980.0f, 0.0f) + vMoveDir * 80.0f;

		//////////////////////////////////////////////////////////////////////////
		// �̵��� �غ���
		// ��Ű ����
		MockController* pController = tg.GetController();

		pController->TestSetCameraDir(vec3(0.0f, -1.0f, 0.0f));
		pController->TestKeyPress(VKEY_FORWARD);

		const float fDelta = 0.05f;
		tg.Update(fDelta);

		pController->TestKeyRelease(VKEY_FORWARD);

		vec3 vNewPos = pMyPlayer->GetPosition();

		CHECK_CLOSE(vMyPlayerMovePos.x, vNewPos.x, 0.2f);
		CHECK_CLOSE(vMyPlayerMovePos.y, vNewPos.y, 0.2f);

		info.npc->Clear();
	}

	// ���� �� �ϰ���...
	//TEST(FAST_ObjectCol_ColumnCapsule)
	//{
	//	// ���� �浹 �׽�Ʈ
	//	TestNetAgent	net_agent;
	//	net_agent.IgnoreCmd(MC_FIELD_LOADING_COMPLETE);

	//	TestGame		tg;
	//	TestModuleContext testContext(&tg, &net_agent);
	//	XUnitTestHelper helper;

	//	XMyPlayer* pMyPlayer = tg.GetMyPlayer();
	//	if (pMyPlayer == NULL) return;

	//	pMyPlayer->SetPosition(vec3(1000.0f, 1100.0f, 0.0f));
	//	pMyPlayer->SetDirection(vec3(0.0f, 1.0f, 0.0f));

	//	XModuleCollision* pModuleCollision = testContext.GetModule<XModuleCollision>(pMyPlayer);
	//	CHECK(pModuleCollision);

	//	//////////////////////////////////////////////////////////////////////////
	//	// npc ���� �߰�
	//	info.npc->CreateNPCFromString("<NPC id=\"107025\" Name=\"����Ʈ ����\" Grade=\"3\" Type=\"monster\" MeshPath=\"Monster/goblin_mage\" MeshName=\"goblin_mage\"	/>");

	//	// NPC ����
	//	const MUID uidNPC = global.system->GenerateLocalUID();
	//	helper.InNPC(uidNPC, 107025, vec3(1000.0f, 980.0f, 0.0f));

	//	XNonPlayer* pNPC = gg.omgr->FindNPC(uidNPC);
	//	CHECK(pNPC);

	//	XModuleEntity* pNPCModuleEntity = testContext.GetModule<XModuleEntity>(pNPC);
	//	CHECK(pNPCModuleEntity);

	//	// ĸ�������� �����ö� �޽��� �̸��� �����Ѵ�. �׷��Ƿ� �ӽ÷� ������ְ� ������ ������ ����
	//	pNPCModuleEntity->GetActor()->m_pMesh = new RMesh("goblin_mage", NULL);

	//	// ����
	//	vec3 vMoveDir = (pMyPlayer->GetPosition() - vec3(1000.0f, 980.0f, 0.0f)).Normalize();
	//	vec3 vMyPlayerMovePos = vec3(1000.0f, 980.0f, 0.0f) + vMoveDir * 80.0f;

	//	//////////////////////////////////////////////////////////////////////////
	//	// �̵��� �غ���
	//	// ��Ű ����
	//	MockController* pController = tg.GetController();

	//	pController->TestSetCameraDir(vec3(0.0f, -1.0f, 0.0f));
	//	pController->TestKeyPress(VKEY_FORWARD);

	//	const float fDelta = 1.0f;
	//	tg.Update(fDelta);

	//	pController->TestKeyRelease(VKEY_FORWARD);

	//	vec3 vNewPos = pMyPlayer->GetPosition();

	//	CHECK_CLOSE(vMyPlayerMovePos.x, vNewPos.x, 0.001f);
	//	CHECK_CLOSE(vMyPlayerMovePos.y, vNewPos.y, 0.001f);

	//	// �ӽ÷� ������̹Ƿ� ����
	//	SAFE_DELETE(pNPCModuleEntity->GetActor()->m_pMesh);
	//}

	//TEST(FAST_ObjectCol_RowCapsule)
	//{
	//	// ���� �浹 �׽�Ʈ
	//	TestNetAgent	net_agent;
	//	net_agent.IgnoreCmd(MC_FIELD_LOADING_COMPLETE);

	//	TestGame		tg;
	//	TestModuleContext testContext(&tg, &net_agent);
	//	XUnitTestHelper helper;

	//	XMyPlayer* pMyPlayer = tg.GetMyPlayer();
	//	if (pMyPlayer == NULL) return;

	//	pMyPlayer->SetPosition(vec3(1000.0f, 1100.0f, 0.0f));
	//	pMyPlayer->SetDirection(vec3(0.0f, 1.0f, 0.0f));

	//	XModuleCollision* pModuleCollision = testContext.GetModule<XModuleCollision>(pMyPlayer);
	//	CHECK(pModuleCollision);

	//	//////////////////////////////////////////////////////////////////////////
	//	// npc ���� �߰�
	//	info.npc->CreateNPCFromString("<NPC id=\"107001\" Name=\"ȸ������\" Grade=\"3\" Type=\"monster\" MeshPath=\"Monster/wolf\" MeshName=\"wolf\"	/>");

	//	// NPC ����
	//	const MUID uidNPC = global.system->GenerateLocalUID();
	//	helper.InNPC(uidNPC, 107001, vec3(1000.0f, 980.0f, 0.0f));

	//	// NPC ĸ�� �Է½� ����� �Ѵ�.
	//	// NPC ������ WorldTransform ������ ���� �����̴�.
	//	MCapsule NPCCapsule(vec3(1070.0f, 980.0f, 40.0f), vec3(930.0f, 980.0f, 40.0f), 40.0f);

	//	// �޽� �浹 ���� �Է�
	//	helper.SetMeshColCasule("wolf", &NPCCapsule);

	//	XNonPlayer* pNPC = gg.omgr->FindNPC(uidNPC);
	//	CHECK(pNPC);

	//	XModuleEntity* pNPCModuleEntity = testContext.GetModule<XModuleEntity>(pNPC);
	//	CHECK(pNPCModuleEntity);

	//	// ĸ�������� �����ö� �޽��� �̸��� �����Ѵ�. �׷��Ƿ� �ӽ÷� ������ְ� ������ ������ ����
	//	pNPCModuleEntity->GetActor()->m_pMesh = new RMesh("goblin_mage", NULL);

	//	// ����
	//	vec3 vMoveDir = (pMyPlayer->GetPosition() - vec3(1000.0f, 980.0f, 0.0f)).Normalize();
	//	vec3 vMyPlayerMovePos = vec3(1000.0f, 980.0f, 0.0f) + vMoveDir * 80.0f;

	//	//////////////////////////////////////////////////////////////////////////
	//	// �̵��� �غ���
	//	// ��Ű ����
	//	MockController* pController = tg.GetController();

	//	pController->TestSetCameraDir(vec3(0.0f, -1.0f, 0.0f));
	//	pController->TestKeyPress(VKEY_FORWARD);

	//	const float fDelta = 1.0f;
	//	tg.Update(fDelta);

	//	pController->TestKeyRelease(VKEY_FORWARD);

	//	vec3 vNewPos = pMyPlayer->GetPosition();

	//	CHECK_CLOSE(vMyPlayerMovePos.x, vNewPos.x, 0.001f);
	//	CHECK_CLOSE(vMyPlayerMovePos.y, vNewPos.y, 0.001f);

	//	// �ӽ÷� ������̹Ƿ� ����
	//	SAFE_DELETE(pNPCModuleEntity->GetActor()->m_pMesh);
	//}

	TEST(TestNPCSetWarpData)
	{
		// NetPlayer, NonPlayer �� �����Խ� MovableFactor�� �ִ� Interpolation�� �ʱ�ȭ�� �۵��Ǵ��� �˻�
		minet::MTestNetAgent	net_agent(global.net->GetClient());
		net_agent.IgnoreCmd(MC_FIELD_LOADING_COMPLETE);

		TestGame		tg;
		TestModuleContext testContext(&tg, &net_agent);
		XUnitTestHelper helper;

		//////////////////////////////////////////////////////////////////////////
		// npc ���� �߰�
		XUnitTestHelper::NewNPCInfo(107001, L"ȸ������", 3, L"wolf", L"Monster/wolf", L"monster");


		// NPC ����
		const MUID uidNPC = global.system->GenerateLocalUID();
		helper.InNPC(uidNPC, 107001, vec3(1000.0f, 980.0f, 0.0f));

		XNonPlayer* pNPC = gg.omgr->FindNPC(uidNPC);
		CHECK(pNPC);

		vec3 vNPCPos = pNPC->GetPosition();

		//////////////////////////////////////////////////////////////////////////
		// 1. ������ �˻�
		XModuleMovable* pModuleMovable = testContext.GetModule<XModuleMovable>(pNPC);
		XMovableFactor MovableFactor = pModuleMovable->GetMovableFactor();

		CHECK_CLOSE(MovableFactor.INTERPOLATION.m_vTargetPos.x, vNPCPos.x, 0.001f);
		CHECK_CLOSE(MovableFactor.INTERPOLATION.m_vTargetPos.y, vNPCPos.y, 0.001f);
		CHECK_CLOSE(MovableFactor.INTERPOLATION.m_vTargetPos.z, vNPCPos.z, 0.001f);

		info.npc->Clear();
	}

	TEST(TestNETSetWarpData)
	{
		// NetPlayer, NonPlayer �� �����Խ� MovableFactor�� �ִ� Interpolation�� �ʱ�ȭ�� �۵��Ǵ��� �˻�
		minet::MTestNetAgent	net_agent(global.net->GetClient());
		net_agent.IgnoreCmd(MC_FIELD_LOADING_COMPLETE);

		TestGame		tg;
		TestModuleContext testContext(&tg, &net_agent);
		XUnitTestHelper helper;

		//////////////////////////////////////////////////////////////////////////
		// net ���� �߰�
		// net player
		XNetPlayer* pNetPlayer = testContext.CreateNetPlayerAndEnterToGmae(vec3(0.f, 0.f, -5.0f), vec3(1.f, 0.f, 0.f), SEX_FEMALE);
		CHECK(pNetPlayer != NULL);

		vec3 pNetPlayerPos = pNetPlayer->GetPosition();

		//////////////////////////////////////////////////////////////////////////
		// 1. ������ �˻�
		XModuleMovable* pModuleMovable = testContext.GetModule<XModuleMovable>(pNetPlayer);
		XMovableFactor MovableFactor = pModuleMovable->GetMovableFactor();

		CHECK_CLOSE(MovableFactor.INTERPOLATION.m_vTargetPos.x, pNetPlayerPos.x, 0.001f);
		CHECK_CLOSE(MovableFactor.INTERPOLATION.m_vTargetPos.y, pNetPlayerPos.y, 0.001f);
		CHECK_CLOSE(MovableFactor.INTERPOLATION.m_vTargetPos.z, pNetPlayerPos.z, 0.001f);

		info.npc->Clear();
	}


	TEST_FIXTURE(SimpleTestFixture, ObjectCol_OnKnockback)
	{
		m_pNetAgent->IgnoreCmd(MC_FIELD_LOADING_COMPLETE);

		TestModuleContext testContext(m_pGame, m_pNetAgent);

		XMyPlayer* pMyPlayer = m_pGame->GetMyPlayer();
		if (pMyPlayer == NULL) return;

		const vec3 vPlayerPos = vec3(9900.0f, 10000.0f, 0.0f);
		const vec3 vNPCPos = vec3(10000.0f, 10000.0f, 0.0f);
		const vec3 vNPCPos2 = vec3(9000.0f, 10000.0f, 0.0f);
		const vec3 vPlayerDir = vec3(1.0f, 0.0f, 0.0f);
		const vec3 vNPCDir = vec3(-1.0f, 0.0f, 0.0f);

		pMyPlayer->SetPosition(vPlayerPos);
		pMyPlayer->SetDirection(vec3(1.0f, 0.0f, 0.0f));

		XModuleCollision* pModuleCollision = testContext.GetModule<XModuleCollision>(pMyPlayer);
		CHECK(pModuleCollision);

		XModuleEntity* pPlayerModuleEntity = testContext.GetModule<XModuleEntity>(pMyPlayer);
		CHECK(pPlayerModuleEntity);

		//////////////////////////////////////////////////////////////////////////
		// npc ���� �߰�
		XUnitTestHelper::NewNPCInfo(107025, L"����Ʈ ����", 3, L"goblin_mage", L"Monster/goblin_mage", L"monster");

		// NPC ����
		const MUID uidNPC = global.system->GenerateLocalUID();
		const MUID uidNPC2 = global.system->GenerateLocalUID();
		m_Helper.InNPC(uidNPC, 107025, vNPCPos);
		m_Helper.InNPC(uidNPC2, 107025, vNPCPos2);


		XNonPlayer* pNPC = gg.omgr->FindNPC(uidNPC);
		CHECK(pNPC);
		XNonPlayer* pNPC2 = gg.omgr->FindNPC(uidNPC2);
		CHECK(pNPC2);


		pNPC->SetDirection(vec3(-1.0f, 0.0f, 0.0f));
		pNPC2->SetDirection(vec3(-1.0f, 0.0f, 0.0f));

		XModuleEntity* pNPCModuleEntity = testContext.GetModule<XModuleEntity>(pNPC);
		CHECK(pNPCModuleEntity);

		// knockback
		UIID nAttackerUIID = pNPC->GetUIID();
		MUID uidAttacker = pNPC->GetUID();
		int nWeight = 1000;
		vec3 vServerKnockbackDir = vNPCDir;

		// ����
		float fKnockbackDistance = 1000.0f;
		vec3 vMoveDir = vNPCDir;
		vec3 vHopeMyPlayerMovePos = vNPCPos2 + (vPlayerDir * 80.0f);

		stMotionFactorResult rtMotionFactor;
		stServerKnockbackVictimInfo knockbackInfo(vServerKnockbackDir, (vec3)vPlayerPos, vPlayerPos + (vServerKnockbackDir * nWeight)) ;

		pMyPlayer->GetModuleAction()->ActMF(MF_KNOCKBACK, 
			nWeight, 
			nAttackerUIID, 
			knockbackInfo,
			0,
			rtMotionFactor);

		for (int i = 0; i < 15; i++)
		{
			m_pGame->Update(0.1f);
		}



		CHECK_CLOSE(0.0f, pMyPlayer->GetPosition().DistanceTo(vHopeMyPlayerMovePos), 1.0f);

	}

}
