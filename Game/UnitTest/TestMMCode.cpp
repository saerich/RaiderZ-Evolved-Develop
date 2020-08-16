#include "stdafx.h"
#include "XMMCode.h"

SUITE(MMcode)
{
	class FMMCode
	{
	public:
		mmcode::MControlCodeTransResult Trans(std::wstring strText)
		{
			mmcode::MControlCodeTransResult result = 
				XMMCodeMgr::GetInstance().TransControlCodeAndReplaceChars(strText.c_str(), XGetMyUID());
			return result;
		}
	};
	TEST_FIXTURE(FMMCode, TestMMCode_Case1)
	{
		USES_CONVERSION_EX;

		std::wstring strQuestDesc = L"{character}���{/character}�� ������ ��� �ִ��� �ƴ� ���� ������ {character}����{/character}�� {character}�̽���{/character} ���̶�� �մϴ�. {CR}{CR}������ {place}�����{/place}�� ����, �츣ũ���� ������ ���ٰ� ���ƿ��� �ʰ� �ִ� ���Ȱ� �̽����� ����� Ȯ���Ͽ� ��ҿ��� �����Ͻʽÿ�.{CR}{COLOR r=50 g=200 b=50}��ҿ� �̽����� ���� ��ġ�� ������ �����ϴ�.{/COLOR}{CR}{BITMAP name=\"Quest0001\" w=250 h=250}{SPACE w=250 h=260}";

		mmcode::MControlCodeTransResult result = Trans(strQuestDesc);
		
		std::wstring strExpected = L"{character}���{/character}�� ������ ��� �ִ��� �ƴ� ���� ������ {character}����{/character}�� {character}�̽���{/character} ���̶�� �մϴ�. {CR}{CR}������ {place}�����{/place}�� ����, �츣ũ���� ������ ���ٰ� ���ƿ��� �ʰ� �ִ� ���Ȱ� �̽����� ����� Ȯ���Ͽ� ��ҿ��� �����Ͻʽÿ�.{CR}{COLOR r=50 g=200 b=50}��ҿ� �̽����� ���� ��ġ�� ������ �����ϴ�.{/COLOR}{CR}{BITMAP name=\"Quest0001\" w=250 h=250}{SPACE w=250 h=260}";

		CHECK(strExpected == A2W_EX(result.m_strOutput.c_str(), 0));
	}

	TEST_FIXTURE(FMMCode, TestMMCode_Case2)
	{
		USES_CONVERSION_EX;

		std::wstring strQuestDesc = L"{ani=bow}����ȭ��{talent=52}";

		mmcode::MControlCodeTransResult result = Trans(strQuestDesc);

		std::wstring strExpected = L"����ȭ��";

		CHECK(strExpected == A2W_EX(result.m_strOutput.c_str(), 0));
	}

}