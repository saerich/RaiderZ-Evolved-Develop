#include "stdafx.h"
#include "ZFieldManager.h"
#include "ZSharedField.h"
#include "ZDynamicField.h"
#include "ZDynamicFieldMaster.h"
#include "ZFieldInfoManager.h"
#include "ZGameServerFieldMap.h"
#include "SChecksum.h"
#include "ZFixtureHelper.h"
#include "ZCommandTable.h"
#include "ZPlayerLogic.h"
#include "STransData_M2G.h"
#include "ZGameServerObject.h"
#include "ZParty.h"
#include "ZTestHelper_CmdHandler.h"


SUITE(GateAskFieldGroup)
{
	struct Fixture : public FBaseMockLink2, FBasePlayer, FBaseParty
	{
		int			m_nTestServerID1;
		int			m_nTestServerID2;
		
		ZPlayer*	m_pTestPlayer;
		ZPlayer*	m_pTestPlayer2;

		int			m_nTestServerFieldID;
		MUID		m_uidGameServer1;
		MUID		m_uidGameServer2;
		MockLink*	m_pGameServerLink1;
		int			m_nFieldGroupID;
		int			m_nDynamicFieldID;

		int			m_nDummyCIDGenerator;

		Fixture()
		{
			InitTestGameServer1();
			InitTestGameServer2();
			InitTestPlayer();
			AddFiledGroupInfo();

			m_nDummyCIDGenerator = 1;
		}
		~Fixture()
		{
		}

		void InitTestGameServer1()
		{
			m_nTestServerFieldID = 100;
			AddFieldInfo(m_nTestServerFieldID, L"FieldOfGameServer", L"", false, 2);

			m_nTestServerID1 = 1;
			m_uidGameServer1 = AddGameServerObject(m_nTestServerID1, m_nTestServerFieldID)->GetUID();
			m_pGameServerLink1 = ZTestHelper::NewLink(m_uidGameServer1);
			m_pGameServerLink1->AddIgnoreID(MMC_FIELD_UPDATE_PROXY_FIELD_INFO);
		}
		void InitTestGameServer2()
		{
			m_nTestServerID2 = 2;
			ZGameServerInfo* pGameServerInfo2 = AddGameServerInfo(m_nTestServerID2, m_nTestServerFieldID, 1, 2);
			m_uidGameServer2 = AddGameServerObject(pGameServerInfo2)->GetUID();
		}
		void InitTestPlayer()
		{
			m_pTestPlayer = AddNewPlayerInWorld(m_nTestServerID1);
			m_pTestPlayer2 = AddNewPlayerInWorld(m_nTestServerID1);
		}
		void AddFiledGroupInfo()
		{
			m_nDynamicFieldID = 1001;

			ZFieldInfo* pFieldInfo = new ZFieldInfo();
			pFieldInfo->m_nFieldID = m_nDynamicFieldID;

			gmgr.pFieldInfoManager->Insert(pFieldInfo);


			m_nFieldGroupID = 1000;

			FIELD_GROUP_INFO* pFieldGroupInfo = new FIELD_GROUP_INFO();
			pFieldGroupInfo->nID = m_nFieldGroupID;
			pFieldGroupInfo->pStartFieldInfo = gmgr.pFieldInfoManager->Find(m_nDynamicFieldID);
			pFieldGroupInfo->vecFieldInfo.push_back(gmgr.pFieldInfoManager->Find(m_nDynamicFieldID));

			gmgr.pFieldInfoManager->InsertFieldGroup(pFieldGroupInfo);
		}
		MUID AddFieldGroup(int nGameServerID, int nFieldGroupID, DYNAMIC_FIELD_TYPE eType, MUID uidCreator)
		{
			ZDynamicFieldMaster* pFieldGroup = gmgr.pFieldManager->CreateDynamicFieldMaster(nFieldGroupID, nGameServerID, eType, uidCreator);
			ZGameServerObject* pServer = gmgr.pGameServerObjectManager->FindGameServer(nGameServerID);
			pServer->GetFieldMap().AddDynamicFieldMaster(pFieldGroup);

			return pFieldGroup->GetUID();
		}
		

		void AddDummyPlayer(ZGameServerObject* pServer, int nCount)
		{
			for (int i = 0; i < nCount; ++i)
				pServer->GetPlayerMap().GetPlayerMap().insert(ZPlayerAIDMap::value_type(NewDummyAID(), NULL));
		}
		int NewDummyAID()
		{
			return m_nDummyCIDGenerator++;
		}
	};
	
	// �ڽſ��� �ʵ� ������ �Ǿ��ٸ�, Gate ���� Ŀ��带 ��������Ѵ�.
	TEST_FIXTURE(Fixture, TestCommandHandler_GateAskDynamicField_Response_EXECUTE)
	{
		TD_GATE_FIELD_GROUP_INFO GateFieldGroupInfo = ZTestHelper::MakeGateFieldGroupInfo(m_nFieldGroupID, m_nDynamicFieldID, DFT_TRIAL);
		MCommandResult nResult = ZTestHelper_CmdHandler::OnRecv_MMC_GATE_ASK_DYNAMIC_FIELD(m_pGameServerLink1, m_pTestPlayer->GetUID(), GateFieldGroupInfo);

		CHECK_EQUAL(nResult, CR_TRUE);

		ASSERT_EQUAL(MMC_FIELD_ADD_DYNAMIC_FIELD_GROUP,	m_pGameServerLink1->GetCommandID(0));
		ASSERT_EQUAL(MMC_GATE_RES,					m_pGameServerLink1->GetCommandID(1));
		CHECK_EQUAL(m_pTestPlayer->GetUID(),			m_pGameServerLink1->GetParam<MUID>(1, 0));
		CHECK_EQUAL(RGT_EXECUTE,						m_pGameServerLink1->GetParam<int>(1, 1));		

		TD_GATE_INFO* pInfo;
		int nCount;
		ASSERT_EQUAL(m_pGameServerLink1->GetParam<MBlob*>(1, 2)->GetBlobCount(), 1);
		ASSERT_CHECK(m_pGameServerLink1->GetCommand(1).GetBlob(pInfo, nCount, 2));
		CHECK(pInfo->uidDynamicFieldGroup != MUID::Invalid());
		CHECK_EQUAL(pInfo->nChannelID, 0);
		CHECK_EQUAL(pInfo->bDynamicField, true);
	}
	
	TEST_FIXTURE(Fixture, TestCommandHandler_GateAskDynamicField_InvalidFieldGroupID)
	{
		int nInvalidFieldGroupID = 300;
		TD_GATE_FIELD_GROUP_INFO GateFieldGroupInfo = ZTestHelper::MakeGateFieldGroupInfo(nInvalidFieldGroupID, m_nDynamicFieldID, DFT_TRIAL);
		MCommandResult nResult = ZTestHelper_CmdHandler::OnRecv_MMC_GATE_ASK_DYNAMIC_FIELD(m_pGameServerLink1, m_pTestPlayer->GetUID(), GateFieldGroupInfo);

		CHECK_EQUAL(nResult, CR_TRUE);
		ASSERT_EQUAL(m_pGameServerLink1->GetCommandID(0), MMC_GATE_RES);
		CHECK_EQUAL(m_pGameServerLink1->GetParam<MUID>(0, 0), m_pTestPlayer->GetUID());
		CHECK_EQUAL(m_pGameServerLink1->GetParam<int>(0, 1), RGT_REMAIN);

		ASSERT_EQUAL(m_pGameServerLink1->GetParam<MBlob*>(0, 2)->GetBlobCount(), 0);
	}

	TEST_FIXTURE(Fixture, TestCommandHandler_GateAskDynamicField_InvalidFieldGroupUID)
	{
		MUID uidInvalidFieldGroupUID = 300;
		TD_GATE_FIELD_GROUP_INFO GateFieldGroupInfo = ZTestHelper::MakeGateFieldGroupInfo(m_nFieldGroupID, m_nDynamicFieldID, DFT_TRIAL);
		GateFieldGroupInfo.GateInfo.uidDynamicFieldGroup = uidInvalidFieldGroupUID;
		MCommandResult nResult = ZTestHelper_CmdHandler::OnRecv_MMC_GATE_ASK_DYNAMIC_FIELD(m_pGameServerLink1, m_pTestPlayer->GetUID(), GateFieldGroupInfo);

		CHECK_EQUAL(nResult, CR_TRUE);
		ASSERT_EQUAL(m_pGameServerLink1->GetCommandID(0), MMC_GATE_RES);
		CHECK_EQUAL(m_pGameServerLink1->GetParam<MUID>(0, 0), m_pTestPlayer->GetUID());
		CHECK_EQUAL(m_pGameServerLink1->GetParam<int>(0, 1), RGT_REMAIN);
	}
	
	// �ٸ� ���Ӽ����� ���ο� �����ʵ尡 ������
	TEST_FIXTURE(Fixture, TestCommandHandler_GateAskDynamicField_MoveServer)
	{
		/// Fixture ��ġ
		ZGameServerObject* pTestServer1 = gmgr.pGameServerObjectManager->FindGameServer(m_nTestServerID1);
		AddDummyPlayer(pTestServer1, 500); ///< �ٸ� ������ �����ʵ尡 �����ǰ� �ϱ� ���� 1�� ������ ���� �÷��̾ 500 �� ����

		/// SUT ����
		TD_GATE_FIELD_GROUP_INFO GateFieldGroupInfo = ZTestHelper::MakeGateFieldGroupInfo(m_nFieldGroupID, m_nDynamicFieldID, DFT_TRIAL);
		MCommandResult nResult = ZTestHelper_CmdHandler::OnRecv_MMC_GATE_ASK_DYNAMIC_FIELD(m_pGameServerLink1, m_pTestPlayer->GetUID(), GateFieldGroupInfo);

		/// ����
		ZGameServerObject* pTestServer2 = gmgr.pGameServerObjectManager->FindGameServer(m_nTestServerID2);
		ZDynamicFieldMaster* pFieldGroup = gmgr.pFieldManager->GetDynamicFieldMasterMap().begin()->second;

		CHECK_EQUAL(nResult, CR_TRUE);
		ASSERT_EQUAL(MMC_FIELD_ADD_DYNAMIC_FIELD_GROUP, m_pGameServerLink1->GetCommandID(0));

		ASSERT_EQUAL(MMC_GATE_MOVE_SERVER, m_pGameServerLink1->GetCommandID(1));
		CHECK_EQUAL(m_pGameServerLink1->GetParam<MUID>(1, 0), m_pTestPlayer->GetUID());
		CHECK_EQUAL(m_pGameServerLink1->GetParam<int>(1, 1), pTestServer2->GetID());
		TD_GATE_INFO* pGateInfo;
		CHECK(m_pGameServerLink1->GetCommand(1).GetSingleBlob(pGateInfo, 2));
		CHECK_EQUAL(pFieldGroup->GetUID(), pGateInfo->uidDynamicFieldGroup);
	}

	// �÷��̾ ���� ������ �̹� �������ִ� �����ʵ���� �̵�
	TEST_FIXTURE(Fixture, TestCommandHandler_GateAskDynamicField_ExistFieldGroup_RequesterServer)
	{
		/// Fixture ��ġ
		MUID uidFieldGroup = AddFieldGroup(m_nTestServerID1, m_nFieldGroupID, DFT_TRIAL, m_pTestPlayer->GetUID());

		TD_GATE_FIELD_GROUP_INFO GateFieldGroupInfo = ZTestHelper::MakeGateFieldGroupInfo(m_nFieldGroupID, m_nDynamicFieldID, DFT_TRIAL);
		GateFieldGroupInfo.GateInfo.uidDynamicFieldGroup = uidFieldGroup; ///< �̹� ������ �����ʵ� UID �� ����

		/// SUT ����
		MCommandResult nResult = ZTestHelper_CmdHandler::OnRecv_MMC_GATE_ASK_DYNAMIC_FIELD(m_pGameServerLink1, m_pTestPlayer->GetUID(), GateFieldGroupInfo);
		
		/// ����
		CHECK_EQUAL(nResult, CR_TRUE);
		ASSERT_EQUAL(m_pGameServerLink1->GetCommandID(0), MMC_GATE_RES);
		CHECK_EQUAL(m_pGameServerLink1->GetParam<MUID>(0, 0), m_pTestPlayer->GetUID());
		CHECK_EQUAL(m_pGameServerLink1->GetParam<int>(0, 1), RGT_EXECUTE);

		TD_GATE_INFO* pInfo;
		int nCount;
		ASSERT_EQUAL(m_pGameServerLink1->GetParam<MBlob*>(0, 2)->GetBlobCount(), 1);
		ASSERT_CHECK(m_pGameServerLink1->GetCommand(0).GetBlob(pInfo, nCount, 2));
		CHECK_EQUAL(pInfo->uidDynamicFieldGroup, uidFieldGroup);
		CHECK_EQUAL(pInfo->nChannelID, 0);
		CHECK_EQUAL(pInfo->bDynamicField, true);
	}

	// �÷��̾ ������ ���� ������ �̹� �������ִ� �����ʵ���� �̵�
	TEST_FIXTURE(Fixture, TestCommandHandler_GateAskDynamicField_ExistFieldGroup_AnotherServer)
	{
		MUID uidFieldGroup = AddFieldGroup(m_nTestServerID2, m_nFieldGroupID, DFT_TRIAL, m_pTestPlayer->GetUID());

		TD_GATE_FIELD_GROUP_INFO GateFieldGroupInfo = ZTestHelper::MakeGateFieldGroupInfo(m_nFieldGroupID, m_nDynamicFieldID, DFT_TRIAL);
		GateFieldGroupInfo.GateInfo.uidDynamicFieldGroup = uidFieldGroup; ///< �̹� ������ �����ʵ� UID �� ����
		MCommandResult nResult = ZTestHelper_CmdHandler::OnRecv_MMC_GATE_ASK_DYNAMIC_FIELD(m_pGameServerLink1, m_pTestPlayer->GetUID(), GateFieldGroupInfo);

		CHECK_EQUAL(nResult, CR_TRUE);
		ASSERT_EQUAL(MMC_GATE_MOVE_SERVER, m_pGameServerLink1->GetCommandID(0));
		CHECK_EQUAL(m_pGameServerLink1->GetParam<MUID>(0, 0), m_pTestPlayer->GetUID());
		CHECK_EQUAL(m_pGameServerLink1->GetParam<int>(0, 1), m_nTestServerID2);
		TD_GATE_INFO* pGateInfo;
		CHECK(m_pGameServerLink1->GetCommand(0).GetSingleBlob(pGateInfo, 2));
		CHECK_EQUAL(uidFieldGroup, pGateInfo->uidDynamicFieldGroup);
	}

	// �÷��̾ ���� ������ ���� ��Ƽ�� �����ص� �����ʵ���� �̵�
	TEST_FIXTURE(Fixture, TestCommandHandler_GateAskDynamicField_Exist_PartyFieldGroup)
	{
		// 1�� �÷��̾ ��Ƽ������ ��Ƽ ����
		ZParty* pParty = gmgr.pPartyManager->AddParty(ZTestHelper::NewUID(), m_pTestPlayer->GetUID(), m_pTestPlayer->GetPlayerName(), m_pTestPlayer->GetGameServerID(), m_pTestPlayer->GetCID());
		m_pTestPlayer->SetPartyUID(pParty->GetUID());

		// 1�� �÷��̾ �����ʵ� ����
		TD_GATE_FIELD_GROUP_INFO GateFieldGroupInfo = ZTestHelper::MakeGateFieldGroupInfo(m_nFieldGroupID, m_nDynamicFieldID, DFT_TRIAL);
		MCommandResult nResult = ZTestHelper_CmdHandler::OnRecv_MMC_GATE_ASK_DYNAMIC_FIELD(m_pGameServerLink1, m_pTestPlayer->GetUID(), GateFieldGroupInfo);
		m_pGameServerLink1->ResetCommands();

		// 1�� �÷��̾��� ��Ƽ�� 2�� �÷��̾ ����� �߰�
		pParty->AddMember(m_pTestPlayer2->GetUID(), m_pTestPlayer2->GetPlayerName(), m_pTestPlayer2->GetGameServerID(), m_pTestPlayer2->GetCID());
		m_pTestPlayer2->SetPartyUID(pParty->GetUID());

		// 2�� �÷��̾� �����ʵ� ����
		nResult = ZTestHelper_CmdHandler::OnRecv_MMC_GATE_ASK_DYNAMIC_FIELD(m_pGameServerLink1, m_pTestPlayer2->GetUID(), GateFieldGroupInfo);

		CHECK_EQUAL(nResult, CR_TRUE);
		ASSERT_EQUAL(m_pGameServerLink1->GetCommandID(0), MMC_GATE_RES);
		CHECK_EQUAL(m_pGameServerLink1->GetParam<MUID>(0, 0), m_pTestPlayer2->GetUID());
		CHECK_EQUAL(m_pGameServerLink1->GetParam<int>(0, 1), RGT_EXECUTE);

		TD_GATE_INFO* pInfo;
		int nCount;
		ASSERT_EQUAL(m_pGameServerLink1->GetParam<MBlob*>(0, 2)->GetBlobCount(), 1);
		ASSERT_CHECK(m_pGameServerLink1->GetCommand(0).GetBlob(pInfo, nCount, 2));
		CHECK_EQUAL(pInfo->uidDynamicFieldGroup, pParty->GetFieldGroupUID(m_nFieldGroupID));
		CHECK_EQUAL(pInfo->nChannelID, 0);
		CHECK_EQUAL(pInfo->bDynamicField, true);
	}

	// �÷��̾ ���� ������ ���� ��Ƽ�� �����ص� �����ʵ���� �̵� ��û - Invalid FieldGrouopUID
	TEST_FIXTURE(Fixture, TestCommandHandler_GateAskDynamicField_Exist_PartyFieldGroup_InvalidFIeldGroupUID)
	{
		// 1�� �÷��̾ ��Ƽ������ ��Ƽ ����
		ZParty* pParty = gmgr.pPartyManager->AddParty(ZTestHelper::NewUID(), m_pTestPlayer->GetUID(), m_pTestPlayer->GetPlayerName(), m_pTestPlayer->GetGameServerID(), m_pTestPlayer->GetCID());
		m_pTestPlayer->SetPartyUID(pParty->GetUID());

		// 1�� �÷��̾ �����ʵ� ����
		TD_GATE_FIELD_GROUP_INFO GateFieldGroupInfo = ZTestHelper::MakeGateFieldGroupInfo(m_nFieldGroupID, m_nDynamicFieldID, DFT_TRIAL);
		MCommandResult nResult = ZTestHelper_CmdHandler::OnRecv_MMC_GATE_ASK_DYNAMIC_FIELD(m_pGameServerLink1, m_pTestPlayer->GetUID(), GateFieldGroupInfo);
		m_pGameServerLink1->ResetCommands();

		// 1�� �÷��̾��� ��Ƽ�� 2�� �÷��̾ ����� �߰�
		pParty->AddMember(m_pTestPlayer2->GetUID(), m_pTestPlayer2->GetPlayerName(), m_pTestPlayer2->GetGameServerID(), m_pTestPlayer2->GetCID());
		m_pTestPlayer2->SetPartyUID(pParty->GetUID());

		// 2�� �÷��̾� �����ʵ� ����
		MUID uidInvalidFieldGroupUID = 300;
		GateFieldGroupInfo.GateInfo.uidDynamicFieldGroup = uidInvalidFieldGroupUID;
		nResult = ZTestHelper_CmdHandler::OnRecv_MMC_GATE_ASK_DYNAMIC_FIELD(m_pGameServerLink1, m_pTestPlayer2->GetUID(), GateFieldGroupInfo);

		CHECK_EQUAL(nResult, CR_TRUE);
		ASSERT_EQUAL(m_pGameServerLink1->GetCommandID(0), MMC_GATE_RES);
		CHECK_EQUAL(m_pGameServerLink1->GetParam<MUID>(0, 0), m_pTestPlayer2->GetUID());
		CHECK_EQUAL(m_pGameServerLink1->GetParam<int>(0, 1), RGT_EXECUTE);

		TD_GATE_INFO* pInfo;
		int nCount;
		ASSERT_EQUAL(m_pGameServerLink1->GetParam<MBlob*>(0, 2)->GetBlobCount(), 1);
		ASSERT_CHECK(m_pGameServerLink1->GetCommand(0).GetBlob(pInfo, nCount, 2));
		CHECK_EQUAL(pInfo->uidDynamicFieldGroup, pParty->GetFieldGroupUID(m_nFieldGroupID));
		CHECK_EQUAL(pInfo->nChannelID, 0);
		CHECK_EQUAL(pInfo->bDynamicField, true);
	}

}