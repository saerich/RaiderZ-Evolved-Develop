#include "stdafx.h"
#include "FBaseGame.h"
#include "GExpSystem.h"
#include "GExpCalculator.h"
#include "GEntityPlayer.h"
#include "GFatigueSystem.h"
#include "CSDef_Fatigue.h"

SUITE(ExpSystem)
{
	struct Fixture : FBaseGame
	{
		Fixture()
		{			
			m_pLink->ResetCommands();
			m_pLink->AddIgnoreID(MC_FIELD_IN_NPC);
			m_pLink->AddIgnoreID(MC_FIELD_IN_PLAYER);
			m_pLink->AddIgnoreID(MC_NPCINTERACTION_ICON);

			m_pNetAgent->ResetSendCommands();
			m_pNetAgent->IgnoreCmd(MMC_PLAYER_IN_FIELD_NOTIFY);			

			m_pNPC = NewNPC(m_pField);
			m_pPlayer2 = NewEntityPlayer(m_pField);			
		}

		void AddSameServerBeneficiary(GEntityPlayer* pPlayer)
		{
			m_vecSameServerBeneficiary.push_back(pPlayer);
		}

		void AddOtherServerBeneficiaryCID(int nCID)
		{
			m_vecOtherServerBeneficiaryCID.push_back(nCID);
		}

		GEntityNPC* m_pNPC;
		GEntityPlayer* m_pPlayer2;
		vector<GEntityPlayer*> m_vecSameServerBeneficiary;
		vector<int> m_vecOtherServerBeneficiaryCID;
		int m_nDivisionExp;
	};

	// NPC�� ���� ������ �����ڰ� �ϳ��� ���´� ����ġ�� ���޵� ��������.
	TEST_FIXTURE(Fixture, RewardExp_EmptySameServerBeneficiary)
	{
		CHECK_EQUAL(false, gsys.pExpSystem->RewardExpByNPCKill(m_vecSameServerBeneficiary, static_cast<int>(m_vecSameServerBeneficiary.size() + m_vecOtherServerBeneficiaryCID.size()), m_pNPC->GetNPCInfo()));
	}

	// ����ġ ȹ���� ������ NPC�� �ƴϸ� ����ġ�� ȹ����� �ʴ´�.
	TEST_FIXTURE(Fixture, RewardExp_NoGainExpNPC)
	{
		AddSameServerBeneficiary(m_pPlayer);

		m_pNPC->GetNPCInfo()->bGainEXP = false;
		CHECK_EQUAL(false, gsys.pExpSystem->RewardExpByNPCKill(m_vecSameServerBeneficiary, static_cast<int>(m_vecSameServerBeneficiary.size() + m_vecOtherServerBeneficiaryCID.size()), m_pNPC->GetNPCInfo()));
	}

	// ����ġ�� ������ ������ �޴´�.
	TEST_FIXTURE(Fixture, RewardExp_LevelFactor)
	{	
		m_pPlayer->GetPlayerInfo()->nLevel = 1;
		m_pPlayer2->GetPlayerInfo()->nLevel = 2;

		int nBeforeExp1 = m_pPlayer->GetPlayerInfo()->nXP;
		int nBeforeExp2 = m_pPlayer2->GetPlayerInfo()->nXP;

		AddSameServerBeneficiary(m_pPlayer);
		AddSameServerBeneficiary(m_pPlayer2);
		
		CHECK_EQUAL(true, gsys.pExpSystem->RewardExpByNPCKill(m_vecSameServerBeneficiary, static_cast<int>(m_vecSameServerBeneficiary.size() + m_vecOtherServerBeneficiaryCID.size()), m_pNPC->GetNPCInfo()));

		int nAddedExp1 = m_pPlayer->GetPlayerInfo()->nXP - nBeforeExp1;
		int nAddedExp2 = m_pPlayer2->GetPlayerInfo()->nXP - nBeforeExp2;

		float fLevelRate = static_cast<float>(m_pPlayer2->GetPlayerInfo()->nLevel) / static_cast<float>(m_pPlayer->GetPlayerInfo()->nLevel);
		float fAddExpRate = static_cast<float>(nAddedExp2) / static_cast<float>(nAddedExp1);

		CHECK_CLOSE(fLevelRate, fAddExpRate, 0.1f);
	}

	// ����ġ�� �Ƿε��� ������ �޴´�.
	TEST_FIXTURE(Fixture, RewardExp_FatigueFactor)
	{	
		m_pPlayer->GetPlayerInfo()->nFatigueQuantity = WORST_FATIGUE;
		m_pPlayer2->GetPlayerInfo()->nFatigueQuantity = NORMAL_FATIGUE;

		int nBeforeExp1 = m_pPlayer->GetPlayerInfo()->nXP;
		int nBeforeExp2 = m_pPlayer2->GetPlayerInfo()->nXP;

		AddSameServerBeneficiary(m_pPlayer);
		AddSameServerBeneficiary(m_pPlayer2);

		CHECK_EQUAL(true, gsys.pExpSystem->RewardExpByNPCKill(m_vecSameServerBeneficiary, static_cast<int>(m_vecSameServerBeneficiary.size() + m_vecOtherServerBeneficiaryCID.size()), m_pNPC->GetNPCInfo()));

		int nAddedExp1 = m_pPlayer->GetPlayerInfo()->nXP - nBeforeExp1;
		int nAddedExp2 = m_pPlayer2->GetPlayerInfo()->nXP - nBeforeExp2;

		float fFatigueRate = GExpCalculator::CalcFatigueFactor(NORMAL_FATIGUE) / GExpCalculator::CalcFatigueFactor(WORST_FATIGUE);
		float fAddExpRate = static_cast<float>(nAddedExp2) / static_cast<float>(nAddedExp1);

		CHECK_CLOSE(fFatigueRate, fAddExpRate, 0.1f);
	}

	// ����ġ�� �������� �ο��� ������ �޴´�.
	TEST_FIXTURE(Fixture, RewardExp_BeneficiaryCount)
	{			
		AddSameServerBeneficiary(m_pPlayer);
		int nBeforeSoloExp = m_pPlayer->GetPlayerInfo()->nXP;
		CHECK_EQUAL(true, gsys.pExpSystem->RewardExpByNPCKill(m_vecSameServerBeneficiary, static_cast<int>(m_vecSameServerBeneficiary.size() + m_vecOtherServerBeneficiaryCID.size()), m_pNPC->GetNPCInfo()));
		int nAddedSoloExp = m_pPlayer->GetPlayerInfo()->nXP - nBeforeSoloExp;

		AddSameServerBeneficiary(m_pPlayer2);
		int nBeforeDuoExp1 = m_pPlayer->GetPlayerInfo()->nXP;
		int nBeforeDuoExp2 = m_pPlayer2->GetPlayerInfo()->nXP;
		CHECK_EQUAL(true, gsys.pExpSystem->RewardExpByNPCKill(m_vecSameServerBeneficiary, static_cast<int>(m_vecSameServerBeneficiary.size() + m_vecOtherServerBeneficiaryCID.size()), m_pNPC->GetNPCInfo()));
		int nAddedDuoExp1 = m_pPlayer->GetPlayerInfo()->nXP - nBeforeDuoExp1;
		int nAddedDuoExp2 = m_pPlayer2->GetPlayerInfo()->nXP - nBeforeDuoExp2;

		float fBeneficiaryCountRate = GExpCalculator::CalcBeneficiaryCountFactor(2) / GExpCalculator::CalcBeneficiaryCountFactor(1);
		float fAddExpRate = static_cast<float>(nAddedDuoExp1 + nAddedDuoExp2) / static_cast<float>(nAddedSoloExp);

		CHECK_CLOSE(fBeneficiaryCountRate, fAddExpRate, 0.1f);
	}	

	TEST_FIXTURE(Fixture, AddExp)
	{
		int nAddExp = 100;
		int nBeforeExp = m_pPlayer->GetPlayerInfo()->nXP;

		gsys.pExpSystem->AddExp(m_pPlayer, nAddExp, 0);

		int nAfterExp = m_pPlayer->GetPlayerInfo()->nXP;
		float fExpPercent = GExpCalculator::CalcExpPercent(m_pPlayer->GetPlayerInfo()->nXP, m_pPlayer->GetPlayerInfo()->nLevel);

		CHECK_EQUAL(nAfterExp, nBeforeExp + nAddExp);

		CHECK_EQUAL(1, m_pLink->GetCommandCount());
		CHECK_EQUAL(MC_CHAR_REWARD_EXP, m_pLink->GetCommandID(0));		
		CHECK_EQUAL(nAddExp, m_pLink->GetParam<int>(0, 0));
		CHECK_EQUAL(fExpPercent, m_pLink->GetParam<float>(0, 1));
		CHECK_EQUAL(nAfterExp, m_pLink->GetParam<int>(0, 2));
	}	
}