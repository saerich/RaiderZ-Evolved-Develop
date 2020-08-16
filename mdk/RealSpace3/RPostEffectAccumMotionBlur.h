#pragma once


#include "RPostEffectInterface.h"



// Motion Blur ( Accum buffer) �� ó���ϴ� Ŭ����

namespace rs3 {


class RS_API RPostEffectAccumMotionBlur : public RPostEffectInterface
{
public:
	RPostEffectAccumMotionBlur();
	virtual ~RPostEffectAccumMotionBlur();

protected:
	virtual void CreateBuffers()  override;
	virtual void DestroyBuffers()  override;

	RShaderFX*		m_pShader;	

	// Render Target Texture
	RTexture* 		m_pAccumBuffer[2];				
	
	bool	m_id;

	float m_fAccumRate;
	DWORD m_dwAccumRate;
	float m_fDecreaseAlpha;
	DWORD m_dwTimeAccum;
	float m_fLastAlphaValue;


public:
	
	virtual	bool	Init( int nW, int nH) override;
	virtual	void	Destroy() override;

	virtual void	Render( RCameraSceneNode* pCamera, RTexture* pColorBuffer, RTexture* pNormalBuffer, RTexture* pDepthBuffer);

	void			FrameMove();	// ī�޶� ��Ʈ�ѷ��� ������ ���갡 ���� ����. �������� ȣ�� �ǵ��� ����.

	void			RenderOverlay();

	inline bool		SetDecreaseAlpha( float f)	{ m_fDecreaseAlpha = f;			return true; }
	inline float	GetDecreaseAlpha()			{ return m_fDecreaseAlpha;		}

	inline bool		SetAccumRate( float f)		{ m_fAccumRate = f;	m_dwAccumRate = (DWORD)(m_fAccumRate*1000.0f);		return true;}
	inline float	GetAccumRate()				{ return m_fAccumRate;		}

	inline bool		IsBeingBlur()				{ return (bool)(m_fLastAlphaValue>0);	}

};

}
