#include "StdAfx.h"
#include "GPlayerRide.h"
#include "GEntityPlayer.h"
#include "GGlobal.h"
#include "GBuffInfo.h"
#include "GModuleBuff.h"
#include "GConst.h"
#include "GCalculator.h"

GPlayerRide::GPlayerRide(GEntityPlayer* pOwner)
: m_pOwner(pOwner)
, m_pRandomHandler(NULL)
, m_pMountBuff(NULL)
{
	m_pRandomHandler = new RandomDismountHandler();
	m_pOwner->AttachObserver(this);
}

GPlayerRide::~GPlayerRide(void)
{
	m_pOwner->DetachObserver(this);
	SAFE_DELETE(m_pRandomHandler);
}

void GPlayerRide::OnCombatBegin( GEntityActor* pOwner )
{
	HandleDismountEvent("begin_combat");
}

void GPlayerRide::OnHit( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo )
{
	HandleDismountEvent("hit_from_enemy", pAttacker);
}

void GPlayerRide::OnGainBuff( int nBuffID )
{
	if (IsRideBuff(nBuffID))
	{
		GBuffInfo* pBuff = gmgr.pBuffInfoMgr->Get(nBuffID);
		if (pBuff &&
			!m_pMountBuff)
			m_pMountBuff = pBuff;
	}
}

void GPlayerRide::OnLostBuff( int nBuffID )
{
	if (IsRideBuff(nBuffID))
	{
		m_pMountBuff = NULL;
	}
}

bool GPlayerRide::IsRideBuff( int nBuffID ) const
{
	GBuffInfo* pBuff = gmgr.pBuffInfoMgr->Get(nBuffID);
	if (!pBuff)
		return false;

	return pBuff->IsRideBuff();
}

bool GPlayerRide::IsMounted() const
{
	return m_pMountBuff != NULL;
}

void GPlayerRide::Dismount()
{
	m_pOwner->GetModuleBuff()->DismountRideBuff();
}

int GPlayerRide::GetTestMode() const
{
	if (!m_pMountBuff)
		return 0;

	return (int)m_pMountBuff->m_fPassiveExtraAttribValue;
}

void GPlayerRide::HandleDismountEvent(const string strEvent, GEntityActor* pAttacker)
{
	GPercentDice dice;

	switch (GetTestMode())
	{
	case 0: // �¾Ƶ� �ȳ���
		break;
	case 1: // ���� �߿��� �� ������ ���� �ӵ�
		break;
	case 2: // ���� �߻��� ������ ����
		if (strEvent == "begin_combat")
			Dismount();
		break;
	case 3: // �ǰݽ� �������Ϳ� �ٰ��� Ȯ���� ������ ����
		if (strEvent == "hit_from_enemy" && 
			dice.Dice(CalcDismountRate(pAttacker)))
		{
			Dismount();
		}
		break; 
	default:
		if (GetTestMode() >= 100 &&
			GetTestMode() < 200)
		{
			// ���� �߻��� x�� �ڿ� ����
			float fAfterCombatSec = (float)(GetTestMode()  - 100);
			GBuffInfo* pDismountBuff = gmgr.pBuffInfoMgr->Get(GConst::DISMOUNT_BUFFID);
			if (!pDismountBuff)
			{
				// ���� ������ ��ȿ���� ����
				Dismount();
			}

			CSBuffEnchantInfo BuffInvokeInfo;
			BuffInvokeInfo.nEnchantEvent = TC_BUFF_GAIN_N_PERIOD;
			BuffInvokeInfo.EffectInfo.m_nPoint = CSEffectInfo::POINT_SELF;
			m_pOwner->GainBuffDetail(GConst::DISMOUNT_BUFFID, fAfterCombatSec, 0.0f);
		}
	}
}

float GPlayerRide::CalcDismountRate( GEntityActor* pAttacker )
{
	return m_pRandomHandler->CalcDismountRate(pAttacker, m_pOwner);
}

void GPlayerRide::SetRandomHandler( RandomDismountHandler* pHandler )
{
	SAFE_DELETE(m_pRandomHandler);
	m_pRandomHandler = pHandler;
}

float GPlayerRide::GetMoveSpeed() const
{
	if (!m_pMountBuff)
		return 100.0f;

	if (GetTestMode() == 1 &&
		m_pOwner->IsNowCombat())
	{
		// ���� �߿��� �� ������ ���� �ӵ�
		return m_pOwner->DefaultWalkSpeed(); 
	}

	return m_pMountBuff->m_ActorModifier.fMoveSpeed;
}

float GPlayerRide::RandomDismountHandler::CalcDismountRate( GEntityActor* pAttacker, GEntityActor* pVictim )
{
	VALID_RET(pAttacker, 0.0f);
	VALID_RET(pVictim, 0.0f);
	int nLF = GCalculator::CalcLevelFactor(pVictim->GetLevel(), pAttacker->GetLevel());
	float fRate = (float)(nLF * 10);
	fRate = max(fRate, 25.0f);
	fRate = min(fRate, 90.0f);
	return fRate;
}
