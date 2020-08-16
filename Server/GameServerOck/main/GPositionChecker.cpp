#include "StdAfx.h"
#include "GPositionChecker.h"
#include "GGlobal.h"
#include "GSystem.h"
#include "GConst.h"

GPositionChecker::GPositionChecker()
: m_dwLastCheckTime(0)
, m_dwLastShallowSpeedhackTime(0)
, m_nShallowSpeedhackCount(0)
, m_dwLastDeepSpeedhackTime(0)
, m_nDeepSpeedhackCount(0)
{
}

void GPositionChecker::Init( vec3 vCurPos )
{
	SaveLastCheckInfo(vCurPos);
}

void GPositionChecker::SaveLastCheckInfo( vec3 vCurPos )
{
	m_vLastPos			= vCurPos;
	m_dwLastCheckTime	= gsys.pSystem->GetNowTime();
}

void GPositionChecker::Check(POSITION_CHECK_TYPE nType, vec3 vCurPos, float fMoveSpeed, POSITION_CHEAT_TYPE& outnPosCheatType, CHEAT_TREAT_TYPE& outnTreatType )
{
	DWORD dwDiffTime = gsys.pSystem->GetNowTime() - m_dwLastCheckTime;

	outnPosCheatType = POSCHEAT_OK;
	outnTreatType = CHEATTREAT_NONE;

	switch(nType)
	{
	case PCT_WALK:			outnPosCheatType = Check_Walk(vCurPos, fMoveSpeed, outnTreatType);	break;
	case PCT_WARP:			outnPosCheatType = Check_Warp(vCurPos, fMoveSpeed);	break;
	case PCT_MOTIONFACTOR:	outnPosCheatType = Check_MotionfactorMove(vCurPos, fMoveSpeed);	break;
	default: 
		assert(!L"invalid position check-type");
	}
}

POSITION_CHEAT_TYPE GPositionChecker::Check_Walk( vec3 vCurPos, float fMoveSpeed, CHEAT_TREAT_TYPE& outnTreatType )
{
	float fDistance  = m_vLastPos.DistanceTo(vCurPos);
	DWORD dwDiffTime = gsys.pSystem->GetNowTime() - m_dwLastCheckTime;
	float fDiffSecond = (float)dwDiffTime*0.001f;

	if (fDiffSecond < 1.0f)
		return POSCHEAT_OK; // �˻����� 1�ʰ� �������ٸ� ����

	//mlog("fDiffSecond=%.3f\tspeed=%.1f/%.1f\n", fDiffSecond, fDistance, fMoveSpeed);
	//mlog("	vCurPos		= %.1f %.1f %.1f\n", vCurPos.x, vCurPos.y, vCurPos.z);
	//mlog("	m_vLastPos	= %.1f %.1f %.1f\n", m_vLastPos.x, m_vLastPos.y, m_vLastPos.z);

	SaveLastCheckInfo(vCurPos);

	float fDeepSpeedHackLimitDistance = ((fDiffSecond+1.0f)*fMoveSpeed)*GConst::CHEAT_SPEEDHACK_DISCONNECT_LIMIT_DISTANCE_RATE;
	float fShallowSpeedHackLimitDistance = ((fDiffSecond+1.0f)*fMoveSpeed)*GConst::CHEAT_SPEEDHACK_WARNING_LIMIT_DISTANCE_RATE;

	if (fDistance >= GConst::CHEAT_WARPHACK_LIMIT_DISTANCE)
	{
		// ���� ����
		outnTreatType = CHEATTREAT_DISCONNECT;
		
		return POSCHEAT_POS_WARP; // ���� �ǽ�
	}

	if (fDistance >= fDeepSpeedHackLimitDistance)
	{
		outnTreatType = CHEATTREAT_TRY;
		if (CheckDeepSpeedhack())
		{
			// ���� ġ������ ���Ӳ���
			outnTreatType = CHEATTREAT_DISCONNECT;
		}

		return POSCHEAT_POS_SPEEDHACK; // ���ǵ��� �ǽ�
	}

	if (fDistance >= fShallowSpeedHackLimitDistance)
	{
		outnTreatType = CHEATTREAT_TRY;
		if (CheckShallowSpeedhack())
		{
			// ���� ġ������ ���Ӳ���
			outnTreatType = CHEATTREAT_WARNING;
		}

		return POSCHEAT_POS_SPEEDHACK; // ���ǵ��� �ǽ�
	}

	return POSCHEAT_OK;
}

POSITION_CHEAT_TYPE GPositionChecker::Check_Warp( vec3 vCurPos, float fMoveSpeed )
{
	SaveLastCheckInfo(vCurPos);
	return POSCHEAT_OK;
}

POSITION_CHEAT_TYPE GPositionChecker::Check_MotionfactorMove( vec3 vCurPos, float fMoveSpeed )
{
	SaveLastCheckInfo(vCurPos);
	return POSCHEAT_OK;
}

bool GPositionChecker::CheckShallowSpeedhack()
{
	if (m_dwLastShallowSpeedhackTime <= gsys.pSystem->GetNowTime())
	{
		// �����ð� ����Ǿ� ī���� �ʱ�ȭ
		m_nDeepSpeedhackCount = 0;
		m_dwLastShallowSpeedhackTime = 
			gsys.pSystem->GetNowTime() + int(GConst::CHEAT_SPEEDHACK_WARNING_EXECUTE_DURATION * 1000.0f);
	}

	++m_nShallowSpeedhackCount;

	return (m_nShallowSpeedhackCount >= GConst::CHEAT_SPEEDHACK_WARNING_EXECUTE_COUNT);
}


bool GPositionChecker::CheckDeepSpeedhack()
{
	if (m_dwLastDeepSpeedhackTime <= gsys.pSystem->GetNowTime())
	{
		// �����ð� ����Ǿ� ī���� �ʱ�ȭ
		m_nDeepSpeedhackCount = 0;
		m_dwLastDeepSpeedhackTime = 
			gsys.pSystem->GetNowTime() + int(GConst::CHEAT_SPEEDHACK_DISCONNECT_EXECUTE_DURATION * 1000.0f);
	}

	++m_nDeepSpeedhackCount;

	return (m_nDeepSpeedhackCount >= GConst::CHEAT_SPEEDHACK_DISCONNECT_EXECUTE_COUNT);
}
