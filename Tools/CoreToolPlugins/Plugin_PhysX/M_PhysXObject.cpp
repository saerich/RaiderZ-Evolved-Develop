#include "stdafx.h"
#include "M_PhysXObject.h"

#include "MPxTransformController.h"
#include "MPxDynamicActorNodeTC.h"

#include "MPxDynamicObjectEntity.h"

#include "RActor.h"
#include "M_ToolSceneManager.h"

#include "C_ControlMediator.h"
#include "C_ControlMediatorHelperRender.h"

// Transform Controller Entity
template< typename T_OwnerType >
class CPhysXTransformControllerEntity : public physx::MPxTcEntity
{
public:
	CPhysXTransformControllerEntity(T_OwnerType* _pTcEntityOwner){ m_pTcEntityOwner = _pTcEntityOwner; }
	virtual ~CPhysXTransformControllerEntity(){}

	// actor
	virtual rs3::RActor*					GetActor(){ return m_pTcEntityOwner->GetActor(); }

	// transform controller
	virtual physx::MPxTransformController*	GetTransformController(int nId){ return m_pTcEntityOwner->GetTransformController(nId); }

	// confirm available actor
	virtual bool							IsAvailableActor(rs3::RActor* pActor){ return true; }

	// notify for dynamic object collision
	virtual void							OnDynamicObjectCollided(NxActor* _pNxActor, physx::MPxDynamicObjectEntity* _pDynamicObjEntity){}

private:
	T_OwnerType* m_pTcEntityOwner;
};

// PhysX Object Entity == Game::ModuleEntity
class CPhysXObjectEntity
{
public:
	CPhysXObjectEntity(const char* _pSzMeshName)
	{
		// init RActor;
		m_pActor = new rs3::RActor;
		rs3::RToolSceneManager* pSceneMgr = dynamic_cast<rs3::RToolSceneManager*>(rs3::REngine::GetSceneManagerPtr());
		_ASSERT(pSceneMgr);
		if (false == pSceneMgr->LoadSceneNodeFromFile(m_pActor, _pSzMeshName))
		{
			delete m_pActor;
			m_pActor = NULL;
			return;
		}

		// call RActor::Stop for default pose
		m_pActor->Stop();

		// init transform controller entity
		m_pTcEntity = new CPhysXTransformControllerEntity<CPhysXObjectEntity>(this);

		// init transform controller
		{
			using namespace physx;
			int i=0;

			// DynamicActorNode
			// ���ͳ����� ���� �ùķ��̼ǿ� ���� �������ִ� TC
			// �극��Ŀ�� ����� ���δ�.
			MPxDynamicActorNodeTC::SetID(i++);
			m_vecTransformControllers.push_back(new MPxDynamicActorNodeTC(m_pTcEntity));

			//// Attacher
			//MPxAttacherTC::SetID(i++);
			//m_vecTransformControllers.push_back( new MPxAttacherTC(m_pTcEntity) );

			//// RagDoll
			//MPxRagDollTC::SetID(i++);
			//MPxRagDollTC* pRagDollTC = new MPxRagDollTC(m_pTcEntity);
			//pRagDollTC->InitRagDollInfoFromModelName(_rPhxName.substr(0, _rPhxName.length() - 8), NULL);
			//m_vecTransformControllers.push_back(pRagDollTC);

			//// AniLookUp
			//// �ִϸ��̼� �̸��� �������(LookUp) ���� �ùķ��̼� Ʈ������ �ϴ� TC
			//MPxAniLookUpTC::SetID(i++);
			//MPxAniLookUpTC* pAniLookupTC = new MPxAniLookUpTC(m_pTcEntity);
			//pAniLookupTC->SetEnableCollisionTcHooking(true);
			//m_vecTransformControllers.push_back(pAniLookupTC);

			//// DynaicCollision
			//// �������� �̿��� �浹ó���� �Ҷ� ���
			//// ������ �浹�Ҽ� �ִ� �浹��ü���� ���鶧 ���
			//MPxDynamicCollisionTC::SetID(i++);
			//m_vecTransformControllers.push_back(new MPxDynamicCollisionTC(m_pTcEntity));
		}
	}

	~CPhysXObjectEntity()
	{
		// halt transform controller entity
		if (m_pTcEntity)
		{
			delete m_pTcEntity;
		}

		// halt transform controller
		for (std::vector<physx::MPxTransformController*>::iterator itr = m_vecTransformControllers.begin();
			itr != m_vecTransformControllers.end(); ++itr)
		{
			delete *itr;
		}
		m_vecTransformControllers.clear();

		// halt RActor;
		if (m_pActor)
		{
			m_pActor->Destroy();
			delete m_pActor;
		}
	}

	rs3::RActor* GetActor(){ return m_pActor; }

	physx::MPxTransformController* GetTransformController(int nId)
	{
		if (nId < 0)
			return NULL;

		int nContainerSize = (int)m_vecTransformControllers.size();
		if (nId >= nContainerSize)
			return NULL;

		return m_vecTransformControllers[nId];
	}


	template< typename T_Tc >
	T_Tc* GetTransformController()
	{
		int nId = T_Tc::GetID();
		if (nId < 0)
			return NULL;

		physx::MPxTransformController* tc = GetTransformController(nId);
		if(NULL == tc)
			return NULL;

		return dynamic_cast<T_Tc*>(tc);
	}

	void Update(float _fTime)
	{
		for (std::vector<physx::MPxTransformController*>::iterator itr = m_vecTransformControllers.begin();
			itr != m_vecTransformControllers.end(); ++itr)
		{
			physx::MPxTransformController* pTc = *itr;
			if (pTc->IsEnabled())
			{
				pTc->Update(_fTime);
			}
		}
	}

private:
	rs3::RActor* m_pActor;
	physx::MPxTcEntity* m_pTcEntity;
	std::vector<physx::MPxTransformController*> m_vecTransformControllers;
};

// PhysX Object == Game::XActor
CPhysXObject::CPhysXObject(const char* _pSzMeshName)
{
	m_pEntity = new CPhysXObjectEntity(_pSzMeshName);
}

CPhysXObject::~CPhysXObject()
{
	delete m_pEntity;
}

void CPhysXObject::Update( CControlMediator* _pControlMediator, float _fTime )
{
	if (NULL == m_pEntity->GetActor())
		return;

	// for rendering
	_pControlMediator->GetHelperRender()->RegisterToInstantRenderSceneNode(m_pEntity->GetActor());

	// for simulation
	m_pEntity->Update(_fTime);
}

bool CPhysXObject::IsActorLoaded()
{
	return (NULL != m_pEntity->GetActor());
}

void CPhysXObject::StartBPart( int _idx )
{
	if (false == IsActorLoaded())
		return;

	// get actor node to simulate
	char chNodeName[257] = {0,};
	sprintf_s(chNodeName, "%s%02d", "b_parts_", _idx);

	rs3::RActorNode* pBreakablePartsNode = m_pEntity->GetActor()->GetActorNode(chNodeName);

	// �극��Ŀ�� ������ ���Ϳ� �������� �ʴ� ���
	if (pBreakablePartsNode == NULL)
		return;

	// Dynamic Actor node Transform controller
	physx::MPxDynamicActorNodeTC* pDynamicActorNodeTC = m_pEntity->GetTransformController<physx::MPxDynamicActorNodeTC>();
	_ASSERT(pDynamicActorNodeTC);

	// �극��Ŀ�� ������ �ùķ��̼����� ��� (������ �ùķ��̼ǿ� �극��Ŀ�� ���� �׷��� ������)
	physx::MPxTcDynamicActorNodesGroup* pDynamicActorNodesGroup = pDynamicActorNodeTC->GetDynamicActorNodesGroup(_idx);
	if (pDynamicActorNodesGroup)
		return;

	std::vector<std::string> vList;
	if (pDynamicActorNodeTC->MakeAndPrepareSimulationActorNodes(pBreakablePartsNode, vList) == false)
		return;

	// �ùķ��̼��� �� �극Ŀ�� ���� �׷� �����
	pDynamicActorNodesGroup = pDynamicActorNodeTC->CreateDynamicActorNodesGroup(_idx);
	_ASSERT(pDynamicActorNodesGroup);

	// �ùķ��̼� force, ������ �����ش�.
	rs3::RVector vForce, vForceDir = rs3::RVector::AXISZ;
	rs3::RVector vPartsPos(pBreakablePartsNode->GetWorldPosition());
	vForceDir = vPartsPos;
	vForceDir.z = 0.f;
	vForceDir.Normalize();
	pDynamicActorNodeTC->MakeUpwardRandomForce(vForce, &vForceDir);

	// start simulation
	pDynamicActorNodesGroup->StartPhysicSimulation(vList, vForce, vPartsPos, chNodeName);
}
