#include "stdafx.h"
#include "SUnitTestUtil.h"
#include "GTestWrapper.h"
#include "MockField.h"
#include "GUTHelper.h"
#include "GNPCInfo.h"
#include "GEntityNPC.h"
#include "GEntityPlayer.h"
#include "GModuleAI.h"
#include "GJob_MoveToPos.h"
#include "GJob_MoveToEntity.h"
#include "MockEntityNPC.h"
#include "GFieldSystem.h"
#include "GConditionInfo.h"
#include "GConditionsInfo.h"
#include "GConditionsInfoMgr.h"
#include "GTalentInfoMgr.h"
#include "GFieldMgr.h"
#include "GModuleCombat.h"
#include "GScriptGroup_Field.h"
#include "GFieldInfoMgr.h"
#include "GUTHelper_Field.h"

const vec3 vCommonPos = vec3(1000.0f, 1000.0f, 0.0f);
const vec3 vCommonDir = vec3(0,1,0);

class TestFieldMgr : public GFieldMgr
{
public:
	virtual MUID	NewUID()
	{
		return SUnitTestUtil::NewUID();
	}
};

SUITE(Sensor)
{
	struct Fixture
	{
		Fixture()
		{ 
			gsys.pScriptManager->Init();

			m_pFieldInfo = new GFieldInfo;
			m_pFieldInfo->m_strFieldFile = L"mockfield";
			m_pFieldInfo->m_nFieldID = 100;
			m_pFieldInfo->SetFieldMaxXY_ForTest(50000.0f, 50000.0f);
			MARKER_INFO MarkerInfo;
			{
				MarkerInfo.nID = 777;
				MarkerInfo.nFieldID = 100;
				MarkerInfo.vPoint = vec3(1000,1000,1500);
				m_pFieldInfo->InsertMarkerInfo(MarkerInfo);
			}
			{
				MarkerInfo.nID = 778;
				MarkerInfo.nFieldID = 100;
				MarkerInfo.vPoint = vec3(3000,3000,3500);
				m_pFieldInfo->InsertMarkerInfo(MarkerInfo);
			}
			{
				MarkerInfo.nID = 779;
				MarkerInfo.nFieldID = 100;
				MarkerInfo.vPoint = vec3(4000,4000,4500);
				m_pFieldInfo->InsertMarkerInfo(MarkerInfo);
			}
			{
				// ���� ����
				infoSensor.nID = 10;
				infoSensor.fRadius = 1000.0f;
				infoSensor.vPoint = vec3(1000,1000,0);
				m_pFieldInfo->InsertSensor(infoSensor);
			}
			
			{
				SENSOR_INFO infoOtherSensor;
				infoOtherSensor.nID = 11;
				infoOtherSensor.fRadius = 1000.0f;
				infoOtherSensor.vPoint = vec3(1000,1000,0);
				m_pFieldInfo->InsertSensor(infoOtherSensor);
			}

			gmgr.pFieldInfoMgr->Insert(m_pFieldInfo);

			m_pField = GUTHelper_Field::NewMockField(m_pFieldInfo);
			gmgr.pFieldMgr->AddField(m_pField);

			m_pNPCInfo = new GNPCInfo;
			m_pNPCInfo->nID = 1;
			gmgr.pNPCInfoMgr->Insert(m_pNPCInfo);

			GetScriptField()->Load(m_pFieldInfo);
		}

		~Fixture() 
		{
			m_pField->Update(1.0f);
			m_pField->Destroy();

			gsys.pScriptManager->Fini();
		}

		bool RunLuaString( const char* szFormat, ... )
		{
			char szBuff[1024];

			va_list args;
			va_start(args,szFormat);
			vsprintf(szBuff,szFormat,args);
			va_end(args);

			return WRUNSTRING(WLUA, szBuff);
		}


		SENSOR_INFO		infoSensor;
		GUTHelper		m_Helper;
		MockField*		m_pField;
		GFieldInfo*		m_pFieldInfo;
		GNPCInfo*		m_pNPCInfo;

		GTestSysWrapper<GScriptManager>				m_ScriptMgr;
		GTestMgrWrapper2<GFieldMgr, TestFieldMgr>	m_FieldMgr;
		GTestMgrWrapper<GFieldInfoMgr>				m_FieldInfoMgr;
		GTestMgrWrapper<GNPCInfoMgr>				m_NPCInfoMgr;
		GTestMgrWrapper<GTalentInfoMgr>				m_TalentInfoMgr;
	
	};

	TEST_FIXTURE(Fixture, CreateSensor)
	{
		const SENSOR_INFO* info = m_pFieldInfo->FindSensor(10);
		if (!info)		{ CHECK(FALSE && "FindSensor() is failed."); return; }
		CHECK_EQUAL(info->nID, 10);
	}

	TEST_FIXTURE(Fixture, SensorValidation)
	{
		GEntityPlayer* pPlayer = GUTHelper::NewEntityPlayer(m_pField, vCommonPos);
		CHECK_EQUAL(pPlayer->IsValidSensor(10, vec3(1000, 1000, 1000)), true);
		pPlayer->SetPos(vec3(5000, 5000, 5000));
		CHECK_EQUAL(pPlayer->IsValidSensor(10, vec3(1000, 1000, 1000)), false);	// �ʹ� �ָ��ִ�.
		pPlayer->SetPos(vCommonPos);
		CHECK_EQUAL(pPlayer->IsValidSensor(10, vec3(1000, 1000, 1000)), true);
		m_pField->EnableSensor(10, false);
//		CHECK_EQUAL(pPlayer->IsValidSensor(10, vec3(1000, 1000, 1000)), false);	// ������ �����ִ�.
	}

	TEST_FIXTURE(Fixture, EnterSensor)
	{
		const char* pszTestFuncName = "Field_100:OnSensorEnter_10";
		const char* pszScript = 
			"function %s(this, Actor)\n"
			"		TEST_VAR = 1;\n"
			"end";

		if (!RunLuaString(pszScript, pszTestFuncName))	{ CHECK(FALSE && "WRunString Failed.");	 return; }

		GEntityPlayer* pPlayer = GUTHelper::NewEntityPlayer(m_pField, vCommonPos);
		CHECK_EQUAL(WLUA->GetVar<int>("TEST_VAR"), 0);
		pPlayer->EnterSensor(10);
		CHECK_EQUAL(WLUA->GetVar<int>("TEST_VAR"), 1);
	}

	TEST_FIXTURE(Fixture, LeaveSensor)
	{
		const char* pszTestFuncName = "Field_100:OnSensorLeave_10";
		const char* pszScript = 
			"function %s(this, Actor)\n"
			"		TEST_VAR = 1;\n"
			"end";

		if (!RunLuaString(pszScript, pszTestFuncName))	{ CHECK(FALSE && "WRunString Failed.");	 return; }

		GEntityPlayer* pPlayer = GUTHelper::NewEntityPlayer(m_pField, vCommonPos);
		CHECK_EQUAL(WLUA->GetVar<int>("TEST_VAR"), 0);
		pPlayer->EnterSensor(10);
		CHECK_EQUAL(WLUA->GetVar<int>("TEST_VAR"), 0);
		pPlayer->SetPos(vec3(5000, 5000, 5000));
		pPlayer->LeaveSensor(10);
		CHECK_EQUAL(WLUA->GetVar<int>("TEST_VAR"), 1);
	}

	TEST_FIXTURE(Fixture, InteractOnSensor)
	{
		const char* pszTestFuncName = "Field_100:OnSensorInteract_10";
		const char* pszScript = 
			"function %s(this, Actor)\n"
			"	TEST_VAR = 1;\n"
			"end";

		if (!RunLuaString(pszScript, pszTestFuncName))	{ CHECK(FALSE && "WRunString Failed.");	 return; }

		GEntityPlayer* pPlayer = GUTHelper::NewEntityPlayer(m_pField, vCommonPos);
		
		CHECK_EQUAL(WLUA->GetVar<int>("TEST_VAR"), 0);

		vector<uint32> vecSensorID;
		vecSensorID.push_back(10);
		pPlayer->SensorInteraction(vecSensorID);
		CHECK_EQUAL(WLUA->GetVar<int>("TEST_VAR"), 0);

		// ������ ������ ���ͷ��� ����
		pPlayer->EnterSensor(10);
		pPlayer->SensorInteraction(vecSensorID);
		CHECK_EQUAL(WLUA->GetVar<int>("TEST_VAR"), 1);
	}

	TEST_FIXTURE(Fixture, UseTalentOnSensor)
	{
		const char* pszTestFuncName = "Field_100:OnSensorTalent_10";
		const char* pszScript = 
			"function %s(this, Actor, TalentID)\n"
			"	TEST_VAR = 1;\n"
			"end";

		if (!RunLuaString(pszScript, pszTestFuncName))	{ CHECK(FALSE && "WRunString Failed.");	 return; }

		//���� �ŷ�Ʈ
		GTalentInfo* pAktTalent = m_Helper.NewMeleeTalentInfo(1);

		GEntityPlayer* pPlayer = GUTHelper::NewEntityPlayer(m_pField, vCommonPos);
		
		pPlayer->SetPos(vec3(5000, 5000, 0));		// ������ �ָ� ����������
		pPlayer->EnterSensor(10);
		pPlayer->doUseTalent(pAktTalent);
		m_pField->Update(1.0f);
		CHECK_EQUAL(WLUA->GetVar<int>("TEST_VAR"), 0);	// �ŷ�Ʈ�� ���������� ��������� �ݹ��� ȣ����� �ʾҴ�.
		pPlayer->GetModuleCombat()->Clear();

		pPlayer->SetPos(vec3(1000, 1000, 0));		// ���� ��ȿ�Ǿ����� ����
		pPlayer->EnterSensor(10);
		pPlayer->doUseTalent(pAktTalent);
		m_pField->Update(1.0f);
		m_pField->Update(1.0f);
		CHECK_EQUAL(WLUA->GetVar<int>("TEST_VAR"), 1);	// �ݹ��Լ� ȣ���
	}

	TEST_FIXTURE(Fixture, ConditionCheck)
	{
		GTestMgrWrapper<GConditionsInfoMgr>			ConditionInfoMgr;

		{
			SENSOR_INFO* pSensor = const_cast<SENSOR_INFO*>(m_pFieldInfo->FindSensor(10));
			if (!pSensor)		{ CHECK(FALSE && "FindSensor(10) Failed.");	 return; }
			pSensor->vPoint = vec3(10000.0f,10000.0f,0);		//���� ������ �ٸ� ���� ġ���д�.
		}
		

		SENSOR_INFO* pSensor = const_cast<SENSOR_INFO*>(m_pFieldInfo->FindSensor(11));
		if (!pSensor)		{ CHECK(FALSE && "FindSensor(11) Failed.");	 return; }
		pSensor->vPoint = vec3(1000.0f,1000.0f,0);		

		// ���� �����1
		{
			SENSOR_WARP_INFO infoWarp;
			infoWarp.nConditionID = 100;
			infoWarp.nFieldID = 100;
			infoWarp.nMarkerID = 777;
			infoWarp.eGateType = TGT_AUTO;
			pSensor->vecWarpInfos.push_back(infoWarp);
		}
		// ���� �����2
		{
			SENSOR_WARP_INFO infoWarp;
			infoWarp.nConditionID = 101;
			infoWarp.nFieldID = 100;
			infoWarp.nMarkerID = 778;
			infoWarp.eGateType = TGT_AUTO;
			pSensor->vecWarpInfos.push_back(infoWarp);
		}


		{
			GConditionsInfo* pCondInfoList = new GConditionsInfo();
			pCondInfoList->Create();
			pCondInfoList->m_nID = 100;
			GCondition_Leaf* pCondInfo = new GCondition_Leaf();
			pCondInfo->m_nCT = CT_LEVEL;
			pCondInfo->m_vecParam1.push_back(1);
			pCondInfo->m_vecParam2.push_back(10);
			pCondInfoList->m_setUsedConditionType.insert(pCondInfo->m_nCT);
			pCondInfoList->m_pConditionComposite->m_vecConditionComponent.push_back(pCondInfo);
			gmgr.pCondtionsInfoMgr->Insert(pCondInfoList);
		}

		{
			GConditionsInfo* pCondInfoList = new GConditionsInfo();
			pCondInfoList->Create();
			pCondInfoList->m_nID = 101;
			GCondition_Leaf* pCondInfo = new GCondition_Leaf();
			pCondInfo->m_nCT = CT_LEVEL;
			pCondInfo->m_vecParam1.push_back(10);
			pCondInfo->m_vecParam2.push_back(20);
			pCondInfoList->m_setUsedConditionType.insert(pCondInfo->m_nCT);
			pCondInfoList->m_pConditionComposite->m_vecConditionComponent.push_back(pCondInfo);
			gmgr.pCondtionsInfoMgr->Insert(pCondInfoList);
		}
		
		const char* pszTestFuncName = "Field_100:OnSensorEnter_11";
		const char* pszScript = 
			"function %s(this, Actor)\n"
			"		TEST_VAR = 1;\n"
			"end";
		if (!RunLuaString(pszScript, pszTestFuncName))	{ CHECK(FALSE && "WRunString Failed.");	 return; }

		GEntityPlayer* pPlayer = GUTHelper::NewEntityPlayer(m_pField, vCommonPos);
		// ����� 100�� ���ǿ� ����
		pPlayer->GetPlayerInfo()->nLevel = 5; 
		CHECK_EQUAL(WLUA->GetVar<int>("TEST_VAR"), 0);
		pPlayer->EnterSensor(11);
		CHECK_EQUAL(WLUA->GetVar<int>("TEST_VAR"), 0);
		CHECK_EQUAL(pPlayer->GetPos().IsEqual(vec3(1000,1000,1500)), true);
		// ����� 101�� ���ǿ� ����
		pPlayer->GetPlayerInfo()->nLevel = 15; 
		pPlayer->SetPos(vec3(10000.0f, 10000.0f, 0));
		pPlayer->EnterSensor(10);
		pPlayer->SetPos(vec3(1000, 1000, 0));
		pPlayer->EnterSensor(11);
		CHECK_EQUAL(WLUA->GetVar<int>("TEST_VAR"), 0);
		CHECK_EQUAL(pPlayer->GetPos().IsEqual(vec3(3000,3000,3500)), true);

		// �⺻ ����� �߰�
		{
			const SENSOR_INFO* pSensor = m_pFieldInfo->FindSensor(11);
			if (!pSensor)		{ CHECK(FALSE && "FindSensor(11) Failed.");	 return; }

			SENSOR_WARP_INFO infoWarp;
			infoWarp.nFieldID = 100;
			infoWarp.nMarkerID = 779;
			infoWarp.eGateType = TGT_AUTO;
			const_cast<SENSOR_INFO*>(pSensor)->DefaultWarpInfo = infoWarp;
		}

		// �ߺ��� ������ ó��
		pPlayer->SetPos(vec3(10000.0f, 10000.0f, 0));
		pPlayer->EnterSensor(10);
		pPlayer->SetPos(vec3(1000, 1000, 0));
		CHECK_EQUAL(pPlayer->GetWarpTriggerHistoryQty(), 0);
		pPlayer->EnterSensor(11);
		CHECK_EQUAL(WLUA->GetVar<int>("TEST_VAR"), 0);
		CHECK_EQUAL(pPlayer->GetPos().IsEqual(vec3(1000,1000,0)), true);	// Ʈ���� �Ͼ�� ����, ��ġ ���� ����
		CHECK_EQUAL(pPlayer->GetWarpTriggerHistoryQty(), 2);				// �������� 2���� ��ħ
		pPlayer->ClearEnteredSensorID();

		// ��� ����� ���н�ų ����
		pPlayer->GetPlayerInfo()->nLevel = 25; 
		pPlayer->SetPos(vec3(10000.0f, 10000.0f, 0));
		pPlayer->EnterSensor(10);
		pPlayer->SetPos(vec3(1000, 1000, 0));
		pPlayer->EnterSensor(11);
		CHECK_EQUAL(WLUA->GetVar<int>("TEST_VAR"), 0);
		CHECK_EQUAL(pPlayer->GetPos().IsEqual(vec3(4000,4000,4500)), true);
		pPlayer->ClearEnteredSensorID();

		// �⺻ ���� Ʈ���� ���� ������ ���� ���
		pSensor->DefaultWarpInfo.SetInvalid();	// �⺻ ��Ż Ʈ���� ��ȿȭ
		pPlayer->SetPos(vec3(10000.0f, 10000.0f, 0));
		pPlayer->EnterSensor(10);
		pPlayer->SetPos(vec3(1000, 1000, 0));
		pPlayer->EnterSensor(11);
		CHECK_EQUAL(WLUA->GetVar<int>("TEST_VAR"), 1);// ��ũ��Ʈ�� �Լ��� ȣ��ƴ�.
		pPlayer->ClearEnteredSensorID();
	}
}
