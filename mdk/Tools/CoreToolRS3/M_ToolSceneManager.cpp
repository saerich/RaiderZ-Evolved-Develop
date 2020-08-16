#include "StdAfx.h"
#include "RUtil.h"
#include <assert.h>
#include <float.h>
#include <fstream>

#include "RResourceID.h"
#include "RTextureManager.h"

#include "RSceneNodeID.h"
#include "RCameraSceneNode.h"
#include "RCameraFPSSceneNode.h"
#include "RCameraArcBallSceneNode.h"
#include "RLightSceneNode.h"
#include "RSpotLightSceneNode.h"
#include "RCameraSceneNode.h"
#include "RSceneSky.h"
#include "MDebug.h"
#include "RGrassSystem.h"
#include "MRTTI.h"

#include "RRenderHelper.h"

#include "C_ControlSceneProperty.h"
#include "C_ControlMediator.h"
#include "C_ControlMediatorHelperAction.h"
#include "C_ControlMediatorHelperResource.h"

#include "M_ToolLogicObjectManager.h"

#include "M_ToolSceneManager.h"
#include "M_ToolActor.h"
#include "M_ToolSceneNodeInstance.h"
#include "M_ToolStaticMeshSceneNode.h"
#include "M_ToolTerrain.h"
#include "M_ToolPointLight.h"
#include "M_LogicObject_SceneNodes.h"
#include "M_Visitors.h"

#include "RTreeSceneNode.h"

#include "C_XMLConfig.h"
#include "C_ControlCursor.h"
#include "FileInfo.h"
#include "MFileSystem.h"

// Tree PassRenderer Lazy Init
#include "C_XMLXConst.h"
#include "C_XMLEditInfoFile.h"

#include "MainFrm.h"
#include "VR_EtcCollision.h"

namespace rs3 {

char* RToolSceneManager::SSceneNodeFinder::pSzSceneNodeName = NULL;

RToolSceneManager::RToolSceneManager(CControlMediator* pControlMediator)
: /*m_pPickingIgnoreSceneNode(NULL), */m_bWireFrameDrawMode(false), m_pSceneNodeModifier(NULL)
, m_pControlMediator(pControlMediator)
{
	// ��Ƽ ��Ʈ ���� ��� ���� �ʾ� ����.
	GetRootSceneNode().SetUserData( (void*) this );

	// TODO: ���� ���Ӽ� ����� �ϴ°͵�
	CControlSceneProperty::m_pControlMediator = pControlMediator;
}

RToolSceneManager::~RToolSceneManager()
{
	SAFE_DELETE(m_pSceneNodeModifier);

	// �ϴ� �θ���� �����
	RSceneManager::Clear();
}

bool RToolSceneManager::LoadEditableSceneXML(const char *szFileName)
{
	// ��Ƽ ��Ʈ �������� ��Ʈ �� ��ü�� ���� �Ǿ��ٰ� �����ǰ� �Ѵ�.
	// �� ��Ʈ ���� ����� �� �ø��� SetProperty()�ٽ� �� �־�� �ϴµ�, 
	// GetRootSceneNode()���� �� ��Ʈ ���� ���� �ϹǷ� �̸� ����
	GetRootSceneNode().SetUserData( (void*) this );

	RWorldSceneNode* pWorldSceneNode = &GetRootSceneNode();

	return LoadEditableSceneXML( pWorldSceneNode, szFileName);
}

void RToolSceneManager::AttachLogicObjectTraverse( RSceneNode* pSceneNodeTree )
{
	CEditableNodeCollector editableNodeCollector;

	pSceneNodeTree->Traverse(&editableNodeCollector);
	for(std::vector<RSceneNode*>::iterator iter = editableNodeCollector.m_SceneNodes.begin();iter != editableNodeCollector.m_SceneNodes.end(); iter++)
	{
		RSceneNode* pSceneNode = (*iter);
		AttachLogicObject( pSceneNode);
	}
}


bool RToolSceneManager::LoadEditableSceneXML(RSceneNode *pSceneNodeToLoad, const char* szFileName)
{
	bool bRet = LoadWorldorInstanceFromRootXMLFile(pSceneNodeToLoad, szFileName, false);

	// scene ������ �ƴѰ�쿡�� ����Ʈ �ϰ� �����Ҽ� �ֵ��� ����
	AttachLogicObjectTraverse( pSceneNodeToLoad );
	
	if (bRet)
		UpdateRootSceneTransform();

	return bRet;
}

void RToolSceneManager::UpdateRootSceneTransform()
{
	// �ε��� ������Ʈ
	GetRootSceneNode().NeedUpdateTransform();
	RSceneNodeUpdateVisitor updateVisitor;
	GetRootSceneNode().Traverse(&updateVisitor);
}

CLogicObject_SceneNode* RToolSceneManager::CreateLogicObject( RSceneNode* pSceneNode )
{
	CLogicObject_SceneNode* pLogicObject = NULL;

	// SceneInstance ���� LoadXML�Լ��� ���� XML ���� �ѹ� �Ҹ���
	// ��� XML �� ROOT ��带 ������ �ѹ� �� �ҷ��� �ߺ��ȴ� �̰�쵵 �ι� ���� �ʵ����Ѵ�
	if( stricmp(pSceneNode->GetNodeIDString(),RSID_SCENEINSTANCE)==0 && pSceneNode->GetUserData()!=NULL ) // �̹� �پ��ִ�
		return NULL;

	if(stricmp(pSceneNode->GetNodeIDString(),RSID_LIGHTSCENENODE)==0 )
		pLogicObject = new CLogicObject_PointLight;
	else if(stricmp(pSceneNode->GetNodeIDString(),RSID_SPOTLIGHTSCENENODE)==0 )
		pLogicObject = new CLogicObject_SpotLight;
	else if(stricmp(pSceneNode->GetNodeIDString(),RSID_DIRECTIONALLIGHTSCENENODE)==0 )
		pLogicObject = new CLogicObject_DirectionalLight;
	else if(stricmp(pSceneNode->GetNodeIDString(),RSID_SUBLIGHTSCENENODE)==0 )
		pLogicObject = new CLogicObject_SubLight;
	else if(stricmp(pSceneNode->GetNodeIDString(),RSID_TREE)==0 )
		pLogicObject = new CLogicObject_Tree;
	else if(stricmp(pSceneNode->GetNodeIDString(),RSID_SCENEINSTANCE)==0 )
		pLogicObject = new CLogicObject_SceneInstance;
	else if(stricmp(pSceneNode->GetNodeIDString(),RSID_STATICMESHSCENENODE)==0 )
		pLogicObject = new CLogicObject_StaticMesh;
	else if(stricmp(pSceneNode->GetNodeIDString(),RSID_TERRAINSCENENODE)==0 )
		pLogicObject = new CLogicObject_Terrain;
	else if(stricmp(pSceneNode->GetNodeIDString(),RSID_ACTOR)==0 )
		pLogicObject = new CLogicObject_Actor;
	else if(stricmp(pSceneNode->GetNodeIDString(),RSID_WATERPLANE)==0 )
		pLogicObject = new CLogicObject_WaterPlane;
	else if(stricmp(pSceneNode->GetNodeIDString(),RSID_EFFECTINSTANCE)==0 )
		pLogicObject = new CLogicObject_EffectInstance;
	else if(stricmp(pSceneNode->GetNodeIDString(),RSID_OCCLUDER)==0 )
		pLogicObject = new CLogicObject_Occluder;

	if(pLogicObject)
	{
		pLogicObject->LinkModelSceneNode( pSceneNode );
		pLogicObject->SetController( m_pControlMediator );
	}

	return pLogicObject;
}

bool RToolSceneManager::DestroyLogicObject( RSceneNode* pSceneNode )
{
	CLogicObject* pLogicObject = GetLogicObjectLinkedTo(pSceneNode);
	if( !pLogicObject )
		return false;

	delete pLogicObject;
	pSceneNode->SetUserData( NULL );

	return true;
}

bool RToolSceneManager::AttachLogicObject( RSceneNode* pSceneNode )
{
	CLogicObject_SceneNode* pLogicObject = pLogicObject = CreateLogicObject( pSceneNode );
	if( pLogicObject == NULL ) return false;

	m_pControlMediator->GetToolLogicObjectMgr()->AddToManager(pLogicObject);

	return true;
}

bool RToolSceneManager::DetachLogicObject( RSceneNode* pSceneNode )
{
	CLogicObject* pLogicObject = GetLogicObjectLinkedTo(pSceneNode);
	if( !pLogicObject )
		return false;

	m_pControlMediator->GetToolLogicObjectMgr()->RemoveFromManager(pLogicObject);

	DestroyLogicObject(pSceneNode);

	return true;
}

void RToolSceneManager::Clear()
{
	RSceneManager::Clear();
}

void RToolSceneManager::Update()
{
	RSceneManager::Update();

	if(m_pSceneNodeModifier)
	{
		CControlMediator* pMediator = ((CMainFrame*)AfxGetMainWnd())->GetControlMediator();
		CRollupEtcCollision* pCollision = pMediator->GetViewPtr<CRollupEtcCollision>();
		if( pCollision )
			m_pSceneNodeModifier->Update( pCollision->GetCollisionTree().GetCollisionTree() );
	}
}

CLogicObject* RToolSceneManager::GetLogicObjectLinkedTo( const RSceneNode* pSceneNode )
{
	CLogicObject* pLogicObject = dynamic_cast<CLogicObject*>((CCoreObject*)pSceneNode->GetUserData());
	return pLogicObject;
}

// �巡�� ����
int RToolSceneManager::AABBPickingFrustum(RSceneNodeList *pOutList, const RSceneNodeList *pChildNodeList, RViewFrustum &kFrustum)
{
	if ( pChildNodeList->size() == 0 )
		return 0;

	int nNum = 0;
	kFrustum.MakePoints();

	for ( RSceneNodeList::const_iterator it = pChildNodeList->begin(); it != pChildNodeList->end(); ++it )
	{
		RSceneNode *pNode = *it;
		assert(pNode!=NULL);

// 		if ( pNode->GetProperty() == NULL )
// 			continue;

		// �������� ��ŷ���� ���þ��� ���
		const char* pSzNodeId = pNode->GetNodeIDString();
		if (_stricmp(pSzNodeId, RSID_CAMERASCENENODE) == 0 ||
			_stricmp(pSzNodeId, RSID_TERRAINSCENENODE) == 0 ||
			_stricmp(pSzNodeId, RSID_DIRECTIONALLIGHTSCENENODE ) == 0 ||
			_stricmp(pSzNodeId, RSID_SUBLIGHTSCENENODE ) == 0 ||
			_stricmp(pSzNodeId, RSID_WATERPLANE ) == 0 
// 			// logicobject �� �����ϴ� ���, ��� �ų�忡 ���� ó���ϰ��� �ĳ���
// 			|| GetLogicObjectLinkedTo(pNode)!=NULL 
			)
			continue;

		// static �޽��� �Ӽ��� �˻��ؼ� ��ŷ����� �Ǵ��� ����
		if( _stricmp(pNode->GetNodeIDString(), RSID_STATICMESHSCENENODE) == 0 )
		{
			RStaticMeshSceneNode* pStaticMesh = (RStaticMeshSceneNode*)pNode;
			if( pStaticMesh->getEnablePicking() == false )
				continue;
		}

		// ����� �ν��Ͻ��� �ڽ��� ��ŷ ����
		if ( pNode->GetChildren().size() && _stricmp(pNode->GetNodeIDString(), RSID_SCENEINSTANCE) != 0 )
			nNum += AABBPickingFrustum(pOutList, &pNode->GetChildren(), kFrustum);

		if ( pNode->GetVisible() == false )
			continue;

// 		// ��ŷ ������ ����� - �ε����� �ٴڵ� - �� �����ϰ�
// 		if( pNode == m_pPickingIgnoreSceneNode )
// 			continue;

		if ( _stricmp(pNode->GetNodeIDString(), RSID_LIGHTSCENENODE) == 0 )
		{
			if(kFrustum.TestBox( pNode->GetAABB() ) )
			{
				++nNum;
				pOutList->push_back(pNode);
			}
		}
		// ���ʹ� ���þ���
		else if ( _stricmp(pNode->GetNodeIDString(), RSID_ACTOR) == 0 )
		{
		}
		else if ( MIsExactlyClass( RTreeSceneNode, pNode) )
		{
			RTreeSceneNode* pTree = (RTreeSceneNode*)pNode;

			if (pTree->SelectFrustumTest(kFrustum))
			{
				++nNum;
				pOutList->push_back(pNode);
			}
		}
		else if ( pNode->GetAABB().isInViewFrustumWithZ( kFrustum.GetPlanes() ) )
		{
			++nNum;
			pOutList->push_back(pNode);
		}
	}

	return nNum;
}

// Ŭ�� ����
RSceneNode* RToolSceneManager::AABBPicking(const RSceneNodeList *pNodeList, const RVector &origin, const RVector &to, RPICKINFO_MAP& _refPickInfoOut, bool _bSelectMode)
{
	if ( pNodeList->size() == 0 )
		return NULL;

	RSceneNode* pCurrentMouseNode = L3DCursor::GetInstance()->GetNode();
	RSceneNode *pPickedNode = NULL;

	for ( RSceneNodeList::const_iterator it = pNodeList->begin(); it != pNodeList->end(); ++it )
	{
		RSceneNode *pNode = *it;
		assert(pNode!=NULL);

		// ������Ƽ�� ���ų� ������ �ʴ� ��ü�� ���� �ȵǰ�
// 		if ( pNode->GetProperty() == NULL )
// 			continue;

		// ���콺 ���� �پ� �ִ� ���� ���� �ȵǰ�
		if ( pNode == pCurrentMouseNode )
			continue;

		// ������ ������
		if (_stricmp(pNode->GetNodeIDString(), RSID_CAMERASCENENODE) == 0 ||
			_stricmp(pNode->GetNodeIDString(), RSID_TERRAINSCENENODE) == 0 
// 			// TODO: logicobject �� �����ϴ� ���, ��� �ų�忡 ���� ó���ϰ��� �ĳ���
// 			|| GetLogicObjectLinkedTo(pNode)!=NULL 
			)
			continue;

		// static �޽��� �Ӽ��� �˻��ؼ� ��ŷ����� �Ǵ��� ����
		if( _stricmp(pNode->GetNodeIDString(), RSID_STATICMESHSCENENODE) == 0 )
		{
			RStaticMeshSceneNode* pStaticMesh = (RStaticMeshSceneNode*)pNode;
			if( pStaticMesh->getEnablePicking() == false && _bSelectMode)
				continue;
		}

		// �ڽ� ����� ��ȸ
		if( pNode->GetChildren().size() && _stricmp(pNode->GetNodeIDString(), RSID_SCENEINSTANCE) != 0 )
		{
			pNode = AABBPicking(&pNode->GetChildren(), origin, to, _refPickInfoOut, _bSelectMode);
			if ( pNode == NULL )
				pNode = *it;
		}

		if ( pNode->GetVisible() == false )
			continue;

		// �ε��� ����� ���ÿ���
		// ��ü�� �����Ҷ��� �ε��� ����� ��ü(scene.xml�� �ִ¹�ü)�� ������ �ȵǾ�� �Ѵ�
// 		if( pNode == m_pPickingIgnoreSceneNode && _bSelectMode == true )
// 			continue;

		if ( pNode->GetAABB().ContainsLineSegment(origin, to))
		{
			RPICKINFO kInfo(origin, to, RPM_DETAIL); //�����̵� �ƴϵ� ��ŷ�� �������ϰ� ����
			if ( pNode->Pick(kInfo))
			{
				float d = kInfo.outPosition.DistanceToSq(origin);
				_refPickInfoOut.insert( RPICKINFO_MAP::value_type( d, kInfo ) );
				pPickedNode = pNode;
			}
		}
	}
	return pPickedNode;
}

bool RToolSceneManager::Pick(const RVector &origin,const RVector &to, RPICKINFO_MAP& _refPickInfoOut, bool _bSelectMode)
{
	_ASSERT( _refPickInfoOut.empty() );
	RSceneNode *pPickedNode = AABBPicking( &GetRootSceneNode().GetChildren(), origin, to, _refPickInfoOut, _bSelectMode);
	return pPickedNode != NULL;
}

bool RToolSceneManager::PickFrustum(RSceneNodeList *pOutList, RViewFrustum &kFrustum)
{
	return ( AABBPickingFrustum(pOutList,&GetRootSceneNode().GetChildren(), kFrustum) > 0 ? true : false);
}


const RVector* RToolSceneManager::GetVerticalContactPos( const RVector& _refPos, const RSceneNode* _pSceneNodeToIgnore )
{
	static RPICKINFO_MAP mapPickInfo;
	static RVector vTerrainPos;

	RVector* pVecPos(NULL);
	mapPickInfo.clear();

	RVector vecVertical( _refPos + RVector( 0, 0, -100000));

	if( GetCurrentTerrain()  && GetCurrentTerrain()->GetPickVertical( vTerrainPos, NULL, _refPos.x, _refPos.y ) )
		pVecPos = &vTerrainPos;

	if (Pick( _refPos, vecVertical, mapPickInfo, false ) )
	{
		RPICKINFO_MAP::iterator itr = mapPickInfo.begin();
		if( itr->second.outNode == _pSceneNodeToIgnore )
			itr++;

		if( itr != mapPickInfo.end() )
		{
			if( pVecPos )
			{
				if( pVecPos->z < itr->second.outPosition.z )
					pVecPos = &(itr->second.outPosition);
			}
			else
				pVecPos = &(itr->second.outPosition);
		}
	}
	return const_cast<const RVector*>(pVecPos);
}

// �ٷ� ���� �Լ��� �ߺ��� ����: charmke
const RVector* RToolSceneManager::GetVerticalContactPos( const RVector& _refPos, const RSceneNode* _pSceneNodeToIgnore, RVector* const _outNormal )
{
	static RPICKINFO_MAP mapPickInfo;

	// �ߺ��� ���� �� ���� ����.
	// �� ������ static���� �������� ������ �ھ����� '������ ��ü ������ ���̱�' ����� ����� �������� �ʴ´�.
	// �ٷ� ���� GetVerticalContactPos()�Լ��� ������ static ���� ������ �����Ƿ� �ϴ� �ߺ��� ä�� ���д�. : charmke
	static RVector vTerrainPos;

	RVector* pVecPos(NULL);
	mapPickInfo.clear();

	RVector vecVertical( _refPos + RVector( 0, 0, -100000));

	if( GetCurrentTerrain()  && GetCurrentTerrain()->GetPickVertical( vTerrainPos, _outNormal, _refPos.x, _refPos.y ) )
		pVecPos = &vTerrainPos;

	if (Pick( _refPos, vecVertical, mapPickInfo, false ) )
	{
		RPICKINFO_MAP::iterator itr = mapPickInfo.begin();
		if( itr->second.outNode == _pSceneNodeToIgnore )
			itr++;

		if( itr != mapPickInfo.end() )
		{
			if( pVecPos )
			{
				if( pVecPos->z < itr->second.outPosition.z )
					pVecPos = &(itr->second.outPosition);
			}
			else
				pVecPos = &(itr->second.outPosition);
		}
	}
	return const_cast<const RVector*>(pVecPos);
}

bool RToolSceneManager::SaveRootSceneToXML(const char *szFileName )
{
	return RSceneManager::SaveRootSceneToXML(szFileName);
}

void RToolSceneManager::SaveRootSceneToXML(const char *szFileName, MXml& _refMXml)
{
	MXmlDeclaration* pDec = _refMXml.CreateDeclaration("1.0", "UTF-8", "");
	_refMXml.Doc()->LinkEndChild(pDec);
	
	GetRootSceneNode().SetXMLFileName( szFileName );
	SaveXML(&GetRootSceneNode(),_refMXml.Doc());
}

RSceneNode* RToolSceneManager::CreateSceneNode(const char *szNodeName)
{
	if ( szNodeName == NULL )
		return NULL;

	RSceneNode *pNewNode = NULL;

	if(stricmp(szNodeName,RSID_SCENEINSTANCE)==0)
	{
		pNewNode = new RToolSceneNodeInstance;
// 		pNewNode->SetProperty(CSceneNodeInstanceProperty::GetInstance());
	}
	else if(stricmp(szNodeName,RSID_STATICMESHSCENENODE)==0)
	{
		pNewNode = new RToolStaticMeshSceneNode;
// 		pNewNode->SetProperty(CStaticMeshSceneNodeProperty::GetInstance());
	}
	else if(stricmp(szNodeName,RSID_TERRAINSCENENODE)==0)
	{
		RToolTerrain *pTerrain = new RToolTerrain(REngine::GetDevicePtr());
// 		pTerrain->SetProperty(CTerrainSceneNodeProperty::GetInstance());

		pNewNode = (RSceneNode*)pTerrain;
	}
	else if(stricmp(szNodeName,RSID_ACTOR)==0 )
	{
		pNewNode = new RToolActor(m_pControlMediator);
// 		pNewNode->SetProperty(CActorSceneNodeProperty::GetInstance());
	}
	else if(stricmp(szNodeName,RSID_TREE)==0 )
	{
		pNewNode = new RTreeSceneNode;
// 		pNewNode->SetProperty(CTreeSceneNodeProperty::GetInstance());
	}
	else if(stricmp(szNodeName,RSID_LIGHTSCENENODE)==0 )
	{
		pNewNode = new RToolPointLightSceneNode(m_pControlMediator);
// 		pNewNode->SetProperty(CPointLightSceneNodeProperty::GetInstance());
	}
	else if(stricmp(szNodeName,RSID_DIRECTIONALLIGHTSCENENODE)==0 ||
			stricmp(szNodeName,RSID_SUBLIGHTSCENENODE)==0)
	{
		pNewNode = RSceneManager::CreateSceneNode(szNodeName);
// 		pNewNode->SetProperty(CDirectionalLightSceneNodeProperty::GetInstance());
	}
	else if(stricmp(szNodeName,RSID_WATERPLANE)==0 )
	{
		pNewNode = RSceneManager::CreateSceneNode(szNodeName);
// 		pNewNode->SetProperty(CWaterPlaneSceneNodeProperty::GetInstance());
	}
	else if(stricmp(szNodeName,RSID_SPOTLIGHTSCENENODE)==0 )
	{
		pNewNode = RSceneManager::CreateSceneNode(szNodeName);
// 		pNewNode->SetProperty(CSpotLightSceneNodeProperty::GetInstance());
	}
	else if(stricmp(szNodeName,RSID_EFFECTINSTANCE)==0 )
	{
		// �⺻ ������Ƽ �Ⱦ�����.. TODO: CControlSceneProperty �迭�� �� �ĳ��� ������
		pNewNode = RSceneManager::CreateSceneNode(szNodeName);
	}
	else if(stricmp(szNodeName,RSID_OCCLUDER)==0 )
	{
		// �⺻ ������Ƽ �Ⱦ�����.. TODO: CControlSceneProperty �迭�� �� �ĳ��� ������
		pNewNode = RSceneManager::CreateSceneNode(szNodeName);
	}
	else
	{
		pNewNode = RSceneManager::CreateSceneNode(szNodeName);
		if(NULL==pNewNode)
		{
			_ASSERT(0);
			mlog("RSceneManager::CreateSceneNode unknown node %s\n" ,szNodeName);
			return NULL;
		}
		pNewNode->SetUserData(CControlSceneProperty::GetInstance());	// �⺻ ������Ƽ
	}

	//if(pNewNode)
	//	//pNewNode->m_bCreatedByManager = true;
	//	m_rootSceneNode.AddToSceneNodeGroup(pNewNode);

	return pNewNode;
}

RSceneNode* RToolSceneManager::GetSceneNodeByName(const char* _pSzSceneNodeName)
{
	SSceneNodeFinder::pSzSceneNodeName = const_cast<char*>(_pSzSceneNodeName);
	vector< RSceneNode* > vCulledSceneNode;
	GetRootSceneNode().SerializeToVector( SSceneNodeFinder::FuncGetByName, vCulledSceneNode );
	_ASSERT( vCulledSceneNode.size() <= 1);
	if (vCulledSceneNode.empty())
	{
		return NULL;
	}
	else
		return vCulledSceneNode[0];
}

void RToolSceneManager::RenderPrimaryCamera()
{
	if( m_bWireFrameDrawMode )
	{
		REngine::GetDevice().SetFillMode(RFILL_WIRE);
		RSceneManager::RenderPrimaryCamera();
		REngine::GetDevice().SetFillMode(RFILL_SOLID);
	}
	else
	{
		RSceneManager::RenderPrimaryCamera();
	}
}

void RToolSceneManager::GatherAllActor(const RSceneNode *pSceneNode, RSceneNodeList* pOutList)
{
	if ( MIsExactlyClass( RActor, pSceneNode) )
		pOutList->push_back( (RSceneNode*)pSceneNode );	// const ���̷��� �ʹ� ���� Ŀ���� ĳ�����Ͽ���

	for ( RSceneNodeList::const_iterator it = pSceneNode->GetChildren().begin(); it != pSceneNode->GetChildren().end(); ++it )
	{
		RSceneNode *pChildNode = *it;
		GatherAllActor(pChildNode, pOutList);
	}
}

void RToolSceneManager::GatherAllTrees(const RSceneNode *pSceneNode, RSceneNodeList* pOutList)
{
	if ( MIsExactlyClass( RTreeSceneNode, pSceneNode) )
		pOutList->push_back( (RSceneNode*)pSceneNode );	// const ���̷��� �ʹ� ���� Ŀ���� ĳ�����Ͽ���

	for ( RSceneNodeList::const_iterator it = pSceneNode->GetChildren().begin(); it != pSceneNode->GetChildren().end(); ++it )
	{
		RSceneNode *pChildNode = *it;
		GatherAllTrees(pChildNode, pOutList);
	}
}

void RToolSceneManager::ExportTerrainMaxScript(FILE* file)
{
	RToolTerrain* pTerrain = (RToolTerrain*)GetCurrentTerrain();
	if( NULL == pTerrain )
		return;

	string strBakedMapName = string(pTerrain->GetPath()) + pTerrain->GetBaseMapName();
	string strBakedMapNameMax = MGetPathReplacedBackSlashToSlash(strBakedMapName);

	string strDisMapName = string(pTerrain->GetPath()) + pTerrain->GetDisplacementMapName();
	string strDisMapNameMax = MGetPathReplacedBackSlashToSlash(strDisMapName);

	fprintf(file,"-- ���� ����Ʈ -----------------------------------------------------------\n" );
	fprintf(file,"ImportTerrain \"%s\" \"%s\" %.3f %.3f\n\n",
		strDisMapNameMax.c_str(), strBakedMapNameMax.c_str(), pTerrain->GetWidthSize(), pTerrain->GetHeightSize() );
}

void RToolSceneManager::ExportLightMapActorMaxScript(FILE* file)
{
	// ��� ����Ʈ�� ���͸� ������
	RSceneNodeList outLightMapActorList;
	GatherAllActor(&GetRootSceneNode(), &outLightMapActorList);

	// ���͸� �޽� ������ �����Ͽ� ������
	typedef map< string , RSceneNodeList > MAXFILETOINSTANCEMAP;
	MAXFILETOINSTANCEMAP mapInstances; // max filename -> list of instance

	for ( RSceneNodeList::const_iterator it = outLightMapActorList.begin(); it != outLightMapActorList.end(); ++it )
	{
		RSceneNode *pNode = *it;
		RActor* pActor = MStaticCast( RActor, pNode );

		// RefferenceMesh�� ������� �ʴ´�
		if ( stricmp(pActor->GetNodeName().c_str(), "RefferenceMesh") == 0 )
			continue;
		
		MAXFILETOINSTANCEMAP::iterator itrInstances;
		itrInstances = mapInstances.find( pActor->GetMeshName());
		if(itrInstances != mapInstances.end())
			itrInstances->second.push_back( pNode );
		else
		{
			RSceneNodeList listInstances;
			listInstances.push_back( pNode );
			
			mapInstances.insert( MAXFILETOINSTANCEMAP::value_type(pActor->GetMeshName(), listInstances) );
		}
	}

	fprintf(file,"-- Actor ����Ʈ ----------------------------------------------------------\n" );

	// �� �޽��� ���ؼ� �ν��Ͻ��� �����ؼ� ���̴� ��ũ��Ʈ ���
	for ( MAXFILETOINSTANCEMAP::iterator itMap = mapInstances.begin(); itMap!= mapInstances.end(); ++itMap)
	{
		char szMaxFileName[256];
		ReplaceExtension(szMaxFileName,itMap->first.c_str(),"max");
		fprintf(file, "maxfile = getFilesRecursive pathXRefSearch \"%s\"\n", szMaxFileName);
		fprintf(file, "bSuccess = XRefMAXFile maxfile\n");

		fprintf(file, "if(bSuccess) then (\n");
		
		RSceneNodeList& listNodes = itMap->second;

		for( RSceneNodeList::iterator itNode = listNodes.begin(); itNode != listNodes.end(); ++itNode)
		{
			// ������ ����̸� ������ ���� ���� �ƴϸ� �ν��Ͻ��� ����
			if(*itNode == listNodes.back() )
			{
				fprintf(file, "\tnewNodes = selection\n");
			}else
				fprintf(file, "\tmaxOps.CloneNodes selection cloneType:#instance newNodes:&newNodes\n");

			const RMatrix& matInstance = (*itNode)->GetWorldTransform();
			fprintf(file, "\ttm = (matrix3 ");
			for( int iRow = 0; iRow<4; iRow++)
			{
				fprintf(file, "[%3.3f,%3.3f,%3.3f] ", 
					matInstance._m[iRow][0],
					matInstance._m[iRow][1],
					matInstance._m[iRow][2] );
			}
			fprintf(file, "\t)\n\tfor t in newNodes do (\n");
			fprintf(file, "\t\tif(t.parent == undefined) then (t.transform = t.transform * tm) \n");
			fprintf(file, "\t)\n");
		}
		fprintf(file, ")\n");
	}
	fprintf(file, "\n");
}

void RToolSceneManager::ExportSpeedTreeMaxScript(FILE* file)
{
	// SpeedTree �����尡 DirLight�� Align�ǵ��� ���̸� ���� ������ ���� ���Ѵ�.
	RVector lightDir;
	RDirectionalLightSceneNode* pDirLight = NULL;
	if( NULL != GetCurrentRootSceneNode(false) )
	{
		pDirLight = GetCurrentRootSceneNode(false)->GetDirectionalLight();
	}

	if(pDirLight)
	{
		lightDir = -pDirLight->GetDirection();
	}
	else
	{
		// ����Ʈ�� ������ Z+�� ����Ʈ ��������
		lightDir = RVector::AXISZ;
	}

	RSceneNodeList outLightmapTreeList;
	GatherAllTrees(&GetRootSceneNode(), &outLightmapTreeList);

	fprintf(file,"-- SpeedTree ����Ʈ ------------------------------------------------------\n" );

	for(RSceneNodeList::iterator it = outLightmapTreeList.begin(); it != outLightmapTreeList.end(); ++it)
	{
		RTreeSceneNode* pTreeSceneNode = MStaticCast( RTreeSceneNode, (*it) );

		fprintf(file, "(\n");

			const RMatrix& matInstance = pTreeSceneNode->GetWorldTransform();
			fprintf(file, "\tlocal tm = matrix3 ");
			for( int iRow = 0; iRow<4; iRow++)
			{
				fprintf(file, "[%3.3f,%3.3f,%3.3f] ", 
					matInstance._m[iRow][0],
					matInstance._m[iRow][1],
					matInstance._m[iRow][2] );
			}
			fprintf(file, "\n");

			fprintf(file, "\timportSpeedTree \"%s\" \"%s\" tm\n", pTreeSceneNode->GetTreeFileName().c_str(), pTreeSceneNode->GetNodeName().c_str() );

		fprintf(file, ")\n");
	}

	fprintf(file, "\n");
}

void RToolSceneManager::ExportMaxScript(const char* szMaxScriptFileName)
{
	FILE* file = fopen( szMaxScriptFileName, "w+" );

	fprintf(file, "global strCurrentFilePath = getFilenamePath ( getSourceFileName() )\n" );
	fprintf(file, "fileIn ( strCurrentFilePath + \"../../../Dev/MAIETExporters/zone.ms\" )\n\n" );

	fprintf(file,"/*\n" );
	fprintf(file,"*/\n\n" );

	//////////////////////////////////////////////////////////////////////////
	// ���� �ͽ���Ʈ
	ExportTerrainMaxScript(file);

	//////////////////////////////////////////////////////////////////////////
	// Actor �ͽ���Ʈ
	ExportLightMapActorMaxScript(file);

	//////////////////////////////////////////////////////////////////////////
	// SpeedTree ����Ʈ�� �ͽ���Ʈ
	ExportSpeedTreeMaxScript(file);

	//////////////////////////////////////////////////////////////////////////
	// target- directional light ����
	RDirectionalLightSceneNode* pDirLight = GetDefaultLight();
	if(pDirLight)
	{
		fprintf(file,"-- Directional light ����Ʈ ----------------------------------------------\n" );

		fprintf(file, "c = TargetDirectionallight pos:[0,0,0] target:(targetObject pos:[%3.3f,%3.3f,%3.3f])\n",
			pDirLight->GetDirection().x, pDirLight->GetDirection().y, pDirLight->GetDirection().z );
		fprintf(file, "c.overShoot = true\n");
	}

	fprintf(file, "\n");
	fprintf(file,"-- ���� �޽��� ��� ; �ּ� ó�� ���� ������-------------------------------\n" );
	fprintf(file, "displayNotExistSpeedFiles()\n");

	fprintf(file, "\n\n");
	fclose(file);
}

void RToolSceneManager::SaveResourceListToXML( const char *szFileName )
{
	typedef std::map< std::string, std::pair< int,std::vector<std::string> > > RES_MAP;
	RES_MAP res_instance_map;

	// managed resource
	for(int i = 0; i < RRESOURCETYPE::RR_MAX; ++i)
	{
		RResourceManager* pManager = GetResourceMgrFromID<RRESOURCETYPE>(i);
		if (!pManager)
			continue;

		std::string strResType = RRESOURCETYPE::GetResoruceIDString(i);
		res_instance_map[strResType].first = i;

		const RRESOURCEHASHMAP& res_mgr_map = pManager->GetResourceMap();
		for (RRESOURCEHASHMAP::const_iterator itr = res_mgr_map.begin(); itr != res_mgr_map.end(); ++itr)
		{
			if (itr->first.find(":\\") != std::string::npos)
				continue;

			res_instance_map[strResType].second.push_back(itr->first);
		}
	}

	// unmanaged resource : texture
	std::string strTextureType("texture");
	res_instance_map[strTextureType].first = -1;
	const RTextureManager* pTextureManager = REngine::GetDevice().GetTextureManager();
	const RRESOURCEHASHMAP& res_tex_mgr_map = pTextureManager->GetResourceMap();
	for (RRESOURCEHASHMAP::const_iterator itr = res_tex_mgr_map.begin(); itr != res_tex_mgr_map.end(); ++itr)
	{
		if (itr->first.find(":\\") != std::string::npos)
			continue;

		res_instance_map[strTextureType].second.push_back(itr->first);
	}

	MXml xml;
	MXmlDeclaration* pDec = xml.CreateDeclaration("1.0", "UTF-8", "");
	xml.Doc()->LinkEndChild(pDec);

	MXmlElement* pRoot = new MXmlElement("ROOT");
	xml.Doc()->LinkEndChild(pRoot);

	for (RES_MAP::iterator itr = res_instance_map.begin(); itr != res_instance_map.end(); ++itr)
	{
		MXmlElement* pResElement = new MXmlElement("RESOURCE");

		_SetAttribute(pResElement, "type", itr->first.c_str());
		_SetAttribute(pResElement, "id", itr->second.first);

		int cnt = 0;
		for (std::vector<std::string>::iterator itr_res_name = itr->second.second.begin(); itr_res_name != itr->second.second.end(); ++itr_res_name)
		{
			MXmlElement* pListElement = new MXmlElement("li");
			string resFileName = MGetPureFileNameExt( *itr_res_name );
			_SetAttribute(pListElement, "name", resFileName.c_str());
			pResElement->LinkEndChild(pListElement);
			++cnt;
		}

		_SetAttribute(pResElement, "cnt", cnt);

		pRoot->LinkEndChild(pResElement);
	}

	xml.SaveFile(szFileName);
}

bool RToolSceneManager::MakeSceneNodeInstanceEditable(RSceneNodeInstance* pSceneInstance, RSceneNodeList* pOutList)
{
	CLogicObject* pLogicObjectSceneInstance = GetLogicObjectLinkedTo( pSceneInstance );
	string strLayerName = pLogicObjectSceneInstance->GetLayerName();

	DetachLogicObject( pSceneInstance );

	RSceneNodeList nodeList;
	RToolSceneNodeInstance::ConvertToAsCopy( pSceneInstance, &nodeList );

 	for(RSceneNodeList::iterator itr = nodeList.begin(); itr != nodeList.end(); ++itr )
 	{
 		// attach logic object
 		rs3::RSceneNode* pNode =  *itr;

		CEditableNodeCollector editableNodeCollector;
		pNode->Traverse(&editableNodeCollector);

		for(std::vector<RSceneNode*>::iterator iter = editableNodeCollector.m_SceneNodes.begin();iter != editableNodeCollector.m_SceneNodes.end(); iter++)
		{
			RSceneNode* pSceneNode = (*iter);
			CLogicObject* pLogicObject = CreateLogicObject( pSceneNode );
			if(pLogicObject)
				pLogicObject->SetLayerName( strLayerName.c_str() );

			m_pControlMediator->GetToolLogicObjectMgr()->AddToManager(pLogicObject);
		}
 	}

	// ���� �ٲ���ٴ°��� �뺸,
	m_pControlMediator->BroadCastMessage( VIEW_MESSAGE::SCENE_BREAK_INSTANCE );
	
	// �ڽĳ�� ����Ʈ ����
	if(pOutList)
		*pOutList = nodeList;

	return true;
}

void RToolSceneManager::ImportScene(const char *szFileName)
{
	using namespace rs3;

	RWorldSceneNode& rootSceneNode = GetRootSceneNode();
	std::string strOrgPath = rootSceneNode.GetXMLFileName();

	RToolSceneNodeInstance* pSceneInstance = static_cast<RToolSceneNodeInstance*>(CreateSceneNode(RSID_SCENEINSTANCE));
	if( pSceneInstance->Create( szFileName, false ) )
	{
		AttachLogicObjectTraverse( pSceneInstance );
		UpdateRootSceneTransform();

		// ���+���ϸ�+Ȯ���� �� ��θ� ������ ���ϸ�+Ȯ���ڸ� ��´�
		string strFileName = MGetRemovePathName( szFileName );
		int nNumber = CXMLEditInfoFile::GetReference().GetNumbering(&strFileName);
		char szBuffer[256];
		szBuffer[0] = '\0';
		sprintf(szBuffer,"%s_%03d", (strFileName.substr( 0, strFileName.length() - 4 )).c_str(), nNumber);
		pSceneInstance->SetNodeName(szBuffer);

		rootSceneNode.AddChild( pSceneInstance );
		rootSceneNode.AddToSceneNodeGroup( pSceneInstance );
	}

	rootSceneNode.SetXMLFileName(strOrgPath);

	m_pControlMediator->GetHelperResource()->SetAutoAnimationProperty();
	m_pControlMediator->GetHelperResource()->SetMediatorAfterSceneLoaded();

	m_pControlMediator->BroadCastMessage(VIEW_MESSAGE::SCENE_IMPORTED);
}

// Ȯ���� ������ ����Ʈ �̸� ���
string RToolSceneManager::GetEffectNameFromFileName( const string& strEffectFileName )
{
	const char* szToRemove = ".effect.scene.xml";

	unsigned int nExtPos = strEffectFileName.find( szToRemove );
	if( nExtPos != string::npos )
		return strEffectFileName.substr(0, nExtPos );
	
	return strEffectFileName;
}

bool RToolSceneManager::LoadSceneNodeFromFile(RSceneNode* pSceneNode,const char* szFileName)
{
	_ASSERT( pSceneNode );
	_ASSERT( szFileName );

	const char* szNodeIDString = pSceneNode->GetNodeIDString();
	bool bRet = false;

	// TODO: WorldSceneNode, SceneInstance, EffectInstance ���� Scene.xml �� ���� �����ϴ� ���� �����̹Ƿ�
	// �������̽��� �����ϰ� �ʹ�, base class �� �־ ������

	if ( strcmp(szNodeIDString,RSID_WORLDSCENENODE) == 0 )
	{
		bRet = LoadSceneNodeFromRootXMLFile(pSceneNode,szFileName,false);
	}
	else if  ( strcmp(szNodeIDString,RSID_SCENEINSTANCE) == 0 ) 
	{
		bRet = static_cast<RSceneNodeInstance*>(pSceneNode)->Create( szFileName, false );
	}
	else if ( strcmp(szNodeIDString,RSID_EFFECTINSTANCE) == 0 )
	{
		string strPureEffectName = GetEffectNameFromFileName( szFileName );

		bRet = static_cast<REffectInstanceSceneNode*>(pSceneNode)->Create(strPureEffectName.c_str(), false);
	}
	else if ( strcmp(szNodeIDString,RSID_ACTOR) == 0 )
	{
		bRet = static_cast<RActor*>(pSceneNode)->Create(szFileName, false);
	}
	else if ( strcmp(szNodeIDString,RSID_TREE) == 0 )
	{
		bRet = static_cast< RTreeSceneNode* >(pSceneNode)->Build(szFileName, false);
	}
	else
	{
		mlog( "%s �� xml ������ ����� ���� Ư���� �°� �����Դϴ�.\n", szFileName);
		MXml xml;
		xml.LoadFile(szFileName);
		MXmlElement* xmlElement = xml.Doc()->FirstChildElement(pSceneNode->GetNodeIDString());
		bRet = LoadXML( pSceneNode, *xmlElement, false);
	}

	return bRet;

}

} // end of namepsace