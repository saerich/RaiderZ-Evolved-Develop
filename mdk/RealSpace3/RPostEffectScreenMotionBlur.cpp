#include "StdAfx.h"
#include "RPostEffectScreenMotionBlur.h"
#include "RCameraSceneNode.h"
#include "RVType.h"
#include "RShaderCompositeManager.h"
#include "RTechnique.h"
#include "RShaderFX.h"


namespace rs3 {



RPostEffectScreenMotionBlur::RPostEffectScreenMotionBlur()
: RPostEffectInterface( PE_SCREEN_MOTION_BLUR, RRP_NONE )
, m_pShader(NULL)
, m_nType(SMB_BY_CAM_MOVE)
, m_dwScatStartTime(0)
{

	m_dwWidth = 0;
	m_dwHeight = 0;

	m_nSamplerCnt = 10;
	m_fSpeed = 5;

	//m_matPrevVP = m_pCameraSceneNode->GetViewProjectionMatrix();
	//m_matPrevVP.GetInverse( &m_matInvVP);

	m_invalidateRotate = true;

	m_vEpicenter = RVector4( 0.5f, 0.5f, 0, 0);

	// ���������� ��Ʈ������ ���� ���� ����
	//m_matPrevVP = m_pCameraSceneNode->GetViewProjectionMatrix();
	//m_matPrevV = m_pCameraSceneNode->GetViewMatrix();
	//m_vPrePos = m_pCameraSceneNode->GetPosition();
	//m_vPreUp = m_pCameraSceneNode->GetUp();
	//m_vPreDir = m_pCameraSceneNode->GetDirection();

}

RPostEffectScreenMotionBlur::~RPostEffectScreenMotionBlur(void)
{
}






bool RPostEffectScreenMotionBlur::Init( int nW, int nH)
{
	if(!RPostEffectInterface::Init(nW, nH))
		return false;
	
	RFORMAT eBufferFormat = RFMT_A8R8G8B8;

	if( false == REngine::GetDevice().QueryFeature(RQF_PS20) )
		return false;
	if( false == REngine::GetDevice().QueryFeature(RQF_PS30) )
		return false;

	if(!m_pShader)
		m_pShader = REngine::GetDevice().CreateShaderFX("ScreenMotionBlur.fx");

	return true;
}

void RPostEffectScreenMotionBlur::Destroy()
{
	RPostEffectInterface::Destroy();

	if (m_pShader)
		REngine::GetDevice().DeleteShaderFX(m_pShader);
	m_pShader = NULL;
}

//-----------------------------------------------------------------------------
// Name: RenderDOF()
// Desc: Renders Blur effect
//-----------------------------------------------------------------------------
void RPostEffectScreenMotionBlur::Render( RCameraSceneNode* pCamera, RTexture* pColorBuffer, RTexture* pNormalBuffer, RTexture* pDepthBuffer)
{
	RPFC_THISFUNC;

	FrameMove();

	RTexture* pRT1 = NULL;
	// ī�޶� �̵� ����� ��츸 ���� ������ ����Ѵ�. ��������忡���� RT1�� NULL�� �صξ��ٰ� �������´�.
	if( m_nType == SMB_BY_EPICENTER)
	{
		pRT1 = REngine::GetDevice().GetRenderTarget( 1);
		REngine::GetDevice().SetRenderTarget( 1, NULL);
	}

	REngine::GetDevice().SetTexture(0, NULL);
	REngine::GetDevice().SetTexture(1, NULL);
	REngine::GetDevice().SetTexture(2, NULL);
	REngine::GetDevice().SetTexture(3, NULL);
	REngine::GetDevice().SetTexture(4, NULL);
	REngine::GetDevice().SetTexture(5, NULL);


	RTechnique Technique;
	switch( m_nType)
	{
	case SMB_BY_CAM_MOVE: Technique = m_pShader->GetTechnique("ScreenMotionBlurBasisCamMove");	break;
	case SMB_BY_EPICENTER: Technique = m_pShader->GetTechnique("ScreenMotionBlurBasisEpicenter");	break;
	}
	
	REngine::GetDevice().SetTexture(0, pColorBuffer);
	// ī�޶� �̵� ����� ��츸 ���� ������ ����Ѵ�.
	if( m_nType == SMB_BY_CAM_MOVE)
		REngine::GetDevice().SetTexture(1, pDepthBuffer);
	
	if( m_nType == SMB_BY_CAM_MOVE)
	{
		// ���������Ǹ�Ʈ������ �״�� ���� �ʰ� ��ġ�� (0,0,0)���� �����. 
		RMatrix matDownScaleV;
		RVector3 vDownScalePos(0,0,0);
		RVector3 vDownScaleDir = pCamera->GetDirection();
		RVector3 vDownScaleUp = pCamera->GetUp();
		matDownScaleV.SetLookAtMatrixRH( vDownScalePos, vDownScalePos+vDownScaleDir, vDownScaleUp);
		matDownScaleV.GetInverse( &m_matInvV);
		

		// ���� ��Ʈ������ �״�� ���� �ϸ� �����Ӹ����� ���� �� ������ �������� �ʾƼ�
		// �̹� ��Ʈ������ �״�� ��� ���� �ʰ� ���� �� ȸ�� ������ �̾Ƴ����� ����
		// �������� ��Ʈ������ ������ ������ ������ �����Ƿ�( ���Ѵٸ� ��!! ����.. �����ȵ�.. �Ĺ���!)
		// ���Ʈ������ ������.

		// TODO: �������� ƥ��� 1�ʰ� GetFrameRate �� ������ �ȵǱ� ������ �̻������ϴ�. ������ �ʿ��غ��Դϴ�
		float fSlerp = REngine::GetDevice().GetFrameRate() / 30.0f;

		// ���ٷ� �� �ִ� �ͺ��� ������ ���鿡�� �̰� ����..
		// pos
		RVector3 vPos = m_vPrePos - pCamera->GetPosition();
		vPos *= fSlerp;
		//vPos += m_pCameraSceneNode->GetPosition();
		vPos += vDownScalePos;
		// dir
		RVector3 vDir =  m_vPreDir - pCamera->GetDirection();
		vDir *= fSlerp;
		vDir += pCamera->GetDirection();
		// up
		RVector3 vUp =  m_vPreUp - pCamera->GetUp();
		vUp *= fSlerp;
		vUp += pCamera->GetUp();


		// ������� ��ġ ������ ���Ʈ���� ����
		RMatrix matMaedenPervView;
		if( m_invalidateRotate)
			// ȸ�� ������ ��ȿȭ �ϴ� ���� ������ ������ ��ġ�� ���� ���� ����
			matMaedenPervView.SetLookAtMatrixRH( vPos, vPos + pCamera->GetDirection(), pCamera->GetUp() );
		else
			matMaedenPervView.SetLookAtMatrixRH( vPos, vPos+vDir, vUp );
		// ���� ������� ���� ������ �� �������� ��Ʈ����
		m_matPrevVP = matMaedenPervView * pCamera->GetProjectionMatrix();

		m_pShader->GetConstant("g_previousViewProjectionMatrix").SetMatrix( m_matPrevVP);
		m_pShader->GetConstant("g_ViewInverseMatrix").SetMatrix(m_matInvV);

		RMatrix matView = pCamera->GetViewMatrix();
		RVector3 vP = pCamera->GetViewFrustum().GetPoints()[0];
		matView.TransformVect(vP);
		m_pShader->GetConstant("g_vFarClipCornerInView").SetVector3(RVector3(abs(vP.x),-abs(vP.y),vP.z));
	}
		
	m_pShader->GetConstant("g_numSamples").SetInt( m_nSamplerCnt);
	m_pShader->GetConstant("g_fFarZ").SetFloat( pCamera->GetFarZ());
	m_pShader->GetConstant("g_fSpeed").SetFloat( m_nType == SMB_BY_CAM_MOVE ? m_fSpeed : m_fSpeed * 0.01f);
	m_pShader->GetConstant("g_vEpicenter").SetVector4( m_vEpicenter);

	REngine::GetDevice().SetTextureFilter( 0, RTF_POINT );
	REngine::GetDevice().SetTextureFilter( 1, RTF_POINT );

	REngine::GetDevice().SetDepthEnable( false, false);

	REngine::GetDevice().SetTextureAddressClamp( 0);
	REngine::GetDevice().SetTextureAddressClamp( 1);

	UINT uiPass, uiPassCount;
	Technique.Begin(&uiPassCount, TECH_PRESERVE_NOT);
	for (uiPass = 0; uiPass < uiPassCount; ++uiPass) 
	{
		Technique.BeginPass(uiPass);
		DrawFullScreenQuad( 0.0f, 0.0f, m_dwWidth, m_dwHeight);
		Technique.EndPass();
	}
	Technique.End();

	REngine::GetDevice().SetTextureFilter( 0, RTF_LINEAR );
	REngine::GetDevice().SetTextureFilter( 1, RTF_LINEAR );

	REngine::GetDevice().SetTextureAddressWrap( 0);
	REngine::GetDevice().SetTextureAddressWrap( 1);

	REngine::GetDevice().SetTexture(0, NULL);
	REngine::GetDevice().SetTexture(1, NULL);
	REngine::GetDevice().SetTexture(2, NULL);
	REngine::GetDevice().SetTexture(3, NULL);
	REngine::GetDevice().SetTexture(4, NULL);
	REngine::GetDevice().SetTexture(5, NULL);

	// ī�޶� �̵� ����� ��츸 ���� ������ ����Ѵ�. ��������忡���� RT1�� NULL�� �صξ��ٰ� �������´�.
	if( m_nType == SMB_BY_EPICENTER)
	{
		REngine::GetDevice().SetRenderTarget( 1, pRT1);
	}

	// ���������� ��Ʈ������ ���� ���� ����
	m_matPrevVP = pCamera->GetViewProjectionMatrix();
	m_matPrevV = pCamera->GetViewMatrix();
	m_vPrePos = pCamera->GetPosition();
	m_vPreUp = pCamera->GetUp();
	m_vPreDir = pCamera->GetDirection();


	REngine::GetDevice().SetDepthEnable(true, true);


	//RenderOverlay();
}


//-----------------------------------------------------------------------------
// Name: RenderOverlay()
// Desc: Visializes one of the surfaces in the overlay
//-----------------------------------------------------------------------------
void RPostEffectScreenMotionBlur::RenderOverlay( RCameraSceneNode* pCamera, RTexture* pColorBuffer, RTexture* pNormalBuffer, RTexture* pDepthBuffer)
{

	// ���� �������̴� RT1�� ���� ���� �ʴ´�.
//	RTexture* pRT1 = REngine::GetDevice().GetRenderTarget( 1);
//	REngine::GetDevice().SetRenderTarget( 1, NULL);

	RTechnique TechniqueOverlay = m_pShader->GetTechnique( "DepthOverlay");

	m_pShader->GetConstant("depthScale").SetFloat( 0.8f);

	REngine::GetDevice().SetTexture(0, pColorBuffer);
	//REngine::GetDevice().SetTexture(0, m_pDepthBuffer);
	REngine::GetDevice().SetTexture(1, NULL);

	REngine::GetDevice().SetDepthEnable( false, false);	// 090625, OZ

	UINT uiPass, uiPassCount;
	TechniqueOverlay.Begin(&uiPassCount, TECH_PRESERVE_NOT);
	for (uiPass = 0; uiPass < uiPassCount; ++uiPass) 
	{
		TechniqueOverlay.BeginPass(uiPass);
		DrawFullScreenQuad( 0.0f, 0.0f, 256, 256);
		TechniqueOverlay.EndPass();
	}
	TechniqueOverlay.End();

	REngine::GetDevice().SetTexture(0, NULL);
	REngine::GetDevice().SetTexture(1, NULL);

	// ���� �������̴� RT1�� ���� ���� �ʴ´�.
//	REngine::GetDevice().SetRenderTarget( 1, pRT1);

}


void RPostEffectScreenMotionBlur::PlayScat( float fPlayTime, float fScatTime, float fSpeed, BYTE nScarFactorType)
{
	SetBasisByEpicenter();

	m_dwScatStartTime = REngine::GetTimer().GetTime();

	m_dwPlayTime = (DWORD)(float)( fPlayTime * 1000.0f);
	m_dwScatTime = (DWORD)(float)( fScatTime * 1000.0f);
	m_fScatSpeed = fSpeed;
	m_nScarFactorType = nScarFactorType;

}


void RPostEffectScreenMotionBlur::FrameMove()
{
	RPFC_THISFUNC;

	if( m_nType == SMB_BY_EPICENTER && m_dwScatStartTime > 0)
	{
		DWORD dwElap = REngine::GetTimer().GetTime() - m_dwScatStartTime;
		if( dwElap > m_dwPlayTime)
		{
			m_dwScatStartTime = 0;
			m_fSpeed = 0;
			m_nScarFactorType = 0;
		}
		else
		{
			float fFactor;
			// ������ ����
			if( dwElap < m_dwScatTime)
			{
				fFactor = (float)dwElap / (float)m_dwScatTime;
			}
			// ������� ����
			else
			{
				fFactor = (float)( m_dwPlayTime-dwElap) / (float)(m_dwPlayTime-m_dwScatTime);
			}

			switch( m_nScarFactorType)
			{
			case SAT_FAST_SCAT: fFactor = 1.0f - ((1.0f-fFactor) * (1.0f-fFactor));	// 1
				break;
			case SAT_SLOW_SCAT: fFactor *= fFactor;									// 2
				break;
			case SAT_GATTER: fFactor = ((1.0f-fFactor) * (1.0f-fFactor));			// 3
				break;
			}

			m_fSpeed = m_fScatSpeed * fFactor;
		}
	}
	else
	{
		m_dwScatStartTime = 0;
	}
}



void RPostEffectScreenMotionBlur::SetEpicenterProjectionCoord( float x, float y)
{ 
	float centerX = (x+1)/2.0f;
	if( centerX < 0)
		centerX = 0;
	else if( centerX > 1)
		centerX = 1;
	float centerY = (-y+1)/2.0f;
	if( centerY < 0)
		centerY = 0;
	else if( centerY > 1)
		centerY = 1;

	m_vEpicenter = RVector4( centerX, centerY, 0, 0);	
}



void RPostEffectScreenMotionBlur::SetEpicenterWorld( RCameraSceneNode* pCurrentCamera, RVector3& v)
{
	// ������� ��ġ�� ���� ��ǥ��� ������Ű�� �̸� �ؽ��� ��ǥ�� ��ȯ.. 
	RVector4 vProjPos = v * pCurrentCamera->GetViewProjectionMatrix();
	SetEpicenterProjectionCoord( vProjPos.x, vProjPos.y);	

}



}