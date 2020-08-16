#pragma once

#include "RPostEffectInterface.h"


namespace rs3 {

class RS_API RPostEffectPixelMotionBlur : public RPostEffectInterface
{
public:
	RPostEffectPixelMotionBlur();
	virtual ~RPostEffectPixelMotionBlur();

protected:
	virtual void CreateBuffers();
	virtual void DestroyBuffers();

	RShaderFX*		m_pShader;	

	// �׷����� �ִ� ���� �� ��
	RSceneNode*		m_pRenderNode;

	DWORD m_dwWidthSmall;
	DWORD m_dwHeightSmall;

	float	m_fBias;		// �ӵ� ���ۿ� �޽��� �׸� �� �÷� ���ۿ� ������ �޽��� z-fight�� ���ϱ� ���� ���̾�� �Ǵ�.

	int		m_nSamplerCnt;						// ���÷���. �ȼ� ���̴������� ���ø� Ƚ���� ����
	int		m_nImgSamCnt;						// �̹����� ��ä�� �� �׸��� Ƚ��.

public:
	RCameraSceneNode*		m_pCameraSceneNode;

	virtual	bool	Init( int nW, int nH) override;
	virtual	void	Destroy() override;

	virtual void	Render( RCameraSceneNode* pCamera, RTexture* pColorBuffer, RTexture* pNormalBuffer, RTexture* pDepthBuffer);

	// ����
	void			RenderOverlay();

	// ������ �������� ������ ������ ����� ����.
	inline void		SetSamplerCount( int n)	{ m_nSamplerCnt = n;			}
	inline int		GetSamplerCount()		{ return m_nSamplerCnt;			}
};

}


