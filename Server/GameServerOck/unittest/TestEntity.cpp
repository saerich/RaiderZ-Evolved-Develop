#include "stdafx.h"
#include "GEntityPlayer.h"
#include "GEntityNPC.h"
#include "SUnitTestUtil.h"
#include "GUTHelper.h"
#include "MockField.h"
#include "GPlayerObjectManager.h"
#include "GUTHelper_Field.h"

SUITE(Entity)
{
	TEST(EntityReferenceCounter)
	{
		GEntityMgr	entity_mgr;
		GEntityPlayer* pEntityPlayer = new GEntityPlayer();
		
		CHECK_EQUAL(pEntityPlayer->GetReferenceCount(), 1);

		pEntityPlayer->Create(SUnitTestUtil::NewUID());		
		
		// Player�� ��� EntityMgr���� PlayerObjectManager�� destroy�� �����Ѵ�.
		{
			MUID uidPlayer = pEntityPlayer->GetUID();
			MockPlayerObject* pPlayer = new MockPlayerObject(uidPlayer);
			pPlayer->Create(pEntityPlayer);
			gmgr.pPlayerObjectManager->AddPlayer(pPlayer);
		}
	
		entity_mgr.AddEntity(pEntityPlayer);

		CHECK_EQUAL(entity_mgr.GetEntityQty(ETID_PLAYER), 1);

		pEntityPlayer->AddRef();
		CHECK_EQUAL(pEntityPlayer->GetReferenceCount(), TFT_SIZE+2);

		pEntityPlayer->DeleteMe();

		// ���� mgr���� update�� �������� 2
		CHECK_EQUAL(pEntityPlayer->GetReferenceCount(), TFT_SIZE+2);

		entity_mgr.Update(1.0f);

		// update���� �ѹ� Drop�� ������ 1
		CHECK_EQUAL(pEntityPlayer->GetReferenceCount(), 1);

		// mgr���� �̹� ������.
		CHECK_EQUAL(entity_mgr.GetEntityQty(ETID_PLAYER), 0);

		// �� �������� ������ delete ��.
		pEntityPlayer->Drop();
	}

	TEST(NPCQtyInEntityMgr)
	{
		MockField* m_pField = GUTHelper_Field::NewMockField();

		GNPCInfo	infoNPC1;
		infoNPC1.nID = 1;
		GNPCInfo	infoNPC2;
		infoNPC2.nID = 2;

		CHECK_EQUAL(m_pField->GetNPCQty(1), 0);
		CHECK_EQUAL(m_pField->GetNPCQty(2), 0);

		GEntity* pFirst=NULL;
		GEntity* pSecond=NULL;
		{
			GEntityNPC* pNPC = m_pField->SpawnTestNPC(&infoNPC1);
			pFirst = pNPC;
		}

		CHECK_EQUAL(m_pField->GetNPCQty(1), 1);
		CHECK_EQUAL(m_pField->GetNPCQty(2), 0);

		{
			GEntityNPC* pNPC = m_pField->SpawnTestNPC(&infoNPC1);
			pSecond = pNPC;
		}
		
		CHECK_EQUAL(m_pField->GetNPCQty(1), 2);
		CHECK_EQUAL(m_pField->GetNPCQty(2), 0);

		{
			GEntityNPC* pNPC = m_pField->SpawnTestNPC(&infoNPC2);
		}

		CHECK_EQUAL(m_pField->GetNPCQty(1), 2);
		CHECK_EQUAL(m_pField->GetNPCQty(2), 1);
		
		{
			m_pField->RemoveEntity(pFirst);
		}

		CHECK_EQUAL(m_pField->GetNPCQty(1), 1);
		CHECK_EQUAL(m_pField->GetNPCQty(2), 1);

		{
			m_pField->RemoveEntity(pSecond);
		}

		CHECK_EQUAL(m_pField->GetNPCQty(1), 0);
		CHECK_EQUAL(m_pField->GetNPCQty(2), 1);

		pFirst->Destroy();
		pSecond->Destroy();
		SAFE_DELETE(pFirst);
		SAFE_DELETE(pSecond);
		m_pField->Destroy();
		SAFE_DELETE(m_pField);
	}

	TEST(IsBackDirection)
	{
		GEntity entity;
		vec3 vecTargetPos;
		
		entity.SetPos(vec3(0.0f, 0.0f, 0.0f));	// �߾ӿ� ����,
		entity.SetDir(vec3(1.0f, 0.0f, 0.0f));	// x������ �ٶ󺸴�.
			

		// ��
		vecTargetPos.Set(-1.0f, 0.0f, 0.0f);
		CHECK(entity.IsBack(vecTargetPos));
		
		// ��
		vecTargetPos.Set(1.f, 0.f, 0.f);
		CHECK(!entity.IsBack(vecTargetPos));

		// �� - ��Ȯ�� 90�� ���� ���ʿ� ����
		vecTargetPos.Set(0.f, 1.0f, 0.f);
		CHECK(!entity.IsBack(vecTargetPos));

		// �밢�� ��
		vecTargetPos.Set(-1.f, 1.0f, 0.f);
		CHECK(entity.IsBack(vecTargetPos));
	}
} // SUITE(AI)
