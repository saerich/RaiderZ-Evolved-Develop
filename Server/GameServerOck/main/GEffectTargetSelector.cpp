#include "StdAfx.h"
#include "GEffectTargetSelector.h"	
#include "GEntityPlayer.h"
#include "GEntityNPC.h"
#include "GDuel.h"
#include "GPlayerBattleArena.h"
#include "GGlobal.h"
#include "GFactionSystem.h"
#include "GRelationChecker.h"
#include "GPartySystem.h"
#include "GFieldGrid.h"
#include "GBuffEntity.h"

void GEffectTargetSelector::Select(const Desc& desc,set<GEntityActor*>& result) const
{
	const CSEffectInfo& EffectInfo = desc.EffectInfo;
	GEntitySync* pSelf = desc.pSelf;
	GEntitySync* pTarget = desc.pTarget;
	GEntitySync* pCaster = desc.pCaster;
	float fAddEffectRadius = desc.fAddEffectRadius;
	vec3 vCenterPoint = desc.vCenterPoint;

	VALID(pSelf);

	GEntitySync* pCenterActor = NULL;
	float fEffectRadius = EffectInfo.m_fRadius + fAddEffectRadius;

	switch (EffectInfo.m_nPoint)
	{
	case CSEffectInfo::POINT_SELF:
		{
			pCenterActor = pSelf;
			vCenterPoint = pCenterActor->GetPos();
		}
		break;
	case CSEffectInfo::POINT_CASTER:
		{
			if (!pCaster)
				return;

			pCenterActor = pCaster;
			vCenterPoint = pCenterActor->GetPos();
		}
		break;
	case CSEffectInfo::POINT_TARGET:
	case CSEffectInfo::POINT_HITCAPSULE: 
		// Hitcapsule�� ������ ���� ����� pTarget���� �Ѿ���� ������,
		// POINT_HITCAPSULE�� pTarget���� ó��
		{
			if (!pTarget) 
				return;
			
			pCenterActor = pTarget;		
			vCenterPoint = pCenterActor->GetPos();
		}
		break;
	case CSEffectInfo::POINT_AREAPOINT:
		{
			// pCenterActor�� �״�� ��
		}break;
	default:
		{
			return;
		}
		break;
	}
	
	if (pCenterActor &&
		pCenterActor->IsActor())
	{
		result.insert(static_cast<GEntityActor*>(pCenterActor));
	}

	if (fEffectRadius == 0.0f)
	{
		return;
	}

	GField* pField = pSelf->GetField();
	if (!pField)
		return;

	GetActors(*pField, vCenterPoint, fEffectRadius, result);
	// ����� �ڱ� �ڽſ��� �����Ѵ�
	FilterRelation(pSelf, EffectInfo.m_nRelation, result);
	FilterLimit(vCenterPoint, EffectInfo.m_nLimit, result);	
}

void GEffectTargetSelector::GetActors(GField& field, const vec3& vCenterPoint, float fRadius, set<GEntityActor*>& outvecEffectTarget) const
{
	GFieldGrid::EntitySelector* entitySelector = 
		field.GetGrid().GetEntitySelector();
	VALID(NULL!=entitySelector);

	class CollectEffectTargets : public GFieldGrid::Sector::IVisitor
	{
	public:
		CollectEffectTargets(GField& field,const vec3& centerPos,const float& radius,set<GEntityActor*>& effectTargets)
			: m_field(field)
			, m_effectTargets(effectTargets)
			, m_centerPos(centerPos)
			, m_radius(radius) {}

	private:
		void OnVisit(const MUID& uid)
		{
			GEntityActor* const target = m_field.FindActor(uid);
			if (NULL == target) return;
			if (m_radius + target->GetColRadius() < m_centerPos.DistanceTo(target->GetPos())) return;

			m_effectTargets.insert(target);
		}

	private:
		GField&				m_field;
		const vec3&			m_centerPos;
		const float&		m_radius;
		set<GEntityActor*>&	m_effectTargets;
	};
	entitySelector->VisitByPos(CollectEffectTargets(field, vCenterPoint, fRadius, outvecEffectTarget), ETID_ACTOR, vCenterPoint, 1);
}

void GEffectTargetSelector::FilterRelation(GEntitySync* pSelf, CSEffectInfo::RELATION nRelation, set<GEntityActor*> &outvecEffectTarget) const
{
	VALID(pSelf);

	GEntityActor* pSelfActor = NULL;
	if (pSelf->IsActor())
	{
		pSelfActor = ToEntityActor(pSelf);
	}
	else if (pSelf->IsBuffEntity())
	{
		pSelfActor = static_cast<GBuffEntity*>(pSelf)->GetOwner();
	}
	else
	{
		VALID(false);
	}

	GRelationChecker relationChecker;
	vector<GEntityActor*> vecFiteredEffcteeTarget;
	switch (nRelation)
	{
	case CSEffectInfo::RELATION_ALL:
		{
			for each (GEntityActor* pEffectTarget in outvecEffectTarget)
			{
				if (relationChecker.IsAll(pSelfActor, pEffectTarget))
				{
					vecFiteredEffcteeTarget.push_back(pEffectTarget);
				}
			}
		}
		break;		
	case CSEffectInfo::RELATION_ENEMY:
		{
			for each (GEntityActor* pEffectTarget in outvecEffectTarget)
			{
				if (relationChecker.IsEnemy(pSelfActor, pEffectTarget))
				{
					vecFiteredEffcteeTarget.push_back(pEffectTarget);
				}
			}
		}
		break;
	case CSEffectInfo::RELATION_ALLIED:
		{
			for each (GEntityActor* pEffectTarget in outvecEffectTarget)
			{
				if (relationChecker.IsAlly(pSelfActor, pEffectTarget))
				{
					vecFiteredEffcteeTarget.push_back(pEffectTarget);
				}
			}
		}
		break;
	case CSEffectInfo::RELATION_PARTY:
		{
			for each (GEntityActor* pEffectTarget in outvecEffectTarget)
			{
				if (relationChecker.IsParty(pSelfActor, pEffectTarget))
				{
					vecFiteredEffcteeTarget.push_back(pEffectTarget);
				}
			}
		}
		break;
	case CSEffectInfo::RELATION_ALLIED_DEAD:
		{
			for each (GEntityActor* pEffectTarget in outvecEffectTarget)
			{
				if (relationChecker.IsAllyDead(pSelfActor, pEffectTarget))
				{
					vecFiteredEffcteeTarget.push_back(pEffectTarget);
				}
			}
		}
		break;
	case CSEffectInfo::RELATION_SELF:
		{
			for each (GEntityActor* pEffectTarget in outvecEffectTarget)
			{
				if (pEffectTarget->GetUID() == pSelfActor->GetUID())
				{
					vecFiteredEffcteeTarget.push_back(pEffectTarget);
				}
			}
		}
		break;
	default:
		{
		}
		break;
	}

	outvecEffectTarget.clear();
	outvecEffectTarget.insert(vecFiteredEffcteeTarget.begin(), vecFiteredEffcteeTarget.end());
}

void GEffectTargetSelector::FilterLimit(vec3 vCenterPoint, int nLimit, set<GEntityActor*> &outvecEffectTarget) const
{
	if (nLimit >= static_cast<int>(outvecEffectTarget.size())) 
		return;

	// vector<GEntityActor*> �״�� �����ϰ� ������ pSelf�� �ʿ��ؼ� nth_element�� �����ϰ� �ִ�.
	// �׷��� map<float, GEntityActor*>�־ �����ϰ� �ִ�. ���� ���� ��� ����?;
	map<float, GEntityActor*> mapEffectTargetDistance;
	for each (GEntityActor* pEffectTarget in outvecEffectTarget)
	{
		if (!pEffectTarget) 
			continue;

		float fDistance = vCenterPoint.DistanceTo(pEffectTarget->GetPos());
		mapEffectTargetDistance.insert(map<float, GEntityActor*>::value_type(fDistance, pEffectTarget));
	}

	outvecEffectTarget.clear();
	map<float, GEntityActor*>::iterator itor = mapEffectTargetDistance.begin();	
	for (int i=0; i<nLimit; ++i)
	{
		outvecEffectTarget.insert((*itor).second);
		itor++;
	}
}

bool GEffectTargetSelector::HasParty(GEntityActor* pSelf) const
{
	VALID_RET(pSelf, false);

	// �÷��̾ ��Ƽ�� ���� �� ����
	if (!pSelf->IsPlayer())				return false;

	// ��Ƽ�� �ִ°�?
	GEntityPlayer* pSelfPlayer = ToEntityPlayer(pSelf);	
	return gsys.pPartySystem->IsPartyMember(pSelfPlayer);
}
