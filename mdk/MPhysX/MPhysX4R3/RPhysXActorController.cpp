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
#include "RActorCollision.h"

#include "REngine.h"
#include "RSceneManager.h"
#include "RDebugPassRenderer.h"

namespace rs3 {

RPhysXActorController::RPhysXActorController(NxScene* pNxScene, RSNCTRL_TYPE eType, const char* _pSzControllerName)
: RActorController(eType)
{
	
	// �ùķ��̼� ���� RSNCTRL_POSTUPDATE ��(���� ������Ʈ ���)
	// ���ø����̼� ������Ʈ�� RSNCTRL_CUSTOMUPDATE(�ùķ��̼� �Ŀ� �Ҹ����� ����Ǿ�� �Ѵ�) ������ ��� �����ϴ�.
	_ASSERT( (RSNCTRL_POSTUPDATE == m_type || RSNCTRL_CUSTOMUPDATE == m_type) );

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
	for (NX_UPDATE_ARRAY::iterator itr = m_vecNxUpdateInfo.begin(); itr != m_vecNxUpdateInfo.end(); ++itr)
	{
		SAFE_DELETE(*itr);
	}
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
	RActorController::OnAdd();

	m_bAddedOnActor = true;

	if ( RSNCTRL_POSTUPDATE == m_type)
		m_pActor->AddUpdateForRenderRefCount();

	SetAutoUpdate(true);	
}

void RPhysXActorController::OnRemove()
{
	RMatrix matWorldCurrent;
	for (NX_UPDATE_ARRAY::iterator itr = m_vecNxUpdateInfo.begin(); itr != m_vecNxUpdateInfo.end(); ++itr)
	{
		SNxUpdateInfo* pUpdateInfo = *itr;
		if (false == pUpdateInfo->QueryAttribute(ADD_HIERARCHY_ATTR) )
		{
			SetEnableActorNodeAnimationUpdate(pUpdateInfo->GetManagedActorNode()->GetNodeName(), true);

			pUpdateInfo->m_dwAttribute |= ADD_HIERARCHY_ATTR;
			GetNxWorldTransform(*pUpdateInfo, matWorldCurrent);

			pUpdateInfo->GetManagedActorNode()->SetTransform(matWorldCurrent);
			pUpdateInfo->GetManagedActorNode()->RemoveAttribute(RSNA_NO_HIERARCHY_UPDATE);

			pUpdateInfo->m_dwAttribute &= ~ADD_HIERARCHY_ATTR;
		}
	}

	if ( RSNCTRL_POSTUPDATE == m_type)
		m_pActor->ReleaseUpdateForRenderRefCount();

	SetAutoUpdate(false);

	m_bAddedOnActor = false;

	RActorController::OnRemove();
}

void RPhysXActorController::Update(RCameraSceneNode* pCamera)
{
	// �ι� ������Ʈ ����
	if ( m_nUpdateCount == REngine::GetDevice().GetFrameCount() )
		return;

	m_nUpdateCount = REngine::GetDevice().GetFrameCount();

	// Update
	if ( RSNCTRL_POSTUPDATE == m_type)
	{
		// post update �ÿ��� RealSpace ActorNode TM �� PhysX NxActor�� ����
		UpdateRsToPx();
	}
	else
	{
		// �ùķ��̼� �� PhysX NxActor TM �� RealSapce ActorNode�� ����
		_ASSERT(RSNCTRL_CUSTOMUPDATE == m_type);
		UpdatePxToRs();
	}
}

void RPhysXActorController::UpdateRsToPx()
{
	// �̹� UpdateForRender�� ȣ��� ���¿��� �Ѵ�.
	_ASSERT(m_pActor->GetUpdatedFrame() == REngine::GetDevice().GetFrameCount());
	_ASSERT(m_pActor->CheckAlreadyUpdatedForRender());

	RVector vScale, vPos;
	RQuaternion qOrient;

	NxVec3 nxPos;
	NxMat33 nxOrient;

	RMatrix matT;
	for (NX_UPDATE_ARRAY::iterator itr = m_vecNxUpdateInfo.begin(); itr != m_vecNxUpdateInfo.end(); ++itr)
	{
		RActorNode* pActorNode = (*itr)->GetManagedActorNode();
		NxActor* pNxActor = (*itr)->m_pNxActor;

		_ASSERT(pActorNode != NULL);
		_ASSERT(pNxActor != NULL);

		// get world transform of actor node
		const RMatrixA16& matNodeWorldTransform = pActorNode->GetWorldTransform();
		if ((*itr)->QueryAttribute(ADD_COLLISION_DATA))
		{
			RActorCollisionObject* pCollisionObject = (RActorCollisionObject*)(*itr)->m_pCustomData;
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
	// AABB ������ ������Ʈ
	// �ùķ��̼� �� �Ŀ� ������Ʈ ���ָ� UpdateTransform �����ִ� ���ͳ����� ������Ʈ �Ǿ TM�� �ٲ��.
	// �׷��� �� ������ ���� ������Ʈ ���ش�.
	RefreshAABB();

	RMatrix matWorldCurrent;
	RMatrix matActorNodeScale(RMatrix::IDENTITY);

	// �ùķ��̼ǵ� ���� RActorNode�� ������ش�.
	for (NX_UPDATE_ARRAY::iterator itr = m_vecNxUpdateInfo.begin(); itr != m_vecNxUpdateInfo.end(); ++itr)
	{
		// get world trasform of this actor node
		SNxUpdateInfo* pUpdateInfo = *itr;
		GetNxWorldTransform(*pUpdateInfo, matWorldCurrent);

		// get actor node
		RActorNode* pActorNode = pUpdateInfo->GetManagedActorNode();

		// set transform
		if (pActorNode->QueryAttribute(RSNA_NO_HIERARCHY_UPDATE))
		{
			matActorNodeScale.SetScale(pActorNode->m_pActor->GetScale() * pUpdateInfo->m_vActorNodeScale);
			pActorNode->SetTransform(matActorNodeScale * matWorldCurrent);
		}
		else
		{
			pActorNode->SetTransform(matWorldCurrent);
		}

		// update transform
		pActorNode->UpdateTransform();
	}

	// Debugging ����
	// REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBoxAndAxis( m_pActor->GetActorNode(RBIPID_RCLAVICLE)->GetWorldTransform(), 5, 1);
	// REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBoxAndAxis( m_pActor->GetActorNode("Bip01 R Hand")->GetWorldTransform(), 5, 0.001f);
	//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBoxAndAxis( m_pActor->GetActorNode("Bip01 R Hand")->GetWorldTransform(), 10, 1000.f);
	//RMatrix mattt = m_pActor->GetActorNode("b_parts_01_frag_07")->GetWorldTransform();
	//mlog("%f, %f, %f\n", mattt._41, mattt._42, mattt._43);
	//REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBoxAndAxis( m_pActor->GetActorNode("b_parts_01_frag_07")->GetWorldTransform(), 5, 30.f);
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

	// Custom Controller
	if ( _rNxInfo.QueryAttribute(ADD_CUSTOMCONTROL_DATA) )
	{
		_ASSERT(_rNxInfo.QueryAttribute(ADD_COLLISION_DATA) == false);
		_ASSERT(_rNxInfo.QueryAttribute(ADD_LINKEDNODE_DATA) == false);

		RPhysXActorNodeCustomController* pCustomController = (RPhysXActorNodeCustomController*)(_rNxInfo.m_pCustomData);
		_ASSERT(pCustomController);

		pCustomController->OnCustomControl(m_pActor, _rNxInfo.m_pNxActor, _rNxInfo.m_pNxJoint, mat);
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
		_ASSERT(_rNxInfo.GetManagedActorNode());
		RActorNode* pParentActorNode = _rNxInfo.GetManagedActorNode()->GetParentActorNode();
		if (pParentActorNode)
		{
			m_matUpdateParent = pParentActorNode->GetWorldTransform();
			m_matUpdateParent.GetInverse(&m_matUpdateParentInv);

			_refWorldTM = _refWorldTM * m_matUpdateParentInv;
		}
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	Custom Force
	//
	//////////////////////////////////////////////////////////////////////////

	if (_rNxInfo.m_fCustomGravity > 0.f)
	{
		_ASSERT(_rNxInfo.m_pNxActor);
		//_rNxInfo.m_pNxActor->addForce( NxVec3(0, 0, -_rNxInfo.m_fCustomGravity), NX_ACCELERATION);
		_rNxInfo.m_pNxActor->addForceAtPos( NxVec3(0, 0, -_rNxInfo.m_fCustomGravity), _rNxInfo.m_pNxActor->getGlobalPosition(), NX_ACCELERATION);
	}
}

void RPhysXActorController::RefreshUpdateInfoMap()
{
	for ( NX_UPDATE_ARRAY::iterator itr = m_vecNxUpdateInfo.begin(); itr != m_vecNxUpdateInfo.end(); )
	{
		SNxUpdateInfo* pNxUpdateInfo = *itr;
		RActorNode* pActorNode = m_pActor->GetActorNode(pNxUpdateInfo->m_strActorNodeName.c_str());
		if ( NULL == pActorNode )
		{
			//mlog("%s actornode is invalid when physx controller refreshes\n", pNxUpdateInfo->m_strActorNodeName.c_str());
			delete pNxUpdateInfo;
			itr = m_vecNxUpdateInfo.erase(itr);
		}
		else
		{
			if (pNxUpdateInfo->QueryAttribute(ADD_HIERARCHY_ATTR) == false)
			{
				pActorNode->AddAttribute(RSNA_NO_HIERARCHY_UPDATE);
				SetEnableActorNodeAnimationUpdate(pActorNode->GetNodeName(), false);
			}

			pNxUpdateInfo->SetManagedActorNode(pActorNode);
			++itr;
		}
	}

	if ( false == m_strKinematicActorNodeName.empty() )
		SetEnableKinematicMode(m_strKinematicActorNodeName.c_str());
}

void RPhysXActorController::SetEnableKinematicMode(const char* pSzFixedActorNodeName)
{
	m_nKinematicUpdateInfoID = -1;
	m_strKinematicActorNodeName.clear();

	int nUpdateID = 0;
	for (NX_UPDATE_ARRAY::iterator itr = m_vecNxUpdateInfo.begin(); itr != m_vecNxUpdateInfo.end(); ++itr, ++nUpdateID)
	{
		NxActor* pActor = (*itr)->m_pNxActor;
		if (!pActor)
			continue;

		if (pSzFixedActorNodeName == (*itr)->m_strActorNodeName)
		{
			pActor->raiseBodyFlag(NX_BF_KINEMATIC);

			m_matKinematicLocalTransform = m_pActor->GetActorNode(pSzFixedActorNodeName)->GetWorldTransform() * m_pActor->GetWorldInverseTransform();
			m_strKinematicActorNodeName = pSzFixedActorNodeName;
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

	NxActor* pActor = m_vecNxUpdateInfo[m_nKinematicUpdateInfoID]->m_pNxActor;
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
		{
			m_pActor->SetTransform(_rMat);
			m_pActor->UpdateTransform();
			UpdateActorForRender();
		}
	}
}

void RPhysXActorController::SetResourceNxActorDensity( float fDensity )
{
	for (NX_ACTOR_RES_MAP::iterator itr = m_mapNxActors.begin(); itr != m_mapNxActors.end(); ++itr)
	{
		NxActor* pActor = itr->second;
		if (!pActor)
			continue;

		bool bUseCustomGravity = false;
		if (fDensity <= 0.01f)
		{
			pActor->raiseBodyFlag(NX_BF_DISABLE_GRAVITY);
			if (fDensity > 0.f)
				bUseCustomGravity = true;
		}
		else
		{
			pActor->clearBodyFlag(NX_BF_DISABLE_GRAVITY);
		}

		for (NX_UPDATE_ARRAY::iterator itr_update = m_vecNxUpdateInfo.begin(); itr_update != m_vecNxUpdateInfo.end(); ++itr_update)
		{
			if ((*itr_update)->m_pNxActor != pActor)
				continue;

			if (bUseCustomGravity)
			{
				NxVec3 vGravity;
				m_pNxScene->getGravity(vGravity);
				fDensity = fDensity * (-vGravity.z) * 100.f;
				(*itr_update)->m_fCustomGravity = fDensity;
				//pActor->addForce(NxVec3(0, 0, -fDensity), NX_ACCELERATION);
				pActor->addForceAtPos(NxVec3(0, 0, -fDensity), pActor->getGlobalPosition(), NX_ACCELERATION);
			}
			else
			{
				(*itr_update)->m_fCustomGravity = -1.f;
			}
			break;
		}

		pActor->updateMassFromShapes(fDensity, 0);
	}
}

void RPhysXActorController::SetResouceNxActorCollisionGroup( int nGroup )
{
	for (NX_ACTOR_RES_MAP::iterator itr = m_mapNxActors.begin(); itr != m_mapNxActors.end(); ++itr)
	{
		NxActor* pActor = itr->second;
		if (pActor)
		{
			// actor
			pActor->setGroup(nGroup);

			// shape
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
