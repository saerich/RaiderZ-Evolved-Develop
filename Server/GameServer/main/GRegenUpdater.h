#ifndef _GREGEN_UPDATER_H
#define _GREGEN_UPDATER_H

#include "GDef.h"
#include "MTime.h"

class GEntityActor;

class GRegenUpdater
{
private:
	bool					m_bActive;
	MRegulator				m_RegenRegulator;

	float					m_fHPRegenModValue; ///< ȸ����Ű�� ���� ������ �Ҽ��� 
	float					m_fENRegenModValue; ///< ȸ����Ű�� ���� ������ �Ҽ��� 
	float					m_fSTARegenModValue; ///< ȸ����Ű�� ���� ������ �Ҽ��� 

	bool UpdateSTARegen( GEntityActor* pActor );
	bool UpdateENRegen( GEntityActor* pActor );
	bool UpdateHPRegen( GEntityActor* pActor );
	float CalcAmount( GEntityActor* pActor, float fRegenAmount, float fRegenModValue, float fRegenRateInCombat, float fPlayerSittingRate );
public:
	GRegenUpdater();
	void Update(float fDelta, GEntityActor* pPlayer);

	void SetActive(bool bActive) { m_bActive = bActive;	}
	bool IsActive() { return m_bActive; }
};







#endif // _GREGEN_UPDATER_H