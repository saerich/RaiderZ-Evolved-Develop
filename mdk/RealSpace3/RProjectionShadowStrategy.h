#pragma once

#include "RDepthMapShadowStrategy.h"

namespace rs3 {

class RProjectionShadowStrategy : public RDepthMapShadowStrategy
{
public:
	RProjectionShadowStrategy(const RShadowDesc& desc);
	virtual ~RProjectionShadowStrategy(void);

	virtual void Init();
	virtual void Destroy();

	//Lost device �߻��� ShadowMap�� �ٽ� �������־���ϱ� ������ ������ ����/�����Ѵ�.
	void CreateShadowMap();
	void DestroyShadowMap();

	virtual void Render(RCameraSceneNode *pViewCamera, RWorldSceneNode *pWorld) override;

	virtual const RMatrix GetCurrentLightTransform();
	virtual const RMatrix GetCurrentLightTransform(RCameraSceneNode& LightCamera);
	virtual const RMatrix GetCurrentLightViewTransform() { return m_pLightCamera.GetViewMatrix(); }
	virtual RSCID GetDepthWriteComponent();
	virtual const RCameraSceneNode* GetCurrentDepthCamera() { return &m_pLightCamera; }


	void RenderShadowBuffer(RCameraSceneNode* pCamera, RLightingShader* pLightingShader);

	virtual RTexture* GetCurrentDepthMap() { return m_pShadowMap; }

protected:
	virtual void SetandClearRenderTarget(RCameraSceneNode* pCamera, RTexture* pRenderTarget);

private:
	RListener*		m_pEventListener;			///< OnRestoreDevice / OnLostDevice �̺�Ʈ ���Ű�ü

	RCameraSceneNode	m_pLightCamera;

	RTexture* m_pShadowMap;
};

}