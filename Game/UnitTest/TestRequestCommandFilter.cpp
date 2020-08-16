#include "stdafx.h"
#include "XNetClient.h"
#include "XRequestCommandFilter.h"

SUITE(RequestCommandFilter)
{
	TEST(TestRequestCommandFilter__Success)
	{
		XRequestCommandFilter filter;

		filter.AddFilter(MC_CHAR_REQUEST_SEL_MYCHAR, MC_CHAR_RESPONSE_SEL_MYCHAR);

		CHECK_EQUAL(true, filter.CheckRequestEnable(MC_CHAR_REQUEST_SEL_MYCHAR));

		// �̹� �ѹ� �������ϱ� �� ���������� ���� �� ����.
		CHECK_EQUAL(false, filter.CheckRequestEnable(MC_CHAR_REQUEST_SEL_MYCHAR));

		// Response Ŀ�ǵ� ����
		filter.OnResponse(MC_CHAR_RESPONSE_SEL_MYCHAR);

		// ���� �ٽ� request�� ���� �� �ִ�.
		CHECK_EQUAL(true, filter.CheckRequestEnable(MC_CHAR_REQUEST_SEL_MYCHAR));
	}

	TEST(TestRequestCommandFilter__CheckRequestEnable)
	{
		XRequestCommandFilter filter;

		filter.AddFilter(MC_CHAR_REQUEST_SEL_MYCHAR, MC_CHAR_RESPONSE_SEL_MYCHAR);

		// ���Ϳ� ���� Ŀ�ǵ�� ������ Request ����
		CHECK_EQUAL(true, filter.CheckRequestEnable(MC_COMM_REQUEST_ACCOUNT_CHAR_LIST));
	}

	TEST(TestRequestCommandFilter__ClearSendRequestCommands)
	{
		XRequestCommandFilter filter;

		filter.AddFilter(MC_CHAR_REQUEST_SEL_MYCHAR, MC_CHAR_RESPONSE_SEL_MYCHAR);

		CHECK_EQUAL(true, filter.CheckRequestEnable(MC_CHAR_REQUEST_SEL_MYCHAR));
		CHECK_EQUAL(false, filter.CheckRequestEnable(MC_CHAR_REQUEST_SEL_MYCHAR));

		filter.ClearSendRequestCommands();

		CHECK_EQUAL(true, filter.CheckRequestEnable(MC_CHAR_REQUEST_SEL_MYCHAR));
	}

}