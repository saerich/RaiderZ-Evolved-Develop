#include "stdafx.h"
#include "GMeleeTalent.h"

void GMeleeTalent::OnStart()
{
	// ��Ʈĸ�� Ÿ�ٸ� ��ȿ�մϴ�. (���� ����)
	m_pTalentInfo->m_EffectInfo.m_nPoint = CSEffectInfo::POINT_HITCAPSULE;

	GTalent::OnStart();
}
