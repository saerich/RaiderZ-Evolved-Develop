#include "StdAfx.h"
#include "GDynamicFieldPolicy_TrialField.h"
#include "GDynamicFieldMaster.h"
#include "GDynamicField.h"
#include "GConst.h"
#include "GTrialField.h"
#include "GEntityPlayer.h"
#include "GSystem.h"
#include "GGlobal.h"
#include "GPlayerQuests.h"
#include "GPlayerQuest.h"
#include "GPartySystem.h"
#include "GPlayerFieldDynamic.h"
#include "GQuestInfoMgr.h"

GDynamicFieldPolicy_TrialField::GDynamicFieldPolicy_TrialField(GDynamicFieldMaster* pOwner)
: GDynamicFieldPolicy(pOwner)
, m_bTrialQuestCompleted(false)
, m_nExpiredTime(0)
{
}

GDynamicFieldPolicy_TrialField::~GDynamicFieldPolicy_TrialField()
{
}

void GDynamicFieldPolicy_TrialField::OnUpdate(float fDelta)
{
	if (CheckBoundary()) return;
	if (CheckTrialQuestCompleted())	return;
	if (CheckExpiredCount()) return;
	if (CheckQuestRunner()) return;
}

bool GDynamicFieldPolicy_TrialField::OnEnterPlayer(GEntityPlayer* pPlayer)
{
	_ASSERT(pPlayer != NULL);

	// ����Ʈ ���� ���
	vector<int> vecQuestID;
	pPlayer->GetQuests().MakeActiveQuestID(&vecQuestID);
	
	if (IsMatchedQuestRunner(pPlayer->GetUID(), &vecQuestID))
	{
		InsertQuestRunner(pPlayer->GetUID(), pPlayer->GetFieldUID());
	}


	// ��Ƽ ���
	if (gsys.pPartySystem->IsPartyMember(pPlayer))
	{
		if (m_uidOwnParty.IsInvalid())
		{
			// ó����ϵ� ��Ƽ
			m_uidOwnParty = pPlayer->GetPartyUID();
		}
		else
		{
			// �ٸ� ��Ƽ�� ���� �� ����
			VALID_RET(m_uidOwnParty == pPlayer->GetPartyUID(), false);
		}		
	}	

	return true;
}

void GDynamicFieldPolicy_TrialField::OnLeavePlayer(GEntityPlayer* pPlayer)
{
	assert(pPlayer);
	if (!pPlayer)	return;

	OnQuestEraseRunner(pPlayer->GetUID());
}

void GDynamicFieldPolicy_TrialField::OnDestroy()
{
	gsys.pPartySystem->RemoveFieldSelf(m_uidOwnParty, m_pOwner->GetGroupID());
}

bool GDynamicFieldPolicy_TrialField::CheckQuestRunner()
{
	if (m_pOwner->HasPreservedEnterPlayers())
	{
		//		mlog("GDynamicFieldGroup::CheckQuestRunner() - HasPreservedEnterPlayers\n");
		return false;
	}

	if (HasQuestRunner())
	{
		//		mlog("GDynamicFieldGroup::CheckQuestRunner() - !m_vecQuestRunner.empty()\n");
		return false;
	}

	if (IsRunForTest())
	{
		mlog_filter("mizelan", "GDynamicFieldGroup::CheckQuestRunner() - Delete me!\n");
	}

	BeginExpiredCount(GConst::DYNAMICFIELD_EXPIRED_TIME);

	return true;
}

void GDynamicFieldPolicy_TrialField::BeginExpiredCount(int nExpiredTime)
{
	m_nExpiredTime = gsys.pSystem->GetNowTime() + nExpiredTime*1000;

	if (IsRunForTest())
	{
		mlog_filter("mizelan", "GDynamicFieldGroup::StartExpiredCount - %u/%u\n", m_nExpiredTime, gsys.pSystem->GetNowTime());
	}

	for each (const MAP_DYNAMICFIELD::value_type& data in m_pOwner->m_mapFieldByID)
	{
		GTrialField* pTrialField = (GTrialField*) (data.second);
		pTrialField->StartExpiredCount(m_nExpiredTime);
	}
}

bool GDynamicFieldPolicy_TrialField::CheckTrialQuestCompleted()
{
	if (!m_setQuestRunner.empty())	return false;
	if (!m_bTrialQuestCompleted)	return false;

	m_pOwner->RunSelfDestructor();

	return true;
}

bool GDynamicFieldPolicy_TrialField::CheckExpiredCount()
{
	if (!IsExpiredCounting())							return false;	// ī��Ʈ�� �ƴ�

	if (m_nExpiredTime > gsys.pSystem->GetNowTime())	return true;	// ���ð����� ����

	OnExpired();

	return true;
}

void GDynamicFieldPolicy_TrialField::OnExpired()
{
	m_pOwner->RunSelfDestructor();
}

void GDynamicFieldPolicy_TrialField::CancelExpiredCount()
{
	m_nExpiredTime = 0;

	for each (const MAP_DYNAMICFIELD::value_type& data in m_pOwner->m_mapFieldByID)
	{
		GTrialField* pTrialField = (GTrialField*) (data.second);
		pTrialField->CancelExpiredCount();
	}
}

bool GDynamicFieldPolicy_TrialField::IsMatchedQuestRunner(MUID uidPlayer, const vector<int>* pVecQuestID)
{
	VALID_RET(m_pOwner->m_pInfo, false);

	set<int>& setRunningQuest = m_pOwner->m_pInfo->setRunningQuest;

	for each(int nQuestID in *pVecQuestID)
	{
		set<int>::iterator it = setRunningQuest.find(nQuestID);
		if (it != setRunningQuest.end())
		{
			return true;
		}
	}

	return false;
}

void GDynamicFieldPolicy_TrialField::InsertQuestRunner(MUID uidPlayer, MUID uidField)
{
	if (!m_pOwner->IsPlayerExistInField(uidField))	return;

	m_setQuestRunner.insert(uidPlayer);
}

void GDynamicFieldPolicy_TrialField::EraseQuestRunner(MUID uidPlayer)
{
	m_setQuestRunner.erase(uidPlayer);
}

bool GDynamicFieldPolicy_TrialField::HasQuestRunner()
{
	return !m_setQuestRunner.empty();
}

void GDynamicFieldPolicy_TrialField::OnQuestSynch(MUID uidPlayer, MUID uidField, const vector<int>* pVecQuestID)
{
	if (IsMatchedQuestRunner(uidPlayer, pVecQuestID))
	{
		InsertQuestRunner(uidPlayer, uidField);
	}
	else
	{
		EraseQuestRunner(uidPlayer);
	}

	if (CheckExpiredCount() && HasQuestRunner())
	{
		CancelExpiredCount();
	}
}

void GDynamicFieldPolicy_TrialField::OnQuestComplete(GEntityPlayer* pPlayer, int nQuestId)
{
	if (m_pOwner->GetInfo()->setRunningQuest.find(nQuestId) == m_pOwner->GetInfo()->setRunningQuest.end())		return;		// �ʵ�׷�� ��� ���� Ʈ���̾� ����Ʈ

	m_bTrialQuestCompleted = true;	
}

void GDynamicFieldPolicy_TrialField::OnQuestEraseRunner(MUID uidPlayer)
{
	EraseQuestRunner(uidPlayer);
}

bool GDynamicFieldPolicy_TrialField::IsExpiredCounting(void)
{
	return m_nExpiredTime > 0;
}

void GDynamicFieldPolicy_TrialField::ChangePlayerUID(MUID uidOffline, MUID uidMember)
{
	set<MUID>::iterator it = m_setQuestRunner.find(uidOffline);
	
	if (it != m_setQuestRunner.end())
	{
		m_setQuestRunner.erase(it);
		m_setQuestRunner.insert(uidMember);
	}
}

bool GDynamicFieldPolicy_TrialField::CheckBoundary()
{
	for each (const MAP_DYNAMICFIELD::value_type& data in m_pOwner->m_mapFieldByID)
	{
		GTrialField* pTrialField = (GTrialField*) (data.second);
		if (pTrialField->IsExpired())
		{
			OnExpired(); // ��� �Ҹ�
			return true;
		}
	}

	return false;
}

FIELD_ENTER_FAIL_TYPE GDynamicFieldPolicy_TrialField::IsEnterable()
{
	if (IsChallengerQuestField() &&
		m_pOwner->IsNowCombat())
		return ENTERFAIL_PROCEED_COMBAT; // ������ ���ԺҰ�
	
	return __super::IsEnterable();
}

bool GDynamicFieldPolicy_TrialField::IsChallengerQuestField() const
{
	VALID_RET(m_pOwner, false);
	return m_pOwner->IsChallengerQuestField();
}
