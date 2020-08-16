#include "StdAfx.h"
#include "RHBAOProcessor.h"
#include "REngine.h"
#include "RVType.h"
#include "RRenderHelper.h"
#include "RShaderFX.h"

namespace rs3 {


RHBAOProcessor::RHBAOProcessor( int n)
: RGIProcessor(n)
{
	m_pTarget = NULL;
	m_pTargetDenoise = NULL;
	m_pCurrentTarget = NULL;
	m_Effect = NULL;
	m_ptexNoise = NULL;

	m_Effect        = NULL;
	m_pRndTexture	= NULL;

	m_bDenoise = false;
	m_bTargetImmediately = true;
}

RHBAOProcessor::~RHBAOProcessor(void)
{
	Destroy();
}


void RHBAOProcessor::Render( RCameraSceneNode* pCamera, int nWidth, int nHeight, bool bRenderToBackBuffer)
{
	if( !m_Effect || !pCamera)
		return;
	
	m_pCurrentTarget = NULL;

	RTexture* pRT0 = REngine::GetDevice().GetRenderTarget( 0);
	RTexture* pRT1 = REngine::GetDevice().GetRenderTarget( 1);
	RTexture* pRT2 = REngine::GetDevice().GetRenderTarget( 2);
	RTexture* pRT3 = REngine::GetDevice().GetRenderTarget( 3);

	REngine::GetDevice().SetRenderTarget( 1, NULL);
	REngine::GetDevice().SetRenderTarget( 2, NULL);
	REngine::GetDevice().SetRenderTarget( 3, NULL);

	RTexture* pTexOrg0 = REngine::GetDevice().GetTexture( 0);
	RTexture* pTexOrg1 = REngine::GetDevice().GetTexture( 1);
	RTexture* pTexOrg2 = REngine::GetDevice().GetTexture( 2);
	RTexture* pTexOrg3 = REngine::GetDevice().GetTexture( 3);
	RTexture* pTexOrg4 = REngine::GetDevice().GetTexture( 4);
	RTexture* pTexOrg5 = REngine::GetDevice().GetTexture( 5);
	RTexture* pTexOrg6 = REngine::GetDevice().GetTexture( 6);

	// ��� ���� : ȭ�� �ػ� ���� --------------------------------------------------------------------
	float fovy = pCamera->GetFovY();
	m_FocalLen[0]      = 1.0f / tanf(fovy * 0.5f) *  (float)m_nHeightHalf / (float)m_nWidthHalf;
	m_FocalLen[1]      = 1.0f / tanf(fovy * 0.5f);
	m_InvFocalLen[0]   = 1.0f / m_FocalLen[0];
	m_InvFocalLen[1]   = 1.0f / m_FocalLen[1];
	m_InvResolution[0] = 1.0f / (float)m_nWidthHalf;
	m_InvResolution[1] = 1.0f / (float)m_nHeightHalf;
	m_Resolution[0]    = (float)m_nWidthHalf;
	m_Resolution[1]    = (float)m_nHeightHalf;

	m_scFocalLen.SetVector4     ( RVector4( m_FocalLen[0], m_FocalLen[1], 0, 0));
	m_scInvFocalLen.SetVector4  ( RVector4( m_InvFocalLen[0], m_InvFocalLen[1], 0, 0));
	m_scInvResolution.SetVector4( RVector4( m_InvResolution[0], m_InvResolution[1], 0, 0));
	m_scResolution.SetVector4   ( RVector4( m_Resolution[0], m_Resolution[1], 0, 0));
	m_scFarClipCornerInView.SetVector3(pCamera->GetFarClipCornerInView());

	// ��� ���� : AO ��ġ ���� --------------------------------------------------------------------
	m_scRadius.SetFloat( m_Radius);
	m_scRadiusInv.SetFloat( 1.0f/m_Radius);
	m_scRadiusSqr.SetFloat( m_Radius*m_Radius);
	float fAngleBias = m_AngleBias * 3.14159265f / 180.0f;
	m_scAngleBias.SetFloat( fAngleBias);
	m_scTanAngleBias.SetFloat( tan( fAngleBias));
	m_scContrast.SetFloat( m_Contrast);
	m_scAttenuation.SetFloat( m_Attenuation);
	m_scBrightBias.SetFloat( m_BrightBias);


#define AO_STAGE		0
#define SHADOW_STAGE	1
#define NOISE_STAGE		2
// --- �Լ� ���� ���� �ܺο��� ���� �Ǿ� �� �͵��̹Ƿ� �ǵ��� ����.
#define NORMAL_STAGE	4
#define DEPTH_STAGE		5
#define MASK_STAGE		0
// --- 

	UINT uiPass, uiPassCount;

	REngine::GetDevice().SetDepthEnable(false, false);
	REngine::GetDevice().SetCullMode(RCULL_CCW);

	REngine::GetDevice().SetTextureAddressWrap( NOISE_STAGE);
	REngine::GetDevice().SetTextureAddressClamp( DEPTH_STAGE);
	REngine::GetDevice().SetTextureAddressClamp( MASK_STAGE);
	REngine::GetDevice().SetTextureAddressClamp( NORMAL_STAGE);
	REngine::GetDevice().SetTextureAddressClamp( SHADOW_STAGE);

	// �켱�� AO������ ���� -------------------------------------------------------------------
	REngine::GetDevice().SetRenderTarget( 0, m_pTarget);
	m_pCurrentTarget = m_pTarget;
	// ���� ���ۿ� �븻 ���۴� �Լ� ���� ���� �ܺο��� ���� �Ǿ������Ƿ� ���ΰ� ������� �ε��� �Է� ����
	REngine::GetDevice().SetTexture( NOISE_STAGE, m_ptexNoise);
	REngine::GetDevice().SetTexture( SHADOW_STAGE, m_pTexShadow);
	REngine::GetDevice().SetTextureFilter( NOISE_STAGE, RTF_POINT);
	REngine::GetDevice().SetTextureFilter( DEPTH_STAGE, RTF_POINT);
	REngine::GetDevice().SetTextureFilter( NORMAL_STAGE, RTF_POINT);
	REngine::GetDevice().SetTextureFilter( MASK_STAGE, RTF_POINT);
	REngine::GetDevice().SetTextureFilter( SHADOW_STAGE, RTF_POINT);

	m_TechniqueAO.Begin(&uiPassCount, TECH_PRESERVE_NOT);
	for( uiPass = 0; uiPass < uiPassCount; ++uiPass) 
	{
		m_TechniqueAO.BeginPass(uiPass);
		RRenderHelper::RenderQuad(0, 0, (float)m_nWidthHalf, (float)m_nHeightHalf);
		m_TechniqueAO.EndPass();
	}
	m_TechniqueAO.End();
	


	if( m_bDenoise)
	{
		REngine::GetDevice().SetRenderTarget( 0, m_pTargetDenoise);
		m_pCurrentTarget = m_pTargetDenoise;
		REngine::GetDevice().SetTexture( AO_STAGE, m_pTarget);
		REngine::GetDevice().SetTextureAddressWrap( AO_STAGE);
		REngine::GetDevice().SetTextureFilter( AO_STAGE, RTF_LINEAR);
		m_TechniqueDenoise.Begin(&uiPassCount, TECH_PRESERVE_NOT);
		for( uiPass = 0; uiPass < uiPassCount; ++uiPass) 
		{
			m_TechniqueDenoise.BeginPass(uiPass);
			RRenderHelper::RenderQuad(0, 0, (float)m_nWidthHalf, (float)m_nHeightHalf);
			m_TechniqueDenoise.EndPass();
		}
		m_TechniqueDenoise.End();
		
	}

	// �����( Ȥ�� ����Ʈ ����)�� ���� �׷���� �Ǵ� ��Ȳ�� ���
	if( bRenderToBackBuffer)
	{
		REngine::GetDevice().SetRenderTarget( 0, pRT0);
		REngine::GetDevice().SetTexture( AO_STAGE, m_pCurrentTarget);
		REngine::GetDevice().SetTextureFilter( AO_STAGE, RTF_LINEAR	);
		RTechnique* pTech = NULL;
		if( m_bSubtract)
		{
			pTech = &m_TechniqueSubtract;
			// Ÿ���� �ؽ��� �ҽ��� ���. D3Ddebug ver.������ ��� ������� �켱�� ��¿ ��..
			REngine::GetDevice().SetTexture( SHADOW_STAGE, pRT0);
			REngine::GetDevice().SetTextureFilter( SHADOW_STAGE, RTF_POINT);
		}
		else
		{
			pTech = &m_TechniqueCopy;
			REngine::GetDevice().SetBlending( RBLEND_ZERO, RBLEND_SRCCOLOR);
		}

		pTech->Begin(&uiPassCount, TECH_PRESERVE_NOT);
		for( uiPass = 0; uiPass < uiPassCount; ++uiPass) 
		{
			pTech->BeginPass(uiPass);
			RRenderHelper::RenderQuad(0, 0, (float)nWidth, (float)nHeight);
			pTech->EndPass();
		}
		pTech->End();
	}

	// -------------------------------------- restore ----------------------------------
 	REngine::GetDevice().SetRenderTarget( 0, pRT0);
 	REngine::GetDevice().SetRenderTarget( 1, pRT1);
 	REngine::GetDevice().SetRenderTarget( 2, pRT2);
 	REngine::GetDevice().SetRenderTarget( 3, pRT3);

	REngine::GetDevice().SetTexture( 0, pTexOrg0);
	REngine::GetDevice().SetTexture( 1, pTexOrg1);
	REngine::GetDevice().SetTexture( 2, pTexOrg2);
	REngine::GetDevice().SetTexture( 3, pTexOrg3);
	REngine::GetDevice().SetTexture( 4, pTexOrg4);
	REngine::GetDevice().SetTexture( 5, pTexOrg5);
	REngine::GetDevice().SetTexture( 6, pTexOrg6);

// 	REngine::GetDevice().SetTextureFilter( 0, RTF_LINEAR);
// 	REngine::GetDevice().SetTextureFilter( 1, RTF_LINEAR);
// 	REngine::GetDevice().SetTextureFilter( 2, RTF_LINEAR);
// 	REngine::GetDevice().SetTextureFilter( 3, RTF_LINEAR);

	REngine::GetDevice().SetTextureAddressWrap( 0);
	REngine::GetDevice().SetTextureAddressWrap( 1);
	REngine::GetDevice().SetTextureAddressWrap( 2);
	REngine::GetDevice().SetTextureAddressWrap( 3);
	REngine::GetDevice().SetTextureAddressWrap( 4);
	REngine::GetDevice().SetTextureAddressWrap( 5);
	REngine::GetDevice().SetTextureAddressWrap( 6);

	REngine::GetDevice().SetBlending(RBLEND_NONE);
	REngine::GetDevice().SetDepthEnable(true, true);
	REngine::GetDevice().SetCullMode(RCULL_CW);
}

void RHBAOProcessor::CalcHalfResolution()
{
	// AO ó���� ������ Half-Res.
///	m_nWidthHalf = m_nWidth/2;
///	m_nHeightHalf = m_nHeight/2;

	m_nWidthHalf = m_nWidth;
	m_nHeightHalf = m_nHeight;

}

void RHBAOProcessor::Init( int nW, int nH)
{
	if( nW <= 0 || nH <= 0)
		return;

	RGIProcessor::Init( nW, nH);

	if(!m_bAllLight)
		m_bSubtract = false;	/// �𷺼ų� �������� ���� �� ��쿡�� ������ ���� ����

	CalcHalfResolution();

	bool bLowDenoise = false;
	int nSampleCount = 3;
	bool bUseRandomTextureSpray = true;
	m_bDenoise = false;
	switch( m_QualityMode)
	{
	case 1:
		nSampleCount = 3;	
		m_bDenoise = true;
		bLowDenoise = false;
		bUseRandomTextureSpray = false;
		break;
	case 2:
		nSampleCount = 4;	
		m_bDenoise = true;
		bLowDenoise = false;
		bUseRandomTextureSpray = false;
		break;
	case 3:
		nSampleCount = 6;	
		m_bDenoise = true;
		bLowDenoise = false;
		bUseRandomTextureSpray = false;
		break;
	}
	
	if(!m_Effect)
	{
		if ( false == REngine::GetDevice().QueryFeature(RQF_PS30) )
			return;

		vector<string> vShaderDefines;
		char szBuf[200];
		sprintf(szBuf, "%d", nSampleCount);

		vShaderDefines.push_back("NUM_SAMPLE");
		vShaderDefines.push_back(szBuf);
		
		if( IsShadowOnly())
		{
			vShaderDefines.push_back("SHADOWONLY");
			vShaderDefines.push_back("1");
		}

		if( bUseRandomTextureSpray)
		{
			vShaderDefines.push_back("RANDOMSPRAY");
			vShaderDefines.push_back("1");
		}

		m_Effect = REngine::GetDevice().CreateShaderFX("HBAO.fx", vShaderDefines);
		
		if( m_Effect)
		{
			m_TechniqueAO				= m_Effect->GetTechnique( "Tmain" );
			m_TechniqueDenoise			= m_Effect->GetTechnique( bLowDenoise ? "Tdenoise2x2" : "Tdenoise4x4");
			m_TechniqueCopy				= m_Effect->GetTechnique( "Tcopy" );
			m_TechniqueSubtract			= m_Effect->GetTechnique( "Tsubtract" );
			m_scFocalLen				= m_Effect->GetConstant( "g_FocalLen"      );
			m_scInvFocalLen				= m_Effect->GetConstant( "g_InvFocalLen"   );
			m_scInvResolution			= m_Effect->GetConstant( "g_InvResolution" );
			m_scResolution				= m_Effect->GetConstant( "g_Resolution"    );
			m_scRadius					= m_Effect->GetConstant( "g_R"    );
			m_scRadiusInv				= m_Effect->GetConstant( "g_inv_R"    );
			m_scRadiusSqr				= m_Effect->GetConstant( "g_sqr_R"    );
			m_scAngleBias				= m_Effect->GetConstant( "g_AngleBias"    );
			m_scTanAngleBias			= m_Effect->GetConstant( "g_TanAngleBias"    );
			m_scBrightBias				= m_Effect->GetConstant( "g_BrightBias"    );
			m_scAttenuation				= m_Effect->GetConstant( "g_Attenuation"    );
			m_scContrast				= m_Effect->GetConstant( "g_Contrast"    );
			m_scFarClipCornerInView		= m_Effect->GetConstant( "g_vFarClipCornerInView" );
			m_scFarZ					= m_Effect->GetConstant( "g_FarZ"    );
		}
	}

	if(!m_ptexNoise)
		m_ptexNoise = REngine::GetDevice().CreateTexture( "noisemono4x4.jpg", RTF_POINT, 0);
	if(!m_ptexNoise)
		m_ptexNoise = REngine::GetDevice().CreateTexture( "vector_noise.dds", RTF_POINT, 0);

	Restore();
}


void RHBAOProcessor::Destroy()
{
	RGIProcessor::Destroy();

	if (m_Effect)
		REngine::GetDevice().DeleteShaderFX(m_Effect);
	if( m_ptexNoise)
		REngine::GetDevice().DeleteTexture( m_ptexNoise);

	Invalidate();

	m_Effect = NULL;
	m_ptexNoise = NULL;
	m_pTarget = NULL;
	m_pTargetDenoise = NULL;
	m_pCurrentTarget = NULL;
}

void RHBAOProcessor::Resize( int nW, int nH )
{
	RGIProcessor::Resize( nW, nH );
	CalcHalfResolution();
}

void RHBAOProcessor::Invalidate()
{
	RGIProcessor::Invalidate();

	REngine::GetDevice().SafeDeleteTexture( m_pTarget );
	REngine::GetDevice().SafeDeleteTexture( m_pTargetDenoise );
}

void RHBAOProcessor::Restore()
{
	RGIProcessor::Restore();

	RFORMAT bufferFormat = RFMT_RGB16;
	if ( false == REngine::GetDevice().QueryFeature(RQF_RGB16_RTF))
		bufferFormat = RFMT_A8R8G8B8;

	if(!m_pTarget)
		m_pTarget = REngine::GetDevice().CreateRenderTargetTexture( m_nWidthHalf, m_nHeightHalf, bufferFormat);
	if( m_bDenoise && !m_pTargetDenoise)
		m_pTargetDenoise = REngine::GetDevice().CreateRenderTargetTexture( m_nWidthHalf, m_nHeightHalf, bufferFormat);
}


//-----------------------------------------------------------------------------
// Desc: Draw a properly aligned quad covering the entire render target
//-----------------------------------------------------------------------------
void RHBAOProcessor::DrawFullScreenQuad(float fLeftU, float fTopV, float fRightU, float fBottomV, int bSmallSize)
{
	REngine::GetDevice().SetFillMode(RFILL_SOLID);

	// Ensure that we're directly mapping texels to pixels by offset by 0.5
	// For more info see the doc page titled "Directly Mapping Texels to Pixels"
	float fWidth, fHeight;

	fWidth		= (float)m_nWidth;
	fHeight		= (float)m_nHeight;

	// Draw the quad
	RVt_pos_tex_rhw vQuad[4];
	vQuad[0].vPos	= RVector4( -0.5f, -0.5f, 0.0f, 1.0f );
	vQuad[0].u		= fLeftU;
	vQuad[0].v		= fTopV;

	vQuad[1].vPos	= RVector4( -0.5f, -0.5f + fHeight, 0.0f, 1.0f );
	vQuad[1].u		= fLeftU;
	vQuad[1].v		= fBottomV;

	vQuad[2].vPos	= RVector4( -0.5f + fWidth, -0.5f, 0.0f, 1.0f );
	vQuad[2].u		= fRightU;
	vQuad[2].v		= fTopV;

	vQuad[3].vPos	= RVector4( -0.5f + fWidth, -0.5f + fHeight, 0.0f, 1.0f );
	vQuad[3].u		= fRightU;
	vQuad[3].v		= fBottomV;

	REngine::GetDevice().SetDepthEnable(false, false);
	REngine::GetDevice().SetFvF(RVt_pos_tex_rhw::FVF);
	REngine::GetDevice().DrawPrimitiveUP( RPT_TRIANGLESTRIP, 2, vQuad, sizeof( RVt_pos_tex_rhw ) );
}

}

















// if(!m_bTargetImmediately)
// {
// 	// denoise ���� ------------------------------------------------------------------------
// 	REngine::GetDevice().SetRenderTarget( 0, pRT0);
// 	// ��� ����Ʈ ó���� �Ǿ� �ִ� ���ۿ� SSAO ����� ���� �ռ����� �׸� ���̴�.
// 	REngine::GetDevice().SetTexture( NOISE_STAGE, NULL);
// 	REngine::GetDevice().SetTexture( SHADOW_STAGE, NULL);
// 	REngine::GetDevice().SetTexture( AO_STAGE, m_pTarget);
// 	REngine::GetDevice().SetBlending( RBLEND_DESTCOLOR, RBLEND_ZERO);
// 	
// 	RTechnique* pTech = NULL;
// 	if( m_bDenoise)
// 	{
// 		pTech = &m_TechniqueDenoise;
// 		REngine::GetDevice().SetTextureFilter( AO_STAGE, RTF_POINT);
// 	}
// 	else
// 	{
// 		pTech = &m_TechniqueCopy;
// 		REngine::GetDevice().SetTextureFilter( AO_STAGE, RTF_LINEAR);
// 	}
// 
// 	pTech->Begin(&uiPassCount, 0);
// 	for( uiPass = 0; uiPass < uiPassCount; ++uiPass) 
// 	{
// 		pTech->BeginPass(uiPass);
// 		RRenderHelper::RenderQuad(0, 0, (float)nWidth, (float)nHeight);
// 		pTech->EndPass();
// 	}
// 	pTech->End();
// 
// 	REngine::GetDevice().SetTextureFilter( 0, RTF_LINEAR);
// 	RRenderHelper::EndRenderState();
// }