#ifndef __RMATERIAL_H
#define __RMATERIAL_H

//#pragma once

//#pragma warning(disable:4786)

#include "RPreDefine.h"
#include <list>
#include <vector>
#include <string>

#include "RTypes.h"
#include "RDevice.h"
#include "RShaderComponentID.h"
#include "RMaterialTexture.h"
#include "RCompositeLayer.h"

using namespace std;

#include "MXml.h"


namespace rs3 {


//------------------------------------------------------------------------------------------
// material�� �÷��׵�
#define RM_FLAG_ADDITIVE				0x0001
#define RM_FLAG_USEOPACITY				0x0002
#define RM_FLAG_TWOSIDED				0x0004
#define RM_FLAG_NOTWALKABLE				0x0008		// �������������� �̸� ǥ��.
#define RM_FLAG_CASTSHADOW				0x0010
#define RM_FLAG_RECEIVESHADOW			0x0020
#define RM_FLAG_PASSTHROUGH				0x0040		// �� �÷��װ� ������ �浹üũ�� ���� �ʴ´�.
#define RM_FLAG_HIDE					0x0080		// �׸����ʴ´�.
#define RM_FLAG_PASSBULLET				0x0100		// �Ѿ��� ����Ѵ�
#define RM_FLAG_PASSROCKET				0x0200		// ����/����ź���� ����Ѵ�.
#define RM_FLAG_USEALPHATEST			0x0400		// alpha test �� (������ ��� .. )
#define RM_FLAG_NOSHADE					0x0800		// ���̵� ����
#define RM_FLAG_AI_NAVIGATION			0x1000		// ����Ʈ AI �׺���̼� ��
#define RM_FLAG_USEPARTSCOLOR			0x80000		// _pc �� �����÷��� ���̴� ��
#define RM_FLAG_TEXTURE_TRANSFORM		0x100000	// �ؽ��� Ʈ�������� ����ϰų� uv �ִϸ��̼��� �ִ°��
#define RM_FLAG_EXTRA_UV				0x200000	// ������ UVä���� ���

#define RM_FLAG_COLLISION_MESH			0x2000
#define RM_FLAG_DUMMY_MESH				0x4000
#define RM_FLAG_BONE_MESH				0x10000		// bone mesh

// CLOTH
#define RM_FLAG_CLOTH_MESH				0x8000	// �ʰ�
#define RM_FLAG_COLLISION_CLOTH_MESH	0x20000	// �ʰ� �浹��
#define RM_FLAG_COLLISION_MESHONLY		0x40000	// �浹 ���� (Box, Sphere, Capsule �� ����)

#define RM_FLAG_OCCLUDER				0x1000000	// ��Ŭ���
//------------------------------------------------------------------------------------------






enum ALPHACHANNEL_USAGE
{
	ALPHAUSAGE_ALPHA =0,
	ALPHAUSAGE_NOISE,
	ALPHAUSAGE_SPECULAR,	
	ALPHAUSAGE_SHINIESS,
	ALPHAUSAGE_MAX
};

class RS_API RMaterial 
{
public:
	RMaterial();
	virtual ~RMaterial();


	void				CreateTexInfo(int nLayer);				// Texture Layer ������ �����Ѵ�. ������ ����� �ٽø���.


	bool				Open(MXmlElement *pElement);
	void				Save(MXmlElement *pElement);

	bool				CheckFlag(DWORD flag) { return (m_dwFlags & flag)!=0; }
	bool				CheckShaderComponentFlag(RSCID shaderComponentFlag) { return m_rscShaderComponent.HasComponent(shaderComponentFlag); }



	void				SetName(std::string name)	{ m_Name=name; }
	void				SetAmbient(RVector3 ambient) { m_Ambient=ambient; }
	void				SetDiffuse(RVector3 diffuse)	{ m_Diffuse=diffuse; }
	void				SetSpecular(RVector3 specular)	{ m_Specular=specular; }
	void				SetSpecularLevel(float fLevel)	{ m_fSpecularLevel=fLevel; }
	void				SetGlossiness(float fGlossiness)	{ m_fGlossiness=fGlossiness; }
	void				SetFlag(DWORD flag)	{ m_dwFlags=flag; }	
	void				SetAlphaCmpFunc(RCMPFUNC func)	{ m_AlphaCmpFunc=func; }	
	void				SetSelfIlluminationScale(float fScale)	{ m_fSelfIlluminationScale=fScale; }

	void				AddFlag(DWORD flag)	{ m_dwFlags|=flag; }
	void				AddMtrlTexInfo(int nLayer, RMtrlTexInfo TexInfo)	{ m_TexInfo[nLayer].push_back(TexInfo); }
	void				AddShaderComponent(RSCID components)	{ m_rscShaderComponent |= components; }
	void				AddCompositeLayer(RCompositeLayer CompositeLayer)	{ m_CompositeLayer.push_back(CompositeLayer); }



	std::string			GetName()												{ return m_Name; }
	DWORD				GetFlag()												{ return m_dwFlags; }
	int					GetMaxLayer()											{ return m_nMaxTexLayer; }
	RTextureType		GetTextureTypeForAlphaUsage(ALPHACHANNEL_USAGE usage)	{ return m_TextureTypeForAlphaUsage[usage]; }
	DWORD				GetAlphaRefValue()										{ return m_AlphaRefValue; }
	float				GetSelfIlluminationScale()								{ return m_fSelfIlluminationScale; }
	float				GetGlossiness()											{ return m_fGlossiness; }
	size_t				GetTextureCountOfLayer(int nLayer)						{ return m_TexInfo[nLayer].size(); }
	RMtrlTexInfo*		GetTextureInfo(int nLayer, int nTexIndex)				{ return &m_TexInfo[nLayer][nTexIndex]; }


	RMtrlTexInfo*		GetTexLayer(int nLayer);
	RMtrlTexInfo*		GetTexLayerByType(RTextureType nType);
	RMtrlTexInfo*		GetTexLayerByNameFromType(RTextureType nType, const std::string& TexName);
	RCompositeLayer*	GetCompositeLayer(RTextureType textureType);


	bool				IsUseSpecular()								{ return m_bUseSpecular; }
	bool				IsUseUVAnimation()							{ return m_bAnimation; }
	bool				IsUVAnimation()								{ return m_UVAniInfo.IsHaveAnimationKey(); }
	bool				IsValidTextureType( RTextureType nType)		{ return ( RTT_UNKNOWN < nType && nType < RTT_MAX);	}
	bool				IsUseAlpha()								{ return CheckFlag(RM_FLAG_USEALPHATEST) || CheckFlag(RM_FLAG_USEOPACITY); }
	bool				IsUseExtraUV();
	bool				IsUsePerLayerTransform(RTextureType textureType);


	void				SetAllLayerUVTransform(RUVTransform UVTrans);
	void				UpdateUVAnimation(UINT nFrame);
	static void 		GetTextureTransform(RMatrix *pOut, const RVector vOffset, const RVector &vScale);
	RMatrix				GetTextureTransform();
	void				OptimizeUVTransform();

	

private:
	bool				SearchFromTextureName( RTextureType _Search, RTextureType _From );


	std::string m_Name;

	int			m_nMaxTexLayer;					// �ؽ��� �ִ� ���̾� ����.

	RVector3	m_Ambient;
	RVector3	m_Diffuse;
	RVector3	m_Specular;

	float		m_fSelfIlluminationScale;

	float		m_fSpecularLevel;	// ������ ����(���) ( Max���� 100 �϶� 1/100 ���� ���� ��´� )
	float		m_fGlossiness;	// ������ ���� ( Max���� 100 �϶� 1/100 ���� ���� ��´� )

	DWORD		m_dwFlags;
	
	RCMPFUNC	m_AlphaCmpFunc;
	DWORD		m_AlphaRefValue;



	RUVAnimationInfo m_UVAniInfo;
	RUVTransform m_vUVTransform;

	bool		m_bAnimation;
	bool		m_bUseSpecular;

	RSCID		m_rscShaderComponent;	// Special Effect Shader Flag

	std::vector<RMtrlTexInfo>* m_TexInfo;
	std::vector<RCompositeLayer> m_CompositeLayer;

	RTextureType	m_TextureTypeForAlphaUsage[ALPHAUSAGE_MAX];
};

}
#endif
