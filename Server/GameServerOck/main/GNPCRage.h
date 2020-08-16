#pragma once

#include "GActorObserver.h"


class GEntityNPC;

class GNPCRage : private GActorObserver, public MTestMemPool<GNPCRage>
{
public:
	GNPCRage(GEntityNPC* pOwner);
	~GNPCRage(void);

	void AddRage(int nAmount);
	void ResetRage(); 
	int GetRage() const;

private:
	
	// �ǰ� �̺�Ʈ
	virtual void OnHit(GEntityActor* pOwner,  GEntityActor* pAttacker, GTalentInfo* pTalentInfo ) override;
	// ġ��Ÿ �ǰ� �̺�Ʈ
	virtual void OnCriHit( GEntityActor* pOwner, GEntityActor* pAttacker, GTalentInfo* pTalentInfo ) override;

private:
	GEntityNPC* m_pOwner;
	int m_nRage;
};
