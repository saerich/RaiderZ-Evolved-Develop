#ifndef _GTALENT_INFO_H
#define _GTALENT_INFO_H

#include "CSTalentInfo.h"
#include "CSTalentInfoHelper.h"
#include "GDef.h"
#include "GTalentDef.h"

using namespace cscommon_server;

class GTalentInfoMgr;
class GEntityActor;

/// �ŷ�Ʈ ����
class GTalentInfo : public CSTalentInfo, public MTestMemPool<GTalentInfo>
{
private:
	friend class GTalentInfoMgr;
	// ���� ������ xml���� �������� �ʰ�, ���� �����ϴ� �͵��̴�.
	bool					m_bExistCastingFX;
	bool					m_bExistUseFX;
	bool					m_bExistModValue;
	bool					m_bExistProjectileEvent;
	bool					m_bIsProjectileType;
	bool					m_bDelayedAct;
	float					m_fDistanceToLongestPoint;
	float					m_fExpiredTime; 
	float					m_fLeastProjectileDistance;
	vec3					m_vLowestProjectileStartPos;
	MBox					m_bxHit;
	bool					m_bHasExtraActiveEvent;
	bool					m_bHasAttack; // �������� ��Ұ� �ִ���
	
private:
	void					InitModValues();
	void					InitDistanceToLongestPoint();
	void					InitExpiredTime();
	void					InitActExtendTime();
	void					InitEvent();
	void					InitLeastProjectileDistance();
	void					InitHitAABB();
	void					InitCheckHeal();
	void					InitHasAttack();

public:
	
	// Ư���ϰ� ������ �ŷ�Ʈ�� �ĺ��� (��ġ�� ���ð��� ȿ���� ����)
	wstring					m_strSpecialization;
	// ��Ʈ�� ������ ������ ����
	bool					m_bBubbleHit;					
	// ���������� ��밡������ ����
	bool					m_bNonCombatOnly;
	// �ش� �ŷ�Ʈ�� �� �ڿ��� ��������� ȿ���� ��ä�� �ŷ�Ʈ
	int						m_nBackHitTalent;
	// ��ǥ ���Ϳ� �������� ���� �� �ִ��� ����
	bool					m_bHitOverlap;
	// �����ڸ� ����� �� �ִ��� ����
	bool					m_bUseOnlyGM;		
	// �� �ŷ�Ʈ���� ����
	bool					m_bHeal;
	// ȸ�� �ŷ�Ʈ ���� ����
	bool					m_bIsDodge;

	// Sound --------------------------
	wstring					m_szTargetSound;		// Ÿ�� ����(Not Used)
	wstring					m_szTargetLoopingSound;	// Ÿ�� �����Ǵ� ����(Not Used)

	// AI -----------------------------
	float					m_fMinAITime;			// NPC�� ���带 ���������� ���Ǵ� �ּ� AITime
	float					m_fTalentStartRange;	// NPC ���� ���� �Ÿ� (�� ���� 0.0f�̸� �� ���� ���õǰ� ĸ�� �Ÿ��� �Ǵ��մϴ�.)

	GTalentInfo();
	virtual ~GTalentInfo();
	virtual void 			Cooking();
	virtual void			CopyAll(CSTalentInfo* pTarTalentInfo) override;
		
	bool					IsExistcastingFX()		{ return m_bExistCastingFX; }
	bool					IsExistUseFX()			{ return m_bExistUseFX; }
	bool					IsExistModValue()		{ return m_bExistModValue; }
	bool					isAttackable()			{ return !m_HitInfo.m_vSegments.empty(); }
	bool					IsMoving();
	bool					IsKnockbackDirSyncToAttackerDir() { return IsMoving(); }
	bool					IsAggressive()			{ return (	m_EffectInfo.m_nRelation == CSEffectInfo::RELATION_ENEMY); }
	bool					IsRequireMoveSpeed()	{ return m_fRequireMoveSpeed >= 0.0f; }
	bool					IsExtraActive()			{ return (m_nExtraActive != TEAT_NONE); }
	bool					IsGuard()				{ return m_nSkillType == ST_GUARD; }
	bool					IsProjectileType()		{ return m_bIsProjectileType; }
	bool					IsDelayedAct()			{ return m_bDelayedAct; }
	bool					HasBuff()				{ return (m_Buff1.nID != INVALID_BUFF_ID || m_Buff2.nID != INVALID_BUFF_ID); }
	bool					HasEvent()				{ return !m_Events.m_vecEvents.empty(); }
	bool					HasActEvent()			{ return !m_Events.m_vecActEvents.empty(); }
	bool					HasExtraActiveEvent()	{ return m_bHasExtraActiveEvent; }
	bool					HasCastingTime()		{ return m_fCastingTime > 0.0f; }
	bool					HasProjectileEvent()	{ return m_bExistProjectileEvent; }
	bool					HasHeal() const; // �ش� �ŷ�Ʈ�� ���Ե� ��� ���(���� ��)�߿� ���� �ִ��� ����
	
	vec3					GetLowestProjectileStartPos()	{ return m_vLowestProjectileStartPos; }
	float					GetDistanceToLongestPoint() { return m_fDistanceToLongestPoint; }
	float					GetExpiredTime()			{ return m_fExpiredTime; }
	float					GetLeastProjectileDistance() { return m_fLeastProjectileDistance; }
	const MBox&				GetHitAABB()				{ return m_bxHit; }

	int						MakeDamageFromMinMaxDamage();
	bool					IsUseAvoidTime()			{ return m_bUseAvoidTime; }
	bool					IsUseInvincibleTime()		{ return m_bUseInvincibleTime; }
	bool					IsUseSuperarmorTime()		{ return m_bUseSuperarmorTime; }

	bool					IsEffective(GEntityActor* pReqActor, GEntityActor* pTarActor);
	bool					IsMovingWithCasting();

	void					RemoveExpiredTime()		{ m_fExpiredTime = 0.0f; }
	
	bool					HasAttack()				{ return m_bHasAttack; }

	bool					IsNPCTalent();	
	bool					IsPortalTalent();
	void					InitDodge();
};



#define TALENT_PROPERTY_SHOUT	L"shout"
#define TALENT_PROPERTY_SAZAHU	L"sazahu"



#endif
