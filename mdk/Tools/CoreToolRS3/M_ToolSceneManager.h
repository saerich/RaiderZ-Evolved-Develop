#pragma once

#include "RSceneManager.h"
#include "RViewFrustum.h"
#include "M_ToolTerrain.h"
#include "RSkyManager.h"
#include "REnvironment.h"
#include "MRTTI.h"

#include <vector>
#include <string>

class CControlMediatorHelperResource;
class CLogicObject_SceneNode;

namespace rs3 {

class RTerrain;
class RTreeSystem;
class RSceneSky;
class RSkyManager;
class RDirectionalLightSceneNode;
class RSceneNodeInstance;

typedef std::multimap<float, RPICKINFO, less<float> > RPICKINFO_MAP;

class RSceneNodeEnvModifier
{
public:
	RSceneNodeEnvModifier(){}
	virtual ~RSceneNodeEnvModifier(){}

	virtual void Clear() = 0;
	virtual void Update(rs3::RCollisionTree* pCollisionTree) = 0;
	virtual void SetEnvModifier(rs3::REnvironment::ETime eTime, rs3::REnvironment::EWeather eWeather, bool bFade, float fFadingTime, RCollisionTree* pCollisionTree =  NULL) = 0;
	virtual void ResetRootScene(const rs3::RSceneNode* pNodeRootToAttach) = 0;
};

template < typename T_EnvModifierImpl >
class RSceneNodeEnvModifierImpl : public RSceneNodeEnvModifier
{
public:
	RSceneNodeEnvModifierImpl()
	{
		m_pModifierImpl = new T_EnvModifierImpl(REnvironmentSet::GetInstancePtr(),rs3::REngine::GetSceneManager().GetCurrentRootSceneNode(false));
	}

	virtual ~RSceneNodeEnvModifierImpl()
	{
		delete m_pModifierImpl;
	}

	virtual void Clear()
	{
		m_pModifierImpl->Clear();
	}

	virtual void Update(rs3::RCollisionTree* pCollisionTree)
	{
		m_pModifierImpl->Update(pCollisionTree);
	}

	T_EnvModifierImpl* GetEnvModifierImpl()
	{
		return m_pModifierImpl;
	}

	virtual void SetEnvModifier(rs3::REnvironment::ETime eTime, rs3::REnvironment::EWeather eWeather, bool bFade, float fFadingTime, RCollisionTree* pCollisionTree = NULL)
	{
		m_pModifierImpl->SetCurrentEnvSet(eTime, eWeather, bFade, fFadingTime, pCollisionTree);
	}

	virtual void ResetRootScene(const rs3::RSceneNode* pNodeRootToAttach)
	{
		m_pModifierImpl->Clear();
		delete m_pModifierImpl;
		m_pModifierImpl = new T_EnvModifierImpl(REnvironmentSet::GetInstancePtr(),rs3::REngine::GetSceneManager().GetCurrentRootSceneNode(false));

		m_pModifierImpl->AttachNightNodeController(pNodeRootToAttach);
	}

private:
	T_EnvModifierImpl* m_pModifierImpl;
};

class CORETOOL_API RToolSceneManager : public RSceneManager
{
	friend class CControlMediatorHelperResource;	// for pick_ignore and error message

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� / �Ҹ� / ����(Clear)
	//
	RToolSceneManager(CControlMediator*);
	virtual ~RToolSceneManager();
	virtual void Clear();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����� ���丮 �Լ� �� �����̸����κ��� �����Լ�. �� ���� �Ļ� ����� ����� ���
	//
	virtual RSceneNode*			CreateSceneNode(const char *szNodeName);

	// File�κ��� �Է¾������ ������ �־���. ��׶���ε��� ���ϴ���..
	bool						LoadSceneNodeFromFile(RSceneNode* pSceneNode,const char* szFileName);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// SceneNode ���� ���丮 �Լ� �� �����̸����κ����� ����� �����Լ�
	//
	virtual bool				SaveRootSceneToXML(const char *szFileName );
	void						SaveRootSceneToXML(const char *szFileName, MXml& _refMXml);

	/// max ���Ϸ� ���� ��ũ��Ʈ�� �ͽ���Ʈ
	void						ExportMaxScript(const char* szMaxScriptFileName);
	void						SaveResourceListToXML(const char *szFileName);

	//////////////////////////////////////////////////////////////////////////
	// update
	virtual void				Update();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// SceneNode ��ŷ�Լ�
	//
	bool						Pick(const RVector &origin,const RVector &to, RPICKINFO_MAP& _refPickInfoOut, bool _bSelectMode);
	bool						PickFrustum(RSceneNodeList *pOutList, RViewFrustum &kFrustum);
	const RVector*				GetVerticalContactPos( const RVector& _refPos, const RSceneNode* _pSceneNodeToIgnore );
	const RVector*				GetVerticalContactPos( const RVector& _refPos, const RSceneNode* _pSceneNodeToIgnore, RVector* const _outNormal );
// 	RSceneNode*					GetPickIgnoredSceneNode(){ return m_pPickingIgnoreSceneNode; }

	//////////////////////////////////////////////////////////////////////////
	// �� ����������Ʈ ����
	CLogicObject_SceneNode*		CreateLogicObject( RSceneNode* pSceneNode );
	bool						DestroyLogicObject( RSceneNode* pSceneNode );

	bool						AttachLogicObject( RSceneNode* pSceneNode );	///< LogicObject �� ����� �ٿ��ְ� ���������ϵ��� LogicObjectList�� ����Ѵ�
	bool						DetachLogicObject( RSceneNode* pSceneNode );	///< LogicObject �� �����ϰ�, ���� ���ɸ�Ͽ��� �����Ѵ�
	
	/// �ش� ���� ��� ���� ��忡 ���������ϵ��� LogicObject ���̱�
	void						AttachLogicObjectTraverse( RSceneNode* pSceneNodeTree );

	/// ������ ���������ϵ��� �� ��ü�� CLogicObject �� ��´�
	CLogicObject*				GetLogicObjectLinkedTo( const RSceneNode* pSceneNode );


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// �̸����� ����� ã��
	//
	RSceneNode*					GetSceneNodeByName(const char* _pSzSceneNodeName);
	string						GetEffectNameFromFileName( const string& strEffectFileName );	///< Ȯ���� ������ ����Ʈ �̸� ���
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// �����̸Ӹ� ī�޶� ������
	// ���̾� ������ ������ ���
	virtual void				RenderPrimaryCamera();
	void						SetWireFrameDrawMode( bool bFlag ) { m_bWireFrameDrawMode = bFlag; }
	bool						GetWireFrameDrawMode() { return m_bWireFrameDrawMode; }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// �����޽���
	//
	void						AddErrorMessage(const char* _pSzMessage){ m_vecErrorMessage_.push_back(_pSzMessage); }
	std::vector<std::string>&	GetErrorMessage(){ return m_vecErrorMessage_; }


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// �ε� ��Ʈ��
	//
	bool						LoadEditableSceneXML(RSceneNode *pSceneNodeToLoad, const char* szFileName);
	bool						LoadEditableSceneXML(const char *szFileName);
//	bool						LoadXML(RSceneNode *pSceneNodeToLoad, MXmlElement &element, bool bBackgroundCreation) override;

	//////////////////////////////////////////////////////////////////////////
	// SceneNodeInstance
	bool						MakeSceneNodeInstanceEditable(RSceneNodeInstance* pSceneInstance, RSceneNodeList* pOutList );

	/// import scene as SceneNodeInstance
	void						ImportScene(const char *szFileName);

	//////////////////////////////////////////////////////////////////////////
	// ��/�� ����
	template< typename T_EnvModifierImpl >
	void CreateSceneNodeEnvModifier()
	{
		DestroySceneNodeEnvModifier();
		m_pSceneNodeModifier = new RSceneNodeEnvModifierImpl<T_EnvModifierImpl>();
	}

	void DestroySceneNodeEnvModifier()
	{
		if(m_pSceneNodeModifier)
		{
			m_pSceneNodeModifier->Clear();
			SAFE_DELETE(m_pSceneNodeModifier);
		}
	}

	template< typename T_EnvModifierImpl >
	T_EnvModifierImpl* GetSceneNodeEnvModifier()
	{
		if (NULL == m_pSceneNodeModifier)
			return NULL;

		RSceneNodeEnvModifierImpl<T_EnvModifierImpl>* pImpl = dynamic_cast<RSceneNodeEnvModifierImpl<T_EnvModifierImpl>*>(m_pSceneNodeModifier);
		if (NULL == pImpl)
			return NULL;

		return pImpl->GetEnvModifierImpl();
	}

	void SetEnvModifier(rs3::REnvironment::ETime eTime, rs3::REnvironment::EWeather eWeather, bool bFade, float fFadingTime, RCollisionTree* pCollisionTree)
	{
		if (m_pSceneNodeModifier)
			m_pSceneNodeModifier->SetEnvModifier(eTime, eWeather, bFade, fFadingTime, pCollisionTree);
	}

	void ResetEnvModifierRootScene()
	{
		if (m_pSceneNodeModifier)
			m_pSceneNodeModifier->ResetRootScene(&GetRootSceneNode());
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� / �������Ұ� �������� ��ŷ. ���� ���� ��ŷ�� ���� AABB�� ����Ѵ�.
	//
	RSceneNode*					AABBPicking(const RSceneNodeList *pNodeList, const RVector &origin, const RVector &to, RPICKINFO_MAP& _refPickInfoOut, bool _bSelectMode);
	int							AABBPickingFrustum(RSceneNodeList *pOutList, const RSceneNodeList *pChildNodeList, RViewFrustum &kFrustum);

	void						UpdateRootSceneTransform();

//	RSceneNode*					m_pPickingIgnoreSceneNode;	// indoor ������ static mesh �� �����ϱ� ���ѵ��ѵ� property �� �����Ƿ� ���ʿ��ϴ�

	bool						m_bWireFrameDrawMode;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// �ε����� �����޽���
	//
	std::vector<std::string>	m_vecErrorMessage_;


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����� �̸����� ã�� Functor
	//
	struct SSceneNodeFinder
	{
		static bool FuncGetByName( RSceneNode* pSceneNode )
		{
			if (pSceneNode->GetNodeName() == pSzSceneNodeName )
				return true;
			else
				return false;
		}
		static char* pSzSceneNodeName;
	};

	void						GatherAllActor(const RSceneNode *pSceneNode, RSceneNodeList* pOutList);
	void						GatherAllTrees(const RSceneNode *pSceneNode, RSceneNodeList* pOutList);
	void						ExportTerrainMaxScript(FILE* file);
	void						ExportLightMapActorMaxScript(FILE* file);
	void						ExportSpeedTreeMaxScript(FILE* file);

	//////////////////////////////////////////////////////////////////////////
	// ��/�� modifier
	RSceneNodeEnvModifier*		m_pSceneNodeModifier;
	CControlMediator*			m_pControlMediator;
};

}