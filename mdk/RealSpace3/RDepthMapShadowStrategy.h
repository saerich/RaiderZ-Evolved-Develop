#pragma once

#include "RShadowStrategy.h"
namespace rs3 {

//�׸��ڸ� ���� ����� ����ϱ� �����ϴ� �׸��ڸ� ����
//�� �������� ���� ��� ���
#define REUSE_MIN_INDEX 2		

class RS_API RDepthMapShadowStrategy : public RShadowStrategy
{
public:
	RDepthMapShadowStrategy();
	virtual ~RDepthMapShadowStrategy();

	virtual RSCID GetDepthWriteComponent();

	virtual RTexture* GetCurrentDepthMap() { return m_pRenderTarget; }
	virtual RTexture* GetDoubleShadowDepthMap() { return m_pDoubleShadowRT; }
	virtual int	GetCurrentDepthMapSize() { return m_nTextureSize; }
	
	virtual bool IsCastShadow(const RMatrix& matView, const RMatrix& matViewProj, const RSphere& boundingSphere);

	RShaderFX* GetClearFloatShader()	{ return m_pShaderClearFloat; }

	void RenderShadowMap(RCameraSceneNode& shadowCamera);
	void CombineDoubleShadowMap();

	//DepthMap ���� ��� ����
	RTexture* GetReusedDepthMap(int nIndex)	{ return m_pReusedRT[nIndex]; }
	void SetResetDepthMap(bool bReset)	{ m_bResetDepthMap = bReset; }
	void CombineDepthMap(int nDepthMapCount);

	//Debug ��
	void RenderDepthMapToScreen(int nX, int nY, int nWidth, int nHeight);

	void SetUseRenderMinArea(bool bEnable)	{ m_bUseRenderMinArea = bEnable; }
	void SetRenderMinArea(float fMinArea)	{ m_fMinRenderArea = fMinArea; }

	bool GetUseRenderMinArea()	{ return m_bUseRenderMinArea; }
	float GetRenderMinArea()	{ return m_fMinRenderArea; }

	RVector4 GetDepthBias() { return m_vDepthBias; }

protected:

	void RenderShadowMap(RCameraSceneNode *pViewCamera, RWorldSceneNode *pWorld, RCameraSceneNode &shadowCamera, RShadowTransform* ShdowTransform, 
							float fNearZ, float fFarZ, const RVector3 &vLightDirection);

	bool CreateClearFX();
	void DestroyClearFX();

	/// ���̰��� �ִ밪���� ä���ִ´�
	void ClearDepth(int x, int y, int nWidth, int nHeight);

	virtual void SetandClearRenderTarget(RCameraSceneNode* pCamera, RTexture* pRenderTarget);

	void MakeTextureScaleTransform();

	int				m_nTextureSize;			///< ����Ÿ�� ������ �ؽ��� ������
	float			m_fBorderSize;			///< ������� �ʴ� ����, �׵θ�1 �� �༭ �þ����� �׸��ڸ� ���´�
	RShaderFX*		m_pShaderClearFloat;	///< float �� clear �Ҽ� �ִ� ���̴�
	RMatrix			m_matTextureScale;		///< ����Ʈ ���������� ���� -> �ؽ��� ����
	RMatrix			m_matToler;				///< �׸��� �����ϴ� �ּ� ����

	RPlane			m_debugPlane;			///< ������ ���

	RTexture*		m_pDepthBuffer;

	RVector4		m_vDepthBias;

	//-------------------------------------------------
	//DepthMap ���� ��� ����
	void SetRenderTargetIndex(int nPrevRT, int nCurrentRT)	{ m_nPrevRT=nPrevRT; m_nCurrentRT=nCurrentRT; }
	
	void RenderReusedShadowMap(RCameraSceneNode *pViewCamera, RWorldSceneNode *pWorld, RCameraSceneNode* shadowCamera, RShadowTransform* ShdowTransform, 
		float fNearZ, float fFarZ);

	bool							m_bResetDepthMap;
	int								m_nPrevRT;
	int								m_nCurrentRT;
	RTexture*						m_pRenderTarget;
	RTexture*						m_pCommonShadowRT;
	RTexture*						m_pDoubleShadowRT;
	RTexture**						m_pReusedRT;
	RDirectionalLightSceneNode*		m_pDirectionalLight;
	RVector3						m_vLightDirection;
	//-------------------------------------------------

private:
	void	CullCasters( RWorldSceneNode* pWorld, RCameraSceneNode &shadowCamera );

	void	FindMinMaxZObject(RCameraSceneNode &shadowCamera, RRENDERPASS beginPass, RRENDERPASS endPass, float& fOutmin, float& fOutmax);

	//-----------------------------------------------------------------------------------------
	//DepthMap ���� ��� ����
	void SetStaticPass(RCameraSceneNode* pCamera);
	void SetDynamicPass(RCameraSceneNode* pCamera);

	bool CullUpdatedRegion(RWorldSceneNode* pWorld, RCameraSceneNode* pCamera, RViewFrustum* pPrevFrustum, RVector3 vCameraDelta,
							RRENDERPASS BeginPass, RRENDERPASS EndPass);
	
	void RenderStaticSceneNode(RCameraSceneNode* pCamera, RWorldSceneNode* pWorld, RShadowTransform* pShadowTranform, RVector3 vPrevLightPos, RMatrix matPrevViewProj);
	void RenderPrevDepthMap(const RMatrix& matPrevViewProj, const RMatrix& matCurrentViewProj);
	void RenderDynamicSceneNode(RWorldSceneNode *pWorld, RCameraSceneNode* pCamera, RShadowTransform* pShadowTranform);
	void RenderSceneNode(RCameraSceneNode* pCamera, RRENDERPASS beginPass, RRENDERPASS endPass);
	void RenderDoubleShadowSceneNode(RCameraSceneNode* pCamera);

	void GetCullPlanesForX(const RViewFrustum* pPrevFrustum, const RViewFrustum* pCurrentFrustum, float fDelta, RPlane* outPlanes);
	void GetCullPlanesForY(const RViewFrustum* pPrevFrustum, const RViewFrustum* pCurrentFrustum, float fDelta, RPlane* outPlanes);
	//-----------------------------------------------------------------------------------------

	RSceneNodeUpdateVisitor m_ShadowCameraUpdator;

	bool m_bUseRenderMinArea;
	float m_fMinRenderArea;
};

}