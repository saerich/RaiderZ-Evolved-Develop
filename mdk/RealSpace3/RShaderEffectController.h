#pragma once

#include "RSceneNodeController.h"
#include "RShaderComponentID.h"

namespace rs3 {

class RS_API RShaderEffectController : public RSceneNodeController
{
public:
	DECLARE_NODE_ID_STRING("ShaderEffectController");

	RShaderEffectController();
	virtual ~RShaderEffectController();

	bool CheckInstancingCondition();

	void EnableAddColorEffect( const RVector3& color );
	void DisableAddColorEffect();
	bool IsEnableAddColorEffect()	{ return m_bAddColor;}

	void EnableGlowEffect( const RVector3& color );
	void DisableGlowEffect();
	bool IsEnableGlowEffect()	{ return m_bGlowEffect;}

	static void EnableRimLighting();
	static void DisableRimLighting();
	static void SetRimLightingDistance( float fRimDistance, float fRimFalloffStart );

	RSCID CommitShaderMStage( RSCID* pBeforeComponent, bool bPointColor, bool bUseShade);
	RSCID CommitShaderGStage( RSCID* pBeforeComponent, bool bPointColor, bool bUseShade, float fDistanceToCamera );

	virtual void Update(RCameraSceneNode* pCamera) override;

private:
	bool		m_bAddColor;
	RVector3	m_vAddColor;

	bool		m_bGlowEffect;
	bool		m_bGlowEffectFading;
	RVector3	m_vGlowColor;
	float		m_fGlowScale;

	static bool	 m_bRimLighting;
	static float m_fRimDistance;		///< �� ����Ʈ ���� �Ÿ�
	static float m_fRimFalloffStart;	///< �� ����Ʈ ���� ���� �Ÿ�
	static float m_fRimFalloffLength;	///< �� ����Ʈ ���� ���� ���� �Ÿ� ( RimDistance - RimFalloffStart )
	static float m_fRimFalloffRate;		///< �� ����Ʈ ���� ����� ( 1 / RimFalloffLength )
};

}