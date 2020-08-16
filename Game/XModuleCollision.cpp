#include "StdAfx.h"
#include "XModuleCollision.h"
#include "RSceneManager.h"
#include "XObject.h"
#include "XModuleEntity.h"
#include "XModuleTalent.h"
#include "XModuleMovable.h"
#include "XGame.h"
#include "RUtil.h"
#include "XNonPlayer.h"
#include "CSMeshInfo.h"
#include "XCharacter.h"
#include "RMesh.h"
#include "XCollisionTree.h"

#include "RDebugPassRenderer.h"
#include "RCameraSceneNode.h"
#define COL_AREA_CENTER_RADIUS		100
#define TARGET_CHECK_OBJECT_DIST	1.0f
#define TARGET_CHECK_LINE_DIST		35.0f
/////////////////////////////////////////////////////////////////////////////////
XModuleCollision::XModuleCollision(XObject* pOwner) : XModule(pOwner),
m_pModuleEntity(NULL), m_pMeshInfo(NULL)
{
	m_MoveCollision.SetRadius(DEFAULT_ENTITY_RADIUS);
	m_MoveCollision.SetMoveAllowedHeight(MOVE_ALLOWED_HEIGHT);
	
	m_ObjectCollisionDebug.Init(pOwner);
}

XModuleCollision::~XModuleCollision(void)
{
}

void XModuleCollision::OnInitialized()
{
	XModule::OnInitialized();

	m_pModuleEntity = m_pOwner->GetModuleEntity();

	InitHitCapsuleIndex();
	_ASSERT(m_pModuleEntity);
}

bool XModuleCollision::GroundMoveTestMap(const RVector &from, const RVector &to, RVector& vOut, RVector& vOutNor, bool bJump, bool *pSliding, bool *pFontCol, vector<MUID>* pvecTerrainColObjectList)	// from->to �� �̵��Ҷ� �����Ѹ�ŭ�� pOut���� ����
{
	// �浹�� ������ �����ΰ�?
	if(CheckTerrainCollisionUsable() == false)
	{
		// �ƴ϶��... �׳� ���� ������ ����������.
		vOut = to;
		vOut.z = from.z;
		vOutNor = vec3::AXISZ;
		return true;
	}

	// ������ ��ŭ �÷��ش�.
	vec3 vFrom	= from;
	vec3 vTo	= to;
	vFrom.z += GetRadius();
	vTo.z += GetRadius();

	MCapsule colCapsule = GetCollisionCapsuleByIndex(0);
	if(m_pOwner->GetUID() != XGetMyUID())
	{
		// �ٸ� �÷��̾�� �浹�� �ϳ��� �Ѵ�.
		vec3 vCapsuleDir = (colCapsule.top - colCapsule.bottom).Normalize();
		colCapsule.top = colCapsule.bottom + (vCapsuleDir * colCapsule.radius * 2.0f);
		colCapsule.CalcHeight();
	}

	if(bJump)
		m_MoveCollision.SetMoveAllowedHeight(0.0f);
	else
		m_MoveCollision.SetMoveAllowedHeight(MOVE_ALLOWED_HEIGHT);

	if(gg.currentgamestate->GetWorld())
	{
		XFieldInfo* pFieldInfo = gg.currentgamestate->GetWorld()->GetInfo();
		if(pFieldInfo)
			m_MoveCollision.SetRestrictedArea(pFieldInfo->m_rtBoundary.left, pFieldInfo->m_rtBoundary.top, pFieldInfo->m_rtBoundary.right, pFieldInfo->m_rtBoundary.bottom);
	}

	vector<RCollisionPolygon> vecTerrainColObject;
	BuildTerrainColObjectPolygon(pvecTerrainColObjectList, vecTerrainColObject);
	m_MoveCollision.SetTerrainColObject(&vecTerrainColObject);

	m_MoveCollision.m_CollisionDebug.Show(gvar.Debug.bShowMoveCollision, gvar.Debug.bShowMoveCollision_Poly, gvar.Debug.bShowmovecollision_Down, gvar.Debug.bShowMoveCollision_Real, gvar.Debug.bShowMoveCollision_View, gvar.Debug.bShowmovecollision_Investigate, gvar.Debug.nShowMoveCollision_InvestigateIndex);
	bool bResult = m_MoveCollision.GroundMoveTest(vFrom,vTo, from, colCapsule, vOut, vOutNor, pSliding, pFontCol);

	// ������ ��ŭ ���ش�.
	vOut.z -= GetRadius();

	return bResult;
}

bool XModuleCollision::WaterMoveTestMap( const RVector &from, const RVector &to, float fWaterHeightLimit, RVector& vOut, RVector& vOutNor, vector<MUID>* pvecTerrainColObjectList )
{
	// �浹�� ������ �����ΰ�?
	if(CheckTerrainCollisionUsable() == false)
	{
		// �ƴ϶��... �׳� ���� ������ ����������.
		vOut = from;
		vOutNor = vec3::AXISZ;
		return true;
	}

	// ������ ��ŭ �÷��ش�.
	vec3 vFrom	= from;
	vec3 vTo	= to;
	vFrom.z += GetRadius();
	vTo.z += GetRadius();

	MCapsule colCapsule = GetCollisionCapsuleByIndex(0);
	if(m_pOwner->GetUID() != XGetMyUID())
	{
		// �ٸ� �÷��̾�� �浹�� �ϳ��� �Ѵ�.
		vec3 vCapsuleDir = (colCapsule.top - colCapsule.bottom).Normalize();
		colCapsule.top = colCapsule.bottom + (vCapsuleDir * colCapsule.radius * 2.0f);
		colCapsule.CalcHeight();
	}

	m_MoveCollision.SetMoveAllowedHeight(MOVE_ALLOWED_HEIGHT);

	vector<RCollisionPolygon> vecTerrainColObject;
	BuildTerrainColObjectPolygon(pvecTerrainColObjectList, vecTerrainColObject);
	m_MoveCollision.SetTerrainColObject(&vecTerrainColObject);

	m_MoveCollision.m_CollisionDebug.Show(gvar.Debug.bShowMoveCollision, gvar.Debug.bShowMoveCollision_Poly, gvar.Debug.bShowmovecollision_Down, gvar.Debug.bShowMoveCollision_Real, gvar.Debug.bShowMoveCollision_View, gvar.Debug.bShowmovecollision_Investigate, gvar.Debug.nShowMoveCollision_InvestigateIndex);
	bool bResult = m_MoveCollision.WaterMoveTest(vFrom,vTo, from, colCapsule, fWaterHeightLimit, vOut, vOutNor);

	// ������ ��ŭ ���ش�.
	vOut.z -= GetRadius();

	return bResult;
}

void XModuleCollision::InitCol(CSMeshInfo* pMeshInfo)
{
	// �ݰ��� �浹 ù��° ĸ������ �����´�.
	float fRadius = DEFAULT_ENTITY_RADIUS;
	m_pMeshInfo = pMeshInfo;

	if ( m_pMeshInfo ) 
	{
		if (m_pMeshInfo->m_vecColCapsules.size() > 0)
		{
			MCapsule cap = m_pMeshInfo->m_vecColCapsules[0].m_Capsule;
			fRadius = cap.radius;
		}
	}

	m_MoveCollision.SetRadius(fRadius);
}

float XModuleCollision::GetRadius()
{
	return m_MoveCollision.GetRadius();
}

//------------------------------------------------------------------------
// Pick
//entity type Id�� ��ü�� üũ�ؼ� ���ܽ�Ű��
//CSMeshHitInfo�� ��� ���� ������ ���� �� �ִ�.
//���� ��ü���� �ε��ļ� ȿ���� ���´ٸ� ???
bool XModuleCollision::Pick( vec3& old_pos, vec3& pos, const vec3& dir )
{
	float fDist = 0.0f;

	return Pick(old_pos, pos, dir, fDist);
}

bool XModuleCollision::Pick( vec3& old_pos, vec3& pos, const vec3& dir, float& fDistance )
{
	CSMeshInfo* c = GetMyObjectMeshInfo();
	if( c == NULL) return false;

	CSHitGroup * pHitGroup = c->FineHitGroup(GetOpenHitGroupID());
	if(pHitGroup == NULL) return false;

	mat4 mm = mat4::IDENTITY;
	mm.SetTranslation(m_pOwner->GetPosition());
	mm.SetScale(m_pOwner->GetScale());

	fDistance				= 0.f;
	bool bCol				= false;
	float fMinDist			= 0.0f;
	
	int nHitCapsuleCount	= pHitGroup->GetHitCapsuleCount();
	for(int i = 0; i < nHitCapsuleCount; i++)
	{
		CSHitCapsule * pHitCapsule = pHitGroup->GetHitCapsuleByCount(i);
		if(pHitCapsule)
		{
			MCapsule tCapsule = pHitCapsule->m_Capsule.Multiply(mm);
			if(tCapsule.IntersectRay(pos, dir, fMinDist))
			{
				if(old_pos.DistanceTo(pos) >= abs(fMinDist))
				{
					fDistance		= fMinDist;
					bCol			= true;
				}
			}
		}
	}

	return bCol;
}

// Pick
//------------------------------------------------------------------------
//------------------------------------------------------------------------
// PickRay
bool XModuleCollision::PickRayToObject( vec3& pos, const vec3& dir, float& fMaxDist, float& fOutDist )
{
	// �� �Լ��� ������Ʈ �浹 �ߴ��� ���ߴ��� ������ üũ �Լ���.
	MCapsule tempCapsule;
	vec3 vTempPos;
	return PickRayToColCapsule(pos, dir, fMaxDist, tempCapsule, vTempPos, fOutDist);
}

bool XModuleCollision::PickRayToTerrain( vec3& vstart, vec3& vend, vec3& voutPos, vec3& voutNor, DWORD dwCollisionAttribue /*= ~CollisionAttribute::WATER*/ )
{
	return gg.currentgamestate->GetWorld()->PickCollision(vstart, vend, &voutPos, &voutNor, dwCollisionAttribue);
}

bool XModuleCollision::PickRayToColCapsule( const vec3& pos, const vec3& dir, float fMaxDist, MCapsule& outCapsule, vec3& vOutPos, float& fOutDist )
{
	CSMeshInfo* c = GetMyObjectMeshInfo();
	if( c == NULL) return false;

	bool bCol = false;

	if(c->m_vecColCapsules.size() > 0)
	{
		int nCollisionCapsule	= GetCollisionCapsuleCount();
		float fMinDist			= fMaxDist;

		for(vector<CSColCapsule>::iterator itCol = c->m_vecColCapsules.begin(); itCol != c->m_vecColCapsules.end(); itCol++)
		{
			MCapsule tar_capsule = CalCollisionCapsuleAnimation(&(*itCol), m_pOwner);
			float fTempDist		= 0.0f;

			if(tar_capsule.IntersectRay(pos, dir, fTempDist))
			{
				if(fMinDist > fTempDist)
				{
					fOutDist			= fTempDist;
					outCapsule			= tar_capsule;
					vOutPos				= tar_capsule.GetCenter();				// ����

					bCol				= true;
				}
			}
		}		
	}

	return bCol;
}

bool XModuleCollision::PickRayFindNearBone( vec3& pos, RActorNode** ppOutBone /*= NULL*/ )
{
	if(m_pOwner->GetActor() == NULL)
	{
		return false;
	}

	float fNearDist = 9999.9f;
	int nActorNodeCount = m_pOwner->GetActor()->GetActorNodeCount();
	for(int i = 0; i < nActorNodeCount; i++)
	{
		RActorNode* pActorNode = m_pOwner->GetActor()->GetActorNodes()[i];
		if(pActorNode)
		{
			float fDist = (pActorNode->GetWorldPosition() - pos).Length();

			if(fDist < fNearDist)
			{
				fNearDist	= fDist;
				*ppOutBone	= pActorNode;
			}
		}
	}

	if(ppOutBone == NULL)
		return false;

	return true;
}

bool XModuleCollision::PickRayToHitEffectCapsule( const vec3& pos, const vec3& dir, float fMaxDist, MCapsule& outCapsule, vec3& vOutPos, float& fOutDist )
{
	CSMeshInfo* c = GetMyObjectMeshInfo();
	if( c == NULL) return false;

	bool bCol = false;

	if(c->m_vecHitEffectInfo.size() > 0)
	{
		float fMinDist			= fMaxDist;

		for(vector<CSColCapsule>::iterator itCol = c->m_vecHitEffectInfo.begin(); itCol != c->m_vecHitEffectInfo.end(); itCol++)
		{
			MCapsule tar_capsule = CalCollisionCapsuleAnimation(&(*itCol), m_pOwner);
			float fTempDist		= 0.0f;

			if(tar_capsule.IntersectRay(pos, dir, fTempDist))
			{
				if(fMinDist > fTempDist)
				{
					fMinDist			= fTempDist;
					fOutDist			= fTempDist;
					outCapsule			= tar_capsule;
					vOutPos				= pos + (dir * fTempDist);

					bCol				= true;
				}
			}
		}		
	}

	return bCol;
}

bool XModuleCollision::PickRayToHitEffectObject( vec3& pos, const vec3& dir, float& fMaxDist, vec3& vOutPos, float& fOutDist, int& nOutGroupID, int& nOutCapsuleID )
{
	// ��Ʈ ����Ʈĸ���� ��ŷ�ؼ� ��Ʈ ĸ������ ��������
	CSMeshInfo* c = GetMyObjectMeshInfo();
	if( c == NULL) return false;

	if(c->m_mapHitGroupInfo.size() > 0)
	{
		int nHitEffectCapsuleCount = GetHitEffectCapsuleCount();

		// ��Ʈ ����Ʈ ĸ�� 2�� �̻��� NPC
		float fMinDist			= fMaxDist;
		int nHitEffectIndex		= -1;
		int nHitPartsGroupID	= GetOpenHitGroupID();
		int nHitPartsCapsuleID	= -1;

		if(CheckErrorGroupID(nHitPartsGroupID))
			return false;

		int nIndex = 0;
		for(vector<CSColCapsule>::iterator itCol = c->m_vecHitEffectInfo.begin(); itCol != c->m_vecHitEffectInfo.end(); itCol++)
		{
			float fTempDist		= 0.0f;
			int nGroupID		= -1;
			int nCpaulseID		= c->FindPartsHitCapsuleIndex(nHitPartsGroupID, nIndex);


			MCapsule tar_capsule = CalCollisionCapsuleAnimation(&(*itCol), m_pOwner);

			if(tar_capsule.IntersectRay(pos, dir, fTempDist))
			{
				if(fMinDist > fTempDist)
				{
					fMinDist			= fTempDist;
					nHitEffectIndex		= nIndex;
					nHitPartsCapsuleID	= nCpaulseID;

					vOutPos				= tar_capsule.GetCenter();				// ����
				}
			}

			++nIndex;
		}

		if(nHitEffectIndex >= 0)
		{
			fOutDist		= fMinDist;
			nOutGroupID		= nHitPartsGroupID;
			nOutCapsuleID	= nHitPartsCapsuleID;

			return true;
		}
	}

	return false;
}

// PickRay
//------------------------------------------------------------------------
//------------------------------------------------------------------------
// LineRay
// LineRay�� ĸ�� ���͸� �������� ���Ѵ�.
bool XModuleCollision::LineRayToObject( vec3& pos, vec3& dir, float& fMaxDist, vec3& vOutPos, float& fOutDist )
{
	CSMeshInfo* c = GetMyObjectMeshInfo();
	if( c == NULL) return false;

	if(c->m_mapHitGroupInfo.size() > 0)
	{
		float fRadius					= m_MoveCollision.GetRadius();
		int nCollisionCapsuleCount		= GetCollisionCapsuleCount();

		// �浹 ĸ�� 2�� �̻��� NPC
		int nCollIndex	= -1;
		float fMinDis	= fMaxDist;

		for(int i = 0; i < nCollisionCapsuleCount; i++)
		{
			vec3 vCapsuleCenter			= GetCollisionCapsuleByIndex(i).GetCenter();

			// Sticky Ÿ�� ���� ���
			if(CalLineTarget(vCapsuleCenter, pos, dir, fRadius) > 0)
			{
				// Ÿ������ ������.
				float fNewDis = (vCapsuleCenter - pos).Length();
				if(fMinDis > fNewDis)
				{
					nCollIndex						= i;
					vOutPos							= vCapsuleCenter;
					fMinDis							= fNewDis;
				}				
			}
		}

		if(nCollIndex >= 0)
		{
			fOutDist = fMinDis;
			return true;
		}
	}

	return false;
}

bool XModuleCollision::LineRayToHitEffectObject( vec3& pos, vec3& dir, float& fMaxDist, vec3& vOutPos, float& fOutDist, int& nOutGroupID, int& nOutCapsuleID )
{
	CSMeshInfo* c = GetMyObjectMeshInfo();
	if( c == NULL) return false;

	if(c->m_mapHitGroupInfo.size() > 0)
	{
		float fRadius					= m_MoveCollision.GetRadius();
		int nHitEffectCapsuleCount		= GetHitEffectCapsuleCount();

		// ��Ʈ ����Ʈ ĸ�� 2�� �̻��� NPC
		int nHitEffectIndex	= -1;
		float fMinDis	= fMaxDist;
		nOutGroupID		= GetOpenHitGroupID();

		if(CheckErrorGroupID(nOutGroupID))
			return false;

		int nIndex = 0;
		for(vector<CSColCapsule>::iterator itCol = c->m_vecHitEffectInfo.begin(); itCol != c->m_vecHitEffectInfo.end(); itCol++)
		{
			int nCpaulseID		= c->FindPartsHitCapsuleIndex(nOutGroupID, nIndex);

			vec3 vCapsuleCenter = CalCollisionCapsuleAnimation(&(*itCol), m_pOwner).GetCenter();

			// Sticky Ÿ�� ���� ���
			float fTempDist		= 0.0f;
			if(CalLineTarget(vCapsuleCenter, pos, dir, fRadius) > 0)
			{
				// Ÿ������ ������.
				float fNewDis = (vCapsuleCenter - pos).Length();
				if(fMinDis > fNewDis)
				{
					nHitEffectIndex					= nIndex;
					vOutPos							= vCapsuleCenter;
					fMinDis							= fNewDis;

					nOutCapsuleID					= nCpaulseID;
				}				
			}

			++nIndex;
		}

		if(nHitEffectIndex >= 0)
		{
			fOutDist = fMinDis;
			return true;
		}
	}

	return false;
}

// LineRay
//------------------------------------------------------------------------

vec3 XModuleCollision::GetMeleeHitEffectPosition( XObject* pAttacker, int nTalentID )
{
	CSMeshInfo* cAttackerMeshInfo = GetOtherObjectMeshInfo(pAttacker);
	CSMeshInfo* cHitterMeshInfo = GetMyObjectMeshInfo();

	return m_HitEffectPosCalculator.GetPos(m_pOwner, cHitterMeshInfo, pAttacker, cAttackerMeshInfo, nTalentID);
}

vec3 XModuleCollision::GetColObjectCenter()
{
	CSMeshInfo* c = GetMyObjectMeshInfo();
	if(c == NULL ||
		(c && c->m_vecColCapsules.size() == 0)) 	
	{
		// ���� ĸ���� ������ �⺻���� AABB �߽ɰ��� �����´�.
		return m_pOwner->GetActor()->GetAABB().GetCenter() + m_pOwner->GetPosition();
	}

	MCapsule capsule = CalCollisionCapsuleAnimation(&(c->m_vecColCapsules[0]), m_pOwner);
	return capsule.GetCenter();
}

vec3 XModuleCollision::GetNotScaleColObjectCenter()
{
	CSMeshInfo* c = GetMyObjectMeshInfo();
	if(c == NULL ||
		(c && c->m_vecColCapsules.size() == 0)) 	
	{
		// ���� ĸ���� ������ �⺻���� AABB �߽ɰ��� �����´�.
		return m_pOwner->GetActor()->GetAABB().GetCenter() + m_pOwner->GetPosition();
	}

	MCapsule capsule = CalCollisionCapsuleAnimation(&(c->m_vecColCapsules[0]), m_pOwner);
	capsule.Scale(1.0f / m_pOwner->GetScale());
	return capsule.GetCenter();
}

CSMeshInfo * XModuleCollision::GetMyObjectMeshInfo()
{
	return m_pMeshInfo;
}

CSMeshInfo * XModuleCollision::GetOtherObjectMeshInfo(XObject * pObject)
{
	if(pObject == NULL)
		return NULL;

	XModuleEntity* pTarModuleEntity = pObject->GetModuleEntity();
	if ( !info.mesh_info ||
		!pTarModuleEntity ||
		!pTarModuleEntity->GetActor()) 
	{
		return NULL;
	}

	RMesh* pMesh = pTarModuleEntity->GetActor()->m_pMesh;
	if (pMesh == NULL) 
	{
		return NULL;
	}

	wstring n = MLocale::ConvAnsiToUTF16(pMesh->GetName().c_str());
	wstring nn = n.substr(n.rfind(L"/") + 1, n.rfind(L".") - n.rfind(L"/") - 1);

	return info.mesh_info->GetInfo(nn);
}

CSMeshInfo * XModuleCollision::GetMeshInfo()
{
	return GetMyObjectMeshInfo();
}

MCapsule XModuleCollision::GetMyObjectCollisionCapsule( int n )
{
	MCapsule tar_capsule;

	CSMeshInfo* c = GetMyObjectMeshInfo();
	if(c)
	{
		tar_capsule = CalCollisionCapsuleAnimation(&(c->m_vecColCapsules[n]), m_pOwner);
	}

	return tar_capsule;
}

MCapsule XModuleCollision::GetCollisionCapsuleByIndex( int n )
{
	return GetMyObjectCollisionCapsule(n);
}

class XInteractionObjectFinder
{
private:
	void RenderDebug( const RMatrix& WorldMat, const RBoundingBox& ColBox, const XObject* pTargetObj, const vec3& vPos )
	{
#ifdef _MADDUCK2
		global.smgr->GetPassRenderer< RDebugPassRenderer >()->AddDebugBox(WorldMat, (RBoundingBox&)ColBox, 0.1f );
		if( pTargetObj)
		{
			vec3 ownerpos;// = m_pOwner->GetPosition();
			//ownerpos.z += m_pOwner->GetActor()->GetAABB().GetCenter().z;
			ownerpos = vPos * WorldMat;
			vec3 rtrt = pTargetObj->GetActor()->GetAABB().GetCenter() /*+ pTargetObj->GetModuleCollision()->GetMeshSimpleCapsule().GetCenter()*/;
			RActorNode* pActorNode = pTargetObj->GetActor()->GetAnimatedActorNodePos(COLBOX_BONE, rtrt, E_TS_WORLD);
			//RActorNode* pActorNode = pTargetObj->GetActor()->GetUpdatedActorNode("Bip01 Spine1");
			//if( pActorNode )
			//{
			//	rtrt = pActorNode->GetWorldPosition();
			//}
			global.smgr->GetPassRenderer< RDebugPassRenderer >()->AddDebugLine(ownerpos, rtrt, 0.1f, D3DCOLOR_XRGB(255, 0, 0));
		}
#endif
	}

	vec3 CalcTargetInteractionPos( XObject* pObj, const RMatrix& WorleMatInv )
	{
		vec3 ObjPos;

		RBoundingBox ObjAABB = pObj->GetActor()->GetInteractionAABB();

		if ( ObjAABB.IsValid() == false )
		{
			ObjPos = pObj->GetActor()->GetPosition() * WorleMatInv;
		}
		else
		{
			ObjPos = ObjAABB.GetCenter() * pObj->GetWorldTransform() * WorleMatInv;
		}

		//pObj->GetActor()->GetAnimatedActorNodePos(COLBOX_BONE, ObjPos, E_TS_LOCAL_RESULT);
		RActorNode* pActorNode = pObj->GetActor()->GetActorNodePos(COLBOX_BONE, ObjPos, E_TS_WORLD, true);
		if( pActorNode )
		{
			ObjPos = ObjPos * WorleMatInv;
		}

		if (ObjPos.z > CHECK_INTERACTION_COL_Z)
		{
			ObjPos.z = CHECK_INTERACTION_COL_Z;
		}

		return ObjPos;
	}

	bool CheckTargetModulesEnabled( XObject* pObj )
	{
		XModuleEntity* pObjModuleEntity = pObj->GetModuleEntity();
		if (pObjModuleEntity == NULL) return false;
		if (pObjModuleEntity->IsLoadingCompleted() == false) return false;

		XModuleCollision* pObjCollision = pObj->GetModuleCollision();
		if (pObjCollision == NULL) return false;

		return true;
	}

	bool CheckInteractEnabled( XObject* pTarObject, XObject* m_pOwner, INTERACTIONTYPE FindType )
	{
		if (pTarObject == NULL) 
			return false;

		if( pTarObject == m_pOwner) 
		{
			return false;	//�ڱ�� ����
		}

		if( pTarObject->GetEntityType() != ETID_NPC && pTarObject->GetEntityType() != ETID_PLAYER )
			return false;

		switch (FindType)
		{
		case FIND_EXCEPTDEAD:
			{
				if( pTarObject->GetEntityType() == ETID_NPC )
				{
					XNonPlayer* pTargetNPC = pTarObject->AsNPC();
					if( pTargetNPC->IsDead() == true) return false;
				}
				else if( pTarObject->GetEntityType() == ETID_PLAYER)
				{
					XNetPlayer* pTargetPlayer = pTarObject->AsNetPlayer();
					if( pTargetPlayer->IsDead() == true) return false;
				}
			}
			break;
		case FIND_NETPLAYER:
			{
				if( pTarObject->GetEntityType() != ETID_PLAYER) return false;
			}
			break;
		case FIND_NPC:
			{
				if( pTarObject->GetEntityType() != ETID_NPC ) return false;
			}
			break;
		case FIND_DEADNPC:
			{
				if( pTarObject->GetEntityType() == ETID_NPC)
				{
					XNonPlayer* pTargetNPC = pTarObject->AsNPC();
					if(pTargetNPC->IsDead() == false) return false;
				}
				else
					return false;
			}
			break;
		case FIND_ALL:
			{
				if( pTarObject->GetEntityType() == ETID_NPC)
				{
					XNonPlayer* pTargetNPC = pTarObject->AsNPC();
					XNPCInfo* pNPCInfo		= pTargetNPC->GetInfo();
					if(pNPCInfo->nNpcType == NPC_TYPE_OBJECT)
					{
						if(pTargetNPC->GetIConState() == NIS_NONE) return false;
					}
				}
			}
			break;
		}


		return true;
	}
public:
	XInteractionObjectFinder()
	{

	}

	bool GetObjectColBox( XObject* pObject, INTERACTIONTYPE FindType, const vec3& vPos, const vec3& vDir, const RBoundingBox& ColBox, float& fDist )
	{
		float fOutDist = FLT_MAX;

		RMatrix WorldMat;
		WorldMat.SetLocalMatrix(vPos, vDir, UP_VECTOR);
		RMatrix WorleMatInv;
		WorldMat.GetInverse(&WorleMatInv);

		vec3 vCenterPos = CalcTargetInteractionPos(pObject, WorleMatInv);

		RenderDebug(WorldMat, ColBox, pObject, vPos);

		vec3 TempPos = vec3::ZERO;
		fOutDist = TempPos.DistanceTo(vCenterPos);
		if(ColBox.Contains(vCenterPos))
		{
			if (fOutDist < fDist)
			{
				fDist = fOutDist;
				return true;
			}
		}
		
		return false;
	}

	XObject* GetInteractionObjectColBox( XObject* pOwner, INTERACTIONTYPE FindType, const vec3& vPos, const vec3& vDir, const RBoundingBox& ColBox, float& fDist )
	{
		XObject* pTargetObj = NULL;

		float fOutDist		= FLT_MAX;

		RMatrix WorldMat = pOwner->GetWorldTransform();
		RMatrix WorleMatInv;
		WorldMat.GetInverse(&WorleMatInv);

		for (XObjectMap::iterator itor = gg.omgr->BeginItor(); itor != gg.omgr->EndItor(); ++itor)
		{
			XObject* pObj = (*itor).second;

			if (CheckInteractEnabled(pObj, pOwner, FindType) == false)
			{
				continue;
			}

			if (CheckTargetModulesEnabled(pObj) == false)
			{
				continue;
			}

			vec3 vCenterPos = CalcTargetInteractionPos(pObj, WorleMatInv);

			fOutDist = vPos.DistanceTo(vCenterPos);

			if(ColBox.Contains(vCenterPos))
			{
				if (fOutDist < fDist)
				{
					pTargetObj = pObj;
					fDist = fOutDist;
				}
			}
		}

		RenderDebug(WorldMat, ColBox, pTargetObj, vPos);

		return pTargetObj;
	}

	void GetInteractionObjectListColBox( vector<XObject*>& outvecObjectList, XObject* pOwner, INTERACTIONTYPE FindType, const vec3& vPos, const vec3& vDir, const RBoundingBox& ColBox )
	{
		RMatrix WorldMat = pOwner->GetWorldTransform();
		RMatrix WorleMatInv;
		WorldMat.GetInverse(&WorleMatInv);

		for (XObjectMap::iterator itor = gg.omgr->BeginItor(); itor != gg.omgr->EndItor(); ++itor)
		{
			XObject* pTarget = (*itor).second;

			if (CheckInteractEnabled(pTarget, pOwner, FindType) == false)
			{
				continue;
			}

			if (CheckTargetModulesEnabled(pTarget) == false)
			{
				continue;
			}

			vec3 vCenterPos = CalcTargetInteractionPos(pTarget, WorleMatInv);

			float fValidInteractionDistance = pTarget->GetValidInteractionDistance();

			if(ColBox.Contains(vCenterPos))
			{
				float fTarDist = vPos.DistanceTo(vCenterPos);

				if (fTarDist < fValidInteractionDistance)
				{
					outvecObjectList.push_back(pTarget);
				}
			}
		}
	}

};

bool XModuleCollision::GetObjectColBox( INTERACTIONTYPE FindType, vec3& vPos, vec3& vDir, RBoundingBox& ColBox, float& fDist )
{
	XInteractionObjectFinder interactionObjectCollector;

	return interactionObjectCollector.GetObjectColBox(m_pOwner, FindType, vPos, vDir, ColBox, fDist);
}

XObject* XModuleCollision::GetInteractionObjectColBox( INTERACTIONTYPE FindType, vec3& vPos, vec3& vDir, RBoundingBox& ColBox, float& fDist )
{
	XInteractionObjectFinder interactionObjectCollector;

	return interactionObjectCollector.GetInteractionObjectColBox(m_pOwner, FindType, vPos, vDir, ColBox, fDist);
}

void XModuleCollision::GetInteractionObjectListColBox( vector<XObject*>& outvecObjectList, INTERACTIONTYPE FindType, vec3& vPos, vec3& vDir, RBoundingBox& ColBox )
{
	XInteractionObjectFinder interactionObjectCollector;

	interactionObjectCollector.GetInteractionObjectListColBox(outvecObjectList, m_pOwner, FindType, vPos, vDir, ColBox);
}

void XModuleCollision::CheckXYTerrainCollision( const vec3& vEntityPosition, vec3 &vec, vec3 &out, float fPickAddOriginZ /*= 100.0f */ )
{
	out = vEntityPosition;
	out.x += vec.x;
	out.y += vec.y;
	vec3 next_pos = out;

	float fGroundZ = 0.0f;
	if (gg.currentgamestate && gg.currentgamestate->GetWorld())
		fGroundZ = gg.currentgamestate->GetWorld()->GetGroundZ(next_pos, fPickAddOriginZ);

	out.z = fGroundZ;
}

bool XModuleCollision::IsCheckXYFieldType( vec3& vEntityPosition, RCollisionTriangle::ATTRIBUTE attr)
{
	XCollisionTree* pCollisionTree = gg.currentgamestate->GetWorld()->GetCollisionTree();
	if (pCollisionTree == NULL) return false;

	const float PICK_MIN_Z = TERRAIN_HEIGHT_COLLISION_VALUE;
	RCollisionPickInfo pick_info;
	pick_info.m_inPickOrigin = vEntityPosition + RVector(0,0,100);
	pick_info.m_inPickTo = vEntityPosition + RVector(0,0,PICK_MIN_Z);

	if (pCollisionTree->Pick(pick_info, attr))
	{
		return true;
	}

	return false;
}

int XModuleCollision::GetCollisionCapsuleCount()
{
	CSMeshInfo* c = GetMyObjectMeshInfo();
	if(c)
	{
		return c->m_vecColCapsules.size();
	}

	return 0;
}

float XModuleCollision::CalLineTarget( vec3& vCenterPos, vec3& pos, vec3& dir, float& fRadius )
{
	// Line Test
	vec3 vPointInLine				= MMath::GetNearestPointOnRay(vCenterPos, pos, dir);
	float fDisPointLine				= (vPointInLine - vCenterPos).Length();
	float fDisCameraToPointInLine	= (pos - vPointInLine).Length();

	float fDis						= ((TARGET_CHECK_OBJECT_DIST * fDisCameraToPointInLine) / TARGET_CHECK_LINE_DIST) + (fRadius);

	if(fDis >= fDisPointLine)
	{
		return fDisPointLine;
	}

	return -1;
}

RActorNode* XModuleCollision::CheckLineTargetDummy( vec3& pos, vec3& dir, wchar_t* pDummyName )
{
	RActorNode* pResultNode			= NULL;
	float fRadius					= 20.0f;
	vec3 vCenterPos					= m_pOwner->GetPosition() + vec3(0, 0, fRadius);

	// ���̸� ã�´�.
	XCharacter * pActor				= m_pOwner->GetActor();
	if(pActor)
	{
		RVector vAnimatedPos;
		RActorNode * pNearNode		= pActor->GetActorNodePos(pDummyName, vAnimatedPos, E_TS_WORLD);
		if(pNearNode)
		{
			float fDis = CalLineTarget(vAnimatedPos, pos, dir, fRadius);
			if(fDis > 0)
			{
				pResultNode = pNearNode;
			}
		}
	}

	return pResultNode;
}

vec3 XModuleCollision::GetColProjectileTargetPos( int nGroupID /*= -1 */, int nIndexID /*= -1*/ )
{
	vec3 vTargetPos(0, 0, 0);
	
	//------------------------------------------------------------------------
	// [1/10/2008 isnara] Ÿ�� ��� ������
	// �׷��� �ִ� Ÿ���� ������ Ÿ���̴�.
	if(nGroupID >= 0 && nIndexID >= 0)
	{
		if(IsHaveHitEffectCount())
		{
			vTargetPos = GetHitEffectProjectileTargetPos(nGroupID, nIndexID);
		}
		else
		{
			vTargetPos = GetHitCapsule(nGroupID, nIndexID).GetCenter();
		}
	}
	else
	{
		// ������ ĸ���� �˻��Ѵ�.
		vTargetPos = GetColObjectCenter();
	}

	return vTargetPos;	
}

bool XModuleCollision::PickTerrainWater( const vec3 &pos, vec3& vOut )
{
	vec3 vTo	= pos;
	vTo.z		+= 50000.0f;

	if (gg.currentgamestate == NULL || gg.currentgamestate->GetWorld() == NULL) return false;

	return gg.currentgamestate->GetWorld()->PickCollision(pos, vTo, &vOut, NULL, CollisionAttribute::WATER);
}

bool XModuleCollision::IsHaveHitCapsuleCount( int nGroupID )
{
	CSMeshInfo* c = GetMyObjectMeshInfo();
	if( c == NULL) return false;

	CSHitGroup * pHitGroup = c->FineHitGroup(nGroupID);
	if(pHitGroup == NULL) return false;

	return (pHitGroup->GetHitCapsuleCount() > 1);
}

int XModuleCollision::GetOpenHitGroupID()
{
	return m_HitCapsuleIndex.GetGroupIndex();
}

bool XModuleCollision::IsHaveCollisionCapsuleCount()
{
	return (GetCollisionCapsuleCount() > 1);
}

MCapsule XModuleCollision::GetHitCapsule( int nHitGroupID, int nHitCapsuleID )
{
	MCapsule tCapsule;

	CSMeshInfo* c = GetMyObjectMeshInfo();
	if( c == NULL) return tCapsule;

	CSHitGroup * pHitGroup = c->FineHitGroup(nHitGroupID);
	if(pHitGroup == NULL) return tCapsule;

	mat4 mm = mat4::IDENTITY;
	mm.SetTranslation(m_pOwner->GetPosition());
	mm.SetScale(m_pOwner->GetScale());

	float outDistance		= 0.f;
	bool bCol				= false;

	CSHitCapsule * pHitCapsule = pHitGroup->GetHitCapsuleByCount(nHitCapsuleID);
	if(pHitCapsule)
	{
		tCapsule = pHitCapsule->m_Capsule.Multiply(mm);
	}

	return tCapsule;
}

MCapsule XModuleCollision::CalCollisionCapsuleAnimation( CSColCapsule * pColCapsule, XObject * pTargetObj )
{
	MCapsule tarCapsule				= pColCapsule->m_Capsule;
	XModuleEntity * pModuleEntity	= pTargetObj->GetModuleEntity();

	if(pModuleEntity == NULL)
		return tarCapsule;

	XCharacter * pTargetActor		= pModuleEntity->GetActor();

	if(pTargetActor == NULL)
		return tarCapsule;

	RMatrix matActor				= pTargetObj->GetWorldTransform();

	// �⺻������ ���� ��ǥ�� �̵��Ѵ�.
	// �������� �������� ���������� ���ش�. �������� ��Ʈ�������� Transform�Ҷ� ������ �ȴ�. 
	// �׸��� ���������� ���̰��� �ٽ� ����ϸ� �ȴ�.
	tarCapsule.radius *= pTargetObj->GetScale();
	matActor.TransformVect(tarCapsule.top);
	matActor.TransformVect(tarCapsule.bottom);
	tarCapsule.CalcHeight();

	//------------------------------------------------------------------------
	// �� �ִϸ��̼��� �ϴ°�?
	vec3 vPosLocal, vPos;

	// TOP
	if (pTargetActor->GetActorNodePos(pColCapsule->m_strBoneTop.c_str(), vPos, E_TS_WORLD, true))
	{
		vPosLocal = vPos - pTargetObj->GetPosition();
		matActor.TransformVect(pColCapsule->m_Capsule.top, vPos);

		tarCapsule.top = vPos + vPosLocal;
	}

	// BOTTOM
	if (pTargetActor->GetActorNodePos(pColCapsule->m_strBoneBottom.c_str(), vPos, E_TS_WORLD, true))
	{
		vPosLocal = vPos - pTargetObj->GetPosition();
		matActor.TransformVect(pColCapsule->m_Capsule.bottom, vPos);

		tarCapsule.bottom = vPos + vPosLocal;
	}

	tarCapsule.CalcHeight();

	// bottom �� top ������ Ȯ���� ����
	if(tarCapsule.bottom.z > tarCapsule.top.z)
	{
		vec3 vtemp = tarCapsule.bottom;
		tarCapsule.bottom = tarCapsule.top;
		tarCapsule.top = vtemp;
	}

	return tarCapsule;
}

bool XModuleCollision::ColCapsuleToObject( MCapsule& start_Capsule, MCapsule& end_Capsule, vec3& vTo, vec3& vImpactPos, vec3& vImpactDir, float& fOutDist, bool bRub /*= true*/, XObjectCollisionDebug* pDebug /*= NULL*/ )
{
	CSMeshInfo * pMeshInfo = GetMyObjectMeshInfo();
	if(pMeshInfo == NULL)
	{
		return false;
	}

	// Test ->
	MCapsule colEndCapsule;
	MCapsule colResultCapsule;

	// <- Test

	bool bCol = false;
	vec3 vFrom_Center = end_Capsule.GetCenter();
	vec3 vFrom_NearPoint = vec3::ZERO;
	vec3 vFrom_NearPoint2 = vec3::ZERO;
	vec3 vCapsuleMoveDir = (vec3(end_Capsule.top.x, end_Capsule.top.y, 0.0f) - vec3(start_Capsule.top.x, start_Capsule.top.y, 0.0f)).Normalize();

	for(vector<CSColCapsule>::iterator itCol = pMeshInfo->m_vecColCapsules.begin(); itCol != pMeshInfo->m_vecColCapsules.end(); itCol++)
	{
		MCapsule tarCapsule = CalCollisionCapsuleAnimation(&(*itCol), m_pOwner);

		//------------------------------------------------------------------------
		// �˻� �ڰ��� �ִ°�...
		float fMinDist = (tarCapsule.GetCenter() - vFrom_Center).Length();
		if(fMinDist > (tarCapsule.height + tarCapsule.radius + end_Capsule.height + end_Capsule.radius))
		{
			continue;
		}

		// �浹 ��ġ ���	
		// ����� ��ǥ ���ϱ�
		vec3 vNearestPoint_target;
		vec3 vNearestPoint_end;
		vec3 vTempImpactPos;
		vec3 vTempImpactDir;
		float fTempOutDist = 0.0f;

		// üũ
		if(ColCapsuleToCapsule(end_Capsule, tarCapsule, vTempImpactPos, vTempImpactDir, fTempOutDist, &vNearestPoint_end, &vNearestPoint_target))
		{
			colEndCapsule = end_Capsule;
			colResultCapsule = tarCapsule;

			// ���� �浹 �˻縦 ���� ĸ���� �浹���� �ٷ� �����Ѵ�.
			vTempImpactDir.z = 0.0f;
			vTempImpactDir.Normalize();

			vec3 vMoveDir = vTempImpactDir * fTempOutDist;
			end_Capsule.bottom += vMoveDir;
			end_Capsule.top	+= vMoveDir;

			start_Capsule.bottom += vMoveDir;
			start_Capsule.top	+= vMoveDir;

			vFrom_NearPoint = vNearestPoint_end;
			vFrom_NearPoint2 = vNearestPoint_target;

			bCol = true;
		}
	}

	if(bCol)
	{
		// �ܺΰ��� ���߱� ���ؼ� ������ ���ؼ� �浹�� ���� �ٽ� �ۼ��Ѵ�. 
		vImpactDir = end_Capsule.GetCenter() - vFrom_Center;
		fOutDist = vImpactDir.Length();
		vImpactDir.Normalize();
		vImpactPos = vFrom_NearPoint + (vImpactDir * (fOutDist - end_Capsule.radius));

		if(pDebug)
			pDebug->SetCollisionInvestigateData(colEndCapsule, colResultCapsule, vFrom_NearPoint, vFrom_NearPoint2, vImpactPos, fOutDist);
	}

	return bCol;
}

bool XModuleCollision::ColCapsuleToCapsule( MCapsule& end_Capsule, MCapsule& target_Capsule, vec3& vImpactPos, vec3& vImpactDir, float& fOutDist, vec3* vNearFrom_Capsule /*= NULL*/, vec3* vNearTarget_Capsule /*= NULL*/ )
{
	// �浹 ��ġ ���	
	// ����� ��ǥ ���ϱ�
	float fradius_sum	= target_Capsule.radius + end_Capsule.radius;
	bool bFromParallel = false;
	bool bTargetParallel = false;

	vec3 vNearestPoint_from		= MMath::GetNearestPointOnLine(end_Capsule.bottom, end_Capsule.top, target_Capsule.bottom, target_Capsule.top, &bFromParallel);
	vec3 vNearestPoint_target	= MMath::GetNearestPointOnLine(target_Capsule.bottom, target_Capsule.top, end_Capsule.bottom, end_Capsule.top, &bTargetParallel);

	// ĸ���� ���̰� ��� ���Ǿ� ����̸� ��ǥ ���ϴ� ������ �ٲ۴�.
	if(end_Capsule.height == 0.0f)
	{
		vNearestPoint_target	= MMath::GetNearestPointOnLine(end_Capsule.GetCenter(), target_Capsule.bottom, target_Capsule.top);
		bTargetParallel = false;
		bFromParallel = false;
	}
	
	if(target_Capsule.height == 0.0f)
	{
		vNearestPoint_from		= MMath::GetNearestPointOnLine(target_Capsule.GetCenter(), end_Capsule.bottom, end_Capsule.top);
		bFromParallel = false;
		bTargetParallel = false;
	}

	//  �����ΰ�?
	if(bFromParallel || bTargetParallel)
	{
		// ��, �Ʒ� �Ǵ�
		// ��,
		vec3 vUp = end_Capsule.bottom - target_Capsule.top;
		if(vUp.z > 0 && vUp.Length() > fradius_sum)
			return false;

		// �Ʒ�
		vec3 vDown = end_Capsule.top - target_Capsule.bottom;
		if(vDown.z < 0 && vDown.Length() > fradius_sum)
			return false;

		// ���̸� Ÿ�� ���̷� �����.
		vNearestPoint_from.z = vNearestPoint_target.z;
	}

	// ����� �Ǹ����� ���� �Է�
	float fdiff			= (vNearestPoint_target - vNearestPoint_from).Length();
	if(fdiff < fradius_sum)
	{
		vec3 vTarTo		= vNearestPoint_from - vNearestPoint_target;
		vTarTo.Normalize();
		vImpactPos		= vNearestPoint_target + vTarTo * target_Capsule.radius;;
		fOutDist		= fradius_sum - fdiff;
		vImpactDir		= vTarTo;


		// ������ ���� �Է�...
		if(vNearFrom_Capsule)
		{
			*vNearFrom_Capsule = vNearestPoint_from;
		}
		if(vNearTarget_Capsule)
		{
			*vNearTarget_Capsule = vNearestPoint_target;
		}

		return true;
	}

	return false;
}

bool XModuleCollision::ColObject( vec3& vTo, vec3 vPos, vec3& vImpactPos, vec3& vImpactDir, float& fOutDist, vector<MUID>& vecTerrainColObjectList, bool bRub /*= true*/, bool bRadiusOffset /*= false*/ )
{
	if (gg.currentgamestate == NULL) return false;

	m_ObjectCollisionDebug.Show(gvar.Debug.bShowObjectCollision_Investigate, gvar.Debug.nShowObjectCollision_InvestigateIndex);

	// NPC���� �⺻�浹�� �ƴ� �κ� �浹 ĸ���� ������ �� �ִ�. �̷������� ���� �Լ��� ���� �⺻ �浹 ĸ���� ��� �� �� �ִ�.
	// ���� �Լ��� �ƽ����� ���� ������ �ʴ� �̻� �⺻���� ĸ���� ������ �´�. 
	// ���� �ƽ����� �浹 ĸ���� ������ٸ�... ������ �ȴ�. �ƽ����� �浹 ĸ���� ������ ����!
	MCapsule capsuleCollision = GetCollisionCapsuleByIndex(0);

	MMatrix mat;
	mat.MakeIdentity();
	mat.SetTranslation(vTo);
	MCapsule moveCapsuleCollision = capsuleCollision.Multiply(mat);

	// ������ offset�� �ֳ�?
	if(bRadiusOffset)
	{
		capsuleCollision.radius += XCONST::COLLISION_CAPSULE_RADIUS_OFFSET;
		moveCapsuleCollision.radius += XCONST::COLLISION_CAPSULE_RADIUS_OFFSET;
	}

	// ���� ������ �ʿ��Ѱ�?
	bool bUseVolumCapsule = false;
	vector<MCapsule> vecVolumCapsule;

	// �̵� ���̰� �������� �Ѿ��...
	if(vTo.Length() >= capsuleCollision.radius)
	{
		bUseVolumCapsule = true;

		// ���� ĸ���� �����.
		MakeVolumCapsule(capsuleCollision, moveCapsuleCollision, vecVolumCapsule);
	}

	XModuleTalent* pTargetTalent = m_pOwner->GetModuleTalent();
	bool bTalentActive = false;
	TALENT_SHOVE_TYPE shoveType = TST_NONE;
	if(pTargetTalent)
	{
		bTalentActive = pTargetTalent->IsActive();
		if(bTalentActive && pTargetTalent->GetInfo())
			shoveType = pTargetTalent->GetInfo()->m_eTalentShoveType;
	}

	float fMinCol	= 100000.0f;
	bool bCol		= false;
	bool bForceNotRub	= false;
	MCapsule capsuleMinForRub;
	MCapsule capsuleOrg;
	vec3 vNearestTargetPos;
	XModuleCollision * pTargetCollForRub = NULL;

	for (XObjectMap::iterator itor = gg.currentgamestate->GetObjectManager()->BeginItor(); itor != gg.currentgamestate->GetObjectManager()->EndItor(); ++itor)
	{
		XObject* pTarget = (*itor).second;
		if (pTarget == m_pOwner) continue;

		bool bMyPlayerToNetPlayerCol = false;

		// �÷��̾�ʹ� �浹���� �ʴ´�. ��, PVP�� ��쿡�� �浹üũ�� �Ѵ�.
		if (pTarget->GetEntityType() == ETID_PLAYER)
		{
			if (m_pOwner->GetEntityType() == ETID_PLAYER)
			{
				// �ŷ�Ʈ ����� �ƴϸ鼭 �̵����� �ƴ϶�� ���� �浹�˻縦 ���� �ʴ´�.
				if(m_pOwner->GetUID() == XGetMyUID())
				{
					vec3 vCheckVec = vTo;
					vCheckVec.z = 0.0f;

					if (vCheckVec.LengthSq() < 0.1f && bTalentActive == false)
					{
						continue;
					}
				}

				// NetPlayer�� ������ �浹 �������� üũ
				XPlayer* pMyPlayer = static_cast<XPlayer*>(m_pOwner);
				XPlayer* pTargetPlayer = static_cast<XPlayer*>(pTarget);
				if (pMyPlayer->IsCollidableTo(pTargetPlayer) == false)
				{
					continue;
				}
			}
			else
			{
				continue;
			}
		}
		else
		{
			// Shove Type �˻�
			XModuleTalent* pTargetModuleTalent = pTarget->GetModuleTalent();
			if(pTargetModuleTalent && pTargetModuleTalent->IsActive() && pTargetModuleTalent->GetInfo())
			{
				// NPC�� Pass�̸鼭 ���� Scrape�̸� Pass�� ó��
				if(pTargetModuleTalent->GetInfo()->m_eTalentShoveType == TST_PASS &&
					shoveType == TST_SCRAPE)
					continue;
			}
		}

		// �������� ���� �˻�
		XModuleCollision * pTargetModuleCollision = pTarget->GetModuleCollision();
		if(pTargetModuleCollision == NULL)
			continue;

		// �浹 ���� ����
		if (pTargetModuleCollision->IsBaseColliable() == false) 
		{
			continue;
		}

		// ���� �浹 �ڽ��� �ֳ�?
		CSMeshInfo* pMeshInfo = pTargetModuleCollision->GetMeshInfo();
		if(pMeshInfo && pMeshInfo->m_vecColBox.size() > 0)
		{
			vecTerrainColObjectList.push_back(pTarget->GetUID());			
		}

		//// ����ĸ���߿� ��ǥ ��ġ�� Ÿ�� ��ġ���� ������... ������ �����Ƿ� ����� �ʰ� �ؼ� ������� �ʰ� �Ѵ�.
		//bool bForceNotRubTest = false;
		//if(bRub && bUseVolumCapsule &&
		//	(moveCapsuleCollision.bottom.z - moveCapsuleCollision.radius < pTarget->GetPosition().z))
		//{
		//	bForceNotRubTest = true;
		//}

		// �浹 ��ġ ���	
		// ����� ��ǥ ���ϱ�
		vec3 vTempImpactPos;
		vec3 vTempImpactDir;
		float fTempDist = 100000.0f;

		bool bColCapsule = false;
		if(bRub == false/* || bForceNotRubTest == true*/)
		{
			vec3 vEnd = moveCapsuleCollision.GetCenter();
			vEnd.z = capsuleCollision.GetCenter().z;
			MCapsule capsuleColTest(capsuleCollision.GetCenter(), vEnd, capsuleCollision.radius);
			MCapsule capsuleMin;
			MCapsule capsuleOrgTemp;
			vector<vec3> vecPoint;
			m_MoveCollision.GetCollisionSphere(capsuleCollision, 0.0f, vecPoint);
			vector<MCapsule> vecTestColCapsule;
			for(vector<vec3>::iterator it = vecPoint.begin(); it != vecPoint.end(); ++it)
			{
				MCapsule colTest(*it, vec3(vEnd.x, vEnd.y, it->z), capsuleCollision.radius);
				vecTestColCapsule.push_back(colTest);
			}

			if(pTargetModuleCollision->GetColCapsuleToObjectNotRub(capsuleColTest, capsuleCollision, vecTestColCapsule, capsuleOrgTemp, capsuleMin, vNearestTargetPos, fTempDist))
			{
				if(fTempDist < fMinCol)
				{
					fMinCol = fTempDist;
					capsuleMinForRub = capsuleMin;
					capsuleOrg = capsuleOrgTemp;
					bCol		= true;
					pTargetCollForRub = pTargetModuleCollision;
					bForceNotRub = true;
				}
			}

			//if(bCol == false)
			//	REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugCapsule( capsuleColTest, 30, D3DCOLOR_XRGB(180, 180, 180));
		}
		else
		{
			if(bUseVolumCapsule)
			{
				// �����浹
				bColCapsule = pTargetModuleCollision->ColVolumCapsuleToCapsule(capsuleCollision, moveCapsuleCollision, vTo, vecVolumCapsule, vTempImpactPos, vTempImpactDir, vPos, fTempDist, bRub, &m_ObjectCollisionDebug);
			}
			else
			{
				// �Ϲ��浹
				bColCapsule = pTargetModuleCollision->ColCapsuleToObject(capsuleCollision, moveCapsuleCollision, vTo, vTempImpactPos, vTempImpactDir, fTempDist, bRub, &m_ObjectCollisionDebug);

				m_ObjectCollisionDebug.m_TempData.capStart = capsuleCollision;
				m_ObjectCollisionDebug.m_TempData.capEnd = moveCapsuleCollision;

				m_ObjectCollisionDebug.SetCollisionInvestigateData_ResultPos(vPos, vTo, vTempImpactDir, fTempDist);
				m_ObjectCollisionDebug.AddData();
			}
		}

		if(bColCapsule)
		{
			if(fTempDist < fMinCol)
			{
				vImpactPos	= vTempImpactPos;
				vImpactDir	= vTempImpactDir;
				fOutDist	= fTempDist;
				fMinCol		= fTempDist;
				bCol		= true;
				bForceNotRub = false;
			}
		}
	}

	if(bCol)
	{
		if(bRub == false || bForceNotRub == true)
		{
			vec3 vStart = capsuleOrg.bottom;
			vec3 vEnd = capsuleOrg.top;

			vec3 vNearest;
			float fBack_Len = ColCapsuleToLineForNotRub(vStart, vEnd, vNearestTargetPos, capsuleCollision.radius + capsuleMinForRub.radius, vNearest);

			//RMatrix mat;
			//mat.MakeIdentity();
			//mat.SetTranslation( vStart);
			//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBox( mat, 2, 30, D3DCOLOR_XRGB(180, 180, 180));
			//mat.SetTranslation( vEnd);
			//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBox( mat, 2, 30, D3DCOLOR_XRGB(255, 255, 255));
			//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugLine( vStart, vEnd, 30, D3DCOLOR_XRGB(255, 255, 255));

			//mat.SetTranslation( vNearest);

			//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBox( mat, 2, 30, D3DCOLOR_XRGB(255, 0, 255));

			if(vNearest == vStart)
			{
				//RMatrix mat;
				//mat.MakeIdentity();
				//mat.SetTranslation( vStart);
				//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBox( mat, 2, 30, D3DCOLOR_XRGB(180, 180, 180));
				//mat.SetTranslation( vEnd);
				//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBox( mat, 2, 30, D3DCOLOR_XRGB(255, 255, 255));
				//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugLine( vStart, vEnd, 30, D3DCOLOR_XRGB(255, 255, 255));

				//mat.SetTranslation( vNearestTargetPos);
				//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBox( mat, 2, 30, D3DCOLOR_XRGB(255, 0, 255));

				vec3 vTempImpactPos;
				vec3 vTempImpactDir;
				float fTempDist = 0.0f;
				MCapsule calCapsuleCollision = capsuleCollision;
				if(pTargetCollForRub->ColCapsuleToObject(capsuleCollision, calCapsuleCollision, vec3(0,0,0), vTempImpactPos, vTempImpactDir, fTempDist, bRub))
				{
					// �̵����� 0�̸� ���ڸ��̴�.
					if(fTempDist == 0.0f)
						return false;

					vImpactPos	= vTempImpactPos;

					vec3 vOrgTemp = capsuleCollision.top + vTo;
					vec3 vNewTemp = capsuleCollision.top + (vTempImpactDir * fTempDist);

					vImpactDir = (vNewTemp - vOrgTemp);
					fOutDist = vImpactDir.Length();
					vImpactDir.Normalize();

					//mat.SetTranslation( vTempImpactPos);
					//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBox( mat, 2, 10, D3DCOLOR_XRGB(255, 255, 0));
					return true;
				}

				//mat.SetTranslation( vTempImpactPos);
				//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBox( mat, 2, 10, D3DCOLOR_XRGB(200, 205, 50));

				return false;
			}

			vec3 vBackDir = (vStart - vEnd).Normalize();
			vec3 vBackPos = vNearest + (vBackDir * fBack_Len);

			vImpactDir = vBackDir;
			fOutDist = (vEnd - vBackPos).Length();

			//RMatrix mat;
			//mat.MakeIdentity();
			//mat.SetTranslation( vStart);
			//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBox( mat, 2, 30, D3DCOLOR_XRGB(180, 180, 180));
			//mat.SetTranslation( vEnd);
			//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBox( mat, 2, 30, D3DCOLOR_XRGB(255, 255, 255));
			//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugLine( vStart, vEnd, 10, D3DCOLOR_XRGB(255, 255, 255));

			//mat.SetTranslation( vNearest);
			//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBox( mat, 2, 30, D3DCOLOR_XRGB(255, 0, 255));

			//mat.SetTranslation( vBackPos);
			//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBox( mat, 2, 30, D3DCOLOR_XRGB(0, 255, 0));
		}

		return true;
	}

	return false;
}

bool XModuleCollision::ColVolumCapsuleToCapsule( MCapsule& start_Capsule, MCapsule& end_Capsule, vec3& vTo, vector<MCapsule>& vecVolumCapsule, vec3& vImpactPos, vec3& vImpactDir, vec3 vCurPos, float& fOutDist, bool bRub /*= true*/, XObjectCollisionDebug* pDebug /*= NULL*/ )
{
	if(vecVolumCapsule.size() == 0) 
		return false;

	// Test ->
	stObjectCollisionInvestigateData resultData;

	// <- Test

	vec3 vTargetPos = m_pOwner->GetPosition();

	float fMinCol = 100000.0f;
	bool bCol = false;
	for(vector<MCapsule>::iterator it = vecVolumCapsule.begin(); it != vecVolumCapsule.end(); ++it)
	{
		vec3 vTempImpactPos;
		vec3 vTempImpactDir;
		
		MCapsule capsuleTest = *it;

		if(capsuleTest.bottom.z < vTargetPos.z + capsuleTest.radius)
		{
			capsuleTest.bottom.z = vTargetPos.z + capsuleTest.radius;
			capsuleTest.top.z = capsuleTest.bottom.z + capsuleTest.height;
		}

		float fTempDist = 100000.0f;

		if(ColCapsuleToObject(*it, *it, vTo, vTempImpactPos, vTempImpactDir, fTempDist, bRub, pDebug))
		{
			if(fMinCol > fTempDist)
			{
				vImpactPos = vTempImpactPos;
				vImpactDir = vTempImpactDir;
				fOutDist = fTempDist;
				bCol =true;

				resultData = pDebug->m_TempData;

				resultData.capStart = start_Capsule;
				resultData.capEnd = end_Capsule;

				pDebug->SetCollisionInvestigateData(resultData);

				pDebug->SetCollisionInvestigateData_ResultPos(vCurPos, vTo, vTempImpactDir, fTempDist);
				pDebug->AddData();
			}
		}
	}

	return bCol;
}

void XModuleCollision::MakeVolumCapsule( MCapsule& start_Capsule, MCapsule& end_Capsule, vector<MCapsule>& vecOutVolumCapsule )
{
	float fHeight			= start_Capsule.height;
	float fDiameter			= start_Capsule.radius * 2.0f;
	int nMakeCapsuleCount	= 0;
	
	// ���̰� �������� �۳�?
	if(fHeight < fDiameter)
	{
		// �۴ٸ�... �ϳ��� ó���Ѵ�.
		nMakeCapsuleCount = 1;

		float fRadius = (start_Capsule.height / 2.0f) + start_Capsule.radius;
		MCapsule OnCapsule(start_Capsule.GetCenter(), end_Capsule.GetCenter(), fRadius);

		vecOutVolumCapsule.push_back(OnCapsule);
		return;
	}
	else
	{
		// ũ�ٸ�...
		// �⺻������ ��,�Ʒ� 2���� ����.
		// ��
		vecOutVolumCapsule.push_back(MCapsule(start_Capsule.top, end_Capsule.top, start_Capsule.radius));

		// �߰�
		nMakeCapsuleCount	= ((fHeight - fDiameter) / fDiameter) + 0.5f;
		vec3 vMoveDir		= (start_Capsule.bottom - start_Capsule.top).Normalize();
		vec3 vStartPos		= start_Capsule.top + vMoveDir * start_Capsule.radius;

		for(int i = 0; i< nMakeCapsuleCount; i++)
		{
			vec3 vBottomCenter	= start_Capsule.top + (vMoveDir * (i + 1) * fDiameter);
			vec3 vTopCenter		= end_Capsule.top + (vMoveDir * (i + 1) * fDiameter);
			vecOutVolumCapsule.push_back(MCapsule(vBottomCenter, vTopCenter, start_Capsule.radius));
		}

		// �Ʒ�
		vecOutVolumCapsule.push_back(MCapsule(start_Capsule.bottom, end_Capsule.bottom, start_Capsule.radius));
	}
}

bool XModuleCollision::GetCompareCollisionCapsule( XObject* pObject, MCapsule& compare_Capsule, MCapsule& out_Capsule, float fDist )
{
	if(pObject == NULL)
		return false;

	XModuleEntity* pModuleEntity = m_pOwner->GetModuleEntity();
	if (!pModuleEntity) 
		return false;

	// �⺻ ���� ����
	float fMinDist					= 99999.0f;
	float fMin_radiust				= 0.0f;
	int nCollisionCapsuleCount		= GetCollisionCapsuleCount();

	//------------------------------------------------------------------------
	// �浹 ĸ�� ������ �ִ� NPC
	RMatrix mTrans;

	//------------------------------------------------------------------------
	// ���� ����� ĸ�� ���ϱ�
	//  [4/16/2008 isnara] ���ο� �浹 ĸ���� ���
	CSMeshInfo * pMeshInfo = GetMyObjectMeshInfo();

	if (pObject != m_pOwner)
	{
		pMeshInfo = GetOtherObjectMeshInfo(pObject);
	}

	if(pMeshInfo == NULL)
	{
		return false;
	}

	if(pModuleEntity->GetActor())
	{
		vector<CSColCapsule>::iterator itCol = pMeshInfo->m_vecColCapsules.begin();
		while(itCol != pMeshInfo->m_vecColCapsules.end())
		{
			MCapsule tarCapsule = CalCollisionCapsuleAnimation(&(*itCol), m_pOwner);
			float fTempDis = compare_Capsule.GetCenter().DistanceTo(tarCapsule.GetCenter());

			if(fTempDis < fMinDist)
			{
				fMinDist		= fTempDis;
				fDist			= fTempDis;
				out_Capsule		= tarCapsule;
			}

			itCol++;
		}

		return true;
	}

	return false;
}

bool XModuleCollision::IsBaseColliable()
{
	// �⺻������ �浹 �������� ����.

	// 1. �浹 ���� üũ
	// �浹 �����Ѱ�?
	if(!IsActorColliable(m_pOwner)) 
		return false;

	return true;
}

void XModuleCollision::InitHitCapsuleIndex()
{
	// NPC�� ���δ�.
	if(m_pOwner->GetEntityType() == ETID_NPC)
	{
		XNonPlayer * pNPC = m_pOwner->AsNPC();
		XNPCInfo * pNpcInfo = pNPC->GetInfo();
		if(pNpcInfo)
		{
			// �⺻ �׷��ε���
			m_HitCapsuleIndex.InsertNPCModeGroupIndex(pNpcInfo->nMode, pNpcInfo->nHitTestIndex);

			// ��庰 �׷��ε���
			for(vector<XNPCInfo*>::iterator it = pNpcInfo->vecModes.begin(); it != pNpcInfo->vecModes.end(); it++)
			{
				m_HitCapsuleIndex.InsertNPCModeGroupIndex((*it)->nMode, (*it)->nHitTestIndex);
			}

			// �⺻ ���� �ε��� ����
			m_HitCapsuleIndex.Change_ByTrigger(pNpcInfo->nHitTestIndex);
		}
	}
	else
	{
		// �⺻ ���� �ε��� ����
		m_HitCapsuleIndex.InsertNPCModeGroupIndex(0, 0);
		m_HitCapsuleIndex.Change_ByTrigger(0);
	}
}

float XModuleCollision::GetDiameter()
{
	// �浹 ĸ���� �������� ����մϴ�.
	// �浹 ĸ���� 2���̻��� ��쿡�� �մϴ�.
	CSMeshInfo* pMeshInfo = GetMyObjectMeshInfo();
	if( pMeshInfo ) 
	{
		#define MAXMINPOSITION(v1, v2, r, out) do {		\
				vec3 v = v2 - v1;						\
				v.Normalize();							\
				v *= r;									\
				out = v1 + v;							\
				} while(0)									

		if(pMeshInfo->m_vecColCapsules.size() > 1)
		{
			vec3 vMin(99999.9f, 99999.9f, 0.0f);
			vec3 vMax(-99999.9f, -99999.9f, 0.0f);

			for(vector<CSColCapsule>::iterator itCol = pMeshInfo->m_vecColCapsules.begin(); itCol != pMeshInfo->m_vecColCapsules.end(); itCol++)
			{
				MCapsule tarCapsule = CalCollisionCapsuleAnimation(&(*itCol), m_pOwner);

				vec3 vTop, vBottom;
				MAXMINPOSITION(tarCapsule.bottom, tarCapsule.top, tarCapsule.radius, vBottom);
				MAXMINPOSITION(tarCapsule.top, tarCapsule.bottom, tarCapsule.radius, vTop);

				if(vBottom.x > vMax.x)	vMax.x = vBottom.x;
				if(vTop.x > vMax.x)		vMax.x = vTop.x;
				if(vBottom.y > vMax.y)	vMax.y = vBottom.y;
				if(vTop.y > vMax.y)		vMax.y = vTop.y;

				if(vBottom.x < vMin.x)	vMin.x = vBottom.x;
				if(vTop.x < vMin.x)		vMin.x = vTop.x;
				if(vBottom.y < vMin.y)	vMin.y = vBottom.y;
				if(vTop.y < vMin.y)		vMin.y = vTop.y;
			}

			return (vMax - vMin).Length();
		}
	}

	
	return (GetRadius() * 2.0f);
}		

MCapsule XModuleCollision::GetNearColCapsuleFromPos( vec3 vPos )
{
	MCapsule nearColCapsule;
	CSMeshInfo* c = GetMyObjectMeshInfo();
	if( c == NULL) return nearColCapsule;

	float fMinDist = 9999.9f;

	for(vector<CSColCapsule>::iterator itCol = c->m_vecColCapsules.begin(); itCol != c->m_vecColCapsules.end(); itCol++)
	{
		MCapsule tar_capsule = CalCollisionCapsuleAnimation(&(*itCol), m_pOwner);
		float fTempDist		= 0.0f;

		vec3 vNearestPoint = MMath::GetNearestPointOnLine(vPos, tar_capsule.bottom, tar_capsule.top);
		float fDis = (vNearestPoint - vPos).Length();

		if(fMinDist > fDis)
		{
			fMinDist			= fDis;
			nearColCapsule		= tar_capsule;
		}
	}

	return nearColCapsule;
}

bool XModuleCollision::CheckErrorGroupID( int nGroupID )
{
	if(nGroupID == INVALID_HITCAPSULEGROUP_ID)
	{
		return true;
	}

	return false;
}

void XModuleCollision::SetRadius_ForTest( float fRadius )
{
	m_MoveCollision.SetRadius(fRadius);
}

bool XModuleCollision::CheckTerrainCollisionUsable()
{
	if(gg.currentgamestate->GetWorld()->IsTest())
		return true;

	if(gg.currentgamestate->GetWorld() == false ||
		gg.currentgamestate->GetWorld()->IsNowLoading())
		return false;

	return true;
}

bool XModuleCollision::GetColCapsuleToObjectNotRub( MCapsule& capsuleColTest, MCapsule& capsuleBase, vector<MCapsule>& vecTestColCapsule, MCapsule& ColOutOrgCapsule, MCapsule& ColOutCapsule, vec3& vNearestTargetPos, float& fOutDist )
{
	CSMeshInfo * pMeshInfo = GetMyObjectMeshInfo();
	if(pMeshInfo == NULL)
	{
		return false;
	}

	bool bCol = false;
	float fMinDist = 10000.0f;

	for(vector<CSColCapsule>::iterator itCol = pMeshInfo->m_vecColCapsules.begin(); itCol != pMeshInfo->m_vecColCapsules.end(); itCol++)
	{
		MCapsule tarCapsule = CalCollisionCapsuleAnimation(&(*itCol), m_pOwner);

		//------------------------------------------------------------------------
		// �˻� �ڰ��� �ִ°�...
		float fCheckDist = (tarCapsule.GetCenter() - capsuleColTest.GetCenter()).Length();
		if(fCheckDist > (tarCapsule.height + (tarCapsule.radius * 2.0f) + capsuleColTest.height + (capsuleColTest.radius * 2.0f)))
		{
			continue;
		}

		// ������...
		float fSumRadius = tarCapsule.radius + capsuleBase.radius;
		if((capsuleBase.bottom.z > tarCapsule.top.z && (capsuleBase.bottom.z - tarCapsule.top.z) > fSumRadius) ||
			(capsuleBase.top.z < tarCapsule.bottom.z && (tarCapsule.bottom.z - capsuleBase.top.z) > fSumRadius))
		{
			continue;
		}

		// �浹 ��ġ ���	
		// ����� ��ǥ ���ϱ�
		vec3 vNearestPoint_target;
		vec3 vNearestPoint_end;
		vec3 vTempImpactPos;
		vec3 vTempImpactDir;
		float fTempOutDist = 0.0f;

		// �˻��� ĸ���� ���̰� 0.0 �̶�� �����ش�.
		if(tarCapsule.height == 0.0f)
		{
			tarCapsule.top.z = capsuleColTest.top.z;
			tarCapsule.CalcHeight();
		}

		float fTarCapsuleMinZ = tarCapsule.bottom.z;
		float fTarCapsuleMaxZ = tarCapsule.top.z;
		if(tarCapsule.bottom.z > tarCapsule.top.z)
		{
			fTarCapsuleMinZ = tarCapsule.top.z;
			fTarCapsuleMaxZ = tarCapsule.bottom.z;
		}

		for(vector<MCapsule>::iterator itCapsule = vecTestColCapsule.begin(); itCapsule != vecTestColCapsule.end(); ++itCapsule)
		{
			MCapsule testColCapsule = *itCapsule;

			if(testColCapsule.top.z < fTarCapsuleMinZ)
				testColCapsule.top.z = testColCapsule.bottom.z = fTarCapsuleMinZ;
			if(testColCapsule.top.z > fTarCapsuleMaxZ)
				testColCapsule.top.z = testColCapsule.bottom.z = fTarCapsuleMaxZ;

			//float fMinLen = 10000.0f;
			if(ColCapsuleToCapsule(testColCapsule, tarCapsule, vTempImpactPos, vTempImpactDir, fTempOutDist, &vNearestPoint_end, &vNearestPoint_target))
			//if(tarCapsule.IntersectRay(colTest[i].bottom, (colTest[i].top - colTest[i].bottom).Normalize(), fMinLen))
			{
				float fMinLen = (vTempImpactPos - testColCapsule.bottom).Length();
				if(fMinDist > fMinLen)
				{
					//ColCapsuleToCapsule(colTest[i], tarCapsule, vTempImpactPos, vTempImpactDir, fTempOutDist, &vNearestPoint_end, &vNearestPoint_target);
					fMinDist = fMinLen;
					ColOutCapsule = tarCapsule;
					ColOutOrgCapsule = testColCapsule;
					vNearestTargetPos = vNearestPoint_target;
					bCol = true;
				}
			}
		}
	}

	if(bCol)
	{
		fOutDist = fMinDist;
	}

	return bCol;
}

float XModuleCollision::ColCapsuleToLineForNotRub( const vec3& vStart, const vec3& vEnd, const vec3& vNearestTargetPos, float fMaxLen, vec3& vOutNearest )
{
	vec3 vClaDir = (vEnd - vStart).Normalize();
	vOutNearest = MMath::GetNearestPointOnLine(vNearestTargetPos, vStart, vEnd + (vClaDir * 500.0f));

	float fNearest_Len = (vOutNearest - vNearestTargetPos).Length();
	double dMax = fMaxLen * fMaxLen;
	double dMin = fNearest_Len * fNearest_Len;
	if(dMin > dMax)
	{
		double dSwap = dMax;
		dMax = dMin;
		dMin = dSwap;
	}

	return (float)sqrt(dMax - dMin);
}

float XModuleCollision::ColCapsuleToLineForNotRub( const vec3& vStart, const vec3& vEnd, MCapsule& cpauleNearestTarget, float fMaxLen, vec3& vOutNearest )
{
	vec3 vTargetMin = cpauleNearestTarget.bottom;
	vec3 vTargetMax = cpauleNearestTarget.top;
	if(vTargetMin.z > vTargetMax.z)
	{
		vec3 vTemp = vTargetMin;
		vTargetMin = vTargetMax;
		vTargetMax = vTemp;
	}

	vec3 vTargetCapsuleDir = (vTargetMax - vTargetMin).Normalize();
	vec3 vMoveDir = (vEnd - vStart).Normalize();

	vec3 vCross = vTargetCapsuleDir.CrossProduct(vMoveDir);
	vCross.Normalize();
	vec3 vCapsuleNor = vTargetCapsuleDir.CrossProduct(vCross);
	vCapsuleNor.Normalize();
	
	if(vMoveDir.DotProduct(vCapsuleNor) < 0)
		vCapsuleNor = -vCapsuleNor;

	RMatrix mat;
	mat.MakeIdentity();
	mat.SetLocalMatrix( vStart, vCapsuleNor, vec3::AXISZ);
	REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugAxis( mat, 5, 15, D3DCOLOR_XRGB(255, 0, 255));

	vec3 vNewStart = vStart + (vCapsuleNor * fMaxLen);

	REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugLine( vNewStart, vNewStart + (vMoveDir * 100.0f), 15, D3DCOLOR_XRGB(255, 255, 255));

	float fOutResult = 0.0f;
	if(cpauleNearestTarget.IntersectRay(vNewStart, vMoveDir, fOutResult))
		return fOutResult;

	return 0.0f;
}

bool XModuleCollision::IsHaveHitEffectCount()
{
	return (GetHitEffectCapsuleCount() > 0);
}

void XModuleCollision::ObjColDebugRender()
{
	m_ObjectCollisionDebug.Render();
}

vec3 XModuleCollision::GetHitEffectProjectileTargetPos( int nGroupID /*= -1 */, int nIndexID /*= -1*/ )
{
	vec3 vTargetPos(0, 0, 0);

	//------------------------------------------------------------------------
	CSMeshInfo* c = GetMyObjectMeshInfo();

	if(c != NULL && nGroupID >= 0 && nIndexID >= 0)
	{
		int nHitEffectCount = c->m_vecHitEffectInfo.size();
		if(nHitEffectCount > 1)
		{
			int nHitEffectIndex = c->FindHitEffectCapsuleIndex(nGroupID, nIndexID);

			if(nHitEffectIndex != -1 && nHitEffectCount > nHitEffectIndex)
			{
				vec3 vCapsuleCenter	 = CalCollisionCapsuleAnimation(&(c->m_vecHitEffectInfo[nHitEffectIndex]), m_pOwner).GetCenter();

				return vCapsuleCenter;
			}

		}

		vTargetPos = GetHitCapsule(nGroupID, nIndexID).GetCenter();
	}
	else
	{
		// ������ ĸ���� �˻��Ѵ�.
		vTargetPos = GetColObjectCenter();
	}

	return vTargetPos;	
}

int XModuleCollision::GetHitEffectCapsuleCount()
{
	CSMeshInfo* c = GetMyObjectMeshInfo();
	if(c)
	{
		return c->m_vecHitEffectInfo.size();
	}

	return 0;
}

void XModuleCollision::BuildTerrainColObjectPolygon( vector<MUID>* pVec, vector<RCollisionPolygon>& buildPolygon )
{
	const static RVector baseVertices[] = 
	{
		RVector(-1, 1, 1),
		RVector( 1, 1, 1),
		RVector(-1,-1, 1),
		RVector( 1,-1, 1),
		RVector(-1, 1,-1),
		RVector( 1, 1,-1),
		RVector(-1,-1,-1),
		RVector( 1,-1,-1)
	}; // 8�� vertex

	const static WORD PolygonIndexArray[] =
	{
		2, 6, 3, 6, 7, 3,	//front
		1, 5, 0, 5, 4, 0,	//back
		0, 2, 1, 1, 2, 3,	//top
		4, 6, 5, 5, 6, 7,	//bottom
		7, 1 ,3, 5, 1, 7,	//left
		2, 0, 4, 6, 2, 4,	//right
	};

	for(vector<MUID>::iterator it = pVec->begin(); it != pVec->end(); ++it)
	{
		XObject* pObject = gg.omgr->FindObject(*it);
		if(pObject == NULL)
			continue;

		XModuleCollision* pModuleCollition = pObject->GetModuleCollision();
		if(pModuleCollition)
		{
			CSMeshInfo * pMeshInfo = pModuleCollition->GetMeshInfo();
			if(pMeshInfo)
			{
				for(vector<CSColBox>::iterator itBox = pMeshInfo->m_vecColBox.begin(); itBox != pMeshInfo->m_vecColBox.end(); ++itBox)
				{
					vec3 vMin, vMax;

					MMatrix matWorld = itBox->m_matWorld * pObject->GetWorldTransform();

					vec3	Vertices[8];

					//		0						1
					//      -----------------------/
					//	   /|                     /|
					//	  / |                    / |
					//	2/----------------------/3 |
					//   |  |                   |  |
					//   |  |                   |  |
					//   |  |                   |  |
					//   |  |                   |  |
					//   |  |4                  |  |
					//   | /--------------------|--/5
					//   |/                     | /
					//   ------------------------/
					//   6						7

					for ( int i = 0; i<8; ++i )
					{
						RVector vec;
						vec.x = ( baseVertices[i].x < 0 ? itBox->vmin.x : itBox->vmax.x );
						vec.y = ( baseVertices[i].y < 0 ? itBox->vmin.y : itBox->vmax.y );
						vec.z = ( baseVertices[i].z < 0 ? itBox->vmin.z : itBox->vmax.z );
						matWorld.TransformVect(vec, Vertices[i]);
					}

					for(int k = 0; k < 12; ++k)
					{
						int nStartIndex = k * 3;
						vec3 v1 = Vertices[PolygonIndexArray[nStartIndex]];
						vec3 v2 = Vertices[PolygonIndexArray[nStartIndex + 1]];
						vec3 v3 = Vertices[PolygonIndexArray[nStartIndex + 2]];

						MPlane plane(v1, v2, v3);
						vec3 vPlaneNor = plane.Normal();
						vPlaneNor.Normalize();

						buildPolygon.push_back(RCollisionPolygon(v1, v2, v3, vec3::ZERO, vPlaneNor, 0, 0));
					}
				}
			}
		}
	}
}
