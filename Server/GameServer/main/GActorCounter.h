#pragma once

#include "GActorObserver.h"

class GEntityActor;
class GTalentInfo;

class GActorCounter : private GActorObserver, public MTestMemPool<GActorCounter>
{
public:
	GActorCounter(GEntityActor* pOwner);
	~GActorCounter(void);

private:
	// �ŷ�Ʈ ����� ���۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnUseTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo ) override;
	// �ŷ�Ʈ ����� �Ϸ������ ȣ��Ǵ� �̺�Ʈ
	virtual void OnFinishTalent(GTalentInfo* pTalentInfo) override;
	// �ŷ�Ʈ�� ��ȿ�������� ȣ��Ǵ� �̺�Ʈ
	virtual void OnTalentImmuned(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo) override;
	// �ŷ�Ʈ�� ȸ�ǽ������� ȣ��Ǵ� �̺�Ʈ
	virtual void OnTalentAvoid(GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo) override;
	
	// ���� ���������� �̺�Ʈ
	virtual void OnHitEnemy(GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pTarget, GTalentInfo* pTalentInfo) override;

private:
	void Clear();
	bool IsCounter() const;
private:
	// ������
	GEntityActor*	m_pOwner;
	// ������� �ŷ�Ʈ ����
	GTalentInfo*	m_pUsingTalentInfo;
	// ��ȿȭ�� ������ �޾Ҵ��� ����
	bool			m_bImmunedTalent;
};
