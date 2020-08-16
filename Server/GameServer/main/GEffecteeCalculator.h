#pragma once

#include "GTalentInfo.h"

class GEntityActor;
class GField;

class GEffecteeCalculator
{
public:
	GEffecteeCalculator(void);
	~GEffecteeCalculator(void);

	// �ش� �ŷ�Ʈ Ÿ��Ÿ�Կ� �´� ������ ���͵��� ��ȯ
	vector<GEntityActor*> Calculate(GField* pField, 
									GEntityActor* pUser, 
									TALENT_TARGET nTargetType,
									GEntityActor* pTargetActor, 
									vec3 vPoint,
									const float fRadius, 
									GEntityActor* pAttacker=NULL,
									GEntityActor* pDefender=NULL) const;
private:
	// �ŷ�Ʈ Ÿ��Ÿ�Կ� �´� ��ƼƼŸ���� ��ȯ
	ENTITY_TYPEID	GetFindActorType(GEntityActor* pUser, TALENT_TARGET nTargetType) const;
	// ��ǥ ���Ͱ� ��ȿ���� ��ȯ
	bool			IsValidTarget( GEntityActor* pTarget ) const;
	// ��ǥ ���Ͱ� ȿ�� ���� �ȿ� �ִ��� ��ȯ
	bool			IsValidDistance(vec3 vPoint, 
									GEntityActor* pTarget, 
									const float fEffectRadius ) const;
	// ��ǥ ���Ϳ� ���� �ʵ忡 �ִ��� ��ȯ
	bool			IsSameField(GEntityActor* pUser, GEntityActor* pTarget) const;
	// �̹� �߰��� �������� ��ȯ
	bool			IsAlreadyInserted(const vector<GEntityActor*> &vecEffectees, GEntityActor* pTarget) const;

	// ��ȿ�� ���� ����ڸ� ��ȯ
	GEntityActor*	FindValidDuelOpponent(	GEntityActor* pUser, 
											TALENT_TARGET nTargetType,
											vec3 vPoint, 
											const float fRadius) const;
	// ��Ƽ�� ������� ��ǥ�� ã�Ƴ�
	void OnTargetParty( GField* pField, 
						GEntityActor* pUser, 
						GEntityActor* pCenterActor, 
						const float fRadius,
						vector<GEntityActor*> &vecEffectees) const;
	// �ڽ��� ������� ��ǥ�� ã�Ƴ�
	void OnTargetSelf(GEntityActor* pUser, vector<GEntityActor*> &vecEffectees) const;
	// ������ ������� ��ǥ�� ã�Ƴ�
	void OnTargetArea(	GField* pField, 
						GEntityActor* pUser,
						ENTITY_TYPEID nFindActorType, 
						vec3 vPoint, 
						const float fRadius,
						vector<GEntityActor*> &vecEffectees) const;
	// ������ ��븦 ������� ��ǥ�� ã�Ƴ�
	void OnTargetVictim(GEntityActor* pTargetActor, vector<GEntityActor*> &vecEffectees ) const;
	// ������ ��븦 ������� ��ǥ�� ã�Ƴ�
	void OnTargetAttacker(GEntityActor* pAttacker, vector<GEntityActor*> &vecEffectees ) const;
	// ������ ��븦 ������� ��ǥ�� ã�Ƴ�
	void OnTargetDefender(GEntityActor* pDefender, vector<GEntityActor*> &vecEffectees ) const;
};
