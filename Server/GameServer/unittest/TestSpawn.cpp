#include "stdafx.h"
#include "GNPCInfo.h"
#include "GTestWrapper.h"
#include "SUnitTestUtil.h"
#include "MockField.h"
#include "GUTHelper.h"
#include "GFieldMgr.h"
#include "GServer.h"
#include "GEnvManager.h"
#include "GWeatherMgr.h"
#include "GFieldInfoMgr.h"
#include "GConst.h"
#include "GEntityNPC.h"
#include "GNPCDecay.h"
#include "GUTHelper_Field.h"
#include "GTestForward.h"

SUITE(Spawn)
{
	struct Fixture
	{
		Fixture()
		{ 
			m_pFieldInfo = new GFieldInfo;
			m_pFieldInfo->m_nFieldID = 100;
			m_pFieldInfo->SetFieldMaxXY_ForTest(50000.0f, 50000.0f);
			gmgr.pFieldInfoMgr->Insert(m_pFieldInfo);

			m_pField = NULL;

			m_pNPCInfo = new GNPCInfo;
			m_pNPCInfo->nID = 1;
			wchar_t szBuff[128];
			swprintf_s(szBuff, L"NPC_%d", m_pNPCInfo->nID);
			m_pNPCInfo->strNameInLua = szBuff;
			gmgr.pNPCInfoMgr->Insert(m_pNPCInfo);
		}

		~Fixture() 
		{
			m_pField->Update(1.0f);
			gmgr.pFieldMgr->DeleteField(m_pField->GetUID());

			m_pFieldInfo->Reset();
		}
		void SetupSpawnInfo(SPAWN_INFO& spawnInfo, int nID)
		{
			spawnInfo.nID = nID;
			spawnInfo.bCheckSpawnPointValidation = false;
			spawnInfo.vPoint = vec3(0,0,0);
			spawnInfo.strName = "1122";
			spawnInfo.vecNPCInfoList.push_back(SPAWN_NPC_INFO(1));
		}

		void InitForGather(int nSpawnInfoCount, GATHER_TYPE nGatherType)
		{
			m_pNPCInfo->nNpcType = NPC_TYPE_GATHER;
			m_pNPCInfo->nGatherType = nGatherType;

			for (int i = 0; i < nSpawnInfoCount; i++)
			{
				// ù��° mine
				SPAWN_INFO Spawninfo;
				SetupSpawnInfo(Spawninfo, 1122 + i);
				Spawninfo.nGatherType = nGatherType;
				m_pFieldInfo->InsertSpawnInfo(Spawninfo);
			}

			m_pField = GUTHelper_Field::NewMockField(m_pFieldInfo);
			gmgr.pFieldMgr->AddField(m_pField);
		}

		GUTHelper		m_Helper;
		MockField*		m_pField;
		GFieldInfo*		m_pFieldInfo;
		GNPCInfo*		m_pNPCInfo;

		GTestMgrWrapper<GFieldMgr>					m_FieldMgr;
		GTestMgrWrapper<GFieldInfoMgr>				m_FieldInfoMgr;
		GTestMgrWrapper<GNPCInfoMgr>				m_NPCInfoMgr;
		DECLWRAPPER_NPCMgr;
	};

	TEST_FIXTURE(Fixture, SpawnBasic)
	{
		SPAWN_INFO Spawninfo;
		SetupSpawnInfo(Spawninfo, 1122);
		m_pFieldInfo->InsertSpawnInfo(Spawninfo);

		m_pField = GUTHelper_Field::NewMockField(m_pFieldInfo);
		gmgr.pFieldMgr->AddField(m_pField);

		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));
		m_pField->Update(0.0f);
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));
	}

	TEST_FIXTURE(Fixture, SpawnStartTime)
	{
		SPAWN_INFO Spawninfo;
		SetupSpawnInfo(Spawninfo, 1122);
		Spawninfo.nStartTime = 124; // 124�� �Ŀ� ����
		m_pFieldInfo->InsertSpawnInfo(Spawninfo);

		m_pField = GUTHelper_Field::NewMockField(m_pFieldInfo);
		gmgr.pFieldMgr->AddField(m_pField);

		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));
		m_pField->Update(0.0f);
		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));	
		m_pField->Update(100.0f);
		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));	
		m_pField->Update(24.0f);
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));	
	}

	TEST_FIXTURE(Fixture, DespawnAndRespawn)
	{
		SPAWN_INFO Spawninfo;
		SetupSpawnInfo(Spawninfo, 1122);
		Spawninfo.nStartTime = 10; // 10�� �Ŀ� ����
		Spawninfo.nRespawnTime = 3; // 3�� �Ŀ� ������
		m_pFieldInfo->InsertSpawnInfo(Spawninfo);

		m_pField = GUTHelper_Field::NewMockField(m_pFieldInfo);
		gmgr.pFieldMgr->AddField(m_pField);

		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));
		m_pField->Update(10.0f);
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));	
		m_pField->GetSpawn()->Despawn(1122, true);		// �ݺ��ǰ� ���� �����ϸ鼭 ����
		m_pField->Update(0.0f);
		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));	
		m_pField->Update(2.0f);
		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));	
		m_pField->Update(1.0f);	// 3�� ����
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));	
	}

	TEST_FIXTURE(Fixture, ForcedSpawn)
	{
		SPAWN_INFO Spawninfo;
		SetupSpawnInfo(Spawninfo, 1122);
		Spawninfo.nStartTime = 10; // 10�� �Ŀ� ����
		Spawninfo.nRespawnTime = 3; // 3�� �Ŀ� ������
		m_pFieldInfo->InsertSpawnInfo(Spawninfo);

		m_pField = GUTHelper_Field::NewMockField(m_pFieldInfo);
		gmgr.pFieldMgr->AddField(m_pField);

		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));
		m_pField->GetSpawn()->Spawn(1122);
		m_pField->Update(0.0f);
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));	
		m_pField->Update(3.0f);
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));	
		m_pField->Update(10.0f);
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));	
	}

	TEST_FIXTURE(Fixture, SpawnTimeDuration)
	{
		SPAWN_INFO Spawninfo;
		SetupSpawnInfo(Spawninfo, 1122);
		Spawninfo.bEnableTimeList[TIME_NIGHT] = true;
		Spawninfo.bUseConditionTime = true;
		m_pFieldInfo->InsertSpawnInfo(Spawninfo);

		m_pField = GUTHelper_Field::NewMockField(m_pFieldInfo);
		gmgr.pFieldMgr->AddField(m_pField);

		gmgr.pEnvManager->ChangeTimeForced(TIME_DAYTIME);			// �ð��밡 ���� ����
		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));
		gsys.pServer->Update(0.1f);
		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));	
		gmgr.pEnvManager->ChangeTimeForced(TIME_NIGHT);
		gsys.pServer->Update(0.1f);
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));
		gmgr.pEnvManager->ChangeTimeForced(TIME_NIGHT);			// ���� �ð���
		gsys.pServer->Update(0.1f);
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));
		gmgr.pEnvManager->ChangeTimeForced(TIME_DAWN);				// �ð��밡 ���� ����
		gsys.pServer->Update(0.1f);
		gsys.pServer->Update(0.1f);	// ��ƼƼ ���ſ� �Һ�Ǵ� ƽ
		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));	
	}

	TEST_FIXTURE(Fixture, SpawnTimeDetailDuration)
	{
		class MockEnvManager : public GEnvManager
		{
		protected:

		public:
			using GEnvManager::SetTimeHour;
			MockEnvManager() : GEnvManager() 
			{
				m_fNextCheckTime = m_fElapsed + 99999.0f;
			}
			virtual ~MockEnvManager() {}
		};

		GTestMgrWrapper2<GEnvManager, MockEnvManager> m_EnvManagerWrapper;
		MockEnvManager* pEnvManager = dynamic_cast<MockEnvManager*>(gmgr.pEnvManager);

		SPAWN_INFO Spawninfo;
		SetupSpawnInfo(Spawninfo, 1122);
		Spawninfo.pairEnableTime = pair<int,int>(0, 12);	// 0~12�ñ����� ������
		m_pFieldInfo->InsertSpawnInfo(Spawninfo);

		m_pField = GUTHelper_Field::NewMockField(m_pFieldInfo);
		gmgr.pFieldMgr->AddField(m_pField);

		pEnvManager->SetTimeHour(13);			// �ð� ���� ����
		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));
		gsys.pServer->Update(0.1f);
		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));	
		pEnvManager->SetTimeHour(6);	
		gsys.pServer->Update(0.1f);
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));
		pEnvManager->SetTimeHour(7);				// ���� �ð���
		gsys.pServer->Update(0.1f);
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));
		pEnvManager->SetTimeHour(15);				// �ð��밡 ���� ����
		gsys.pServer->Update(0.1f);
		gsys.pServer->Update(0.1f);	// ��ƼƼ ���ſ� �Һ�Ǵ� ƽ
		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));	
	}

	TEST_FIXTURE(Fixture, NoRespawnWhenChangedBetweenSameTimeDuration)
	{
		SPAWN_INFO Spawninfo;
		SetupSpawnInfo(Spawninfo, 1122);
		Spawninfo.bEnableTimeList[TIME_NIGHT] = true;
		Spawninfo.bUseConditionTime = true;
		m_pFieldInfo->InsertSpawnInfo(Spawninfo);

		m_pField = GUTHelper_Field::NewMockField(m_pFieldInfo);
		gmgr.pFieldMgr->AddField(m_pField);

		gmgr.pEnvManager->SetTime(TIME_NIGHT);
		gsys.pServer->Update(0.1f);

		GEntityNPC* pNPC1 = m_pField->GetSpawn()->GetNPCInstance(1122);
		if (!pNPC1)	{ CHECK(FALSE); return; }

		gmgr.pEnvManager->SetTime(TIME_NIGHT);
		gsys.pServer->Update(0.1f);

		GEntityNPC* pNPC2 = m_pField->GetSpawn()->GetNPCInstance(1122);
		if (!pNPC2)	{ CHECK(FALSE); return; }

		CHECK_EQUAL(pNPC1->GetUID(), pNPC2->GetUID());
	}

	TEST_FIXTURE(Fixture, SpawnWeatherDuration)
	{
		SPAWN_INFO Spawninfo;
		SetupSpawnInfo(Spawninfo, 1122);
		Spawninfo.bEnableWeatherList[WEATHER_HEAVY_SNOWY] = true;
		Spawninfo.bUseConditionWeather = true;
		m_pFieldInfo->InsertSpawnInfo(Spawninfo);

		m_pField = GUTHelper_Field::NewMockField(m_pFieldInfo);
		gmgr.pFieldMgr->AddField(m_pField);

		GWeatherMgr* pWeatherMgr = new GWeatherMgr();
		pWeatherMgr->Init(m_pFieldInfo, GConst::CONST_ENV_WEATHER_TICK_SEC);
		m_pField->SetWeatherMgr(pWeatherMgr);

		m_pField->ChangeWeather(WEATHER_CLOUDY);			// �ð��밡 ���� ����
		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));
		m_pField->Update(0.1f);
		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));	
		m_pField->ChangeWeather(WEATHER_HEAVY_SNOWY);	
		m_pField->Update(0.1f);
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));
		m_pField->ChangeWeather(WEATHER_HEAVY_SNOWY);				// ���� �ð���
		m_pField->Update(0.1f);
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));
		m_pField->ChangeWeather(WEATHER_RAINY);				// �ð��밡 ���� ����
		m_pField->Update(0.1f);
		m_pField->Update(0.1f);	// ��ƼƼ ���ſ� �Һ�Ǵ� ƽ
		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));	
	}

	TEST_FIXTURE(Fixture, NoRespawnWhenChangedBetweenSameWeatherDuration)
	{
		SPAWN_INFO Spawninfo;
		SetupSpawnInfo(Spawninfo, 1122);
		Spawninfo.bEnableWeatherList[WEATHER_HEAVY_SNOWY] = true;
		Spawninfo.bEnableWeatherList[WEATHER_RAINY] = true;
		Spawninfo.bUseConditionWeather = true;
		m_pFieldInfo->InsertSpawnInfo(Spawninfo);

		m_pField = GUTHelper_Field::NewMockField(m_pFieldInfo);
		gmgr.pFieldMgr->AddField(m_pField);

		GWeatherMgr* pWeatherMgr = new GWeatherMgr();
		pWeatherMgr->Init(m_pFieldInfo, GConst::CONST_ENV_WEATHER_TICK_SEC);
		m_pField->SetWeatherMgr(pWeatherMgr);

		m_pField->ChangeWeather(WEATHER_HEAVY_SNOWY);
		m_pField->Update(0.1f);

		GEntityNPC* pNPC1 = m_pField->GetSpawn()->GetNPCInstance(1122);
		if (!pNPC1)	{ CHECK(FALSE); return; }

		m_pField->ChangeWeather(WEATHER_RAINY);
		m_pField->Update(0.1f);

		GEntityNPC* pNPC2 = m_pField->GetSpawn()->GetNPCInstance(1122);
		if (!pNPC2)	{ CHECK(FALSE); return; }

		CHECK_EQUAL(pNPC1->GetUID(), pNPC2->GetUID());
	}

	TEST_FIXTURE(Fixture, SpawnWeatherAndTimeDuration)
	{
		SPAWN_INFO Spawninfo;
		SetupSpawnInfo(Spawninfo, 1122);
		Spawninfo.bEnableWeatherList[WEATHER_HEAVY_SNOWY] = true;
		Spawninfo.bEnableTimeList[TIME_NIGHT] = true;
		Spawninfo.bUseConditionTime = true;
		Spawninfo.bUseConditionWeather = true;
		m_pFieldInfo->InsertSpawnInfo(Spawninfo);

		m_pField = GUTHelper_Field::NewMockField(m_pFieldInfo);
		gmgr.pFieldMgr->AddField(m_pField);

		GWeatherMgr* pWeatherMgr = new GWeatherMgr();
		pWeatherMgr->Init(m_pFieldInfo, GConst::CONST_ENV_WEATHER_TICK_SEC);
		m_pField->SetWeatherMgr(pWeatherMgr);

		gmgr.pEnvManager->SetTime(TIME_DAYTIME);			// �ð��밡 ���� ����
		m_pField->ChangeWeather(WEATHER_CLOUDY);			// �ð��밡 ���� ����
		gsys.pServer->Update(0.1f);
		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));
		gmgr.pEnvManager->SetTime(TIME_NIGHT);
		m_pField->ChangeWeather(WEATHER_HEAVY_SNOWY);	
		gsys.pServer->Update(0.1f);
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));
		gmgr.pEnvManager->SetTime(TIME_NIGHT);
		m_pField->ChangeWeather(WEATHER_HEAVY_SNOWY);					// ���� �ð���
		gsys.pServer->Update(0.1f);
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));
		gmgr.pEnvManager->SetTime(TIME_DAYTIME);			// �ð��밡 ���� ����
		m_pField->ChangeWeather(WEATHER_CLOUDY);			// �ð��밡 ���� ����
		gsys.pServer->Update(0.1f);
		gsys.pServer->Update(0.1f);	// ��ƼƼ ���ſ� �Һ�Ǵ� ƽ
		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));	
	}

	TEST_FIXTURE(Fixture, RemoveToSpawnManagerWhenDecay)
	{
		SPAWN_INFO Spawninfo;
		SetupSpawnInfo(Spawninfo, 1122);
		Spawninfo.nRespawnTime = 3; // 3���Ŀ� ������
		m_pFieldInfo->InsertSpawnInfo(Spawninfo);
		m_pField = GUTHelper_Field::NewMockField(m_pFieldInfo);
		gmgr.pFieldMgr->AddField(m_pField);

		// NPC ����
		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));
		m_pField->Update(0.0f);
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));
		
		// NPC ����
		GEntityNPC* pNPC1 = m_pField->GetSpawn()->GetNPCInstance(1122);
		TVALID(pNPC1);
		pNPC1->GetNPCDecay().SetCustomDecayTick(5.0f); // 5�ʵڿ� ����
		pNPC1->doDie();
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));

		// NPC ������
		m_pField->Update(3.0f);
		CHECK_EQUAL(2, m_pField->GetEntityQty(ETID_NPC));
		GEntityNPC* pNPC2 = m_pField->GetSpawn()->GetNPCInstance(1122);
		TVALID(pNPC2);
		CHECK(pNPC1->GetUID() != pNPC2->GetUID());	// ���� �ٸ� NPC

		// ��ü �Ҹ� �Ϸ�
		m_pField->Update(2.0f); // 5�ʰ� ���� ������ ��
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));
	}

	TEST_FIXTURE(Fixture, TestSpawn_IsGatherEntitySpawnEnabled_NoMaxGatherInstance)
	{
		// �ִ� 1���� ��������
		m_pFieldInfo->m_nMaxGatherInstance[GATHER_MINE] = 0;	

		InitForGather(2, GATHER_MINE);

		// mine ����
		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));
		m_pField->Update(0.0f);

		// 2�� �� ������
		CHECK_EQUAL(2, m_pField->GetEntityQty(ETID_NPC));
	}

	TEST_FIXTURE(Fixture, TestSpawn_IsGatherEntitySpawnEnabled)
	{
		// �ִ� 1���� ��������
		m_pFieldInfo->m_nMaxGatherInstance[GATHER_MINE] = 1;	

		InitForGather(2, GATHER_MINE);

		// mine ����
		CHECK_EQUAL(0, m_pField->GetEntityQty(ETID_NPC));
		m_pField->Update(0.0f);

		// �ִ� 1���� ���������ϴ� �ϳ��� �����Ǿ�� �Ѵ�.
		CHECK_EQUAL(1, m_pField->GetEntityQty(ETID_NPC));
	}
}

