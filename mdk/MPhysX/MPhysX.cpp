#include "stdafx.h"

#include "MPhysX.h"

#include "MPxObjDynamic.h"
#include "MPxObjStatic.h"
#include "MPhysXUserAllocator.h"

#include "NxCooking.h"
#include "NxCapsuleController.h"
#include "ControllerManager.h"

#include "MTime.h"

#define _ASSERT_PX_DATA
#ifdef _ASSERT_PX_DATA
	#define _INIT_PX_DATA	\
		MPhysXHelper::CUpdatableChecker<true> ch(m_pScene);	\
		if(!ch.checkData()){ mlog("MPhysX Error : Some PhysX Objects have invalid value(before update)\n"); };
	#define _CHECK_PX_DATA	\
		if(!ch.checkData()){ mlog("MPhysX Error : Some PhysX Objects have invalid value(after update)\n"); };
#else
	#define _INIT_PX_DATA
	#define _CHECK_PX_DATA
#endif

#define MPX_DEFAULT_DYN_SELF_COL "def_dyn_self_col"
#define MPX_DEFAULT_DYN_NONE_SELF_COL "def_dyn_col"

namespace physx {

//////////////////////////////////////////////////////////////////////////
// Scene Collision Detector
//
class MPhysXCollisionReport : public NxUserContactReport
{
	void onContactNotify(NxContactPair& pair, NxU32 events)
	{
		// NX_NOTIFY_ON_START_TOUCH ������ �浹ó��
		if ( false == (events & NX_NOTIFY_ON_START_TOUCH) )
			return;

		// ������ ����
		if (pair.isDeletedActor[0] == true || pair.isDeletedActor[1] == true)
			return;

		// �� �� �ϳ��� NULL
		if (pair.actors[0] == NULL || pair.actors[1] == NULL)
			return;

		// static casting for callback
		MPxObjDynamic::DynamicObjectEntity* pDynamicObjEntity[2]  = {
			(MPxObjDynamic::DynamicObjectEntity* )pair.actors[0]->userData,
			(MPxObjDynamic::DynamicObjectEntity* )pair.actors[1]->userData
		};

		// callback
		for (int i = 0; i < 2; ++i)
		{
			if (pDynamicObjEntity[i])
				pDynamicObjEntity[i]->OnSubActorCollisionStarted(pair.actors[i]);
		}
	}
};

MPhysX* MPhysX::m_pInitedGlobalPhysX = NULL;

MPhysX::MPhysX() 
: m_pSDK(NULL)
, m_pRemoteDebugger(NULL)
, m_pScene(NULL)
, m_pCooking(NULL)
, m_pMyAllocator(NULL)
, m_vecSceneGravity(MVector3::ZERO)
, m_eMPhysXLOD(PL_DEFAULT)
, m_nLastGroup(0)
, m_eLastErrorCode(NXCE_NO_ERROR)
{
	m_fTimestep = 1.f / 60.f;

	m_Elapsed[E_1_PER_35].m_fElapsedTime = 1.f / 35.f;
	m_Elapsed[E_1_PER_35].m_nElapsedGroup = 0;

	m_Elapsed[E_1_PER_65].m_fElapsedTime = 1.f / 65.f;
	m_Elapsed[E_1_PER_65].m_nElapsedGroup = 0;

	m_Elapsed[E_1_PER_120].m_fElapsedTime = 1.f / 120.f;
	m_Elapsed[E_1_PER_120].m_nElapsedGroup = 0;

	m_Elapsed[E_1_PER_180].m_fElapsedTime = 1.f / 180.f;
	m_Elapsed[E_1_PER_180].m_nElapsedGroup = 0;

	m_fCurrentElapsed = 0.f;

	m_pCollisionReport = new MPhysXCollisionReport;

	gAppData = 0;
}

MPhysX::~MPhysX()
{
	_ASSERT( m_pSDK == NULL );
	ClearPxInfoXML();
	m_pInitedGlobalPhysX = NULL;

	delete m_pCollisionReport;
}

MPhysX::PHYSX_INIT_CODE MPhysX::Init(float fScale, const char* _pSzVisualRemoteDebuggerAddress, bool bUseCustomAllocator)
{
	_ASSERT( m_pMyAllocator == NULL );
	_ASSERT( m_pSDK == NULL );
	_ASSERT( m_pCooking == NULL );


	//////////////////////////////////////////////////////////////////////////
	// SDK Init
	//
	NxSDKCreateError errorCode = NXCE_NO_ERROR;
	if (true == bUseCustomAllocator)
		m_pMyAllocator = new MPhysXUserAllocator;

	m_pSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, m_pMyAllocator, NULL, NxPhysicsSDKDesc(), &errorCode);
	if( m_pSDK == NULL  ||  errorCode != 0) 
	{
		SAFE_DELETE(m_pMyAllocator);
		mlog("ERROR in PhysX, ERROR Code : %d\n", errorCode);

		m_eLastErrorCode = errorCode;
		return PIC_NOT_INSTALLED;
	}


	//////////////////////////////////////////////////////////////////////////
	// Cooking Init
	//
	m_pCooking = NxGetCookingLib(NX_PHYSICS_SDK_VERSION);
	if (m_pCooking == NULL)
	{
		mlog("Failed to Init PhysX Cooking\n");
		return PIC_FAILD;
	}
	m_pCooking->NxInitCooking();
	
	// ������ RemoteDebugger�� �����ϱ�
	//connectToVRD();

	//////////////////////////////////////////////////////////////////////////
	// Set the physics parameters
	//

	// �⺻������ object���� ���� ������ �� �ֵ��� ����ϴ¡�grace-depth���̴�.
	m_pSDK->setParameter(NX_SKIN_WIDTH, 0.025f*(fScale));
	m_pSDK->setParameter(NX_DEFAULT_SLEEP_LIN_VEL_SQUARED, 0.15f*0.15f*(fScale)*(fScale));
	m_pSDK->setParameter(NX_DEFAULT_SLEEP_ENERGY, 0.005f * (fScale) );
	m_pSDK->setParameter(NX_BOUNCE_THRESHOLD, -2.f*(fScale));

	// Set the debug visualization parameters
	m_pSDK->setParameter(NX_VISUALIZATION_SCALE, 1);			// ���� ������ ũ�� ����
	m_pSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);		// �浹 ��缱�� ũ��
	m_pSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);			// ��ǥ�༱�� ũ��
	m_pSDK->setParameter(NX_VISUALIZE_CLOTH_MESH, 1);

	//m_pSDK->setParameter(NX_VISUALIZE_COLLISION_FNORMALS, 1);
	//m_pSDK->setParameter(NX_VISUALIZE_CLOTH_VALIDBOUNDS, 1);
	//m_pSDK->setParameter(NX_CONTINUOUS_CD, true);
	//m_pSDK->setParameter(NX_CCD_EPSILON, 0.01f);


	//////////////////////////////////////////////////////////////////////////
	// Scene Description
	//
	NxSceneDesc sceneDesc;

	//sceneDesc.setToDefault();
	//sceneDesc.subdivisionLevel = 10;
	//sceneDesc.staticStructure = NX_PRUNING_STATIC_AABB_TREE;
	//sceneDesc.dynamicStructure = NX_PRUNING_DYNAMIC_AABB_TREE;
	//sceneDesc.simType = NX_SIMULATION_HW;
	//// Use 1 for Y = up, 2 for Z = up, or 0 to disable this feature. It is not possible to use X = up
	//sceneDesc.upAxis  = 2;

	sceneDesc.gravity = NxVec3(0, 0, -9.8f * fScale * fScale);
	m_vecSceneGravity.Set(0, 0, -9.8f * fScale * fScale);


	//////////////////////////////////////////////////////////////////////////
	// Description���� ���� �� ����
	//
	m_pScene = m_pSDK->createScene(sceneDesc);
	if(!m_pScene)
	{ 
		//�ϵ��� �������ϸ� ����Ʈ���� ���� �����Ѵ�
		sceneDesc.simType = NX_SIMULATION_SW; 
		m_pScene = m_pSDK->createScene(sceneDesc);  

		if(!m_pScene)
		{
			_ASSERT(0); //�� ���� ����
			return PIC_FAILD;
		}
	}

	// timestep
	m_pScene->setTiming(m_fTimestep, 8, NX_TIMESTEP_FIXED);


	//////////////////////////////////////////////////////////////////////////
	// Scene Default Material Init
	//
	NxMaterial* defaultMaterial = m_pScene->getMaterialFromIndex(0); 
	defaultMaterial->setRestitution(0.5f);		// ź�� ȸ����
	defaultMaterial->setStaticFriction(0.5f);	// ���������� : ��ü�� ó�� �����϶� �ʿ��� ������
	defaultMaterial->setDynamicFriction(0.5f);	// ������� : ��ü�� �����϶� ����Ǵ� ������


	//////////////////////////////////////////////////////////////////////////
	// Add Collision Report
	//
	m_pScene->setUserContactReport(m_pCollisionReport);
	AddCollisionGroup(MPX_DEFAULT_DYN_SELF_COL, true, true, true);
	AddCollisionGroup(MPX_DEFAULT_DYN_NONE_SELF_COL, true, false, true);

	//////////////////////////////////////////////////////////////////////////
	// Set Global MPhysXObject
	//
	m_pInitedGlobalPhysX = this;

	return PIC_SUCCESS;
}

void MPhysX::connectToVRD()
{
	if (m_pRemoteDebugger && m_pRemoteDebugger->isConnected())
	{
		m_pRemoteDebugger->disconnect();
		gAppData = 0;
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// The settings for the VRD host and port are found in SampleCommonCode/SamplesVRDSettings.h
		//
		m_pRemoteDebugger = m_pSDK->getFoundationSDK().getRemoteDebugger();
		m_pRemoteDebugger->connect("localhost");

		m_pRemoteDebugger->createObject(&gAppData, NX_DBG_OBJECTTYPE_GENERIC, "AppData", NX_DBG_EVENTMASK_EVERYTHING);
		m_pRemoteDebugger->writeParameter("Whatever data would be interesting for debugging", &gAppData, true, "Info", NX_DBG_EVENTMASK_EVERYTHING);
		m_pRemoteDebugger->createObject(&gAppData+1, NX_DBG_OBJECTTYPE_VECTOR, "AVector", NX_DBG_EVENTMASK_EVERYTHING);
		m_pRemoteDebugger->writeParameter(NxVec3(0, 0, 0), &gAppData+1, true, "Origin", NX_DBG_EVENTMASK_EVERYTHING);
		m_pRemoteDebugger->writeParameter(NxVec3(1, 1, 1), &gAppData+1, true, "Extent", NX_DBG_EVENTMASK_EVERYTHING);
		m_pRemoteDebugger->addChild(&gAppData, &gAppData+1, NX_DBG_EVENTMASK_EVERYTHING);
		gAppData = 1;
	}
}

void MPhysX::SetSceneGravity( const MVector3& _rGravity )
{
	_ASSERT( m_pScene != NULL );
	if (m_pScene)
	{
		m_pScene->setGravity( NxVec3(_rGravity.x, _rGravity.y, _rGravity.z) );
		m_vecSceneGravity.Set(_rGravity);
	}
}

void MPhysX::SetSceneGravity( float _fGravity )
{
	SetSceneGravity( MVector3(0.f, 0.f, _fGravity) );
}

void MPhysX::Destroy()
{
	_ASSERT( m_mapShapes.empty() );

	// �� �����ֱ�
	if (m_pScene != NULL)
	{
		// �� �����ֱ� ���� ���򿹾� NxActor �����ֱ�
		ClearReleaseReservedNxActor();

		// �� �����ֱ�
		m_pSDK->releaseScene(*m_pScene);
		m_pScene = NULL;
	}

	//Cooking ����
	if (m_pCooking)
	{
		m_pCooking->NxCloseCooking();
		m_pCooking = NULL;
	}

	// SDK ����
	if (m_pSDK) 
	{
		m_pSDK->release();
		m_pSDK = NULL;
	}

	// Allocator ����
	SAFE_DELETE(m_pMyAllocator);
}

void MPhysX::RemovePxObject(MPhysXObject* pPxObject)
{
	if (pPxObject)
	{
		MPhysXObject::PHYSX_OBJECT_TYPE eType = pPxObject->GetPxObjectType();
		m_PxObjects[eType].remove(pPxObject);
		delete pPxObject;
	}
}

void MPhysX::Update(float _fElapsed)
{
	if (NULL == m_pScene)
	{
//		mlog("Update Error : Check MPhysx Scene Creation\n");
		return;
	}

	m_fCurrentElapsed = _fElapsed;

	// Time Step
	SetTimeStep(_fElapsed);
	_ASSERT( (m_fTimestep <= _fElapsed) || (m_fTimestep == 1.f / 200.f) );

	// ������ ����� ������Ʈ
	// NxActor Updatable Checker
	_INIT_PX_DATA;
	m_pScene->simulate(_fElapsed);
	m_pScene->flushStream();
	m_pScene->fetchResults(NX_ALL_FINISHED, true);
	_CHECK_PX_DATA;

	// Ȱ��ȭ ���� ������Ʈ�� ���� ���� Ȯ��
	_ASSERT( m_vecActiveAreas.empty() == true );
	// Static Object�� ������ ������Ʈ ������Ʈ �� Ȱ��ȭ ���� Ȯ��
	for ( int i = MPhysXObject::PO_RIGID_DYNAMIC; i < MPhysXObject::PO_TOTAL_NUM; ++i )
	{
		for ( PX_OBJ_LIST::iterator itr = m_PxObjects[i].begin(); itr != m_PxObjects[i].end(); ++itr)
		{
			// MPhysX Object Update
			MPhysXObject* pPxObject = *itr;
			pPxObject->NotifyResultOnSimulated();

			// LOD�� ���� �ֺ� Ȱ��ȭ
			const MBox* pAreaBox = pPxObject->GetAreaBox();
			if (pAreaBox && PL_DEFAULT == m_eMPhysXLOD )
			{
				// �߸��� AABB �ɷ�����
				//float fLength = pAreaBox->GetRadius();
				//if ( fLength > 1000.f) continue;

				m_vecActiveAreas.push_back(pAreaBox);

				//mlog("Box vMin x:%f y:%f z:%f , vMax x:%f y:%f z:%f \n",
				//	pAreaBox->vmin.x,pAreaBox->vmin.y,pAreaBox->vmin.z,
				//	pAreaBox->vmax.x,pAreaBox->vmax.y,pAreaBox->vmax.z);
			}
		}
	}

	// Ȱ��ȭ ������ ���� Static Object ������Ʈ
	if (PL_NONE != m_eMPhysXLOD && !m_vecActiveAreas.empty())
	{
		//MTimeChecker tc;
		//tc.Start();

		for(PX_OBJ_LIST::iterator itr = m_PxObjects[MPhysXObject::PO_RIGID_STATIC].begin();
			itr != m_PxObjects[MPhysXObject::PO_RIGID_STATIC].end(); ++itr)
		{
			MPxObjStatic* pStaticBodyObject = static_cast<MPxObjStatic*>(*itr);
			pStaticBodyObject->ActivateStaticSubActorsByAreaList(m_vecActiveAreas);
		}

		//unsigned int nTime = tc.Check();
		//if (nTime > 3)
		//{
		//	mlog("[MPhysX] %u ms : in MPhysX::Update, making static mesh - overtime than 3ms\n", nTime);
		//}
		m_vecActiveAreas.clear();
	}

	// ���� NxActor �����ֱ�
	ClearReleaseReservedNxActor();
}

void MPhysX::ClearReleaseReservedNxActor()
{
	for (vector<NxActor *>::iterator itr = m_vecNxActorToRelease.begin(); itr != m_vecNxActorToRelease.end(); ++itr)
	{
		NxActor* pActor = *itr;
		NxU32 nShapes = pActor->getNbShapes();
		if(nShapes > 0)
		{
			NxShape*const* shapes = pActor->getShapes();
			while( nShapes-- )	
				pActor->releaseShape( *shapes[nShapes] );
		}
		pActor->getScene().releaseActor(*pActor);
	}
	m_vecNxActorToRelease.clear();
}

void MPhysX::SetTimeStep( float _fElapsed )
{
	int nIteration;
	int nThreshold;
	#define _FACTOR( _iteration, _threshold ) nIteration = _iteration; nThreshold = _threshold;

	// Set Timing
	if (_fElapsed < m_Elapsed[E_1_PER_180].m_fElapsedTime)
	{
		m_Elapsed[E_1_PER_35].m_nElapsedGroup = 0;
		m_Elapsed[E_1_PER_65].m_nElapsedGroup = 0;
		m_Elapsed[E_1_PER_120].m_nElapsedGroup = 0;
		m_Elapsed[E_1_PER_180].m_nElapsedGroup = 0;

		// 180 �� ������ 200���� ����
		_fElapsed = 1.f / 200.f;

		_FACTOR(2, 50);
	}
	else if (_fElapsed <  m_Elapsed[E_1_PER_120].m_fElapsedTime)
	{
		m_Elapsed[E_1_PER_35].m_nElapsedGroup = 0;
		m_Elapsed[E_1_PER_65].m_nElapsedGroup = 0;
		m_Elapsed[E_1_PER_120].m_nElapsedGroup = 0;
		m_Elapsed[E_1_PER_180].m_nElapsedGroup += 1;

		_FACTOR(4, 50);
	}
	else if (_fElapsed <  m_Elapsed[E_1_PER_65].m_fElapsedTime)
	{
		m_Elapsed[E_1_PER_35].m_nElapsedGroup = 0;
		m_Elapsed[E_1_PER_65].m_nElapsedGroup = 0;
		m_Elapsed[E_1_PER_120].m_nElapsedGroup += 1;
		m_Elapsed[E_1_PER_180].m_nElapsedGroup = 0;

		_FACTOR(6, 50);
	}
	else if (_fElapsed <  m_Elapsed[E_1_PER_35].m_fElapsedTime)
	{
		m_Elapsed[E_1_PER_35].m_nElapsedGroup = 0;
		m_Elapsed[E_1_PER_65].m_nElapsedGroup += 1;
		m_Elapsed[E_1_PER_120].m_nElapsedGroup = 0;
		m_Elapsed[E_1_PER_180].m_nElapsedGroup = 0;

		_FACTOR(8, 30);
	}
	else
	{
		m_Elapsed[E_1_PER_35].m_nElapsedGroup += 1;
		m_Elapsed[E_1_PER_65].m_nElapsedGroup = 0;
		m_Elapsed[E_1_PER_120].m_nElapsedGroup = 0;
		m_Elapsed[E_1_PER_180].m_nElapsedGroup = 0;

		_FACTOR(10, 10);
	}

	// �ʴ� �������� 180�� �Ѵ°��
	if (0 == m_Elapsed[E_1_PER_35].m_nElapsedGroup &&
		0 == m_Elapsed[E_1_PER_65].m_nElapsedGroup &&
		0 == m_Elapsed[E_1_PER_120].m_nElapsedGroup &&
		0 == m_Elapsed[E_1_PER_180].m_nElapsedGroup)
	{
		if (_fElapsed < m_fTimestep)
		{
			SetTiming(_fElapsed, nIteration);
		}
	}

	// timing ����
	#define _SET_TIMING( _t ) {																\
		if ( 0 < m_Elapsed[E_1_PER_##_t##].m_nElapsedGroup )	{							\
			bool bCandidate = false;														\
			if( _fElapsed < m_fTimestep ) bCandidate = true;								\
			if( nThreshold < m_Elapsed[E_1_PER_##_t##].m_nElapsedGroup ) bCandidate = true;	\
			if (m_Elapsed[E_1_PER_##_t##].m_fElapsedTime != m_fTimestep && bCandidate){		\
				SetTiming(m_Elapsed[E_1_PER_##_t##].m_fElapsedTime, nIteration);			\
				m_Elapsed[E_1_PER_##_t##].m_nElapsedGroup = 0;								\
			}																				\
		}																					\
	}

	_SET_TIMING( 35 );
	_SET_TIMING( 65 );
	_SET_TIMING( 120 );
	_SET_TIMING( 180 );
}

void MPhysX::SetTiming( float _fTimeStep, int _nIteration )
{
	m_fTimestep = _fTimeStep;
	m_pScene->setTiming(m_fTimestep/ (float)_nIteration, _nIteration + int( _nIteration/2 ) , NX_TIMESTEP_FIXED);
}

bool MPhysX::CreatePxInfoXML( const std::string& strInfoFileName, bool bReCreate )
{
	// �̹� �����Ǿ� �ִ°Ÿ� ���������� �ʴ´�.
	if (IsCreatedPxInfoXML(strInfoFileName))
	{
		if (!bReCreate)
			return true;

		PX_INFO_OBJ& objInfo = m_mapPxInfo[strInfoFileName].m_FileInfo;
		for (PX_INFO_OBJ::iterator itr_obj = objInfo.begin(); itr_obj != objInfo.end(); ++itr_obj)
		{
			PX_INFO_XML& xmlInfo = itr_obj->second;
			for (PX_INFO_XML::iterator itr_xml = xmlInfo.begin(); itr_xml != xmlInfo.end(); ++itr_xml)
			{
				delete itr_xml->second;
			}
		}
		objInfo.clear();
	}

	MXml aXml;
	if (!aXml.LoadFile(strInfoFileName.c_str()))
		return false;

	MXmlElement* pObjectElement = aXml.DocHandle().FirstChildElement("ROOT").FirstChildElement("MPHYSX").FirstChildElement("OBJECT").Element();
	if (!pObjectElement)
		return false;

	std::string strContextName;
	pObjectElement = pObjectElement->FirstChildElement();
	while (pObjectElement)
	{
		MXmlElement* pContext = pObjectElement->FirstChildElement("CONTEXT");
		while(pContext)
		{
			_Attribute(strContextName, pContext, "name");
			m_mapPxInfo[strInfoFileName].m_FileInfo[pObjectElement->ValueStr()][strContextName] = (MXmlElement*)pContext->Clone();
			pContext = pContext->NextSiblingElement("CONTEXT");
		}
		pObjectElement = pObjectElement->NextSiblingElement();
	}
	return true;
}

void MPhysX::ClearPxInfoXML()
{
	for (PX_INFO::iterator itr = m_mapPxInfo.begin(); itr != m_mapPxInfo.end(); ++itr)
	{
		PX_INFO_OBJ& objInfo = itr->second.m_FileInfo;
		for (PX_INFO_OBJ::iterator itr_obj = objInfo.begin(); itr_obj != objInfo.end(); ++itr_obj)
		{
			PX_INFO_XML& xmlInfo = itr_obj->second;
			for (PX_INFO_XML::iterator itr_xml = xmlInfo.begin(); itr_xml != xmlInfo.end(); ++itr_xml)
			{
				delete itr_xml->second;
			}
		}
	}

	m_mapPxInfo.clear();
}

bool MPhysX::IsCreatedPxInfoXML( const std::string& strInfoFileName )
{
	PX_INFO::iterator itr_info = m_mapPxInfo.find(strInfoFileName);
	return (itr_info != m_mapPxInfo.end());
}

int MPhysX::AddCollisionGroup( const char* _pSzCollsionGroup, bool _bWithDefaultCollision, bool _bWithSelfGroupCollision, bool _bWithOtherGroupCollision )
{
	if (m_mapCollisionGroups.find( _pSzCollsionGroup ) != m_mapCollisionGroups.end())
		return -1;

	// ���ο� ���̵�
	++m_nLastGroup;

	std::pair< PX_COL::iterator, bool > ret = m_mapCollisionGroups.insert(PX_COL::value_type( _pSzCollsionGroup, SCollisionGroup(m_nLastGroup, _bWithDefaultCollision, _bWithSelfGroupCollision, _bWithOtherGroupCollision) ));
	_ASSERT(ret.second == true);
	SCollisionGroup& rNewed = ret.first->second;

	// �⺻ �浹
	if (!rNewed.m_bColWithDefault)
	{
		m_pScene->setGroupCollisionFlag(0, rNewed.m_nGroupID, false);
	}
	else
	{
		m_pScene->setGroupCollisionFlag(0, rNewed.m_nGroupID, true);

		// ���� �⺻ �浹�� �浹 ���� �̺�Ʈ�� �����ϰ� �Ѵ�.
		m_pScene->setActorGroupPairFlags(0, rNewed.m_nGroupID, NX_NOTIFY_ON_START_TOUCH);
	}

	// �׷����� �浹
	for (PX_COL::iterator itr = m_mapCollisionGroups.begin(); itr != m_mapCollisionGroups.end(); ++itr)
	{
		SCollisionGroup& rCol = itr->second;
		if (rCol.m_nGroupID == rNewed.m_nGroupID)
		{
			// �ڱ� �ڽ��� ���
			m_pScene->setGroupCollisionFlag(rCol.m_nGroupID, rNewed.m_nGroupID, rNewed.m_bColWithSelfGroup);
		}
		else
		{
			// �׷��� ������ 
			m_pScene->setGroupCollisionFlag(rCol.m_nGroupID, rNewed.m_nGroupID, rNewed.m_bColWithOtherGroup);
		}
	}

	// �߱޵� ���ο� ���̵� ����
	return m_nLastGroup;
}

int MPhysX::GetCollsionGroupId( const std::string& _rStrGroupName )
{
	PX_COL::iterator itr = m_mapCollisionGroups.find(_rStrGroupName);
	if(itr == m_mapCollisionGroups.end())
		return -1;

	return itr->second.m_nGroupID;
}

int MPhysX::GetDefaultDynamicObjectCollisionGroupId(bool bSelfGroupCollision)
{
	if (bSelfGroupCollision)
		return GetCollsionGroupId(MPX_DEFAULT_DYN_SELF_COL);

	return GetCollsionGroupId(MPX_DEFAULT_DYN_NONE_SELF_COL);
}

MPhysXShape* MPhysX::CreatePxShape( MPhysXShape::PHYSX_SHAPE_TYPE _eShapeType, const std::string& _strShapeResourceName )
{
	MPhysXShape* pShape = FindPxShape(_strShapeResourceName);
	if (pShape)
	{
		++pShape->m_nRefCnt;
	}
	else
	{
		pShape = new MPhysXShape(_eShapeType, _strShapeResourceName, this);
		m_mapShapes.insert( PX_SHAPE_MAP::value_type(_strShapeResourceName, pShape) );
	}

	return pShape;
}

MPhysXShape* MPhysX::CreatePxShape( NxShapeDesc* _pShapeDesc )
{
	MPhysXShape* pShape = new MPhysXShape(_pShapeDesc, this);
	m_mapShapes.insert( PX_SHAPE_MAP::value_type(pShape->m_strShapeName, pShape) );

	return pShape;
}

MPhysXShape* MPhysX::FindPxShape( const std::string& _strShapeResourceName )
{
	PX_SHAPE_MAP::iterator itr = m_mapShapes.find(_strShapeResourceName);
	if (itr != m_mapShapes.end())
	{
		return itr->second;
	}

	return NULL;
}

void MPhysX::RemovePxShape( MPhysXShape* _pShape )
{
	if (!_pShape)
		return;

	--_pShape->m_nRefCnt;
	if (_pShape->m_nRefCnt == 0)
	{
		m_mapShapes.erase(_pShape->m_strShapeName);
		delete _pShape;
	}
}

void MPhysX::RemovePxShape( const std::string& _strShapeResourceName )
{
	PX_SHAPE_MAP::iterator itr = m_mapShapes.find(_strShapeResourceName);
	if (itr != m_mapShapes.end())
	{
		RemovePxShape(itr->second);
	}
}

} // namespace physx