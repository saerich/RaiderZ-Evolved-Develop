#include "stdafx.h"
#include "GCollision.h"
#include "GEntityActor.h"
#include "GField.h"

#include "GTalent.h"
#include "GMessageID.h"
#include "GEntityNPC.h"
#include "GCommandTable.h"
#include "GConfig.h"
#include "GMath.h"
#include "GModuleCombat.h"
#include "GGlobal.h"
#include "GNPCInfoMgr.h"
#include "GAIProfiler.h"
#include "GFactionSystem.h"
#include "GTestSystem.h"
#include "GConst.h"

bool GCollision::HitTest2( GEntityActor* pThisEntity, MCapsule& tarCapsule, int8& noutHitCapsuleGroup, int8& noutHitCapsuleindex )
{
	if (pThisEntity->IsDead()) return false;

	GActorMeshInfo* pMeshInfo = pThisEntity->GetMeshInfo();
	if (pMeshInfo == NULL) return false;

	noutHitCapsuleGroup = INVALID_ID;
	noutHitCapsuleindex = INVALID_ID;

	if (pMeshInfo->IsSimple())
	{
		MCapsule thisCapsule = pMeshInfo->GetSimpleCapsule();
		thisCapsule.bottom += pThisEntity->GetPos();
		thisCapsule.top += pThisEntity->GetPos();

		//mlog("hit_cap: top=%.1f %.1f %.1f, bottom=%.1f %.1f %.1f, height=%.1f, radius=%.1f\n",
		//	tarCapsule.top.x, tarCapsule.top.y, tarCapsule.top.z,
		//	tarCapsule.bottom.x, tarCapsule.bottom.y, tarCapsule.bottom.z,
		//	tarCapsule.height, tarCapsule.radius
		//	);

		//mlog("chr_cap: top=%.1f %.1f %.1f, bottom=%.1f %.1f %.1f, height=%.1f, radius=%.1f\n",
		//	thisCapsule.top.x, thisCapsule.top.y, thisCapsule.top.z,
		//	thisCapsule.bottom.x, thisCapsule.bottom.y, thisCapsule.bottom.z,
		//	thisCapsule.height, thisCapsule.radius
		//	);

//		mlog("char: pos=%.1f %.1f %.1f\n",
//			pThisEntity->GetPos().x, pThisEntity->GetPos().y, pThisEntity->GetPos().z
//			);

		gsys.pTestSystem->ClientRenderHitCapsule(pThisEntity, tarCapsule);

		return thisCapsule.CollisionCapsule(tarCapsule);
	}
	else if (pThisEntity->IsNPC())
	{
		GEntityNPC* pThisEntityNPC = ToEntityNPC(pThisEntity);
		int nCurCapsuleGroupIndex = pThisEntityNPC->GetCapsuleGroupIndex();
		if (!pMeshInfo->HasHitGroup(nCurCapsuleGroupIndex))
		{
			//ASSERT(0);  ��Ʈĸ���׷��� �ٲ�� ���� �ǰ��ϸ� �ش�� �� ����
			return false;
		}

		const CSHitGroup& CapsuleGroup = pMeshInfo->GetHitGroup(nCurCapsuleGroupIndex);
		float fHitCapsuleDistance = 0.0f;
		for (size_t i=0; i<CapsuleGroup.m_vecHitCapsules.size(); i++)
		{
			const CSHitCapsule& hitCapsule = CapsuleGroup.m_vecHitCapsules[i];

			if (hitCapsule.IsAlwaysActive() == false)
			{
				float fTalentElapsedTime = pThisEntity->GetModuleCombat()->GetTalentActElapsedTime();
//				mlog("%s - idx:%d, cap_time:%.1f/%.1f, act_time:%.1f\n", __FUNCTION__, i, hitCapsule.GetActiveTimeBegin(), hitCapsule.GetActiveTimeEnd(), fTalentElapsedTime);
				if (hitCapsule.CheckActiveTime(fTalentElapsedTime) == false) 
				{
					continue;
				}
			}

			MCapsule thisCapsule = hitCapsule.m_Capsule;
			vec3 dir = pThisEntity->GetDir();
			GMath::NormalizeZ0(dir);

			mat4 matTM;
			matTM.SetLocalMatrix(vec3::ZERO, -dir, vec3::AXISZ);

			thisCapsule.top = pThisEntity->GetPos() + thisCapsule.top * matTM;
			thisCapsule.bottom = pThisEntity->GetPos() + thisCapsule.bottom * matTM;

			gsys.pTestSystem->ClientRenderHitCapsule(pThisEntity, tarCapsule);

			if (tarCapsule.CollisionCapsule(thisCapsule)) 
			{
				if (hitCapsule.IsHitFirst())
				{
					noutHitCapsuleGroup = nCurCapsuleGroupIndex;
					noutHitCapsuleindex = static_cast<char>(i);
					return true;
				}

				float fDistance = tarCapsule.GetCenter().DistanceTo(thisCapsule.GetCenter());
				if (noutHitCapsuleindex == INVALID_ID ||	// ó�� ���ݹ޴� ĸ���̰ų�
					fDistance < fHitCapsuleDistance)	// �� ����� ĸ���� �¾Ҵٸ�
				{
					fHitCapsuleDistance = fDistance;
					noutHitCapsuleGroup = nCurCapsuleGroupIndex;
					noutHitCapsuleindex = static_cast<char>(i);
				}
			}
		}

		if (noutHitCapsuleGroup != INVALID_ID)
			return true;
	}
	else
		assert(0);

	return false;
}

bool GCollision::HitTest3( GEntityActor* pEntityActor, const MVector3& rayOrigin, const MVector3& rayDir, float &outDistance )
{
	if (pEntityActor->IsDead()) return false;

	GActorMeshInfo* pMeshInfo = pEntityActor->GetMeshInfo();
	if (pMeshInfo == NULL) return false;

	if (pMeshInfo->IsSimple())
	{
		MCapsule thisCapsule = pMeshInfo->GetSimpleCapsule();
		thisCapsule.bottom += pEntityActor->GetPos();
		thisCapsule.top += pEntityActor->GetPos();


		return thisCapsule.IntersectRay(rayOrigin, rayDir, outDistance);
	}
	else if (pEntityActor->IsNPC())
	{
		GEntityNPC* pNPC = ToEntityNPC(pEntityActor);
		int nCurCapsuleGroupIndex = pNPC->GetCapsuleGroupIndex();
		if (pMeshInfo->HasHitGroup(nCurCapsuleGroupIndex))
		{
			_ASSERT(0);
			return false;
		}

		const CSHitGroup& CapsuleGroup = pMeshInfo->GetHitGroup(nCurCapsuleGroupIndex);

		for (size_t i=0; i<CapsuleGroup.m_vecHitCapsules.size(); i++)
		{
			const CSHitCapsule& hitCapsule = CapsuleGroup.m_vecHitCapsules[i];

			MCapsule thisCapsule = hitCapsule.m_Capsule;
			vec3 dir = pEntityActor->GetDir();
			GMath::NormalizeZ0(dir);

			mat4 matTM;
			matTM.SetLocalMatrix(vec3::ZERO, -dir, vec3::AXISZ);

			thisCapsule.top = pEntityActor->GetPos() + thisCapsule.top * matTM;
			thisCapsule.bottom = pEntityActor->GetPos() + thisCapsule.bottom * matTM;

			if (thisCapsule.IntersectRay(rayOrigin, rayDir, outDistance))
				return true;
		}
	}
	else
		assert(0);

	return false;
}

bool GCollision::HitTest( GEntityActor* pThisEntity, GEntityActor* pTarEntity, float fToler )
{
	MSphere tarSphere;
	tarSphere.radius = max(0.0f, pTarEntity->GetHitRadius() + fToler);
	tarSphere.center = pTarEntity->GetPos();
	tarSphere.center.z += tarSphere.radius;

	return HitTest(pThisEntity, tarSphere);
}

bool GCollision::HitTest( GEntityActor* pThisEntity, MSphere& tarSphere )
{
	if (pThisEntity->IsDead()) return false;

	float fRadiusDist = pThisEntity->GetHitRadius() + tarSphere.radius;

	vec3 pos = pThisEntity->GetPos();
	pos.z += pThisEntity->GetHitRadius();

	float fDist = pos.DistanceTo(tarSphere.center);
	if (fDist > fRadiusDist) return false;

	return true;
}

bool GCollision::HitTest1( GEntityActor* pThisEntity, GTalentInfo* pTalentInfo, int nTalentColIndex, const vec3& vOwnerPos, const vec3& vOwnerDir, int8& nHitCapsuleGroup, int8& nHitCapsuleindex )
{
	CSTalentHitInfo& hit_info = pTalentInfo->m_HitInfo;

	if ((nTalentColIndex < 0) || (nTalentColIndex >= (int)hit_info.m_vSegments.size())) return false;

	vec3 dir = vOwnerDir;
	GMath::NormalizeZ0(dir);

	mat4 matTM;
	matTM.SetLocalMatrix(vec3::ZERO, -dir, vec3::AXISZ);

	nHitCapsuleindex = INVALID_ID;

	MCapsule capTalent;
	for (size_t k = 0; k < hit_info.m_vSegments[nTalentColIndex].m_vCapsules.size(); ++k)
	{
		capTalent = hit_info.m_vSegments[nTalentColIndex].m_vCapsules[k];
		capTalent.top = vOwnerPos + capTalent.top * matTM;
		capTalent.bottom = vOwnerPos + capTalent.bottom * matTM;

		//mlog("use_talent: pos=%.1f %.1f %.1f, dir=%.1f %.1f %.1f\n",
		//	vOwnerPos.x, vOwnerPos.y, vOwnerPos.z, 
		//	dir.x, dir.y, dir.z
		//	);
		
		if (HitTest2(pThisEntity, capTalent, nHitCapsuleGroup, nHitCapsuleindex))
		{
			return true;
		}
	}

	return false;
}

void GCollision::HitTestTalent( GTalent* pAttackTalent, int nHitColIndex, set<MUID>* psetAlreadyVictim, vector<TALENT_TARGET_INFO>& outvecVictim)
{
	AI_PFI_BLOCK(APT_ID_HIT_COLLISION, L"HitTestTalent");

	GEntityActor* pAttacker = pAttackTalent->GetOwner();
	GField* pField = pAttacker->GetField();
	GTalentInfo* pTalentInfo = pAttackTalent->GetInfo();

	GFieldGrid::EntitySelector* grid_selector = 
		pField->GetGrid().GetEntitySelector();

	GVectorMUID vecMUID;
	vector<MUID>& vecActors = vecMUID.Vector();
	class CollectActors : public GFieldGrid::Sector::IVisitor
	{
	public:
		CollectActors(vector<MUID>& actors) : m_actors(actors) { }

	private:
		void OnVisit(const MUID& uid)
		{
			m_actors.push_back(uid);
		}

	private:
		vector<MUID>& m_actors;
	};
	grid_selector->VisitIntersectWithAABB(CollectActors(vecActors), ETID_ACTOR, pAttacker->GetPos(), pTalentInfo->GetHitAABB());

	if (pAttacker->IsPlayer())
	{
		grid_selector->VisitInHugeSectors(CollectActors(vecActors), ETID_ACTOR, pAttacker->GetSectorPoint());
	}

	sort(vecActors.begin(), vecActors.end());
	vecActors.erase(unique(vecActors.begin(), vecActors.end()), vecActors.end());

	for each (const MUID& uid in vecActors)
	{
		if (pTalentInfo->m_EffectInfo.m_nRelation != CSEffectInfo::RELATION_ALLIED &&
			uid == pAttacker->GetUID())
		{
			continue; // �Ʊ����� üũ���� �������� �ڱ� �ڽ��� �˻����� ����
		}

		GEntityActor* pTarActor = pField->FindActor(uid);
		if (pTarActor == NULL) 
			continue;

		if (false == pTalentInfo->IsEffective(pAttacker, pTarActor))
		{
			continue;
		}

		// �̹� �� �ŷ�Ʈ�� �ǰݴ��� ����� �� �ٽ� �ǰݵ��� �ʴ´�.
		if (pTalentInfo->m_bSeparateHit == false && psetAlreadyVictim)
		{
			if (psetAlreadyVictim->find(pTarActor->GetUID()) != psetAlreadyVictim->end()) 
				continue;
		}

		if (pTarActor->IsHittable(pAttacker, pTalentInfo) == false)
		{
			continue;
		}

		TALENT_TARGET_INFO infoTarget;
		infoTarget.uidTarget = uid;
		
		vec3 vOriginPos;
		vec3 vOriginDir;

		if (pTalentInfo->m_bHitCapsulePosSync) 
		{
			GEntityActor* pOwner = pAttackTalent->GetOwner();
			if (pOwner &&
				pOwner->IsPlayer())
			{
				// �÷��̾��� ��쿡�� �ŷ�Ʈ �̵� ���ø� �����Ͱ� �����Ƿ� ���� ��ǥ�� �����մϴ�.
				vOriginPos = pOwner->GetPos();
				vOriginDir = pAttackTalent->GetInitDir();	// ������ ���� �� �� ����
			}
			else
			{
				GTalentMove::MOVEINFO expectedMoveInfo = pAttackTalent->GetExpectedMoveInfo(pAttackTalent->GetActElapsedTime()+GConst::PRE_HIT_CHECK_TIME);
				vOriginPos = expectedMoveInfo.vPos;
				vOriginDir = expectedMoveInfo.vDir;
			}
		}
		else
		{
			vOriginPos = pAttackTalent->GetInitPos();
			vOriginDir = pAttackTalent->GetInitDir();
		}

		// �ŷ�Ʈ�� ĸ���� ��ǥ�� �浹ĸ���� �����Ǹ� �ǰ�
		if (GCollision::HitTest1(pTarActor, pTalentInfo, nHitColIndex, vOriginPos, vOriginDir, infoTarget.nCapsuleGroupIndex, infoTarget.nCapsuleIndex))
		{
			outvecVictim.push_back(infoTarget);
		}
		// ��ǥ�� �����ִ� ���� ������ �ǰ� (��Ʈĸ���׷��� -1�̸� ����)
		else
		{
			bool bCheckOverlap = IsCheckableOverlappedCol(pAttacker, pTarActor);

			if (bCheckOverlap && pTalentInfo->m_bHitOverlap)
			{
				float fToler = pAttacker->GetHitRadius() * 0.2f;

				if (GCollision::HitTest(pTarActor, pAttacker, -fToler))
				{
					outvecVictim.push_back(infoTarget);
					continue;
				}
			}

		}
	}
}

bool GCollision::IsCheckableOverlappedCol( GEntityActor* pAttacker, GEntityActor* pVictim )
{
	bool bCheckOverlap = false;

	if (pVictim->IsPlayer() ||
		(pVictim->IsNPC() && ToEntityNPC(pVictim)->GetCapsuleGroupIndex() != INVALID_HITCAPSULEGROUP_ID) ||
		pVictim->GetTypeID() == ETID_ACTOR)
	{
		bCheckOverlap = true;
	}

	if (pAttacker->IsNPC())
	{
		GEntityNPC* pAttackerNPC = ToEntityNPC(pAttacker);
		GNPCInfo* pNPCInfo = pAttackerNPC->GetNPCInfo();

		if (pNPCInfo && 
			CSAltitudeHelper::IsColliable(pNPCInfo->nAltitude) == false)
		{
			bCheckOverlap = false;
		}
	}

	return bCheckOverlap;
}

