#include "stdafx.h"
#include "GTestForward.h"
#include "CSNameStringChecker.h"
#include "CSNameWordFilter.h"

SUITE(NameStringChecker)
{
	TEST(ShouldBeChararacterLegnthLargerThenDefaultSize)
	{
		struct CheckData
		{
			const wchar_t* pszWord;
			CSNameStringChecker::ERROR_CODE nRetCode;
		} cases[] = {
			// ����, �ʹ� ª��
			{L"", CSNameStringChecker::ERROR_LENGTH_TOO_SHORT },
			// ����, �ʹ� ª��
			{L"a", CSNameStringChecker::ERROR_LENGTH_TOO_SHORT },
			// ����, ������ ����
			{L"hello", CSNameStringChecker::OK },
			// ����, �ʹ� ����
			{L"helloworldhowareyoudoinghere", CSNameStringChecker::ERROR_LENGTH_TOO_LARGE },
			// �ѱ�, �ʹ� ª��
			{L"��", CSNameStringChecker::ERROR_LENGTH_TOO_SHORT },
			// �ѱ�, ������ ����
			{L"����", CSNameStringChecker::OK },
			// �ѱ�, �ʹ� ����
			{L"���̽�ũ�����ֽ��ϴ�", CSNameStringChecker::ERROR_LENGTH_TOO_LARGE },
		};

		for (int i=0; i<ArraySize(cases); ++i) 
		{
			CheckData data = cases[i];
			CSNameStringChecker checker(data.pszWord);
			CHECK_EQUAL(data.nRetCode, checker.Check());
		}
	}

	TEST(ShouldNotBeConatinNumberAndSpace)
	{
		struct CheckData
		{
			const wchar_t* pszWord;
			CSNameStringChecker::ERROR_CODE nRetCode;
		} cases[] = {
			// ����, ���ڰ� ���� ����
			{L"test", CSNameStringChecker::OK},
			// ����, ���ڰ� ��
			{L"aa347", CSNameStringChecker::ERROR_CONTAIN_NUMBER_OR_SPACE},
			// ����, ���ڰ� ��
			{L"347aa", CSNameStringChecker::ERROR_CONTAIN_NUMBER_OR_SPACE},
			// ����, ��������
			{L"t est", CSNameStringChecker::ERROR_CONTAIN_NUMBER_OR_SPACE},
			// ����, ��������
			{L" test", CSNameStringChecker::ERROR_CONTAIN_NUMBER_OR_SPACE},
			// ����, ��������
			{L"test ", CSNameStringChecker::ERROR_CONTAIN_NUMBER_OR_SPACE},
		};

		for (int i=0; i<ArraySize(cases); ++i) 
		{
			CheckData data = cases[i];
			CSNameStringChecker checker(data.pszWord);
			CHECK_EQUAL(data.nRetCode, checker.Check());
		}
	}

	TEST(ShouldBeConatinValidHangul)
	{
		tstring strIllegalStr = _T("END");
		tstring strAllowedChar = _T("��\r\nEND");
		GetNameWordFilter()->LoadFromString((TCHAR*)strIllegalStr.c_str(), (TCHAR*)strAllowedChar.c_str());

		struct CheckData
		{
			const wchar_t* pszWord;
			CSNameStringChecker::ERROR_CODE nRetCode;
		} cases[] = {
			// ����
			{L"test", CSNameStringChecker::OK},
			// �ѱ� �ϼ���
			{L"��������", CSNameStringChecker::OK},
			// �ѱ� �ϼ���
			{L"�ƾƾƾ�", CSNameStringChecker::OK},
			// �ѱ� �ϼ���
			{L"��������", CSNameStringChecker::OK},
			// �ѱ� Ȯ��ϼ���
			{L"�����", CSNameStringChecker::ERROR_CONTAIN_INVALID_CHARACTER},
			// �ѱ� Ȯ��ϼ���
			{L"����f��", CSNameStringChecker::ERROR_CONTAIN_INVALID_CHARACTER},
			// Ư������
			{L"�١ڡۡΡϡ���", CSNameStringChecker::ERROR_CONTAIN_INVALID_CHARACTER},
		};

		for (int i=0; i<ArraySize(cases); ++i) 
		{
			CheckData data = cases[i];
			CSNameStringChecker checker(data.pszWord);
			CHECK_EQUAL(data.nRetCode, checker.Check());
		}

		GetNameWordFilter()->Clear();
	}

	TEST(NumberOnly)
	{
		struct CheckData
		{
			const wchar_t* pszWord;
			CSNameStringChecker::ERROR_CODE nRetCode;
		} cases[] = {
			// ���ڸ�
			{L"123456", CSNameStringChecker::ERROR_NUMBER_ONLY},
			// ���� ª��
			{L"123", CSNameStringChecker::ERROR_NUMBER_ONLY},
			// ���� ��
			{L"1234567890", CSNameStringChecker::ERROR_NUMBER_ONLY},
		};

		for (int i=0; i<ArraySize(cases); ++i) 
		{
			CheckData data = cases[i];
			CSNameStringChecker checker(data.pszWord);
			CHECK_EQUAL(data.nRetCode, checker.Check());
		}
	}

	TEST(CheckHanguleAssemble)
	{
		struct CheckData
		{
			const wchar_t* pszWord;
			CSNameStringChecker::ERROR_CODE nRetCode;
		} cases[] = {
			// �ʼ���
			{L"��������", CSNameStringChecker::ERROR_CONTAIN_INVALID_CHARACTER},
			// �߼���
			{L"�����Ǥ�", CSNameStringChecker::ERROR_CONTAIN_INVALID_CHARACTER},
			// ������
			{L"��������", CSNameStringChecker::ERROR_CONTAIN_INVALID_CHARACTER},
		};

		for (int i=0; i<ArraySize(cases); ++i) 
		{
			CheckData data = cases[i];
			CSNameStringChecker checker(data.pszWord);
			CHECK_EQUAL(data.nRetCode, checker.Check());
		}
	}

	TEST(MixingCheck)
	{
		struct CheckData
		{
			const wchar_t* pszWord;
			CSNameStringChecker::ERROR_CODE nRetCode;
		} cases[] = {
			// �ѱ� + ����
			{L"abc������abc", CSNameStringChecker::ERROR_MIXING_LANGUAGE},
			// �ѱ� + ����
			{L"������111", CSNameStringChecker::ERROR_CONTAIN_NUMBER_OR_SPACE},
			// ���� + ����
			{L"abcDEF111", CSNameStringChecker::ERROR_CONTAIN_NUMBER_OR_SPACE},
			// �ѱ� + Ư��
			{L"���١ڡϳ���", CSNameStringChecker::ERROR_CONTAIN_INVALID_CHARACTER},
			// ���� + Ư��
			{L"aaa�١ڡ�bbb", CSNameStringChecker::ERROR_MIXING_LANGUAGE},
			// �ѱ� + ���� + Ư��
			{L"��a�ٸ����ڡ�b��", CSNameStringChecker::ERROR_MIXING_LANGUAGE},
			// Ȯ���ѱ� + ����
			{L"abc���abc", CSNameStringChecker::ERROR_MIXING_LANGUAGE},
			// Ȯ���ѱ� + ����
			{L"ac ��� bc", CSNameStringChecker::ERROR_CONTAIN_NUMBER_OR_SPACE},
		};

		for (int i=0; i<ArraySize(cases); ++i) 
		{
			CheckData data = cases[i];
			CSNameStringChecker checker(data.pszWord);
			CHECK_EQUAL(data.nRetCode, checker.Check());
		}
	}

	TEST(CheckAlphabetless)
	{
		struct CheckData
		{
			const wchar_t* pszWord;
			CSNameStringChecker::ERROR_CODE nRetCode;
		} cases[] = {
			// a
			{L"aaaaaa", CSNameStringChecker::OK},
			// z
			{L"zzzzzz", CSNameStringChecker::OK},
			// A
			{L"AAAAAA", CSNameStringChecker::OK},
			// Z
			{L"ZZZZZZ", CSNameStringChecker::OK},
			// !
			{L"!!!!!!", CSNameStringChecker::ERROR_CONTAIN_ALPHABETLESS},
			// @
			{L"@@@@@@", CSNameStringChecker::ERROR_CONTAIN_ALPHABETLESS},
			// +
			{L"++++++", CSNameStringChecker::ERROR_CONTAIN_ALPHABETLESS},
		};

		for (int i=0; i<ArraySize(cases); ++i) 
		{
			CheckData data = cases[i];
			CSNameStringChecker checker(data.pszWord);
			CHECK_EQUAL(data.nRetCode, checker.Check());
		}
	}
}


