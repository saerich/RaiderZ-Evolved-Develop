#include "StdAfx.h"
#include "SUnitTestUtil.h"
#include "LTestHelper_CmdHandler.h"
#include "FBasePlayer.h"
#include "FBaseMasterClient.h"
#include "LPlayerLoginGameServerRetryer.h"
#include "LPlayerLoginGameServerProcManager.h"
#include "LPlayerObject.h"
#include "CCommandTable_Master.h"
#include "CCommandResultTable.h"
#include "LPlayerLogic.h"
#include "LServer.h"
#include "MLocale.h"


SUITE(PlayerLoginGameServerRetryer)
{
	class TestPlayerLoginGameServerRetryer : public LPlayerLoginGameServerRetryer
	{
	public:
		using LPlayerLoginGameServerRetryer::m_listRetryer;
	};

	struct Fixture : public FBasePlayer, FBaseMasterClient
	{
		Fixture()
		{
			m_pPlayerLoginGameServerProcManager = gsys.pServer->GetPlayerLoginGameServerProcManager();

			/// �÷��̾� ����
			m_pPlayerObject = NewTestPlayerObject();

			/// POS_SELECT_CHAR ���·� ����.
			ACCOUNT_INFO accountInfo;
			accountInfo.nAID = SUnitTestUtil::NewID();

			_tcscpy_s(accountInfo.szUserID, MLocale::ConvUTF16ToTCHAR(SUnitTestUtil::NewName().c_str()).c_str());
			m_pPlayerObject->InitAccountInfo(accountInfo);

			m_pNetAgent->ResetSendCommands();
			m_pNetAgent->ResetIgnoreCmd();
			m_pNetAgent->IgnoreCmd(MLC_PLAYER_STATE_SELECT_CHAR);
		}
		~Fixture()
		{
			m_pPlayerLoginGameServerProcManager->Clear();
		}

		LPlayerObject*	m_pPlayerObject;

		LPlayerLoginGameServerProcManager* m_pPlayerLoginGameServerProcManager;
		LTestSysWrapper2<LPlayerLoginGameServerRetryer, TestPlayerLoginGameServerRetryer> m_Retryer;
	};

	TEST_FIXTURE(Fixture, TestPlayerLoginGameServerRetryer_Fixture)
	{
		CHECK_EQUAL(POS_SELECT_CHAR, m_pPlayerObject->GetState());
	}

	TEST_FIXTURE(Fixture, TestPlayerLoginGameServerRetryer_AddRetryer)
	{
		CHECK_EQUAL(true,	m_Retryer.Get()->m_listRetryer.empty());

		/// SUT ����
		m_Retryer.Get()->AddRetryer(m_pPlayerObject->GetUID(), m_pPlayerObject->GetSelectedCharIndex());

		/// ����
		CHECK_EQUAL(false,	m_Retryer.Get()->m_listRetryer.empty());
		CHECK_EQUAL(1,		m_Retryer.Get()->m_listRetryer.size());
	}

	TEST_FIXTURE(Fixture, TestPlayerLoginGameServerRetryer_DelRetryer)
	{
		/// Fixture ��ġ
		m_Retryer.Get()->AddRetryer(m_pPlayerObject->GetUID(), m_pPlayerObject->GetSelectedCharIndex());

		/// SUT ����
		m_Retryer.Get()->DelRetryer(m_pPlayerObject->GetUID());

		/// ����
		CHECK_EQUAL(true,	m_Retryer.Get()->m_listRetryer.empty());
	}

	TEST_FIXTURE(Fixture, TestPlayerLoginGameServerRetryer_Update)
	{
		/// Fixture ��ġ - �ӽ÷� ��ȿ�� ĳ���Ͷ� ����. �����͸� �۾��� �ʿ��ϸ�, �׽�Ʈ �ڵ嵵 ���� �����ؾ� �մϴ�.
		SAccountCharFieldInfo charFieldInfo;
		charFieldInfo.nCID = SUnitTestUtil::NewID();
		m_pPlayerObject->AddCharFieldInfo(charFieldInfo);
		m_Retryer.Get()->AddRetryer(m_pPlayerObject->GetUID(), 0);

		/// SUT ����
		m_Retryer.Get()->Update(LPlayerLoginGameServerRetryer::TIME_TO_UPDATE_SEC);

		/// ����
		CHECK_EQUAL(true,	m_Retryer.Get()->m_listRetryer.empty());
		CHECK_EQUAL(1,		m_pPlayerLoginGameServerProcManager->GetQueueSize());
	}

	TEST_FIXTURE(Fixture, TestPlayerLoginGameServerRetryer_Update_EmptyNodeList)
	{
		CHECK_EQUAL(true,	m_Retryer.Get()->m_listRetryer.empty());

		/// SUT ����
		m_Retryer.Get()->Update(LPlayerLoginGameServerRetryer::TIME_TO_UPDATE_SEC);

		/// ����
		CHECK_EQUAL(0,	m_pNetAgent->GetSendCommandCount());
	}

	TEST_FIXTURE(Fixture, TestPlayerLoginGameServerRetryer_Update_LackTime)
	{
		/// Fixture ��ġ
		m_Retryer.Get()->AddRetryer(m_pPlayerObject->GetUID(), m_pPlayerObject->GetSelectedCharIndex());

		/// SUT ����
		m_Retryer.Get()->Update(0.0f);

		/// ����
		CHECK_EQUAL(1,	m_Retryer.Get()->m_listRetryer.size());
		CHECK_EQUAL(0,	m_pNetAgent->GetSendCommandCount());
	}

	TEST_FIXTURE(Fixture, TestPlayerLoginGameServerRetryer_OnRecv_MLC_COMM_RESPONSE_LOGIN_GAME_SERVER)
	{
		m_pPlayerObject->OnLoginGameServer(0);
		CHECK_EQUAL(true,	m_Retryer.Get()->m_listRetryer.empty());

		/// SUT ����
		LTestHelper_CmdHandler::OnRecv_MLC_COMM_RESPONSE_LOGIN_GAME_SERVER(m_pNetAgent
			, m_pPlayerObject->GetUID()
			, CR_SUCCESS_LOGIN_GAME_SERVER_RETRY
			, NULL);

		/// ����
		CHECK_EQUAL(false,	m_Retryer.Get()->m_listRetryer.empty());
		CHECK_EQUAL(1,		m_Retryer.Get()->m_listRetryer.size());
	}

	TEST_FIXTURE(Fixture, TestPlayerLoginGameServerRetryer_OnRecv_MLC_COMM_RESPONSE_LOGIN_GAME_SERVER_And_Update)
	{
		/// Fixture ��ġ
		SAccountCharFieldInfo charFieldInfo;
		charFieldInfo.nCID = SUnitTestUtil::NewID();
		m_pPlayerObject->AddCharFieldInfo(charFieldInfo);

		LPlayerLogic playerLogic;
		playerLogic.SelMyChar(m_pPlayerObject->GetUID(), 0);		
		m_pPlayerLoginGameServerProcManager->Update(LPlayerLoginGameServerProcManager::TIME_UPDATE_SEC);
		
		LTestHelper_CmdHandler::OnRecv_MLC_COMM_RESPONSE_LOGIN_GAME_SERVER(m_pNetAgent	///< Retryer �� ���
			, m_pPlayerObject->GetUID()
			, CR_SUCCESS_LOGIN_GAME_SERVER_RETRY
			, NULL);

		CHECK_EQUAL(1,	m_Retryer.Get()->m_listRetryer.size());

		/// SUT ����
		m_Retryer.Get()->Update(LPlayerLoginGameServerRetryer::TIME_TO_UPDATE_SEC);

		/// ����
		CHECK_EQUAL(0,	m_Retryer.Get()->m_listRetryer.size());
		CHECK_EQUAL(1,	m_pPlayerLoginGameServerProcManager->GetQueueSize());
	}

}