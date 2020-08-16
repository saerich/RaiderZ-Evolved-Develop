
#include "stdafx.h"
#include "GTestForward.h"

SUITE(ScriptHandler)
{
	struct Fixture
	{
		Fixture()
		{
			gsys.pScriptManager->Init();
		}

		~Fixture() 
		{
			gsys.pScriptManager->Fini();
		}
	};

	TEST(CreateHandlerInstance)
	{
		gsys.pScriptManager->Fini();

		// �λ���
		CHECK(!GetScriptField());
		CHECK(!GetScriptQuest());
		CHECK(!GetScriptNPC());
		// �ʱ�ȭ�ǰ� �ν��Ͻ� ����
		gsys.pScriptManager->Init();
		CHECK(GetScriptField());
		CHECK(GetScriptQuest());
		CHECK(GetScriptNPC());
		// �������ǰ� �ν��Ͻ� ����
		gsys.pScriptManager->Fini();
		CHECK(!GetScriptField());
		CHECK(!GetScriptQuest());
		CHECK(!GetScriptNPC());
	}

	TEST(MakeLuaString)
	{
		// ���ϸ�
		CHECK_EQUAL(MAKE_LUAFILE_FIELD(101), "Field_101");
		CHECK_EQUAL(MAKE_LUAFILE_QUEST(707), "Quest_707");
		// ���̺��
		CHECK_EQUAL(MAKE_LUATABLE_FIELD(101), "Field_101");
		CHECK_EQUAL(MAKE_LUATABLE_QUEST(707), "Quest_707");
		// �Լ���
		CHECK_EQUAL(MAKE_LUAFUNC0("FUNC"), "FUNC");
		CHECK_EQUAL(MAKE_LUAFUNC1("FUNC", 1), "FUNC_1");
		CHECK_EQUAL(MAKE_LUAFUNC2("FUNC", 1, 2), "FUNC_1_2");
	}

	TEST_FIXTURE(Fixture, MakeTable)
	{
		CHECK_EQUAL(false, WLUA->IsExistGlobalTable("haha"));
		CHECK_EQUAL(false, WLUA->IsExistGlobalTable("haha2"));
		MAKE_TABLE("haha");
		MAKE_TABLE("haha2");
		CHECK_EQUAL(true, WLUA->IsExistGlobalTable("haha"));
		CHECK_EQUAL(true, WLUA->IsExistGlobalTable("haha2"));
	}
}


