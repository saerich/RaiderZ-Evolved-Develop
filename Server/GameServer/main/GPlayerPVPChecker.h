#pragma once

#include "GActorObserver.h"

class GEntityPlayer;

class GPlayerPVPChecker : public GActorObserver, public MTestMemPool<GPlayerPVPChecker>
{
public:
	GPlayerPVPChecker(GEntityPlayer* pOwner);
	~GPlayerPVPChecker();
	
	// ���� ����
	void doMakePeace();
	// PVP ������ ����
	bool IsNowPVP() const;
	// ƽ ó��
	void Update(float fDelta);
private:
	// PVP ���� ����
	void BeginPVP();
	// PVP ���� ����
	void FinishPVP();

	// �ǰ� �̺�Ʈ
	virtual void OnHit( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo ) override;
	// ���� ���������� �̺�Ʈ
	virtual void OnHitEnemy(GEntityActor* pOwner, uint32 nCombatResultFalg, GEntityActor* pTarget, GTalentInfo* pTalentInfo) override;
private:
	GEntityPlayer* m_pOwner;
	MRegulator	m_rgrReleaser;
	bool		m_bNowPVP;
};
