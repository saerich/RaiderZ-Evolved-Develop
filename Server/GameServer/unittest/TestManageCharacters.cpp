#include "stdafx.h"
#include "SUnitTest.h"
#include "GItemManager.h"
#include "GGlobal.h"
#include "GDef.h"

SUITE(ManageCharacters)
{
	TEST3(ExistInitialItems)
	{
		// ����� ���� ������ id�� �ٲٸ� DB Proc 'spCharInsert' ������ ��������� �ϱ� ����. - birdkr
		CHECK_EQUAL(INITIAL_ITEM_SET::WEAPON_ID,	1);
		CHECK_EQUAL(INITIAL_ITEM_SET::BODY_ID,		1200);
		CHECK_EQUAL(INITIAL_ITEM_SET::LEG_ID,		1201);
		CHECK_EQUAL(INITIAL_ITEM_SET::HANDS_ID,		1202);
		CHECK_EQUAL(INITIAL_ITEM_SET::FEET_ID,		1203);

		CHECK(gmgr.pItemManager->IsExist(INITIAL_ITEM_SET::WEAPON_ID) == true);
		CHECK(gmgr.pItemManager->IsExist(INITIAL_ITEM_SET::BODY_ID) == true);
		CHECK(gmgr.pItemManager->IsExist(INITIAL_ITEM_SET::LEG_ID) == true);
		CHECK(gmgr.pItemManager->IsExist(INITIAL_ITEM_SET::HANDS_ID) == true);
		CHECK(gmgr.pItemManager->IsExist(INITIAL_ITEM_SET::FEET_ID) == true);
	}
}