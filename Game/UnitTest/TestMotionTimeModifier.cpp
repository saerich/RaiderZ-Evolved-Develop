#include "stdafx.h"
#include "XMotionTimeModifier.h"
#include "MockCharacter.h"
#include "MockAnimationController.h"
#include "XTestWrapper.h"

SUITE(MotionTimeModifier)
{
	struct FMotionTimeModifier
	{
		FMotionTimeModifier()
		{
			AniDataSet.AddAni(L"none_idle", 1.0, true);
			AniDataSet.AddAni(L"beaten", 1.0, false);

			m_pCharacter = new MockCharacter();

			m_pAnimationController = new MockAnimationController(m_pCharacter, NULL);
			m_pAnimationController->SetAniDataSet(&AniDataSet);
		}
		~FMotionTimeModifier()
		{
			SAFE_DELETE(m_pAnimationController);
			SAFE_DELETE(m_pCharacter);
		}

		MockAnimationController*	m_pAnimationController;
		MockCharacter*				m_pCharacter;
		TestAniDataSet				AniDataSet;

		XTestInfoWrapper<XMotionMgr>	m_MotionInfoWrapper;
	};

	class TestMotionTimeModifier : public XMotionTimeModifier
	{
	public:
		TestMotionTimeModifier(RActor* pActor, XGameTransformControllerManager* pAnimationController)
			: XMotionTimeModifier(pActor, pAnimationController) {}
		void InitMotionLength(float fTime)
		{
			m_nCurrentMotionLength = (uint32)(fTime * 4800);
		}
	};
	TEST_FIXTURE(FMotionTimeModifier, TestMotionTimeModifier_SetMotionTime)
	{
		TestMotionTimeModifier modifier(m_pCharacter, m_pAnimationController);

		m_pAnimationController->SetAnimation(wstring(L"beaten"));
		m_pAnimationController->Update(0.1f);

		CHECK(wstring(L"beaten") == m_pAnimationController->GetAnimationName());
		CHECK_EQUAL(false, m_pAnimationController->IsOncePlayDone());

		// 1�ʰ� �������Ƿ� �ִϸ��̼� ������
		m_pAnimationController->Update(1.0f);
		CHECK(wstring(L"beaten") == m_pAnimationController->GetAnimationName());
		CHECK_EQUAL(true, m_pAnimationController->IsOncePlayDone());

		m_pAnimationController->SetAnimation(wstring(L"none_idle"), true);
		m_pAnimationController->SetAnimation(wstring(L"beaten"), true);
		CHECK_EQUAL(false, m_pAnimationController->IsOncePlayDone());

		modifier.InitMotionLength(1.0f);
		modifier.SetMotionTime(2000, 0);

		// �ִϸ��̼� �ð��� 2�ʷ� ���������Ƿ� 1.5�ʰ� ������ �ִϸ��̼��� ������ �ʾƾ� �Ѵ�.
		m_pAnimationController->Update(1.5f);
		CHECK(wstring(L"beaten") == m_pAnimationController->GetAnimationName());
		CHECK_EQUAL(false, m_pAnimationController->IsOncePlayDone());

		m_pAnimationController->Update(0.51f);
		CHECK_EQUAL(true, m_pAnimationController->IsOncePlayDone());
	}
}