#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////

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

#include "NxCooking.h"
#include "RPhysXStream.h"

#pragma pop_macro("min")
#pragma pop_macro("max")

//////////////////////////////////////////////////////////////////////////

#include "RPhysXClothController.h"
#include "RPhysXActorControllerBuilder.h"

#include "RActorNode.h"
#include "RPhysXClothActorNode.h"
#include "RActor.h"

#include "REngine.h"
#include "RSceneManager.h"
#include "RDebugPassRenderer.h"
#include "RPathSplitter.h"

#include "RMesh.h"
#include "RMeshNode.h"

namespace rs3 {

void RPhysXClothController::ReNew()
{
	RefreshUpdateInfoMap();

	// �ʰ� 
	for (CLOTH_MAP::iterator itr = m_mapClothNodesAdded.begin(); itr != m_mapClothNodesAdded.end(); ++itr)
	{
		SNxCloth& rCloth = itr->second;

		// ���� - ���������� �Ѵ�.
		if (rCloth.m_vecAnchorNodes.empty())
		{
			// ��Ŀ�� ������ ���� ������ �ؾ��ϴ� ���� �Ǵ��Ѵ�.
			RPhysXClothActorNode* pClothActorNode = dynamic_cast<RPhysXClothActorNode*>(m_pActor->GetActorNode(itr->first.c_str()));
			if (NULL == pClothActorNode)
			{
				// ���ҽ� �����̰ų� Ŭ�ξ� ���ͳ�尡 �������ٰ� �ٷ� ������ ���
				// ( ��׶��� �ε��߿� ������ ��û�Ѱ�� )
				continue;
			}

			// ��Ŀ �����
			NXACTOR_MAP mapAnchor;
			CreateClothAnchor(pClothActorNode, mapAnchor);
			if (mapAnchor.empty())
			{
				// ���� ��Ŀ��尡 �ȸ������ ���(Ȥ�� ��Ŀ��尡 ���°��)
				continue;
			}

			// �ʱ⿡ CPU�� Ŭ�ξ� ���ͳ�� ����
			// ���� PhysX Ŭ�ξ��� ���ϴ� ������ ���������ȴ�.
			pClothActorNode->ChangeClothState(RPhysXClothActorNode::E_CPU);
			m_mapClothCandidates.insert( CLOTH_CANDIDATE_MAP::value_type(pClothActorNode->GetNodeName(), 0) );

			// ��Ŀ���
			for (NXACTOR_MAP::iterator itr_anchor = mapAnchor.begin(); itr_anchor != mapAnchor.end(); ++itr_anchor)
			{
				const std::string& rNodeName = itr_anchor->first;
				NxActor* pNxActor = itr_anchor->second;

				_ASSERT(m_mapAnchorNxActors.find(rNodeName) == m_mapAnchorNxActors.end());

				// ��Ŀ ���ͳ��
				RActorNode* pAnchorActorNode = m_pActor->GetActorNode(rNodeName.c_str());
				_ASSERT(pAnchorActorNode != NULL);

				// ��Ŀ ���� �����ֱ⸦ ���ش�
				pAnchorActorNode->SetVisible(false);

				// ���ҽ��� �߰�
				m_mapAnchorNxActors.insert( NXACTOR_MAP::value_type(rNodeName, pNxActor) );

				// ������Ʈ �ʿ� �߰�
				m_vecNxUpdateInfo.push_back(SNxUpdateInfo(rNodeName, pNxActor, NULL, ADD_HIERARCHY_ATTR, pAnchorActorNode));

				// ��Ŀ����Ͽ� �߰�
				rCloth.m_vecAnchorNodes.push_back(rNodeName);
			}
		}
	}

	// �浹ü
	CreateClothCollision();
}

void RPhysXClothController::CreateClothCollision()
{
	// �浹ü ���� ����
	typedef std::map< std::string, std::map<std::string, RActorCollisionObject*> > COL_INFO;
	COL_INFO mapCollsions;
	std::map<std::string,  RActorCollisionObject*> mapColObj;
	RPathSplitter splitter;
	RActorCollision* pCollision = m_pActor->m_pCollision;
	for(size_t i=0;i<pCollision->m_vCollisions.size(); i++)
	{
		RActorCollisionObject& rCollisionObject = pCollision->m_vCollisions[i];
		if ( rCollisionObject.m_MeshType != MCollision::MESHTYPE_CLOTH )
			continue;

		if (NULL == rCollisionObject.m_pParent)
			continue;

		splitter.SetPath(rCollisionObject.m_strRelatedNodeNames);
		const vector<string>& rStrClothNodeNamesSplitted = splitter.GetPathList();
		for (vector<string>::const_iterator itr_cloth = rStrClothNodeNamesSplitted.begin(); itr_cloth != rStrClothNodeNamesSplitted.end(); ++itr_cloth)
		{
			mapCollsions[itr_cloth->substr(0, itr_cloth->length()-1)][rCollisionObject.m_pParent->GetNodeName()] = &rCollisionObject;
			mapColObj[rCollisionObject.m_pParent->GetNodeName()] = &rCollisionObject;
		}
	}

	// ���� ���� �浹ü NxActor
	// ���� ������� ���翡 �ִ°��� �����ϰ� �����ִ°� �� ����� �˰���
	// ���� ������� ���翡 �ִ��� ���ΰ� �߿�
	// ���翡 �ִ� ���� ����� ����� ����� ������ �� ��� ������ ���ο���� ������ ������ ��ü
	NXACTOR_MAP mapNewNxActors;
	std::set<std::string> setNotDeleted;
	for (COL_INFO::iterator itr = mapCollsions.begin(); itr != mapCollsions.end(); ++itr)
	{
		// �䱸�Ǵ� Ŭ�ξ��� ���� ������ Ŭ�ξ� ��Ͽ� ������
		if (m_mapClothNodesAdded.find(itr->first) == m_mapClothNodesAdded.end())
			continue;

		// �䱸�Ǵ� Ŭ�ξ��� ���ͳ��� ������
		if (NULL == m_pActor->GetActorNode( itr->first.c_str() ))
			continue;

		for (std::map<std::string, RActorCollisionObject*>::iterator itr_actor_node = itr->second.begin(); itr_actor_node != itr->second.end(); ++itr_actor_node)
		{
			// ���ͳ�尡 ���°��
			const std::string& rActorNodeName = itr_actor_node->first;
			RActorNode* pActorNode = m_pActor->GetActorNode( rActorNodeName.c_str() );
			if (NULL == pActorNode)
				continue;

			NXACTOR_MAP::iterator itr_nx_old = m_mapCollsionNxActors.find(rActorNodeName);

			// ������ �ִ°��
			if ( itr_nx_old != m_mapCollsionNxActors.end() )
			{
				if (mapNewNxActors.find(rActorNodeName) != mapNewNxActors.end())
					continue;

				mapNewNxActors.insert( NXACTOR_MAP::value_type(itr_nx_old->first, itr_nx_old->second) );

				// ���߿� m_mapCollsionNxActors �� �ִ°��� �� �����ٰ��̱� ������
				// �����ﲨ�� mapNewNxActors �� �ű�� m_mapCollsionNxActors ���� �����ش�.
				setNotDeleted.insert(itr_nx_old->first);
				m_mapCollsionNxActors.erase(itr_nx_old);
			}
			// ������ ������ ���
			else
			{
				if (mapNewNxActors.find(rActorNodeName) != mapNewNxActors.end())
					continue;

				// �����ֱ�
				NxActor* pNxActor = NULL;
				RActorCollisionObject* pCollsionObject = itr_actor_node->second;
				switch(pCollsionObject->m_Type)
				{
				case MCollision::CAPSULE:
					pNxActor = CreateNxActorFromCapsuleCollision(pCollsionObject->m_Capsule);
					break;
				}

				if (pNxActor)
					mapNewNxActors.insert( NXACTOR_MAP::value_type(rActorNodeName, pNxActor) );
			}
		}
	}

	// �������߿��� ���� �ʿ��� ���� �ƴѰ� �����ֱ�
	for (NXACTOR_MAP::iterator itr = m_mapCollsionNxActors.begin(); itr != m_mapCollsionNxActors.end(); ++itr)
	{
		RemoveNxActorUpdateInfo(itr->second);
		m_pScene->releaseActor(* (itr->second) );
	}

	// �������� ��������� ����
	m_mapCollsionNxActors = mapNewNxActors;

	// ������Ʈ �� ����
	for (NXACTOR_MAP::iterator itr = m_mapCollsionNxActors.begin(); itr != m_mapCollsionNxActors.end(); ++itr)
	{
		if(setNotDeleted.find(itr->first) != setNotDeleted.end() )
			continue;

		// ������Ʈ �ʿ� �߰�
		RActorCollisionObject* pCollisionObject = mapColObj[itr->first];
		_ASSERT(pCollisionObject);
		if (pCollisionObject == NULL)
			continue;

		RActorNode* pActorNode = dynamic_cast<RActorNode*>(pCollisionObject->m_pParent);
		_ASSERT(pActorNode);
		if (pActorNode == NULL)
			continue;

		m_vecNxUpdateInfo.push_back(SNxUpdateInfo(itr->first, itr->second, NULL, ADD_COLLISION_DATA | ADD_HIERARCHY_ATTR, pActorNode, pCollisionObject));
	}
}

void RPhysXClothController::AddClothNode( const std::string& _rNodeName )
{
	_ASSERT(m_mapClothNodesAdded.find(_rNodeName) == m_mapClothNodesAdded.end());
	m_mapClothNodesAdded.insert( CLOTH_MAP::value_type(_rNodeName, SNxCloth() ) );
}

void RPhysXClothController::RemoveClothNode( const std::string& _rNodeName )
{
	CLOTH_MAP::iterator itr = m_mapClothNodesAdded.find(_rNodeName);
	_ASSERT(itr != m_mapClothNodesAdded.end());
	if (itr == m_mapClothNodesAdded.end())
		return;

	SNxCloth& rCloth = itr->second;

	// NxCloth �����ֱ�
	ClearNxCloth(rCloth);

	// ��Ŀ �����ֱ�
	for (std::vector<std::string>::iterator itr = rCloth.m_vecAnchorNodes.begin(); itr != rCloth.m_vecAnchorNodes.end(); ++itr)
	{
		NXACTOR_MAP::iterator itr_anchor = m_mapAnchorNxActors.find(*itr);
		if (itr_anchor != m_mapAnchorNxActors.end())
		{
			RemoveNxActorUpdateInfo(itr_anchor->second);
			m_pScene->releaseActor(*(itr_anchor->second));
			m_mapAnchorNxActors.erase(itr_anchor);
		}
	}

	// Ŭ�ξ� ���ʿ��� �����ֱ�
	m_mapClothNodesAdded.erase(itr);

	// stable candidates������ �����ֱ�
	CLOTH_CANDIDATE_MAP::iterator itr_candidate = m_mapClothCandidates.find(_rNodeName);
	if (itr_candidate != m_mapClothCandidates.end())
		m_mapClothCandidates.erase(itr_candidate);

	// cf. extern actornode�� ��Ʈ�ѷ� �Ҹ����� ClearExternAnchorNodes���� �����ش�
}

void RPhysXClothController::CreateCloth(RPhysXClothActorNode* pActorNode, SNxCloth& _rCloth)
{
	_ASSERT(m_pSDK != NULL);
	_ASSERT(m_pScene != NULL);
	_ASSERT(m_pCooking != NULL);

	_ASSERT(_rCloth.m_pCloth == NULL);
	_ASSERT(_rCloth.m_pClothMesh == NULL);


	//////////////////////////////////////////////////////////////////////////
	//
	// Create Buffer For Cloth Creation From RealSpace
	//
	//////////////////////////////////////////////////////////////////////////

	RMeshNode* pMeshNode = pActorNode->m_pMeshNode;

	int nVertexNum	= pMeshNode->m_nVertexIndexTableCnt;
	int nFaceIndexNum = pMeshNode->m_nFaceIndexTableCnt;

	NxVec3*	pPoints	= (NxVec3*)malloc( sizeof(NxVec3) * nVertexNum );
	NxU32*	pFaces	= (NxU32*)malloc( sizeof(NxU32) * nFaceIndexNum );


	//////////////////////////////////////////////////////////////////////////
	//
	// Get Mesh Information from RActorNode::m_pMeshNode
	//
	//////////////////////////////////////////////////////////////////////////

	// create vertex buffer from rs vertex buffer
	BYTE* pVertex = (BYTE*)REngine::GetDevice().LockVertexBuffer(pMeshNode->m_pVb->m_hVB);
	_ASSERT(pVertex != NULL);
	const int nVertexSize = pMeshNode->m_pVb->m_nVertexSize;
	NxVec3* vDest = pPoints;
	for (int i = 0; i < nVertexNum; i++, vDest++)
	{
		RVector* pV = (RVector*) ( pVertex + pMeshNode->m_pVb->m_nPosOffset + i * nVertexSize );
		vDest->x = pV->x;
		vDest->y = pV->y;
		vDest->z = pV->z;
	}
	REngine::GetDevice().UnlockVertexBuffer(pMeshNode->m_pVb->m_hVB);

	//// create vertex buffer
	//NxVec3* vDest	= pPoints;
	//for (int i = 0; i < nVertexNum; i++, vDest++)
	//{
	//	NxVec3 vRet = pMeshNode->m_pPointTable[pMeshNode->m_pVertexIndexTable[i].p];
	//	vDest->x = vRet.x;
	//	vDest->y = vRet.y;
	//	vDest->z = vRet.z;
	//}

	// create face buffer
	NxU32 *vFace = pFaces;
	for (int i = 0; i < nFaceIndexNum; i++, vFace++)
		*vFace = (NxU32)pMeshNode->m_pFaceIndexTable[i];


	//////////////////////////////////////////////////////////////////////////
	//
	// Cloth Desc
	//
	//////////////////////////////////////////////////////////////////////////

	NxClothDesc ClothDesc;
	FillClothDesc(ClothDesc, _rCloth, pActorNode);


	//////////////////////////////////////////////////////////////////////////
	//
	// Cloth Mesh Desc / Cloth Mesh �����
	//
	//////////////////////////////////////////////////////////////////////////

	NxClothMeshDesc ClothMeshDesc;
	ClothMeshDesc.numVertices				= nVertexNum;
	ClothMeshDesc.numTriangles				= nFaceIndexNum/3;
	ClothMeshDesc.pointStrideBytes			= sizeof(NxVec3);
	ClothMeshDesc.triangleStrideBytes		= 3 * sizeof(NxU32);
	ClothMeshDesc.vertexMassStrideBytes		= sizeof(NxReal);
	ClothMeshDesc.vertexFlagStrideBytes		= sizeof(NxU32);
	ClothMeshDesc.points					= pPoints;
	ClothMeshDesc.triangles					= pFaces;
	ClothMeshDesc.flags						= 0;
	ClothMeshDesc.vertexFlags				= 0;
	//ClothMeshDesc.flags					= NX_CLOTH_MESH_WELD_VERTICES;
	//ClothMeshDesc.weldingDistance			= 0.0001f;

	if (ClothMeshDesc.flags & NX_CLF_TEARABLE)
		ClothMeshDesc.flags |= NX_CLOTH_MESH_TEARABLE;

	// Enable Debug rendering for this cloth
	ClothMeshDesc.flags |= NX_CLF_VISUALIZATION;

	_ASSERT( (ClothMeshDesc.flags & NX_CF_16_BIT_INDICES) == 0 );
	_ASSERT(ClothMeshDesc.isValid());

	// ClothMeshDesc�� ������ ���۸� ������ cloth mesh �����
	RPhysXMemoryWriteBuffer wb;
	if ( m_pCooking->NxCookClothMesh(ClothMeshDesc, wb) == false ) 
	{
		assert(0 && "Error: Cook Failed!!\n");
		return;
	}

	RPhysXMemoryReadBuffer rb(wb.data);
	ClothDesc.clothMesh = m_pSDK->createClothMesh(rb);


	//////////////////////////////////////////////////////////////////////////
	//
	// Cloth Data �����
	// Tearing ���� �ùķ��̼ǿ� ���� ����Ǿ����� ����� ����
	//
	//////////////////////////////////////////////////////////////////////////

	NxMeshData& meshData = ClothDesc.meshData;

	NxU32 maxVertices					= (NxU32)ClothMeshDesc.numVertices;
	meshData.verticesPosBegin			= (NxVec3*)malloc(sizeof(NxVec3) * maxVertices);
	meshData.verticesNormalBegin		= (NxVec3*)malloc(sizeof(NxVec3) * maxVertices);
	meshData.verticesPosByteStride		= sizeof(NxVec3);
	meshData.verticesNormalByteStride	= sizeof(NxVec3);
	meshData.maxVertices				= maxVertices;
	meshData.numVerticesPtr				= (NxU32*)malloc(sizeof(NxU32));

	// the number of triangles is constant, even if the cloth is torn
	NxU32 maxIndices					= 3 * ClothMeshDesc.numTriangles;
	meshData.indicesBegin				= (NxU32*)malloc(sizeof(NxU32)*maxIndices);
	meshData.indicesByteStride			= sizeof(NxU32);
	meshData.maxIndices					= maxIndices;
	meshData.numIndicesPtr				= (NxU32*)malloc(sizeof(NxU32));

	// the parent index information would be needed if we used textured cloth
	NxU32 maxParentIndices				= maxVertices;
	meshData.parentIndicesBegin			= (NxU32*)malloc(sizeof(NxU32)*maxParentIndices);
	meshData.parentIndicesByteStride	= sizeof(NxU32);
	meshData.maxParentIndices			= maxParentIndices;
	meshData.numParentIndicesPtr		= (NxU32*)malloc(sizeof(NxU32));
	meshData.dirtyBufferFlagsPtr		= 0;

	// init the buffers in case we want to draw the mesh before the SDK as filled in the correct values
	*(meshData.numVerticesPtr)			= 0;
	*(meshData.numIndicesPtr)			= 0;


	//////////////////////////////////////////////////////////////////////////
	//
	// Cloth �����
	//
	//////////////////////////////////////////////////////////////////////////

	_rCloth.m_pCloth = m_pScene->createCloth(ClothDesc);
	_ASSERT(_rCloth.m_pCloth && "Error: Unable to create the cloth for the current scene.\n");

	_rCloth.m_pClothMesh = ClothDesc.clothMesh;
	_rCloth.m_MeshData = ClothDesc.meshData;


	//////////////////////////////////////////////////////////////////////////
	//
	// Delete Buffer For Cloth Creation From RealSpace
	//
	//////////////////////////////////////////////////////////////////////////

	free(pPoints);
	free(pFaces);
}

void RPhysXClothController::CreateClothAnchor(RActorNode* _pClothActorNode, NXACTOR_MAP& _rAnchorOut)
{
	RMeshNode* pMeshNode = _pClothActorNode->m_pMeshNode;
	int nUserPropertySize = pMeshNode->GetUserPropertyCount();

	// ������Ƽ�κ��� ���� ������
	std::vector< std::string > vAnchorNames;
	for (int i = 0; i < nUserPropertySize; ++i)
	{
		const std::string& rProperty = pMeshNode->GetUserPropertyString(i);
		size_t tPos = rProperty.find("set_cloth_anchor_name");
		if (tPos != std::string::npos)
		{
			size_t tEqPos = rProperty.find_last_of(" = ");
			RPathSplitter splitter;
			splitter.SetPath( rProperty.substr(tEqPos + 1) );
			const vector<string>& rStrAnchorNamesSplitted = splitter.GetPathList();
			for (vector<string>::const_iterator itr = rStrAnchorNamesSplitted.begin(); itr != rStrAnchorNamesSplitted.end(); ++itr)
			{
				vAnchorNames.push_back( itr->substr(0, itr->length()-1) );
			}

			break;
		}
	}

	// ��Ŀ �����
	std::vector<NxVec3> vertices;
	std::vector<NxU32> faces;
	for (std::vector< std::string >::iterator itr = vAnchorNames.begin(); itr != vAnchorNames.end(); ++itr)
	{
		if (_rAnchorOut.find(*itr) != _rAnchorOut.end())
			continue;
		
		RActorNode* pAnchorActorNode = m_pActor->GetActorNode(itr->c_str());
		if (NULL == pAnchorActorNode)
		{
			RMeshNode* pMeshNode = _pClothActorNode->m_pMeshNode->m_pParentMesh->GetNode(itr->c_str());
			if (pMeshNode != NULL && m_mapExternAnchorActorNodeCandidates.find(*itr) == m_mapExternAnchorActorNodeCandidates.end())
			{
				RActorNode* pActorNode = m_pActor->GetActorNode(pMeshNode->GetParentName());
				m_mapExternAnchorActorNodeCandidates.insert( ANCHOR_CANDIDATE_MAP::value_type(*itr,  std::pair<RMeshNode*, RActorNode*>(pMeshNode, pActorNode) ) );
			}
			continue;
		}

		vertices.clear();
		faces.clear();

		for(int i = 0; i < pAnchorActorNode->m_pMeshNode->m_nVertexIndexTableCnt; ++i )
		{
			int nPosIndex = pAnchorActorNode->m_pMeshNode->m_pVertexIndexTable[i].p;
			RVector3 vRet = pAnchorActorNode->m_pMeshNode->m_pPointTable[nPosIndex];
			vertices.push_back( NxVec3( vRet.x, vRet.y, vRet.z ) );
		}

		for (int i = 0; i < pAnchorActorNode->m_pMeshNode->m_nFaceIndexTableCnt; ++i)
		{
			NxU32 f = (NxU32)pAnchorActorNode->m_pMeshNode->m_pFaceIndexTable[i];
			faces.push_back(f);
		}

		// mesh desc
		//NxTriangleMeshDesc triMeshDesc;	// ���̳��� ���ͳ����� �ƴϴ�.
		NxConvexMeshDesc convexMeshDesc;

		convexMeshDesc.numVertices				= (NxU32)vertices.size();
		convexMeshDesc.numTriangles				= (NxU32)faces.size() / 3;
		convexMeshDesc.pointStrideBytes			= sizeof(NxVec3);
		convexMeshDesc.triangleStrideBytes		= 3 * sizeof(NxU32);
		convexMeshDesc.points					= &vertices[0];
		convexMeshDesc.triangles				= &faces[0];
		convexMeshDesc.flags					= NX_MF_HARDWARE_MESH;

		RPhysXMemoryWriteBuffer wb;
		assert(convexMeshDesc.isValid());

		//if ( m_pCooking->NxCookTriangleMesh(triMeshDesc, wb) == false)
		if ( m_pCooking->NxCookConvexMesh(convexMeshDesc, wb) == false)
		{
			_ASSERT(!"Triangle Mesh Cook Failed!!");
			continue;
		}

		RPhysXMemoryReadBuffer rb(wb.data);

		// mesh shape desc
		//NxTriangleMeshShapeDesc meshDesc;
		NxConvexShapeDesc meshDesc;
		meshDesc.group = m_nCollsionGroup;

		//meshDesc.meshData = m_pSDK->createTriangleMesh(rb);
		meshDesc.meshData = m_pSDK->createConvexMesh(rb);

		// NxActor
		NxActorDesc actorDesc;
		NxBodyDesc	bodyDesc;
		actorDesc.density = 1.f;
		actorDesc.flags |= NX_AF_DISABLE_COLLISION;
		bodyDesc.flags |= NX_BF_KINEMATIC;
		actorDesc.body = &bodyDesc;
		actorDesc.shapes.push_back(&meshDesc);
		_ASSERT(actorDesc.isValid());

		NxActor* pNxActor = m_pScene->createActor(actorDesc);
		if (pNxActor == NULL)
		{
			while (pNxActor != NULL)
			{
				pNxActor = m_pScene->createActor(actorDesc);
			}
		}

		// ���� �Ӽ�
		//pNxActor->setSolverIterationCount(20);
		_rAnchorOut.insert( NXACTOR_MAP::value_type( *itr, pNxActor) );
	}
}

NxActor* RPhysXClothController::CreateNxActorFromCapsuleCollision( const RCapsule& _rCapsule )
{
	NxCapsuleShapeDesc capsuleDesc;
	capsuleDesc.height = _rCapsule.height;
	capsuleDesc.radius = _rCapsule.radius;
	//capsuleDesc.skinWidth = 0.00001f;
	capsuleDesc.group = m_nCollsionGroup;

	// NxActor
	NxActorDesc actorDesc;
	NxBodyDesc	bodyDesc;
	actorDesc.density = 1.f;
	bodyDesc.flags |= NX_BF_KINEMATIC;
	actorDesc.body = &bodyDesc;

	actorDesc.shapes.push_back(&capsuleDesc);

	_ASSERT(actorDesc.isValid());
	NxActor* pNxActor = m_pScene->createActor(actorDesc);
	if (pNxActor == NULL)
	{
		while (pNxActor != NULL)
		{
			pNxActor = m_pScene->createActor(actorDesc);
		}
	}
	return pNxActor;
}

void RPhysXClothController::ClearCollsionNxActors()
{
	for (NXACTOR_MAP::iterator itr = m_mapCollsionNxActors.begin(); itr != m_mapCollsionNxActors.end(); ++itr)
	{
		NxActor* pActor = itr->second;
		m_pScene->releaseActor(*pActor);
	}

	m_mapCollsionNxActors.clear();
}

void RPhysXClothController::Update(RCameraSceneNode* pCamera)
{
	if (m_pActor->GetVisible()== false)
		return;

	// update extern Anchor ActorNode candidates
	UpdateExternAnchorActorNodesCreation();
	
	// cloth candidates
	UpdateClothCreationCandidates();

	RPhysXActorController::Update(pCamera);
}

void RPhysXClothController::ClearExternAnchorActorNodes()
{
	for (std::set<RActorNode*>::iterator itr = m_setExternAnchorNodes.begin(); itr != m_setExternAnchorNodes.end(); ++itr)
	{
		RActorNode* pExternAnchor = *itr;
		RMesh* pMesh = pExternAnchor->m_pMeshNode->m_pBaseMesh;

		RActor* pParentActor = pExternAnchor->m_pActor;
		pParentActor->DeleteActorNode(pExternAnchor->GetNodeName().c_str());

		REngine::GetSceneManager().ReleaseResource(pMesh);
	}
}

void RPhysXClothController::RemoveNxActorUpdateInfo( NxActor* pActor )
{
	for (NX_UPDATE_ARRAY::iterator itr_update = m_vecNxUpdateInfo.begin(); itr_update != m_vecNxUpdateInfo.end(); ++itr_update)
	{
		if (itr_update->m_pNxActor == pActor)
		{
			m_vecNxUpdateInfo.erase(itr_update);
			break;
		}
	}
}

void RPhysXClothController::ClearNxCloth( SNxCloth& _rCloth )
{
	if(_rCloth.m_pCloth == NULL)
		return;

	m_pScene->releaseCloth(*_rCloth.m_pCloth);
	_rCloth.m_pCloth = NULL;

	_ASSERT(_rCloth.m_pClothMesh != NULL);
	_ASSERT(0 == _rCloth.m_pClothMesh->getReferenceCount());
	m_pSDK->releaseClothMesh(*_rCloth.m_pClothMesh);
	_rCloth.m_pClothMesh = NULL;

	NxVec3* vp;
	NxU32* up;

	// �ܺο����� ����ϴ� ����Ÿ. ��ó���� ���ش�.
	vp = (NxVec3*)_rCloth.m_MeshData.verticesPosBegin;
	free(vp);
	_rCloth.m_MeshData.verticesPosBegin = NULL;

	vp = (NxVec3*)_rCloth.m_MeshData.verticesNormalBegin;
	free(vp);
	_rCloth.m_MeshData.verticesNormalBegin = NULL;

	// ���ο��� ����ϴ� ����Ÿ. ��ó���� �����ش�
	up = (NxU32*)_rCloth.m_MeshData.numVerticesPtr;
	free(up);
	_rCloth.m_MeshData.numVerticesPtr = NULL;

	up = (NxU32*)_rCloth.m_MeshData.indicesBegin;
	free(up);
	_rCloth.m_MeshData.indicesBegin = NULL;

	up = (NxU32*)_rCloth.m_MeshData.numIndicesPtr;
	free(up);
	_rCloth.m_MeshData.numIndicesPtr = NULL;

	up = (NxU32*)_rCloth.m_MeshData.parentIndicesBegin;
	free(up);
	_rCloth.m_MeshData.parentIndicesBegin = NULL;

	up = (NxU32*)_rCloth.m_MeshData.numParentIndicesPtr;
	free(up);
	_rCloth.m_MeshData.numParentIndicesPtr = NULL;
}

void RPhysXClothController::UpdateExternAnchorActorNodesCreation()
{
	if(m_mapExternAnchorActorNodeCandidates.empty() == false)
	{
		for (ANCHOR_CANDIDATE_MAP::iterator itr = m_mapExternAnchorActorNodeCandidates.begin(); itr != m_mapExternAnchorActorNodeCandidates.end(); ++itr)
		{
			RMeshNode* pMeshNode = itr->second.first;
			RActorNode* pParentActorNode = itr->second.second;

			_ASSERT(m_pActor->GetActorNode(pMeshNode->GetName()) == NULL);

			pMeshNode->m_pBaseMesh->AddRef();
			RActorNode* pAnchorNode = m_pActor->AddMeshNode(pMeshNode, pParentActorNode);
			pAnchorNode->SetVisible(false);
			m_setExternAnchorNodes.insert(pAnchorNode);
		}
		m_mapExternAnchorActorNodeCandidates.clear();
	}
}

void RPhysXClothController::UpdateClothCreationCandidates()
{
	if (m_mapClothCandidates.empty() == false)
	{
		for (CLOTH_CANDIDATE_MAP::iterator itr = m_mapClothCandidates.begin(); itr != m_mapClothCandidates.end();)
		{
			++itr->second;
			bool bNextIter = true;

			CLOTH_MAP::iterator itr_cloth = m_mapClothNodesAdded.find(itr->first);
			SNxCloth& rClothInfo = itr_cloth->second;

			// �߸��� Ŭ�ξ�. ��Ŀ�� ���°��� ���� ���� �ʿ䰡 ����.
			if (rClothInfo.m_vecAnchorNodes.empty())
			{
				m_mapClothCandidates.erase(itr++);
				bNextIter = false;
			}

			if (itr->second == 5)
			{
				RPhysXClothActorNode* pClothActorNode = dynamic_cast<RPhysXClothActorNode*>( m_pActor->GetActorNode(itr->first.c_str()) );
				_ASSERT(pClothActorNode!= NULL);

				if (rClothInfo.m_pCloth != NULL)
					ClearNxCloth(rClothInfo);

				// Ŭ�ξ� ����
				pClothActorNode->ChangeClothState(RPhysXClothActorNode::E_CPU);
				CreateCloth(pClothActorNode, rClothInfo);

				// ��Ŀ�� ���̱�
				int nAnchorCount = (int)rClothInfo.m_vecAnchorNodes.size();
				for (int i = 0; i < nAnchorCount; ++i)
				{
					NXACTOR_MAP::iterator itr_anchor = m_mapAnchorNxActors.find(rClothInfo.m_vecAnchorNodes[i]);
					rClothInfo.m_pCloth->attachToShape(*(itr_anchor->second->getShapes()), 0);
				}
			}
			else if (itr->second > 10)
			{
				_ASSERT(rClothInfo.m_pCloth != NULL);
				RPhysXClothActorNode* pClothActorNode = dynamic_cast<RPhysXClothActorNode*>(m_pActor->GetActorNode(itr->first.c_str()));
				_ASSERT(pClothActorNode != NULL);
				pClothActorNode->ConnectClothMeshData(rClothInfo.m_MeshData);
				pClothActorNode->ChangeClothState(RPhysXClothActorNode::E_PHYSX);
				m_mapClothCandidates.erase(itr++);
				bNextIter = false;
			}

			if (bNextIter)
				++itr;
		}
	}
}

void RPhysXClothController::FillClothDesc( NxClothDesc& _rDesc, SNxCloth& _rCloth, const RActorNode* _pClothActorNode )
{
	//////////////////////////////////////////////////////////////////////////
	//
	// Cloth Desc
	//
	//////////////////////////////////////////////////////////////////////////
	_rDesc.collisionGroup		= m_nCollsionGroup;

	_rDesc.globalPose.t			= NxVec3(0, 0, 0);
	//_rDesc.thickness			= 1.0f;
	//_rDesc.thickness				= 0.55f;
	//_rDesc.thickness				= 2.5f;
	_rDesc.thickness				= 1.2f;
	//_rDesc.density				= 1.0f;
	_rDesc.density				= 0.5f;
	//_rDesc.bendingStiffness	= 1.0f;
	//_rDesc.bendingStiffness		= 0.3f;
	//_rDesc.bendingStiffness		= 0.2f;
	_rDesc.bendingStiffness		= 0.5f;

	//_rDesc.stretchingStiffness	= 1.0f;
	//_rDesc.stretchingStiffness	= 0.7f;
	_rDesc.stretchingStiffness	= 0.6f;

	//_rDesc.dampingCoefficient	= 0.9f;			// ����
	//_rDesc.dampingCoefficient	= 0.5f;			// ����
	//_rDesc.friction				= 0.0f;			// ����
	_rDesc.dampingCoefficient	= 0.2f;			// ����
	//_rDesc.friction				= 0.0f;			// ����
	_rDesc.friction				= 1.0f;			// ����

	_rDesc.solverIterations	= 7;			// itr Ƚ��, �⺻�� 5
	//_rDesc.solverIterations	= 10;			// itr Ƚ��, �⺻�� 5

	// strength of the feedback force on rigid bodies.
	//_rDesc.collisionResponseCoefficient	= 0.2f;
	//_rDesc.attachmentResponseCoefficient	= 1.0f;

	// �з�
	//_rDesc.pressure			= 1.0f;

	//_rDesc.flags |= NX_CLF_GRAVITY; // �⺻��
	//_rDesc.flags &= ~NX_CLF_GRAVITY; // �߷� ���ֱ�
	_rDesc.flags |= NX_CLF_BENDING;
	//_rDesc.flags |= NX_CLF_BENDING_ORTHO;
	_rDesc.flags |= NX_CLF_SELFCOLLISION;		// Enable/disable self-collision handling within a single piece of cloth. 

	_rDesc.flags |= NX_CLF_DAMPING;
	_rDesc.flags |= NX_CLF_COMDAMPING;

	_rDesc.flags |= NX_CLF_ADHERE;	// ���� ��ü�̵�. ������Ű��
	//_rDesc.flags |= NX_CLF_STATIC;
	//_rDesc.minAdhereVelocity = 10.f;
	//_rDesc.minAdhereVelocity = 20.f;
	//_rDesc.minAdhereVelocity = 0.f;
	//_rDesc.minAdhereVelocity = 100.f; //Ŀ���� AdHere ȿ���� ��������. ���� Ŀ���� AdHere �÷��� ���°Ͱ� ��������

	//_rDesc.flags |= NX_CLF_HARDWARE;				// simulated on the PPU.
	//_rDesc.flags |= NX_CLF_TRIANGLE_COLLISION;	// Not supported in current release.
	//_rDesc.flags |= NX_CLF_SELFCOLLISION;			// Enable/disable self-collision handling within a single piece of cloth. 
	//_rDesc.flags |= NX_CLF_COLLISION_TWOWAY;		// ��Ŀ���Ϳ� Ŭ�ξ��� ��ȣ�ۿ��� �ϴ��� ����
	//_rDesc.flags |= NX_CLF_PRESSURE;
}

}