#include "stdafx.h"
#include "ZPlayerManager.h"
#include "ZGameServerPlayerMap.h"

SUITE(PlayerManager)
{
	struct FPlayerManager
	{
		FPlayerManager()
		{
			m_nAID = 1;
			m_strUserID = L"TestUser";
			m_nCID = 1;
			m_nGameServerID = 1;
			m_uidPlayer = MUID(1);
			m_strName = L"TestPlayer";
			m_nGID = 1;
		}
		virtual ~FPlayerManager() {}

		ZPlayerManager	m_pmgr;

		AID				m_nAID;
		wstring			m_strUserID;
		CID				m_nCID;
		int				m_nGameServerID;
		MUID			m_uidPlayer;
		wstring			m_strName;
		int				m_nGID;

	};
	TEST_FIXTURE(FPlayerManager, TestPlayerManager_NewPlayer)
	{
		/// Fixture ��ġ
		ZPlayer* pPlayer = m_pmgr.NewPlayer(m_uidPlayer, m_nAID, m_strUserID);
		pPlayer->SetWorldInfo(m_nCID, m_nGameServerID, m_uidPlayer, m_strName, m_nGID);

		/// SUT ����
		m_pmgr.Add(pPlayer);

		/// ����
		CHECK_EQUAL(1, m_pmgr.GetObjectCount());
		CHECK(m_nCID == pPlayer->GetCID());
		CHECK(m_nGameServerID == pPlayer->GetGameServerID());
		CHECK(m_uidPlayer == pPlayer->GetUID());
	}

	TEST_FIXTURE(FPlayerManager, TestGameServerPlayerMap_Add)
	{
		/// Fixture ��ġ
		ZGameServerPlayerMap playerMap;

		ZPlayer* pPlayer = m_pmgr.NewPlayer(m_uidPlayer, m_nAID, m_strUserID);
		pPlayer->SetWorldInfo(m_nCID, m_nGameServerID, m_uidPlayer, m_strName, m_nGID);
		m_pmgr.Add(pPlayer);
		CHECK_EQUAL(1, pPlayer->GetReferenceCount());

		/// SUT ����
		playerMap.Add(pPlayer);	///< PlayerMap�� ���۷��� ī���͸� ������Ų��.

		/// ����
		CHECK_EQUAL(2, pPlayer->GetReferenceCount());
		CHECK_EQUAL(1, playerMap.GetObjectCount());
	}

}