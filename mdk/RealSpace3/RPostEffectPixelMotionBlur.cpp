#include "StdAfx.h"
#include "RPostEffectPixelMotionBlur.h"
#include "RCameraSceneNode.h"
#include "RVType.h"
#include "RShaderCompositeManager.h"
#include "RShaderFX.h"



namespace rs3 {


RPostEffectPixelMotionBlur::RPostEffectPixelMotionBlur()
: RPostEffectInterface( PE_PIXEL_MOTION_BLUR, RRP_PBLUR)
, m_pShader(NULL)
, m_pRenderNode(NULL)
, m_dwWidthSmall(0)
, m_dwHeightSmall(0)
, m_nSamplerCnt(15)
, m_nImgSamCnt(1)
{
	// 1/65535�� �ص� z-fight�Ͼ��. �� �� �����ִ� ��ġ��.
	m_fBias	= (-1.f / 65535.f) * 10;

	m_nImgSamCnt = 1;

	SetActive( false);	// �⺻���� ���д�.
}

RPostEffectPixelMotionBlur::~RPostEffectPixelMotionBlur(void)
{
}



bool RPostEffectPixelMotionBlur::Init( int nW, int nH)
{
	if(!RPostEffectInterface::Init(nW, nH))
		return false;

	m_dwWidthSmall	= m_dwWidth / 2;
	m_dwHeightSmall	= m_dwHeight / 2;

	if ( false == REngine::GetDevice().QueryFeature(RQF_PS20) )
		return false;

	if ( false == REngine::GetDevice().QueryFeature(RQF_PS30) )
		return false;

	Restore();

	if(!m_pShader)
		m_pShader = REngine::GetDevice().CreateShaderFX("ObjectMotionBlur.fx");

	return true;
}

void RPostEffectPixelMotionBlur::Destroy()
{
	RPostEffectInterface::Destroy();

	if (m_pShader)
		REngine::GetDevice().DeleteShaderFX(m_pShader);
	m_pShader = NULL;
}

void RPostEffectPixelMotionBlur::CreateBuffers()
{
	// �ӵ� ���۴� ��ũ�� ��ǥ�� �ӵ��� ���̰� �ȴ�. xy�� ������Ʈ�� ��ȿ�� �Ǽ��� ���۷�..
	RFORMAT eVelocityBufferFormat = RFMT_G16R16F;

	// ����ŷ Ÿ��
	MakeMaskTarget( m_dwWidth, m_dwHeight, eVelocityBufferFormat);
}

void RPostEffectPixelMotionBlur::DestroyBuffers()
{
	DeleteMaskTarget();
}

//-----------------------------------------------------------------------------
// Name: RenderDOF()
// Desc: Renders Blur effect
//-----------------------------------------------------------------------------
void RPostEffectPixelMotionBlur::Render( RCameraSceneNode* pCamera, RTexture* pColorBuffer, RTexture* pNormalBuffer, RTexture* pDepthBuffer)
{
	RPFC_THISFUNC;
	// 2D ������ ���� ���� ����
	REngine::GetDevice().SetTextureFilter( 0, RTF_POINT );
	REngine::GetDevice().SetTextureFilter( 1, RTF_POINT );

	REngine::GetDevice().SetDepthEnable( false, false);

	REngine::GetDevice().SetTextureAddressClamp( 0);
	REngine::GetDevice().SetTextureAddressClamp( 1);

	RTechnique Technique;
	UINT uiPass, uiPassCount;

	Technique = m_pShader->GetTechnique("Blur");

	m_pShader->GetConstant("g_numSamples").SetInt( m_nSamplerCnt);
	switch( m_nImgSamCnt)
	{
		case 2:		m_pShader->GetConstant("g_fDevideVelocity").SetFloat( 0.6f); break;
		case 3:		m_pShader->GetConstant("g_fDevideVelocity").SetFloat( 0.45f); break;
		default:	m_pShader->GetConstant("g_fDevideVelocity").SetFloat( 1.0f/(float)m_nImgSamCnt);  break;
	}

	REngine::GetDevice().SetTexture(1, GetMaskBuffer());
	REngine::GetDevice().SetTexture(0, pColorBuffer);
	Technique.Begin(&uiPassCount, TECH_PRESERVE_NOT);
	for (uiPass = 0; uiPass < uiPassCount; ++uiPass) 
	{
		Technique.BeginPass(uiPass);

		DrawFullScreenQuad( 0.0f, 0.0f, m_dwWidth, m_dwHeight);

		Technique.EndPass();
	}
	Technique.End();


	// ���� ���� ���� ----------------------------------------------------------------------------------------------------------
	REngine::GetDevice().SetTextureFilter( 0, RTF_LINEAR );
	REngine::GetDevice().SetTextureFilter( 1, RTF_LINEAR );
	REngine::GetDevice().SetTextureAddressWrap( 0);
	REngine::GetDevice().SetTextureAddressWrap( 1);
	REngine::GetDevice().SetTexture( 0, NULL);
	REngine::GetDevice().SetTexture( 1, NULL);

	REngine::GetDevice().SetDepthEnable(true, true);


	//RenderOverlay();
}


//-----------------------------------------------------------------------------
// Name: RenderOverlay()
// Desc: Visializes one of the surfaces in the overlay
//-----------------------------------------------------------------------------
void RPostEffectPixelMotionBlur::RenderOverlay()
{
	// Ȱ��ȭ �ø� ó��
	if( !m_bActive)
		return;

	RTechnique Technique = m_pShader->GetTechnique( "Overlay");

	REngine::GetDevice().SetTexture(0, GetMaskBuffer());

	REngine::GetDevice().SetDepthEnable( false, false);

	UINT uiPass, uiPassCount;
	Technique.Begin(&uiPassCount, TECH_PRESERVE_NOT);
	for (uiPass = 0; uiPass < uiPassCount; ++uiPass) 
	{
		Technique.BeginPass(uiPass);

		DrawFullScreenQuad( 0.0f, 0.0f, 256, 256);

		Technique.EndPass();
	}
	Technique.End();

	REngine::GetDevice().SetTexture(0, NULL);

}



}