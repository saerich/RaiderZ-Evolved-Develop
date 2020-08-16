#pragma once
#include "RCameraSceneNode.h"
#include "XBaseGameCamera.h"

// ����ǥ��
struct XPolarCoord
{
	float fRadius;
	float fTheta;		// z�� xy��� ������ ��, ����
	float fPhi;			// x�࿡�� y�������� ��, �¿�
};

class XBaseCameraSceneNode : public rs3::RCameraSceneNode
{
protected:
	XPolarCoord		m_PolarCoord;
	MUID			m_uidTarget;

	virtual void				OnUpdate();

public:
	XBaseCameraSceneNode(void);
	virtual ~XBaseCameraSceneNode(void);

	XPolarCoord*	GetPolarCoord() { return &m_PolarCoord; }
	void			SetPolarCoord( XPolarCoord* pCoord );

	virtual bool	GetTargetPosition( vec3& outPos );
	void			SetTargetMUID(MUID uid);

	const float		GetDist();
};
