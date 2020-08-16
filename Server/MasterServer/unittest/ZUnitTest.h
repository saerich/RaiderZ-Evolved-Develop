#pragma once

#include "UnitTest++.h"
#include "ZUnitTestRunner.h"

#define UNITTEST_STAGE1_NAME		"Stage1"


// ���� ��, TEST ����
#define ASSERT_CHECK(value)	\
     do \
    { \
		int nFailureCount = UnitTest::CurrentTest::Results()->GetFailureCount();	\
        try { \
            if (!UnitTest::Check(value)) \
                UnitTest::CurrentTest::Results()->OnTestFailure( UnitTest::TestDetails(m_details, __LINE__), #value); \
        } \
        catch (...) { \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(m_details, __LINE__), \
                    "Unhandled exception in CHECK(" #value ")"); \
        } \
		if (nFailureCount < UnitTest::CurrentTest::Results()->GetFailureCount())	return;	\
    } while (0)


// ���� ��, TEST ����
#define ASSERT_EQUAL(expected, actual)	\
    do \
    { \
		int nFailureCount = UnitTest::CurrentTest::Results()->GetFailureCount();	\
        try { \
            UnitTest::CheckEqual(*UnitTest::CurrentTest::Results(), expected, actual, UnitTest::TestDetails(m_details, __LINE__)); \
        } \
        catch (...) { \
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(m_details, __LINE__), \
                    "Unhandled exception in CHECK_EQUAL(" #expected ", " #actual ")"); \
        } \
		if (nFailureCount < UnitTest::CurrentTest::Results()->GetFailureCount())	return;	\
    } while (0)
