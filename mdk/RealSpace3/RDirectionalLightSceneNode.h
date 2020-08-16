#pragma once

#include "RSceneNodeID.h"
#include "RLightSceneNode.h"

namespace rs3 {

const float DEFAULT_SHADOW_LUMINOSITY = 0.35f;	// �׸��� ���� �ּ� �⺻��

class RS_API RDirectionalLightSceneNode : public RLightSceneNode
{
	DECLARE_NODE_ID_STRING(RSID_DIRECTIONALLIGHTSCENENODE);

public:
	RDirectionalLightSceneNode(RRENDERPASS eRenderPass = RRP_DIRLIGHT);
	virtual ~RDirectionalLightSceneNode(void);

	void		SetLightDiffuse(float r, float g, float b, float a) { SetLightDiffuse(RVector4(r, g, b, a)); }
	void		SetLightDiffuse(const RVector4& light) { m_diffuse = light; }
	void		SetLightSpecular(float r, float g, float b, float a) { SetLightSpecular(RVector4(r, g, b, a)); }
	void		SetLightSpecular(const RVector4& light) { m_specular = light; }
	void		SetLightAmbient(float r, float g, float b, float a) { SetLightAmbient(RVector4(r, g, b, a)); } // �� ���� ����
	void		SetLightAmbient(const RVector4& light) { m_ambient = light; }
	void		SetLightPosition(float x, float y, float z) { SetLightPosition(RVector3(x, y, z)); }
	void		SetLightPosition(const RVector3& pos) { SetPosition(pos); }
	void		SetLightDirection(float x, float y, float z) { SetLightDirection(RVector3(x, y, z)); }
	void		SetLightDirection(const RVector3& pos) { SetDirection(pos); }
	void		SetPower(float f) { m_fPower = f; }
	void		SetSize(float f)	{ m_fSize = f; }
	
	void		SetShadowLuminosity(float fShadowLuminosity)	{ m_fShadowLuminosity = fShadowLuminosity; }
	void		SetSkyColor(const RVector3& vSkyColor)			
	{ 
	//	_ASSERT(0 && "������ ����. RealSpace3 v0.9 ���� ���� ����\n");
		mlog("RDirectionalLightSceneNode::SetSkyColor() ������ ����. RealSpace3 v0.9 ���� ���� ����\n");
	}
	void		SetGroundColor(const RVector3& vGroundColor)
	{ 
	//	_ASSERT(0 && "������ ����. RealSpace3 v0.9 ���� ���� ����\n");
		mlog("RDirectionalLightSceneNode::SetGroundColor() ������ ����. RealSpace3 v0.9 ���� ���� ����\n");
	}
	void		SetSkySpecular(float fScale)	{ m_fSkySpecular=fScale; }


	inline const RVector4& GetDiffuse() const		{ return m_diffuse; }
	inline const RVector4& GetSpecular() const		{ return m_specular; }
	inline const RVector4& GetAmbient() const		{ return m_ambient; }
	
	inline float GetPower() const { return m_fPower; }
	inline float GetSize() const { return m_fSize; }
	
	float		GetShadowLuminosity() const			{ return m_fShadowLuminosity; }
	
	RVector3	GetSkyColor()
	{ 
		_ASSERT(0 && "������ ����. RealSpace3 v0.9 ���� ���� ����\n");
		mlog("RDirectionalLightSceneNode::GetSkyColor() ������ ����. RealSpace3 v0.9 ���� ���� ����\n");

		return RVector3::ZERO;
	}
	RVector3	GetGroundColor()
	{ 
		_ASSERT(0 && "������ ����. RealSpace3 v0.9 ���� ���� ����\n");
		mlog("RDirectionalLightSceneNode::GetGroundColor() ������ ����. RealSpace3 v0.9 ���� ���� ����\n");

		return RVector3::ZERO;
	}
	float GetSkySpecular() { return m_fSkySpecular; }

	bool IsUseSkySpecular() { return GetSkySpecular()>0.0f; }

	virtual bool CreateFromXML(MXmlElement &element, bool bBackgroundCreation);
	virtual void SaveToXML(MXmlElement &element);

protected:
	virtual void OnAddScene();
	virtual void OnRemoveScene();

private:
	void	InitFakeAABB(); // directional light�� ��ġ,ũ�Ⱑ ��� AABB�� �����峪,  AABB�� ����ϴ� �ٸ� ��ƾ������ ������ ������ AABB�� �����

	RVector4		m_diffuse;   
	RVector4		m_specular;  
	RVector4		m_ambient;
	float			m_fPower;
	float			m_fSize;
	float			m_fShadowLuminosity;	///< �׸��� ������ �ּҰ�
	float			m_fSkySpecular;
};

class RS_API RSubLightSceneNode : public RLightSceneNode
{
public:
	DECLARE_NODE_ID_STRING(RSID_SUBLIGHTSCENENODE);

	RSubLightSceneNode(RRENDERPASS eRenderPass = RRP_SUBLIGHT) : RLightSceneNode(eRenderPass)
	{
		m_color =RVector3(0,0,0);
		SetAABB( RBoundingBox::MBOX_INFINITY ); // ������ ������ �ǵ���
	}

	virtual bool CreateFromXML(MXmlElement &element, bool bBackgroundCreation);
	virtual void SaveToXML(MXmlElement &element);
	virtual RSceneNode*	Clone();
};

}
