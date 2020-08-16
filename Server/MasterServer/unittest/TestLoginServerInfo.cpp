#include "stdafx.h"
#include "ZLoginServerInfo.h"
#include "ZLoginServerInfoLoader.h"
#include "ZLoginServerInfoManager.h"
#include "ZFixtureHelper.h"

SUITE(LoginServerInfo)
{
	struct FLoginServerInfo : public FBaseLoginServer
	{
		FLoginServerInfo()
		{

		}
		~FLoginServerInfo()
		{

		}
	};

	TEST(TestLoginServerInfo_Constructor)
	{
		ZLoginServerInfo info;

		CHECK_EQUAL(0, info.m_nID);
	}

	TEST_FIXTURE(FLoginServerInfo, TestLoginServerInfoLoader_Sample1)
	{
		string strXmlSample1 = "<Maiet>						\
							   <LoginServer id='1'>			\
							   </LoginServer>				\
							   <LoginServer id='2'>			\
							   </LoginServer>				\
							   </Maiet>";

		ZLoginServerInfoManager loginServerInfoManager;
		ZLoginServerInfoLoader loader;

		bool bSuccess = loader.LoadStream(&loginServerInfoManager, strXmlSample1.c_str());
		CHECK_EQUAL(true, bSuccess);


		// �α��μ����� �����ͼ������� ���谡 1:1�� �Ǿ�������, �Ѱ��� �α��μ��� ������ �е��� �����Ǿ����ϴ�.


		CHECK_EQUAL(1, loginServerInfoManager.GetSize());

		// 1�� ����
		ZLoginServerInfo* pLoginServerInfo = loginServerInfoManager.GetInfo(1);
		CHECK(pLoginServerInfo != NULL);
		CHECK_EQUAL(1, pLoginServerInfo->m_nID);

		// 2�� ����
		ZLoginServerInfo* pLoginServerInfo2 = loginServerInfoManager.GetInfo(2);
		CHECK(pLoginServerInfo2 == NULL);
	}
}
