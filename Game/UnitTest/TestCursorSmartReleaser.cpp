#include "stdafx.h"
#include "TestGame.h"
#include "MockController.h"
#include "XCursorSmartReleaser.h"
#include "XUISystem.h"

SUITE(CursorSmartReleaser)
{
	TEST(TestCursorSmartReleaser)
	{
		TestGame tGame;
		MockController* pController = tGame.GetController();

		XCursor thisCursor;
		XCursorSmartReleaser csr;

		thisCursor.Show(false);
		CHECK_EQUAL(false, thisCursor.IsVisible());

		thisCursor.Show(true);
		CHECK_EQUAL(true, thisCursor.IsVisible());

		// ����Ű�� ������ ���� ������ �ƹ��� ��ȭ�� ����.
		csr.Update(CURSOR_SMART_RELEASE_TIME+0.5f, &thisCursor, global.ui, gg.controller);
		csr.Update(CURSOR_SMART_RELEASE_TIME+0.5f, &thisCursor, global.ui, gg.controller);
		CHECK_EQUAL(true, thisCursor.IsVisible());

		pController->TestKeyPress(VKEY_FORWARD, 2.0f);

		// CURSOR_SMART_RELEASE_TIME �ð� ���� ����Ű�� ������ �־�� Ŀ���� �����ȴ�.
		csr.Update(CURSOR_SMART_RELEASE_TIME + 0.01f, &thisCursor, global.ui, gg.controller);
		CHECK_EQUAL(false, thisCursor.IsVisible());

		thisCursor.Show(false);
	}
}

