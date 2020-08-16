#include "stdafx.h"
#include "GUTHelper.h"
#include "GUTHelper_Party.h"
#include "FBaseFieldInfo.h"
#include "MockField.h"
#include "GFieldMgr.h"
#include "GFieldInfoMgr.h"
#include "GSharedField.h"
#include "GDynamicFieldMaster.h"
#include "GPlayerFieldDynamic.h"
#include "GServer.h"
#include "GParty.h"
#include "GUTHelper_Field.h"
#include "GTestForward.h"

const int TEST_SHARED_FIELD_ID = 300;
const int TEST_FIELD_GROUP_ID = 10000;
const int TEST_FIRST_DYNAMIC_FIELD_ID = 10100;
const vec3 vCommonPos = vec3(1000.0f, 1000.0f, 0.0f);
const vec3 vCommonDir = vec3(0,1,0);

SUITE(Field_Dynamic_Leave)
{
	struct FFieldDynamic : public FBaseFieldInfo
	{
		FFieldDynamic()
		{
			InitTestField();
			InitTestDynamicFieldInfos();
			InitTestPlayer();
		}
		virtual ~FFieldDynamic()
		{
		}
		void InitTestField()
		{
			m_pFieldInfo = AddFieldInfo(TEST_SHARED_FIELD_ID, 50000.0f, 50000.0f);
			m_pTestSharedField1 = GUTHelper_Field::NewSharedField(m_pFieldInfo, 1);
		}
		void InitTestDynamicFieldInfos()
		{
			FIELD_GROUP_INFO* info = new FIELD_GROUP_INFO;
			info->nID = TEST_FIELD_GROUP_ID;
			info->setRunningQuest.insert(100);
			bool bStartField = true;
			for (int i=TEST_FIRST_DYNAMIC_FIELD_ID; i<=TEST_FIRST_DYNAMIC_FIELD_ID+2; i++)
			{
				// ���� �ʵ�
				{
					GFieldInfo* pFieldInfo = AddFieldInfo(i, 50000.0f, 50000.0f);
					pFieldInfo->m_bDynamic = true;
					MARKER_INFO marker(1, pFieldInfo->m_nFieldID, vec3(1000, 1000, 1000));
					pFieldInfo->InsertMarkerInfo(marker);

					info->vecFieldInfo.push_back(pFieldInfo);

					if (true == bStartField)
					{
						info->pStartFieldInfo = pFieldInfo;
						info->pStartMarkerInfo = pFieldInfo->FindMarker(1);
						bStartField = false;
					}
				}

			}

			gmgr.pFieldInfoMgr->InsertGroup(info);
		}
		void InitTestPlayer()
		{
			m_pTestPlayer = GUTHelper::NewEntityPlayer(m_pTestSharedField1, vCommonPos);
		}

		GUTHelper					m_Helper;
		GFieldInfo*					m_pFieldInfo;
		GSharedField*				m_pTestSharedField1;
		GEntityPlayer*				m_pTestPlayer;

		GTestMgrWrapper<GFieldMgr>	m_FieldMgr;
		DECLWRAPPER_NPCMgr;
	};

	TEST_FIXTURE(FFieldDynamic, LeaveDynamicField_DestroyDynamicField)
	{
		/// Fixture ��ġ : �÷��̾� �����ʵ�� ����.
		m_pTestPlayer->GetPlayerField().GateToTrial(TEST_FIELD_GROUP_ID);
		gsys.pServer->Update(0.1f);
		m_pTestPlayer->GetPlayerField().GetFieldEntry().OnEnterFieldComplete();

		CHECK_EQUAL(1, gmgr.pFieldMgr->GetDynamicFieldGroupQty());

		GDynamicFieldMaster* pFieldGroup = gmgr.pFieldMgr->GetDynamicFieldMaster(m_pTestPlayer);
		CHECK(NULL != pFieldGroup);
		CHECK_EQUAL(1, pFieldGroup->GetPlayerQty());

		/// SUT ���� : �÷��̾� �����ʵ� ����.
		m_pTestPlayer->GetPlayerField().Gate(m_pFieldInfo->m_nFieldID, vCommonPos, vCommonDir);
		gsys.pServer->Update(0.1f);

		m_pTestPlayer->GetPlayerField().GetFieldEntry().OnEnterFieldComplete();
		gsys.pServer->Update(0.1f);

		/// ����
		CHECK_EQUAL(0, gmgr.pFieldMgr->GetDynamicFieldGroupQty());	///< �����ʵ� ���ŵ�

		pFieldGroup = gmgr.pFieldMgr->GetDynamicFieldMaster(m_pTestPlayer);
		CHECK(NULL == pFieldGroup);
		CHECK_EQUAL(m_pTestPlayer->GetFieldUID(), m_pTestSharedField1->GetUID());
	}

	TEST_FIXTURE(FFieldDynamic, LeaveDynamicField_RemoveFromParty)
	{
		// �÷��̾� �����ʵ�� ����.
		m_pTestPlayer->GetPlayerField().GateToTrial(TEST_FIELD_GROUP_ID);
		gsys.pServer->Update(0.1f);
		m_pTestPlayer->GetPlayerField().GetFieldEntry().OnEnterFieldComplete();

		GDynamicFieldMaster* pFieldGroup = gmgr.pFieldMgr->GetDynamicFieldMaster(m_pTestPlayer);

		// �÷��̾� �����ʵ� ����.
		m_pTestPlayer->OnLeaveFromParty();
		gsys.pServer->Update(0.1f);
		m_pTestPlayer->GetPlayerField().GetFieldEntry().OnEnterFieldComplete();

		gsys.pServer->Update(0.1f);
		CHECK_EQUAL(0, gmgr.pFieldMgr->GetDynamicFieldGroupQty());	///< �����ʵ� ��� ���ŵ�

		pFieldGroup = gmgr.pFieldMgr->GetDynamicFieldMaster(m_pTestPlayer);
	}

	TEST_FIXTURE(FFieldDynamic, LeaveDynamicField_LeaveOnePartyMember)
	{
		/// Fixture ��ġ
		GEntityPlayer* pPlayer2 = GUTHelper::NewEntityPlayer(m_pTestSharedField1, vCommonPos);
		GParty* pParty = GUTHelper_Party::CreateParty(m_pTestPlayer, pPlayer2); ///< ��Ƽ ����

		// 1�� �÷��̾� �����ʵ� ����.
		m_pTestPlayer->GetPlayerField().GateToTrial(TEST_FIELD_GROUP_ID);
		gsys.pServer->Update(0.1f);
		m_pTestPlayer->GetPlayerField().GetFieldEntry().OnEnterFieldComplete();

		GDynamicFieldMaster* pPlayer1_FieldGroup = gmgr.pFieldMgr->GetDynamicFieldMaster(m_pTestPlayer);
		CHECK(NULL != pPlayer1_FieldGroup);
		CHECK_EQUAL(1, pPlayer1_FieldGroup->GetPlayerQty());


		// 2�� �÷��̾�, 1�� �÷��̾�� ���� �����ʵ�� ����.
		pPlayer2->GetPlayerField().GateToTrial(TEST_FIELD_GROUP_ID);
		gsys.pServer->Update(0.1f);
		pPlayer2->GetPlayerField().GetFieldEntry().OnEnterFieldComplete();

		GDynamicFieldMaster* pPlayer2_FieldGroup = gmgr.pFieldMgr->GetDynamicFieldMaster(pPlayer2);
		CHECK(NULL != pPlayer2_FieldGroup);
		CHECK_EQUAL(pPlayer1_FieldGroup->GetUID(), pPlayer2_FieldGroup->GetUID());

		CHECK_EQUAL(2, pPlayer2_FieldGroup->GetPlayerQty());


		// 1�� �÷��̾� ����
		m_pTestPlayer->GetPlayerField().Gate(m_pFieldInfo->m_nFieldID, vCommonPos, vCommonDir);
		gsys.pServer->Update(0.1f);
		m_pTestPlayer->GetPlayerField().GetFieldEntry().OnEnterFieldComplete();
		CHECK_EQUAL(m_pTestPlayer->GetFieldUID(), m_pTestSharedField1->GetUID());

		gsys.pServer->Update(0.1f);
		CHECK_EQUAL(1, gmgr.pFieldMgr->GetDynamicFieldGroupQty());	///< �����ʵ� ���� ���ŵ��� ����


		// 2�� �÷��̾� ����
		pPlayer2->GetPlayerField().Gate(m_pFieldInfo->m_nFieldID, vCommonPos, vCommonDir);
		gsys.pServer->Update(0.1f);
		pPlayer2->GetPlayerField().GetFieldEntry().OnEnterFieldComplete();
		CHECK_EQUAL(pPlayer2->GetFieldUID(), m_pTestSharedField1->GetUID());

		gsys.pServer->Update(0.1f);
		CHECK_EQUAL(0, gmgr.pFieldMgr->GetDynamicFieldGroupQty());	///< �����ʵ� ���ŵ�
	}

	TEST_FIXTURE(FFieldDynamic, LeaveDynamicField_ReturnSameChannel)
	{
		/// Fixture ��ġ : �÷��̾� �����ʵ�� ����.
		GSharedField* pTestSharedField2 = GUTHelper_Field::NewSharedField(m_pFieldInfo, 2);
		GEntityPlayer* pPlayer2 = GUTHelper::NewEntityPlayer(pTestSharedField2, vCommonPos);

		CHECK_EQUAL(2, GetChannelID(pTestSharedField2)); ///< 2ä��

		pPlayer2->GetPlayerField().GateToTrial(TEST_FIELD_GROUP_ID); ///< �����ʵ� ����
		gsys.pServer->Update(0.1f);
		pPlayer2->GetPlayerField().GetFieldEntry().OnEnterFieldComplete();

		/// SUT ���� : �÷��̾� �����ʵ� ����.
		pPlayer2->GetPlayerField().GetFieldDynamic().GateToEnterPos(TEST_FIRST_DYNAMIC_FIELD_ID);
		gsys.pServer->Update(0.1f);
		pPlayer2->GetPlayerField().GetFieldEntry().OnEnterFieldComplete();

		/// ����
		CHECK_EQUAL(pPlayer2->GetFieldUID(), pTestSharedField2->GetUID());
	}

}