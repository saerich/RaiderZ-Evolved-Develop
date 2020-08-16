#include "stdafx.h"
#include "CSStringFilter.h"

SUITE(StringFilter)
{
	struct Fixture
	{
		Fixture()
		{
			m_filter = GetStringFilter();
			m_filter->LoadFromStringDB(_T("���۳�\r\naibeast\r\nEND"));
		}
		~Fixture()
		{
			m_filter->Clear();
		}

		CSStringFilter* m_filter;
	};

	TEST_FIXTURE(Fixture, TestStringFilter_Success)
	{
		tstring strOriginString = _T("�ȳ��ϼ��� aibeast�Դϴ�. ai@#$beast");

		bool bRet = m_filter->IsValidStr(strOriginString.c_str());
		CHECK_EQUAL(false, bRet);

		tstring strFilteredString = m_filter->Filter(strOriginString.c_str());
		tstring strExpectString = _T("�ȳ��ϼ��� *******�Դϴ�. **********");

		bRet = (strExpectString == strFilteredString);
		CHECK_EQUAL(true, bRet);
	}

	TEST_FIXTURE(Fixture, TestStringFilter_Success2)
	{
		tstring strOriginString = _T("!@#aibeast");

		bool bRet = m_filter->IsValidStr(strOriginString.c_str());
		CHECK_EQUAL(false, bRet);

		tstring strFilteredString = m_filter->Filter(strOriginString.c_str());
		tstring strExpectString = _T("!@#*******");

		bRet = (strExpectString == strFilteredString);
		CHECK_EQUAL(true, bRet);
	}

	TEST_FIXTURE(Fixture, TestStringFilter_Success3)
	{
		tstring strOriginString = _T("��1�۳�");

		bool bRet = m_filter->IsValidStr(strOriginString.c_str());
		CHECK_EQUAL(false, bRet);

		tstring strFilteredString = m_filter->Filter(strOriginString.c_str());
		tstring strExpectString = _T("****");

		bRet = (strExpectString == strFilteredString);
		CHECK_EQUAL(true, bRet);
	}

	TEST_FIXTURE(Fixture, TestStringFilter_NotFiltered)
	{
		tstring strOriginString = _T("��1�ѳ�");

		bool bRet = m_filter->IsValidStr(strOriginString.c_str());
		CHECK_EQUAL(true, bRet);

		tstring strFilteredString = m_filter->Filter(strOriginString.c_str());
		tstring strExpectString = _T("��1�ѳ�");

		bRet = (strExpectString == strFilteredString);
		CHECK_EQUAL(true, bRet);
	}

}
