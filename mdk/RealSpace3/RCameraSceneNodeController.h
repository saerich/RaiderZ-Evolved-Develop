#ifndef _RCAMERA_SCENE_NODE_CONTROLLER_H
#define _RCAMERA_SCENE_NODE_CONTROLLER_H

#include "RTypes.h"

namespace rs3 {

/// ��Ʈ�ѷ��� ������Ʈ �Ǵ� Ÿ�ֿ̹� ���� Ÿ���� ������
enum RCAMERACONTROLLERTYPE {

	RCCT_PREUPDATE = 0,	// �ִϸ��̼� ������Ʈ ��
	RCCT_POSTUPDATE,	// �ִϸ��̼� ������Ʈ ��
	RCCT_PRERENDER,		// ������ ����
	RCCT_POSTRENDER,	// ������ ��

	RCCT_MAX,
};

class RCameraSceneNode;
class RInferredRenderingStrategy;
class RS_API RCameraSceneNodeController
{
	friend RCameraSceneNode;

public:
	// ����
	RCameraSceneNodeController(RCAMERACONTROLLERTYPE eType = RCCT_PREUPDATE, const char* _pSzCameraControllerID = NULL, bool _bManagedByCamera = true)
	: m_strCameraControllerID(_pSzCameraControllerID)
	, m_eType(eType)
	, m_pCameraSceneNode(NULL)
	, m_bRemoveReserved(false)
	, m_bManagedByCamera(_bManagedByCamera)
	{
	}

	RCAMERACONTROLLERTYPE	GetType() { return m_eType; }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���̵��� ���ĺ�������� ������Ʈ�� �˴ϴ�.
	// ���߿� ��� ��Ʈ���� �����ϴ� ��Ʈ�ѷ��� ����Ŷ��, ���̵� z_ �� �����Ѵٰų�.. �̷������� �ϼż�
	// ������� ������ ��ġ���� ���ý�Ű�� �˴ϴ�.
	//
	const std::string&					GetCameraControllerID()	{ return m_strCameraControllerID; }


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ��Ʈ�ѷ����� ����
	//
	void								RemoveFromCamera();
	void								ReserveToRemove()		{ m_bRemoveReserved = true; }


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ī�޶� ����
	//
	bool								IsRemoveReserved()	{ return m_bRemoveReserved; }
	bool								IsManagedByCamera()	{ return m_bManagedByCamera; }


protected:

	// �Ҹ�
	virtual ~RCameraSceneNodeController(void)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� ������Ʈ
	//
	virtual void						UpdateCameraController(RMatrix& _rMatApplyToCamera) = 0;
	virtual void						OnClear(){};
	virtual void						OnRender(){};
	virtual void						OnPostRender(){};

	// ī�޶�κ��� ����, ���ö�
	virtual bool						IsBlendableBeforeController(){ return false; }	// ����, �̹� ���� Controller�� �ִ���
	virtual void						OnInsertToCamera( RCameraSceneNodeController* _pBeforeController ){}
	virtual void						OnRemoveFromCamera(){}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Controller id / Controller owner camera
	//
	RCAMERACONTROLLERTYPE				m_eType;
	std::string							m_strCameraControllerID;
	RCameraSceneNode*					m_pCameraSceneNode;


	// ��Ʈ�ѷ� �����̳ʿ��� ���ŵǱ� ���� ���
	bool m_bRemoveReserved;
	bool m_bManagedByCamera;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// RCameraSceneNode������ �Ҹ��°�
	//
	void InsertToCameraWhenControlAdded( RCameraSceneNode* _pCameraSceneNode, RCameraSceneNodeController* _pBeforeController )
	{
		m_pCameraSceneNode = _pCameraSceneNode;
		OnInsertToCamera(_pBeforeController);
	}
};

}

#endif
