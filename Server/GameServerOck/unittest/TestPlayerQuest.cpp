#include "stdafx.h"
#include "FBaseGame.h"
#include "GTestForward.h"
#include "GQuestInfo.h"
#include "GPlayerQuest.h"



SUITE(PlayerQuest)
{
	struct Fixture : public FBaseGame
	{
		Fixture()
		{
			m_pQuestInfo = test.quest.NewQuestInfo();			
		}

		GTestMgrWrapper<GQuestInfoMgr>	m_QuestInfoMgr;		
		GQuestInfo*			m_pQuestInfo;
		GEntityPlayer*		m_pShareTarget;
	};

	TEST_FIXTURE(Fixture, FailByTimeExpired)
	{
		const int nTimeLimit = 2;
		GPlayerQuest* pPlayerQuest = test.quest.GiveNewPlayerQuest(m_pPlayer, m_pQuestInfo->nID, nTimeLimit);

		CHECK(NULL != pPlayerQuest);
		if (NULL == pPlayerQuest)	return;

		MUID uidDummyNPC;
		CHECK_EQUAL(pPlayerQuest->IsFail(), false);	// �������� �ʾ���
		gsys.pSystem->Sleep(1.0f);
		m_pField->Update(1.0f);
		CHECK_EQUAL(pPlayerQuest->IsFail(), false);	// �������� �ʾ���
		gsys.pSystem->Sleep(1.0f);
		m_pField->Update(1.0f);
		CHECK_EQUAL(pPlayerQuest->IsFail(), true);	// �ð��� ������ ����.
	}	
}