#pragma once

#include "GEntityActor.h"

#define DEAD_IMPACT_MOTION_FACTOR_KB_WEIGHT_MIN		200

#define DEAD_IMPACT_MOTION_FACTOR_KB_WEIGHT_MAX_1	300
#define DEAD_IMPACT_MOTION_FACTOR_KB_WEIGHT_MAX_2	400
#define DEAD_IMPACT_MOTION_FACTOR_KB_WEIGHT_MAX_3	500

// �׾��� ������ ����Ʈ �ִ� ȿ���� ���� ������ �˹����� �״´�.
class GDeadMotionFactorImpactor
{
protected:
	virtual bool Dice();
	bool CheckTargetMFModifier(GEntityNPC* pTarget);
	MF_STATE MakeFinishEffectMF(GTalentInfo* pTalentInfo);
	int MakeFinishEffectMFWeight(GEntityNPC* pTarget);
public:
	GDeadMotionFactorImpactor() {}

	bool CheckUp(GMFApplyArgs& outArgs, GEntityActor* pTarget, GTalentInfo* pTalentInfo, int nDamage);


};