#include "stdafx.h"
#include "FBaseGame.h"
#include "GNPCRewardOwner.h"
#include "GUnitTestUtil.h"
#include "GTestForward.h"
#include "GHateTable.h"



SUITE(NPCRewardOwner)
{
	struct Fixture : public FBaseGame
	{
		Fixture()
		{
			m_pNPC = NewNPC(m_pField);
			m_pMember = NewEntityPlayer(m_pField);
			m_pParty = test.party.CreateParty(m_pPlayer, m_pMember);
		}

		GEntityNPC*		m_pNPC;
		GEntityPlayer*	m_pMember;
		GParty*		   	m_pParty;
	};

	// �÷��̾ ��׷� ȹ���ϸ� �������� �ĺ��� ����ϰ�, ù����̸� �������ڷ� ����
	TEST_FIXTURE(Fixture, OnAddHateEntry)
	{
		m_pNPC->GetNPCRewardOwner().OnAddHateEntry(m_pPlayer);
	
		CHECK_EQUAL(true, m_pNPC->GetNPCRewardOwner().IsCandidate(m_pPlayer->GetCID()));
		CHECK_EQUAL(m_pPlayer->GetCID(), m_pNPC->GetNPCRewardOwner().GetRewardOwnerCID());	
	}

	// ���� ��׷θ� ȹ���� �÷��̾ ��� �������ڷ� ����
	TEST_FIXTURE(Fixture, OnAddHateEntry_MaintainRewardOwner)
	{
		m_pNPC->GetNPCRewardOwner().OnAddHateEntry(m_pPlayer);
		m_pNPC->GetNPCRewardOwner().OnAddHateEntry(m_pMember);

		CHECK_EQUAL(m_pPlayer->GetCID(), m_pNPC->GetNPCRewardOwner().GetRewardOwnerCID());	
	}

	// �÷��̾��� ��Ƽ ����� ��׷� ȹ���ϸ� �������� �ĺ��� ���
	TEST_FIXTURE(Fixture, OnAddHateEntryMember)
	{
		m_pNPC->GetNPCRewardOwner().OnAddHateEntryMember(m_pMember);

		CHECK_EQUAL(true, m_pNPC->GetNPCRewardOwner().IsCandidate(m_pMember->GetCID()));
	}	

	// ��Ƽ���� �Ϻθ� ��׷ΰ� ���ŵǸ� �������� �ĺ� ����
	TEST_FIXTURE(Fixture, OnLostHateEntry_PartPartyMember)
	{
		m_pNPC->GetHateTable().AddPoint_FoundEnemy(m_pPlayer);
		m_pNPC->GetHateTable().DropPoint(m_pPlayer->GetUID());

		CHECK_EQUAL(true, m_pNPC->GetNPCRewardOwner().IsCandidate(m_pPlayer->GetCID()));
		CHECK_EQUAL(true, m_pNPC->GetNPCRewardOwner().IsCandidate(m_pMember->GetCID()));
	}

	// ��Ƽ�� ����� ��׷ΰ� ���ŵǸ� �������� �ĺ����� ����
	TEST_FIXTURE(Fixture, OnLostHateEntry_AllPartyMember)
	{
		m_pNPC->GetHateTable().AddPoint_FoundEnemy(m_pPlayer);
		m_pNPC->GetHateTable().DropPoint(m_pPlayer->GetUID());
		m_pNPC->GetHateTable().DropPoint(m_pMember->GetUID());

		CHECK_EQUAL(false, m_pNPC->GetNPCRewardOwner().IsCandidate(m_pPlayer->GetCID()));
		CHECK_EQUAL(false, m_pNPC->GetNPCRewardOwner().IsCandidate(m_pMember->GetCID()));
	}

	// �������ڰ� ���� �Ǹ� ���� �������� �ĺ��� �������ڰ� �ȴ�.
	TEST_FIXTURE(Fixture, OnLostHateEntry_ChangeRewardOwner)
	{
		GEntityPlayer* pOther = NewEntityPlayer(m_pField);

		m_pNPC->GetHateTable().AddPoint_FoundEnemy(pOther);
		m_pNPC->GetHateTable().AddPoint_FoundEnemy(m_pPlayer);

		m_pNPC->GetHateTable().DropPoint(pOther->GetUID());

		int nRewardOwner = m_pNPC->GetNPCRewardOwner().GetRewardOwnerCID();
		CHECK(	m_pPlayer->GetCID() == nRewardOwner ||
				m_pMember->GetCID() == nRewardOwner);
	}

	// NPC�� ��׷� ���̺��� �ʱ�ȭ�Ǹ� �������� �ĺ� ��ϵ� �ʱ�ȭ
	TEST_FIXTURE(Fixture, OnEmptyHateTable)
	{
		m_pNPC->GetNPCRewardOwner().OnAddHateEntry(m_pPlayer);

		m_pNPC->GetNPCRewardOwner().OnEmptyHateTable();

		CHECK_EQUAL(true, m_pNPC->GetNPCRewardOwner().IsEmpty());
	}	
}
