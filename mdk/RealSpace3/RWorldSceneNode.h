#ifndef _RWORLDSCENENODE_H
#define _RWORLDSCENENODE_H


#include "RSceneNode.h"
#include "RResourceLoadingGroup.h"
#include "RCullingMgr.h"

// http://dev/redmine/issues/16758
/*	
namespace tbb {
class task_scheduler_init;
};
*/

namespace rs3 {

RS_API extern const char *RSID_WORLDSCENENODE;

class REnvironmentSet;
class RTerrain;
class RStaticMeshSceneNode;
class RDirectionalLightSceneNode;

class cScenodeUpdate
{
public:
	cScenodeUpdate()
	{

	}
	~cScenodeUpdate(){}

	virtual void update( RSceneNodeUpdateVisitor& _pSceneNodeUpdator ) = 0;
};

class cAutoSceneNode : public cScenodeUpdate
{
private:
	RAutoUpdateNodeList				m_listAutoUpdateSceneNode;
public:
	cAutoSceneNode(){}
	~cAutoSceneNode()		// ��ü ����
	{

	}

	void	update( RSceneNodeUpdateVisitor& _pSceneNodeUpdator );
	void	pushBack( RUPDATENODE& node );
	bool	ExistSceneNode(RSceneNode* pSceneNode);
	list<RUPDATENODE>::iterator	getEnd();
};

class cEmitter : public cScenodeUpdate
{
private:
	RAutoUpdateNodeList				m_listAutoUpdateSceneNode;		// �̹���
public:
	cEmitter(){}
	~cEmitter()
	{

	}

	void	update( RSceneNodeUpdateVisitor& _pSceneNodeUpdator );
	void	preEmitterUpdate( RSceneNodeUpdateVisitor& _pSceneNodeUpdator );
	void	parallelEmitterUpdate( RSceneNodeUpdateVisitor& _pSceneNodeUpdator );
	void	postEmitterUpdate( RSceneNodeUpdateVisitor& _pSceneNodeUpdator );
	void	seriesEmitterUpdate( RSceneNodeUpdateVisitor& _pSceneNodeUpdator );
	void	pushBack( RUPDATENODE& node );
	list<RUPDATENODE>::iterator	getEnd();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// ���带 �����ϴ� ���� �����.
//
class RS_API RWorldSceneNode : public RSceneNode, public RResourceLoadingGroup
{
	friend RSceneManager;
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// SceneNode Declaration
	//
	MDeclareRTTI;

//	DECLARE_USING_PASS(RRP_NORMAL);
	DECLARE_NODE_ID_STRING(RSID_WORLDSCENENODE);

	RWorldSceneNode(void);
	virtual ~RWorldSceneNode();

	const string&				GetXMLFileName() const;	// ���� �ε��ϰ��ְų� ������ �ε�� XML �����̸��� ����
	void						SetXMLFileName(const string& _strFileName){ m_strXMLFileName = _strFileName; };
	string						GetXMLFilePath() const;

	virtual bool				CreateFromXML(MXmlElement &element, bool bBackgroundCreation);

	void SetEnvironmentSet( REnvironmentSet* pEnv)								{ m_pEnvironmentSet = pEnv;	}
	REnvironmentSet* GetEnvironmentSet()										{ return m_pEnvironmentSet;	}
	void SetTerrain( RTerrain* pTerrain)										{ m_pTerrain = pTerrain;	}
	RTerrain* GetTerrain()														{ return m_pTerrain;	}
	void SetStaticMeshSceneNode( RStaticMeshSceneNode* pStaticMeshSceneNode)	{ m_pStaticMeshSceneNode = pStaticMeshSceneNode;	}
	RStaticMeshSceneNode* GetStaticMeshSceneNode()								{ return m_pStaticMeshSceneNode;	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// �������� ���� �ĺ���(Candidate) ����� ��� / ������
	//
	const RSceneNodeArray*		GetNodeArray(RRENDERPASS pass){ return &m_candidateNodes[pass]; }	///* �� �н��� NodeArray�� ����

	bool						RegisterRenderCandidate( RSceneNode* pSceneNode );
	bool						UnregisterRenderCandidate( RSceneNode* pSceneNode );

	//�ø��Ŵ��� ������
	void			RegisterCullingMgr(RCullingMgr* pCullingMgr);
	void			UnRegisterCullingMgr(RCullingMgr* pCullingMgr);
	void			MoveSceneNodeInBVH(RSceneNode *pNode);
	void			RemoveSceneNodeFromBVH(RSceneNode *pNode);
	void			AddSceneNodeToBVH(RSceneNode* pNode);
	void			RefreshRenderingList(RSceneNode* pNode);

	//directional light
	RDirectionalLightSceneNode*	GetDirectionalLight(void)											{	return m_pDirectionalLight;					}
	void						SetDirectionalLight(RDirectionalLightSceneNode* pDirectionalLight)	{	m_pDirectionalLight = pDirectionalLight;	}

	//AutoUpdate
	RAutoUpdateNodeListItr			RegisterAutoUpdate( RSceneNode* pSceneNode );
	void							UnregisterAutoUpdate( RAutoUpdateNodeListItr itr );
	virtual	void							UpdateAutoUpdateSceneNode();
	void							GatherAABB( RBoundingBox* _pGatherAABB );
	bool							ExistSceneNodeInAutoUpdateList(RSceneNode* pSceneNode );

	//Bounding Volume
	virtual	void							UpdateBoundingVolumeAllSceneNodes();

private:
	void			RemoveGlobalLightingCache();


protected:
	string						m_strXMLFileName;			// ���� ���� SceneXML�� �� ��ġ
	REnvironmentSet*			m_pEnvironmentSet;
	RTerrain*					m_pTerrain;
	RStaticMeshSceneNode*		m_pStaticMeshSceneNode;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Rendering
	//
	RSceneNodeArray					m_candidateNodes[RRP_MAX];		// render candidate scene node array

private:
	
	RCullingMgrObserver		m_CullingMgrObserver;		//�ø��Ŵ��� ������
	RDirectionalLightSceneNode	*m_pDirectionalLight;	//directional light

	RAutoUpdateNodeList				m_listAutoUpdateSceneNode;	// ������Ʈ�� �ҷ���� �ϴ� ����

	// http://dev/redmine/issues/16758
	/* 
	cAutoSceneNode					m_cAutoSceneNode;
	cEmitter						m_cEmitter;
	tbb::task_scheduler_init*		m_pTaskInit;
	*/
	
};

//////////////////////////////////////////////////////////////////////////
inline const string& RWorldSceneNode::GetXMLFileName() const { return m_strXMLFileName; }	// ���� �ε��ϰ��ְų� ������ �ε�� XML �����̸��� ����


}

#endif
