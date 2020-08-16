#pragma once

#include "V_LogicObjectRenderer.h"

class CLogicObject_AABBPicking;


/** @brief CLogicObjectRenderer LogicObject �� custom view �� �����ϱ� ���� interface class */
class CLogicObjectRenderer
{
public:
	virtual ~CLogicObjectRenderer() {}

	virtual void	OnAdd(CControlMediator*	pControlMediator_) {}			// manager �� ��ϵɶ� 
	virtual void	OnRemove(CControlMediator*	pControlMediator_) {}		// manager �� ������
	virtual void	OnChanged() {}			// logicobject �Ӽ��� �ٲ������
	virtual void	OnRender() {}			// render helper ������ ���� �׸����
	virtual void	OnRenderSelection() {}	// ���õȰ��
	virtual void	OnRenderTextInfo() {}
	virtual void	OnShow() {}				// ��������
	virtual void	OnHide() {}				// ��������
};


//////////////////////////////////////////////////////////////////////////
/** @brief CLogicObjectRenderer_SceneNode sceneNode �� renderer �� ����ϱ� ���� CLogicObjectRenderer */

class CORETOOL_API CLogicObjectRenderer_SceneNode : public CLogicObjectRenderer
{
public:
	CLogicObjectRenderer_SceneNode(CLogicObject_AABBPicking* pLogicObject, rs3::RSceneNode* pViewSceneNode)
		: m_pViewNode(pViewSceneNode) { m_pLogicObject = pLogicObject; }
	virtual ~CLogicObjectRenderer_SceneNode();

	void			SetViewSceneNode(rs3::RSceneNode* pSceneNode) { m_pViewNode = pSceneNode; }
	rs3::RSceneNode* GetViewSceneNode() { return m_pViewNode; }

	virtual void	OnAdd(CControlMediator*	pControlMediator_) override;
	virtual void	OnRemove(CControlMediator*	pControlMediator_) override;
	virtual void	OnChanged() override;
	virtual void	OnShow() override;
	virtual void	OnHide() override;

protected:
	virtual bool	GetCollisionEnabled() { return false; }

	rs3::RSceneNode*			m_pViewNode;
	CLogicObject_AABBPicking*	m_pLogicObject;	///< �����ϴ� document
};

//////////////////////////////////////////////////////////////////////////
/** @brief CLogicObjectRenderer_SceneNodeInstance sceneNodeInstance �� renderer �� ����ϱ� ���� CLogicObjectRenderer */

class CORETOOL_API CLogicObjectRenderer_SceneNodeInstance : public CLogicObjectRenderer_SceneNode
{
public:
	CLogicObjectRenderer_SceneNodeInstance(CLogicObject_AABBPicking* pLogicObject, rs3::RSceneNode* pSceneNode)
		: CLogicObjectRenderer_SceneNode( pLogicObject, pSceneNode ) {}

	void	LoadSceneNodeInstance(CControlMediator* pControlMediator_, const char* szSceneFilename);
	void	ReloadSceneNodeInstance(CControlMediator* pControlMediator_, const char* szSceneFilename);
};


//////////////////////////////////////////////////////////////////////////
/** @brief CLogicObjectRenderer_SceneNodeItself scenenode model �ڱ� �ڽ��� renderer �� ����ϱ� ���� CLogicObjectRenderer */

class CORETOOL_API CLogicObjectRenderer_SceneNodeItself : public CLogicObjectRenderer
{
public:
	CLogicObjectRenderer_SceneNodeItself(CLogicObject_AABBPicking* pLogicObject, rs3::RSceneNode* pSceneNode)
		: m_pSceneNode(pSceneNode) { m_pLogicObject = pLogicObject; }

protected:
	rs3::RSceneNode*			m_pSceneNode;	///< model ���� view ���
	CLogicObject_AABBPicking*	m_pLogicObject;	///< �����ϴ� document

	virtual void	OnChanged() override;
	virtual void	OnShow() override;
	virtual void	OnHide() override;
};
