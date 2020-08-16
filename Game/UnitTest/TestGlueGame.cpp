#include "stdafx.h"
#include "XGlueGame.h"
#include "XGlobal.h"
#include "XGlobalInstance.h"

#define WLUA global.script->GetLua()

SUITE(GlueGame)
{
	struct TestGlueGame
	{		
	};

	TEST_FIXTURE(TestGlueGame, Format)
	{
		// ��� ��ũ��Ʈ ���
		const char* szScript =
			"function TestFormat()\n"
			"	return gamefunc:Format(\"FormatMessage {0}, {1}, {2}, {3}\", \"test1\", 2, 3, \"test4\")\n"
			"end";

		ASSERT_CHECK(WRUNSTRING(WLUA, szScript));


		// ��ũ��Ʈ ȣ��
		const char* szResult;
		WCALL(WLUA, "TestFormat", &szResult);
		CHECK("FormatMessage test1, 2, 3, test4" == string(szResult));
	}
}
