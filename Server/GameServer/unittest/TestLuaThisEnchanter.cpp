#include "stdafx.h"
#include "GTestForward.h"
#include "GLuaThisEnchanter.h"

SUITE(TestLuaThisEnchanter)
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

	TEST(ConvertEachLine)
	{
		const int ActionQty=4;
		struct ScriptConvertCheck
		{
			const char* pszScript;
			const char* pszExpected;
		} cases[] = {
			//// ���� 0�� --------------------------------------------
			// [����] �����Լ�, �Լ����Ī
			{"function OnAggro()", "function OnAggro()"	},
			// [����] �����Լ�, �Լ�����Ī
			{"function OnPowerup()", "function OnPowerup()"},
			// [��ȯ] ����Լ�, �Լ����Ī
			{"function NPC_0110001:OnAggro()","function NPC_0110001:OnAggro(this)"},
			// [��ȯ] ����Լ� (�����߰�), �Լ����Ī
			{"function NPC_0110001:OnAggro( )","function NPC_0110001:OnAggro(this )"},
			// [����] ����Լ�, �Լ�����Ī
			{"function NPC_0110001:OnPowerup()","function NPC_0110001:OnPowerup()"},
			// [����] ����Լ� (�����߰�), �Լ�����Ī
			{"function NPC_0110001:OnPowerup( )","function NPC_0110001:OnPowerup( )"},

			// ���� 1�� --------------------------------------------
			// [����] �����Լ�, �Լ����Ī
			{"function OnAggro(arg1)", "function OnAggro(arg1)"	},
			// [����] �����Լ�, �Լ�����Ī
			{"function OnPowerup(arg1)", "function OnPowerup(arg1)"},
			// [��ȯ] ����Լ�, �Լ����Ī
			{"function NPC_0110001:OnAggro(arg1)","function NPC_0110001:OnAggro(this, arg1)"},
			// [����] ����Լ�, �Լ�����Ī
			{"function NPC_0110001:OnPowerup(arg1)","function NPC_0110001:OnPowerup(arg1)"},

			// ���� 2�� --------------------------------------------
			// [����] �����Լ�, �Լ����Ī
			{"function OnAggro(arg1, arg2)", "function OnAggro(arg1, arg2)"	},
			// [����] �����Լ�, �Լ�����Ī
			{"function OnPowerup(arg1, arg2)", "function OnPowerup(arg1, arg2)"},
			// [��ȯ] ����Լ�, �Լ����Ī
			{"function NPC_0110001:OnAggro(arg1, arg2)","function NPC_0110001:OnAggro(this, arg1, arg2)"},
			// [����] ����Լ�, �Լ�����Ī
			{"function NPC_0110001:OnPowerup(arg1, arg2)","function NPC_0110001:OnPowerup(arg1, arg2)"},

			// ���� �Լ��� --------------------------------------------
			{"function NPC_0110001:OnHitCapsule_1(arg1, arg2)","function NPC_0110001:OnHitCapsule_1(this, arg1, arg2)"},
			{"function NPC_0110001:OnHitCapsule_3_2(arg1, arg2)","function NPC_0110001:OnHitCapsule_3_2(this, arg1, arg2)"}
		};

		// ����Լ��� ���
		GLuaThisEnchanter enchanter;
		enchanter.AddMatchMemFunc("OnDialog");
		enchanter.AddMatchMemFunc("OnAggro");
		enchanter.AddMatchMemFunc("OnHitCapsule");
		enchanter.AddMatchMemFunc("OnInteract");

		// �׽�Ʈ����
		for (int i=0; i<ArraySize(cases); ++i) 
		{
			ScriptConvertCheck check = cases[i];
			CHECK_EQUAL(check.pszExpected, enchanter.ConvertLine(check.pszScript));	
		}
	}


	TEST(ConvertLuaScriptStreamParam0)
	{
		string strLua = "function TEST_1:OnTest()\nlocal Val = 1\nend";

		const string strCheck = "function TEST_1:OnTest(this)\nlocal Val = 1\nend\n";

		string strCnv;
		GLuaThisEnchanter e;

		e.AddMatchMemFunc("TEST_1:OnTest");

		CHECK(e.ConvertFromStream(strLua, strCnv));
		CHECK_EQUAL(strCheck, strCnv);
	}


	TEST(ConvertLuaScriptStreamParam1)
	{
		string strLua = "function TEST_1:OnTest(arg1)\nlocal Val = 1\nend";

		const string strCheck = "function TEST_1:OnTest(this, arg1)\nlocal Val = 1\nend\n";

		string strCnv;
		GLuaThisEnchanter e;

		e.AddMatchMemFunc("TEST_1:OnTest");

		CHECK(e.ConvertFromStream(strLua, strCnv));
		CHECK_EQUAL(strCheck, strCnv);
	}


	TEST(ConvertLuaScriptStreamParam2)
	{
		string strLua = "function TEST_1:OnTest(arg1, arg2)\nlocal Val = 1\nend";

		const string strCheck = "function TEST_1:OnTest(this, arg1, arg2)\nlocal Val = 1\nend\n";

		string strCnv;
		GLuaThisEnchanter e;

		e.AddMatchMemFunc("TEST_1:OnTest");

		CHECK(e.ConvertFromStream(strLua, strCnv));
		CHECK_EQUAL(strCheck, strCnv);
	}
}


