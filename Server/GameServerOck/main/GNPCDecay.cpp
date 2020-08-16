#include "StdAfx.h"
#include "GNPCDecay.h"
#include "MTime.h"
#include "GJobMgr.h"
#include "GEntityNPC.h"
#include "GModuleAI.h"
#include "GConst.h"
#include "GNPCLoot.h"
#include "GDropList.h"
#include "GGlobal.h"
#include "GTestSystem.h"
#include "GField.h"
#include "GSpawnManager.h"

GNPCDecay::GNPCDecay(GEntityNPC* pOwner)
: m_pOwner(pOwner)
, m_fCustomDecayTick(-1.0f)
{
	m_pDecayRegulator = new MRegulator();
}

GNPCDecay::~GNPCDecay()
{
	SAFE_DELETE(m_pDecayRegulator);
}

void GNPCDecay::Decay()
{
	VALID(m_pOwner->GetNPCInfo());

	float fKeepCorpseTime = m_pOwner->GetNPCInfo()->m_fKeepCorpseTime;
	if (fKeepCorpseTime == 0.0f)
	{
		// �������� ����
		m_pOwner->doDespawn(true);
		return;
	}

	if (fKeepCorpseTime != -1.0f)
	{
		// ������ �ð� ����
		SetCustomDecayTick(m_pOwner->GetNPCInfo()->m_fKeepCorpseTime);
	}
	

	if (true == m_pOwner->GetNPCLoot().GetDropList().IsEmpty())
	{
		_Decay(GConst::NPC_DECAY_TIME_NOITEM);
	}
	else
	{
		_Decay(GConst::NPC_DECAY_TIME_GRADE[m_pOwner->GetNPCInfo()->nGrade]);
	}

	// ��ü�� �ɶ� ���� �Ŵ������� ����
	RemoveToSpawnMgr();
}

void GNPCDecay::_Decay(float fDecayInterval)
{
	GJobMgr* pJobMgr = m_pOwner->GetModuleAI()->GetJobMgr();
	pJobMgr->Clear();

	m_pDecayRegulator->Start();		

	// ������ �ð��� ������ ������ �� �ð��� �����ش�.
	m_pDecayRegulator->SetTickTime(m_fCustomDecayTick>=0.0f?
										m_fCustomDecayTick:fDecayInterval);
}

void GNPCDecay::Update(float fDelta)
{
	if (false == m_pOwner->IsDead()) return;

	if (true == m_pDecayRegulator->IsReady(fDelta))
	{
		m_pOwner->doDespawn(true);
		gsys.pTestSystem->DespawnLog(m_pOwner->GetUID(), __FUNCTIONW__);
	}
}

void GNPCDecay::SetCustomDecayTick(const float fTick)
{
	m_fCustomDecayTick = fTick;
}

void GNPCDecay::RemoveToSpawnMgr()
{
	if (!m_pOwner->GetField())
		return;

	if (!m_pOwner->GetSpawnInfo())
		return; // ���� ������ ����

	m_pOwner->GetField()->GetSpawn()->Reset(m_pOwner->GetSpawnInfo()->nID);
}