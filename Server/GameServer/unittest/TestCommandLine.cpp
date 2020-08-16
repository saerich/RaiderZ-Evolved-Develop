#include "stdafx.h"
#include "MCommandLine.h"

using namespace cml2;

SUITE(CommandLine)
{
	TEST(TestCommandLine_GetSwitchValue)
	{
		MCommandLine cl;
		cl.ParseFromString("prog.exe -aaa=1 -bbb=������ -ccc=\"����ȭ ����\" --ddd");

		string strSwitchValue;
		strSwitchValue = cl.GetSwitchValue("aaa");
		CHECK_EQUAL("1", strSwitchValue);

		strSwitchValue = cl.GetSwitchValue("bbb");
		CHECK_EQUAL("������", strSwitchValue);

		strSwitchValue = cl.GetSwitchValue("ccc");
		CHECK_EQUAL("����ȭ ����", strSwitchValue);

		strSwitchValue = cl.GetSwitchValue("ddd");
		CHECK_EQUAL("", strSwitchValue);

	}

	TEST(TestCommandLine_GetSwitchValueNegative)
	{
		MCommandLine cl;
		cl.ParseFromString("prog.exe -aaa -b bb=������ -ccc=\"����ȭ ����");

		string strSwitchValue;
		strSwitchValue = cl.GetSwitchValue("aaa");
		CHECK_EQUAL("", strSwitchValue);

		strSwitchValue = cl.GetSwitchValue("bbb");
		CHECK_EQUAL("", strSwitchValue);

		strSwitchValue = cl.GetSwitchValue("b");
		CHECK_EQUAL("", strSwitchValue);
	}

	TEST(TestCommandLine_HasSwitch)
	{
		MCommandLine cl;
		cl.ParseFromString("prog.exe -aaa=1 -bbb=������ -ccc=\"����ȭ ����\" --ddd -����ȭ=111 fff ggg hhh");

		CHECK_EQUAL(true, cl.HasSwitch("aaa"));
		CHECK_EQUAL(true, cl.HasSwitch("bbb"));
		CHECK_EQUAL(true, cl.HasSwitch("ccc"));
		CHECK_EQUAL(true, cl.HasSwitch("ddd"));		// "--" �� ���� �͵� ����ġ
		CHECK_EQUAL(true, cl.HasSwitch("����ȭ"));

		CHECK_EQUAL(false, cl.HasSwitch("fff"));


	}

}