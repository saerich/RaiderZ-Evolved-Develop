#include "StdAfx.h"

#pragma push_macro("min")
#pragma push_macro("max")
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include "NxPhysics.h"

#include "NxController.h"
#include "NxControllerManager.h"

#pragma pop_macro("min")
#pragma pop_macro("max")

#include "RPhysXActorController.h"
#include "RPhysXActorControllerBuilder.h"

#include "RActorNode.h"
#include "RActor.h"

#include "REngine.h"
#include "RSceneManager.h"
#include "RDebugPassRenderer.h"

namespace rs3 {

RPhysXActorController::RPhysXActorController(NxScene* pNxScene, RACTORCONTROLLERTYPE eType, const char* _pSzControllerName)
: RActorController(eType)
{
	
	// �ùķ��̼� ������ RACT_POSTUPDATE �� �ùķ��̼� ������ RACT_PRERENDER �� �����ϴ�.
	_ASSERT( (RACT_POSTUPDATE == m_type || RACT_PRERENDER == m_type) );

	m_pNxScene = pNxScene;

	m_bAddedOnActor = false;
	m_nKinematicUpdateInfoID = -1;
	m_nUpdateCount = 0;

	// Kinematic ������ �⺻������ set function�� �̿�
	m_bUseMoveFuncInKinematic = false;

	// UpdateToPx ������ �⺻������ move function ���
	m_bUseMoveFuncInUpdateToPx = true;

	if(_pSzControllerName)
	{
		m_strControllerName = _pSzControllerName;
	}
}

RPhysXActorController::~RPhysXActorController(void)
{
	ReleaseNxObject();
}

void RPhysXActorController::ReleaseNxObject()
{
	// NxActor
	for (NX_ACTOR_RES_MAP::iterator itr = m_mapNxActors.begin(); itr != m_mapNxActors.end(); ++itr)
	{
		NxActor* pActor = itr->second;
		if (pActor)
		{
			m_pNxScene->releaseActor( *pActor );
		}
	}
	m_mapNxActors.clear();

	// NxJoint
	for (NX_JOINT_RES_MAP::iterator itr = m_mapNxJoints.begin(); itr != m_mapNxJoints.end(); ++itr)
	{
		NxJoint* pNxJoint = itr->second;
		if (pNxJoint)
		{
			m_pNxScene->releaseJoint( *pNxJoint );
		}
	}
	m_mapNxJoints.clear();

	// NxUpdateObject
	m_vecNxUpdateInfo.clear();

	// Kinematic ���� �ʱ�ȭ
	m_nKinematicUpdateInfoID = -1;
	m_strKinematicActorNodeName.clear();
}

bool RPhysXActorController::BuildNxObject(RPhysXActorControllerBuilder* pNxBuilder)
{
	_ASSERT(pNxBuilder != NULL);
	_ASSERT(m_bAddedOnActor == true);

	OnBuilderAdded();
	return pNxBuilder->Build(this);
}

void RPhysXActorController::OnAdd()
{
	m_bAddedOnActor = true;

	if ( RACT_POSTUPDATE == m_type)
		m_pActor->AddUpdateForRenderRefCount();
}

void RPhysXActorController::OnRemove()
{
	RMatrix matWorldCurrent;
	for (NX_UPDATE_ARRAY::iterator itr = m_vecNxUpdateInfo.begin(); itr != m_vecNxUpdateInfo.end(); ++itr)
	{
		SNxUpdateInfo& updateInfo = *itr;
		if (false == updateInfo.QueryAttribute(ADD_HIERARCHY_ATTR) )
		{
			SetEnableActorNodeAnimationUpdate(updateInfo.m_pManagedActorNode->GetNodeName(), true);

			updateInfo.m_dwAttribute |= ADD_HIERARCHY_ATTR;
			GetNxWorldTransform(updateInfo, matWorldCurrent);

			updateInfo.m_pManagedActorNode->SetTransform(matWorldCurrent);
			updateInfo.m_pManagedActorNode->RemoveAttribute(RSNA_NO_HIERARCHY_UPDATE);

			updateInfo.m_dwAttribute &= ~ADD_HIERARCHY_ATTR;
		}
	}

	if ( RACT_POSTUPDATE == m_type)
		m_pActor->ReleaseUpdateForRenderRefCount();

	m_bAddedOnActor = false;
}

void RPhysXActorController::Update()
{
	// �ι� ������Ʈ ����
	if ( m_nUpdateCount == REngine::GetDevice().GetFrameCount() )
		return;

	m_nUpdateCount = REngine::GetDevice().GetFrameCount();

	// Update
	if ( RACT_POSTUPDATE == m_type)
	{
		// post update �ÿ��� RealSpace ActorNode TM �� PhysX NxActor�� ����
		UpdateRsToPx();
	}
	else
	{
		// prerender update �ÿ��� PhysX NxActor TM �� RealSapce ActorNode�� ����
		_ASSERT(RACT_PRERENDER == m_type);
		UpdatePxToRs();
	}
}

void RPhysXActorController::UpdateRsToPx()
{
	_ASSERT(m_pActor->GetUpdatedFrame() == REngine::GetDevice().GetFrameCount());
	_ASSERT(m_pActor->CheckAlreadyUpdatedForRender());

	RVector vScale, vPos;
	RQuaternion qOrient;

	NxVec3 nxPos;
	NxMat33 nxOrient;

	RMatrix matT;

	RAnimationController* pAnimationController = &m_pActor->GetAnimationController();

	for (NX_UPDATE_ARRAY::iterator itr = m_vecNxUpdateInfo.begin(); itr != m_vecNxUpdateInfo.end(); ++itr)
	{
		RActorNode* pActorNode = itr->m_pManagedActorNode;
		NxActor* pNxActor = itr->m_pNxActor;

		_ASSERT(pActorNode != NULL);
		_ASSERT(pNxActor != NULL);

		RMatrix matNodeWorldTransform;
		pActorNode->GetWorldTransformAt(pAnimationController->GetFrame(), matNodeWorldTransform );

		if (itr->QueryAttribute(ADD_COLLISION_DATA))
		{
			RActorCollisionObject* pCollisionObject = (RActorCollisionObject*)itr->m_pCustomData;
			if (pCollisionObject->m_Type == MCollision::CAPSULE)
			{
				MCapsule cap(pCollisionObject->m_Capsule.Multiply( pCollisionObject->m_matTransform * matNodeWorldTransform ));

				// ������ ĸ���� �����ִ°� �⺻�̶� y�� top-bottom���� �ؾ��Ѵ�.
				matT.SetLocalMatrix(cap.GetCenter(), cap.GetDir(), m_pActor->GetWorldUp());
				//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugCapsule(cap, 0.1f);
			}
			else
			{
				matT.MakeIdentity();
			}
		}
		else
		{
			matT = matNodeWorldTransform;
		}

		// ��û���� ��Ʈ������ �ɰ���
		matT.Decompose(vScale, vPos, qOrient);
		qOrient.SetMatrixRotation(matT);

		// ���⼳��
		nxOrient.setColumn(0, NxVec3(matT._11, matT._12, matT._13));
		nxOrient.setColumn(1, NxVec3(matT._21, matT._22, matT._23));
		nxOrient.setColumn(2, NxVec3(matT._31, matT._32, matT._33));

		// ��ġ����
		nxPos.set(vPos.x, vPos.y, vPos.z);

		if (m_bUseMoveFuncInUpdateToPx)
		{
			pNxActor->moveGlobalOrientation(nxOrient);
			pNxActor->moveGlobalPosition(nxPos);
		}
		else
		{
			pNxActor->setGlobalOrientation(nxOrient);
			pNxActor->setGlobalPosition(nxPos);
		}
	}
}

void RPhysXActorController::UpdatePxToRs()
{
	RMatrix matWorldCurrent;

	// �ùķ��̼ǵ� ���� RActorNode�� ������ش�.
	for (NX_UPDATE_ARRAY::iterator itr = m_vecNxUpdateInfo.begin(); itr != m_vecNxUpdateInfo.end(); ++itr)
	{
		GetNxWorldTransform(*itr, matWorldCurrent);
		itr->m_pManagedActorNode->SetTransform(matWorldCurrent);
		itr->m_pManagedActorNode->UpdateTransform();
	}

	// AABB ������ ������Ʈ
	RefreshAABB();
	// Debugging ����
	// REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBoxAndAxis( m_pActor->GetActorNode(RBIPID_RCLAVICLE)->GetWorldTransform(), 5, 1);
}

void RPhysXActorController::GetNxWorldTransform( const SNxUpdateInfo& _rNxInfo, RMatrix& _refWorldTM )
{
	//////////////////////////////////////////////////////////////////////////
	//
	// Local Transform ���ϱ�
	//
	//////////////////////////////////////////////////////////////////////////

	NxMat34 mat;

	// axis
	if ( _rNxInfo.QueryAttribute(APPLY_ACTOR_AXIS) )
	{
		mat = _rNxInfo.m_pNxActor->getGlobalPose();
	}
	else
	{
		mat.id();
	}

	// pos
	if ( _rNxInfo.QueryAttribute(APPLY_ACTOR_POS) )
	{
		_ASSERT( _rNxInfo.QueryAttribute(APPLY_JOINT_POS) == false );
		_ASSERT( _rNxInfo.m_pNxActor != NULL);
		mat.t = _rNxInfo.m_pNxActor->getGlobalPosition();
	}

	if ( _rNxInfo.QueryAttribute(APPLY_JOINT_POS) )
	{
		_ASSERT( _rNxInfo.QueryAttribute(APPLY_ACTOR_POS) == false );
		_ASSERT( _rNxInfo.m_pNxJoint != NULL);
		mat.t = _rNxInfo.m_pNxJoint->getGlobalAnchor();
	}

	// ankle
	if ( _rNxInfo.QueryAttribute(ADD_ANKLE_ATTR) )
	{
		NxU32 nShapes = _rNxInfo.m_pNxActor->getNbShapes();
		_ASSERT(nShapes == 1);

		NxShape*const* shapes = _rNxInfo.m_pNxActor->getShapes();
		float fHeight = static_cast<NxCapsuleShape*>( shapes[0] )->getHeight();
		NxMat33 matPose = _rNxInfo.m_pNxActor->getGlobalOrientation();

		NxVec3 vNxUp = matPose.getColumn(0);
		RVector vUp(vNxUp.x, vNxUp.y, vNxUp.z);
		vUp.Normalize();
		vUp = vUp * fHeight;
		vNxUp.set(vUp.x, vUp.y, vUp.z);

		RBIPID eBip;
		std::string strNodeName;
		NxMat33 matRotX;
		if (_rNxInfo.QueryAttribute(ADD_RIGHT_ATTR))
		{
			matRotX.rotX(NxPi*0.5f);
			eBip = RBIPID_RFOREARM;
			strNodeName = "Bip01 R ForeTwist";
		}
		else
		{
			_ASSERT( _rNxInfo.QueryAttribute(ADD_LEFT_ATTR) );
			matRotX.rotX(NxPi*(-0.5f) );

			eBip = RBIPID_LFOREARM;
			strNodeName = "Bip01 L ForeTwist";
		}

		RActorNode* pActorNodeTwist = m_pActor->GetActorNode(strNodeName.c_str());
		if ( NULL != pActorNodeTwist )
		{
			RVector vScale;
			RVector vPos;
			RQuaternion qOrientForeArm;
			RMatrix matT;

			RActorNode* pActorNodeForeArm = m_pActor->GetActorNode( RSkeleton::GetBipName(eBip) );
			matT = pActorNodeForeArm->GetLocalTransform();
			matT.Decompose(vScale, vPos, qOrientForeArm);

			pActorNodeTwist->SetRotation( qOrientForeArm );
		}

		mat.M.multiply(matPose, matRotX);
		mat.t = _rNxInfo.m_pNxActor->getGlobalPosition() + vNxUp;
	}

	_refWorldTM._11 = mat.M(0,0);
	_refWorldTM._12 = mat.M(1,0);
	_refWorldTM._13 = mat.M(2,0);

	_refWorldTM._21 = mat.M(0,1);
	_refWorldTM._22 = mat.M(1,1);
	_refWorldTM._23 = mat.M(2,1);

	_refWorldTM._31 = mat.M(0,2);
	_refWorldTM._32 = mat.M(1,2);
	_refWorldTM._33 = mat.M(2,2);

	_refWorldTM._41 = mat.t.x;
	_refWorldTM._42 = mat.t.y;
	_refWorldTM._43 = mat.t.z;

	_refWorldTM._14 = _refWorldTM._24 = _refWorldTM._34 = 0;
	_refWorldTM._44 = 1.f;


	//////////////////////////////////////////////////////////////////////////
	//
	//	Root
	//
	//////////////////////////////////////////////////////////////////////////

	if (_rNxInfo.QueryAttribute(ADD_BIP_ROOT_ATTR))
	{
		// Kinematic mode �� ���
		if ( m_nKinematicUpdateInfoID > -1 )
		{
			_ASSERT( m_nKinematicUpdateInfoID <= (int)m_vecNxUpdateInfo.size() );
			_refWorldTM = m_matKinematicLocalTransform * _refWorldTM;
		}
		else
		{
			m_pActor->SetTransform( m_matHierarchyRootInv * _refWorldTM );
			m_pActor->UpdateTransform();
			_refWorldTM.MakeIdentity();
		}

		return;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	Pelvis
	//
	//////////////////////////////////////////////////////////////////////////

	if (_rNxInfo.QueryAttribute(ADD_PELVIS_ATTR) && m_strKinematicActorNodeName.empty() )
	{
		_refWorldTM = _refWorldTM * m_pActor->GetWorldInverseTransform();
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	��ũ�� ���ͳ��
	//
	//////////////////////////////////////////////////////////////////////////

	if (_rNxInfo.QueryAttribute(ADD_LINKEDNODE_DATA))
	{
		SNxUpdateLinkedData* pLinkedNodeData = (SNxUpdateLinkedData*)(_rNxInfo.m_pCustomData);
		_ASSERT(_rNxInfo.QueryAttribute(ADD_COLLISION_DATA) == false);
		_ASSERT(pLinkedNodeData);

		RMatrix* pLocalMat = pLinkedNodeData->m_pLocalMat;
		RActorNode* pLinkedActorNode = m_pActor->GetActorNode(pLinkedNodeData->m_strLinkedNodeName.c_str());
		if (pLinkedActorNode)
		{
			if(pLocalMat)
			{
				_refWorldTM = (*pLocalMat) * pLinkedActorNode->GetWorldTransform();
			}
			else
			{
				_refWorldTM = pLinkedActorNode->GetWorldTransform();
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	���� ���谡 �ִ� ���ͳ���� Transform�� ���
	//
	//////////////////////////////////////////////////////////////////////////

	if ( _rNxInfo.QueryAttribute(ADD_HIERARCHY_ATTR) )
	{
		// ���������̸� �θ�� ��ũ�ȴ�.
		_ASSERT(_rNxInfo.m_pManagedActorNode);
		RActorNode* pParentActorNode = _rNxInfo.m_pManagedActorNode->GetParentActorNode();
		if (pParentActorNode)
		{
			m_matUpdateParent = pParentActorNode->GetWorldTransform();
			m_matUpdateParent.GetInverse(&m_matUpdateParentInv);

			_refWorldTM = _refWorldTM * m_matUpdateParentInv;
		}
	}
}

void RPhysXActorController::RefreshUpdateInfoMap()
{
	for ( NX_UPDATE_ARRAY::iterator itr = m_vecNxUpdateInfo.begin(); itr != m_vecNxUpdateInfo.end(); )
	{
		RActorNode* pActorNode = m_pActor->GetActorNode(itr->m_strActorNodeName.c_str());
		if ( NULL == pActorNode )
		{
			itr = m_vecNxUpdateInfo.erase(itr);
		}
		else
		{
			if (itr->QueryAttribute(ADD_HIERARCHY_ATTR) == false)
			{
				pActorNode->AddAttribute(RSNA_NO_HIERARCHY_UPDATE);
				SetEnableActorNodeAnimationUpdate(pActorNode->GetNodeName(), false);
			}

			itr->m_pManagedActorNode = pActorNode;
			++itr;
		}
	}

	if ( false == m_strKinematicActorNodeName.empty() )
		SetEnableKinematicMode(m_strKinematicActorNodeName.c_str());
}

void RPhysXActorController::SetEnableKinematicMode(const char* pSzActorNodeName)
{
	m_nKinematicUpdateInfoID = -1;
	m_strKinematicActorNodeName.clear();

	int nUpdateID = 0;
	for (NX_UPDATE_ARRAY::iterator itr = m_vecNxUpdateInfo.begin(); itr != m_vecNxUpdateInfo.end(); ++itr, ++nUpdateID)
	{
		NxActor* pActor = itr->m_pNxActor;
		if (!pActor)
			continue;

		pActor->raiseBodyFlag(NX_BF_DISABLE_GRAVITY);
		if (pSzActorNodeName == itr->m_strActorNodeName)
		{
			pActor->raiseBodyFlag(NX_BF_KINEMATIC);

			m_matKinematicLocalTransform = m_pActor->GetActorNode(pSzActorNodeName)->GetWorldTransform() * m_pActor->GetWorldInverseTransform();
			m_strKinematicActorNodeName = pSzActorNodeName;
			m_nKinematicUpdateInfoID = nUpdateID;
		}
		else
		{
			pActor->clearBodyFlag(NX_BF_KINEMATIC);
		}
	}
}

void RPhysXActorController::SetKinematicModeTransform(const RMatrix& _rMat, bool bAlsoUpdateActor)
{
	if ((m_nKinematicUpdateInfoID < 0) ||
		(int)m_vecNxUpdateInfo.size() <= m_nKinematicUpdateInfoID )
	{
		return;
	}

	NxActor* pActor = m_vecNxUpdateInfo[m_nKinematicUpdateInfoID].m_pNxActor;
	if (pActor)
	{
		RMatrix matKinematicWorldTransform = m_matKinematicLocalTransform * _rMat;

		// ��û���� ��Ʈ������ �ɰ���
		matKinematicWorldTransform.Decompose(m_vKinematicCurrentScale, m_vKinematicCurrentPos, m_qKinematicCurrentOrient);
		m_qKinematicCurrentOrient.SetMatrixRotation(m_matKinematicCurrentOrient);

		// ���� ����
		NxMat33 orient;
		orient.setColumn(0, NxVec3(m_matKinematicCurrentOrient._11, m_matKinematicCurrentOrient._12, m_matKinematicCurrentOrient._13));
		orient.setColumn(1, NxVec3(m_matKinematicCurrentOrient._21, m_matKinematicCurrentOrient._22, m_matKinematicCurrentOrient._23));
		orient.setColumn(2, NxVec3(m_matKinematicCurrentOrient._31, m_matKinematicCurrentOrient._32, m_matKinematicCurrentOrient._33));

		// ��ġ ��ȭ�� �����ϱ�
		NxVec3 vPos;
		vPos.x = m_vKinematicCurrentPos.x;
		vPos.y = m_vKinematicCurrentPos.y;
		vPos.z = m_vKinematicCurrentPos.z;

		// ������ �� ����
		if (m_bUseMoveFuncInKinematic)
		{
			pActor->moveGlobalOrientation(orient);
			pActor->moveGlobalPosition(vPos);
		}
		else
		{
			pActor->setGlobalOrientation(orient);
			pActor->setGlobalPosition(vPos);
		}

		// ���Ϳ��� �����ϴ��� ����
		if (bAlsoUpdateActor)
			m_pActor->SetTransform(_rMat);
	}
}

void RPhysXActorController::SetResourceNxActorDensity( float fDensity )
{
	for (NX_ACTOR_RES_MAP::iterator itr = m_mapNxActors.begin(); itr != m_mapNxActors.end(); ++itr)
	{
		NxActor* pActor = itr->second;
		if (pActor)
		{
			pActor->updateMassFromShapes(fDensity, 0);
		}
	}
}

void RPhysXActorController::SetResouceNxActorCollisionGroup( int nGroup )
{
	for (NX_ACTOR_RES_MAP::iterator itr = m_mapNxActors.begin(); itr != m_mapNxActors.end(); ++itr)
	{
		NxActor* pActor = itr->second;
		if (pActor)
		{
			NxU32 nShapes = pActor->getNbShapes();
			if(nShapes > 0)
			{
				NxShape*const* shapes = pActor->getShapes();
				while( nShapes-- )
					shapes[nShapes]->setGroup(nGroup);
			}
		}
	}
}

void RPhysXActorController::SetEnableActorNodeAnimationUpdate( const std::string& _rStrActorNodeName, bool bEnable )
{
	if (NULL == m_pActor)
		return;

	RAnimationControllerNode* pNode = m_pActor->GetAnimationController().GetNodeController( _rStrActorNodeName.c_str() );
	if (pNode)
	{
		pNode->SetEnableAnimationUpdate(bEnable);
	}
}

}