#pragma once

#include "GTalent.h"
#include "GTalentEventChecker.h"
#include "MMemPool.h"

/// ���� �ŷ�Ʈ
class GItemTalent : public GTalent, public MMemPool<GItemTalent>
{
protected:
	virtual void	OnEnterPhaseAct();
	
public:
	GItemTalent(GEntityActor* pOwner, GTalentInfo* pTalentInfo, TALENT_TARGET_INFO Target, bool bGainStress=true);
	virtual ~GItemTalent() {}

	
};

