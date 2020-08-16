#include "StdAfx.h"
#include "RCameraPixelMotionBlurController.h"
#include "RCameraSceneNode.h"
#include "RVType.h"
#include "RShaderCompositeManager.h"



namespace rs3 {

static RTexture* pTargetOrg0 = NULL;
static RTexture* pTargetOrg1 = NULL;
static RTexture* pTargetOrg2 = NULL;
static RTexture* pTargetOrg3 = NULL;


class PixelMotionBlurControllerListener : public RListener
{
	RCameraPixelMotionBlurController* m_pController;
public:
	PixelMotionBlurControllerListener(RCameraPixelMotionBlurController* pController) : m_pController(pController) {}

	virtual void Update() override
	{
		m_pController->Destroy();
		m_pController->Create();
	}
};



RCameraPixelMotionBlurController::RCameraPixelMotionBlurController(  RCameraSceneNode* pCameraSceneNode )
	: RCameraSceneNodeController( RCCT_POSTRENDER, "4_Pixel_MOTION_BLUR", true )
	, m_pCameraSceneNode(pCameraSceneNode)
	//, m_pColorBuffer(NULL)
	, m_pColorBufferOrg(NULL)
	, m_pShader(NULL)
	, m_pVelocityBuffer(NULL)
	, m_pRenderNode(NULL)
	, m_dwWidth(0)
	, m_dwHeight(0)
	, m_dwWidthSmall(0)
	, m_dwHeightSmall(0)
	, m_nSamplerCnt(15)
	, m_nImgSamCnt(1)
{
	m_pColorBuffer[0] = NULL;
	m_pColorBuffer[1] = NULL;

	// �ʱⰪ�� false�� �ϰ� �׶� �׶� �ʿ�� active��Ű�� �ɷ� �Ѵ�.
	m_bActive	= true;
	m_bLockFlag = false;
	// 1/65535�� �ص� z-fight�Ͼ��. �� �� �����ִ� ��ġ��.
	m_fBias	= (-1.f / 65535.f) * 10;

	// event ����
	m_pEventListener = new PixelMotionBlurControllerListener(this);
	REngine::GetEventBroadcaster(RE_DEVICE_RESTORE)->AddListener( m_pEventListener );

	m_nImgSamCnt = 1;

	if( !Create())
		mlog3("Pixel Motion Blur ī�޶� ���� ����!\n");
}

RCameraPixelMotionBlurController::~RCameraPixelMotionBlurController(void)
{
	// event ����
	REngine::GetEventBroadcaster(RE_DEVICE_RESTORE)->RemoveListener( m_pEventListener );

	SAFE_DELETE(m_pEventListener);

	Destroy();
}



bool RCameraPixelMotionBlurController::Create()
{
	// �ӵ� ������ RFMT_A16B16G16R16F(64bit)�� ��� ���� �ʰ� 32bit¥�� ��� 
	// 64bit¥���� ��� �ϴ� HDR�� ������ �� ���� �ؾ� �� ���̴�..
	RFORMAT eColorBufferFormat = RFMT_A8R8G8B8;
	// �ӵ� ���۴� ��ũ�� ��ǥ�� �ӵ��� ���̰� �ȴ�. xy�� ������Ʈ�� ��ȿ�� �Ǽ��� ���۷�..
	RFORMAT eVelocityBufferFormat = RFMT_G16R16F;
	//RFORMAT eVelocityBufferFormat = RFMT_A8R8G8B8;

	m_dwWidth	= (DWORD)REngine::GetDevice().GetScreenWidth();
	m_dwHeight	= (DWORD)REngine::GetDevice().GetScreenHeight();

	m_dwWidthSmall	= m_dwWidth / 2;
	m_dwHeightSmall	= m_dwHeight / 2;

	if ( false == REngine::GetDevice().QueryFeature(RQF_PS20) )
		return false;

	if ( false == REngine::GetDevice().QueryFeature(RQF_PS30) )
		return false;

	m_pColorBuffer[0] = REngine::GetDevice().CreateRenderTargetTexture( m_dwWidth, m_dwHeight, eColorBufferFormat);
	if(R_NONE==m_pColorBuffer[0]) return false;
	m_pColorBuffer[1] = REngine::GetDevice().CreateRenderTargetTexture( m_dwWidth, m_dwHeight, eColorBufferFormat);
	if(R_NONE==m_pColorBuffer[1]) return false;
	m_pVelocityBuffer = REngine::GetDevice().CreateRenderTargetTexture( m_dwWidth, m_dwHeight, eVelocityBufferFormat);
	if(R_NONE==m_pVelocityBuffer) return false;
	REngine::GetDevice().ColorFill( m_pVelocityBuffer);

	m_pShader = REngine::GetDevice().CreateShaderFX("ObjectMotionBlur.fx");

	return true;
}

void RCameraPixelMotionBlurController::Destroy()
{
	REngine::GetDevicePtr()->DeleteTexture( m_pColorBuffer[0]);
	REngine::GetDevicePtr()->DeleteTexture( m_pColorBuffer[1]);
	REngine::GetDevicePtr()->DeleteTexture( m_pVelocityBuffer);

	if (m_pShader)
		REngine::GetDevice().DeleteShaderFX(m_pShader);

	m_bLockFlag = false;
}


void RCameraPixelMotionBlurController::Restore()
{
	REngine::GetDevice().ColorFill( m_pColorBuffer[0]);
	REngine::GetDevice().ColorFill( m_pColorBuffer[1]);
	REngine::GetDevice().ColorFill( m_pVelocityBuffer);

	m_bLockFlag = false;
}


// ���� ���� ���۷� ���� �ʰ� ���� ���۷� �Ѵ�.
bool RCameraPixelMotionBlurController::BeginRender( RSceneNode* pActorNode)
{
	m_pRenderNode = pActorNode;

	pTargetOrg0 = REngine::GetDevice().GetRenderTarget(0);
	pTargetOrg1 = REngine::GetDevice().GetRenderTarget(1);
	pTargetOrg2 = REngine::GetDevice().GetRenderTarget(2);
	pTargetOrg3 = REngine::GetDevice().GetRenderTarget(3);

	REngine::GetDevice().SetRenderTarget( 0, m_pVelocityBuffer);
	REngine::GetDevice().SetRenderTarget( 1, NULL);
	REngine::GetDevice().SetRenderTarget( 2, NULL);
	REngine::GetDevice().SetRenderTarget( 3, NULL);

	REngine::GetDevice().SetDepthBias( m_fBias);

	return true;
}

bool RCameraPixelMotionBlurController::EndRender( RSceneNode* pActorNode)
{
	_ASSERT( m_pRenderNode == pActorNode);
	m_pRenderNode = NULL;

	REngine::GetDevice().SetRenderTarget( 0, pTargetOrg0);
	REngine::GetDevice().SetRenderTarget( 1, pTargetOrg1);
	REngine::GetDevice().SetRenderTarget( 2, pTargetOrg2);
	REngine::GetDevice().SetRenderTarget( 3, pTargetOrg3);

	REngine::GetDevice().SetDepthBias( 0);

	return true;
}


//-----------------------------------------------------------------------------
// Desc: Draw a properly aligned quad covering the entire render target
//-----------------------------------------------------------------------------
void RCameraPixelMotionBlurController::DrawFullScreenQuad(float fLeftU, float fTopV, float fRightU, float fBottomV, int bSmallSize)
{
	REngine::GetDevice().SetFillMode(RFILL_SOLID);

	// Ensure that we're directly mapping texels to pixels by offset by 0.5
	// For more info see the doc page titled "Directly Mapping Texels to Pixels"
	float fWidth, fHeight;

	if( bSmallSize == 1)
	{
		fWidth		= 256;
		fHeight		= 256;
	}
	else if( bSmallSize == -1)
	{
		fWidth		= (float)m_dwWidthSmall;
		fHeight		= (float)m_dwHeightSmall;
	}
	else
	{
		fWidth		= (float)m_dwWidth;
		fHeight		= (float)m_dwHeight;
	}

	// Draw the quad
	RVt_pos_tex_rhw vQuad[4];
	vQuad[0].vPos	= RVector4( -0.5f, -0.5f, 0.0f, 1.0f );
	vQuad[0].u		= fLeftU;
	vQuad[0].v		= fTopV;

	vQuad[1].vPos	= RVector4( -0.5f, fHeight, 0.0f, 1.0f );
	vQuad[1].u		= fLeftU;
	vQuad[1].v		= fBottomV;

	vQuad[2].vPos	= RVector4( fWidth, -0.5f, 0.0f, 1.0f );
	vQuad[2].u		= fRightU;
	vQuad[2].v		= fTopV;

	vQuad[3].vPos	= RVector4( fWidth, fHeight, 0.0f, 1.0f );
	vQuad[3].u		= fRightU;
	vQuad[3].v		= fBottomV;

	REngine::GetDevice().SetDepthEnable(false, false);
	REngine::GetDevice().SetFvF(RVt_pos_tex_rhw::FVF);
	REngine::GetDevice().DrawPrimitiveUP( RPT_TRIANGLESTRIP, 2, vQuad, sizeof( RVt_pos_tex_rhw ) );
	REngine::GetDevice().SetDepthEnable(true, true);
}


//-----------------------------------------------------------------------------
// Name: RenderDOF()
// Desc: Renders Blur effect
//-----------------------------------------------------------------------------
void RCameraPixelMotionBlurController::Render()
{
	// Ȱ��ȭ �ø� ó��
	if( !m_bActive)
		return;

	FrameMove();

	// 2D ������ ���� ���� ����
	REngine::GetDevice().SetTextureFilter( 0, RTF_POINT );
	REngine::GetDevice().SetTextureFilter( 1, RTF_POINT );

	REngine::GetDevice().SetBlending(RBLEND_NONE, RBLEND_NONE);
	REngine::GetDevice().SetAlphaTestEnable(false);

	REngine::GetDevice().SetDepthEnable( false, false);

	REngine::GetDevice().SetTextureAddressClamp( 0);
	REngine::GetDevice().SetTextureAddressClamp( 1);

	RTechnique Technique;
	UINT uiPass, uiPassCount;

	RTexture* pTargetOrg = REngine::GetDevice().GetRenderTarget(0);
	
	Technique = m_pShader->GetTechnique("Blur");

	m_pShader->GetConstant("g_numSamples").SetInt( m_nSamplerCnt);
	switch( m_nImgSamCnt)
	{
		case 2:		m_pShader->GetConstant("g_fDevideVelocity").SetFloat( 0.6f); break;
		case 3:		m_pShader->GetConstant("g_fDevideVelocity").SetFloat( 0.45f); break;
		default:	m_pShader->GetConstant("g_fDevideVelocity").SetFloat( 1.0f/(float)m_nImgSamCnt);  break;
	}

	REngine::GetDevice().SetTexture(1, m_pVelocityBuffer);

	// nImgSamCnt�� �ּ� 1 �̻�
	for( int i=0; i<m_nImgSamCnt; i++)
	{
		int nSrcIdx = i%2;
		int nDestIdx = 1 - (i%2);
		if( i == m_nImgSamCnt-1)
		{
			REngine::GetDevice().SetRenderTarget( 0, pTargetOrg);
		}
		else
		{
			REngine::GetDevice().SetRenderTarget( 0, m_pColorBuffer[ nDestIdx]);
		}
		REngine::GetDevice().SetTexture(0, m_pColorBuffer[ nSrcIdx]);
		Technique.Begin(&uiPassCount, 0);
		for (uiPass = 0; uiPass < uiPassCount; ++uiPass) 
		{
			Technique.BeginPass(uiPass);

			DrawFullScreenQuad( 0.0f, 0.0f, 1.0f, 1.0f, false );

			Technique.EndPass();
		}
		Technique.End();
	}

	// ���� ���� ���� ----------------------------------------------------------------------------------------------------------
	REngine::GetDevice().SetTextureFilter( 0, RTF_LINEAR );
	REngine::GetDevice().SetTextureFilter( 1, RTF_LINEAR );
	REngine::GetDevice().SetTextureAddressWrap( 0);
	REngine::GetDevice().SetTextureAddressWrap( 1);
	REngine::GetDevice().SetTexture( 0, NULL);
	REngine::GetDevice().SetTexture( 1, NULL);
}


//-----------------------------------------------------------------------------
// Name: RenderOverlay()
// Desc: Visializes one of the surfaces in the overlay
//-----------------------------------------------------------------------------
void RCameraPixelMotionBlurController::RenderOverlay()
{
	// Ȱ��ȭ �ø� ó��
	if( !m_bActive)
		return;

	RTechnique Technique = m_pShader->GetTechnique( "Overlay");

	REngine::GetDevice().SetTexture(0, m_pVelocityBuffer);

	REngine::GetDevice().SetBlending(RBLEND_NONE, RBLEND_NONE);
	REngine::GetDevice().SetAlphaTestEnable(false);

	REngine::GetDevice().SetDepthEnable( false, false);

	UINT uiPass, uiPassCount;
	Technique.Begin(&uiPassCount, 0);
	for (uiPass = 0; uiPass < uiPassCount; ++uiPass) 
	{
		Technique.BeginPass(uiPass);

		DrawFullScreenQuad( 0.0f, 0.0f, 1.0f, 1.0f, 1);

		Technique.EndPass();
	}
	Technique.End();

	REngine::GetDevice().SetTexture(0, NULL);

}

void RCameraPixelMotionBlurController::OnRender()
{
	// Ȱ��ȭ �ø� ó��
	if(!m_bActive)
		return;

	REngine::GetDevice().SetFogEnable(false);

	Render();

	//TESTOZ
	//RenderOverlay();

	// �ӵ� ���۸� ����ش�.
	REngine::GetDevice().ColorFill( m_pVelocityBuffer);
}


void RCameraPixelMotionBlurController::OnPostRender()
{
	// Ȱ��ȭ �ø� ó��
	if(!m_bActive)
		return;

	REngine::GetDevice().SetRenderTarget( 0, m_pColorBufferOrg );

	// RT1�� �������� �����Ƿ� NULL���� �ߴٰ� �����ش�.
	RTexture* pRT1 = REngine::GetDevice().GetRenderTarget( 1);
	REngine::GetDevice().SetRenderTarget( 1, NULL);

	OnRender();

	// RT1�� �������Ƽ� �ٸ� ī�޶� ��Ʈ�ѷ��� RT1 �۵��� ����� �ǰ� �Ѵ�.
	REngine::GetDevice().SetRenderTarget( 1, pRT1);

	m_bLockFlag = false;

}


// ���� Ÿ���� ���� - 090617, OZ
void RCameraPixelMotionBlurController::OnPreRender( RInferredRenderingStrategy* pInferredRenderingStrategy)
{
	RCameraSceneNodeController::OnPreRender( pInferredRenderingStrategy);

	// Ȱ��ȭ �ø� ó��
	if(!m_bActive)
		return;

	// ���� RT ���
	m_pColorBufferOrg = REngine::GetDevice().GetRenderTarget(0);
	// �� RT ����
	REngine::GetDevice().SetRenderTarget( 0, m_pColorBuffer[0] );

	// ���� Ÿ���� �����ߴ�. ��Ƽ�� �÷��װ� ���� �Ǹ� �ȵȴ�.
	m_bLockFlag = true;
}


void RCameraPixelMotionBlurController::OnRemoveFromCamera()
{
}

void RCameraPixelMotionBlurController::OnClear()
{
	REngine::GetDevice().Clear( true, true, 
		m_pCameraSceneNode->isHaveStencil(),
		m_pCameraSceneNode->GetClearColor(),
		m_pCameraSceneNode->GetClearDepth(),
		m_pCameraSceneNode->GetClearStencil() );
}

void RCameraPixelMotionBlurController::FrameMove()
{
}





}