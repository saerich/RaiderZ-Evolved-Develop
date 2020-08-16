#include "StdAfx.h"
#include "RCameraAccumMotionBlurController.h"
#include "RCameraSceneNode.h"
#include "RVType.h"
#include "RShaderCompositeManager.h"


#pragma warning(disable:4800)


namespace rs3 {


static RTexture* pTargetOrg0 = NULL;
static RTexture* pTargetOrg1 = NULL;
static RTexture* pTargetOrg2 = NULL;
static RTexture* pTargetOrg3 = NULL;


class AccumMotionBlurControllerListener : public RListener
{
	RCameraAccumMotionBlurController* m_pController;
public:
	AccumMotionBlurControllerListener(RCameraAccumMotionBlurController* pController) : m_pController(pController) {}

	virtual void Update() override
	{
		m_pController->Destroy();
		m_pController->Create();
	}
};



RCameraAccumMotionBlurController::RCameraAccumMotionBlurController(  RCameraSceneNode* pCameraSceneNode )
	: RCameraSceneNodeController( RCCT_POSTRENDER, "3_ACCUM_MOTION_BLUR", true )
	, m_pCameraSceneNode(pCameraSceneNode)
	, m_pColorBuffer(NULL)
	, m_pColorBufferOrg(NULL)
	, m_pShader(NULL)
	, m_pTmpBuffer(NULL)
	, m_pRenderNode(NULL)
	, m_fDecreaseAlpha(0.2f)
	, m_dwTimeAccum(0)
	, m_dwWidth(0)
	, m_dwHeight(0)
	, m_dwWidthSmall(0)
	, m_dwHeightSmall(0)
	, m_fLastAlphaValue(0)
	, m_id(0)
{
	m_pAccumBuffer[0] = NULL;
	m_pAccumBuffer[1] = NULL;

	// �ʱⰪ�� false�� �ϰ� �׶� �׶� �ʿ�� active��Ű�� �ɷ� �Ѵ�.
	m_bActive	= true;
	m_bLockFlag = false;

	SetAccumRate( 0.02f);

	// event ����
	m_pEventListener = new AccumMotionBlurControllerListener(this);
	REngine::GetEventBroadcaster(RE_DEVICE_RESTORE)->AddListener( m_pEventListener );

	if( !Create())
		mlog3("Accum Motion Blur ī�޶� ���� ����!\n");
}

RCameraAccumMotionBlurController::~RCameraAccumMotionBlurController(void)
{
	// event ����
	REngine::GetEventBroadcaster(RE_DEVICE_RESTORE)->RemoveListener( m_pEventListener );

	SAFE_DELETE(m_pEventListener);

	Destroy();
}



bool RCameraAccumMotionBlurController::Create()
{
	// �ӵ� ������ RFMT_A16B16G16R16F(64bit)�� ��� ���� �ʰ� 32bit¥�� ��� 
	// 64bit¥���� ��� �ϴ� HDR�� ������ �� ���� �ؾ� �� ���̴�..
	RFORMAT eBufferFormat = RFMT_A8R8G8B8;

	m_dwWidth	= (DWORD)REngine::GetDevice().GetScreenWidth();
	m_dwHeight	= (DWORD)REngine::GetDevice().GetScreenHeight();

	m_dwWidthSmall	= m_dwWidth / 2;
	m_dwHeightSmall	= m_dwHeight / 2;


	if ( false == REngine::GetDevice().QueryFeature(RQF_PS20) )
		return false;

	m_pColorBuffer = REngine::GetDevice().CreateRenderTargetTexture( m_dwWidth, m_dwHeight, eBufferFormat);
	if(R_NONE==m_pColorBuffer) return false;
	m_pTmpBuffer = REngine::GetDevice().CreateRenderTargetTexture( m_dwWidth, m_dwHeight, eBufferFormat);
	if(R_NONE==m_pTmpBuffer) return false;
	m_pAccumBuffer[0] = REngine::GetDevice().CreateRenderTargetTexture( m_dwWidth, m_dwHeight, eBufferFormat);
	if(R_NONE==m_pAccumBuffer[0]) return false;
	REngine::GetDevice().ColorFill( m_pAccumBuffer[0]);
	m_pAccumBuffer[1] = REngine::GetDevice().CreateRenderTargetTexture( m_dwWidth, m_dwHeight, eBufferFormat);
	if(R_NONE==m_pAccumBuffer[1]) return false;
	REngine::GetDevice().ColorFill( m_pAccumBuffer[1]);
	
	m_pShader = REngine::GetDevice().CreateShaderFX("AccumMotionBlur.fx");

	m_fLastAlphaValue = 0;

	return true;
}

void RCameraAccumMotionBlurController::Destroy()
{
	REngine::GetDevicePtr()->DeleteTexture( m_pColorBuffer);
	REngine::GetDevicePtr()->DeleteTexture( m_pTmpBuffer);
	REngine::GetDevicePtr()->DeleteTexture( m_pAccumBuffer[0]);
	REngine::GetDevicePtr()->DeleteTexture( m_pAccumBuffer[1]);

	if (m_pShader)
		REngine::GetDevice().DeleteShaderFX(m_pShader);

	m_bLockFlag = false;
	m_fLastAlphaValue = 0;
}


void RCameraAccumMotionBlurController::Restore()
{
	REngine::GetDevice().ColorFill( m_pColorBuffer);
	REngine::GetDevice().ColorFill( m_pTmpBuffer);
	REngine::GetDevice().ColorFill( m_pAccumBuffer[0]);
	REngine::GetDevice().ColorFill( m_pAccumBuffer[1]);

	m_bLockFlag = false;
	m_fLastAlphaValue = 0;
}


// ���� ���� ���۷� ���� �ʰ� ���� ���۷� �Ѵ�.
bool RCameraAccumMotionBlurController::BeginRender( RSceneNode* pActorNode)
{
	m_pRenderNode = pActorNode;

	pTargetOrg0 = REngine::GetDevice().GetRenderTarget(0);
	pTargetOrg1 = REngine::GetDevice().GetRenderTarget(1);
	pTargetOrg2 = REngine::GetDevice().GetRenderTarget(2);
	pTargetOrg3 = REngine::GetDevice().GetRenderTarget(3);

	REngine::GetDevice().SetRenderTarget( 0, m_pTmpBuffer);
	REngine::GetDevice().SetRenderTarget( 1, NULL);
	REngine::GetDevice().SetRenderTarget( 2, NULL);
	REngine::GetDevice().SetRenderTarget( 3, NULL);

	return true;
}

bool RCameraAccumMotionBlurController::EndRender( RSceneNode* pActorNode)
{
	_ASSERT( m_pRenderNode == pActorNode);
	m_pRenderNode = NULL;

	REngine::GetDevice().SetRenderTarget( 0, pTargetOrg0);
	REngine::GetDevice().SetRenderTarget( 1, pTargetOrg1);
	REngine::GetDevice().SetRenderTarget( 2, pTargetOrg2);
	REngine::GetDevice().SetRenderTarget( 3, pTargetOrg3);

	m_fLastAlphaValue = 1;

	return true;
}


//-----------------------------------------------------------------------------
// Desc: Draw a properly aligned quad covering the entire render target
//-----------------------------------------------------------------------------
void RCameraAccumMotionBlurController::DrawFullScreenQuad(float fLeftU, float fTopV, float fRightU, float fBottomV, int bSmallSize)
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
void RCameraAccumMotionBlurController::Render()
{
	// Ȱ��ȭ �ø� ó��
	if( !m_bActive)
		return;

	FrameMove();

	// 2D ������ ���� ���� ����
	REngine::GetDevice().SetTextureFilter( 0, RTF_POINT );
	REngine::GetDevice().SetTextureFilter( 1, RTF_POINT );
	REngine::GetDevice().SetTextureFilter( 2, RTF_POINT );

	REngine::GetDevice().SetBlending(RBLEND_NONE, RBLEND_NONE);
	REngine::GetDevice().SetAlphaTestEnable(false);

	REngine::GetDevice().SetDepthEnable( false, false);

	REngine::GetDevice().SetTextureAddressClamp( 0);
	REngine::GetDevice().SetTextureAddressClamp( 1);
	REngine::GetDevice().SetTextureAddressClamp( 2);

	RTechnique Technique;
	UINT uiPass, uiPassCount;

	// ���� �ֱ⸶�� ���� ���۸� �����Ѵ�.
	if( m_dwTimeAccum > m_dwAccumRate)
	{
		m_dwTimeAccum = 0;

		// �������ۿ� ���� �ӽ� ����(������ ������� ���)�� ������Ų��. ------------------------------------------------
		// �� ������ �� �з��־ �� ���̹Ƿ� ���� ȭ�鿡 ���� �� ���� ��Ų��.
		RTexture* pOrg = REngine::GetDevice().GetRenderTarget(0);
		REngine::GetDevice().SetRenderTarget( 0, m_pAccumBuffer[(int)(1-m_id)]);

		Technique = m_pShader->GetTechnique("Accum");
		m_pShader->GetConstant("g_fDecreaseAlpha").SetFloat( m_fDecreaseAlpha);

		REngine::GetDevice().SetTexture(0, m_pAccumBuffer[(int)m_id]);
		REngine::GetDevice().SetTexture(1, m_pTmpBuffer);
		REngine::GetDevice().SetTexture(2, m_pColorBuffer);

		Technique.Begin(&uiPassCount, 0);
		for (uiPass = 0; uiPass < uiPassCount; ++uiPass) 
		{
			Technique.BeginPass(uiPass);

			DrawFullScreenQuad( 0.0f, 0.0f, 1.0f, 1.0f, false);

			Technique.EndPass();
		}
		Technique.End();
		REngine::GetDevice().SetRenderTarget( 0, pOrg);

		m_fLastAlphaValue -= m_fDecreaseAlpha;

		// idx swap
		m_id = (bool)(1-m_id);
	}

	// ���� ������ ������ ���� ȭ�鿡 �ݿ��Ͽ� ���� ��. -----------------------------------------------------------------------
	Technique = m_pShader->GetTechnique("Blur");
	REngine::GetDevice().SetTexture(0, m_pColorBuffer);
	REngine::GetDevice().SetTexture(1, m_pAccumBuffer[(int)m_id]);
	REngine::GetDevice().SetTexture(2, NULL);
	Technique.Begin(&uiPassCount, 0);
	for (uiPass = 0; uiPass < uiPassCount; ++uiPass) 
	{
		Technique.BeginPass(uiPass);

		DrawFullScreenQuad( 0.0f, 0.0f, 1.0f, 1.0f, false );

		Technique.EndPass();
	}
	Technique.End();

	// ���� ���� ���� ----------------------------------------------------------------------------------------------------------
	REngine::GetDevice().SetTextureFilter( 0, RTF_LINEAR );
	REngine::GetDevice().SetTextureFilter( 1, RTF_LINEAR );
	REngine::GetDevice().SetTextureFilter( 2, RTF_LINEAR );
	REngine::GetDevice().SetTextureAddressWrap( 0);
	REngine::GetDevice().SetTextureAddressWrap( 1);
	REngine::GetDevice().SetTextureAddressWrap( 2);
	REngine::GetDevice().SetTexture( 0, NULL);
	REngine::GetDevice().SetTexture( 1, NULL);
	REngine::GetDevice().SetTexture( 2, NULL);

	// �ӽ� ���� �׸� �����ش�.
	REngine::GetDevice().ColorFill( m_pTmpBuffer);

	m_dwTimeAccum += REngine::GetTimer().GetElapsedTime();
}


//-----------------------------------------------------------------------------
// Name: RenderOverlay()
// Desc: Visializes one of the surfaces in the overlay
//-----------------------------------------------------------------------------
void RCameraAccumMotionBlurController::RenderOverlay()
{
	// Ȱ��ȭ �ø� ó��
	if( !m_bActive)
		return;

	RTechnique Technique = m_pShader->GetTechnique( "Overlay");

	REngine::GetDevice().SetTexture(0, m_pAccumBuffer[(int)m_id]);

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

void RCameraAccumMotionBlurController::OnRender()
{
	// Ȱ��ȭ �ø� ó��
	if(!m_bActive)
		return;

	REngine::GetDevice().SetFogEnable(false);

	Render();

	//RenderOverlay();

}


void RCameraAccumMotionBlurController::OnPostRender()
{
	// Ȱ��ȭ �ø� ó��
	if(!m_bActive)
		return;

	REngine::GetDevice().SetRenderTarget( 0, m_pColorBufferOrg );

	OnRender();

	m_bLockFlag = false;

	m_id = (bool)(1 - m_id);

}


// ���� Ÿ���� ���� - 090617, OZ
void RCameraAccumMotionBlurController::OnPreRender( RInferredRenderingStrategy* pInferredRenderingStrategy)
{
	RCameraSceneNodeController::OnPreRender( pInferredRenderingStrategy);

	// Ȱ��ȭ �ø� ó��
	if(!m_bActive)
		return;

	// ���� RT ���
	m_pColorBufferOrg = REngine::GetDevice().GetRenderTarget(0);
	// �� RT ����
	REngine::GetDevice().SetRenderTarget( 0, m_pColorBuffer );

	// ���� Ÿ���� �����ߴ�. ��Ƽ�� �÷��װ� ���� �Ǹ� �ȵȴ�.
	m_bLockFlag = true;
}


void RCameraAccumMotionBlurController::OnRemoveFromCamera()
{
}

void RCameraAccumMotionBlurController::OnClear()
{
	REngine::GetDevice().Clear( true, true, 
		m_pCameraSceneNode->isHaveStencil(),
		m_pCameraSceneNode->GetClearColor(),
		m_pCameraSceneNode->GetClearDepth(),
		m_pCameraSceneNode->GetClearStencil() );
}

void RCameraAccumMotionBlurController::FrameMove()
{
}





}