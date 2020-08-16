#include "stdafx.h"
#include "GHateTable.h"
#include "GEntitySync.h"
#include "GEntityNPC.h"
#include "GEntityPlayer.h"
#include "GField.h"
#include "GConst.h"
#include "GGlobal.h"
#include "GTalentInfoMgr.h"
#include "GPartySystem.h"
#include "GConst.h"
#include "GPlayerObjectManager.h"



//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

GHateTable::GHateTable(GEntityNPC* pOwner)
: m_pOwner(pOwner)
, m_Swapper(pOwner, m_mapHateTable)
, m_bEnableTimeout(true)
, m_fExpiredTime(0.0f)
{
	m_rgrTick.SetTickTime(1.0f);
	m_rgrTick.Start();

	m_pOwner->AttachObserver(this);
}

GHateTable::~GHateTable()
{
	Reset();
	m_pOwner->DetachObserver(this);
}

size_t GHateTable::GetQty() const
{
	return m_mapHateTable.size();
}

bool GHateTable::IsEmpty() const
{
	return m_mapHateTable.empty();
}

void GHateTable::Init(float fExpiredTime)
{
	m_fExpiredTime = fExpiredTime;
}

void GHateTable::Reset()
{
	m_uidLastTarget.SetInvalid();

	GVectorMUID vecHateUID;
	for (HatePointMap::iterator itor = m_mapHateTable.begin(); itor != m_mapHateTable.end(); ++itor)
	{
		MUID id = (*itor).first;
		vecHateUID.Vector().push_back(id);
	}

	for each (const MUID& nHateUID in vecHateUID.Vector())
	{
		Drop(nHateUID);
	}
}

MUID GHateTable::GetTarget() const
{
	// ����Ʈ ���� ���
	if (m_TargetLocker.IsLock())
	{
		return m_TargetLocker.GetTarget();
	}

	// ����Ʈ ������ ���
	if (m_Swapper.HasAppled())
	{
		return m_Swapper.GetTargetByEvent();
	}

	// �⺻ ���̺� ���
	return m_uidLastTarget;
}

void GHateTable::GetHighestTargetsUID(int nQty, vector<MUID>& vecTargets) const
{
	// TODO: ���� �ӵ� ����
	vecTargets.clear();
	if (m_mapHateTable.empty())	
		return;

	HatePoint_MAP_REVERSE mapScore;
	for (HatePointMap::const_iterator iter = m_mapHateTable.begin();
		iter != m_mapHateTable.end();)
	{
		MUID uid = iter->first;
		HatePoint nScore = iter->second;
		mapScore.insert(HatePoint_MAP_REVERSE::value_type(nScore, uid));
		++iter;
	}

	if (mapScore.empty())	return;

	HatePoint_MAP_REVERSE::iterator iter = mapScore.end();
	for (size_t i=0; i<min((size_t)nQty, mapScore.size()); i++)
	{
		iter--;
		vecTargets.push_back(iter->second);
	}
}

void GHateTable::GetHighestTargets( int nQty, vector<pair<MUID,HatePoint>>& vecTargets ) const
{
	// TODO: ���� �ӵ� ����
	vecTargets.clear();
	if (m_mapHateTable.empty())	
		return;

	HatePoint_MAP_REVERSE mapScore;
	for (HatePointMap::const_iterator iter = m_mapHateTable.begin();
		iter != m_mapHateTable.end();)
	{
		MUID uid = iter->first;
		HatePoint nScore = iter->second;
		mapScore.insert(HatePoint_MAP_REVERSE::value_type(nScore, uid));
		++iter;
	}

	if (mapScore.empty())	return;

	HatePoint_MAP_REVERSE::iterator iter = mapScore.end();
	for (size_t i=0; i<min((size_t)nQty, mapScore.size()); i++)
	{
		iter--;
		vecTargets.push_back(make_pair(iter->second,iter->first));
	}
}

void GHateTable::AddPoint_FoundEnemy(GEntityActor* pActor)
{
	VALID(pActor);
	AdjHatePoint(pActor, max(1, m_pOwner->GetHP()/10));
}

void GHateTable::AddPoint_GainDamage( GEntityActor* pActor, int nDamage, float fHateFactor )
{	
	VALID(pActor);
	AdjHatePoint(pActor, HatePoint(nDamage * fHateFactor));
}

void GHateTable::AddPoint_EnemyApplyEffect(GEntityActor* pActor, int nHateAdj)
{
	// �ŷ�Ʈ, ���� ȿ�� �߻� �� ����
	VALID(pActor);
	AdjHatePoint(pActor, nHateAdj);
}

void GHateTable::AddPoint_EnemyRecoverHP( GEntityActor* pActor, int nRecoveredHP, float fHateFactor )
{
	VALID(pActor);
	AdjHatePoint(pActor, HatePoint(nRecoveredHP * fHateFactor * 0.5f));
}

void GHateTable::AddPoint_Custom( GEntityActor* pActor, int nAmount)
{
	VALID(pActor);
	AdjHatePoint(pActor, HatePoint(nAmount));
}


void GHateTable::DropPoint( MUID id )
{
	if (!IsExist(id))
		return; // ���̺� ���� ���

	Drop(id);

	UpdateHateRanking();
}

void GHateTable::AdjHatePoint(GEntityActor* pActor, HatePoint point)
{
	if (m_pOwner->IsDead())
		return; // �̹� �׾���

	VALID(pActor);
	MUID id = pActor->GetUID();

	if (id == m_pOwner->GetUID())
		return; // �ڱ��ڽ�

	if (point == 0)
		return; // ����Ʈ ����Ʈ�� ����

	HatePointMap::iterator iter =
		m_mapHateTable.find(id);
	bool bIsEntry = 
		iter != m_mapHateTable.end();

	// NPC�� ����Ǿ� �����
	if (pActor->IsNPC())
	{
		float fHateMod = 0.5f;
		if (m_pOwner->GetNPCInfo())
			fHateMod = m_pOwner->GetNPCInfo()->m_fHateModifierForNPC;

		point = int((float)point * fHateMod);

		if (!bIsEntry)
		{
			point = max(point, 1);
		}
	}

	if (!bIsEntry)
	{
		InsertEntry(id, point);

		// ���� ������ �̺�Ʈ
		pActor->OnAddHateTarget(m_pOwner);
		m_pOwner->OnAddHateEntry(pActor);
	}
	else
	{
		DCHECK(m_mapTimeout.find(id) != m_mapTimeout.end());
		HatePoint& each_point = (*iter).second;
		each_point = max(each_point+point, 1);
		m_mapTimeout[id] = 0.0f;
	}

	// ����Ʈ ��ŷ ����
	UpdateHateRanking();
}

bool GHateTable::IsValidTarget(MUID id) const
{
	if (id.IsInvalid())
		return false; // ��ȿ���� ���� UID

	HatePointMap::const_iterator iter = m_mapHateTable.find(id);
	if (iter == m_mapHateTable.end())
		return false; // ����Ʈ ���̺� �������� ����
	
	GField* pField = m_pOwner->GetField();
	if (!pField)
		return false; // �ʵ尡 �������� ����

	GEntityActor* pTargetActor = 
		pField->FindActor(iter->first);

	if (!pTargetActor) 
		return false; // ��ǥ ���͸� ã�� �� ����
	if (pTargetActor->IsDead()) 
		return false; // ��ǥ�� �׾�����

	const HatePoint& nHate = iter->second;
	return (nHate > 0); // ����Ʈ�� ���ġ ����
}

bool GHateTable::IsExist(const MUID& id) const
{
	HatePointMap::const_iterator itor = m_mapHateTable.find(id);
	if (m_mapHateTable.end() == itor) return false;

	return true;
}

HatePoint GHateTable::GetHate(const MUID& id) const
{
	HatePointMap::const_iterator itor = m_mapHateTable.find(id);
	if (m_mapHateTable.end() == itor) return 0;

	return (*itor).second;
}

float GHateTable::GetTimeoutTime( const MUID& id ) const
{
	HateTimeoutMap::const_iterator itor = m_mapTimeout.find(id);
	if (m_mapTimeout.end() == itor) return -1.0f;

	return (*itor).second;
}

GEntityActor* GHateTable::FindActor( const MUID& id )
{
	GField* pField = m_pOwner->GetField();
	if (!pField)
		return NULL;

	return pField->FindActor(id);
}

void GHateTable::Update( float fDelta )
{	
	PFC_THISFUNC;
	PFI_BLOCK_THISFUNC(4006);
	
	if (m_pOwner->IsDead())
	{
		Reset();
		return;
	}

	float fElapsedTime = m_rgrTick.GetElapsedTime();
	if (!m_rgrTick.IsReady(fDelta))
		return; // �ֱⰡ ���� ����

	fElapsedTime += fDelta;

	m_TargetLocker.Update(fElapsedTime);
	m_Swapper.Update(fElapsedTime);
	
	if (m_bEnableTimeout &&
		!m_pOwner->IsNowUsingTalent()) // �ŷ�Ʈ ������϶��� �������� ����
	{
		UpdateTimeout(fElapsedTime);
	}
}

bool GHateTable::ApplyEvent( HATE_TARGET_SWAP_EVENT nEventType, float fDuration )
{
	if (!m_Swapper.ApplyEvent(nEventType, fDuration))
		return false;

	UpdateHateRanking();

	return true;
}

void GHateTable::Lock( MUID uidTarget, float fDurationTime )
{
	m_TargetLocker.Lock(uidTarget, fDurationTime);
}

void GHateTable::SetTauntable( bool bEnable )
{
	m_TargetLocker.Enable(bEnable);
}

bool GHateTable::IsTauntable()
{
	return m_TargetLocker.IsEnable();
}

void GHateTable::EnableTimeout( bool bSwitch )
{
	m_bEnableTimeout = bSwitch;
}

void GHateTable::InsertEntry(MUID uid, HatePoint point)
{
	m_mapHateTable.insert(std::make_pair(uid, max(point, 1)));
	m_mapTimeout.insert(std::make_pair(uid, 0.0f));
}

void GHateTable::DeleteEntity( const MUID& uid )
{
	m_mapHateTable.erase(uid);
	m_mapTimeout.erase(uid);
}

void GHateTable::UpdateTimeout(float fElapsedTime)
{
	GVectorMUID vecInvalidTargetUID;

	size_t nOldEntryQty = m_mapTimeout.size();	
	for (HateTimeoutMap::iterator iter = m_mapTimeout.begin(); iter != m_mapTimeout.end(); iter++)
	{
		MUID   uidEach		= (*iter).first;
		float& nTimeout		= (*iter).second;

		nTimeout += fElapsedTime;

		if (nTimeout >= GetExpiredTime())
		{
			vecInvalidTargetUID.Vector().push_back(uidEach);
		}

	}
	for each (const MUID& nInvalidTargetUID in vecInvalidTargetUID.Vector())
	{
		Drop(nInvalidTargetUID);
	}

	// ��Ʈ���� ������ ���� ��, ���� ����
	if (nOldEntryQty != m_mapTimeout.size())
	{
		UpdateHateRanking();
	}
}

void GHateTable::Drop(const MUID& uid)
{
	DeleteEntity(uid);		

	// Hate�� ���ŵǴ� ����� ã���� ������, �ڽſ��Դ� �˷��� �Ѵ�.
	m_pOwner->OnLostHateEntry(uid);

	GEntityActor* pEachActor = FindActor(uid); 
	if (!pEachActor) return;

	pEachActor->OnLostHateTarget(m_pOwner->GetUID());

	if (IsEmpty())
	{
		m_pOwner->OnEmptyHateTable();
	}
}


void GHateTable::UpdateHateRanking()
{
	m_uidLastTarget.SetInvalid();

	if (m_mapHateTable.empty())	
		return; // ���̺��� �������

	HatePoint fHighistScore=0;
	GVectorMUID vecInvalidUID;
	for (HatePointMap::iterator iter = m_mapHateTable.begin(); iter != m_mapHateTable.end(); iter++)
	{
		MUID	  uidEach		= (*iter).first;
		HatePoint nEachScore	= (*iter).second;

		if (!IsValidTarget(uidEach))
		{
			vecInvalidUID.Vector().push_back(uidEach);
		}
		else if (fHighistScore <= nEachScore)
		{
			fHighistScore = nEachScore;
			m_uidLastTarget = uidEach;
		}
	}

	for each (const MUID& nInvalidUID in vecInvalidUID.Vector())
	{
		Drop(nInvalidUID);
	}
}

void GHateTable::OnCombatBegin(GEntityActor* pOwner)
{
}

void GHateTable::OnCombatEnd(GEntityActor* pOwner)
{
	// ���� ����
	Reset();
}

void GHateTable::OnDie()
{
	// ����
	Reset();
}

void GHateTable::OnDespawn()
{
	// ���� ��
	Reset();
}

void GHateTable::OnDestroy()
{
	// ���� ��ü �Ҹ��
	Reset();
}

float GHateTable::GetExpiredTime() const
{
	if (m_fExpiredTime != 0.0f)	
		return m_fExpiredTime;

	return GConst::HATETABLE_ENTRY_TIMEOUT;
}

void GHateTable::OnAddHateEntry( GEntityActor* pEnemy )
{
	if (false == pEnemy->IsPlayer()) return;

	GEntityPlayer* pEnemyPlayer = static_cast<GEntityPlayer*>(pEnemy);
	if (false == pEnemyPlayer->HasParty()) return;

	vec3 vPlayerPos = pEnemyPlayer->GetPos();

	GParty* pParty = gsys.pPartySystem->FindParty(pEnemyPlayer->GetPartyUID());
	if (NULL == pParty) return;

	for(partymember_iterator it = pParty->GetMemberBegin(); it != pParty->GetMemberEnd(); it++)
	{		
		MUID uidMember = it->first;

		HatePointMap::iterator iter = m_mapHateTable.find(uidMember);

		if (iter == m_mapHateTable.end())
		{
			// �ڽ��� ����
			if (uidMember == pEnemyPlayer->GetUID())	continue;

			// �ٸ� ���� ����
			GEntityPlayer* pMember = gmgr.pPlayerObjectManager->GetEntity(uidMember);
			if (pMember == NULL)	continue;

			// �ٸ� �ʵ� ����
			if (pEnemyPlayer->GetFieldUID() != pMember->GetFieldUID())	continue;

			// �������� �̻� ����
			if (!CheckHeight(pMember))	continue;

			// Attacker�� ��Ƽ�� Ž���Ÿ� �� ����
			if (GConst::HATETABLE_ENTRY_PARTY_SCAN_DISTANCE < pEnemyPlayer->DistanceTo(pMember))	continue;

			// HateTable ��Ʈ���� �߰�
			InsertEntry(pMember->GetUID(), 1);

			pMember->OnAddHateTarget(m_pOwner);
			m_pOwner->OnAddHateEntryMember(pMember);
		}		
	}
}

bool GHateTable::CheckHeight(GEntityPlayer* pEnemy)
{
	const GFieldInfo* pFieldInfo = m_pOwner->GetFieldInfo();
	VALID_RET(pFieldInfo != NULL, true);

	if (pFieldInfo->IsSectorHeightLimitField())
	{
		vec3 vOwnerPos = m_pOwner->GetPos();
		vec3 vEnemyPos = pEnemy->GetPos();

		return pFieldInfo->IsAllowSectorHeightLimit(vOwnerPos.z - vEnemyPos.z);
	}

	return true;
}

void GHateTable::CollectTarget( vector<MUID>& vecTargets ) const
{
	for each (const HatePointMap::value_type& val in m_mapHateTable)
	{
		MUID uidTarget = val.first;
		vecTargets.push_back(uidTarget);
	}
}

