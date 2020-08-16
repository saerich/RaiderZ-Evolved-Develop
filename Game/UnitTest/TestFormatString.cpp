#include "stdafx.h"
#include "CSFormatString.h"
#include "CStringResMgr.h"

SUITE(FormatString)
{
	struct MockDictionary: public CStringResMgr
	{
		MOCK_METHOD1(Plural,wstring (wstring strKey));
	};

	struct TestFormatString
	{
		MockDictionary m_dic;

		TestFormatString()
		{
			CSFormatString::SetDictionary(&m_dic);
		}

		~TestFormatString()
		{
			CSFormatString::SetDictionary(NULL);
		}
	};


	// �Ķ���� ����
	TEST_FIXTURE(TestFormatString, NoParam)
	{
		wstring strForamt = L"TestNoParam {0}, {1}, {2}";
		wstring strResult = CSFormatString::Format(strForamt);
		CHECK(L"TestNoParam 0, 1, 2" == strResult);
	}

	// ���ڿ� �Ķ����
	TEST_FIXTURE(TestFormatString, EmptyStringParam)
	{
		wstring strForamt = L"EmptyParam_{0}{1}{2}{3}";
		wstring strResult = CSFormatString::Format(strForamt, FSParam(L"Test", L"", L"Param", L""));
		CHECK(L"EmptyParam_TestParam" == strResult);
	}

	// �ε��� ����
	TEST_FIXTURE(TestFormatString, Index)
	{		
		wstring strFormat = L"{2}, {0}, {1}";
		wstring strObject[] = {L"������", L"�ٳ���", L"���"};

		wstring strResult = CSFormatString::Format(strFormat, FSParam(strObject[0], strObject[1], strObject[2]));
		CHECK(L"���, ������, �ٳ���" == strResult);
	}

	// ������, ������
	TEST_FIXTURE(TestFormatString, ParticleCasting)
	{
		wstring strFormat = L"{0,��} {1,��} {2,��}";
		wstring strObject[] = {L"�����", L"Ŭ����", L"�м�"};

		wstring strResult = CSFormatString::Format(strFormat, FSParam(strObject[0], strObject[1], strObject[2]));
		CHECK(L"������� Ŭ������ �м⸦" == strResult);
	}


	// ������/������ �� ���̻縦 {}���� �ǵڿ� ���̵��� �ߴ� - �ܾ �ٹ��� ���� �� �ִ�.
	TEST_FIXTURE(TestFormatString, PostfixParticleCasting)
	{
		wstring strFormat = L"����� {0,��:<<$>>} ���ʴϴ�.";
		wstring strObject[] = {L"����"};

		wstring strResult = CSFormatString::Format(strFormat, FSParam(strObject[0]));
		CHECK(L"����� <<����>>�� ���ʴϴ�." == strResult);
	}

	// ����ܾ� ������
	TEST_FIXTURE(TestFormatString, PluralCasting)
	{
		using testing::_;
		using testing::Return;
		EXPECT_CALL(m_dic, Plural(_))
			.WillRepeatedly(Return(L"wolves"));


		wstring strFormat = L"{0,s} {0}";
		wstring strObject = L"wolf";

		wstring strResult = CSFormatString::Format(strFormat, FSParam(strObject));
		CHECK(L"wolves wolf" == strResult);
	}

	// �ڱ����� ��¾��
	TEST_FIXTURE(TestFormatString, SelfReferenceFormat)
	{
		wstring strFormat = L"{0:����� $ �Դϴ�.}";
		wstring strObject = L"�����";

		wstring strResult = CSFormatString::Format(strFormat, FSParam(strObject));
		CHECK(L"����� ����� �Դϴ�." == strResult);
	}

	// ��� ��¾��
	TEST_FIXTURE(TestFormatString, RecursiveFormat)
	{
		wstring strFormat = L"{0:����� {1,��} �����մϴ�.}";
		wstring strObject[] = {L"Ŭ����", L"�����"};

		wstring strResult = CSFormatString::Format(strFormat, FSParam(strObject[0], strObject[1]));
		CHECK(L"����� ������� �����մϴ�." == strResult);
	}

	// �׷����� ��¾��
	TEST_FIXTURE(TestFormatString, GroupFormat)
	{
		wstring strFormat = L"{0:����;�ϳ�;����}";

		CHECK(L"����" == CSFormatString::Format(strFormat, FSParam(0)));
		CHECK(L"�ϳ�" == CSFormatString::Format(strFormat, FSParam(1)));
		CHECK(L"����" == CSFormatString::Format(strFormat, FSParam(2)));
	}
}
