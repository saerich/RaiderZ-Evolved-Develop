#pragma once

#include <gmock/gmock.h>

// GoogleMock ���� �޽��� UnitTest++ �����ͷ� ��� Ŭ����
class CSGoogleMockListener: public testing::EmptyTestEventListener
{
public:
	static void InitListener(void);

	virtual void OnTestPartResult(const ::testing::TestPartResult& test_part_result);	
};
