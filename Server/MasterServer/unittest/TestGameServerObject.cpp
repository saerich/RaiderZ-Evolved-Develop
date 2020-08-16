#include "stdafx.h"
#include "ZGameServerObject.h"
#include "ZFieldManager.h"
#include "ZFixtureHelper.h"
#include "STransData.h"
#include "ZSharedField.h"


SUITE(GameServerObject)
{
	struct FGameServerObject : public FBaseFieldInfo, public FBaseGameServer
	{
		FGameServerObject() : m_nGameServerID(1), m_GameServerUID(0, 1)
		{
			SetupFieldInfo();
			SetupServerInfo();


			m_pGameServerObject = new ZGameServerObject(m_GameServerUID, m_pServerInfo);
		}
		~FGameServerObject()
		{
			SAFE_DELETE(m_pGameServerObject);
		}

		void SetupFieldInfo()
		{
			MockFieldInfoManager* pFieldInfoManager = dynamic_cast<MockFieldInfoManager*>(gmgr.pFieldInfoManager);

			pFieldInfoManager->AddFieldInfo(107, L"FieldName1", L"", false, 4);
			pFieldInfoManager->AddFieldInfo(110, L"FieldName2", L"", false, 4);
			pFieldInfoManager->AddFieldInfo(111, L"FieldName3", L"", false, 4);

		}

		void SetupServerInfo()
		{
			// ���� ���� ���� ----------
			// 107 �ʵ� - ä��(1, 2)
			// 110 �ʵ� - ä��(3)
			// 111 �ʵ� - ä��(1)

			ZGameServerInfo* pInfo = AddGameServerInfo(m_nGameServerID, 107, 2, 1, 2);
			AddGameServerInfo(m_nGameServerID, 110, 1, 3);
			AddGameServerInfo(m_nGameServerID, 111, 1, 1);

			m_pServerInfo = pInfo;
		}

		// const
		MUID					m_GameServerUID;
		int						m_nGameServerID;


		ZGameServerInfo*		m_pServerInfo;
		ZGameServerObject*		m_pGameServerObject;

		ZTestMgrWrapper<ZFieldManager>	m_FieldManagerWrapper;		
	};

	TEST_FIXTURE(FGameServerObject, TestGameServerObject_Init)
	{
		bool bInit = m_pGameServerObject->Init(gmgr.pFieldManager);

		// return �� üũ
		CHECK_EQUAL(true, bInit);
		CHECK_EQUAL(1, m_pGameServerObject->GetID());


		// �Ҵ�� �ʵ尡 ����� �����Ǿ��� üũ
		CHECK_EQUAL(4, gmgr.pFieldManager->GetSharedFieldCount());
		CHECK_EQUAL(4, m_pGameServerObject->GetFieldMap().GetSharedFieldCount());

		// ���Ӽ��� ������Ʈ�� ���ִ� ������ �ʵ� �ν��Ͻ� Ȯ��
		{
			ZSharedField* pField = m_pGameServerObject->GetFieldMap().FindSharedField(107, 1);
			CHECK_EQUAL(m_nGameServerID, pField->GetGameServerID());
			CHECK_EQUAL(107, pField->GetFieldInfo()->m_nFieldID);
			CHECK_EQUAL(FT_SHARED, pField->GetType());
			CHECK_EQUAL(1, pField->GetChannelID());
		}

		{
			ZSharedField* pField = m_pGameServerObject->GetFieldMap().FindSharedField(107, 2);
			CHECK_EQUAL(m_nGameServerID, pField->GetGameServerID());
			CHECK_EQUAL(107, pField->GetFieldInfo()->m_nFieldID);
			CHECK_EQUAL(FT_SHARED, pField->GetType());
			CHECK_EQUAL(2, pField->GetChannelID());
		}

		{
			ZSharedField* pField = m_pGameServerObject->GetFieldMap().FindSharedField(110, 3);
			CHECK_EQUAL(m_nGameServerID, pField->GetGameServerID());
			CHECK_EQUAL(110, pField->GetFieldInfo()->m_nFieldID);
			CHECK_EQUAL(FT_SHARED, pField->GetType());
			CHECK_EQUAL(3, pField->GetChannelID());
		}
		{
			ZSharedField* pField = m_pGameServerObject->GetFieldMap().FindSharedField(111, 1);
			CHECK_EQUAL(m_nGameServerID, pField->GetGameServerID());
			CHECK_EQUAL(111, pField->GetFieldInfo()->m_nFieldID);
			CHECK_EQUAL(FT_SHARED, pField->GetType());
			CHECK_EQUAL(1, pField->GetChannelID());
		}

		// ���� ó��
		{
			ZSharedField* pField = m_pGameServerObject->GetFieldMap().FindSharedField(107, 5);
			CHECK(pField == NULL);
		}
		{
			ZSharedField* pField = m_pGameServerObject->GetFieldMap().FindSharedField(110, 1);
			CHECK(pField == NULL);
		}

	}

	TEST_FIXTURE(FGameServerObject, TestGameServerObject_AddObject)
	{
		ZGameServerObjectManager serverObjectManager;
		ZGameServerObject* pGameServerObject = new ZGameServerObject(m_GameServerUID, m_pServerInfo);

		CHECK_EQUAL(0, serverObjectManager.GetClientsCount());
		CHECK_EQUAL(0, serverObjectManager.GetObjectIDCount());

		serverObjectManager.AddObject(pGameServerObject);

		CHECK_EQUAL(1, serverObjectManager.GetClientsCount());
		CHECK_EQUAL(1, serverObjectManager.GetObjectIDCount());
	}

	TEST_FIXTURE(FGameServerObject, TestGameServerObject_DeleteObject)
	{
		ZGameServerObjectManager serverObjectManager;
		ZGameServerObject* pGameServerObject = new ZGameServerObject(m_GameServerUID, m_pServerInfo);

		serverObjectManager.AddObject(pGameServerObject);

		CHECK_EQUAL(1, serverObjectManager.GetClientsCount());
		CHECK_EQUAL(1, serverObjectManager.GetObjectIDCount());

		serverObjectManager.DeleteObject(pGameServerObject->GetUID());

		CHECK_EQUAL(0, serverObjectManager.GetClientsCount());
		CHECK_EQUAL(0, serverObjectManager.GetObjectIDCount());
	}

	TEST_FIXTURE(FGameServerObject, TestGameServerObject_DeleteAllObject)
	{
		ZGameServerObjectManager serverObjectManager;
		ZGameServerObject* pGameServerObject = new ZGameServerObject(m_GameServerUID, m_pServerInfo);

		serverObjectManager.AddObject(pGameServerObject);

		CHECK_EQUAL(1, serverObjectManager.GetClientsCount());
		CHECK_EQUAL(1, serverObjectManager.GetObjectIDCount());

		serverObjectManager.DeleteAllObject();

		CHECK_EQUAL(0, serverObjectManager.GetClientsCount());
		CHECK_EQUAL(0, serverObjectManager.GetObjectIDCount());
	}

	TEST_FIXTURE(FGameServerObject, TestGameServerObject_FindGameServer)
	{
		ZGameServerObjectManager serverObjectManager;
		ZGameServerObject* pGameServerObject = new ZGameServerObject(m_GameServerUID, m_pServerInfo);

		serverObjectManager.AddObject(pGameServerObject);

		ZGameServerObject* pFindedObject = serverObjectManager.FindGameServer(pGameServerObject->GetUID());

		CHECK_EQUAL(pGameServerObject->GetUID(), pFindedObject->GetUID());
		CHECK_EQUAL(pGameServerObject->GetID(), pFindedObject->GetID());
	}

	TEST_FIXTURE(FGameServerObject, TestGameServerObject_FindGameServerFromServerID)
	{
		ZGameServerObjectManager serverObjectManager;
		ZGameServerObject* pGameServerObject = new ZGameServerObject(m_GameServerUID, m_pServerInfo);

		serverObjectManager.AddObject(pGameServerObject);

		ZGameServerObject* pFindedObject = serverObjectManager.FindGameServer(pGameServerObject->GetID());

		CHECK_EQUAL(pGameServerObject->GetUID(), pFindedObject->GetUID());
		CHECK_EQUAL(pGameServerObject->GetID(), pFindedObject->GetID());
	}

}
