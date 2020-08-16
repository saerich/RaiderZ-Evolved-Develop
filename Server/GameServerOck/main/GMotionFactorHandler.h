#pragma once

#include "CSMotionFactorInfo.h"

class GEntityActor;

/// ���� ��� ���� ���� - ��ġ ���� ��
/// - GMotionFactor���� GEntityActor�� �и�
class GMotionFactorHandler
{
private:
	vec3 MakeKnockbackPos( MF_STATE nMF, GEntityActor* pVictimActor, const GEntityActor* pOpponent, int nWeight, bool bKnockbackDirSyncToOpponentDir );
public:
	GMotionFactorHandler() {}
	~GMotionFactorHandler() {}
	void Handle(GEntityActor* pVictimActor, const GEntityActor* pOpponent, MF_STATE nMF, int nWeight, bool bKnockbackDirSyncToOpponentDir);

	vec3 GetKnockbackDir(MF_STATE nMF, GEntityActor* pVictimActor, const GEntityActor* pOpponent, bool bKnockbackDirSyncToOpponentDir);
};
