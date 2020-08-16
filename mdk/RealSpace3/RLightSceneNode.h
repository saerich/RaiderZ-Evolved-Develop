#pragma once

#include "RSceneNode.h"


namespace rs3 {

/// ��� ������ base Ŭ����. ������ �� �ν��Ͻ��� ��������� �ʽ��ϴ�.
class RS_API RLightSceneNode : public RSceneNode
{
	MDeclareRTTI;

//	DECLARE_USING_PASS(RRP_LIGHT);

public:
	RLightSceneNode(RRENDERPASS eRenderPass = RRP_LIGHT);
	virtual ~RLightSceneNode(void);

	bool GetEnable();

	void SetColorVector(RVector vColor) { m_color = vColor; }
	void SetColorRGB(DWORD dwColor);
	void SetAttenuationStart(float fAttenuationStart) { m_fAttenuationStart =fAttenuationStart; }
	
	RVector GetColorVector() const { return m_color; }
	DWORD GetColorRGB() const;
	float GetAttenuationStart() const { return m_fAttenuationStart; }	

	float GetApplyIntensity();

	bool CheckCameraInLight(RCameraSceneNode* pCamera);


	float	m_fIntensity;		// �ε�ǰų� �ʱ�ȭ�� Intensity
	float	m_fApplyIntensity;	// ���� ����Ǵ� Intensity
	RVector	m_color;
	float	m_fAttenuationStart;
};

}