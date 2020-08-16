#include "stdafx.h"

#include "TestModule.h"

#include "XNetwork.h"
#include "XControlCmd.h"
#include "XModuleNetControl.h"
#include "XModuleEntity.h"
#include "XModuleMovable.h"
#include "XModuleTalent.h"
#include "RMesh.h"

SUITE(NetPlayer)
{
	TEST( TestCreateNetControl )
	{
		// build context
		TestGame			testGame;
		minet::MTestNetAgent		netAgent(global.net->GetClient());
		TestModuleContext	testContext(&testGame, &netAgent);
		testContext.MakeContextFieldLoadingCompleteIgnore();

		XNetPlayer* pNetPlayer = testContext.CreateNetPlayerAndEnterToGmae(vec3(0.f, 0.f, 0.0f), vec3(1.f, 0.f, 0.f), SEX_FEMALE);

		// net control
		XModuleNetControl* pModuleNetControl= testContext.GetModule<XModuleNetControl>(pNetPlayer);
		CHECK( pModuleNetControl != NULL);
		return;
	}

	TEST( TestNetControlOnUseTalent)
	{
		// build context
		TestGame			testGame;
		minet::MTestNetAgent		netAgent(global.net->GetClient());
		TestModuleContext	testContext(&testGame, &netAgent);
		testContext.MakeContextFieldLoadingCompleteIgnore();

		// net player
		XNetPlayer* pNetPlayer = testContext.CreateNetPlayerAndEnterToGmae(vec3(0.f, 0.f, 0.0f), vec3(1.f, 0.f, 0.f), SEX_FEMALE);
		CHECK(pNetPlayer != NULL);

		// talent
		bool bTalent = testContext.CreateTalent(L"<TALENT id=\"101071\" name=\"1hs �Ĺ� ȸ��\" TalentType=\"skill\" Category=\"melee\" SkillType=\"melee\" EffectRadius=\"3\" HitTestType=\"area\" HitMotionDelay=\"0\" HPCost=\"0\" APCost=\"0\" ENCost=\"0\" STACost=\"10\" HPTickCost=\"0\" ENTickCost=\"0\" STATickCost=\"0\" CoolTime=\"0\" Stance=\"all\" UseAni=\"PS_dodge_back\" AttackDirection=\"front\" InvincibleTimeStart=\"0.2\" InvincibleTimeEnd=\"0.5\" MF_BEATEN=\"0\" MF_WEIGHT_BEATEN=\"0\" MF_KNOCKBACK=\"0\" MF_WEIGHT_KNOCKBACK=\"0\" MF_STUN=\"0\" MF_WEIGHT_STUN=\"0\" MF_DEFLECT=\"0\" MF_WEIGHT_DEFLECT=\"0\" MF_THROWUP=\"0\" MF_WEIGHT_THROWUP=\"0\" MF_KNOCKDOWN=\"0\" MF_WEIGHT_KNOCKDOWN=\"0\" MF_UPPERED=\"0\" MF_WEIGHT_UPPERED=\"0\" MinDamage=\"0\" MaxDamage=\"0\" DamageAttrib=\"slash\" Guardable=\"True\" Cancelable=\"True\" NeededTP=\"0\" />");
		CHECK_EQUAL(bTalent, true);

		// Module
		// net control
		XModuleNetControl* pModuleNetControl= testContext.GetModule<XModuleNetControl>(pNetPlayer);
		CHECK( pModuleNetControl != NULL);
		if (pModuleNetControl == NULL) return;

		// movable control
		XModuleMovable* pModuleMovable = testContext.GetModule<XModuleMovable>(pNetPlayer);
		CHECK( pModuleMovable != NULL);
		if (pModuleMovable == NULL) return;

		//////////////////////////////////////////////////////////////////////////
		// 1. ���� MovableFactor üũ
		//
		const XMovableFactor::SState& sCurrentGeneralState = pModuleMovable->GetMovableFactor().GENERAL_STATE;
		const XMovableFactor::SInterpolation& sCurrentInterpolation = pModuleMovable->GetMovableFactor().INTERPOLATION;

		CHECK_EQUAL(sCurrentInterpolation.m_bWorking, false);

		CHECK_EQUAL(sCurrentInterpolation.m_bFinishMove, true);
		CHECK_EQUAL(sCurrentInterpolation.m_bFinishRotate, true);

		// use talent message
		MUID uidNetPlayer = pNetPlayer->GetUID();
		vec3 vNetPlayerDir = pNetPlayer->GetDirection();
		vec3 vNetPlayerPos = pNetPlayer->GetPosition();
		vec3 vNetPlayerRecvPos = vNetPlayerPos + vec3(10, 0, 0);

		netAgent.OnRecv( MC_ACTION_USE_TALENT, 4,

			NEW_USHORT( pNetPlayer->GetUIID() ),
			NEW_INT(101071),
			NEW_SVEC( vNetPlayerDir ),
			NEW_VEC( vNetPlayerRecvPos)
			);

		//////////////////////////////////////////////////////////////////////////
		// Talent ��� �� MovableFactor üũ, Interpolation�� ����Ѵ�.
		//
		testContext.UpdateGame(0.1f);
		CHECK_EQUAL(sCurrentGeneralState.m_bJumping, false);
		CHECK_EQUAL(sCurrentGeneralState.m_bFallingButNotJump, false);
		XModuleTalent* pModuleTalent = testContext.GetModule<XModuleTalent>(pNetPlayer);
		CHECK_EQUAL(pModuleTalent->IsActive(), true );

		// OnUseTalent�� ���������̼� Ÿ�ٿ� ���� ��ǥ�� �����Ѵ�.
		// �׷��� Ż��Ʈ ����߿��� ���������̼� ����
		CHECK_EQUAL(sCurrentGeneralState.m_bGoingDirection, false);
		//CHECK_EQUAL(sCurrentInterpolation.m_bUseInterpolation, true);
		CHECK_EQUAL(sCurrentInterpolation.m_bForceInterpolation, false);
		//CHECK_EQUAL(pModuleMovable->GetMovableFactor().IsInterpolationIgnoringState(), true);
		//CHECK_CLOSE(sCurrentInterpolation.m_vTarPos.x, vNetPlayerPos.x, 0.001);
		CHECK_CLOSE(sCurrentInterpolation.m_vTargetPos.y, vNetPlayerPos.y, 0.001);
		CHECK_CLOSE(sCurrentInterpolation.m_vTargetPos.z, vNetPlayerPos.z, 0.001);

		//////////////////////////////////////////////////////////////////////////
		// TODO : OnUseTalent�ÿ� �ӵ��� ��� �Ǿ�� �ϴ°�.
		//

		testContext.TalentClear();
	}

	TEST( TestNetControlOnCancelTalent)
	{
		// build context
		TestGame			testGame;
		minet::MTestNetAgent		netAgent(global.net->GetClient());
		TestModuleContext	testContext(&testGame, &netAgent);
		testContext.MakeContextFieldLoadingCompleteIgnore();

		// net player
		XNetPlayer* pNetPlayer = testContext.CreateNetPlayerAndEnterToGmae(vec3(0.f, 0.f, 0.0f), vec3(1.f, 0.f, 0.f), SEX_FEMALE);
		CHECK(pNetPlayer != NULL);

		// talent
		bool bTalent = testContext.CreateTalent(L"<TALENT id=\"101071\" name=\"1hs �Ĺ� ȸ��\" TalentType=\"skill\" Category=\"melee\" SkillType=\"melee\" EffectRadius=\"3\" HitTestType=\"area\" HitMotionDelay=\"0\" HPCost=\"0\" APCost=\"0\" ENCost=\"0\" STACost=\"10\" HPTickCost=\"0\" ENTickCost=\"0\" STATickCost=\"0\" CoolTime=\"0\" Stance=\"all\" UseAni=\"PS_dodge_back\" AttackDirection=\"front\" InvincibleTimeStart=\"0.2\" InvincibleTimeEnd=\"0.5\" MF_BEATEN=\"0\" MF_WEIGHT_BEATEN=\"0\" MF_KNOCKBACK=\"0\" MF_WEIGHT_KNOCKBACK=\"0\" MF_STUN=\"0\" MF_WEIGHT_STUN=\"0\" MF_DEFLECT=\"0\" MF_WEIGHT_DEFLECT=\"0\" MF_THROWUP=\"0\" MF_WEIGHT_THROWUP=\"0\" MF_KNOCKDOWN=\"0\" MF_WEIGHT_KNOCKDOWN=\"0\" MF_UPPERED=\"0\" MF_WEIGHT_UPPERED=\"0\" MinDamage=\"0\" MaxDamage=\"0\" DamageAttrib=\"slash\" Guardable=\"True\" Cancelable=\"True\" NeededTP=\"0\" />");
		CHECK_EQUAL(bTalent, true);

		MUID uidNetPlayer = pNetPlayer->GetUID();
		vec3 vNetPlayerDir = pNetPlayer->GetDirection();
		vec3 vNetPlayerPos = pNetPlayer->GetPosition();
		netAgent.OnRecv( MC_ACTION_CANCEL_TALENT, 2,

			NEW_USHORT( pNetPlayer->GetUIID() ),
			NEW_INT(101071)
			);

		testContext.UpdateGame(0.1f);
		XModuleTalent* pModuleTalent = testContext.GetModule<XModuleTalent>(pNetPlayer);
		CHECK_EQUAL(pModuleTalent->IsActive(), false );

		XModuleMovable* pModuleMovable = testContext.GetModule<XModuleMovable>(pNetPlayer);
		const XMovableFactor::SInterpolation& sCurrentInterpolation = pModuleMovable->GetMovableFactor().INTERPOLATION;
		CHECK_EQUAL(pModuleMovable->GetMovableFactor().IsInterpolationIgnoringState(), false);
		CHECK_CLOSE(sCurrentInterpolation.m_vTargetPos.x, vNetPlayerPos.x, 0.001);
		CHECK_CLOSE(sCurrentInterpolation.m_vTargetPos.y, vNetPlayerPos.y, 0.001);
		CHECK_CLOSE(sCurrentInterpolation.m_vTargetPos.z, vNetPlayerPos.z, 0.001);


		//////////////////////////////////////////////////////////////////////////
		// TODO : OnCancelTalent�ÿ� �ӵ��� ��� �Ǿ�� �ϴ°�.
		//

		testContext.TalentClear();
	}

	TEST(TestNetControlOnDie)
	{
		// build context
		TestGame			testGame;
		minet::MTestNetAgent		netAgent(global.net->GetClient());
		TestModuleContext	testContext(&testGame, &netAgent);
		testContext.MakeContextFieldLoadingCompleteIgnore();

		// net player
		XNetPlayer* pNetPlayer = testContext.CreateNetPlayerAndEnterToGmae(vec3(0.f, 0.f, 0.0f), vec3(1.f, 0.f, 0.f), SEX_FEMALE);
		CHECK(pNetPlayer != NULL);

		// ó���� idle �ִϸ��̼�
		CHECK(pNetPlayer->GetModuleMotion()->GetCurrMotion() == MOTION_NAME_IDLE);

		MUID uidNetPlayer = pNetPlayer->GetUID();
		vec3 vNetPlayerDir = pNetPlayer->GetDirection();
		vec3 vNetPlayerPos = pNetPlayer->GetPosition();
		netAgent.OnRecv( MC_ACTION_DIE, 2, NEW_USHORT(pNetPlayer->GetUIID()), NEW_UID(MUID::Invalid()));

		testGame.Update(0.1f);

		// �ִϸ��̼� üũ
		CHECK(pNetPlayer->GetModuleMotion()->GetCurrMotion() == MOTION_NAME_DIE);
		CHECK(pNetPlayer->GetModuleEntity()->GetAnimationController()->GetAnimationName() == wstring(L"none_die"));


		//////////////////////////////////////////////////////////////////////////
		// 1. �׾����� Movable�� Stop�� �Ǵ� �÷��� üũ
		//
		XModuleMovable* pModuleMovable = testContext.GetModule<XModuleMovable>(pNetPlayer);
		const XMovableFactor::SState& sCurrentGeneralState = pModuleMovable->GetMovableFactor().GENERAL_STATE;
		const XMovableFactor::SVelocity& sCurrentVelocity = pModuleMovable->GetMovableFactor().VELOCITY;
		
		CHECK_EQUAL(sCurrentGeneralState.m_bGoingDirection, false);
		CHECK_CLOSE( sCurrentVelocity.m_vVelocity.x, 0.f, 0.001f );
		CHECK_CLOSE( sCurrentVelocity.m_vVelocity.y, 0.f, 0.001f );
		CHECK_CLOSE( sCurrentVelocity.m_vVelocity.z, 0.f, 0.001f );
	}

	TEST(TestNetControlOnGuard)
	{
		// build context
		TestGame			testGame;
		minet::MTestNetAgent		netAgent(global.net->GetClient());
		TestModuleContext	testContext(&testGame, &netAgent);
		testContext.MakeContextFieldLoadingCompleteIgnore();

		// net player
		XNetPlayer* pNetPlayer = testContext.CreateNetPlayerAndEnterToGmae(vec3(0.f, 0.f, 0.0f), vec3(1.f, 0.f, 0.f), SEX_FEMALE);
		CHECK(pNetPlayer != NULL);

		MUID uidNetPlayer = pNetPlayer->GetUID();
		vec3 vNetPlayerDir = pNetPlayer->GetDirection();
		vec3 vNetPlayerPos = pNetPlayer->GetPosition() + vec3(100, 0, 0);
		netAgent.OnRecv( MC_ACTION_GUARD, 3,
			NEW_USHORT( pNetPlayer->GetUIID() ),
			NEW_SVEC2( vNetPlayerDir),
			NEW_VEC( vNetPlayerPos)
			);

		//////////////////////////////////////////////////////////////////////////
		// 1. ����� Movable�� Stop�� �Ǵ� �÷��� üũ
		//
		XModuleMovable* pModuleMovable = testContext.GetModule<XModuleMovable>(pNetPlayer);
		const XMovableFactor::SState& sCurrentGeneralState = pModuleMovable->GetMovableFactor().GENERAL_STATE;
		const XMovableFactor::SVelocity& sCurrentVelocity = pModuleMovable->GetMovableFactor().VELOCITY;

		CHECK_EQUAL(sCurrentGeneralState.m_bGoingDirection, false);
		CHECK_CLOSE( sCurrentVelocity.m_vVelocity.x, 0.f, 0.001f );
		CHECK_CLOSE( sCurrentVelocity.m_vVelocity.y, 0.f, 0.001f );
		CHECK_CLOSE( sCurrentVelocity.m_vVelocity.z, 0.f, 0.001f );
	}

	TEST(TestNetControlOnMove)
	{
		// build context
		TestGame			testGame;
		minet::MTestNetAgent		netAgent(global.net->GetClient());
		TestModuleContext	testContext(&testGame, &netAgent);
		testContext.MakeContextFieldLoadingCompleteIgnore();

		// net player
		XNetPlayer* pNetPlayer = testContext.CreateNetPlayerAndEnterToGmae(vec3(0.f, 0.f, 0.0f), vec3(1.f, 0.f, 0.f), SEX_FEMALE);
		CHECK(pNetPlayer != NULL);

		MUID uidNetPlayer = pNetPlayer->GetUID();
		UIID nNetPlayerUIID = pNetPlayer->GetUIID();
		vec3 vNetPlayerDir = pNetPlayer->GetDirection();
		vec3 vNetPlayerPos = pNetPlayer->GetPosition();

		TD_PC_MOVE mi;
		mi.vTarPos	= vNetPlayerPos + vNetPlayerDir * (PROCESS_MOVE_EXCEPTION_DISTANCE - 1.0f);
		mi.svDir.SetX(vNetPlayerDir.x);
		mi.svDir.SetY(vNetPlayerDir.y);
		mi.nFlags	= MOVEMENTFLAG_FORWARD;

		netAgent.OnRecv( MC_ACTION_MOVE, 2,

			NEW_USHORT( nNetPlayerUIID ),
			NEW_SINGLE_BLOB(&mi, sizeof(TD_PC_MOVE))
			);

		// ������Ʈ�� ���־�� XModuleNetControl�� Histroy�� ���� Move �迭 ����� ���ŵȴ�.
		testContext.UpdateGame(1.0f);

		//////////////////////////////////////////////////////////////////////////
		// 1. �̵��� ���� üũ
		//
		XModuleMovable* pModuleMovable = testContext.GetModule<XModuleMovable>(pNetPlayer);
		XModuleNetControl* pModuleNetControl = testContext.GetModule<XModuleNetControl>(pNetPlayer);

		// ���� ��ġ ���ϱ�
		float speed = pNetPlayer->GetMoveSpeed( mi.nFlags);
		vec3 vExpectPos = vNetPlayerPos + vNetPlayerDir * (PROCESS_MOVE_EXCEPTION_DISTANCE - 1.0f) + vNetPlayerDir * VIRTUAL_POSITION_VELOCITY;

		// �� ��ġ ��������
		vec3 vCurrentPos = pNetPlayer->GetPosition();

		CHECK_CLOSE( vExpectPos.x, vCurrentPos.x, 0.001f );
		CHECK_CLOSE( vExpectPos.y, vCurrentPos.y, 0.001f );
		CHECK_CLOSE( vExpectPos.z, vCurrentPos.z, 0.001f );
	}

	///////////////////////////////////////////////////////////////////////


	TEST(TestNetControlOnStop)
	{
		// build context
		TestGame			testGame;
		minet::MTestNetAgent		netAgent(global.net->GetClient());
		TestModuleContext	testContext(&testGame, &netAgent);
		testContext.MakeContextFieldLoadingCompleteIgnore();

		// net player
		XNetPlayer* pNetPlayer = testContext.CreateNetPlayerAndEnterToGmae(vec3(0.f, 0.f, 0.0f), vec3(1.f, 0.f, 0.f), SEX_FEMALE);
		CHECK(pNetPlayer != NULL);

		MUID uidNetPlayer = pNetPlayer->GetUID();
		vec3 vNetPlayerDir = pNetPlayer->GetDirection();
		vec3 vNetPlayerPos = pNetPlayer->GetPosition();

		////////////////////////////////////////////////////////////////////////////
		//// 1. ������ ��ǥ�� ��ġ�Ҷ�
		////
		//testContext.GetNetAgent().OnRecv( MC_ACTION_STOP, 3,

		//	NEW_UID( uidNetPlayer ),
		//	NEW_VEC(vNetPlayerPos),
		//	NEW_VEC(vNetPlayerDir)
		//	);

		//// ������Ʈ�� ���־�� XModuleNetControl�� Histroy�� ���� Move �迭 ����� ���ŵȴ�.
		//testContext.UpdateGame(HISTORY_UPDATE_TIME);

		//XModuleMovable* pModuleMovable = testContext.GetModule<XModuleMovable>(pNetPlayer);
		//XModuleNetControl* pModuleNetControl = testContext.GetModule<XModuleNetControl>(pNetPlayer);
		//XMovableFactor MovableFactor = pModuleMovable->DumpMovableFactor();

		//CHECK_EQUAL(MovableFactor.m_bGoing, false);
		//CHECK_EQUAL(MovableFactor.m_bMovingByTarget, false);
		//CHECK_CLOSE( MovableFactor.m_vVelocity.x, 0.f, 0.001f );
		//CHECK_CLOSE( MovableFactor.m_vVelocity.y, 0.f, 0.001f );
		//CHECK_CLOSE( MovableFactor.m_vVelocity.z, 0.f, 0.001f );

		////////////////////////////////////////////////////////////////////////////
		//// 2. ������ ��ǥ�� ��ġ���� ������
		////
		//TD_PC_MOVE mi;
		//mi.vTarPos	= vNetPlayerPos;			// �������� ������ ���� ��ġ�� ������.
		//mi.vDir		= vNetPlayerDir;
		//mi.nFlags	= MOVEMENTFLAG_FORWARD;

		//testContext.GetNetAgent().OnRecv( MC_ACTION_MOVE, 2,

		//	NEW_UID( uidNetPlayer ),
		//	NEW_SINGLE_BLOB(&mi, sizeof(TD_PC_MOVE))
		//	);

		//vec3 vNewNetPlayerPos = vNetPlayerPos + vec3(100.0f, 0.0f, 0.0f);
		//testContext.GetNetAgent().OnRecv( MC_ACTION_STOP, 3,

		//	NEW_UID( uidNetPlayer ),
		//	NEW_VEC(vNewNetPlayerPos),
		//	NEW_VEC(vNetPlayerDir)
		//	);

		//// ������Ʈ�� ���־�� XModuleNetControl�� Histroy�� ���� Move �迭 ����� ���ŵȴ�.
		//testContext.UpdateGame(HISTORY_UPDATE_TIME);

		//MovableFactor = pModuleMovable->DumpMovableFactor();
		//float fCurrentSpeed = pNetPlayer->GetMoveSpeed(mi.nFlags) * pModuleNetControl->GetBuffMoveSpeed() * 1.5f;
		//vec3 vDir = (vNewNetPlayerPos - vNetPlayerPos).Normalize();
		//vec3 vExpectVelocity = vec3(vDir.x, vDir.y, 0.0f) * fCurrentSpeed;

		//CHECK_EQUAL(MovableFactor.m_bGoing, false);
		//CHECK_EQUAL(MovableFactor.m_bMovingByTarget, true);
		//CHECK_CLOSE( MovableFactor.m_vVelocity.x, vExpectVelocity.x, 0.001f );
		//CHECK_CLOSE( MovableFactor.m_vVelocity.y, vExpectVelocity.y, 0.001f );
		//CHECK_CLOSE( MovableFactor.m_vVelocity.z, 0.f, 0.001f );
	}

	TEST(TestNetControlOnMoveStop)
	{
		// build context
		TestGame			testGame;
		minet::MTestNetAgent		netAgent(global.net->GetClient());
		TestModuleContext	testContext(&testGame, &netAgent);
		testContext.MakeContextFieldLoadingCompleteIgnore();

		// net player
		XNetPlayer* pNetPlayer = testContext.CreateNetPlayerAndEnterToGmae(vec3(0.f, 0.f, 0.0f), vec3(1.f, 0.f, 0.f), SEX_FEMALE);
		CHECK(pNetPlayer != NULL);

		MUID uidNetPlayer = pNetPlayer->GetUID();
		vec3 vNetPlayerDir = pNetPlayer->GetDirection();
		vec3 vNetPlayerPos = pNetPlayer->GetPosition();

		////////////////////////////////////////////////////////////////////////////
		//// 1. ������ ��ǥ�� ��ġ�Ҷ�
		////
		//TD_PC_MOVE mi;
		//mi.vTarPos	= vNetPlayerPos;			// �������� ������ ���� ��ġ�� ������.
		//mi.vDir		= vNetPlayerDir;
		//mi.nFlags	= MOVEMENTFLAG_FORWARD;

		//testContext.GetNetAgent().OnRecv( MC_ACTION_MOVE, 2,

		//	NEW_UID( uidNetPlayer ),
		//	NEW_SINGLE_BLOB(&mi, sizeof(TD_PC_MOVE))
		//	);

		//testContext.GetNetAgent().OnRecv( MC_ACTION_STOP_MOVE, 2,

		//	NEW_UID( uidNetPlayer ),
		//	NEW_VEC(vNetPlayerPos)
		//	);

		//// ������Ʈ�� ���־�� XModuleNetControl�� Histroy�� ���� Move �迭 ����� ���ŵȴ�.
		//testContext.UpdateGame(HISTORY_UPDATE_TIME);

		//XModuleMovable* pModuleMovable = testContext.GetModule<XModuleMovable>(pNetPlayer);
		//XModuleNetControl* pModuleNetControl = testContext.GetModule<XModuleNetControl>(pNetPlayer);
		//XMovableFactor MovableFactor = pModuleMovable->DumpMovableFactor();

		//float fCurrentSpeed = pNetPlayer->GetMoveSpeed(mi.nFlags) * pModuleNetControl->GetBuffMoveSpeed();
		//vec3 vDir = (vNetPlayerPos - vNetPlayerPos).Normalize();
		//vec3 vExpectVelocity = vec3(vDir.x, vDir.y, 0.0f) * fCurrentSpeed;

		////////////////////////////////////////////////////////////////////////////
		//// 2. ������ ��ǥ�� ����ġ�Ҷ�
		////
		//fCurrentSpeed = pNetPlayer->GetMoveSpeed(mi.nFlags) * pModuleNetControl->GetBuffMoveSpeed();
		//vec3 vNewNetPlayerPos = vNetPlayerPos + vec3(100.0f, 100.0f, 0.0f);

		//testContext.GetNetAgent().OnRecv( MC_ACTION_STOP_MOVE, 2,

		//	NEW_UID( uidNetPlayer ),
		//	NEW_VEC(vNewNetPlayerPos)
		//	);

		//// ������Ʈ�� ���־�� XModuleNetControl�� Histroy�� ���� Move �迭 ����� ���ŵȴ�.
		//testContext.UpdateGame(HISTORY_UPDATE_TIME);

		//MovableFactor = pModuleMovable->DumpMovableFactor();
		//float fLen = (vNewNetPlayerPos - vNetPlayerPos).Length();
		//CHECK(fLen <= 600.0f);

		//vDir = (vNewNetPlayerPos - vNetPlayerPos).Normalize();
		//vExpectVelocity = vec3(vDir.x, vDir.y, 0.0f) * fCurrentSpeed;

		////////////////////////////////////////////////////////////////////////////
		//// 3. ������ ��ǥ�� ����ġ���ϰ� �Ÿ��� 600 �̻��϶�
		////
		//fCurrentSpeed = pNetPlayer->GetMoveSpeed(mi.nFlags) * pModuleNetControl->GetBuffMoveSpeed();
		//vNewNetPlayerPos = vNetPlayerPos + vec3(800.0f, 700.0f, 0.0f);

		//testContext.GetNetAgent().OnRecv( MC_ACTION_STOP_MOVE, 2,

		//	NEW_UID( uidNetPlayer ),
		//	NEW_VEC(vNewNetPlayerPos)
		//	);

		//// ������Ʈ�� ���־�� XModuleNetControl�� Histroy�� ���� Move �迭 ����� ���ŵȴ�.
		//testContext.UpdateGame(HISTORY_UPDATE_TIME);

		//MovableFactor = pModuleMovable->DumpMovableFactor();
		//fLen = (vNewNetPlayerPos - vNetPlayerPos).Length();
		//CHECK(fLen > 600.0f);
		//fCurrentSpeed *= 3.0f;

		//vDir = (vNewNetPlayerPos - vNetPlayerPos).Normalize();
		//vExpectVelocity = vec3(vDir.x, vDir.y, 0.0f) * fCurrentSpeed;
	}

	TEST(TestNetControlOnJump)
	{
		// build context
		TestGame			testGame;
		minet::MTestNetAgent		netAgent(global.net->GetClient());
		TestModuleContext	testContext(&testGame, &netAgent);
		testContext.MakeContextFieldLoadingCompleteIgnore();

		// net player
		XNetPlayer* pNetPlayer = testContext.CreateNetPlayerAndEnterToGmae(vec3(0.f, 0.f, 0.0f), vec3(1.f, 0.f, 0.f), SEX_FEMALE);
		CHECK(pNetPlayer != NULL);

		MUID uidNetPlayer = pNetPlayer->GetUID();
		vec3 vNetPlayerDir = pNetPlayer->GetDirection();
		vec3 vNetPlayerPos = pNetPlayer->GetPosition();

		////////////////////////////////////////////////////////////////////////////
		//// 1. ������ ���� ��ġ�� ��ġ�Ҷ�
		////
		//testContext.GetNetAgent().OnRecv( MC_ACTION_JUMP, 3,

		//	NEW_UID( uidNetPlayer ),
		//	NEW_VEC(vNetPlayerPos),
		//	NEW_VEC(vNetPlayerDir)
		//	);

		//// ������Ʈ�� ���־�� XModuleNetControl�� Histroy�� ���� Move �迭 ����� ���ŵȴ�.
		//testContext.UpdateGame(HISTORY_UPDATE_TIME);

		//XModuleMovable* pModuleMovable = testContext.GetModule<XModuleMovable>(pNetPlayer);
		//XMovableFactor MovableFactor = pModuleMovable->DumpMovableFactor();

		//float fCurrentSpeed = pNetPlayer->GetMoveSpeed(MOVEMENTFLAG_JUMPING);
		//vec3 vNewJumpDir = vNetPlayerDir * fCurrentSpeed;
		//vNewJumpDir.z = 0.0f;
		//vNewJumpDir.Normalize();
		//vec3 vExpectVelocity = vec3(vNewJumpDir.x * XCONST::MOVE_SPEED, vNewJumpDir.y * XCONST::MOVE_SPEED, XCONST::JUMP_SPEED);

		////CHECK_EQUAL(MovableFactor.m_bGoing, false);
		////CHECK_EQUAL(MovableFactor.m_bMovingByInterpolation, true);
		////CHECK_CLOSE(MovableFactor.m_fMoveInterpolationTime, XMovableGoParam::TIME_JUMP_INTERPOLATION, 0.001f);		
		////CHECK_EQUAL(MovableFactor.m_bJumping, true);
		////CHECK_EQUAL(MovableFactor.m_bJumpAscending, true);
		////CHECK_EQUAL(MovableFactor.m_bLanding, false);

		////CHECK_CLOSE( MovableFactor.m_vVelocity.x, vExpectVelocity.x, 0.001f );
		////CHECK_CLOSE( MovableFactor.m_vVelocity.y, vExpectVelocity.y, 0.001f );
		////CHECK_CLOSE( MovableFactor.m_vVelocity.z, XCONST::JUMP_SPEED, 0.001f );

		////CHECK_CLOSE(MovableFactor.m_vMoveInterpolationPosStart.x, 1000.0f, 0.001f);
		////CHECK_CLOSE(MovableFactor.m_vMoveInterpolationPosStart.y, 1000.0f, 0.001f);
		////CHECK_CLOSE(MovableFactor.m_vMoveInterpolationPosTarget.x, 1000.0f, 0.001f);
		////CHECK_CLOSE(MovableFactor.m_vMoveInterpolationPosTarget.y, 1000.0f, 0.001f);

		////////////////////////////////////////////////////////////////////////////
		//// 2. ������ ���� ��ġ�� ����ġ�Ҷ�
		////
		//vec3 vNewNetPlayerPos = vNetPlayerPos + vec3(0.0f, 100.0f, 0.0f);

		//testContext.GetNetAgent().OnRecv( MC_ACTION_JUMP, 3,

		//	NEW_UID( uidNetPlayer ),
		//	NEW_VEC(vNewNetPlayerPos),
		//	NEW_VEC(vNetPlayerDir)
		//	);

		//// ������Ʈ�� ���־�� XModuleNetControl�� Histroy�� ���� Move �迭 ����� ���ŵȴ�.
		//testContext.UpdateGame(HISTORY_UPDATE_TIME);

		//MovableFactor = pModuleMovable->DumpMovableFactor();

		//fCurrentSpeed = pNetPlayer->GetMoveSpeed(MOVEMENTFLAG_JUMPING);
		//vNewJumpDir = vNetPlayerDir * fCurrentSpeed;
		//vNewJumpDir.z = 0.0f;
		//vNewJumpDir.Normalize();
		//vExpectVelocity = vec3(vNewJumpDir.x * XCONST::MOVE_SPEED, vNewJumpDir.y * XCONST::MOVE_SPEED, XCONST::JUMP_SPEED);

		////CHECK_EQUAL(MovableFactor.m_bGoing, false);
		////CHECK_EQUAL(MovableFactor.m_bMovingByInterpolation, true);
		////CHECK_CLOSE(MovableFactor.m_fMoveInterpolationTime, XMovableGoParam::TIME_JUMP_INTERPOLATION, 0.001f);		
		////CHECK_EQUAL(MovableFactor.m_bJumping, true);
		////CHECK_EQUAL(MovableFactor.m_bJumpAscending, true);
		////CHECK_EQUAL(MovableFactor.m_bLanding, false);

		////CHECK_CLOSE( MovableFactor.m_vVelocity.x, vExpectVelocity.x, 0.001f );
		////CHECK_CLOSE( MovableFactor.m_vVelocity.y, vExpectVelocity.y, 0.001f );
		////CHECK_CLOSE( MovableFactor.m_vVelocity.z, XCONST::JUMP_SPEED, 0.001f );

		////CHECK_CLOSE(MovableFactor.m_vMoveInterpolationPosStart.x, 1000.0f, 0.001f);
		////CHECK_CLOSE(MovableFactor.m_vMoveInterpolationPosStart.y, 1000.0f, 0.001f);
		////CHECK_CLOSE(MovableFactor.m_vMoveInterpolationPosTarget.x, 1000.0f, 0.001f);
		////CHECK_CLOSE(MovableFactor.m_vMoveInterpolationPosTarget.y, 1100.0f, 0.001f);

	}

	TEST(TestNetControlOnFalling)
	{
		// build context
		TestGame			testGame;
		minet::MTestNetAgent		netAgent(global.net->GetClient());
		TestModuleContext	testContext(&testGame, &netAgent);
		testContext.MakeContextFieldLoadingCompleteIgnore();

		// net player
		XNetPlayer* pNetPlayer = testContext.CreateNetPlayerAndEnterToGmae(vec3(0.f, 0.f, 0.0f), vec3(1.f, 0.f, 0.f), SEX_FEMALE);
		CHECK(pNetPlayer != NULL);

		MUID uidNetPlayer = pNetPlayer->GetUID();
		vec3 vNetPlayerDir = pNetPlayer->GetDirection();
		vec3 vNetPlayerPos = pNetPlayer->GetPosition();

		////////////////////////////////////////////////////////////////////////////
		//// 1. Falling�� ���� üũ
		////
		//testContext.GetNetAgent().OnRecv( MC_ACTION_FALLING, 3,

		//	NEW_UID( uidNetPlayer ),
		//	NEW_VEC(vNetPlayerPos),
		//	NEW_VEC(vNetPlayerDir)
		//	);

		//// ������Ʈ�� ���־�� XModuleNetControl�� Histroy�� ���� Move �迭 ����� ���ŵȴ�.
		//testContext.UpdateGame(HISTORY_UPDATE_TIME);

		//XModuleMovable* pModuleMovable = testContext.GetModule<XModuleMovable>(pNetPlayer);
		//XMovableFactor MovableFactor = pModuleMovable->DumpMovableFactor();

		////CHECK_EQUAL(MovableFactor.m_bFallingButNotJump, true);
	}

	TEST(TestNetControlGrappledStart)
	{
		// build context
		TestGame			testGame;
		minet::MTestNetAgent		netAgent(global.net->GetClient());
		TestModuleContext	testContext(&testGame, &netAgent);
		testContext.MakeContextFieldLoadingCompleteIgnore();

		// net player
		XNetPlayer* pNetPlayer = testContext.CreateNetPlayerAndEnterToGmae(vec3(0.f, 0.f, 0.0f), vec3(1.f, 0.f, 0.f), SEX_FEMALE);
		CHECK(pNetPlayer != NULL);

		MUID uidNetPlayer = pNetPlayer->GetUID();
		vec3 vNetPlayerDir = pNetPlayer->GetDirection();
		vec3 vNetPlayerPos = pNetPlayer->GetPosition();

		//////////////////////////////////////////////////////////////////////////
		// 1. GrappledStart�� ���� üũ
		//

		//TD_TALENT_HIT	td_talent_hit;
		//td_talent_hit.uidAttacker = /*������ UID*/
		//td_talent_hit.uidVictim = /*�ǰ��� UID*/;
		//td_talent_hit.nTalentID = /*�����ڰ� ����� �ŷ�Ʈ ID*/;
		//td_talent_hit.nDamage = 45;
		//td_talent_hit.nFlags = 0;
		//td_talent_hit.nMFState = MF_GRAPPLED;
		//td_talent_hit.nMFWeight = 0;

		//testContext.GetNetAgent().OnRecv( MC_ACTION_TALENT_HIT, 1,

		//	NEW_BLOB(&td_talent_hit, sizeof(TD_TALENT_HIT), 1)
		//	);
	}

	TEST(TestNetControlChangeStance)
	{
		// build context
		TestGame			testGame;
		minet::MTestNetAgent		netAgent(global.net->GetClient());
		TestModuleContext	testContext(&testGame, &netAgent);
		testContext.MakeContextFieldLoadingCompleteIgnore();

		// net player
		XNetPlayer* pNetPlayer = testContext.CreateNetPlayerAndEnterToGmae(vec3(0.f, 0.f, 0.0f), vec3(1.f, 0.f, 0.f), SEX_FEMALE);
		CHECK(pNetPlayer != NULL);

		MUID uidNetPlayer = pNetPlayer->GetUID();
		UIID nNetPlayerUIID = pNetPlayer->GetUIID();
		vec3 vNetPlayerDir = pNetPlayer->GetDirection();
		vec3 vNetPlayerPos = pNetPlayer->GetPosition();

		netAgent.OnRecv( MC_ACTION_CHANGE_STANCE, 2,

			NEW_USHORT( nNetPlayerUIID ),
			NEW_CHAR(1)					// 0-Normal, 1-Battle, 2-Fast Normal, 3-Fast Battle
			);

		XModuleMovable* pModuleMovable = testContext.GetModule<XModuleMovable>(pNetPlayer);
		XMovableFactor MovableFactor = pModuleMovable->GetMovableFactor();

		////////////////////////////////////////////////////////////////////////////
		//// 1. Normal -> Battle�� ����� XMovableFactor üũ
		////
		//CHECK_EQUAL(MovableFactor.m_bGoing, false);
		//CHECK_EQUAL(MovableFactor.m_bMovingByTarget, false);
		//CHECK_CLOSE(MovableFactor.m_vVelocity.x, 0.0f, 0.001f);		
		//CHECK_CLOSE(MovableFactor.m_vVelocity.y, 0.0f, 0.001f);		
		//CHECK_CLOSE(MovableFactor.m_vVelocity.z, 0.0f, 0.001f);		

		////////////////////////////////////////////////////////////////////////////
		//// 2. ChangeStance�� �ִϸ��̼� ���� ���� üũ
		////
		////testContext.UpdateGame(0.1f);
		////
		////XModuleMotion * pModuleMotion = testContext.GetModule<XModuleMotion>(pNetPlayer);
		////
		////CHECK_EQUAL(pModuleMotion->GetCurrMotion(), MOTION_NAME_CHANGE_STANCE_TO_BATTLE);
	}


	TEST(TestNetPlayerProcessMoveException_1_1)
	{
		// build context
		TestGame			testGame;
		minet::MTestNetAgent		netAgent(global.net->GetClient());
		TestModuleContext	testContext(&testGame, &netAgent);
		testContext.MakeContextFieldLoadingCompleteIgnore();

		// net player
		XNetPlayer* pNetPlayer = testContext.CreateNetPlayerAndEnterToGmae(vec3(0.f, 0.f, 0.0f), vec3(1.f, 0.f, 0.f), SEX_FEMALE);
		CHECK(pNetPlayer != NULL);

		vec3 pNetPlayerPos = pNetPlayer->GetPosition();
		MUID uidNetPlayer = pNetPlayer->GetUID();
		UIID nNetPlayerUIID = pNetPlayer->GetUIID();

		//XModuleTalent* pMdouleTalent = testContext.GetModule<XModuleTalent>(pNetPlayer);
		XModuleMovable* pModuleMovable = testContext.GetModule<XModuleMovable>(pNetPlayer);
		XMovableFactor MovableFactor = pModuleMovable->GetMovableFactor();

		vec3 vCurrentNetPlayerPos(0.0f, 0.0f, 0.0f);

		//////////////////////////////////////////////////////////////////////////
		// 1. �Ÿ����� ���� ����ó�� üũ (����)
		//
		TD_PC_MOVE moveData;
		moveData.nFlags		= MOVEMENTFLAG_FORWARD;
		moveData.vTarPos	= pNetPlayer->GetPosition() + pNetPlayer->GetDirection() * (PROCESS_MOVE_EXCEPTION_DISTANCE + 1.0f);
		moveData.svDir.SetX(pNetPlayer->GetDirection().x);
		moveData.svDir.SetY(pNetPlayer->GetDirection().y);
		netAgent.OnRecv( MC_ACTION_MOVE, 2,

			NEW_USHORT( nNetPlayerUIID ),
			NEW_SINGLE_BLOB(&moveData, sizeof(TD_PC_MOVE))
			);

		testContext.UpdateGame(1.0f);

		vec3 vExceptionNetPlayerPos = pNetPlayer->GetPosition();

		CHECK_CLOSE( moveData.vTarPos.x, vExceptionNetPlayerPos.x, 0.001f);		
		CHECK_CLOSE( moveData.vTarPos.y, vExceptionNetPlayerPos.y, 0.001f);		
		CHECK_CLOSE( moveData.vTarPos.z, vExceptionNetPlayerPos.z, 0.001f);	
	}

	TEST(TestNetPlayerProcessMoveException_1_2)
	{
		// build context
		TestGame			testGame;
		minet::MTestNetAgent		netAgent(global.net->GetClient());
		TestModuleContext	testContext(&testGame, &netAgent);
		testContext.MakeContextFieldLoadingCompleteIgnore();

		// net player
		XNetPlayer* pNetPlayer = testContext.CreateNetPlayerAndEnterToGmae(vec3(0.f, 0.f, 0.0f), vec3(1.f, 0.f, 0.f), SEX_FEMALE);
		CHECK(pNetPlayer != NULL);

		vec3 pNetPlayerPos = pNetPlayer->GetPosition();
		MUID uidNetPlayer = pNetPlayer->GetUID();
		UIID nNetPlayerUIID = pNetPlayer->GetUIID();
		TD_PC_MOVE moveData;

		//XModuleTalent* pMdouleTalent = testContext.GetModule<XModuleTalent>(pNetPlayer);
		XModuleMovable* pModuleMovable = testContext.GetModule<XModuleMovable>(pNetPlayer);
		XMovableFactor MovableFactor = pModuleMovable->GetMovableFactor();

		vec3 vCurrentNetPlayerPos(0.0f, 0.0f, 0.0f);

		//////////////////////////////////////////////////////////////////////////
		// 1. �Ÿ����� ���� ����ó�� üũ (�Ϲ�)
		//
		moveData.nFlags		= MOVEMENTFLAG_FORWARD;
		moveData.vTarPos	= pNetPlayer->GetPosition() + pNetPlayer->GetDirection() * (PROCESS_MOVE_EXCEPTION_DISTANCE - 1.0f);
		moveData.svDir.SetX(pNetPlayer->GetDirection().x);
		moveData.svDir.SetY(pNetPlayer->GetDirection().y);

		// �ǵ��� ���� ��ǥ
		float fNetPlayerDist = pNetPlayer->GetMoveSpeed(MOVEMENTFLAG_FORWARD) * 0.01f;
		vCurrentNetPlayerPos = pNetPlayer->GetPosition();
		vCurrentNetPlayerPos += pNetPlayer->GetDirection() * fNetPlayerDist;

		netAgent.OnRecv( MC_ACTION_MOVE, 2,

			NEW_USHORT( nNetPlayerUIID ),
			NEW_SINGLE_BLOB(&moveData, sizeof(TD_PC_MOVE))
			);

		testContext.UpdateGame(0.01f);

		// �Ÿ��� 30m�� �ȳѾ �׳� �̵� ó���� �Ǿ��°�? (������ǥ�� ��ȭ�� ����� �Ѵ�.)
		vec3 vNotExceptionNetPlayerPos = vCurrentNetPlayerPos;

		CHECK_CLOSE(vNotExceptionNetPlayerPos.x, vCurrentNetPlayerPos.x, 0.001f);		
		CHECK_CLOSE(vNotExceptionNetPlayerPos.y, vCurrentNetPlayerPos.y, 0.001f);		
		CHECK_CLOSE(vNotExceptionNetPlayerPos.z, vCurrentNetPlayerPos.z, 0.001f);	
	}

	TEST(TestNetPlayerProcessMoveException_2_1)
	{
		// build context
		TestGame			testGame;
		minet::MTestNetAgent		netAgent(global.net->GetClient());
		TestModuleContext	testContext(&testGame, &netAgent);
		testContext.MakeContextFieldLoadingCompleteIgnore();

		// net player
		XNetPlayer* pNetPlayer = testContext.CreateNetPlayerAndEnterToGmae(vec3(0.f, 0.f, -5.0f), vec3(1.f, 0.f, 0.f), SEX_FEMALE);
		CHECK(pNetPlayer != NULL);

		vec3 pNetPlayerPos = pNetPlayer->GetPosition();
		MUID uidNetPlayer = pNetPlayer->GetUID();
		UIID nNetPlayerUIID = pNetPlayer->GetUIID();
		TD_PC_MOVE moveData;

		//XModuleTalent* pMdouleTalent = testContext.GetModule<XModuleTalent>(pNetPlayer);
		XModuleMovable* pModuleMovable = testContext.GetModule<XModuleMovable>(pNetPlayer);
		XMovableFactor MovableFactor = pModuleMovable->GetMovableFactor();

		vec3 vCurrentNetPlayerPos(0.0f, 0.0f, 0.0f);

		//////////////////////////////////////////////////////////////////////////
		// 1. �������� ���� ����ó�� üũ (����)
		//
		moveData.nFlags		= MOVEMENTFLAG_FORWARD;
		moveData.vTarPos	= pNetPlayer->GetPosition() + vec3(0.0f, 0.0f, 10.0f);
		moveData.svDir.SetX(pNetPlayer->GetDirection().x);
		moveData.svDir.SetY(pNetPlayer->GetDirection().y);

		// �ǵ��� ���� ��ǥ
		vCurrentNetPlayerPos = moveData.vTarPos;

		netAgent.OnRecv( MC_ACTION_MOVE, 2,

			NEW_USHORT( nNetPlayerUIID ),
			NEW_SINGLE_BLOB(&moveData, sizeof(TD_PC_MOVE))
			);

		testContext.UpdateGame(0.01f);

		// ������ ���� ���ܼ� �����Ѵ�. (���ο� ��ǥ�� ������ �Ǿ� �־�� �Ѵ�.)
		vec3 vExceptionNetPlayerPos = pNetPlayer->GetPosition();

		//CHECK_CLOSE(vExceptionNetPlayerPos.x, vCurrentNetPlayerPos.x, 0.001f);		
		//CHECK_CLOSE(vExceptionNetPlayerPos.y, vCurrentNetPlayerPos.y, 0.001f);		
		//CHECK_CLOSE(vExceptionNetPlayerPos.z, vCurrentNetPlayerPos.z, 0.001f);	
	}
	TEST(TestNetPlayerDuelWinner)
	{
		// build context
		TestGame			testGame;
		minet::MTestNetAgent		netAgent(global.net->GetClient());
		TestModuleContext	testContext(&testGame, &netAgent);
		testContext.MakeContextFieldLoadingCompleteIgnore();

		// net player
		XNetPlayer* pNetPlayer = testContext.CreateNetPlayerAndEnterToGmae(vec3(0.f, 0.f, -5.0f), vec3(1.f, 0.f, 0.f), SEX_FEMALE);
		CHECK(pNetPlayer != NULL);

		vec3 pNetPlayerPos = pNetPlayer->GetPosition();
		MUID uidNetPlayer = pNetPlayer->GetUID();

		netAgent.OnRecv( MC_DUEL_FINISHED, 2,

			NEW_UID( uidNetPlayer ),
			NEW_UID( 0 )
			);

		//////////////////////////////////////////////////////////////////////////
		// 1. �̰����� ����
		//

	}

	TEST(TestNetOnUseTalentOnMove)
	{
		// build context
		TestGame			testGame;
		minet::MTestNetAgent		netAgent(global.net->GetClient());
		TestModuleContext	testContext( &testGame, &netAgent);
		testContext.MakeContextFieldLoadingCompleteIgnore();


		vec3 vFirstPos = vec3(  0.0f, 0.0f, 0.0f);
		vec3 vFirstDir = vec3( -1.0f, 0.0f, 0.0f);
		vec3 vLastDir  = vec3(  1.0f, 0.0f, 0.0f);


		// Create net player
		XNetPlayer* pNetPlayer = testContext.CreateNetPlayerAndEnterToGmae( vFirstPos, vFirstDir, SEX_FEMALE);
		CHECK( pNetPlayer != NULL);

		// Move
		TD_PC_MOVE mi;
		mi.vTarPos = vFirstPos + vLastDir * (PROCESS_MOVE_EXCEPTION_DISTANCE - 1.0f);
		mi.svDir.SetX(vLastDir.x);
		mi.svDir.SetY(vLastDir.y);
		mi.nFlags  = MOVEMENTFLAG_FORWARD;

		netAgent.OnRecv( MC_ACTION_MOVE, 2, NEW_USHORT( pNetPlayer->GetUIID()), NEW_SINGLE_BLOB( &mi, sizeof( TD_PC_MOVE)) );


		// update
		testContext.UpdateGame( 1.0f);
		testContext.UpdateGame( 1.0f);


		//////////////////////////////////////////////////////////////////////////
		// 1. �̵��� ȸ���� ���� üũ
		//
		XModuleMovable* pModuleMovable = testContext.GetModule<XModuleMovable>(pNetPlayer);
		XModuleNetControl* pModuleNetControl = testContext.GetModule<XModuleNetControl>(pNetPlayer);

		// ��ġ
		vec3 vExpectPos = vFirstPos + vLastDir * (PROCESS_MOVE_EXCEPTION_DISTANCE - 1.0f) + vLastDir * VIRTUAL_POSITION_VELOCITY;

		CHECK_CLOSE( vExpectPos.x, pNetPlayer->GetPosition().x, 0.001f );
		CHECK_CLOSE( vExpectPos.y, pNetPlayer->GetPosition().y, 0.001f );
		CHECK_CLOSE( vExpectPos.z, pNetPlayer->GetPosition().z, 0.001f );


		// ����
		CHECK_CLOSE( vLastDir.x, pNetPlayer->GetDirection().x, 0.001f );
		CHECK_CLOSE( vLastDir.y, pNetPlayer->GetDirection().y, 0.001f );
		CHECK_CLOSE( vLastDir.z, pNetPlayer->GetDirection().z, 0.001f );

		//////////////////////////////////////////////////////////////////////////


		vFirstPos = pNetPlayer->GetPosition();
		vFirstDir = pNetPlayer->GetDirection();
		vLastDir = vec3( 0.0f, 1.0f, 0.0f);


		// Make talent
		testContext.CreateTalent(L"<TALENT id=\"101001\" name=\"1hs �Ϲݰ��� 1Ÿ\" TalentType=\"skill\" Category=\"melee\" SkillType=\"melee\" EffectTiming=\"hit_this_talent\" EffectRadius=\"3\" HitTestType=\"area\" HitMotionDelay=\"0.4\" HPCost=\"0\" APCost=\"0\" ENCost=\"0\" STACost=\"0\" HPTickCost=\"0\" ENTickCost=\"0\" STATickCost=\"0\" CoolTime=\"0\" Stance=\"combat\" UseAni=\"PS_atk1;PS_atk1_r\" AttackDirection=\"left\" MF_BEATEN=\"100\" MF_WEIGHT_BEATEN=\"500\" MF_KNOCKBACK=\"10\" MF_WEIGHT_KNOCKBACK=\"100\" MF_STUN=\"0\" MF_WEIGHT_STUN=\"0\" MF_DEFLECT=\"0\" MF_WEIGHT_DEFLECT=\"0\" MF_THROWUP=\"0\" MF_WEIGHT_THROWUP=\"0\" MF_KNOCKDOWN=\"0\" MF_WEIGHT_KNOCKDOWN=\"0\" MF_UPPERED=\"0\" MF_WEIGHT_UPPERED=\"0\" MinDamage=\"0\" MaxDamage=\"0\" WeaponApplyRate=\"1\" DodgeDifficulty=\"230\" CriticalEnable=\"True\" DamageAttrib=\"slash\" Guardable=\"True\" Cancelable=\"True\" UseBloodEffect=\"True\" />");


		// Use talent
		pNetPlayer->SetStance(CS_BATTLE);
		vec3 vRecvPos = vFirstPos + vLastDir * (PROCESS_MOVE_EXCEPTION_DISTANCE - 1.0f);

		netAgent.OnRecv( MC_ACTION_USE_TALENT, 4, NEW_USHORT( pNetPlayer->GetUIID()), NEW_INT( 101001), NEW_SVEC( vLastDir), NEW_VEC( vRecvPos) );


		// Update
		float fUpdateTime = 1.0f;
		testContext.UpdateGame( fUpdateTime);


		//////////////////////////////////////////////////////////////////////////
		// 2. �̵��� ȸ���� ���� üũ
		//

		// ��ġ
		float speed = pNetPlayer->GetMoveSpeed( mi.nFlags);
		vExpectPos = vFirstPos + vLastDir * speed * fUpdateTime;

		CHECK_CLOSE( vExpectPos.x, pNetPlayer->GetPosition().x, 0.001f );
		CHECK_CLOSE( vExpectPos.y, pNetPlayer->GetPosition().y, 0.001f );
		CHECK_CLOSE( vExpectPos.z, pNetPlayer->GetPosition().z, 0.001f );


		// ����
		CHECK_CLOSE( vLastDir.x, pNetPlayer->GetDirection().x, 0.001f );
		CHECK_CLOSE( vLastDir.y, pNetPlayer->GetDirection().y, 0.001f );
		CHECK_CLOSE( vLastDir.z, pNetPlayer->GetDirection().z, 0.001f );

		//////////////////////////////////////////////////////////////////////////


		testContext.TalentClear();
	}
}
