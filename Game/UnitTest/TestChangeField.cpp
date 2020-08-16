#include "stdafx.h"
#include "XUnitTestHelper.h"
#include "XGamePlayState.h"
#include "XMyPlayer.h"
#include "FTestGame.h"
#include "XUnitTestHelper.h"
#include "XUnitTestUtil.h"
#include "XPost_Local.h"
#include "MockGameApp.h"
#include "CCommandResultTable.h"

SUITE(ChangeField)
{
	class FChangeField : public FTestGame
	{
	public:
		FChangeField() : FTestGame() 
		{
		}
		virtual ~FChangeField() {}
	};

	TEST_FIXTURE(FChangeField, TestNewUIID_WhenChangeChannelAndMoveServer)
	{
		XMyPlayer* pMyPlayer = GetMyPlayer();
		pMyPlayer->GetActor()->SetAnimationUpdated(true);

		global.app->ChangeFrame(PLAY_FRAME_ID);
		Update(0.01f);

		UIID nFirstUIID = XGetMyUIID();
		XObjectManager* pObjectManager = gg.omgr;

		CHECK(pObjectManager->FindActor_UIID(nFirstUIID) != NULL);


		// XGamePlayState::FinalGame_ForChangeField ���� �ʿ��� �κи� ������ - �����丵 �ʿ�
		// clear world object
		if (pMyPlayer)
		{
			pObjectManager->Erase(pMyPlayer);
		}
		pObjectManager->DestroyAllObject();


		MUID uidNew = XUnitTestUtil::NewMUID();
		UIID nNewUIID = 1001;

		// ���� �̵� ����
		m_pNetAgent->OnRecv(MC_COMM_RESPONSE_MOVE_GAME_SERVER, 3, NEW_INT(CR_SUCCESS), NEW_UID(uidNew), NEW_USHORT(nNewUIID));

		// XGamePlayState::InitGame_ForChangeField ���� �ʿ��� �κи� ������ - �����丵 �ʿ�
		// create user and init
		pObjectManager->Add(pMyPlayer);

		CHECK(pObjectManager->FindActor_UIID(nNewUIID) != NULL);
	}
}