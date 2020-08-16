#include "stdafx.h"
#include "XMotionLogic.h"
#include "XMotionTimeModifier.h"
#include "XMotion.h"
#include "XMotionEventListener.h"
#include "MockAnimationController.h"
#include "XTestWrapper.h"
#include "MockPlayer.h"
#include "CSChrInfo.h"

SUITE(MotionLogic)
{
	struct FMotionLogicFixture
	{
		FMotionLogicFixture()
		{
			AniDataSet.AddAni(L"none_idle", 1.0, true);
			AniDataSet.AddAni(L"none_idle1", 1.0, true);
			AniDataSet.AddAni(L"none_idle2", 1.0, true);
			AniDataSet.AddAni(L"none_jump1", 1.0, false);
			AniDataSet.AddAni(L"none_jump2", 1.0, false);

			AniDataSet.AddAni(L"1hs_idle", 1.0, true);
			AniDataSet.AddAni(L"2hd_idle", 1.0, true);

			AniDataSet.AddAni(L"none_jump_up", 1.0, false);
			AniDataSet.AddAni(L"none_jump_down", 1.0, false);
			AniDataSet.AddAni(L"none_jump_land", 1.0, false);

			AniDataSet.AddAni(L"none_idle_sneak", 1.0, true);

			m_pAnimationController = new MockAnimationController(NULL, NULL);
			m_pAnimationController->SetAniDataSet(&AniDataSet);

			m_pMockPlayer = new MockPlayer(MUID::ZERO, SEX_FEMALE);

			m_pMotionLogic = new XMotionLogic(m_pMockPlayer, m_pAnimationController, new XMotionTimeModifier(NULL, NULL), NULL);

			InitMotionMgr();
		}
		virtual ~FMotionLogicFixture()
		{
			SAFE_DELETE(m_pMockPlayer);
			SAFE_DELETE(m_pMotionLogic);
			SAFE_DELETE(m_pAnimationController);
		}
		void InitMotionMgr()
		{
			info.motion->AddMotionForTest(L"	<MOTION name='idle' stance='all' loop='true'>		\
										  <Default>									\
										  <Sequence ani='idle'/>					\
										  </Default>								\
										  </MOTION>");

			info.motion->AddMotionForTest(L"	<MOTION name='idle_postfix' stance='all' loop='true' postfix='true'>		\
										  <Default>									\
										  <Sequence ani='idle'/>					\
										  </Default>								\
										  </MOTION>");

			info.motion->AddMotionForTest(L"	<MOTION name='jump' stance='all'>		\
										  <Default>									\
										  <Sequence ani='jump1'/>					\
										  <Sequence ani='jump2'/>					\
										  </Default>								\
										  </MOTION>");

			info.motion->AddMotionForTest(L"	<MOTION name='idle_trigger' stance='all'>		\
										  <Default>									\
										  <Sequence ani='idle1'>					\
										  <PostTrigger if='Dead' then='Hold' param1='1'/>		\
										  </Sequence>								\
										  <Sequence ani='idle2'/>					\
										  </Default>								\
										  </MOTION>");

			info.motion->AddMotionForTest(L"	<MOTION name='idle_trigger2' stance='all'>		\
										  <Default>									\
										  <Sequence ani='idle1'>					\
										  <PostTrigger if='Dead' then='ChangeMotion' param1='jump'/>		\
										  </Sequence>								\
										  <Sequence ani='idle2'/>					\
										  </Default>								\
										  </MOTION>");

			info.motion->AddMotionForTest(L"	<MOTION name='jump_event' loop='false' stance='all'>		\
										  <Default>									\
										  <Sequence ani='jump_up' event='gravity_down'/>			\
										  <Sequence ani='jump_down' event='event_landed'/>			\
										  <Sequence ani='jump_land'/>					\
										  </Default>								\
										  </MOTION>");

		}
		void Update(float fDelta)
		{
			m_pAnimationController->Update(fDelta);
			m_pMotionLogic->Update(fDelta);
		}

		MockPlayer* m_pMockPlayer;
		XMotionLogic* m_pMotionLogic;
		MockAnimationController* m_pAnimationController;
		TestAniDataSet		AniDataSet;

		XTestInfoWrapper<XMotionMgr>	m_MotionInfoWrapper;
	};

	TEST_FIXTURE(FMotionLogicFixture, TestMotionLogic_ChangeMotion)
	{
		m_pMotionLogic->ChangeMotion(L"idle");
		CHECK(wstring(L"idle") == m_pMotionLogic->GetCurrMotion());

		m_pMotionLogic->ChangeMotion(L"jump", L"idle");
		CHECK(wstring(L"jump") == m_pMotionLogic->GetCurrMotion());

		CHECK(wstring(L"none_jump1") == m_pAnimationController->GetAnimationName());

		Update(1.1f);

		CHECK(wstring(L"none_jump2") == m_pAnimationController->GetAnimationName());

		Update(1.1f);

		m_pMotionLogic->ChangeMotion(L"idle");
		CHECK(wstring(L"none_idle") == m_pAnimationController->GetAnimationName());
	}

	TEST_FIXTURE(FMotionLogicFixture, TestMotionLogic_Event)
	{
		m_pMotionLogic->ChangeMotion(L"jump_event", L"idle");
		CHECK(wstring(L"jump_event") == m_pMotionLogic->GetCurrMotion());

		CHECK(wstring(L"none_jump_up") == m_pAnimationController->GetAnimationName());

		Update(1.1f);

		CHECK(wstring(L"none_jump_up") == m_pAnimationController->GetAnimationName());

		m_pMotionLogic->OnEvent(L"gravity_down");
		CHECK(wstring(L"none_jump_down") == m_pAnimationController->GetAnimationName());

		Update(0.5f);
		CHECK(wstring(L"none_jump_down") == m_pAnimationController->GetAnimationName());

		// event_landed ��� �ٸ� �̺�Ʈ�� �Դ�.
		m_pMotionLogic->OnEvent(L"gravity_down");
		CHECK(wstring(L"none_jump_down") == m_pAnimationController->GetAnimationName());

		m_pMotionLogic->OnEvent(L"event_landed");
		CHECK(wstring(L"none_jump_land") == m_pAnimationController->GetAnimationName());

		Update(1.5f);
		CHECK(wstring(L"none_idle") == m_pAnimationController->GetAnimationName());
	}

	TEST_FIXTURE(FMotionLogicFixture, TestMotionLogic_HoldSequence)
	{
		m_pMotionLogic->ChangeMotion(L"jump", L"idle");

		CHECK(wstring(L"none_jump1") == m_pAnimationController->GetAnimationName());

		// Hold�����Ƿ� ���� Sequence�� �Ѿ�� �ʴ´�.
		m_pMotionLogic->HoldSequence();
		

		for (int i = 0; i < 2; i++)
		{
			Update(1.1f);
			CHECK(wstring(L"none_jump1") == m_pAnimationController->GetAnimationName());
		}

		// Release�����Ƿ� ���� Sequence���� �Ѿ��.
		m_pMotionLogic->HoldRelease();

		Update(0.5f);
		CHECK(wstring(L"none_jump2") == m_pAnimationController->GetAnimationName());

		// �ٽ� Hold�����Ƿ� �ȳѾ.
		m_pMotionLogic->HoldSequence();

		for (int i = 0; i < 5; i++)
		{
			Update(0.5f);
			CHECK(wstring(L"none_jump2") == m_pAnimationController->GetAnimationName());
		}

		// �ٽ� Release�����Ƿ� �Ѿ.
		m_pMotionLogic->HoldRelease();
		Update(0.5f);

		CHECK(wstring(L"idle") == m_pMotionLogic->GetCurrMotion());

		// Hold�߿� ChangeMotion�ϸ� Hold�� Ǯ����.
		m_pMotionLogic->ChangeMotion(L"jump");
		m_pMotionLogic->HoldSequence();
		CHECK_EQUAL(true, m_pMotionLogic->IsHoldingSequence());
		Update(0.5f);
		m_pMotionLogic->ChangeMotion(L"idle");
		CHECK_EQUAL(false, m_pMotionLogic->IsHoldingSequence());

	}

	TEST_FIXTURE(FMotionLogicFixture, TestMotionLogic_Trigger_Hold)
	{
		m_pMockPlayer->SetDead(true);
		m_pMotionLogic->ChangeMotion(L"idle_trigger");

		Update(0.5f);
		CHECK(wstring(L"none_idle1") == m_pAnimationController->GetAnimationName());

		Update(0.7f);

		// Hold�Ǿ����Ƿ� ��� none_idle1 �̾�� �Ѵ�.
		CHECK(wstring(L"none_idle1") == m_pAnimationController->GetAnimationName());

	}

	TEST_FIXTURE(FMotionLogicFixture, TestMotionLogic_Trigger_ChangeMotion)
	{
		m_pMockPlayer->SetDead(true);
		m_pMotionLogic->ChangeMotion(L"idle_trigger2");

		Update(0.5f);
		CHECK(wstring(L"none_idle1") == m_pAnimationController->GetAnimationName());

		Update(0.7f);

		// jump ������� ����Ǿ� �־�� �Ѵ�.
		CHECK(wstring(L"jump") == m_pMotionLogic->GetCurrMotion());		
	}

	TEST_FIXTURE(FMotionLogicFixture, TestMotionLogic_ChangeWeaponAniType)
	{
		m_pMotionLogic->ChangeMotion(L"idle");
		CHECK(wstring(L"idle") == m_pMotionLogic->GetCurrMotion());

		CHECK(wstring(L"none_idle") == m_pAnimationController->GetAnimationName());

		// ��Ʋ ���Ľ�
		m_pMockPlayer->SetStance(CS_BATTLE);

		m_pMotionLogic->ChangeWeaponAniType(WEAPON_1H_SLASH);
		CHECK(wstring(L"1hs_idle") == m_pAnimationController->GetAnimationName());

		m_pMotionLogic->ChangeWeaponAniType(WEAPON_TWO_HANDED);
		CHECK(wstring(L"2hd_idle") == m_pAnimationController->GetAnimationName());
	}

	TEST_FIXTURE(FMotionLogicFixture, TestMotionLogic_TimeOut)
	{
		info.motion->AddMotionForTest(L"	<MOTION name='test_timeout' loop='false' stance='all'>		\
									  <Default>									\
									  <Sequence ani='jump_up' />			\
									  <Sequence ani='jump_down' timeout='0.5'/>			\
									  <Sequence ani='jump_land'/>					\
									  </Default>								\
									  </MOTION>");

		info.motion->AddMotionForTest(L"	<MOTION name='test_timeout2' loop='false' stance='all'>		\
									  <Default>									\
									  <Sequence ani='jump_up' />			\
									  <Sequence ani='idle' timeout='2'/>			\
									  <Sequence ani='jump_land'/>					\
									  </Default>								\
									  </MOTION>");

		m_pMotionLogic->ChangeMotion(L"test_timeout", L"idle");

		CHECK(wstring(L"test_timeout") == m_pMotionLogic->GetCurrMotion());

		CHECK(wstring(L"none_jump_up") == m_pAnimationController->GetAnimationName());

		Update(1.01f);
		CHECK(wstring(L"none_jump_down") == m_pAnimationController->GetAnimationName());

		// jump_down�� timeout�� 0.5�̹Ƿ� ���� �������� �Ѿ
		Update(0.5f);
		CHECK(wstring(L"none_jump_land") == m_pAnimationController->GetAnimationName());	

		// test2

		m_pMotionLogic->ChangeMotion(L"test_timeout2", L"idle");

		CHECK(wstring(L"test_timeout2") == m_pMotionLogic->GetCurrMotion());

		CHECK(wstring(L"none_jump_up") == m_pAnimationController->GetAnimationName());

		Update(1.01f);
		CHECK(wstring(L"none_idle") == m_pAnimationController->GetAnimationName());

		// idle�� timeout�� 2�̹Ƿ� ���� �������� �Ѿ
		Update(1.1f);
		CHECK(wstring(L"none_idle") == m_pAnimationController->GetAnimationName());	

		Update(0.5f);
		CHECK(wstring(L"none_idle") == m_pAnimationController->GetAnimationName());	

		Update(0.5f);
		CHECK(wstring(L"none_jump_land") == m_pAnimationController->GetAnimationName());	

	}

	TEST_FIXTURE(FMotionLogicFixture, TestMotionLogic_AnimationNamePostfix)
	{
		m_pMotionLogic->ChangeMotion(L"idle");
		CHECK(wstring(L"none_idle") == m_pAnimationController->GetAnimationName());

		m_pMotionLogic->ChangeMotion(L"idle_postfix");
		CHECK(wstring(L"none_idle") == m_pAnimationController->GetAnimationName());


		// postfix ����
		m_pMotionLogic->SetAnimationNamePostfix(L"_sneak");

		m_pMotionLogic->ChangeMotion(L"idle");
		CHECK(wstring(L"none_idle") == m_pAnimationController->GetAnimationName());

		m_pMotionLogic->ChangeMotion(L"idle_postfix");
		CHECK(wstring(L"none_idle_sneak") == m_pAnimationController->GetAnimationName());
	}

	TEST_FIXTURE(FMotionLogicFixture, TestMotionLogic_RefreshThisMotion)
	{
		m_pMotionLogic->ChangeMotion(L"idle_postfix");
		CHECK(wstring(L"none_idle") == m_pAnimationController->GetAnimationName());


		// postfix ����
		m_pMotionLogic->SetAnimationNamePostfix(L"_sneak");

		m_pMotionLogic->RefreshThisMotion();
		CHECK(wstring(L"none_idle_sneak") == m_pAnimationController->GetAnimationName());
	}

	TEST_FIXTURE(FMotionLogicFixture, TestMotionLogic_RefreshThisMotion2)
	{
		// �������� �ִ� ���
		m_pMotionLogic->ChangeMotion(L"jump");
		m_pMotionLogic->PushMotion(L"idle");

		CHECK(wstring(L"none_jump1") == m_pAnimationController->GetAnimationName());


		Update(0.5f);

		// RefreshThisMotion�ص� ������ ���� �ִϸ��̼�
		m_pMotionLogic->RefreshThisMotion();
		CHECK(wstring(L"none_jump1") == m_pAnimationController->GetAnimationName());

		Update(1.1f);
		CHECK(wstring(L"none_jump2") == m_pAnimationController->GetAnimationName());

		// PushMotion���� �о���� idle�� ���ߴ�.
		Update(1.1f);
		CHECK(wstring(L"none_idle") == m_pAnimationController->GetAnimationName());
	}

	class TestMotionEventListner : public XMotionEventListener
	{
	protected:
		bool m_bDonePreEnter;
		bool m_bDoneEnter;
		bool m_bDoneLeave;
		bool m_bDoneUpdate;
	public:
		TestMotionEventListner() : XMotionEventListener(), m_bDonePreEnter(false), 
			m_bDoneEnter(false), m_bDoneLeave(false), m_bDoneUpdate(false)
		{

		}
		virtual ~TestMotionEventListner()
		{

		}
		virtual XMotion* OnPreEnter(XActor* pOwner)	
		{ 
			m_bDonePreEnter = true;
			return NULL; 
		}
		virtual void OnEnter(XActor* pOwner)		
		{
			m_bDoneEnter = true;
		}
		virtual void OnLeave(XActor* pOwner)	
		{
			m_bDoneLeave = true;
		}
		virtual void OnUpdate(XActor* pOwner)		
		{
			m_bDoneUpdate = true;
		}

		bool IsDonePreEnter() { return m_bDonePreEnter; }
		bool IsDoneEnter() { return m_bDoneEnter; }
		bool IsDoneLeave() { return m_bDoneLeave; }
		bool IsDoneUpdate() { return m_bDoneUpdate; }
	};

	class FMotionLogic_EventListener : public FMotionLogicFixture
	{
	public:
		FMotionLogic_EventListener() : FMotionLogicFixture()
		{
			m_pIdleMotion = info.motion->GetMotion(L"idle");
			CHECK(m_pIdleMotion);
			m_pJumpMotion = info.motion->GetMotion(L"jump");
			CHECK(m_pJumpMotion);

			m_pIdleEvent = new TestMotionEventListner();
			m_pJumpEvent = new TestMotionEventListner();

			m_pIdleMotion->SetEventListener(m_pIdleEvent);
			m_pJumpMotion->SetEventListener(m_pJumpEvent);
		}
		virtual ~FMotionLogic_EventListener()
		{

		}
		XMotion* m_pIdleMotion;
		XMotion* m_pJumpMotion;
		TestMotionEventListner* m_pIdleEvent;
		TestMotionEventListner* m_pJumpEvent;
	};

	TEST_FIXTURE(FMotionLogic_EventListener, TestHasListner)
	{
		CHECK(m_pIdleMotion->GetEventListener());
		CHECK(m_pJumpMotion->GetEventListener());
	}

	TEST_FIXTURE(FMotionLogic_EventListener, TestMotionEventListener_ChangeMotion)
	{
		m_pMotionLogic->ChangeMotion(L"idle");

		CHECK_EQUAL(true, m_pIdleEvent->IsDonePreEnter());
		CHECK_EQUAL(true, m_pIdleEvent->IsDoneEnter());
		CHECK_EQUAL(false, m_pIdleEvent->IsDoneLeave());
	}

	TEST_FIXTURE(FMotionLogic_EventListener, TestMotionEventListener_ChangeMotion2)
	{
		m_pMotionLogic->ChangeMotion(L"idle");
		m_pMotionLogic->ChangeMotion(L"jump");

		CHECK_EQUAL(true, m_pIdleEvent->IsDonePreEnter());
		CHECK_EQUAL(true, m_pIdleEvent->IsDoneEnter());
		CHECK_EQUAL(true, m_pIdleEvent->IsDoneLeave());

		CHECK_EQUAL(true, m_pJumpEvent->IsDonePreEnter());
		CHECK_EQUAL(true, m_pJumpEvent->IsDoneEnter());
		CHECK_EQUAL(false, m_pJumpEvent->IsDoneLeave());
	}

	TEST_FIXTURE(FMotionLogic_EventListener, TestMotionEventListener_OnUpdate)
	{
		m_pMotionLogic->ChangeMotion(L"idle");

		CHECK_EQUAL(false, m_pIdleEvent->IsDoneUpdate());

		m_pMotionLogic->Update(0.1f);

		CHECK_EQUAL(true, m_pIdleEvent->IsDoneUpdate());
	}

}