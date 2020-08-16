#include "StdAfx.h"
#include "GTestForward.h"
#include "GGlobal.h"
#include "GScriptManager.h"

class TestForLua
{
public:
	TestForLua(): var1(333) {}
	int Func() { return 777; }
	int var1;

	/// �� ���۷����� �����ε�
	inline friend bool operator==(const TestForLua& a, const TestForLua& b){
		return (a.var1==b.var1);
	}

	/// Stream�� ���� ���۷����� �����ε�
	inline friend ostream& operator<<(ostream& stream, const TestForLua& rhs){
		stream << rhs.var1;
		return stream;
	}
};



SUITE(WLua_Table)
{
	struct FWLua
	{
		FWLua()
		{ 
			gsys.pScriptManager->Init();

			// TestForLua �ɺ� ���
			MWLua::class_<TestForLua>(WLUA->GetState(), "TestForLua")
				.def(MWLua::constructor<>())
				.def("Func",			&TestForLua::Func)
				.def_readwrite("var1",	&TestForLua::var1)
				;
		}

		~FWLua() 
		{
			gsys.pScriptManager->Fini();
		}
	};

	TEST_FIXTURE(FWLua, Basic)
	{
		// ���̺��� ����
		MWLua::table t(WLUA->GetState());

		// Ŀ���� �ν��Ͻ� ����
		TestForLua custom;

		// ����� ����
		t.set("var1", 111);
		t.set("var2", 222.222f);
		t.set("var3", "hello");
		t.set("var4", custom);

		// ����� ���
		CHECK_EQUAL(111, t.get<int>("var1"));
		CHECK_CLOSE(222.222f, t.get<float>("var2"), 0.001f);
		CHECK_EQUAL("hello", t.get<const char*>("var3"));
		CHECK_EQUAL(333, t.get<TestForLua>("var4").var1);
	}

	TEST_FIXTURE(FWLua, InnerTable)
	{
		// ���̺��� ����
		MWLua::table t(WLUA->GetState());

		// ���� ���̺� ����
		MWLua::table t2(WLUA->GetState());

		// ����� ����
		t.set("var1", "hello");
		t.set("var2", 111);
		t.set("inner", t2);
		t2.set("var1", "world");
		t2.set("var2", 222.222f);

		// ����� ���
		CHECK_EQUAL("hello", t.get<const char*>("var1"));
		CHECK_EQUAL(111, t.get<int>("var2"));
		MWLua::table inner_table = t.get<MWLua::table>("inner");
		TVALID_EQ(true, inner_table.validate());	// ��ȿ�� ���̺�
		CHECK_EQUAL("world", inner_table.get<const char*>("var1"));
		CHECK_CLOSE(222.222f, inner_table.get<float>("var2"), 0.001f);
	}

	TEST_FIXTURE(FWLua, CheckNullElement)
	{
		// ���̺��� ����
		MWLua::table t(WLUA->GetState());

		// ����� ���
		CHECK_EQUAL(0, t.get<int>("var1"));
		CHECK_CLOSE(0.0f, t.get<float>("var2"), 0.001f);
		CHECK(NULL == t.get<const char*>("var3"));
		CHECK_EQUAL(TestForLua(), t.get<TestForLua>("var4"));
		MWLua::table inner_table = t.get<MWLua::table>("inner");
		CHECK_EQUAL(false, inner_table.validate());	// ��ȿ���� ���� ���̺�
	}

	TEST_FIXTURE(FWLua, PassLuaSide)
	{
		// ��� �׽�Ʈ�Լ� �����
		WRUNSTRING(WLUA, 
			"function test_func(t)\n"
			"TEST1 = t.var1\n"
			"TEST2 = t.var2\n"
			"TEST3 = t.var3\n"
			"TEST4 = t.var4\n"
			"TEST5 = t.inner.var1\n"
			"TEST6 = t.inner.var2\n"
			"TEST7 = t.inner.child.left\n"
			"TEST8 = t.inner.child.right\n"
			"end\n"
			);

		// ���̺��� ����
		MWLua::table t(WLUA->GetState());

		// Ŀ���� �ν��Ͻ� ����
		TestForLua custom;

		// ����� ����
		t.set("var1", 111);
		t.set("var2", 222.222f);
		t.set("var3", "hello");
		t.set("var4", custom);

		// ���� ���̺�
		MWLua::table inner(WLUA->GetState());
		inner.set("var1", "world");
		inner.set("var2", 333.333f);
		t.set("inner", inner);

		// ������ ���� ���̺�
		TestForLua custom2;
		custom2.var1 = 789;
		MWLua::table inner2(WLUA->GetState());
		inner2.set("left", "why so serious?");
		inner2.set("right", custom2);
		inner.set("child", inner2);

		// ��� �׽�Ʈ�Լ��� ������̺� �ѱ��
		WCALL1(WLUA, "test_func", t, WNULL); 

		// ����� ���
		CHECK_EQUAL(111, WLUA->GetVar<int>("TEST1"));
		CHECK_CLOSE(222.222f, WLUA->GetVar<float>("TEST2"), 0.001f);
		CHECK_EQUAL("hello", WLUA->GetVar<const char*>("TEST3"));
		CHECK_EQUAL(333, WLUA->GetVar<TestForLua>("TEST4").var1);
		CHECK_EQUAL("world", WLUA->GetVar<const char*>("TEST5"));
		CHECK_CLOSE(333.333f, WLUA->GetVar<float>("TEST6"), 0.001f);
		CHECK_EQUAL("why so serious?", WLUA->GetVar<const char*>("TEST7"));
		CHECK_EQUAL(789, WLUA->GetVar<TestForLua>("TEST8").var1);
	}

	TEST_FIXTURE(FWLua, PassCppSide)
	{
		// ���̺� �����Լ� �����
		WRUNSTRING(WLUA, 
			"function create_table(var1, var2, var3, var4)\n"
			"local t = {}\n"
			"t.var1 = var1\n"
			"t.var2 = var2\n"
			"t.var3 = var3\n"
			"t.var4 = var4\n"
			"t.inner = {}\n"
			"t.inner.var1 = var4\n"
			"t.inner.var2 = var3\n"
			"t.inner.var3 = var2\n"
			"t.inner.var4 = var1\n"
			"t.inner.child = {}\n"
			"t.inner.child.left = \"why so serious?\"\n"
			"t.inner.child.right = TestForLua()\n"
			"return t\n"
			"end\n"
			);

		// Ŀ���� �ν��Ͻ� ����
		TestForLua custom;
		custom.var1 = 123;

		// ��ƿ��� ���� ���̺� �ޱ�
		MWLua::table t;
		WCALL4(WLUA, "create_table", 
				111, 222.222f, "hello", custom,
				&t);

//		CHECK_EQUAL(0, WLUA->GetVar<int>("invalid_value"));

		// ����� üũ
		CHECK_EQUAL(111, t.get<int>("var1"));
		CHECK_EQUAL(true, t.is_exist("var1"));
		CHECK_EQUAL(false, t.is_exist("rav777"));
		CHECK_CLOSE(222.222f, t.get<float>("var2"), 0.001f);
		CHECK_EQUAL("hello", t.get<const char*>("var3"));
		CHECK_EQUAL(123, t.get<TestForLua>("var4").var1);
		MWLua::table inner_table = t.get<MWLua::table>("inner");
		TVALID_EQ(true, inner_table.validate());	// ��ȿ�� ���̺�
		CHECK_EQUAL(123, inner_table.get<TestForLua>("var1").var1);
		CHECK_EQUAL("hello", inner_table.get<const char*>("var2"));
		CHECK_CLOSE(222.222f, inner_table.get<float>("var3"), 0.001f);
		CHECK_EQUAL(111, inner_table.get<int>("var4"));
		MWLua::table child_table = inner_table.get<MWLua::table>("child");
		TVALID_EQ(true, child_table.validate());	// ��ȿ�� ���̺�
		CHECK_EQUAL("why so serious?", child_table.get<const char*>("left"));
		CHECK_EQUAL(333, child_table.get<TestForLua>("right").var1);
	}

	vector<int> g_vecTestFuncPassedVars;
	void TestFunc(MWLua::table t)
	{
		t.iteration();
		int nVar;
		while (t.next<int>(&nVar))  
		{ 
			g_vecTestFuncPassedVars.push_back(nVar);
		} 
	}

	TEST_FIXTURE(FWLua, DirictlyPassCppSide)
	{
		// ���̺� �����Լ� �����
		WRUNSTRING(WLUA, 
			"function test_func(t)\n"
			"	local ret=0\n"
			"	for key,var in pairs(t) do ret = ret + var end\n"
			"	return ret\n"
			"end\n"
			);

		// ����� ����
		MWLua::table t(WLUA->GetState());
		t.set("var1", 111);
		t.set("var2", 222);
		t.set("var3", 333);

		// ��� �׽�Ʈ�Լ��� ������̺� �ѱ��
		int nRet=0;
		WCALL1(WLUA, "test_func", t, &nRet);

		CHECK_EQUAL(666, nRet);
	}

	TEST_FIXTURE(FWLua, DirictlyPassLuaSide)
	{
		WLUA->Def("TestFunc", &TestFunc);

		// ���̺� �����Լ� �����
		WRUNSTRING(WLUA, 
			"TestFunc({1, 2, 3, 5, 8})\n"
			);

		TVALID_EQ(5U, g_vecTestFuncPassedVars.size());
		CHECK_EQUAL(1, g_vecTestFuncPassedVars[0]);
		CHECK_EQUAL(2, g_vecTestFuncPassedVars[1]);
		CHECK_EQUAL(3, g_vecTestFuncPassedVars[2]);
		CHECK_EQUAL(5, g_vecTestFuncPassedVars[3]);
		CHECK_EQUAL(8, g_vecTestFuncPassedVars[4]);
	}


	TEST_FIXTURE(FWLua, PassCross)
	{
		WRUNSTRING(WLUA, 
			// ��� �׽�Ʈ�Լ� �����
			"function create_table(var1, var2, var3, var4)\n"
			"local t = {}\n"
			"t.var1 = var1\n"
			"t.var2 = var2\n"
			"t.var3 = var3\n"
			"t.var4 = var4\n"
			"return t\n"
			"end\n"
			// ���̺� �����Լ� �����
			"function test_func(t)\n"
			"TEST1 = t.var1\n"
			"TEST2 = t.var2\n"
			"TEST3 = t.var3\n"
			"TEST4 = t.var4\n"
			"end\n"
			);

		// Ŀ���� �ν��Ͻ� ����
		TestForLua custom;
		custom.var1 = 123;

		// ��ƿ��� ���� ���̺� �ޱ�
		MWLua::table t;
		WCALL4(WLUA, "create_table", 
			111, 222.222f, "hello", custom,
			&t);

		// ��� �׽�Ʈ�Լ��� ������̺� �ѱ��
		WCALL1(WLUA, "test_func", t, WNULL);

		// ����� ���
		CHECK_EQUAL(111, WLUA->GetVar<int>("TEST1"));
		CHECK_CLOSE(222.222f, WLUA->GetVar<float>("TEST2"), 0.001f);
		CHECK_EQUAL("hello", WLUA->GetVar<const char*>("TEST3"));
		CHECK_EQUAL(123, WLUA->GetVar<TestForLua>("TEST4").var1);
	}
}