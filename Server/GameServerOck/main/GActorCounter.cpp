#include "StdAfx.h"
#include "GActorCounter.h"
#include "GEntityActor.h"
#include "GTalentEffector.h"

GActorCounter::GActorCounter( GEntityActor* pOwner )
: m_pOwner(pOwner)
, m_pUsingTalentInfo(NULL)
, m_bImmunedTalent(false)
{
	m_pOwner->AttachObserver(this);
}

GActorCounter::~GActorCounter(void)
{
	m_pOwner->DetachObserver(this);
}

void GActorCounter::Clear()
{
	m_pUsingTalentInfo = NULL;
	m_bImmunedTalent = false;
}

bool GActorCounter::IsCounter() const
{
	if (!m_pUsingTalentInfo)
		return false;

	if (!m_bImmunedTalent)
		return false;

	return true;
}

void GActorCounter::OnUseTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo )
{
//	DCHECK(!m_pUsingTalentInfo && L"�ŷ�Ʈ�� ������ �ʾҴµ� �� ����");
	m_pUsingTalentInfo = pTalentInfo;
}

void GActorCounter::OnFinishTalent( GTalentInfo* pTalentInfo )
{
	DCHECK(m_pUsingTalentInfo == pTalentInfo && L"���۵� ���� �ٸ� �ŷ�Ʈ�� �Ϸ�Ǿ���");
	Clear();
}

void GActorCounter::OnTalentImmuned( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo )
{
	m_bImmunedTalent = true;
}

void GActorCounter::OnTalentAvoid( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo )
{
	m_bImmunedTalent = true;
}

void GActorCounter::OnHitEnemy( GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pTarget, GTalentInfo* pTalentInfo )
{
	if (!IsCounter())
		return;	// ī���� �غ� ���� ����

	if (m_pUsingTalentInfo != pTalentInfo)
		return;	// �ٸ� �ŷ�Ʈ�� ī���͸� ���� �� ����

	// ī���� ����
	GTalentEffector::Info info;
	info.pTalentInfo = m_pUsingTalentInfo;
	info.pUser = m_pOwner;
	info.pTarget = pTarget;
	info.nTalentCondition = TC_COUNTER;
	GTalentEffector effector;
	effector.ApplyTalent(info);
}

