#pragma once

#include "GActorObserver.h"

class GActorTalent : private GActorObserver, public MTestMemPool<GActorTalent>
{
public:
	GActorTalent(GEntityActor* pOwner);
	~GActorTalent(void);

	bool HasUseTalentPos() const;
	vec3 GetUseTalentPos() const;

private:
	// �ŷ�Ʈ ����� ���۵ɶ� ȣ��Ǵ� �̺�Ʈ
	virtual void OnUseTalent( GEntityActor* pUser, GTalentInfo* pTalentInfo ) override;
	// �ŷ�Ʈ ����� �Ϸ������ ȣ��Ǵ� �̺�Ʈ
	virtual void OnFinishTalent(GTalentInfo* pTalentInfo) override;
private:
	GEntityActor*	m_pOwner;
	// �ŷ�Ʈ�� ����� ���� ��ġ
	vec3			m_vUseTalentPos;
};
