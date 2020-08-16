#include "stdafx.h"
#include "ZSmartFieldAllocator.h"
#include "ZFixtureHelper.h"
#include "ZGameServerObject.h"
#include "ZSharedField.h"
#include "ZDynamicFieldMaster.h"
#include "ZFitnessCalculator.h"
#include "ZSharedField.h"
#include "ZSharedFieldMaster.h"


SUITE(SmartFieldAllocator)
{
	struct FSmartFieldAllocator : FBaseServerField, FBasePlayer
	{
		FSmartFieldAllocator()
		{
			m_nTestFieldID = 110;

			InitGameServer();
			InitTestPlayer();
			InitFieldGroupInfo();
		}
		virtual ~FSmartFieldAllocator()
		{			
		}

		void InitGameServer()
		{
			m_nTestServerID = 1;
			InitDefaultGameServer(m_nTestServerID, m_nTestFieldID);
			m_pSharedFieldMaster = gmgr.pFieldManager->FindSharedFieldMaster(m_nTestFieldID);
		}
		void InitTestPlayer()
		{
			m_pTestPlayer = AddNewPlayerInWorld(m_nTestServerID);
		}
		void InitFieldGroupInfo()
		{
			m_nTestFieldGroupID = 1;
			AddFieldGroupInfo(m_nTestFieldGroupID, m_nTestFieldID);
		}

		void AddDummyPlayer(ZGameServerObject* pServer, int nCount)
		{
			for (int i = 0; i < nCount; ++i)
				pServer->GetPlayerMap().GetPlayerMap().insert(ZPlayerAIDMap::value_type(ZTestHelper::NewAID(), NULL));
		}

		int m_nTestServerID;
		int m_nTestFieldID;
		int m_nTestFieldGroupID;
		ZPlayer* m_pTestPlayer;
		ZSharedFieldMaster* m_pSharedFieldMaster;
	};

	TEST_FIXTURE(FSmartFieldAllocator, TestSmartFieldAllocator_AllocNewChannel1)
	{
		/// Fixture ��ġ
		CHECK_EQUAL(1, m_pSharedFieldMaster->GetChannelCount());
		ZSharedField* pSharedField2 = m_pSharedFieldMaster->FindChannel(2);
		CHECK_EQUAL((ZSharedField*)NULL, pSharedField2);
		ZSmartFieldAllocator fieldAllocator(gmgr.pFieldManager);

		ZGameServerObject* pGameServerObject = gmgr.pGameServerObjectManager->FindGameServer(m_nTestServerID);
		CHECK_EQUAL(1, pGameServerObject->GetFieldMap().GetSharedFieldCount());

		/// SUT ����
		fieldAllocator.AllocNewChannel(m_pSharedFieldMaster);

		/// ����
		CHECK_EQUAL(2, m_pSharedFieldMaster->GetChannelCount());
		pSharedField2 = m_pSharedFieldMaster->FindChannel(2);
		CHECK(NULL != pSharedField2);
		CHECK_EQUAL(2, pGameServerObject->GetFieldMap().GetSharedFieldCount());
	}

	TEST_FIXTURE(FSmartFieldAllocator, TestSmartFieldAllocator_AllocNewChannel2)
	{
		/// Fixture ��ġ
		int nChannelID3 = 3;
		AddSharedField(m_pSharedFieldMaster->GetFieldID(), nChannelID3, m_nTestServerID);
		CHECK_EQUAL(2, m_pSharedFieldMaster->GetChannelCount());			///< 1, 3 ä���� ������.
		ZSharedField* pSharedField2 = m_pSharedFieldMaster->FindChannel(2);	///< 2 ä���� ����.
		CHECK_EQUAL((ZSharedField*)NULL, pSharedField2);
		ZSmartFieldAllocator fieldAllocator(gmgr.pFieldManager);

		/// SUT ����
		fieldAllocator.AllocNewChannel(m_pSharedFieldMaster);

		/// ���� : 2 ä���� ������
		CHECK_EQUAL(3, m_pSharedFieldMaster->GetChannelCount());
		pSharedField2 = m_pSharedFieldMaster->FindChannel(2);
		CHECK(NULL != pSharedField2);
	}

	TEST_FIXTURE(FSmartFieldAllocator, TestSmartFieldAllocator_AllocNewDynamicFieldGroup)
	{
		/// Fixture ��ġ
		MUID uidNewFieldGroup;
		ZSmartFieldAllocator fieldAllocator(gmgr.pFieldManager);

		/// SUT ����
		ZGameServerObject* pTargetServerObject = fieldAllocator.AllocNewDynamicFieldGroup(m_pTestPlayer->GetUID(), m_nTestFieldGroupID, DFT_TRIAL, &uidNewFieldGroup);

		/// ����
		CHECK_EQUAL(1, pTargetServerObject->GetFieldMap().GetDynamicFieldMasterCount());

		ZDynamicFieldMaster* pFieldGroup = pTargetServerObject->GetFieldMap().FindDynamicFieldMaster(uidNewFieldGroup);
		CHECK_EQUAL(uidNewFieldGroup, pFieldGroup->GetUID());
		CHECK_EQUAL(pFieldGroup->GetGameServerID(), pTargetServerObject->GetID());
	}

	TEST_FIXTURE(FSmartFieldAllocator, TestSmartFieldAllocator_AllocNewDynamicFieldGroup_AllocToRequesterServer)
	{
		/// Fixture ��ġ
		CHECK_EQUAL(m_nTestServerID, m_pTestPlayer->GetGameServerID());

		ZGameServerObject* pServer1 = gmgr.pGameServerObjectManager->FindGameServer(m_pTestPlayer->GetGameServerID());
		MUID uidNewFieldGroup;
		ZSmartFieldAllocator fieldAllocator(gmgr.pFieldManager);

		/// SUT ����
		ZGameServerObject* pTargetServerObject = fieldAllocator.AllocNewDynamicFieldGroup(m_pTestPlayer->GetUID(), m_nTestFieldGroupID, DFT_TRIAL, &uidNewFieldGroup);

		/// ����
		CHECK_EQUAL(pServer1->GetID(), pTargetServerObject->GetID());
	}

	TEST_FIXTURE(FSmartFieldAllocator, TestSmartFieldAllocator_AllocNewDynamicFieldGroup_AllocToAnotherServer)
	{
		/// Fixture ��ġ
		int nTestServerID2 = 2;
		int nTestChannelID2 = 2;	///< TestField �� 2�� ä���� 2�� ������ ����
		ZGameServerInfo* pGameServerInfo2 = AddGameServerInfo(nTestServerID2, m_nTestFieldID, 1, nTestChannelID2);
		AddGameServerObject(pGameServerInfo2);

		CHECK_EQUAL(m_nTestServerID, m_pTestPlayer->GetGameServerID());

		ZGameServerObject* pServer1 = gmgr.pGameServerObjectManager->FindGameServer(m_pTestPlayer->GetGameServerID());
		ZGameServerObject* pServer2 = gmgr.pGameServerObjectManager->FindGameServer(nTestServerID2);

		AddDummyPlayer(pServer1, 500);	///< 1�� �ʵ忡 ���� �÷��̾ 500�� ä���. (Fitness ���� ����)

		MUID uidNewFieldGroup;
		ZSmartFieldAllocator fieldAllocator(gmgr.pFieldManager);

		/// SUT ����
		ZGameServerObject* pTargetServerObject = fieldAllocator.AllocNewDynamicFieldGroup(m_pTestPlayer->GetUID(), m_nTestFieldGroupID, DFT_TRIAL, &uidNewFieldGroup);

		/// ����
		CHECK(pServer1->GetID() != pTargetServerObject->GetID());
		CHECK_EQUAL(pServer2->GetID(), pTargetServerObject->GetID());
	}

}