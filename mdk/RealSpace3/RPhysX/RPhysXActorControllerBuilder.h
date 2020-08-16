#pragma once

#include "RPhysXActorController.h"

//////////////////////////////////////////////////////////////////////////
//
// PhysXSceneNode 'Controller' Builder
//
//////////////////////////////////////////////////////////////////////////

namespace rs3 {

class RPhysXActorControllerBuilder
{
public:
	RPhysXActorControllerBuilder(){}
	virtual ~RPhysXActorControllerBuilder(){}


	//////////////////////////////////////////////////////////////////////////
	//
	// Builder Interface
	//
	//////////////////////////////////////////////////////////////////////////

	virtual bool		Build( RPhysXActorController* pPhysXController ) = 0;


protected:

	//////////////////////////////////////////////////////////////////////////
	//
	// RPhysXActorControllerBuilder �� ��ӹ��� ��ü���� �θ� ���ؼ���
	// RPhysXActorController�� ������ �� �ִ�.
	//
	//////////////////////////////////////////////////////////////////////////

	inline RActor*		GetActor(RPhysXActorController* pPhysXController);
	inline NxScene*		GetNxScene(RPhysXActorController* pPhysXController);


	// ���ҽ� ����
	inline void			AddNxActorResource( RPhysXActorController* pPhysXController, int nActorID, NxActor* pNxActor);
	inline void			AddNxJointResource( RPhysXActorController* pPhysXController, int nJointID, NxJoint* pNxJoint );

	// ������Ʈ ����
	inline void			PushBackNxUpdateInfo( RPhysXActorController* pPhysXController, const std::string& _rActorNodeName, NxActor* pNxActor, NxJoint* pNxJoint, DWORD dwAttr, const char* pSzLinkedNodeName = NULL);
	inline void			RefreshNxUpdateInfoMap(RPhysXActorController* pPhysXController){ pPhysXController->RefreshUpdateInfoMap(); }
	inline void			SetHierarchyRootInvTransform(RPhysXActorController* pPhysXController, const RMatrix& _rMat ){ pPhysXController->m_matHierarchyRootInv = _rMat; }
};
//----------------------------------------------------------------------------------------------------
inline RActor* RPhysXActorControllerBuilder::GetActor(RPhysXActorController* pPhysXController)
{
	return pPhysXController->m_pActor;
}
//----------------------------------------------------------------------------------------------------
inline NxScene* RPhysXActorControllerBuilder::GetNxScene(RPhysXActorController* pPhysXController)
{
	return pPhysXController->m_pNxScene;
}
//----------------------------------------------------------------------------------------------------
inline void RPhysXActorControllerBuilder::AddNxActorResource( RPhysXActorController* pPhysXController, int nActorID, NxActor* pNxActor)
{
	_ASSERT(pPhysXController->m_mapNxActors.find(nActorID) == pPhysXController->m_mapNxActors.end());
	pPhysXController->m_mapNxActors.insert(RPhysXActorController::NX_ACTOR_RES_MAP::value_type( nActorID, pNxActor ) );
}
//----------------------------------------------------------------------------------------------------
inline void RPhysXActorControllerBuilder::AddNxJointResource( RPhysXActorController* pPhysXController, int nJointID, NxJoint* pNxJoint )
{
	_ASSERT(pPhysXController->m_mapNxJoints.find(nJointID) == pPhysXController->m_mapNxJoints.end());
	pPhysXController->m_mapNxJoints.insert(RPhysXActorController::NX_JOINT_RES_MAP::value_type( nJointID, pNxJoint ) );
}
//----------------------------------------------------------------------------------------------------
inline void RPhysXActorControllerBuilder::PushBackNxUpdateInfo( RPhysXActorController* pPhysXController, const std::string& _rActorNodeName, NxActor* pNxActor, NxJoint* pNxJoint, DWORD dwAttr, const char* pSzLinkedNodeName)
{
	RPhysXActorController::SNxUpdateLinkedData* pLinkedNodeData = NULL;
	if (pSzLinkedNodeName)
	{
		pLinkedNodeData = new RPhysXActorController::SNxUpdateLinkedData;
		pLinkedNodeData->m_strLinkedNodeName = pSzLinkedNodeName;
	}

	pPhysXController->m_vecNxUpdateInfo.push_back( RPhysXActorController::SNxUpdateInfo(_rActorNodeName, pNxActor, pNxJoint, dwAttr, NULL, (void*)pLinkedNodeData) );
}
//----------------------------------------------------------------------------------------------------
}
