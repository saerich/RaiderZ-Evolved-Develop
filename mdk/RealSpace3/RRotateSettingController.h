#pragma once

#include "RSceneNodeController.h"

namespace rs3 {

// Enable() �� ������ ������ ��� ȸ�� ���� ������ �����ϰ� ���� ȸ������ �������ش�.
class RRotateSettingController : public RSceneNodeController
{
public:
	DECLARE_NODE_ID_STRING( "RotateSettingController" );

	RRotateSettingController( RVector vRotateAxis, float fRotateMin, float fRotateMax );

	float	GetRotateMin() { return m_fRotateMin; }
	float	GetRotateMax() { return m_fRotateMax; }

protected:
	virtual void	OnAdd() override;
	virtual void	OnRemove() override;

	virtual void	OnEnable() override;
	virtual void	OnDisable() override;

private:
	void	ResetLocalTransform();

	RVector	m_vDirectionOrg;
	RVector	m_vRotateAxis;
	float	m_fRotateMin;
	float	m_fRotateMax;
};

}