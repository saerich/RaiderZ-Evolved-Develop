#pragma once

class GEntityActor;
class GTalentInfo;
struct GTalentResist;

__interface ICombatCalculator
{
public:
	virtual bool CheckMiss(GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentInfo* pTalentInfo);
	virtual bool CheckRiposte(GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentInfo* pTalentInfo);
	virtual bool CheckResist(float& foutResistPercent, GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentResist& Resist);
	virtual bool CheckCritical(GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentInfo* pTalentInfo);
	virtual int CalcDamage(GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentInfo* pTalentInfo, bool bCritical, float fResistFactor, const GDamageRangedInfo& DamageInfo, float* fOutMotionFactor);
};


class GCombatCalculator : public ICombatCalculator, public MTestMemPool<GCombatCalculator>
{
public:
	GCombatCalculator();

	// ȸ�� ���������� true ��ȯ
	virtual bool CheckMiss(GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentInfo* pTalentInfo);
	virtual bool CheckRiposte(GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentInfo* pTalentInfo);
	// ���� ���������� true ��ȯ
	virtual bool CheckResist(float& foutResistPercent, GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentResist& Resist);
	// ũ��Ƽ�� ���������� true ��ȯ
	virtual bool CheckCritical(GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentInfo* pTalentInfo);
	virtual int CalcDamage(GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentInfo* pTalentInfo, bool bCritical, float fResistFactor, const GDamageRangedInfo& DamageInfo,float* pfoutMotionFactor = NULL);
	virtual int CalcHealAmount( GEntityActor* pAttacker, GEntityActor* pVictim, const GTalentInfo* pTalentInfo, const GHealRangedInfo& infoHeal);
	
	virtual float CalcPerfectGuardRate(GEntityActor* pAttacker, GEntityActor* pGuarder);	
	static float CalcPenetrationFactor(GEntityActor* pAttacker, const GTalentInfo* pTalentInfo);

private:
	bool IsFineDurationWeapon(GEntityActor* pAttacker, const GTalentInfo* pTalentInfo);
};
