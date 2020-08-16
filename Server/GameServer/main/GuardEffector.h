#pragma once

#include "CSDef.h"

class GEntityActor;
class GTalentInfo;

class GuardEffector
{
public:
	enum Result
	{
		GUARD_FAILED = 0,
		GUARD_SUCCESS,
	};

public:
	// ���尡 �����ߴ��� ����, ���带 �� �� �ְ� �����ߴٸ� true�� ��ȯ
	Result CheckGuard(GEntityActor* pGuarder, GEntityActor* pAttacker, GTalentInfo* pTalentInfo);

	// ���尡 �����ߴ��� ����, ���带 �� �� �ְ� �����ߴٸ� true�� ��ȯ
	GUARD_TYPE GetGuardLevel( GEntityActor* pGuarder, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo );
	
	// ���� ȿ�� ����
	void ApplyEffect_Guard( GEntityActor* pGuarder, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo, GUARD_TYPE nGuardType, uint16 nCheckTime, int nDamage);

	// ������� ȿ�� ����
	void ApplyEffect_PerfectGuard( GEntityActor* pGuarder, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo, int nDamage );

	// �κй�� ȿ�� ����
	void ApplyEffect_PartialGuard( GEntityActor* pGuarder, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo, int nDamage );

	// ���带 �õ� �ߴ��� ����
	bool IsTryGuard(GEntityActor* pGuarder);

	// ���嵥���� ���ϱ�
	int CalcGuardedDamage(GEntityActor* pGuarder, GEntityActor* pAttacker, GUARD_TYPE nGuardType, int nDamage);

	// ���ⵥ���� ���ϱ�
	float CalcReducedDamage(GEntityActor* pGuarder, GEntityActor* pAttacker, GUARD_TYPE nGuardType);
private:
	// ���� �� �������� ����
	bool CheckPerfectGuard(GEntityActor* pGuarder, GEntityActor* pAttacker);

	// ��� ���� ��Ŷ
	void RouteGuardDefense(GEntityActor* pGuarder, GUARD_TYPE nType, UIID nAttackerUIID, int nTalentID, uint32 nHitTime, int nGuardedDamage);

	// ���ι�� ȿ������ �����Լ�
	void ApplyEffect_SpecificGuardImpl( GEntityActor* pGuarder, GEntityActor* pAttacker, GTalentInfo* pAttackTalentInfo, GUARD_TYPE nGuardType, int nDamage );

	// ������ �������� ����
	bool IsAbsoluteGuard( GEntityActor* pGuarder );
};
