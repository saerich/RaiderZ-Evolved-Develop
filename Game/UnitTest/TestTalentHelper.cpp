#include "stdafx.h"
#include "XTalentHelper.h"

SUITE(TalentHelper)
{
	TEST(TestTalentHelper_MakeFullPlayerTalentAniName)
	{
		// battle ���Ľ��� ���
		CHECK(wstring(L"1hs_active8") == XTalentHelper::MakeFullPlayerTalentAniName(L"active8", CS_BATTLE, WEAPON_1H_SLASH));
		CHECK(wstring(L"sta_active5") == XTalentHelper::MakeFullPlayerTalentAniName(L"active5", CS_BATTLE, WEAPON_STAFF));

		// normal ���Ľ��̸� none_
		CHECK(wstring(L"none_active8") == XTalentHelper::MakeFullPlayerTalentAniName(L"active8", CS_NORMAL, WEAPON_1H_SLASH));
		CHECK(wstring(L"none_active8") == XTalentHelper::MakeFullPlayerTalentAniName(L"active8", CS_NORMAL, WEAPON_STAFF));
		CHECK(wstring(L"none_active1") == XTalentHelper::MakeFullPlayerTalentAniName(L"active1", CS_NORMAL, WEAPON_NONE));

		// ����ó��
		CHECK(wstring(L"active1") == XTalentHelper::MakeFullPlayerTalentAniName(L"active1", CS_BATTLE, WEAPON_NONE));
	}

	TEST(TestTalentHelper_IsTalentCancelableMotionFactor)
	{
		CHECK_EQUAL(false, XTalentHelper::IsTalentCancelableMotionFactor(MF_NONE));
		CHECK_EQUAL(false, XTalentHelper::IsTalentCancelableMotionFactor(MF_FAKE_BEATEN));
		CHECK_EQUAL(false, XTalentHelper::IsTalentCancelableMotionFactor(MF_FAKE_KNOCKBACK));

		CHECK_EQUAL(true, XTalentHelper::IsTalentCancelableMotionFactor(MF_BEATEN));
		CHECK_EQUAL(true, XTalentHelper::IsTalentCancelableMotionFactor(MF_KNOCKBACK));
		CHECK_EQUAL(true, XTalentHelper::IsTalentCancelableMotionFactor(MF_STUN));
		CHECK_EQUAL(true, XTalentHelper::IsTalentCancelableMotionFactor(MF_DEFLECT));
		CHECK_EQUAL(true, XTalentHelper::IsTalentCancelableMotionFactor(MF_THROWUP));
		CHECK_EQUAL(true, XTalentHelper::IsTalentCancelableMotionFactor(MF_KNOCKDOWN));
		CHECK_EQUAL(true, XTalentHelper::IsTalentCancelableMotionFactor(MF_UPPERED));
		CHECK_EQUAL(true, XTalentHelper::IsTalentCancelableMotionFactor(MF_GRAPPLED));
		CHECK_EQUAL(true, XTalentHelper::IsTalentCancelableMotionFactor(MF_SWALLOWED));
		CHECK_EQUAL(true, XTalentHelper::IsTalentCancelableMotionFactor(MF_DRAG));
	}
}