#pragma once

#include "GEffectTargetSelector.h"
#include "GExtraActiveTalentRunner.h"
#include "CSTalentInfo.h"
#include "GModEffector.h"
#include "GActorDamageCalculator.h"

class GEntityActor;
class GTalentInfo;

// ��Ȱ: �������� �ŷ�Ʈ�� ȿ���� ���Ϳ��� �����Ŵ
class GTalentEffector
{
public:
	struct Info
	{
		GTalentInfo* pTalentInfo;
		TALENT_CONDITION nTalentCondition;
		GField* pField;
		GEntityActor* pUser;
		GEntityActor* pTarget;
		vec3		vGroundPoint;
		bool bIgnoreMF;

		Info()
			: pTalentInfo(NULL), nTalentCondition(TC_NONE), pField(NULL), pUser(NULL), pTarget(NULL), bIgnoreMF(false), vGroundPoint(vec3::ZERO)
		{}
	};
public:
	// ȿ���� ��� Ÿ�� ����� ����� �ŷ�Ʈ ȿ�� ���(���� ����)�� ����
	void ApplyTalent(Info& info);
	// Target���� �ŷ�Ʈ�� ���� ȿ���� ��
	bool GainTalentEffect(GEntityActor* pUser, GTalentInfo* pTalentInfo, GEntityActor* pTarget, bool bIgnoreMF, bool bTalentInfoModified=false);
	// Target���� �ŷ�Ʈ�� ���� ȿ���� ����
	void LostTalentEffect(GEntityActor* pUser, GTalentInfo* pTalentInfo, GEntityActor* pTarget);
	// Target���� �ŷ�Ʈ ���� ȿ���� ��
	bool GainDamageAndHeal(GEntityActor* pUser, GEntityActor* pTarget, GTalentInfo* pTalentInfo, GDamageRangedInfo infoDamage, GHealRangedInfo infoHeal, bool bIgnoreMF, bool bNeedDamageRecalc=false, float fDamageApplyRate=1.0f);
protected:
	// Victim�� �߽����� Ÿ��Ÿ�Կ� �ش�Ǵ� ��� ���Ϳ��� ���� ȹ���� �õ�
	void GainSingleBuff( const Info& info, GTalentInfo* pTalentInfo, const CSBuffEnchantInfo& BuffInvokeInfo );
	// ���ظ� ������ �ŷ�Ʈ���� ����
	bool IsNeedEffect(GTalentInfo* pTalentInfo);	
	// ������ ���õ� ȿ���� ó��
	void GainTalentBuff(const Info& info, GTalentInfo* pTalentInfo);
	// �ŷ�Ʈ�� �ν���Ʈ ������̾� ȿ���� ���� ��Ŷ �߼�
	void RouteGainInstantEffect(GEntityActor* pUser, GTalentInfo* pTalentInfo);	

private:
	// �ŷ�Ʈ�� ȸ����
	void OnHealed( GEntityActor* pTarget, GTalentInfo* pTalentInfo, int nHealHPAmount );
	// �ŷ�Ʈ�� ��� ȿ���� �ִ��� ����
	bool IsInstantEffect( GTalentInfo* pTalentInfo );
protected:
	// �齺�� �ŷ�Ʈ�� ��� ����Ʈ�� ���� �ŷ�Ʈ ������ �ٲ۴�.
	GTalentInfo* ModifyBackHitTalentInfo(GTalentInfo* pTalentInfo, GEntityActor* pTarget);
private:
	GEffectTargetSelector		m_EffectTargetSelector;
	GExtraActiveTalentRunner	m_ExtraActiveTalentRunner;
	GModEffector				m_ModEffector;
	GActorDamageCalculator		m_DamageCalculator;
};	
