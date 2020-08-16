#pragma once

#include "GTalentInfo.h"

class GEntityActor;
class GEntitySync;
class GField;
class GRelationChecker;

class GEffectTargetSelector
{
public:
	struct Desc
	{
		const CSEffectInfo& EffectInfo;
		GEntitySync* pSelf;
		GEntitySync* pTarget;
		GEntitySync* pCaster;
		float fAddEffectRadius;
		vec3 vCenterPoint;

		Desc(const CSEffectInfo& _EffectInfo)
			: EffectInfo(_EffectInfo)
		{
			pSelf = NULL;
			pTarget = NULL;
			pCaster = NULL;
			fAddEffectRadius = 0.0f;
			vCenterPoint=vec3::ZERO;
		}
	};
public:
	// �ش� CSEffectInfo�� �´� ����ڵ��� ã�´�.
	void Select(const Desc& desc,set<GEntityActor*>& result) const;
private:

	// ������ ��ǥ�� ã�Ƴ�
	void GetActors(GField& field, const vec3& vCenterPoint, float fRadius, set<GEntityActor*> &outvecEffectTarget) const;

	// ����� ��ǥ�� �ɷ���
	void FilterRelation(GEntitySync* pSelf, CSEffectInfo::RELATION nRelation, set<GEntityActor*> &outvecEffectTarget) const;

	// �����ο����� ��ǥ�� �ɷ���
	void FilterLimit(vec3 vCenterPoint, int nLimit, set<GEntityActor*> &outvecEffectTarget) const;
	
	// �ڽ��� ��Ƽ�� �������� ����
	bool HasParty(GEntityActor* pSelf) const;
};
