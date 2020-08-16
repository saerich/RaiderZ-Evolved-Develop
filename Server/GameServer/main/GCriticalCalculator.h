#pragma once

#include "GCalculator.h"
#include "GTalentInfo.h"
#include "GEntityPlayer.h"
#include "GEntityNPC.h"

/// ũ��Ƽ�� ��� ����
/// Factor�� 1 == 1, Percent�� 100(%) == 1
class GCriticalCalculator : public MTestMemPool<GCriticalCalculator>
{
protected:
	virtual int _GetSpecializationRankForDamageAttrib(GEntityPlayer* pPlayer, DAMAGE_ATTRIB nDamageAttrib);

	// ġ��Ÿ��
	virtual float CalcActorCriticalPercent(const GEntityActor* pActor, const GTalentInfo* pTalentInfo);	// ���̽� ���� ũ���� ���
	virtual float _CalcPlayerCriticalPercent( TALENT_CATEGORY nTalentCategory, PLAYER_INFO* pPlayerInfo );	// PC�� ĳ���� ġ��Ÿ ��
	virtual float _CalcNPCCriticalPercent( TALENT_SKILL_TYPE nSkillType, GNPCInfo* pNPCInfo );

	virtual float CalcTalentPercent(GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentInfo* pTalentInfo);
	virtual float CalcDamageAttribPercent(GEntityPlayer* pPlayer, DAMAGE_ATTRIB nDamageAttrib);
	virtual float _CalcDamageAttribPercent( DAMAGE_ATTRIB nDamageAttrib, int nSpecializationRank );

	// ������ ����
	virtual float CalcBuffDamagePercent(GEntityActor* pActor, const GTalentInfo* pTalentInfo);
	virtual int CalcDamageBonusPercent(GEntityPlayer* pPlayer, DAMAGE_ATTRIB nDamageAttrib);
	virtual int _CalcDamageBonusPercent(DAMAGE_ATTRIB nDamageAttrib, int nSpecializationRank);

public:
	virtual float CalcCriticalPercent(GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentInfo* pTalentInfo);
	virtual float CalcCriticalDamageFactor(GEntityActor* pAttacker, const GTalentInfo* pTalentInfo);
};

