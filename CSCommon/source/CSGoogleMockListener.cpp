#include "stdafx.h"
#include "CSGoogleMockListener.h"
#include "CurrentTest.h"
#include "TestResults.h"
#include "TestDetails.h"

void CSGoogleMockListener::InitListener(void)
{
	::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

	// ���� ������ ����
	delete listeners.Release(listeners.default_result_printer());

	// �� ������ �߰�
	listeners.Append(new CSGoogleMockListener);
}

void CSGoogleMockListener::OnTestPartResult(const ::testing::TestPartResult& test_part_result) 
{
	// ���� ��, UnitTest++ ����� ���
	if (test_part_result.failed())
	{
		UnitTest::CurrentTest::Results()->OnTestFailure(
			UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), test_part_result.line_number()), 
			test_part_result.summary()
			);
	}
}
