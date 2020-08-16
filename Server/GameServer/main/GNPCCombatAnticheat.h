#pragma once

#include "GActorObserver.h"

class GEntityNPC;

class GNPCCombatAnticheat : public GActorObserver, public MTestMemPool<GNPCCombatAnticheat>
{
public:
	GNPCCombatAnticheat(GEntityNPC* pOwner);
	~GNPCCombatAnticheat(void);

	bool IsCheating() const;

private:
	void Reset();
	bool IsAvoidRange() const;

private:
	// ���� ����
	virtual void OnCombatBegin(GEntityActor* pOwner) override;
	// ���� ����
	virtual void OnCombatEnd(GEntityActor* pOwner) override;
	// �ŷ�Ʈ ����� ���۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnUseTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo ) override;
	// �ŷ�Ʈ ����� �Ϸ������ ȣ��Ǵ� �̺�Ʈ
	virtual void OnFinishTalent(GTalentInfo* pTalentInfo) override;
	// ���� ���������� �̺�Ʈ
	virtual void OnHitEnemy(GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pTarget, GTalentInfo* pTalentInfo) override;
	// ���� ������ �������� �̺�Ʈ
	virtual void OnGuardEnemy(GEntityActor* pOwner, GEntityActor* pTarget, GTalentInfo* pAttackTalentInfo) override;
	// ���� �ŷ�Ʈ�� ��ȿȭ�������� ȣ��Ǵ� �̺�Ʈ
	virtual void OnTalentImmunedEnemy(GEntityActor* pOwner, GEntityActor* pTarget, GTalentInfo* pTalentInfo) override;

private:
	GEntityNPC* m_pOwner;
	int m_nAvoidCount;
	vec3 m_vLastAttackedPos;
	vec3 m_vLastAttackedDir;
};
