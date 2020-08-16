#pragma once

#include "GActorObserver.h"

class GEntityNPC;
class GEntityActor;
class GTalentInfo;
class GNPCInfo;

class GNPCSwallow : public GActorObserver, public MTestMemPool<GNPCSwallow>
{
public:
	GNPCSwallow(GEntityNPC* pOwner);
	~GNPCSwallow(void);

	// ��ų �� �ִ��� ����
	bool IsSwallowable();
	// ��Ű�� �ִ� �������� ����
	bool IsSwallowing();
	// ��Ű��
	bool Swallow( GEntityActor* pTarget, GTalentInfo* pTalentInfo );
	// ���
	bool Spewup(bool bInterrupt);

private:
	// �ڽ��� �׾����� ȣ��Ǵ� �̺�Ʈ
	virtual void OnDie() override;
	// �������� ���� ��� �����Լ�
	void GainBuffImpl(GEntityActor* pTarget, GTalentInfo* pTalentInfo, int nBuffID );
	// �Ա� ���� ���� ���
	void GainBuff(GEntityActor* pTarget, GNPCInfo* pNPCInfo, GTalentInfo* pTalentInfo );
	// �Ա� ���� ���� ����
	void LostBuff(GEntityActor* pTargetActor);
	// ��� ��Ŷ ����
	void RouteSpewup(bool bInterrupt);
private:
	GEntityNPC* m_pOwner;
	MUID m_uidTarget;
};
