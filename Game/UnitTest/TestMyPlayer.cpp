#include "stdafx.h"

#include "TestModule.h"

#include "XMyPlayer.h"

#include "XNetwork.h"
#include "XModuleMyControl.h"
#include "XModuleMovable.h"
#include "XUnitTestUtil.h"
#include "RMesh.h"

#include "XTalentHitParam.h"

SUITE (MyPlayer)
{
	TEST( TestCreateMyControl )
	{
		TestGame tGame;
		XMyPlayer* pMyPlayer = tGame.GetMyPlayer();
		CHECK(pMyPlayer != NULL);
		if (pMyPlayer == NULL) return;

		XModuleMyControl* pModuleMyControl = static_cast<XModuleMyControl*>(pMyPlayer->GetModule( XMID_PLAYER_CONTROL ));
		CHECK(pModuleMyControl != NULL);
		if (pModuleMyControl)
		{
			// OnHit
			XTalentHitParam talentHitParam;

			talentHitParam.nAttackerUIID = XUnitTestUtil::NewUIID();
			talentHitParam.nDamage = 0;
			talentHitParam.nTalentID = 0;
			talentHitParam.nMFState = MF_BEATEN;
			talentHitParam.nMFWeight = 0;
			talentHitParam.nFlags = 0;

			pModuleMyControl->OnHit(&talentHitParam);

			pMyPlayer->Update(0.5f);
		}
	}

	TEST( TestMyPlayerTerrainHeightCollisionCheck )
	{
		// �����˻�� �ִ����(-100000.0f)�� üũ�ϴ� ���̴�.
		TestGame tGame;
		minet::MTestNetAgent tNetAgent(global.net->GetClient());
		TestModuleContext testContext(&tGame, &tNetAgent);

		XMyPlayer* pMyPlayer = testContext.GetMyPlayer();
		CHECK(pMyPlayer != NULL);

		// �ʱ� ��ġ ����
		pMyPlayer->SetPosition(vec3(1.0f, 1.0f, 0.0f));

		// ����
		XModuleMyControl* pModuleMyControl = testContext.GetModule<XModuleMyControl>(pMyPlayer);
		CHECK(pModuleMyControl != NULL);
		if (pModuleMyControl)
		{
			pModuleMyControl->OnWarp(vec3(1.0f, 1.0f, 25000.0f), pMyPlayer->GetDirection());
			pMyPlayer->Update(0.1f);
		}

		// �������� ���� Ȯ��
		XModuleMovable* pModuleMovable = testContext.GetModule<XModuleMovable>(pMyPlayer);
		const XMovableFactor::SState& sCurrentGeneralState = pModuleMovable->GetMovableFactor().GENERAL_STATE;

		CHECK_EQUAL(sCurrentGeneralState.m_bFallingButNotJumpReserved, false);
		CHECK_EQUAL(sCurrentGeneralState.m_bFallingButNotJump, true);
	}

	TEST(TestMyPlayerNormalMovingForward)
	{
		minet::MTestNetAgent	net_agent(global.net->GetClient());
		net_agent.IgnoreCmd(MC_FIELD_LOADING_COMPLETE);

		TestGame		tg;

		XMyPlayer* pMyPlayer = tg.GetMyPlayer();
		if (pMyPlayer == NULL) return;

		MockController* pController = tg.GetController();

		XUnitTestHelper helper;
		TestModuleContext testContext(&tg, &net_agent);

		XModuleEntity* pPlayerModuleEntity = testContext.GetModule<XModuleEntity>(pMyPlayer);
		CHECK(pPlayerModuleEntity);

		// Controller
		XModuleMyControl* pModuleMyControl = pMyPlayer->GetModuleMyControl();
		CHECK(pModuleMyControl != NULL);
		if (pModuleMyControl == NULL) return;

		// Movable
		XModuleMovable* pModuleMovable = (XModuleMovable*)pMyPlayer->GetModule(XMID_MOVABLE);
		CHECK(pModuleMovable != NULL);
		if (pModuleMovable == NULL) return;

		//////////////////////////////////////////////////////////////////////////

		pMyPlayer->SetPosition(vec3(1000.0f, 1000.0f, 0.0f));

		vec3 vMyDir = vec3(0.0f, 1.0f, 0.0f);

		pController->TestSetCameraDir(vMyDir);
		pModuleMyControl->SyncPlayerDirToCameraDir();

		CHECK_CLOSE(pMyPlayer->GetPosition().x, 1000.0f, 0.001f);
		CHECK_CLOSE(pMyPlayer->GetPosition().y, 1000.0f, 0.001f);
		CHECK_CLOSE(pMyPlayer->GetPosition().z, 0.0f, 0.001f);

		// �޽� �浹 ���� �Է�

		// ��Ű ����
		pController->TestSetCameraDir(vec3(0.0f, 1.0f, 0.0f));
		pController->TestKeyPress(VKEY_FORWARD);

		const float fDelta = 1.0f;
		tg.Update(fDelta);

		pController->TestKeyRelease(VKEY_FORWARD);
		float fMyPlayerDist = pMyPlayer->GetMoveSpeed(MOVEMENTFLAG_FORWARD) * fDelta;
		CHECK_CLOSE(pMyPlayer->GetPosition().y, 1000.0f + fMyPlayerDist, 0.001f);

		// MyCharacter�� �̵����϶� ����Ҽ� �ִ� MovableProc Ȯ���ϱ�.
		const XMovableFactor& movableFactor = pModuleMovable->GetMovableFactor();
		const XMovableFactor::SInterpolation& sInterpolation = movableFactor.INTERPOLATION;
		CHECK_EQUAL(sInterpolation.m_bWorking, false);

		CHECK_EQUAL(sInterpolation.m_bFinishMove, true);
		CHECK_EQUAL(sInterpolation.m_bFinishRotate, true);
	}

	TEST(TestMyPlayerNormalJump)
	{
		minet::MTestNetAgent	net_agent(global.net->GetClient());
		net_agent.IgnoreCmd(MC_FIELD_LOADING_COMPLETE);

		TestGame		tg;

		XMyPlayer* pMyPlayer = tg.GetMyPlayer();
		if (pMyPlayer == NULL) return;

		pMyPlayer->SetPosition(vec3(1000.0f, 1000.0f, 0.0f));
		pMyPlayer->SetDirection(vec3(0.0f, 1.0f, 0.0f));

		XModuleMyControl* pModuleMyControl = pMyPlayer->GetModuleMyControl();
		CHECK(pModuleMyControl != NULL);
		if (pModuleMyControl == NULL) return;
		XModuleMovable* pModuleMovable = (XModuleMovable*)pMyPlayer->GetModule(XMID_MOVABLE);
		CHECK(pModuleMovable != NULL);
		if (pModuleMovable == NULL) return;

		// ����ӵ� / ���ӵ�
		const vec3& vCurrentVel = pModuleMovable->GetMovableFactor().VELOCITY.m_vVelocity;
		const vec3& vCurrentAccel = pModuleMovable->GetMovableFactor().VELOCITY.m_vAccel;
		const XMovableFactor::SState& sCurrentGeneralState = pModuleMovable->GetMovableFactor().GENERAL_STATE;

		MockController* pController = tg.GetController();
		pController->TestSetCameraDir(vec3(0.0f, 1.0f, 0.0f));

		// �ʱ� �ӵ� - 0, 0, 0
		vec3 vInitVel = pModuleMovable->GetMovableFactor().VELOCITY.m_vVelocity;
		CHECK_CLOSE(vInitVel.x, 0.f, 0.001);
		CHECK_CLOSE(vInitVel.y, 0.f, 0.001);
		CHECK_CLOSE(vInitVel.z, 0.f, 0.001);

		// �ʱ� ���ӵ� 0, 0, GRAVITY
		vec3 vInitAccel = pModuleMovable->GetMovableFactor().VELOCITY.m_vAccel;
		CHECK_CLOSE(vInitAccel.x, 0.f, 0.001);
		CHECK_CLOSE(vInitAccel.y, 0.f, 0.001);
		CHECK_CLOSE(vInitAccel.z, GRAVITY, 0.001);

		// �ʱ� �浹 ����
		CHECK_EQUAL(sCurrentGeneralState.m_bColTerrain, false);
		CHECK_EQUAL(sCurrentGeneralState.m_bColObject, false);

		// ���� Ű ����
		pController->TestKeyPress(VKEY_JUMP);

		// ������ ���� ������ �ӵ�
		// ���� ���� �ӵ��� �ʱ� �ӵ������Ƿ� 0, 0, 0
		vec3 vecJumpVel = vec3(0*XCONST::MOVE_SPEED_NONE, 0*XCONST::MOVE_SPEED_NONE , XCONST::JUMP_SPEED);
		CHECK_CLOSE(vecJumpVel.x, vCurrentVel.x, 0.001);
		CHECK_CLOSE(vecJumpVel.y, vCurrentVel.y, 0.001);
		CHECK_CLOSE(vecJumpVel.z, vCurrentVel.z, 0.001);

		// ������ ���� �����Ǵ� ���ӵ�. �ʱ�� �����ؾ� �Ѵ�.
		CHECK_CLOSE(vInitAccel.x, vCurrentAccel.x, 0.001);
		CHECK_CLOSE(vInitAccel.y, vCurrentAccel.y, 0.001);
		CHECK_CLOSE(vInitAccel.z, vCurrentAccel.z, 0.001);

		//////////////////////////////////////////////////////////////////////////

		// �ð��� �����Ŀ� �ӵ� ����ϱ�
		float fElapsed = 0.01f;
		vec3 vecExpectedVel = vCurrentVel + (vCurrentAccel * fElapsed);

		// ������Ʈ
		tg.Update(fElapsed);
		CHECK_EQUAL(pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_JUMP);
		CHECK(net_agent.IsExistSendCommand(MC_ACTION_JUMP_REQ) == true);

		// ������Ʈ�� üũ
		CHECK(pMyPlayer->GetPosition().z > 0.0f);
		CHECK_CLOSE(vecExpectedVel.x, vCurrentVel.x, 0.001);
		CHECK_CLOSE(vecExpectedVel.y, vCurrentVel.y, 0.001);
		CHECK_CLOSE(vecExpectedVel.z, vCurrentVel.z, 0.001);


		// 0.01�̶�� �ð��̶��, �ö󰡰� �ִ� ���̴�.
		CHECK(vecExpectedVel.z > 0.f);
		CHECK_EQUAL(sCurrentGeneralState.m_bJumpAscending, true);
		CHECK_EQUAL(sCurrentGeneralState.m_bColTerrain, false);
		CHECK_EQUAL(sCurrentGeneralState.m_bColObject, false);


		// �ӵ��� z���� 0�� �Ǹ� �������� �����Ѵ�.
		// z �� 0 ������ fElapsed ���ϱ�
		fElapsed = - vCurrentVel.z / vCurrentAccel.z;
		fElapsed += 0.0001f;

		// ����(falling) �Ҷ����� ������Ʈ
		tg.Update(fElapsed);

		// �������ʹ� ����
		CHECK_EQUAL(sCurrentGeneralState.m_bJumping, true);
		CHECK_EQUAL(sCurrentGeneralState.m_bJumpAscending, false);

		// �����ؼ� �������� �Ͱ� �׳� IDLE ���¿��� �������°��� �ִ�.(�ִϸ��̼� ������� ���� �ٸ� ���� ó��)
		CHECK_EQUAL(sCurrentGeneralState.m_bFallingButNotJump, false);
		CHECK_EQUAL(sCurrentGeneralState.m_bLanding, false);
		CHECK_EQUAL(sCurrentGeneralState.m_bColTerrain, false);
		CHECK_EQUAL(sCurrentGeneralState.m_bColObject, false);

		// ����(landing) �Ҷ����� ������Ʈ.
		// 2�ʶ�� �ð��� �������� ������ �浹�� �ؼ� �����ϴµ� ����� �ð��̴�.
		// mock world���� ������ ������ ��ü�� ����.
		tg.Update(2.f);
		CHECK_EQUAL(sCurrentGeneralState.m_bColTerrain, true);
		CHECK_EQUAL(sCurrentGeneralState.m_bColObject, false);

		CHECK_EQUAL(sCurrentGeneralState.m_bJumping, false);
		CHECK_EQUAL(sCurrentGeneralState.m_bJumpAscending, false);
		CHECK_EQUAL(sCurrentGeneralState.m_bFallingButNotJump, false);
		CHECK_EQUAL(sCurrentGeneralState.m_bLanding, true);

		// �����ִϸ��̼� ���������� ������Ʈ
		// 10�ʶ�� �ð��� ���� �ִϸ��̼��� �����⿡ ����� �ð��̴�.
		// �ִϸ��̼��� ������ XModuleMotion::PlayDone�� ȣ��Ǹ� XMyAction  ���°� ������ IDLE�� ����ȴ�.
		tg.Update(10.f);
		CHECK_EQUAL(pModuleMyControl->GetCurrentActionStateID(), ACTION_STATE_IDLE);
	}

	TEST(TestMyPlayerKnockbackDie)
	{
		minet::MTestNetAgent	net_agent(global.net->GetClient());
		net_agent.IgnoreCmd(MC_FIELD_LOADING_COMPLETE);

		TestGame		tg;
		TestModuleContext testContext(&tg, &net_agent);
		XUnitTestHelper helper;

		XMyPlayer* pMyPlayer = tg.GetMyPlayer();
		if (pMyPlayer == NULL) return;

		pMyPlayer->SetPosition(vec3(1000.0f, 1000.0f, 0.0f));
		pMyPlayer->SetDirection(vec3(0.0f, 1.0f, 0.0f));

		// npc ���� �߰�
		XUnitTestHelper::NewNPCInfo(1);

		// NPC ����
		const MUID uidNPC = global.system->GenerateLocalUID();
		XNonPlayer* pNPC = helper.InNPC(uidNPC, 1, vec3(1000.0f, 1000.0f, 0.0f));

		// �ŷ�Ʈ ���� �߰�
		int ID = XUnitTestUtil::NewID();
		helper.CreateTalentInfo(ID, L"<TALENT id=\"211006\" name=\"���\" npc=\"110\" TalentType=\"skill\" Category=\"melee\" SkillType=\"melee\" \
									EffectTiming=\"hit_this_talent\" EffectRadius=\"2\" HitTestType=\"area\" HitMotionDelay=\"0\" TargetsAllowed=\"enemy\" \
									HPCost=\"0\" APCost=\"0\" ENCost=\"0\" STACost=\"0\" HPTickCost=\"0\" ENTickCost=\"0\" STATickCost=\"0\" CoolTime=\"5\" \
									UseAni=\"atk_d\" AttackDirection=\"front\" IsMoving=\"True\" IsSetDirection=\"True\" MF_BEATEN=\"50\" MF_WEIGHT_BEATEN=\"2000\" \
									MF_KNOCKBACK=\"0\" MF_WEIGHT_KNOCKBACK=\"0\" MF_STUN=\"0\" MF_WEIGHT_STUN=\"0\" MF_DEFLECT=\"0\" MF_WEIGHT_DEFLECT=\"0\" \
									MF_THROWUP=\"200\" MF_WEIGHT_THROWUP=\"1000\" MF_KNOCKDOWN=\"0\" MF_WEIGHT_KNOCKDOWN=\"0\" \
									MF_UPPERED=\"0\" MF_WEIGHT_UPPERED=\"0\" MinDamage=\"75\" MaxDamage=\"80\" DamageAttrib=\"blunt\" Guardable=\"True\" Cancelable=\"False\" />"
								);

		//////////////////////////////////////////////////////////////////////////
		// �˹� ��Ʈ�� ������.
		TD_TALENT_HIT	td_talent_hit;
		td_talent_hit.nAttackerUIID = pNPC->GetUIID();
		td_talent_hit.nVictimUIID = XGetMyUIID();
		td_talent_hit.nTalentID = 211006;
		td_talent_hit.nDamage = 45;
		td_talent_hit.nFlags = 0;
		td_talent_hit.nMFState = MF_KNOCKBACK;
		td_talent_hit.nMFWeight = 100.0;
		td_talent_hit.nHitTime = 0;

		net_agent.OnRecv(MC_ACTION_TALENT_HIT, 1, NEW_SINGLE_BLOB(&td_talent_hit, sizeof(TD_TALENT_HIT)));

		// �׾���.
		net_agent.OnRecv( MC_ACTION_DIE, 2, NEW_USHORT(XGetMyUIID()), NEW_UID(MUID::Invalid()));

		tg.Update(1.0f);

		// üũ
		// ���� �׼� ���°� ���� �����ΰ�?
		XModuleMyControl* pModuleMyControl = testContext.GetModule<XModuleMyControl>(pMyPlayer);
		CHECK(pModuleMyControl->GetCurrentActionStateID() == ACTION_STATE_DIE);

		helper.Clear();
	}

	TEST(TestMyPlayerStunDie)
	{
		// ���� �߿� �׾�����...
		minet::MTestNetAgent	net_agent(global.net->GetClient());
		net_agent.IgnoreCmd(MC_FIELD_LOADING_COMPLETE);

		TestGame		tg;
		TestModuleContext testContext(&tg, &net_agent);
		XUnitTestHelper helper;

		XMyPlayer* pMyPlayer = tg.GetMyPlayer();
		if (pMyPlayer == NULL) return;

		pMyPlayer->SetPosition(vec3(1000.0f, 1000.0f, 0.0f));
		pMyPlayer->SetDirection(vec3(0.0f, 1.0f, 0.0f));

		// npc ���� �߰�
		XUnitTestHelper::NewNPCInfo(1, L"����Ʈ ����", 3, L"goblin_mage", L"Monster/goblin_mage", L"monster");

		// NPC ����
		const MUID uidNPC = global.system->GenerateLocalUID();
		XNonPlayer* pNPC = helper.InNPC(uidNPC, 1, vec3(1000.0f, 1000.0f, 0.0f));

		// �ŷ�Ʈ ���� �߰�
		int ID = XUnitTestUtil::NewID();
		helper.CreateTalentInfo(ID, L"<TALENT id=\"211000111\" name=\"�������\" npc=\"110001\" TalentType=\"skill\" Category=\"melee\" SkillType=\"melee\" \
									EffectTiming=\"hit_this_talent\" EffectRadius=\"2\" HitTestType=\"area\" HitMotionDelay=\"0\" TargetsAllowed=\"enemy\" HPCost=\"0\"\
									APCost=\"0\" ENCost=\"0\" STACost=\"0\" HPTickCost=\"0\" ENTickCost=\"0\" STATickCost=\"0\" CoolTime=\"5\" UseAni=\"talent_fatalgas\" \
									AttackDirection=\"front\" MF_BEATEN=\"0\" MF_WEIGHT_BEATEN=\"0\" MF_KNOCKBACK=\"0\" MF_WEIGHT_KNOCKBACK=\"0\" MF_STUN=\"150\" MF_WEIGHT_STUN=\"5000\" \
									MF_DEFLECT=\"0\" MF_WEIGHT_DEFLECT=\"0\" MF_THROWUP=\"0\" MF_WEIGHT_THROWUP=\"0\" MF_KNOCKDOWN=\"0\" \
									MF_WEIGHT_KNOCKDOWN=\"0\" MF_UPPERED=\"0\" MF_WEIGHT_UPPERED=\"0\" MinDamage=\"50\" MaxDamage=\"50\" DamageAttrib=\"poison\" Guardable=\"False\" \
									Cancelable=\"False\" />"
									);

		//////////////////////////////////////////////////////////////////////////
		// ���� ��Ʈ�� ������.
		TD_TALENT_HIT	td_talent_hit;
		td_talent_hit.nAttackerUIID = pNPC->GetUIID();
		td_talent_hit.nVictimUIID = XGetMyUIID();
		td_talent_hit.nTalentID = 211000111;
		td_talent_hit.nDamage = 45;
		td_talent_hit.nFlags = 0;
		td_talent_hit.nMFState = MF_STUN;
		td_talent_hit.nMFWeight = 5.0f;
		td_talent_hit.nHitTime = 0;

		net_agent.OnRecv(MC_ACTION_TALENT_HIT, 1, NEW_SINGLE_BLOB(&td_talent_hit, sizeof(TD_TALENT_HIT)));

		XModuleMyControl* pModuleMyControl = testContext.GetModule<XModuleMyControl>(pMyPlayer);
		//////////////////////////////////////////////////////////////////////////
		// üũ
		// ���� �׼� ���°� ��Ʈ �����ΰ�?
		CHECK(pModuleMyControl->GetCurrentActionStateID() == ACTION_STATE_HIT);

		// �׾���.
		net_agent.OnRecv( MC_ACTION_DIE, 2, NEW_USHORT(XGetMyUIID()), NEW_UID(MUID::Invalid()));

		tg.Update(1.0f);

		//////////////////////////////////////////////////////////////////////////
		// üũ
		// ���� �׼� ���°� ���� �����ΰ�?
		CHECK(pModuleMyControl->GetCurrentActionStateID() == ACTION_STATE_DIE);

		helper.Clear();

	}
}
