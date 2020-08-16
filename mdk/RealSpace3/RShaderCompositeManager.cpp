#include "StdAfx.h"
#include "RShaderCompositeManager.h"
#include "RShaderFX.h"
#include "RDeviceD3D.h"
#include "RProfileInfo.h"
#include "RShaderComposite.h"
#include "REnvironmentSet.h"
#include "RShadowStrategy.h"
#include "RMaterial.h"
#include "RUtil.h"
#include "RDirectionalLightSceneNode.h"
#include "RPSSMShadowStrategy.h"



namespace rs3 {

RShaderFXPool* RShaderCompositeManager::m_pEffectPool = NULL;

RShaderConstant	RShaderCompositeManager::m_cWorldViewProj;
RShaderConstant	RShaderCompositeManager::m_cWorldView;
RShaderConstant	RShaderCompositeManager::m_cWorld;
RShaderConstant	RShaderCompositeManager::m_cViewProj;
RShaderConstant	RShaderCompositeManager::m_cView;
RShaderConstant	RShaderCompositeManager::m_cViewInv;
RShaderConstant	RShaderCompositeManager::m_cWorldPrev;
RShaderConstant	RShaderCompositeManager::m_cPartsColor;
RShaderConstant	RShaderCompositeManager::m_cVisiblity;
RShaderConstant RShaderCompositeManager::m_cBlendFactor;
RShaderConstant	RShaderCompositeManager::m_cFogFactor;
RShaderConstant	RShaderCompositeManager::m_cFogColor;
RShaderConstant	RShaderCompositeManager::m_cEyePos;
RShaderConstant	RShaderCompositeManager::m_cEyeDir;
RShaderConstant	RShaderCompositeManager::m_cTextureTransform;
RShaderConstant	RShaderCompositeManager::m_cDecalCenterPos;
RShaderConstant	RShaderCompositeManager::m_cTextureFactor;
RShaderConstant	RShaderCompositeManager::m_cGlossiness;

RShaderConstant	RShaderCompositeManager::m_cBlendMatrices;

#ifdef USING_RSC_PHYSIQUE_DQ
RShaderConstant	RShaderCompositeManager::m_cBlendDQ;
#endif

RShaderConstant	RShaderCompositeManager::m_cBlendQuats;
RShaderConstant	RShaderCompositeManager::m_cBlendTrans;			
RShaderConstant	RShaderCompositeManager::m_cBlendQuatsPrev;		
RShaderConstant	RShaderCompositeManager::m_cBlendTransPrev;		
RShaderConstant	RShaderCompositeManager::m_cBlendScale;			

RShaderConstant	RShaderCompositeManager::m_cGlowColorAndDiffuseRate;
RShaderConstant RShaderCompositeManager::m_cIlluminationScale;
//RShaderConstant RShaderCompositeManager::m_cRimScale;
RShaderConstant	RShaderCompositeManager::m_cAddColor;

RShaderConstant	RShaderCompositeManager::m_cAlphaRef;
RShaderConstant	RShaderCompositeManager::m_cMotionBlurFactorFront;
RShaderConstant	RShaderCompositeManager::m_cScreenSpaceTransform;
RShaderConstant	RShaderCompositeManager::m_cScreenSizeInv;
RShaderConstant	RShaderCompositeManager::m_cDSFID;

// �ν��Ͻ� ����
RShaderConstant RShaderCompositeManager::m_cInstancingMatrices;
RShaderConstant RShaderCompositeManager::m_cEdgeColor;
RShaderConstant RShaderCompositeManager::m_cTickNess;
RShaderConstant RShaderCompositeManager::m_cPEEdgeColor;
RShaderConstant RShaderCompositeManager::m_cUseEnvShininessAlpha;

RShaderConstant RShaderCompositeManager::m_cUVTransform;
RShaderConstant RShaderCompositeManager::m_cMapChannel;
RShaderConstant RShaderCompositeManager::m_cDiffuseLayerOpacity;
RShaderConstant RShaderCompositeManager::m_cNormalLayerOpacity;
RShaderConstant RShaderCompositeManager::m_cSpecularLayerOpacity;
RShaderConstant RShaderCompositeManager::m_cUseMaskMap;
RShaderConstant RShaderCompositeManager::m_scFarDist;

RShaderConstant RShaderCompositeManager::m_cNoiseFadeParam;
RShaderConstant RShaderCompositeManager::m_cNoiseFadeColor1;
RShaderConstant RShaderCompositeManager::m_cNoiseFadeColor2;

RShaderConstant RShaderCompositeManager::m_cShadowValue;

RVector4 RShaderCompositeManager::m_UVTransform[MAX_PIXELSAMPLER] = 
{
	RVector4(0,0,1,1), RVector4(0,0,1,1), RVector4(0,0,1,1), RVector4(0,0,1,1),
	RVector4(0,0,1,1), RVector4(0,0,1,1), RVector4(0,0,1,1), RVector4(0,0,1,1), 
	RVector4(0,0,1,1), RVector4(0,0,1,1), RVector4(0,0,1,1), RVector4(0,0,1,1), 
	RVector4(0,0,1,1), RVector4(0,0,1,1), RVector4(0,0,1,1), RVector4(0,0,1,1),
};

int RShaderCompositeManager::m_MapChannel[16] = {0};

DWORD RShaderCompositeManager::m_dwFogColor =0;
RVector4 RShaderCompositeManager::m_vFogFactor =RVector4(0,0,0,0);


RShaderCompositeManager::RShaderCompositeManager(void)
: m_bViewLightingOnly(false)
, m_bSpecularEnable(true)
, m_bUseDSF(false)
, m_bUseNormalMap(true)
, m_dwCheckSumShaderComponentCRCs(0)
{
#ifdef _DEBUG
// RSCID ��� ������ ShaderComponentPair componentNames �� ������ �´��� �˻��ϴ� ����
	size_t size = sizeof(componentNames)/sizeof(ShaderComponentPair);
	int total = 0;
	int count = 0;
	int offset = RSCID::ELEMENTS_NUM - 1;
	RSCID rscCurrent( 0, 1 );
	for ( size_t i = 0; i<size; ++i )
	{
		_ASSERT( rscCurrent == componentNames[i].flag );

		total++;
		count++;
		if( count >= 64 )
		{
			count = 0;

			for( int j = offset; j < RSCID::ELEMENTS_NUM; ++j )
			{
				rscCurrent.v[j] = 0;
			}
			offset--;

			rscCurrent.v[offset] = 1;
		}
		else
		{
			rscCurrent.v[offset] = rscCurrent.v[offset]<<1;
		}

	}

	_ASSERT(total == RSC_COUNT);

#endif
}

RShaderCompositeManager::~RShaderCompositeManager(void)
{
}

void RShaderCompositeManager::Destroy()
{
	DestroyEffects( m_effectMap );

	for(size_t i=0;i<m_shaderComponents.size();i++)
	{
		delete m_shaderComponents[i];
	}
	m_shaderComponents.clear();
	SAFE_DELETE(m_pEffectPool);
}

void RShaderCompositeManager::DestroyEffects( RSCIDMAP& effectMap )
{
	RSCIDMAP::iterator itr = effectMap.begin();
	for( ; itr!=effectMap.end(); ++itr )
	{
		RShaderComposite* pShader = itr->second;

		
		if( pShader->GetShaderFX() && pShader->GetShaderFX()->GetLoader() )
			pShader->GetShaderFX()->GetLoader()->InvalidateWork();

		pShader->Destroy();
		delete pShader;
	}
	effectMap.clear();
}

bool RShaderCompositeManager::Create()
{
	_ASSERT(!m_pEffectPool);
	m_pEffectPool = new RShaderFXPool;

	m_shaderComponents.resize(RSC_COUNT);
	if(RSC_COUNT!=_countof(componentNames))
	{
		_ASSERT(FALSE);
		return false;
	}
	
	// �� ������Ʈ�� �ε�
	for(int i=0;i<RSC_COUNT;i++)
	{
		m_shaderComponents[i] = new RShaderComponent;
		if(!m_shaderComponents[i]->CreateFromFile( componentNames[i].name) )
		{
			mlog(m_shaderComponents[i]->GetErrorString().c_str());
			mlog("file %s line %d : %s\n", componentNames[i].name, m_shaderComponents[i]->GetErrorLine(), 
				m_shaderComponents[i]->GetErrorLineString().c_str());
			SAFE_DELETE(m_shaderComponents[i]);
			return false;
		}

		m_dwCheckSumShaderComponentCRCs += m_shaderComponents[i]->GetCRC();
	}

	// ���̴� ���ҽ� ĳ�� ���� �˻�� ����
	REngine::GetSystemInfo().CheckAndDeleteCache();

	//===

	//vector<string> vecMacro;

	{
		RShaderFX* pEffect = GetShader(
			RSC_INSTANCING | RSC_POSITION | RSC_NORMAL | RSC_TANGENT | RSC_PPL_ALPHA_FROM_VCOLOR_A | RSC_VCOLOR | RSC_TRANSFORM_CONSTANT | /*RSC_TRANSFORM |*/ GetUVAniTexcoordComponent() |
			RSC_DIFFUSEMAP | RSC_DIFFUSEMAP_BLEND | RSC_DIFFUSE_PARTSCOLOR_CONSTANT | RSC_DIFFUSE_PARTSCOLOR | RSC_DIFFUSE_NOISEFADE | RSC_PPL_VIEW_DIR | RSC_PPL_COS_VIEW |  
			RSC_PPL_LIGHTING | RSC_PPL_VISIBLITY | RSC_PPL_SELFILLUMINATION | RSC_SELFILLUMINATIONMAP | RSC_REFLECTMAP |
			RSC_SPECULARMAP | RSC_FACTOR | RSC_PPL_GLOW | RSC_ADD_SPECIFIC_COLOR | RSC_PPL_FOG | RSC_PPL_EDGE_COLOR | RSC_TRANSFORM_EDGE | RSC_PPL_PE_EDGE_MASK, false );

		if( NULL == pEffect )
			return false;

		m_cWorld = pEffect->GetConstant( "g_matWorld" );
		m_cWorldViewProj = pEffect->GetConstant( "g_matWorldViewProj");
		m_cWorldView = pEffect->GetConstant( "g_matWorldView");
		m_cPartsColor = pEffect->GetConstant( "g_vPartsColor" );
		m_cPartsColor.SetVector4( 1.f, 1.f, 1.f, 1.f );
		m_cVisiblity = pEffect->GetConstant( "g_fVisiblity" );
		m_cBlendFactor = pEffect->GetConstant( "g_fBlendFactor" );

		m_cFogFactor = pEffect->GetConstant( "g_vFogFactor" );
		m_cFogColor = pEffect->GetConstant( "g_vFogColor" );

		m_cEyePos = pEffect->GetConstant( "g_vEyePosition" );
		m_cEyeDir = pEffect->GetConstant( "g_vEyeDir" );
		m_cTextureTransform = pEffect->GetConstant( "g_textureTransform" );
		m_cTextureFactor = pEffect->GetConstant( "g_textureFactor" );
		m_cTextureFactor.SetVector4(1, 1, 1, 1);
		m_cGlowColorAndDiffuseRate = pEffect->GetConstant( "g_vGlowColorAndDiffuseRate");
		m_cIlluminationScale = pEffect->GetConstant( "g_vIlluminationScale");
		m_cIlluminationScale.SetFloat(1.f);
		m_cAddColor = pEffect->GetConstant( "g_vAddColor");
		m_cAddColor.SetVector3( 0.f, 0.f, 0.f );
		m_cNoiseFadeParam = pEffect->GetConstant( "g_NoiseFadeParam");
		m_cNoiseFadeColor1 = pEffect->GetConstant( "g_NoiseFadeColor1");
		m_cNoiseFadeColor2 = pEffect->GetConstant( "g_NoiseFadeColor2");
		m_cInstancingMatrices = pEffect->GetConstant( "g_InstancingMatrices");
		m_cEdgeColor		= pEffect->GetConstant( "g_vEdgeColor" );
		m_cTickNess			= pEffect->GetConstant( "g_fTickNess" );
		m_cPEEdgeColor		= pEffect->GetConstant( "g_vPEEdgeColor" );
		m_cUseEnvShininessAlpha		= pEffect->GetConstant( "g_iUseEnvAlpha" );
	}

	{
		RShaderFX* pEffectPhysique = GetShader(
			RSC_POSITION | RSC_NORMAL | RSC_PHYSIQUE | RSC_TRANSFORM_CONSTANT | RSC_TRANSFORM | GetUVAniTexcoordComponent(), false );

		if( NULL == pEffectPhysique )
			return false;

		// VBlur, DOF_sky, Lignting->White
		RShaderFX* pEffectPhysiqueVBlur = GetShader(
			RSC_POSITION | RSC_NORMAL | RSC_PHYSIQUE_VBLUR | RSC_TRANSFORM_CONSTANT | RSC_TRANSFORM_VBLUR | GetUVAniTexcoordComponent() |
			RSC_PPL_WHITE | RSC_PPL_VBLUR, false );

		if( NULL == pEffectPhysiqueVBlur )
			return false;

		m_cWorldPrev = pEffectPhysiqueVBlur->GetConstant( "g_matWorldPrev");	
		m_cMotionBlurFactorFront = pEffectPhysiqueVBlur->GetConstant( "g_fMotionBlurFactorFront");

		m_cViewProj = pEffectPhysiqueVBlur->GetConstant( "g_matViewProj");
		m_cView = pEffectPhysiqueVBlur->GetConstant( "g_matView");
		m_cViewInv = pEffectPhysiqueVBlur->GetConstant( "g_mViewInv");


		// ���� ���ʹϾ� ��� 
	#ifdef USING_RSC_PHYSIQUE_DQ	
		m_cBlendDQ = pEffectPhysique->GetConstant( "g_blendDQs" );
	#else
	 #ifdef USING_RSC_PHYSIQUE_QT
		m_cBlendScale = pEffectPhysique->GetConstant( "g_blendScale" );
		// ���� ��������
		m_cBlendQuats = pEffectPhysique->GetConstant( "g_blendQuats" );
		m_cBlendTrans = pEffectPhysique->GetConstant( "g_blendTrans" );
		// ���� ��������
		m_cBlendQuatsPrev = pEffectPhysiqueVBlur->GetConstant( "g_blendQuatsPrev" );
		m_cBlendTransPrev = pEffectPhysiqueVBlur->GetConstant( "g_blendTransPrev" );
	 #else
		m_cBlendMatrices = pEffectPhysique->GetConstant( "g_blendMatrices" );
	 #endif
	#endif
	}

	{
		RShaderFX* pEffect = GetShader( RSC_POSITION | RSC_TRANSFORM_CONSTANT | RSC_TRANSFORM | GetVertexTexcoordComponent() | RSC_DIFFUSEMAP | RSC_M_CONSTANT | RSC_LIGHTBUFFER_READ_DSF | RSC_NORMALBUFFER_READ | 
			RSC_PPL_LIGHTING | RSC_PPL_CLIP_ALPHA, false);

		if( NULL == pEffect )
			return false;

		m_cScreenSpaceTransform = pEffect->GetConstant( "g_ScreenSpaceTransformMStage" );
		m_cAlphaRef =  pEffect->GetConstant( "g_fAlphaRef" );;
		m_cScreenSizeInv = pEffect->GetConstant( "g_vScreenSizeInv" );
	}

	// UV, Mapchannel
	{
		RShaderFX* pEffect = GetShader( RSC_POSITION | RSC_NORMAL | RSC_TRANSFORM_CONSTANT | RSC_TRANSFORM | GetVertexTexcoordComponent() | 
			RSC_LAYER_1STBLEND_COLORBURN | RSC_DIFFUSEMAPLAYER2 | RSC_DUAL_NORMALMAP_OVERAY | RSC_DUAL_SPECULARMAP_COLORDODGE | RSC_INSTANCING | RSC_GSTAGE_CONSTANT, false );

		if( NULL == pEffect )
			return false;

		m_cUVTransform = pEffect->GetConstant("g_UVTransform");
		m_cMapChannel = pEffect->GetConstant("g_MapChannel");
		m_cDiffuseLayerOpacity = pEffect->GetConstant("g_vDiffuseLayerOpacity");
		m_cNormalLayerOpacity = pEffect->GetConstant("g_fNormalLayerOpacity");
		m_cSpecularLayerOpacity = pEffect->GetConstant("g_fSpecularLayerOpacity");
		m_cUseMaskMap = pEffect->GetConstant("g_bUseMaskMap");

		RShaderCompositeManager::m_cUVTransform.SetVector4Array(RShaderCompositeManager::m_UVTransform, MAX_PIXELSAMPLER);
		RShaderCompositeManager::m_cMapChannel.SetIntArray(RShaderCompositeManager::m_MapChannel, MAX_PIXELSAMPLER);
	}

	//G-buffer
	{
		RShaderFX* pEffect = GetShader( RSC_TRANSFORM_CONSTANT | RSC_TRANSFORM | RSC_DIFFUSEMAP | GetVertexTexcoordComponent() | RSC_POSITION | RSC_NORMAL | RSC_GSTAGE_CONSTANT | RSC_PPL_G_DEPTH | RSC_PPL_VISIBLITY | RSC_PPL_G_WRITE_NORMAL_DEPTH_SPECULAR_RIM_RT01 | RSC_PPL_G_WRITE_DSF_RT2 | RSC_PPL_G_RIM | RSC_PPL_SPECULAR_CONSTANT | RSC_MASK_AO, false);

		if( NULL == pEffect )
			return false;

		m_cGlossiness = pEffect->GetConstant( "g_fGlossiness" );
		m_cDSFID = pEffect->GetConstant( "g_fDSFID" ); // L G �������� ���
		m_scFarDist = pEffect->GetConstant( "g_fFarDist" );
	}

	//Projection Shadow
	{
		RShaderFX* pEffect = GetShader( RSC_POSITION | RSC_TRANSFORM_CONSTANT | GetVertexTexcoordComponent() | RSC_SHADOW_WRITE, false);

		if( NULL == pEffect )
			return false;

		m_cShadowValue  = pEffect->GetConstant( "g_fShadowValue" );
	}

	// depthmap
	{
		RShaderFX* pEffect = GetShader( RSC_POSITION | RSC_NORMAL | GetVertexTexcoordComponent() | RSC_TRANSFORM_CONSTANT | RSC_DEPTH_WRITE_ESM, false );
		if( NULL == pEffect )
			return false;

		pEffect = GetShader( RSC_POSITION | RSC_NORMAL | GetVertexTexcoordComponent() | RSC_PHYSIQUE | RSC_TRANSFORM_CONSTANT | RSC_DEPTH_WRITE_ESM, false);
		if( NULL == pEffect )
			return false;
	}

	// Decal
	{
		RShaderFX* pEffect = GetShader( RSC_POSITION | RSC_TRANSFORM_CONSTANT | RSC_TRANSFORM | GetProjectionTexcoordComponent() | RSC_DECAL, false );
		if( NULL == pEffect )
			return false;

		m_cDecalCenterPos = pEffect->GetConstant( "g_decalCenterpos" );
	}

	return true;
}

RShaderComposite* RShaderCompositeManager::GetExistShader(RSCID rscComponents)
{
	RSCIDMAP::iterator itr = m_effectMap.find(rscComponents);
	if(itr!=m_effectMap.end())
	{
		_ASSERT( rscComponents == itr->first );
		return itr->second;
	}
	return NULL;
}

RShaderComposite* RShaderCompositeManager::NewShaderComposite(RSCID rscComponents)
{
	return new RShaderComposite(rscComponents);
}

RShaderFX* RShaderCompositeManager::GetShader(RSCID rscComponents, bool bBackgroundLoading)
{
	RShaderComposite* pExist = GetExistShader(rscComponents);
	if(pExist)
	{
		if(pExist->IsLoadingSucceeded())
			return pExist->GetShaderFX();

		return NULL;
	}

	RShaderComposite* pShaderComposite = NewShaderComposite( rscComponents );
	m_effectMap.insert( RSCIDMAP::value_type(rscComponents, pShaderComposite) );
	pShaderComposite->CompositeAndBuildShader( this, bBackgroundLoading );

	if( pShaderComposite->IsLoadingSucceeded() )
		return pShaderComposite->GetShaderFX();

	return NULL;
}

void RShaderCompositeManager::SetGlobalRenderState(RCameraSceneNode* pCurrentCamera)
{
	RPFC_THISFUNC;
	_ASSERT(REngine::GetDevicePtr());
	_ASSERT(pCurrentCamera);

	
	//Fog�� �⺻������ ������� ����. �ʿ��� �н����� �����ؼ� ���.
	FogOff();


	RShaderCompositeManager::m_cEyePos.SetVector3( pCurrentCamera->GetWorldPosition() );
	RShaderCompositeManager::m_cEyeDir.SetVector3( pCurrentCamera->GetWorldDirection() );

	RShaderCompositeManager::m_cView.SetMatrix( pCurrentCamera->GetViewMatrix() );

	RMatrix matView, matViewInv;
	matView = pCurrentCamera->GetViewMatrix();
	matView.GetInverse( &matViewInv );
	RShaderCompositeManager::m_cViewInv.SetMatrix( matViewInv );

	RShaderCompositeManager::m_cViewProj.SetMatrix( pCurrentCamera->GetViewProjectionMatrix());		

	RShaderCompositeManager::m_scFarDist.SetFloat(pCurrentCamera->GetFarZ());


	//Set Default state
	{
		REngine::GetDevice().SetDepthEnable(true, true);
		REngine::GetDevice().SetDepthFunc(RCMP_LESSEQUAL);
		REngine::GetDevice().SetDepthBias(0);

		REngine::GetDevice().SetCullMode(RCULL_CW);
		REngine::GetDevice().SetAlphaTestEnable(false);
		REngine::GetDevice().SetBlending(RBLEND_NONE);

		REngine::GetDevice().SetStencilEnable(false);
		REngine::GetDevice().SetStencilFunc(RCMP_ALWAYS);
		REngine::GetDevice().SetStencilRef(0);
		REngine::GetDevice().SetStencilMask(0xFFFFFFFF);
	}
}

void RShaderCompositeManager::FogOn(float fNear, float fFar, float fOffset, DWORD dwColor)
{
	RPFC_THISFUNC;
	SetFogFactor(fNear, fFar, fOffset);
	SetFogColor(dwColor);
}

void RShaderCompositeManager::SetFogColor(DWORD dwColor)
{
	RPFC_THISFUNC;
	RVector3 vFogColor;
	DWColorToFloat3( &vFogColor, dwColor );
	m_cFogColor.SetVector3(vFogColor);

	m_dwFogColor = dwColor;
}

void RShaderCompositeManager::SetFogFactor(float fNear, float fFar, float fOffset)
{
	RPFC_THISFUNC;
	float fDist = fFar - fNear;
	fDist = ( fDist == 0.0f ? 0 : 1.0f/fDist );
	m_vFogFactor= RVector4(fNear, fFar, fDist, fOffset);
	m_cFogFactor.SetVector4(m_vFogFactor);
}

void RShaderCompositeManager::FogOff()
{
	SetFogFactor(0, 0, 0);
	SetFogColor(0);
}

void RShaderCompositeManager::GetFogInfo(RVector4& outFogFactor, DWORD& outColor)
{
	outFogFactor = m_vFogFactor;
	outColor = m_dwFogColor;
}

void RShaderCompositeManager::SetMaterialRenderState( RMaterial* pMaterial, bool bUseNormalmap, bool bUseSpecularValue, bool bAlphaMesh)
{
	RPFC_THISFUNC;
	SetCullMode(pMaterial->CheckFlag(RM_FLAG_TWOSIDED));
	SetBlending(pMaterial->GetFlag(), bAlphaMesh);

	SetMaterialTexture(pMaterial, bUseNormalmap);
	SetMaterialShaderConstant(pMaterial, bUseSpecularValue, 1.0f, 1.0f);
}

void RShaderCompositeManager::SetCompositeLayerRenderState(RCompositeLayer* CompositeLayer)
{
	RPFC_THISFUNC;
	if(CompositeLayer==NULL)
		return;

	_ASSERT(CompositeLayer->GetLayerCount()<=3);

	const int LayerSampler[6][6] = 
	{
		//Unknown Type
		{-1,-1,-1,-1,-1,-1},

		//Diffuse Type
		{AS_DIFFUSE, AS_DIFFUSELAYER0MASK, AS_DIFFUSELAYER1, AS_DIFFUSELAYER1MASK, AS_DIFFUSELAYER2 ,AS_DIFFUSELAYER2MASK},

		//Opacity Type
		{-1,-1,-1,-1,-1,-1},

		//Specular Type
		{AS_SPECULARMAP, -1, AS_SECOND_SPECULARMAP, -1, -1, -1},

		//Normal Type
		{AS_NORMALMAP, -1, AS_SECOND_NORMALMAP, -1, -1, -1},

		//Self illumination Type
		{-1,-1,-1,-1,-1,-1},
	};

	RVector3 vLayerOpacity(1,1,1);
	RVector3 bUseMaskMap(0,0,0);

	for(int i=0; i<CompositeLayer->GetLayerCount(); ++i)
	{
		RTextureType TexType = CompositeLayer->GetTextureType();

		const COMPOSITE_LAYER_INFO& LayerInfo = CompositeLayer->GetLayerInfo(i);


		//------------------------------------------------------
		//Set Layer Texture
		int SamplerIndex = LayerSampler[TexType][i*2];
		if(SamplerIndex>=0)
		{
			SetTexture(SamplerIndex, LayerInfo.pLayerMap, REngine::GetConfig().GetTextureFilter(), REngine::GetConfig().GetTextureMaxAnisotropy());
		}
		//Set Layer Texture
		//------------------------------------------------------



		//------------------------------------------------------
		//Set Mask Texture
		SamplerIndex = LayerSampler[TexType][i*2 +1];
		if(SamplerIndex>=0)
		{
			RMtrlTexInfo* MaskTexInfo=LayerInfo.pMaskMap;
			SetTexture(SamplerIndex, MaskTexInfo, REngine::GetConfig().GetTextureFilter(), REngine::GetConfig().GetTextureMaxAnisotropy());
			
			if(MaskTexInfo)
				bUseMaskMap[i] = 1;
		}
		//Set Mask Texture
		//------------------------------------------------------
		

		vLayerOpacity[i] = LayerInfo.fOpacity;
	}

	SetMultiLayerConstant(CompositeLayer->GetTextureType(), vLayerOpacity, bUseMaskMap);
}

void RShaderCompositeManager::ApplyAlphaComponent( RSCID& rscComponents, RMaterial* pMtrl, bool bVColor )
{
	RPFC_THISFUNC;
	if( bVColor)
		rscComponents |= RSC_PPL_ALPHA_FROM_VCOLOR_A;
	if( pMtrl)
	{
		bool bUseOpacity = pMtrl->CheckFlag(RM_FLAG_USEOPACITY);
		bool bUseAlphaTest = pMtrl->CheckFlag(RM_FLAG_USEALPHATEST);
		if( bUseOpacity || bUseAlphaTest)
		{
			/// ��� ������� ���� ���� �б�
			switch( pMtrl->GetTextureTypeForAlphaUsage(ALPHAUSAGE_ALPHA) )
			{
			case RTT_DIFFUSE:
				rscComponents |= RSC_PPL_ALPHA_FROM_DIFFUSE_A;
				break;
			case RTT_SPECULAR:
				rscComponents |= RSC_PPL_ALPHA_FROM_SPECULAR_A;
				rscComponents |= GetSpecularMapComponent(pMtrl);
				break;
			case RTT_NORMAL:
				rscComponents |= RSC_PPL_ALPHA_FROM_NORMAL_A;
				rscComponents |= GetTextureLayerReadComponent(pMtrl, RTT_NORMAL);
				break;
			case RTT_SELFILLUMINATION:
				rscComponents |= RSC_PPL_ALPHA_FROM_SI_A;
				rscComponents |= GetTextureLayerReadComponent(pMtrl, RTT_SELFILLUMINATION);
				break;
			case RTT_OPACITY:
				rscComponents |= RSC_PPL_ALPHA_FROM_OPACITY_A;
				rscComponents |= GetTextureLayerReadComponent(pMtrl, RTT_OPACITY);
				break;
			}

			if(bUseAlphaTest)
				rscComponents |= RSC_PPL_CLIP_ALPHA;
		}
	}	
}

void RShaderCompositeManager::ApplySpecularComponent( RSCID& rscComponents, RMaterial* pMtrl, bool bTexCoord)
{
	RPFC_THISFUNC;
	if( pMtrl==NULL )
		return;

	/// ����ŧ�� ���ġ ������ �� ����.
	/// ���� ������ ����ϸ� ����ŧ����� ������Ʈ ó���� ���� �ʴ´�.
	/// ���۵� ���� ó���� �����ϸ� �׶� ����... 
	if( pMtrl->IsUseSpecular()==false || pMtrl->CheckFlag(RM_FLAG_USEOPACITY) || pMtrl->CheckFlag(RM_FLAG_ADDITIVE) || pMtrl->CheckShaderComponentFlag(RSC_PPL_GHOST))
		return;

	rscComponents |= RSC_PPL_SPECULAR_CONSTANT;

	// ����ŧ�� ��� ��� ���� ���� ���� ���� �б�
	switch(pMtrl->GetTextureTypeForAlphaUsage(ALPHAUSAGE_SPECULAR))
	{
	case RTT_NONE:
	case RTT_UNKNOWN:
		rscComponents |= RSC_PPL_SPECULAR_PLAIN;
		break;
	case RTT_DIFFUSE:
		rscComponents |= RSC_PPL_SPECULAR_FROM_DIFFUSE_A;
		break;
	case RTT_SPECULAR:
		rscComponents |= RSC_PPL_SPECULAR_FROM_SPECULARMAP;
		rscComponents |= GetSpecularMapComponent(pMtrl);
		break;
	case RTT_NORMAL:
		rscComponents |= RSC_PPL_SPECULAR_FROM_NORMAL_A;
		rscComponents |= GetTextureLayerReadComponent(pMtrl, RTT_NORMAL);
		break;
	case RTT_SELFILLUMINATION:
		rscComponents |= RSC_PPL_SPECULAR_FROM_SI_A;
		rscComponents |= GetTextureLayerReadComponent(pMtrl, RTT_SELFILLUMINATION);
		break;
	case RTT_OPACITY:
		rscComponents |= RSC_PPL_SPECULAR_FROM_OPACITY_A;
		rscComponents |= GetTextureLayerReadComponent(pMtrl, RTT_OPACITY);
		break;
//	default:
//		rscComponents &= ~RSC_PPL_SPECULAR_CONSTANT;
//		break;
	}
}

RSCID RShaderCompositeManager::GetMaterialDecalDependentComponent()
{
	return GetVertexTexcoordComponent() | RSC_DIFFUSEMAP;
}

RSCID RShaderCompositeManager::GetIncompatibleComponentWithMaterialDecal()
{
	return RSC_DIFFUSEMAP | RSC_TEXCOORD_DIFFUSE_MAPCHANNEL | RSC_TEXCOORD_DIFFUSE_TRANS;
}

RSCID RShaderCompositeManager::GetMaterialDecalComponent()
{
	return RSC_LAYER_1STBLEND_NORMAL | RSC_DIFFUSEMAPLAYER2 | RSC_LAYER_RESULT_EXPORT;
}

void RShaderCompositeManager::ApplyMaterialDecalComponents(RSCID& rscComponents)
{
	if( rscComponents.HasAllComponent(GetMaterialDecalDependentComponent()) == false)
		return;

	rscComponents &= ~GetIncompatibleComponentWithMaterialDecal();
	rscComponents |= GetMaterialDecalComponent();
}

RSCID RShaderCompositeManager::GetVertexTexcoordComponent()
{
	return RSC_TEXCOORD_VERTEX | RSC_TEXCOORD;
}

RSCID RShaderCompositeManager::GetUVAniTexcoordComponent()
{
	return RSC_TEXCOORD_TRANSFORM | RSC_TEXCOORD;
}

RSCID RShaderCompositeManager::GetProjectionTexcoordComponent()
{
	return RSC_TEXCOORD_WORLD | RSC_TEXCOORD;
}

void RShaderCompositeManager::ApplyNormalMapComponent( RSCID& rscComponents, RMaterial* pMtrl, bool _bUseNormalMap )
{
	RPFC_THISFUNC;
	if( pMtrl && pMtrl->GetTexLayerByType(RTT_NORMAL) && _bUseNormalMap )
	{
		rscComponents |= RSC_TANGENT | RSC_PPL_G_NORMALMAP;

		if(RCompositeLayer* pNormalComposite = pMtrl->GetCompositeLayer(RTT_NORMAL))
		{
			_ASSERT(pNormalComposite->GetLayerCount()>1);
			COMPOSITE_LAYER_BLEND_MODE BlendMode = pNormalComposite->GetBlendingMode(1);

			// Support only Overlay & Average two blend mode
			if( BlendMode!=LAYER_BLEND_OVERLAY && BlendMode!=LAYER_BLEND_AVERAGE)
				rscComponents |= GetTextureLayerReadComponent(pMtrl, RTT_NORMAL);

			if(BlendMode==LAYER_BLEND_OVERLAY)
				rscComponents |= RSC_DUAL_NORMALMAP_OVERAY;
			else if(BlendMode==LAYER_BLEND_AVERAGE)
				rscComponents |= RSC_DUAL_NORMALMAP_AVERAGE;
		}
		else
			rscComponents |= GetTextureLayerReadComponent(pMtrl, RTT_NORMAL);
	}
	//else
	//{
	//	rscComponents &= ~(RSC_TANGENT | RSC_DUAL_NORMALMAP | RSC_PPL_G_NORMALMAP);
	//	rscComponents &= ~GetTextureLayerReadComponent(pMtrl, RTT_NORMAL);
	//}
}

void RShaderCompositeManager::ApplyTexcoordComponent( RSCID& rscComponents, RMaterial* pMtrl, bool bTexCoord, bool bExtraTexCoord)
{
	RPFC_THISFUNC;
	if( bTexCoord )
	{
		if( pMtrl && pMtrl->CheckFlag(RM_FLAG_TEXTURE_TRANSFORM))
			rscComponents |= GetUVAniTexcoordComponent();
		else
			rscComponents |= GetVertexTexcoordComponent();	
	}

	if(bExtraTexCoord)
		rscComponents |= RSC_TEXCOORD2;
}

void RShaderCompositeManager::ApplyDiffuseComponent( RSCID& rscComponents, RMaterial* pMtrl, bool bBlend, bool bTexCoord)
{
	RPFC_THISFUNC;
	if( bTexCoord )
		rscComponents |= GetDiffuseComponent(pMtrl);

	// ��ǻ��� ����
	if( rscComponents.HasComponent(RSC_DIFFUSEMAP) && bBlend )
		rscComponents |= RSC_DIFFUSEMAP_BLEND;
}

void RShaderCompositeManager::ApplyVertexColorComponent( RSCID& rscComponents, bool bVertexColor)
{
	if( bVertexColor)
		rscComponents |= RSC_VCOLOR;
}

void RShaderCompositeManager::ApplyMultiplyColorComponent( RSCID& rscComponents, bool bMultiplyColor)
{
	if( bMultiplyColor)
		rscComponents |= RSC_FACTOR;
}

void RShaderCompositeManager::ApplyPartsColorComponent( RSCID& rscComponents, RMaterial* pMtrl, bool bTexCoord)
{
	if( pMtrl && pMtrl->CheckFlag(RM_FLAG_USEPARTSCOLOR) && bTexCoord)
	{
		rscComponents |= RSC_DIFFUSE_PARTSCOLOR_CONSTANT | RSC_DIFFUSE_PARTSCOLOR;
	}	
}

void RShaderCompositeManager::ApplyNoiseFadeComponent( RSCID& rscComponents, bool bTexCoord, bool bNoiseFade)
{
	if( bNoiseFade && bTexCoord)
		rscComponents |= RSC_DIFFUSE_NOISEFADE;
}

void RShaderCompositeManager::ApplyTransformComponent( RSCID& rscComponents, bool bPhysique, bool bHaveNormal )
{
	rscComponents |= RSC_TRANSFORM_CONSTANT | RSC_TRANSFORM | RSC_POSITION;

	if( bHaveNormal )
		rscComponents |= RSC_NORMAL;

	if( bPhysique)
		rscComponents |= RSC_PHYSIQUE;
}

void RShaderCompositeManager::ApplyAOMaskComponent( RSCID& rscComponents, bool bAO)
{
	if( bAO)
		rscComponents |= RSC_MASK_AO;
}


// �ν��Ͻ� ���� ������ ���� ���·� ���;� �ϸ� �帧�� �������� �־�� �Ѵ�.
void RShaderCompositeManager::CheckAndApplyInstancingComponent( RSCID& rscComponents, bool bInstancing)
{
	RPFC_THISFUNC;
	if( bInstancing)
	{
		rscComponents |= RSC_INSTANCING;

		/// �����÷��� �ν��Ͻ� �����Ϳ� ���Ե����� ��� ������ ���� ������ �ƴϴ�.
		if( (rscComponents & RSC_DIFFUSE_PARTSCOLOR).IsNonZero() )
			rscComponents |= RSC_INSTANCE_PARTSCOLOR;
		
		/// AO�� �ν��Ͻ��� �ƴ� �ô� AO�� ������ RSC_MASK_AO�� ���Եǰ�, �ƴϸ� �������� �������ν� ����ŷ ���ΰ� �Ǻ�������
		/// �ν��Ͻ��� ���� �ν��Ͻ� �����Ϳ� ����ŷ ���ΰ� ���ԵǾ� �ִ�.
		/// ���� ������ ������ �ִ´�.
		rscComponents &= ~RSC_MASK_AO;
		rscComponents |= RSC_INSTANCE_AO;
	}
		
}


void RShaderCompositeManager::AddReflectRSC( RSCID& rscComponents )
{
	rscComponents |= RSC_REFLECTMAP;
	rscComponents |= RSC_PPL_REFLECT;
}


void RShaderCompositeManager::ApplyReflectComponent( RSCID& rscComponents, RMaterial* pMtrl )
{
	RPFC_THISFUNC;
	RMtrlTexInfo* pTexInfo = pMtrl->GetTexLayerByType( RTT_REFLECT );
	if( pTexInfo )
	{
		AddReflectRSC( rscComponents );

		switch(pMtrl->GetTextureTypeForAlphaUsage(ALPHAUSAGE_SHINIESS))
		{
		case RTT_DIFFUSE:
			RShaderCompositeManager::m_cUseEnvShininessAlpha.SetInt( 0 );
			break;
		case RTT_SPECULAR:
			RShaderCompositeManager::m_cUseEnvShininessAlpha.SetInt( 1 );
			break;
		case RTT_NORMAL:
			RShaderCompositeManager::m_cUseEnvShininessAlpha.SetInt( 2 );
			break;
		case RTT_OPACITY:
			RShaderCompositeManager::m_cUseEnvShininessAlpha.SetInt( 3 );
			break;
		}
	}
}


RSCID RShaderCompositeManager::GetGStageOutputComponents( RRENDERINGMETHOD renderMethod)
{
	RPFC_THISFUNC;
	/// | RSC_PPL_VISIBLITY�� ���� �־�� �� �ʿ伺�� ���� ��.
	RSCID rscComponents = RSC_GSTAGE_CONSTANT | RSC_PPL_G_DEPTH;

	rscComponents |= RSC_PPL_G_WRITE_NORMAL_DEPTH_SPECULAR_RIM_RT01;
	switch( renderMethod)
	{
	case RENDERING_INFERRED: 
		if( m_bUseDSF)
			rscComponents |= RSC_PPL_G_WRITE_DSF_RT2; 
		break;
	case RENDERING_DEFERRED: 
		rscComponents |= RSC_PPL_G_WRITE_DIFFUSE_AO_RT2; 
		break;
	}

	return rscComponents;
}


RSCID RShaderCompositeManager::MakeComponentBitflagForGStage( RRENDERINGMETHOD renderMethod, ComponentDesc_GStage desc)
{
	RPFC_THISFUNC;

	RSCID rscComponents = GetGStageOutputComponents( renderMethod);

	ApplyTransformComponent( rscComponents, desc.bPhysique&& REngine::m_bUseSkinning, true );
	ApplyTexcoordComponent( rscComponents, desc.pMtrl, desc.bTexCoord, desc.bExtraTexCoord);
	ApplyNoiseFadeComponent( rscComponents, desc.bTexCoord, desc.bNoiseRef);
	ApplyDiffuseComponent( rscComponents, desc.pMtrl, desc.bBlendingMaterial, desc.bTexCoord);
	ApplyVertexColorComponent( rscComponents, desc.bVertexColor);
	ApplyNormalMapComponent( rscComponents, desc.pMtrl, REngine::GetSceneManager().GetShaderCompositeManager()->GetUseNormalMap() );
	ApplyAlphaComponent( rscComponents, desc.pMtrl, desc.bVertexColor );			/// ���� �׽�Ʈ�� ���� �ʿ� �ϴ�.
	ApplyAOMaskComponent( rscComponents, desc.bAO && !(desc.pMtrl && desc.pMtrl->CheckFlag(RM_FLAG_TWOSIDED)));
	ApplyReflectComponent( rscComponents, desc.pMtrl );
	
	// ���۵��Ͻø� G���ۿ��� ���� �÷�, ���ؽ� �÷� ���� �Ҹ� �ȴ�.
	if( renderMethod == RENDERING_DEFERRED)
	{
		ApplySpecularComponent( rscComponents, desc.pMtrl, desc.bTexCoord);
		ApplyPartsColorComponent( rscComponents, desc.pMtrl, desc.bTexCoord);
		ApplyMultiplyColorComponent( rscComponents, desc.bMultiplyColor);
	}

	CheckAndApplyInstancingComponent( rscComponents, desc.bInstancing);

	return rscComponents;
}



RSCID RShaderCompositeManager::MakeComponentBitflagForMStage(ComponentDesc_MStage desc)
{
	RPFC_THISFUNC;
	static const RSCID rscNoLightingFilter = ~(RSC_PPL_LIGHTING | RSC_LIGHTBUFFER_READ | RSC_LIGHTBUFFER_READ_DSF);

	bool bDontSpec = false;

	RSCID rscComponents = RSC_M_CONSTANT | RSC_PPL_LIGHTING | RSC_PPL_VISIBLITY;		// �÷�, E_vLitColor, E_vIlluminationColor�� ������ ���� ���. �� �ڿ� ����ŧ�� ��� �ǰ���?
	rscComponents |= m_bUseDSF ? RSC_LIGHTBUFFER_READ_DSF : RSC_LIGHTBUFFER_READ;
	rscComponents |= RSC_PPL_FOG;

	ApplyTransformComponent( rscComponents, desc.bPhysique && REngine::m_bUseSkinning, true );
	ApplyTexcoordComponent( rscComponents, desc.pMtrl, desc.bTexCoord, desc.bExtraTexCoord );
	ApplyNoiseFadeComponent( rscComponents, desc.bTexCoord, desc.bNoiseRef );

	if( desc.bAddColorPass)
	{
		// �÷� �ֵ� �н������� ������ ���� �ʿ� ����. 
		rscComponents &= rscNoLightingFilter;
	}
	// ���� ���ǵ��� �÷� �ֵ� �н��ʹ� ��� ���� �̾߱��.. 
	else
	{
		ApplyDiffuseComponent( rscComponents, desc.pMtrl, desc.bBlendingMaterial, desc.bTexCoord);
		ApplyAlphaComponent( rscComponents, desc.pMtrl, desc.bVertexColor );
		if( desc.bVertexColor)
			bDontSpec = true;

		if( desc.fVisibility < 1)
			bDontSpec = true;

		if( desc.bMultiplyColor)
			rscComponents |= RSC_FACTOR;
		
		if(desc.bVertexColor)
		{
			rscComponents |= RSC_VCOLOR /*| RSC_FACTOR*/;
			// ���� �ֵ�Ƽ���� ��� ���ؽ��� ���ĸ� �÷��� �ݿ������ش�.
			if( desc.pMtrl && desc.pMtrl->CheckFlag(RM_FLAG_ADDITIVE))
				rscComponents |= RSC_PPL_VALPHA_TO_VCOLOR;
			rscComponents &= rscNoLightingFilter;
		}
		else
		{
			if( desc.bUseShade == false )
			{
				// ������ ���� �� ���� ��� -> ��Ƽ�ö��� �÷��� ���� �� ���
				//rscComponents |= RSC_FACTOR;
				// �������� ������ �ʾƾ� �Ѵ�.
				rscComponents &= rscNoLightingFilter;
			}
			else
			{
				// ���� ������Ʈ�� ������Ʈ�� �н��� �������� �����Ƿ� �ش� ��ġ�� ����ŧ�� ���� ������ �ƴϴ�.
				// bVertexColor || !bUseShade ������ �̰��� ���� �����Ƿ� �������� ����ŧ�� ������Ʈ ���ص� ��.
				if( !bDontSpec)
					ApplySpecularComponent( rscComponents, desc.pMtrl, desc.bTexCoord);

				ApplyPartsColorComponent( rscComponents, desc.pMtrl, desc.bTexCoord);
				
			}
		}

		if( desc.pMtrl && desc.pMtrl->CheckShaderComponentFlag(RSC_PPL_GHOST) )
		{
			// ��Ʈ�� ���� �κ��� ���� ���� ��Ű�� ���ε�, �̶� �븻 ������ ������Ʈ�� �븻 ���ۿ��� �о� ���� �ȵȴ�.
			// �ֳĸ� ��Ʈ�� ���� ���� ������Ʈ�ε� �̴� ������Ʈ�� ���ۿ� ������ �ʱ� ������.
			// �׷��Ƿ� �Է� ���ؽ� ������ ������ Ʈ������ �� ����� ��� �Ͽ��� �Ѵ�.
			rscComponents |= RSC_EXPORT_NORMAL | RSC_PPL_VIEW_DIR | RSC_PPL_COS_VIEW | RSC_PPL_GHOST;
			// �������� ������ �ʾƾ� �Ѵ�.
			rscComponents &= rscNoLightingFilter;
		}
	}

	if( desc.pMtrl && desc.pMtrl->GetTexLayerByType(RTT_SELFILLUMINATION))
	{
		rscComponents |= GetTextureLayerReadComponent(desc.pMtrl, RTT_SELFILLUMINATION);
		rscComponents |= RSC_PPL_SELFILLUMINATION | RSC_ADD_COLOR;
	}

	CheckAndApplyInstancingComponent( rscComponents, desc.bInstancing);

	return rscComponents;
}

RSCID RShaderCompositeManager::GetTextureLayerReadComponent(RMaterial* pMtrl, RTextureType type)
{
	RPFC_THISFUNC;
	_ASSERT( type != RTT_UNKNOWN );

	if(pMtrl->GetTexLayerByType(type) == NULL)
	{
		return RSCID();
	}


	//-------------------------------------------------------------
	// �ؽ��� ���̾ ���� ����Ʈ
	// RTT_UNKNOWN, RTT_DIFFUSE, RTT_OPACITY, RTT_SPECULAR, RTT_NORMAL, RTT_SELFILLUMINATION,
	static const RSCID rscMap[] = {RSCID(), RSC_DIFFUSEMAP, RSC_OPACITYMAP, RSC_SPECULARMAP, RSC_NORMALMAP, RSC_SELFILLUMINATIONMAP};
	
	static const RSCID rscMapchannel[] = {RSCID(), RSC_TEXCOORD_DIFFUSE_MAPCHANNEL, RSC_TEXCOORD_OPACITY_MAPCHANNEL, RSC_TEXCOORD_SPECULAR_MAPCHANNEL, 
		RSC_TEXCOORD_NORMAL_MAPCHANNEL, RSC_TEXCOORD_SI_MAPCHANNEL};
	
	static const RSCID rscTransform[] = {RSCID(), RSC_TEXCOORD_DIFFUSE_TRANS, RSC_TEXCOORD_OPACITY_TRANS, RSC_TEXCOORD_SPECULAR_TRANS, 
		RSC_TEXCOORD_NORMAL_TRANS, RSC_TEXCOORD_SI_TRANS};
	//-------------------------------------------------------------

	bool bUseSecondMapChannel =(pMtrl->GetTexLayerByType(type)->GetMapChannel()>0);
	bool bUseTransform = pMtrl->IsUsePerLayerTransform(type);
	
	RSCID rscComponents =rscMap[type];
	_ASSERT( rscComponents != RSCID() );

	if(bUseSecondMapChannel)
		rscComponents |= rscMapchannel[type];
	if(bUseTransform)
		rscComponents |= rscTransform[type];


	return rscComponents;
}

RSCID RShaderCompositeManager::GetSpecularMapComponent(RMaterial* pMtrl)
{
	RPFC_THISFUNC;
	RSCID rscComponents;

	RCompositeLayer* pSpecularComposite = pMtrl->GetCompositeLayer(RTT_SPECULAR);
	if(pSpecularComposite==NULL)
		return GetTextureLayerReadComponent(pMtrl, RTT_SPECULAR);


	_ASSERT(pSpecularComposite->GetLayerCount()>1);


	COMPOSITE_LAYER_BLEND_MODE BlendMode = pSpecularComposite->GetBlendingMode(1);

	// Support only ColorBurn & ColorDodge two blend mode
	if( BlendMode!=LAYER_BLEND_COLORBURN && BlendMode!=LAYER_BLEND_COLORDODGE)
		return GetTextureLayerReadComponent(pMtrl, RTT_SPECULAR);

	if(BlendMode==LAYER_BLEND_COLORBURN)
		return RSC_DUAL_SPECULARMAP_COLORBURN;
	else
		return RSC_DUAL_SPECULARMAP_COLORDODGE;
}

RSCID RShaderCompositeManager::GetDiffuseComponent(RMaterial* pMtrl)
{
	RPFC_THISFUNC;
	if(pMtrl==NULL)
		return RSC_DIFFUSEMAP;

	if(pMtrl->GetTexLayerByType(RTT_DIFFUSE) == NULL)
		return RSCID();

	RCompositeLayer* DiffuseCompositeLayer = pMtrl->GetCompositeLayer(RTT_DIFFUSE);
	if(DiffuseCompositeLayer==NULL)
		return GetTextureLayerReadComponent(pMtrl, RTT_DIFFUSE);
	

	int nLayerCount = DiffuseCompositeLayer->GetLayerCount();
	if(nLayerCount <= 1)
		return GetTextureLayerReadComponent(pMtrl, RTT_DIFFUSE);;


	RSCID rscComponents = RSC_DIFFUSEMAPLAYER2 | GetDiffuseLayerBlendingComponent(0, DiffuseCompositeLayer->GetBlendingMode(1));
	
	if(nLayerCount > 2)
		rscComponents |= RSC_DIFFUSEMAPLAYER3 | GetDiffuseLayerBlendingComponent(1, DiffuseCompositeLayer->GetBlendingMode(2));

	rscComponents |= RSC_LAYER_RESULT_EXPORT;
	return rscComponents;
}

RSCID RShaderCompositeManager::GetDiffuseLayerBlendingComponent(int BlendingOrder, COMPOSITE_LAYER_BLEND_MODE mode)
{
	_ASSERT(BlendingOrder<2 && mode<LAYER_BLEND_MAX);

	RSCID ComponentList[2][LAYER_BLEND_MAX] = 
	{
		{
			RSC_LAYER_1STBLEND_NORMAL, RSC_LAYER_1STBLEND_AVERAGE, RSC_LAYER_1STBLEND_ADD, RSC_LAYER_1STBLEND_SUBTRACT, RSC_LAYER_1STBLEND_DARKEN, RSC_LAYER_1STBLEND_MULTIPLY,
				RSC_LAYER_1STBLEND_COLORBURN, 0, RSC_LAYER_1STBLEND_LIGHTEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		},

		{
			0, RSC_LAYER_2NDBLEND_AVERAGE, RSC_LAYER_2NDBLEND_ADD, RSC_LAYER_2NDBLEND_SUBTRACT, RSC_LAYER_2NDBLEND_DARKEN, RSC_LAYER_2NDBLEND_MULTIPLY,
				RSC_LAYER_2NDBLEND_COLORBURN, 0, RSC_LAYER_2NDBLEND_LIGHTEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		}
	};

	return ComponentList[BlendingOrder][mode];
}

// RSCID RShaderCompositeManager::GetMStageComponents()
// {
// 	RSCID rscComponents = RSC_POSITION | RSC_DIFFUSEMAP
// 		| RSC_LIGHTBUFFER_READ	// E_vLitColor, E_vPixelSpecular �ͽ���Ʈ
// 		| RSC_PPL_LIGHTING;		// �÷�, E_vLitColor, E_vIlluminationColor�� ������ ���� ���. �� �ڿ� ����ŧ�� ��� �ǰ���?
// 
// 	return rscComponents;
// }

RShaderComponent* RShaderCompositeManager::GetShaderComponent(unsigned int nComponentID)
{
	if( nComponentID>= m_shaderComponents.size() ) 
		return NULL;
	return m_shaderComponents[nComponentID];
}

void RShaderCompositeManager::SetViewLightingOnly(bool val)
{
	m_bViewLightingOnly = val;
	REngine::BroadcastEvent(RE_ENVIRONMENT_CHANGE);
}

void RShaderCompositeManager::SetSpecularEnable(bool val)
{
	m_bSpecularEnable = val;
	REngine::BroadcastEvent(RE_ENVIRONMENT_CHANGE);
}

void RShaderCompositeManager::SetDSF(bool val)
{
	m_bUseDSF = val;
}

void RShaderCompositeManager::SetUseNormalMap(bool val)
{
	m_bUseNormalMap = val;
}

void RShaderCompositeManager::ClearFogColor()
{
	m_cFogColor.SetVector3( RVector(0,0,0) );
}

void RShaderCompositeManager::RestoreFogColor()
{
	SetFogColor(m_dwFogColor);
}

void RShaderCompositeManager::SetFogColorForBlending(bool bAdditive)
{
	if(bAdditive)
		RShaderCompositeManager::ClearFogColor();
	else
		RShaderCompositeManager::RestoreFogColor();
}

void RShaderCompositeManager::SetTransformConstant(const RMatrix& matWorld, const RMatrix& matView, const RMatrix& matViewProj)
{
	RPFC_THISFUNC;
	RShaderCompositeManager::m_cWorld.SetMatrix(matWorld, false);
	RShaderCompositeManager::m_cWorldView.SetMatrix( matWorld * matView, false);
	RShaderCompositeManager::m_cWorldViewProj.SetMatrix( matWorld * matViewProj, false);	
}

void RShaderCompositeManager::SetNoiseFadeConstant(RVector4 vParam, RVector4 vColor1, RVector4 vColor2)
{
	RShaderCompositeManager::m_cNoiseFadeParam.SetVector4(vParam);
	RShaderCompositeManager::m_cNoiseFadeColor1.SetVector4( vColor1);
	RShaderCompositeManager::m_cNoiseFadeColor2.SetVector4( vColor2);
}

void RShaderCompositeManager::SetMultiLayerConstant(RTextureType type, RVector3 vOpacity, RVector3 vUseMaskMap)
{
	RPFC_THISFUNC;
	if(type==RTT_DIFFUSE)
	{
		RShaderCompositeManager::m_cDiffuseLayerOpacity.SetVector3(vOpacity);
		RShaderCompositeManager::m_cUseMaskMap.SetVector3(vUseMaskMap);
	}
	else if(type==RTT_NORMAL)
		RShaderCompositeManager::m_cNormalLayerOpacity.SetVector3(vOpacity);
	else if(type==RTT_SPECULAR)
		RShaderCompositeManager::m_cSpecularLayerOpacity.SetVector3(vOpacity);	
}

void RShaderCompositeManager::SetTextureByType(RTextureType type, RMaterial* pMaterial, RTEXTUREFILTERTYPE filter, int nMaxAnisotropy)
{
	RPFC_THISFUNC;
	RMtrlTexInfo* pTexInfo = pMaterial->GetTexLayerByType(type);
	if(pTexInfo==NULL)
		return;

	int nSampler=0;
	bool bUseComsiteLayer=false;

	switch(type)
	{
	case RTT_DIFFUSE:
		{
			nSampler = AS_DIFFUSE;
			bUseComsiteLayer =true;			
		}
		break;
	case RTT_NORMAL:
		{
			nSampler = AS_NORMALMAP;
			bUseComsiteLayer =true;			
		}
		break;
	case RTT_SPECULAR:
		{
			nSampler = AS_SPECULARMAP;
			bUseComsiteLayer =true;			
		}
		break;
	case RTT_SELFILLUMINATION:
		{
			nSampler = AS_SELFILLUMINATION;
			bUseComsiteLayer =false;			
		}
		break;

	}

	//SetTexture( nSampler, pTexInfo, filter, nMaxAnisotropy);
	SetTexture( nSampler, pTexInfo, (filter<RTF_COUNT) ? filter : REngine::GetConfig().GetTextureFilter(), (nMaxAnisotropy>-1) ? nMaxAnisotropy : REngine::GetConfig().GetTextureMaxAnisotropy() );
	if(bUseComsiteLayer)
		SetCompositeLayerRenderState(pMaterial->GetCompositeLayer(type));
}

void RShaderCompositeManager::SetTexture(int nStage, RMtrlTexInfo* pTexInfo, RTEXTUREFILTERTYPE filter, int nMaxAnisotropy)
{
	RPFC_THISFUNC;
	_ASSERT(0<=nStage && nStage<MAX_PIXELSAMPLER);

	if(pTexInfo==NULL)
	{
		REngine::GetDevice().SetTexture(nStage, NULL);
		return;
	}
	

	if(nStage<MAX_PIXELSAMPLER)
	{
		RVector4 vUVTransform = pTexInfo->GetUVTranform().GetOffsetScaleVector();
		if(RShaderCompositeManager::m_UVTransform[nStage] != vUVTransform)
		{
			RShaderCompositeManager::m_UVTransform[nStage] =vUVTransform;
			RShaderCompositeManager::m_cUVTransform.SetVector4Array(&vUVTransform, 1, nStage);
		}

		int nMapChannel = pTexInfo->GetMapChannel();
		if(RShaderCompositeManager::m_MapChannel[nStage] != nMapChannel)
		{
			RShaderCompositeManager::m_MapChannel[nStage] =nMapChannel;
			RShaderCompositeManager::m_cMapChannel.SetIntArray(&nMapChannel, 1, nStage);
		}
	}

	REngine::GetDevice().SetTexture(nStage, pTexInfo->GetTexture());
	REngine::GetDevice().SetTextureFilter(nStage, filter);
	REngine::GetDevice().SetTextureMaxAnisotropy( nStage, nMaxAnisotropy);
	REngine::GetDevice().SetTextureAddress(nStage, pTexInfo->GetTextureAddressU(), pTexInfo->GetTextureAddressV(), pTexInfo->GetTextureAddressU());
}

void RShaderCompositeManager::SetMaterialTexture(RMaterial* pMaterial, bool bUseNormalmap)
{
	RPFC_THISFUNC;
	//Diffuse �ؽ��� ����
	SetTexture(AS_DIFFUSE, pMaterial->GetTexLayerByType(RTT_DIFFUSE), REngine::GetConfig().GetTextureFilter(), REngine::GetConfig().GetTextureMaxAnisotropy());
	SetCompositeLayerRenderState(pMaterial->GetCompositeLayer(RTT_DIFFUSE));


	//Self-Illumination �ؽ��� ����
	SetTexture(AS_SELFILLUMINATION, pMaterial->GetTexLayerByType(RTT_SELFILLUMINATION));


	//Normal �ؽ��� ����
	if( bUseNormalmap && REngine::GetSceneManager().GetShaderCompositeManager()->GetUseNormalMap())
	{
		SetTexture(AS_NORMALMAP, pMaterial->GetTexLayerByType(RTT_NORMAL), REngine::GetConfig().GetTextureFilter(), REngine::GetConfig().GetTextureMaxAnisotropy());
		SetCompositeLayerRenderState(pMaterial->GetCompositeLayer(RTT_NORMAL));
	}


	//Specular �ؽ��� ����
	if( pMaterial->IsUseSpecular() )
	{
		SetTexture(AS_SPECULARMAP, pMaterial->GetTexLayerByType(RTT_SPECULAR), REngine::GetConfig().GetTextureFilter(), REngine::GetConfig().GetTextureMaxAnisotropy());
		SetCompositeLayerRenderState(pMaterial->GetCompositeLayer(RTT_SPECULAR));
	}


	// ���͸��� ���� ������ �ؽ��İ� ���� �ִ� ��츦 ����Ͽ� �̰����� ������ �ؽ��ĸ� �����Ѵ�. 
	// �ߺ� üũ�� �ǹǷ� �Ź� ���� �Ͽ��� ��� ���� ���̴�.
	REngine::GetDevice().SetTexture( AS_NOISEMAP, REngine::GetDevice().GetDefaultNoiseTexture());
}

void RShaderCompositeManager::SetMaterialShaderConstant(RMaterial* pMaterial, bool bUseSpecularValue, float fVisibility, float fSelfilluminationScale)
{
	if(pMaterial->CheckFlag(RM_FLAG_USEALPHATEST))
	{
		//m_cAlphaRef.SetFloat(ceilf( pMaterial->AlphaRefValue) / 255.0f);
		float fCeilf = ceilf(pMaterial->GetAlphaRefValue() *fVisibility);
		if( fCeilf == 255)
			m_cAlphaRef.SetFloat( 1);
		else
			m_cAlphaRef.SetFloat( fCeilf / 255.0f);
	}
	else
		m_cAlphaRef.SetFloat(0);
	

	if(pMaterial->GetTexLayerByType(RTT_SELFILLUMINATION))
		m_cIlluminationScale.SetFloat(pMaterial->GetSelfIlluminationScale()*fSelfilluminationScale);


	if( bUseSpecularValue && m_bSpecularEnable)
	{
		m_cGlossiness.SetFloat(pMaterial->GetGlossiness());
	}
	else
	{
		m_cGlossiness.SetFloat(0.0f);
	}

	if(pMaterial->IsUVAnimation())
		m_cTextureTransform.SetMatrix( pMaterial->GetTextureTransform() );
}

void RShaderCompositeManager::SetMaterialDecalRenderState(COMPOSITE_LAYER_INFO* pDecalLayer)
{
	if(pDecalLayer==NULL)
		return;

	RVector3 vUseMaskMap(0,0,0);
	RShaderCompositeManager::SetTexture(AS_DIFFUSELAYER1, pDecalLayer->pLayerMap, REngine::GetConfig().GetTextureFilter(), REngine::GetConfig().GetTextureMaxAnisotropy());
	if(pDecalLayer->pMaskMap)
	{
		RShaderCompositeManager::SetTexture(AS_DIFFUSELAYER1MASK, pDecalLayer->pMaskMap, REngine::GetConfig().GetTextureFilter(), REngine::GetConfig().GetTextureMaxAnisotropy());
		vUseMaskMap.y =1;
	}
	
	RShaderCompositeManager::SetMultiLayerConstant(RTT_DIFFUSE, RVector3(1,1,1), vUseMaskMap);
}

void RShaderCompositeManager::SetCullMode(bool bTwoSide)
{
	RPFC_THISFUNC;
	if(bTwoSide)
		REngine::GetDevice().SetCullMode(RCULL_NONE);
	else
		REngine::GetDevice().SetCullMode(RCULL_CW);
}

void RShaderCompositeManager::SetBlending(DWORD dwMaterialFlag, bool bAlphaMesh)
{
	if(dwMaterialFlag & RM_FLAG_ADDITIVE)
	{
		REngine::GetDevice().SetBlending(RBLEND_SRCALPHA,RBLEND_ONE);
	}
	else if( (dwMaterialFlag & RM_FLAG_USEOPACITY) || bAlphaMesh)
	{
		REngine::GetDevice().SetBlending(RBLEND_SRCALPHA,RBLEND_INVSRCALPHA);
	}
	else
	{
		REngine::GetDevice().SetBlending(RBLEND_NONE, RBLEND_NONE);
	}
}

}