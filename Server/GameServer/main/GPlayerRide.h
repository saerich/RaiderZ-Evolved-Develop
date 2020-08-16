#pragma once

#include "GActorObserver.h"

class GEntityPlayer;

class GPlayerRide : public GActorObserver
{
public:
	class RandomDismountHandler
	{
	public:
		virtual float CalcDismountRate( GEntityActor* pAttacker, GEntityActor* pVictim );
	};
public:
	GPlayerRide(GEntityPlayer* pOwner);
	~GPlayerRide(void);

	bool IsMounted() const;
	void Dismount();

	void SetRandomHandler(RandomDismountHandler* pHandler);
	float GetMoveSpeed() const;

private:
	bool IsRideBuff(int nBuffID) const;
	int GetTestMode() const;
	void HandleDismountEvent(const string strEvent, GEntityActor* pAttacker=NULL);
	float CalcDismountRate(GEntityActor* pAttacker);
private:
	// ���� ����
	virtual void OnCombatBegin(GEntityActor* pOwner) override;
	// �ǰ� �̺�Ʈ
	virtual void OnHit( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo ) override;
	// ������ ������� �̺�Ʈ
	virtual void OnGainBuff(int nBuffID) override;
	// ������ �Ҿ����� �̺�Ʈ
	virtual void OnLostBuff(int nBuffID) override;
private:
	GEntityPlayer* m_pOwner;
	GBuffInfo*		m_pMountBuff;
	RandomDismountHandler* m_pRandomHandler;
};
