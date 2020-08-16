#include "stdafx.h"
#include "XObjectManager.h"
#include "XMyPlayer.h"
#include "XNetPlayer.h"
#include "XNonPlayer.h"
#include "XUnitTestUtil.h"

SUITE(ObjectManager)
{
	class FObjectManager
	{
	public:
		FObjectManager()
		{

		}
		~FObjectManager()
		{

		}
		void CreateAndAddNetPlayer(MUID uidNew, UIID nNewUIID)
		{
			XObject* pObject = XObjectManager::CreateObject(XOBJ_NET_PLAYER, uidNew, nNewUIID);

			mgr.Add(pObject);
		}

		XObject* CreateAndAddNPC(MUID uidNew, UIID nNewUIID)
		{
			XObject* pObject = XObjectManager::CreateObject(XOBJ_NPC, uidNew, nNewUIID);

			mgr.Add(pObject);

			return pObject;
		}

		XObjectManager		mgr;
	};

	TEST_FIXTURE(FObjectManager, TestFindPlayer_UIID)
	{
		// 1�� UIID �÷��̾� �߰�
		CreateAndAddNetPlayer(XUnitTestUtil::NewMUID(), 1);
		XActor* pPlayer = mgr.FindActor_UIID(1);
		CHECK(pPlayer != NULL);
		CHECK_EQUAL(1, pPlayer->GetUIID());

		// 2�� UIID �÷��̾� �߰�
		CreateAndAddNetPlayer(XUnitTestUtil::NewMUID(), 2);
		XActor* pPlayer2 = mgr.FindActor_UIID(2);
		CHECK(pPlayer2 != NULL);
		CHECK_EQUAL(2, pPlayer2->GetUIID());

		// 2�� �÷��̾ ����
		mgr.Erase(pPlayer2);
		pPlayer2->Destroy();	
		delete pPlayer2;

		pPlayer2 = mgr.FindActor_UIID(2);
		CHECK(pPlayer2 == NULL);
	}

	TEST_FIXTURE(FObjectManager, TestFindPlayer_UIID_Invalid)
	{
		// Invalid UIID �÷��̾� �߰� - UIID�� 0�̹Ƿ� UIIDMap���� ���� �ʴ´�.
		UIID nUIID = 0;
		MUID uid = XUnitTestUtil::NewMUID();
		CreateAndAddNetPlayer(uid, nUIID);

		XActor* pPlayer = mgr.FindActor_UIID(nUIID);
		CHECK(pPlayer == NULL);

		// UIID�� 0�̾ uid�� �˻��ȴ�.
		pPlayer = mgr.FindPlayer(uid);
		CHECK(pPlayer != NULL);
		CHECK_EQUAL(uid, pPlayer->GetUID());
		CHECK_EQUAL(nUIID, pPlayer->GetUIID());
	}

	TEST_FIXTURE(FObjectManager, TestFindPlayer_UIID_Negative)
	{
		// 1�� UIID �÷��̾� �߰�
		CreateAndAddNetPlayer(XUnitTestUtil::NewMUID(), 1);
		XActor* pPlayer = mgr.FindActor_UIID(1);
		CHECK(pPlayer != NULL);
		CHECK_EQUAL(1, pPlayer->GetUIID());

		// 1��, 2�� UIID NPC �߰�
		CreateAndAddNPC(XUnitTestUtil::NewMUID(), 10001);
		CreateAndAddNPC(XUnitTestUtil::NewMUID(), 10002);

		// 2�� UIID�� �÷��̾ ã�Ƶ� ����.
		pPlayer = mgr.FindActor_UIID(2);
		CHECK(pPlayer == NULL);

		// 2�� UIID �÷��̾� �߰�
		CreateAndAddNetPlayer(XUnitTestUtil::NewMUID(), 2);
		XActor* pPlayer2 = mgr.FindActor_UIID(2);
		CHECK(pPlayer2 != NULL);
		CHECK_EQUAL(2, pPlayer2->GetUIID());
	}


	TEST_FIXTURE(FObjectManager, TestDestroyAllObject)
	{
		CreateAndAddNPC(XUnitTestUtil::NewMUID(), 1);
		CreateAndAddNPC(XUnitTestUtil::NewMUID(), 2);
		CreateAndAddNPC(XUnitTestUtil::NewMUID(), 3);

		CreateAndAddNetPlayer(XUnitTestUtil::NewMUID(), 11);
		CreateAndAddNetPlayer(XUnitTestUtil::NewMUID(), 12);

		CHECK_EQUAL(5, mgr.GetCount());

		mgr.DestroyAllObject();

		CHECK_EQUAL(0, mgr.GetCount());
	}

	TEST_FIXTURE(FObjectManager, TestDestroyAllObject_ExceptUID)
	{
		CreateAndAddNPC(XUnitTestUtil::NewMUID(), 1);
		CreateAndAddNPC(XUnitTestUtil::NewMUID(), 2);
		CreateAndAddNPC(XUnitTestUtil::NewMUID(), 3);

		CreateAndAddNetPlayer(XUnitTestUtil::NewMUID(), 11);
		CreateAndAddNetPlayer(XUnitTestUtil::NewMUID(), 12);

		MUID uidExcept = XUnitTestUtil::NewMUID();
		CreateAndAddNetPlayer(uidExcept, 13);

		CHECK_EQUAL(6, mgr.GetCount());

		mgr.DestroyAllObject(uidExcept);

		CHECK_EQUAL(1, mgr.GetCount());

		XObject* pExceptObject = mgr.Find(uidExcept);
		CHECK(pExceptObject != NULL);

		CHECK_EQUAL(uidExcept, pExceptObject->GetUID());

		XActor* pExceptPlayer = mgr.FindActor_UIID(13);
		CHECK(pExceptPlayer != NULL);
		CHECK_EQUAL(13, pExceptPlayer->GetUIID());
	}

	TEST_FIXTURE(FObjectManager, TestObjectManager_ReserveDelete)
	{
		MUID uid = XUnitTestUtil::NewMUID();
		UIID nUIID = 1;

		CreateAndAddNPC(uid, nUIID);

		CHECK_EQUAL(1, mgr.GetCount());
		CHECK_EQUAL(0, mgr.GetReservedDelObjectCount());

		// 1���� ����
		mgr.ReserveDelete(uid, 1.0f);

		mgr.Update(0.5f);
		CHECK_EQUAL(1, mgr.GetCount());
		CHECK_EQUAL(1, mgr.GetReservedDelObjectCount());

		mgr.Update(0.3f);
		CHECK_EQUAL(1, mgr.GetCount());
		CHECK_EQUAL(1, mgr.GetReservedDelObjectCount());

		// 1�ʰ� �������� ������
		mgr.Update(1.0f);
		CHECK_EQUAL(0, mgr.GetCount());
		CHECK_EQUAL(0, mgr.GetReservedDelObjectCount());

	}

	TEST_FIXTURE(FObjectManager, TestObjectManager_CancelReserveDelete)
	{
		const MUID uid = XUnitTestUtil::NewMUID();
		const UIID nUIID = 1;

		CreateAndAddNPC(uid, nUIID);

		CHECK(mgr.Find(uid) != NULL);
		CHECK(mgr.FindActor_UIID(nUIID) != NULL);

		// 1���� ����
		mgr.ReserveDelete(uid, 1.0f);

		mgr.Update(0.5f);
		CHECK_EQUAL(1, mgr.GetCount());
		CHECK_EQUAL(1, mgr.GetReservedDelObjectCount());

		CHECK(mgr.Find(uid) != NULL);

		// ���� �����ϸ� UIID�� ��ĥ �� �����Ƿ� �����.
		CHECK(mgr.FindActor_UIID(nUIID) == NULL);

		// ���� ���
		mgr.CancelReserveDelete(uid);

		CHECK_EQUAL(1, mgr.GetCount());
		CHECK_EQUAL(0, mgr.GetReservedDelObjectCount());

		CHECK(mgr.Find(uid) != NULL);
		CHECK(mgr.FindActor_UIID(nUIID) != NULL);

		mgr.Update(1.0f);
		CHECK_EQUAL(1, mgr.GetCount());
	}


	TEST_FIXTURE(FObjectManager, TestObjectManager_ReserveDelete_UIID)
	{
		const MUID uid1 = XUnitTestUtil::NewMUID();
		const MUID uid2 = XUnitTestUtil::NewMUID();

		const UIID nUIID = 1;

		CreateAndAddNPC(uid1, nUIID);

		CHECK(mgr.Find(uid1) != NULL);
		CHECK(mgr.FindActor_UIID(nUIID) != NULL);

		// 5���� ����
		mgr.ReserveDelete(uid1, 5.0f);

		// �������� npc�� �̹� �����Ǿ���, �ٽ� ���� UIID�� �ٸ� npc�� �����Ǿ���.
		CreateAndAddNPC(uid2, nUIID);

		mgr.Update(1.0f);
		CHECK_EQUAL(2, mgr.GetCount());
		CHECK_EQUAL(1, mgr.GetReservedDelObjectCount());

		// uiid�� ã���� �ι�° NPC�� ã������ �Ѵ�.
		XActor* pActor2 = mgr.FindActor_UIID(nUIID);
		CHECK_EQUAL(uid2, pActor2->GetUID());

		// �ð��� �귯 ù��° NPC ���� ����
		mgr.Update(10.0f);
		CHECK_EQUAL(1, mgr.GetCount());
		CHECK_EQUAL(0, mgr.GetReservedDelObjectCount());

		// ������ uiid�� ã���� �ι�° NPC�� ã������ �Ѵ�.
		pActor2 = mgr.FindActor_UIID(nUIID);
		CHECK_EQUAL(uid2, pActor2->GetUID());
	}


	TEST_FIXTURE(FObjectManager, TestObjectManager_ReserveDelete_Cancel)
	{
		const MUID uid1 = XUnitTestUtil::NewMUID();
		const UIID nUIID = 1;

		XObject* pObj = CreateAndAddNPC(uid1, nUIID);

		// 5���� ����
		mgr.ReserveDelete(uid1, 5.0f);

		// UpdateCashe�Ǹ鼭 ��������.
		mgr.Erase(pObj);
		pObj->Destroy();
		pObj->Drop();

		// �ٽ� Add
		pObj = CreateAndAddNPC(uid1, nUIID);

		// 5���� Reserve�� ���� ��ҵǾ�� �Ѵ�.
		mgr.Update(1.0f);
		mgr.Update(5.0f);
		mgr.Update(1.0f);


		XActor* pActor = mgr.FindActor_UIID(nUIID);
		CHECK(pActor != NULL);

		CHECK(pActor->GetUID() == uid1);
		CHECK(pActor->GetUIID() == nUIID);

		CHECK((unsigned int)pActor->GetModuleEntity() != NULL);
	}

}