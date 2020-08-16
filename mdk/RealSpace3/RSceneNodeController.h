#pragma once

// RSceneNodeController
// TODO : RActor �� RActorContrller �� ���տ���

namespace rs3 {

class RSceneNode;
class RCameraSceneNode;

enum RCONTROLLER_ATTRIBUTE : DWORD
{
	RCA_STICKY			= 1,			///< ����ɶ� ���� ����Ǿ�� �ϴ���
	RCA_MANAGED			= 1<<1			///< �ų�尡 �������� ���� �������� �ϴ���
};

class RS_API RSceneNodeController : public RHaveAttributes< RCONTROLLER_ATTRIBUTE >
{
	friend			RSceneNode;
protected:
	RSceneNode*		m_pSceneNode;
	bool			m_bEnable;
	bool			m_bManaged;	///< �ų�� ������ ���� ����
	RSNCTRL_TYPE	m_type;
	string			m_strControllerName;

	void			SetAutoUpdate( RSceneNode* pSceneNodeToSet, bool bUseAutoUpdate );
	bool			IsAutoUpdate( RSceneNode* pSceneNode );

	virtual void	OnAdd(){}
	virtual void	OnRemove(){}

	virtual void	OnEnable(){}
	virtual void	OnDisable(){}

public:
	RSceneNodeController(RSNCTRL_TYPE eType = RSNCTRL_UPDATE);
	virtual ~RSceneNodeController(void);

	DECLARE_NODE_ID_STRING("unknown_controller");

	virtual bool	CreateFromXML(MXmlElement &element) { return false; }
	virtual void	SaveToXML(MXmlElement &element) { }

	virtual			RSceneNodeController* Clone() { return NULL; }

	void			Enable(bool bEnable);
	bool			IsEnable()	{ return m_bEnable; }
	RSNCTRL_TYPE	GetType()	{ return m_type; }

	virtual void	Update(RCameraSceneNode* pCamera /*= NULL*/) { }

	virtual void	ReNew() { }

protected:
	void			SetWorldTransform(const RMatrix& matWorld);		///< �ų���� world transform �� ����
	void			SetVisibilityResult( float fVisibilityResult ); ///< �ų���� Visibility Result ���� ����
	void			SetNoiseRefResult( float fNoiseRefResult );		///< �ų���� NoiseRef Result ���� ����
};

//////////////////////////////////////////////////////////////////////////
// inlines

inline void RSceneNodeController::Enable(bool bEnable) 
{ 
	if(bEnable) OnEnable(); else OnDisable(); 
	m_bEnable = bEnable;
}

}