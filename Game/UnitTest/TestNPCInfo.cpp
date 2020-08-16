#include "stdafx.h"
#include "XNPCInfo.h"

SUITE(NPCInfo)
{
	class TestNPCInfo : public XNPCInfo
	{
	public:
		using XNPCInfo::m_bShowIndicator;

		TestNPCInfo() : XNPCInfo() {}
	};
	class FNPCInfo
	{
	public:
		FNPCInfo()
		{

		}
		~FNPCInfo()
		{

		}
		TestNPCInfo		m_NPCInfo;
	};
	TEST_FIXTURE(FNPCInfo, TestNPCInfo_ShowIndicator_ValueFalse)
	{
		m_NPCInfo.m_bShowIndicator = false;
		CHECK_EQUAL(false, m_NPCInfo.IsShowIndicator());
	}

	TEST_FIXTURE(FNPCInfo, TestNPCInfo_ShowIndicator_ValueTrue)
	{
		m_NPCInfo.m_bShowIndicator = true;

		// m_bShowIndicator�� true�̾ ���� Ÿ�Ը� IsShowIndicator()�� True�� ��ȯ�Ѵ�.
		m_NPCInfo.nNpcType = NPC_TYPE_MONSTER;
		CHECK_EQUAL(true, m_NPCInfo.IsShowIndicator());

		m_NPCInfo.nNpcType = NPC_TYPE_NPC;
		CHECK_EQUAL(false, m_NPCInfo.IsShowIndicator());

		m_NPCInfo.nNpcType = NPC_TYPE_OBJECT;
		CHECK_EQUAL(false, m_NPCInfo.IsShowIndicator());

		m_NPCInfo.nNpcType = NPC_TYPE_GATHER;
		CHECK_EQUAL(false, m_NPCInfo.IsShowIndicator());

	}

}