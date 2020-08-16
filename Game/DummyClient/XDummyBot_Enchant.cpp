#include "StdAfx.h"
#include "XDummyBot_Enchant.h"
#include "XBirdDummyAgent.h"
#include "XDummyLazyCommandPoster.h"

static vec3 vOutPoint(30000,30000,100);
static vec3 vInPoint(8957,5077,100);
static int nAutopartyQuestID = 999022;

XDummyBot_Enchant::XDummyBot_Enchant( XBirdDummyAgent* pAgent, XDummyAgentInfo* pAgentInfo )
: XDummyBot(pAgent, pAgentInfo)
, m_nPhase(PHASE_INIT)
{
	m_rgrTick.SetTickTime(0.5f);
	m_rgrTick.Start();
}

XDummyBot_Enchant::~XDummyBot_Enchant(void)
{
}

void XDummyBot_Enchant::OnRun(float fDelta)
{
	__super::OnRun(fDelta);

	if (!m_rgrTick.IsReady(fDelta))
		return;

	switch(m_nPhase)
	{
	case PHASE_INIT:
		m_nPhase = PHASE_PREPARE;
		XBIRDPOSTCMD2(m_pAgent->GetClient(), MC_GM_SET_ME_REQ, MCommandParameterWideString(L"grade"), MCommandParameterWideString(L"3"));
		XBIRDPOSTCMD2(m_pAgent->GetClient(), MC_GM_SET_ME_REQ, MCommandParameterWideString(L"level"), MCommandParameterWideString(L"99"));
	case PHASE_PREPARE:
		m_nPhase = PHASE_ENCHANT;
		XBIRDPOSTCMD0(m_pAgent->GetClient(), MC_GM_QUEST_RESET_ALL_REQ);
		XBIRDPOSTCMD0(m_pAgent->GetClient(), MC_GM_CLEAR_INVENTORY_REQ);
		MakeTestItem();
		break;
	case PHASE_ENCHANT:
		m_nPhase = PHASE_PREPARE;
		XBIRDPOSTCMD0(m_pAgent->GetClient(), MC_ENCHANT_PREPARE);
		XBIRDPOSTCMD4(m_pAgent->GetClient(), MC_ENCHANT_REQ, 
			MCommandParameterInt(SLOTTYPE_INVENTORY), 
			MCommandParameterInt(0), // ��ȭ�� ���
			MCommandParameterInt(1), // ��ȭ��
			MCommandParameterInt(3));// ��ȭ��
		XBIRDPOSTCMD0(m_pAgent->GetClient(), MC_ENCHANT_PREPARE);
		XBIRDPOSTCMD4(m_pAgent->GetClient(), MC_ENCHANT_REQ, 
			MCommandParameterInt(SLOTTYPE_INVENTORY), 
			MCommandParameterInt(0), // ��ȭ�� ���
			MCommandParameterInt(2), // ��ȭ��
			MCommandParameterInt(4));// ��ȭ��
		XBIRDPOSTCMD0(m_pAgent->GetClient(), MC_GM_CLEAR_INVENTORY_REQ);
		break;
	}
}

void XDummyBot_Enchant::MakeTestItem()
{		
	vector<int> vecItemID;
	vecItemID.push_back(70002);		// ������ ��� (�Ϲ� ���� 2��)
	vecItemID.push_back(30000000);		// ���Ǽ���
	vecItemID.push_back(30000000);		// ���Ǽ���
	vecItemID.push_back(39999004);		// Ư�� ��ȭ��
	vecItemID.push_back(39999004);		// Ư�� ��ȭ��
	
	for each (int nItemID in vecItemID)
	{
		XBIRDPOSTCMD2(m_pAgent->GetClient(), MC_GM_ITEM_GIVE_REQ, MCommandParameterInt(nItemID), MCommandParameterInt(1));
	}
}
