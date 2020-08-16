#pragma once


namespace rs3 {

enum POST_EFFECT_TYPE
{
	 
	PE_HDR = 0,		// HDR�� 64bit�� ���� �������� ���̹Ƿ� �ݵ�� 0��° �ε������� ��.
	PE_VOID,
	PE_EDGE,
	PE_DISTORTION,
	PE_ACCUM_MOTION_BLUR,
	PE_DOF,
	PE_PIXEL_MOTION_BLUR,
	PE_SCREEN_MOTION_BLUR,
	PE_FINAL_SCENE,
	PE_COUNT,
};

class RShaderFX;

class RS_API RPostEffectInterface
{
public:
	RPostEffectInterface( POST_EFFECT_TYPE eType, RRENDERPASS maskingRenderPass);
	virtual ~RPostEffectInterface(void);

	const POST_EFFECT_TYPE GetID()	{ return m_eType; }

public:
	virtual bool Init( int nW, int nH );
	virtual void Destroy();

	void Invalidate();
	void Restore();

	virtual void ClearMaskBuffer();
	virtual void Render( RCameraSceneNode* pCamera, RTexture* pColorBuffer, RTexture* pNormalBuffer, RTexture* pDepthBuffer) = 0;
	//virtual void OnRender( RCameraSceneNode* pCamera, RTexture* pColorBuffer, RTexture* pNormalBuffer, RTexture* pDepthBuffer)
	//{
	//	if( m_bActive)
	//		Render( pCamera, pColorBuffer, pNormalBuffer, pDepthBuffer);
	//}

	bool Resize( int nW, int nH );
	bool IsUsingMaskBuffer()	{ return ( m_pMaskingTarget != NULL);	}
	RTexture* GetMaskBuffer()	{ return m_pMaskingTarget;				}
	RRENDERPASS GetMaskingRenderPass() { return m_maskingRenderPass;	}
	void ShowDebug( bool b)		{ m_bDebug = b;							}
	void SetActive( bool b)		{ m_bActive = b;						}
	bool IsActive()				{ return m_bActive;						}

	void SetLastPostEffect(bool _bLasetPostEffect)	{ m_bLastPostEffect = _bLasetPostEffect; }
	bool GetLastPostEffect()	{ return m_bLastPostEffect; }

protected:
	virtual void CreateBuffers() {}
	virtual void DestroyBuffers() {}


	POST_EFFECT_TYPE	m_eType;
	// ������ �⺻ ������
	DWORD				m_dwWidth,m_dwHeight;
	RTexture*			m_pMaskingTarget;
	RRENDERPASS			m_maskingRenderPass;
	bool				m_bDebug;
	bool				m_bActive;
	bool				m_bLastPostEffect;

protected:
	virtual void DrawFullScreenQuad( DWORD dwX, DWORD dwY, DWORD dwW, DWORD dwH);
	virtual bool MakeMaskTarget( int nW, int nH, RFORMAT eBufferFormat);
	virtual void DeleteMaskTarget();
};



//=====================================================================================
//RPostEffectVoid : Ư���� ������ ���� �ܼ��� �ؽ��ĸ� ���� Ÿ������ ����ϱ� ���� �뵵

class RPostEffectVoid : public RPostEffectInterface
{
public:
	RPostEffectVoid();
	virtual ~RPostEffectVoid() {};
	
	virtual void Render( RCameraSceneNode* pCamera, RTexture* pColorBuffer, RTexture* pNormalBuffer, RTexture* pDepthBuffer);
};
//=====================================================================================

}
