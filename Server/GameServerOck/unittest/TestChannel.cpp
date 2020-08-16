#include "stdafx.h"
#include "GManagedFieldInfoMgr.h"
#include "GUTHelper.h"
#include "GFieldMgr.h"
#include "MockLink.h"
#include "CCommandTable.h"
#include "GPlayerObjectManager.h"
#include "GTestWrapper.h"
#include "GConst.h"
#include "GSharedFieldMaster.h"
#include "GDynamicFieldMaster.h"
#include "GFieldSystem.h"
#include "GFieldInfoMgr.h"
#include "GTestForward.h"
#include "GDef.h"
#include "GPlayerField.h"
#include "GFieldFactory.h"
#include "GServer.h"
#include "GMasterServerFacade.h"
#include "FBaseFieldInfo.h"
#include "FBaseMockLink.h"
#include "GPlayerFieldDynamic.h"
#include "CCommandResultTable.h"
#include "GConfig.h"
#include "MockFieldEnteringMgr.h"


SUITE(Channel)
{
	class FakeFieldMgr : public GFieldMgr
	{
	public:
		FakeFieldMgr()
		{
			SAFE_DELETE(m_pFieldEnteringQueue);
			m_pFieldEnteringQueue = new MockFieldEnteringMgr();
		}
	};

	struct Fixture : public FBaseFieldInfo, FBaseMockLink
	{
		enum CHANNEL_TYPE
		{
			CHANNEL_A = 0,
			CHANNEL_B = 1,
			CHANNEL_COUNT,
		};

		Fixture()
		{
			InitFieldInfo();
			InitFields();
		}
		virtual ~Fixture()
		{
		}

		void InitFieldInfo()
		{
			m_pFieldInfo = AddFieldInfo(TEST_FIELD_ID);
			m_pFieldInfo->m_nDefaultChannelCount = 0;
			m_pFieldInfo->m_bDynamic = false;
		}
		void InitFields()
		{
			gmgr.pFieldMgr->GetFactory()->CreateSharedFieldMaster(m_pFieldInfo);
			m_pFields[CHANNEL_A] = gmgr.pFieldMgr->GetFactory()->CreateSharedField(gsys.pServer->NewUID(), TEST_FIELD_ID, TEST_FIELD_CHANNEL_ID);
			m_pFields[CHANNEL_B] = gmgr.pFieldMgr->GetFactory()->CreateSharedField(gsys.pServer->NewUID(), TEST_FIELD_ID, TEST_FIELD_CHANNEL_ID+1);
		}
		void AddTestPlayer(CHANNEL_TYPE eChannelType, GEntityPlayer** OUT_pPlayer, MockLink** OUT_pLink)
		{
			AddTestPlayer(m_pFields[eChannelType], OUT_pPlayer, OUT_pLink);
		}
		void AddTestPlayer(GSharedField* pChannel, GEntityPlayer** OUT_pPlayer, MockLink** OUT_pLink)
		{
			CHANNEL_TYPE eChannelType;
			if (pChannel->GetChannelID() == TEST_FIELD_CHANNEL_ID)
				eChannelType = CHANNEL_A;
			else if (pChannel->GetChannelID() == TEST_FIELD_CHANNEL_ID+1)
				eChannelType = CHANNEL_B;

			GEntityPlayer* pPlayer = GUTHelper::NewEntityPlayer(pChannel, vec3(1000,1000,0));
			m_vecTestPlayers[eChannelType].push_back(pPlayer);
			GPlayerObject* pPlayerObject = gmgr.pPlayerObjectManager->GetPlayer(pPlayer->GetUID());
			pPlayerObject->OnStartGame();

			MockLink* pLink = NewLink(pPlayer);
			m_vecTestPlayerLinks[eChannelType].push_back(pLink);

			if (NULL != OUT_pPlayer)	*OUT_pPlayer = pPlayer;
			if (NULL != OUT_pLink)		*OUT_pLink = pLink;
		}

		void FillWithTestPlayersByChannel(GSharedField* pChannel, int nPlayerCountToFill)
		{
			int nCurrentFieldPlyaerCount = pChannel->GetPlayerQty();
			if (nCurrentFieldPlyaerCount >= nPlayerCountToFill) return;

			for (int i = nCurrentFieldPlyaerCount; i < nPlayerCountToFill; ++i)
			{
				GEntityPlayer* pTestPlayer = NULL;
				MockLink* pTestPlayerLink = NULL;
				AddTestPlayer(pChannel, &pTestPlayer, &pTestPlayerLink);
			}

			ResetCommandAllTestPlayerLink();
		}
		void ResetCommandAllTestPlayerLink()
		{
			for (int i = 0; i < (int)m_vecTestPlayerLinks[CHANNEL_A].size(); ++i)
			{
				m_vecTestPlayerLinks[CHANNEL_A][i]->ResetCommands();
			}
			for (int i = 0; i < (int)m_vecTestPlayerLinks[CHANNEL_B].size(); ++i)
			{
				m_vecTestPlayerLinks[CHANNEL_B][i]->ResetCommands();
			}
		}

		typedef vector<GEntityPlayer* > VECTOR_FIELD_TESTPLAYERS;
		typedef vector<MockLink* > VECTOR_FIELD_TESTPLAYER_LINKS;

		static const int TEST_FIELD_ID = 10000;
		static const int TEST_FIELD_CHANNEL_ID = 1;

		GUTHelper						m_Helper;
		GSharedField*					m_pFields[CHANNEL_COUNT];
		VECTOR_FIELD_TESTPLAYERS		m_vecTestPlayers[CHANNEL_COUNT];
		VECTOR_FIELD_TESTPLAYER_LINKS	m_vecTestPlayerLinks[CHANNEL_COUNT];
		GFieldInfo*						m_pFieldInfo;

		GTestMgrWrapper2<GFieldMgr, FakeFieldMgr>	m_FieldMgr;
		GTestMgrWrapper<GPlayerObjectManager>		m_PlayerObjectManager;
		DECLWRAPPER_FakeSystem;
		DECLWRAPPER_NPCMgr;
	};

	TEST(DefaultChannel)
	{
		GTestMgrWrapper<GFieldMgr> wrapperFieldMgr;

		const int TEST_FIELD_ID = 777;

		// �ʵ� ���� �ʱ�ȭ
		GFieldInfo* pFieldInfo = new GFieldInfo();
		pFieldInfo->m_nDefaultChannelCount = 7;	// �⺻ä�� 7��
		pFieldInfo->m_bDynamic = false;
		pFieldInfo->m_nFieldID = TEST_FIELD_ID;
		pFieldInfo->SetFieldMaxXY_ForTest(50000.0f, 50000.0f);
		gmgr.pFieldInfoMgr->Insert(pFieldInfo);


		GManagedFieldInfoMgr* pManagedFieldInfoMgr = gsys.pMasterServerFacade->GetManagedFieldInfoMgr();
		pManagedFieldInfoMgr->AssignAllForStandAlone(gmgr.pFieldInfoMgr);

		gmgr.pFieldMgr->GetFactory()->CreateAllSharedField(gmgr.pFieldInfoMgr, gsys.pTestSystem, gsys.pServer);

		GSharedFieldMaster* pSharedField = gmgr.pFieldMgr->GetSharedFieldMaster(TEST_FIELD_ID);
		CHECK_EQUAL(7, pSharedField->GetChannels().size());
	}

	TEST_FIXTURE(Fixture, CheckFieldIsNotNull)
	{
		CHECK(m_pFields[CHANNEL_A] != NULL);
		CHECK(m_pFields[CHANNEL_B] != NULL);
	}

	TEST_FIXTURE(Fixture, TestChannel_GetEnterableSharedField_AddNewChannel_When_NotExistEnterableChannel_ForStandAlone)
	{
		ASSERT_EQUAL(true, GConfig::m_bStandAlone);

		/// Fixture ��ġ - A ä���� �÷��̾�� �� ä��.
		FillWithTestPlayersByChannel(m_pFields[CHANNEL_A], m_pFields[CHANNEL_A]->GetCapacity());
		m_pFields[CHANNEL_B]->DeleteMe();
		gmgr.pFieldMgr->Update(.0f); ///< B ä���� ����.

		GSharedFieldMaster* pSharedField = m_FieldMgr.Get()->GetSharedFieldMaster(TEST_FIELD_ID);
		int nChannelCount = (int)pSharedField->GetChannels().size();

		/// SUT ����
		gmgr.pFieldMgr->GetEnterableSharedField(TEST_FIELD_ID);

		/// ����
		CHECK_EQUAL(nChannelCount+1, pSharedField->GetChannels().size());
	}

	TEST_FIXTURE(Fixture, TestChannel_PrepareChangeChannel_NotExistChannel)
	{
		/// Fixture ��ġ
		MockLink*		pTestPlayerLink;
		GEntityPlayer* pPlayer;
		AddTestPlayer(CHANNEL_A, &pPlayer, &pTestPlayerLink);

		CHECK_EQUAL(1,	m_pFields[CHANNEL_A]->GetPlayerQty());
		CHECK_EQUAL(0,	m_pFields[CHANNEL_B]->GetPlayerQty());

		const int TEST_NOT_EXIST_CHANNEL_ID = 999;
		GSharedFieldMaster* pSharedField = gmgr.pFieldMgr->GetSharedFieldMaster(TEST_FIELD_ID);
		GSharedField* pField = pSharedField->GetChannel(TEST_NOT_EXIST_CHANNEL_ID);
		CHECK_EQUAL((GSharedField*)NULL,	pField);

		/// SUT ����
		gsys.pFieldSystem->PrepareChangeChannel(pPlayer, TEST_NOT_EXIST_CHANNEL_ID);

		/// ����
		TVALID_EQ(1,				pTestPlayerLink->GetCommandCount());
		ASSERT_EQUAL(MC_FIELD_PREPARE_CHANGE_CHANNEL_RES,	pTestPlayerLink->GetCommand(0).GetID());
		CHECK_EQUAL(CR_FAIL_CHANGE_CHANNEL_IS_INVALID,		pTestPlayerLink->GetParam<int>(0, 0));

		CHECK_EQUAL(1,	m_pFields[CHANNEL_A]->GetPlayerQty());
		CHECK_EQUAL(0,	m_pFields[CHANNEL_B]->GetPlayerQty());
	}

	namespace SimpleTestBad
	{
		struct Fixture_AddedTestPlayers : public Fixture
		{
			Fixture_AddedTestPlayers()
			{
				AddTestPlayer(CHANNEL_A, &m_pChangeChannelPlayer, &m_pChangeChannelPlayerLink);
				AddTestPlayer(CHANNEL_A, &m_pTestPlayers[CHANNEL_A], &m_pTestPlayerLinks[CHANNEL_A]);
				AddTestPlayer(CHANNEL_B, &m_pTestPlayers[CHANNEL_B], &m_pTestPlayerLinks[CHANNEL_B]);

				m_pChangeChannelPlayerLink->ResetCommands();
				m_pTestPlayerLinks[CHANNEL_A]->ResetCommands();
				m_pTestPlayerLinks[CHANNEL_B]->ResetCommands();
			}

			GEntityPlayer*	m_pChangeChannelPlayer;
			MockLink*		m_pChangeChannelPlayerLink;

			GEntityPlayer*	m_pTestPlayers[CHANNEL_COUNT];
			MockLink*		m_pTestPlayerLinks[CHANNEL_COUNT];
		};

		TEST_FIXTURE(Fixture_AddedTestPlayers, ChangeChannel)
		{
			/// Fixture ��ġ
			CHECK_EQUAL(2,	m_pFields[CHANNEL_A]->GetPlayerQty());
			CHECK_EQUAL(1,	m_pFields[CHANNEL_B]->GetPlayerQty());

			m_pChangeChannelPlayerLink->AddIgnoreID(MC_FIELD_PREPARE_CHANGE_CHANNEL_RES);

			/// SUT ���� - TestPlayer, B ä�η� �̵�
			gsys.pFieldSystem->PrepareChangeChannel(m_pChangeChannelPlayer, GetChannelID(m_pFields[CHANNEL_B]));
			gsys.pSystem->Sleep(CHANNEL_CHANGE_PREPARE_TIME);
			m_pChangeChannelPlayerLink->OnRecv(MC_FIELD_REQUEST_CHANGE_CHANNEL, 0, NULL);

			/// ����
			CHECK_EQUAL(1,	m_pFields[CHANNEL_A]->GetPlayerQty());
			CHECK_EQUAL(2,	m_pFields[CHANNEL_B]->GetPlayerQty());

			CHECK_EQUAL(1,						m_pTestPlayerLinks[CHANNEL_A]->GetCommandCount());
			CHECK_EQUAL(MC_FIELD_OUT_PLAYER,	m_pTestPlayerLinks[CHANNEL_A]->GetCommand(0).GetID());
			CHECK_EQUAL(1,						m_pChangeChannelPlayerLink->GetCommandCount());
			CHECK_EQUAL(MC_FIELD_CHANGE_FIELD,	m_pChangeChannelPlayerLink->GetCommand(0).GetID());
		}

		TEST_FIXTURE(Fixture_AddedTestPlayers, MoveChannel_Return)
		{
			/// Fixture ��ġ
			m_pChangeChannelPlayerLink->AddIgnoreID(MC_FIELD_PREPARE_CHANGE_CHANNEL_RES);

			// B ä�η� �̵�
			gsys.pFieldSystem->PrepareChangeChannel(m_pChangeChannelPlayer, GetChannelID(m_pFields[CHANNEL_B]));
			gsys.pSystem->Sleep(CHANNEL_CHANGE_PREPARE_TIME);
			m_pChangeChannelPlayerLink->OnRecv(MC_FIELD_REQUEST_CHANGE_CHANNEL, 0, NULL);
			m_pChangeChannelPlayerLink->OnRecv(MC_FIELD_LOADING_COMPLETE, 0, NULL);

			m_pTestPlayerLinks[CHANNEL_A]->ResetCommands();
			m_pTestPlayerLinks[CHANNEL_B]->ResetCommands();
			m_pChangeChannelPlayerLink->ResetCommands();

			/// SUT ���� - A  ä�η� ��ȯ
			gsys.pFieldSystem->PrepareChangeChannel(m_pChangeChannelPlayer, GetChannelID(m_pFields[CHANNEL_A]));
			gsys.pSystem->Sleep(CHANNEL_CHANGE_PREPARE_TIME);
			m_pChangeChannelPlayerLink->OnRecv(MC_FIELD_REQUEST_CHANGE_CHANNEL, 0, NULL);

			CHECK_EQUAL(1,						m_pTestPlayerLinks[CHANNEL_B]->GetCommandCount());
			CHECK_EQUAL(MC_FIELD_OUT_PLAYER,	m_pTestPlayerLinks[CHANNEL_B]->GetCommand(0).GetID());
			CHECK_EQUAL(1,						m_pChangeChannelPlayerLink->GetCommandCount());
			CHECK_EQUAL(MC_FIELD_CHANGE_FIELD,	m_pChangeChannelPlayerLink->GetCommand(0).GetID());

			CHECK_EQUAL(2,	m_pFields[CHANNEL_A]->GetPlayerQty());
			CHECK_EQUAL(1,	m_pFields[CHANNEL_B]->GetPlayerQty());
		}

		TEST_FIXTURE(Fixture_AddedTestPlayers, MoveChannel_Iteration)
		{
			CHECK_EQUAL(2,	m_pFields[CHANNEL_A]->GetPlayerQty());
			CHECK_EQUAL(1,	m_pFields[CHANNEL_B]->GetPlayerQty());

			const int TEST_LOOP_COUNT = 100;	///< 100�� �ݺ�
			for (int i = 0; i < TEST_LOOP_COUNT; ++i)
			{
				/// TestPlayer �� A�ʵ忡�� B�ʵ�� �̵�.
				gsys.pFieldSystem->PrepareChangeChannel(m_pChangeChannelPlayer, GetChannelID(m_pFields[CHANNEL_B]));
				gsys.pSystem->Sleep(CHANNEL_CHANGE_PREPARE_TIME);
				m_pChangeChannelPlayerLink->OnRecv(MC_FIELD_REQUEST_CHANGE_CHANNEL, 0, NULL);
				m_pChangeChannelPlayerLink->OnRecv(MC_FIELD_LOADING_COMPLETE, 0, NULL);

				CHECK_EQUAL(1,	m_pFields[CHANNEL_A]->GetPlayerQty());
				CHECK_EQUAL(2,	m_pFields[CHANNEL_B]->GetPlayerQty());

				/// TestPlayer �� B�ʵ忡�� A�ʵ�� �̵�.
				gsys.pFieldSystem->PrepareChangeChannel(m_pChangeChannelPlayer, GetChannelID(m_pFields[CHANNEL_A]));
				gsys.pSystem->Sleep(CHANNEL_CHANGE_PREPARE_TIME);
				m_pChangeChannelPlayerLink->OnRecv(MC_FIELD_REQUEST_CHANGE_CHANNEL, 0, NULL);
				m_pChangeChannelPlayerLink->OnRecv(MC_FIELD_LOADING_COMPLETE, 0, NULL);

				CHECK_EQUAL(2,	m_pFields[CHANNEL_A]->GetPlayerQty());
				CHECK_EQUAL(1,	m_pFields[CHANNEL_B]->GetPlayerQty());
			}
		}
	}

	TEST_FIXTURE(Fixture, MoveChannel_CanNotMoveChannel_BecauseDestFieldIsFull)
	{
		/// Fixture ��ġ
		FillWithTestPlayersByChannel(m_pFields[CHANNEL_B], m_pFields[CHANNEL_B]->GetCapacity());

		MockLink* pTestPlayerLink;
		GEntityPlayer*	pChangeChannelPlayer;
		AddTestPlayer(CHANNEL_A, &pChangeChannelPlayer, &pTestPlayerLink);

		CHECK_EQUAL(1,										m_pFields[CHANNEL_A]->GetPlayerQty());
		CHECK_EQUAL(m_pFields[CHANNEL_B]->GetCapacity(),	m_pFields[CHANNEL_B]->GetPlayerQty());

		/// SUT ����
		gsys.pFieldSystem->PrepareChangeChannel(pChangeChannelPlayer, GetChannelID(m_pFields[CHANNEL_B]));

		/// ����
		CHECK_EQUAL(1,			pTestPlayerLink->GetCommandCount());
		CHECK_EQUAL(MC_FIELD_PREPARE_CHANGE_CHANNEL_RES,	pTestPlayerLink->GetCommand(0).GetID());
		CHECK_EQUAL(CR_FAIL_CHANGE_CHANNEL_IS_FULL,			pTestPlayerLink->GetParam<int>(0, 0));

		CHECK_EQUAL(1,										m_pFields[CHANNEL_A]->GetPlayerQty());
		CHECK_EQUAL(m_pFields[CHANNEL_B]->GetCapacity(),	m_pFields[CHANNEL_B]->GetPlayerQty());
	}

	TEST_FIXTURE(Fixture, MoveChannel_CanNotMoveChannel_BecauseDestFieldIsFullAndCapacityIsOnlyOne)
	{
		/// Fixture ��ġ
		const int DEST_CAPACITY = 1;

		m_pFieldInfo->m_nCapacity = DEST_CAPACITY;

		FillWithTestPlayersByChannel(m_pFields[CHANNEL_B], DEST_CAPACITY);

		MockLink* pTestPlayerLink;
		GEntityPlayer*	pChangeChannelPlayer;
		AddTestPlayer(CHANNEL_A, &pChangeChannelPlayer, &pTestPlayerLink);

		CHECK_EQUAL(DEST_CAPACITY,		m_pFields[CHANNEL_B]->GetPlayerQty());

		/// SUT ����
		gsys.pFieldSystem->PrepareChangeChannel(pChangeChannelPlayer, GetChannelID(m_pFields[CHANNEL_B]));

		/// ����
		CHECK_EQUAL(1,			pTestPlayerLink->GetCommandCount());
		CHECK_EQUAL(MC_FIELD_PREPARE_CHANGE_CHANNEL_RES,	pTestPlayerLink->GetCommand(0).GetID());
		CHECK_EQUAL(CR_FAIL_CHANGE_CHANNEL_IS_FULL,			pTestPlayerLink->GetParam<int>(0, 0));

		CHECK_EQUAL(DEST_CAPACITY,							m_pFields[CHANNEL_B]->GetPlayerQty());
	}


	TEST_FIXTURE(Fixture, MoveChannel_DestFieldHasOnlyOneMargin)
	{
		/// Fixture ��ġ
		FillWithTestPlayersByChannel(m_pFields[CHANNEL_B], m_pFields[CHANNEL_B]->GetCapacity()-1);

		MockLink* pTestPlayerLink;
		GEntityPlayer*	pChangeChannelPlayer;
		AddTestPlayer(CHANNEL_A, &pChangeChannelPlayer, &pTestPlayerLink);

		CHECK_EQUAL(1,										m_pFields[CHANNEL_A]->GetPlayerQty());
		CHECK_EQUAL(m_pFields[CHANNEL_B]->GetCapacity()-1,	m_pFields[CHANNEL_B]->GetPlayerQty());

		/// SUT ����
		gsys.pFieldSystem->PrepareChangeChannel(pChangeChannelPlayer, GetChannelID(m_pFields[CHANNEL_B]));
		gsys.pSystem->Sleep(CHANNEL_CHANGE_PREPARE_TIME);
		pTestPlayerLink->OnRecv(MC_FIELD_REQUEST_CHANGE_CHANNEL, 0, NULL);

		/// ����
		CHECK_EQUAL(0,										m_pFields[CHANNEL_A]->GetPlayerQty());
		CHECK_EQUAL(m_pFields[CHANNEL_B]->GetCapacity(),	m_pFields[CHANNEL_B]->GetPlayerQty());
	}

	TEST_FIXTURE(Fixture, ChannelChangeReservation)
	{
		/// Fixture ��ġ
		MockLink* pTestPlayerLink;
		GEntityPlayer*	pChangeChannelPlayer;
		AddTestPlayer(CHANNEL_A, &pChangeChannelPlayer, &pTestPlayerLink);

		CHECK_EQUAL(1,	m_pFields[CHANNEL_A]->GetPlayerQty());
		CHECK_EQUAL(0,	m_pFields[CHANNEL_B]->GetPlayerQty());

		/// SUT ����
		CHECK_EQUAL(true, gsys.pFieldSystem->PrepareChangeChannel(pChangeChannelPlayer, GetChannelID(m_pFields[CHANNEL_B])));

		/// ����
		CHECK_EQUAL(1,	m_pFields[CHANNEL_A]->GetPlayerQty());
		CHECK_EQUAL(1,	m_pFields[CHANNEL_B]->GetPlayerQty());
	}

	TEST_FIXTURE(Fixture, MoveChannel_DestFieldHasOnlyOneMargin_Iteration)
	{
		/// Fixture ��ġ
		FillWithTestPlayersByChannel(m_pFields[CHANNEL_B], m_pFields[CHANNEL_B]->GetCapacity()-1);

		MockLink* pTestPlayerLink;
		GEntityPlayer*	pChangeChannelPlayer;
		AddTestPlayer(CHANNEL_A, &pChangeChannelPlayer, &pTestPlayerLink);

		CHECK_EQUAL(1,										m_pFields[CHANNEL_A]->GetPlayerQty());
		CHECK_EQUAL(m_pFields[CHANNEL_B]->GetCapacity()-1,	m_pFields[CHANNEL_B]->GetPlayerQty());

		const int TEST_LOOP_COUNT = 10;
		for (int i = 0; i < TEST_LOOP_COUNT; ++i)
		{
			/// TestPlayer �� A�ʵ忡�� B�ʵ�� �̵�.
			gsys.pFieldSystem->PrepareChangeChannel(pChangeChannelPlayer, GetChannelID(m_pFields[CHANNEL_B]));
			gsys.pSystem->Sleep(CHANNEL_CHANGE_PREPARE_TIME);
			pTestPlayerLink->OnRecv(MC_FIELD_REQUEST_CHANGE_CHANNEL, 0, NULL);
			pTestPlayerLink->OnRecv(MC_FIELD_LOADING_COMPLETE, 0, NULL);

			CHECK_EQUAL(0,										m_pFields[CHANNEL_A]->GetPlayerQty());
			CHECK_EQUAL(m_pFields[CHANNEL_B]->GetCapacity(),	m_pFields[CHANNEL_B]->GetPlayerQty());


			/// TestPlayer �� B�ʵ忡�� A�ʵ�� �̵�.
			gsys.pFieldSystem->PrepareChangeChannel(pChangeChannelPlayer, GetChannelID(m_pFields[CHANNEL_A]));
			gsys.pSystem->Sleep(CHANNEL_CHANGE_PREPARE_TIME);
			pTestPlayerLink->OnRecv(MC_FIELD_REQUEST_CHANGE_CHANNEL, 0, NULL);
			pTestPlayerLink->OnRecv(MC_FIELD_LOADING_COMPLETE, 0, NULL);

			CHECK_EQUAL(1,										m_pFields[CHANNEL_A]->GetPlayerQty());
			CHECK_EQUAL(m_pFields[CHANNEL_B]->GetCapacity()-1,	m_pFields[CHANNEL_B]->GetPlayerQty());
		}
	}

	TEST_FIXTURE(Fixture, MoveChannel_AllFieldPlayerMoveToAnotherField)
	{
		/// Fixture ��ġ
		FillWithTestPlayersByChannel(m_pFields[CHANNEL_A], m_pFields[CHANNEL_A]->GetCapacity());

		CHECK_EQUAL(m_pFields[CHANNEL_A]->GetCapacity(),	m_pFields[CHANNEL_A]->GetPlayerQty());
		CHECK_EQUAL(0,										m_pFields[CHANNEL_B]->GetPlayerQty());

		for (int i = 0; i < m_pFields[CHANNEL_A]->GetCapacity(); ++i)
		{
			/// B�ʵ�� ä�� �̵� ��û
			gsys.pFieldSystem->PrepareChangeChannel(m_vecTestPlayers[CHANNEL_A][i], GetChannelID(m_pFields[CHANNEL_B]));
			gsys.pSystem->Sleep(CHANNEL_CHANGE_PREPARE_TIME);
			m_vecTestPlayerLinks[CHANNEL_A][i]->OnRecv(MC_FIELD_REQUEST_CHANGE_CHANNEL, 0, NULL);

			/// �ε� �Ϸ� ����
			m_vecTestPlayerLinks[CHANNEL_A][i]->OnRecv(MC_FIELD_LOADING_COMPLETE, 0, NULL);
		}

		CHECK_EQUAL(0,										m_pFields[CHANNEL_A]->GetPlayerQty());
		CHECK_EQUAL(m_pFields[CHANNEL_B]->GetCapacity(),	m_pFields[CHANNEL_B]->GetPlayerQty());
	}

	TEST_FIXTURE(Fixture, CreateNewChannelWhenMoveToFullChannel)
	{
		/// Fixture ��ġ
		FillWithTestPlayersByChannel(m_pFields[CHANNEL_A], m_pFields[CHANNEL_A]->GetCapacity());
		m_pFields[CHANNEL_B]->DeleteMe();
		gmgr.pFieldMgr->Update(.0f); ///< B ä���� ����.

		GEntityPlayer* pTestPlayer = GUTHelper::NewEntityPlayer(GUTHelper_Field::DefaultMockField(), vec3(1000,1000,0));
		GSharedFieldMaster* pSharedFieldMaster = gmgr.pFieldMgr->GetSharedFieldMaster(TEST_FIELD_ID);

		CHECK_EQUAL(1, pSharedFieldMaster->GetChannels().size());

		GSharedField* pChannel2 = pSharedFieldMaster->GetChannel(2);
		CHECK_EQUAL((GSharedField*)NULL, pChannel2);

		/// SUT ����
		pTestPlayer->GetPlayerField().Gate(TEST_FIELD_ID, vec3(0,0,0), vec3(0,1,0));
		gsys.pServer->Update(0.1f);
		pTestPlayer->GetPlayerField().GetFieldEntry().OnEnterFieldComplete();

		/// ����
		pChannel2 = pSharedFieldMaster->GetChannel(2);
		CHECK(NULL != pChannel2);
		CHECK_EQUAL(2, pSharedFieldMaster->GetChannels().size());
		CHECK_EQUAL(1, pChannel2->GetPlayerQty());
	}

	TEST_FIXTURE(Fixture, MoveChannel_DeleteChannel)
	{
		GSharedFieldMaster* pSharedField = m_FieldMgr.Get()->GetSharedFieldMaster(TEST_FIELD_ID);
		int nFieldB_ChannelID = GetChannelID(m_pFields[CHANNEL_B]);

		/// B ä�ο� TestPlayer ����
		MockLink* pTestPlayerLink;
		GEntityPlayer*	pChangeChannelPlayer;
		AddTestPlayer(CHANNEL_B, &pChangeChannelPlayer, &pTestPlayerLink);

		CHECK_EQUAL(0,	m_pFields[CHANNEL_A]->GetPlayerQty());
		CHECK_EQUAL(1,	m_pFields[CHANNEL_B]->GetPlayerQty());
		CHECK_EQUAL(2,	pSharedField->GetChannels().size());

		/// B ä�ο��� A ä�η� �̵�
		gsys.pFieldSystem->PrepareChangeChannel(pChangeChannelPlayer, GetChannelID(m_pFields[CHANNEL_A]));
		gsys.pSystem->Sleep(CHANNEL_CHANGE_PREPARE_TIME);
		pTestPlayerLink->OnRecv(MC_FIELD_REQUEST_CHANGE_CHANNEL, 0, NULL);
		pTestPlayerLink->OnRecv(MC_FIELD_LOADING_COMPLETE, 0, NULL);
		gmgr.pFieldMgr->Update(0.0f);	// �ʵ尡 lazy�ϰ� ������

		CHECK_EQUAL(1,	m_pFields[CHANNEL_A]->GetPlayerQty());
		CHECK_EQUAL(1,	pSharedField->GetChannels().size());

		/// B ä�� ����� üũ
		GField* pNULLField = NULL;
		CHECK_EQUAL(pNULLField,	pSharedField->GetChannel(nFieldB_ChannelID));
		
	}

	TEST_FIXTURE(Fixture, MoveChannel_DontMoveChannel_BecauseInDynamicField)
	{
		/// TestPlayer ����
		GEntityPlayer* pPlayer = NULL;
		MockLink* pLink = NULL;
		AddTestPlayer(CHANNEL_A, &pPlayer, &pLink);

		/// Dynamic Field ���� ����
		const int TEST_DYNAMIC_FIELD_ID = 100;
		const int TEST_DYNAMIC_FIELD_GROUP_ID = 200;

		MARKER_INFO stMarkerInfo;
		stMarkerInfo.nFieldID = TEST_DYNAMIC_FIELD_ID;
		stMarkerInfo.vPoint = vec3(1000, 1000, 0);

		FIELD_GROUP_INFO* pFieldGroupInfo = new FIELD_GROUP_INFO;
		pFieldGroupInfo->nID = TEST_DYNAMIC_FIELD_GROUP_ID;
		pFieldGroupInfo->pStartMarkerInfo = &stMarkerInfo;

		GFieldInfo* pFieldInfo = new GFieldInfo();
		pFieldInfo->m_bDynamic = true;
		pFieldInfo->m_nFieldID = TEST_DYNAMIC_FIELD_ID;
		pFieldInfo->SetFieldMaxXY_ForTest(50000.0f, 50000.0f);

		pFieldGroupInfo->pStartFieldInfo = pFieldInfo;
		pFieldGroupInfo->vecFieldInfo.push_back(pFieldInfo);

		m_FieldInfoMgrWrapper.Get()->Insert(pFieldInfo);
		m_FieldInfoMgrWrapper.Get()->InsertGroup(pFieldGroupInfo);

		/// Player�� Dynamic Field�� �����Ŵ.
		pPlayer->GetPlayerField().GateToTrial(TEST_DYNAMIC_FIELD_GROUP_ID);
		GDynamicFieldMaster* pFieldGroup = gmgr.pFieldMgr->GetDynamicFieldMaster(pPlayer);
		GDynamicField* pDynamicField = pFieldGroup->Get(TEST_DYNAMIC_FIELD_ID);
		gmgr.pFieldMgr->Update(0.1f);
		pPlayer->GetPlayerField().GetFieldEntry().OnEnterFieldComplete();
		CHECK_EQUAL(pDynamicField, pPlayer->GetField());

		/// B ä�η� �̵� ��û : ó�� ���� ����.
		CHECK_EQUAL(1,	pDynamicField->GetEntityQty(ETID_PLAYER));
		CHECK_EQUAL(0,	m_pFields[CHANNEL_B]->GetEntityQty(ETID_PLAYER));

		gsys.pFieldSystem->PrepareChangeChannel(pPlayer, GetChannelID(m_pFields[CHANNEL_B]));
		gsys.pSystem->Sleep(CHANNEL_CHANGE_PREPARE_TIME);
		pLink->OnRecv(MC_FIELD_REQUEST_CHANGE_CHANNEL, 0, NULL);

		CHECK_EQUAL(1,	pDynamicField->GetEntityQty(ETID_PLAYER));
		CHECK_EQUAL(0,	m_pFields[CHANNEL_B]->GetEntityQty(ETID_PLAYER));
	}

	
}
