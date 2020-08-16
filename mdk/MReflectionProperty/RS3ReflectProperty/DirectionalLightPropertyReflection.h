#pragma once

#include "BaseObject.h"
#include "SceneNodeCommonProperty.h"
#include "RDirectionalLightSceneNode.h"

#include "RShadowStrategy.h"

using namespace rs3;

class CDirectionalLightPropertyRef : public CRTTIClass<CDirectionalLightPropertyRef, CSceneNodeCommonProperty,SPID_DIRECTIONAL_LIGHT>
{
public:
	CDirectionalLightPropertyRef()
	{
		m_shadowDesc.eMethod = SHADOW_NONE;
	}

	virtual void SetSceneNode(rs3::RSceneNode* pSceneNode)
	{ 
		CSceneNodeCommonProperty::SetSceneNode(pSceneNode);

		m_pDirectionalLight =(RDirectionalLightSceneNode*)pSceneNode; 
	}

	
	//------------------------------------------------------------------------------------------------------
	//Setter
	void SetShaderLuminosity(float fLuminosity) { m_pDirectionalLight->SetShadowLuminosity(fLuminosity); }
	void SetPower(float fPower)					{ m_pDirectionalLight->SetPower(fPower); }
	void SetAmbient(DWORD dwAmbient)			{ m_pDirectionalLight->SetLightAmbient(ConvertRGBToVector4(dwAmbient)); }
	void SetDiffuse(DWORD dwDiffuse)			{ m_pDirectionalLight->SetLightDiffuse(ConvertRGBToVector4(dwDiffuse)); }
	void SetSpecular(DWORD dwSpecular)			{ m_pDirectionalLight->SetLightSpecular(ConvertRGBToVector4(dwSpecular)); }


	void SetShadowMethod(int e)
	{
		switch(e)
		{
		case 0:
			m_shadowDesc.eMethod = SHADOW_NONE;
			break;
		case 1:
			m_shadowDesc.eMethod = SHADOW_PROJECTION;
			break;
		case 2:
			m_shadowDesc.eMethod = SHADOW_ESM;
			break;
		case 3:
			m_shadowDesc.eMethod = SHADOW_PSSM;
			break;
		}

		REngine::GetSceneManager().SetRealtimeShadowMethod(m_shadowDesc);
	}
	void SetShadowMapCount(int nCount) { m_shadowDesc.nTextureNum=min(MAX_PSSM_SPLIT_COUNT,nCount); REngine::GetSceneManager().SetRealtimeShadowMethod(m_shadowDesc); }
	void SetShadowDistance0(float fDist) { m_shadowDesc.fSplitDistance[0] = fDist; REngine::GetSceneManager().SetRealtimeShadowMethod(m_shadowDesc); }
	void SetShadowDistance1(float fDist) { m_shadowDesc.fSplitDistance[1] = fDist; REngine::GetSceneManager().SetRealtimeShadowMethod(m_shadowDesc); }
	void SetShadowDistance2(float fDist) { m_shadowDesc.fSplitDistance[2] = fDist; REngine::GetSceneManager().SetRealtimeShadowMethod(m_shadowDesc); }
	void SetShadowDistance3(float fDist) { m_shadowDesc.fSplitDistance[3] = fDist; REngine::GetSceneManager().SetRealtimeShadowMethod(m_shadowDesc); }
	void SetSkySpecular(float fScale)	{ m_pDirectionalLight->SetSkySpecular(fScale); }
	//------------------------------------------------------------------------------------------------------
	


	//------------------------------------------------------------------------------------------------------
	//Getter
	float GetShaderLuminosity()		{ return m_pDirectionalLight->GetShadowLuminosity(); }
	float GetPower()				{ return m_pDirectionalLight->GetPower(); }
	DWORD GetAmbient()				{ return ConvertVector4ToRGB(m_pDirectionalLight->GetAmbient()); }
	DWORD GetDiffuse()				{ return ConvertVector4ToRGB(m_pDirectionalLight->GetDiffuse()); }
	DWORD GetSpecular()				{ return ConvertVector4ToRGB(m_pDirectionalLight->GetSpecular()); }

	int GetShadowMethod()					
	{
		switch(m_shadowDesc.eMethod)
		{
		case SHADOW_PROJECTION:
			return 1;
		case SHADOW_ESM:
			return 2;
		case SHADOW_PSSM:
			return 3;
		}

		return 0; 
	}
	int GetShadowMethodMin() { return 0; }
	int GetShadowMethodMax() { return 3; }
	int GetShadowMapCount() { return m_shadowDesc.nTextureNum; }
	float GetShadowDistance0() { return m_shadowDesc.fSplitDistance[0]; }
	float GetShadowDistance1() { return m_shadowDesc.fSplitDistance[1]; }
	float GetShadowDistance2() { return m_shadowDesc.fSplitDistance[2]; }
	float GetShadowDistance3() { return m_shadowDesc.fSplitDistance[3]; }
	float GetSkySpecular()	{ return m_pDirectionalLight->GetSkySpecular(); }
	//------------------------------------------------------------------------------------------------------



	static void RegisterReflection()
	{
		const char* strCategory = "Directional Light �Ӽ�";
		{
			RegisterProperty<float>("���� �ּ�",&GetShaderLuminosity,&SetShaderLuminosity, 0, 0, "���� �ּ�", strCategory);
			RegisterProperty<DWORD>("Ambient ��",&GetAmbient,&SetAmbient, 0, 0, "Ambient ��", strCategory, PE_COLOR);
			RegisterProperty<DWORD>("Diffuse ��",&GetDiffuse,&SetDiffuse, 0, 0, "Diffuse ��", strCategory, PE_COLOR);
			RegisterProperty<DWORD>("Specular ��",&GetSpecular,&SetSpecular, 0, 0, "Specular ��", strCategory, PE_COLOR);
			RegisterProperty<float>("Sky Specular",&GetSkySpecular,&SetSkySpecular, 0, 0, "Specular Specular", strCategory);
		}
				

		const char* strCategoryShadow = "�׸��� �Ӽ�";
		{
			RegisterProperty<int>("�׸��� ���",&GetShadowMethod,&SetShadowMethod, &GetShadowMethodMin, &GetShadowMethodMax, "@enum[None,Projection,ESM,PSSM]�׸��� ���", strCategoryShadow);
			RegisterProperty<int>("�׸��� ���� ��",&GetShadowMapCount,&SetShadowMapCount, 0, 0, "�׸��ڸ� �� (PSSM ��� �ɼ�)", strCategoryShadow);
			RegisterProperty<float>("1���� �Ÿ�",&GetShadowDistance0,&SetShadowDistance0, 0, 0, "1���� �Ÿ�", strCategoryShadow);
			RegisterProperty<float>("2���� �Ÿ�",&GetShadowDistance1,&SetShadowDistance1, 0, 0, "2���� �Ÿ� (PSSM ��� �ɼ�)", strCategoryShadow);
			RegisterProperty<float>("3���� �Ÿ�",&GetShadowDistance2,&SetShadowDistance2, 0, 0, "3���� �Ÿ� (PSSM ��� �ɼ�)", strCategoryShadow);
			RegisterProperty<float>("4���� �Ÿ�",&GetShadowDistance3,&SetShadowDistance3, 0, 0, "4���� �Ÿ� (PSSM ��� �ɼ�)", strCategoryShadow);
		}
	}

private:
	RDirectionalLightSceneNode* m_pDirectionalLight;

	RShadowDesc			m_shadowDesc;

	RVector3 m_vSkyDirection;
	RVector3 m_vGroundDirection;
};

class CSubLightPropertyRef : public CRTTIClass<CSubLightPropertyRef, CSceneNodeCommonProperty,SPID_SUB_LIGHT>
{
public:
	CSubLightPropertyRef()
	{
	}

	virtual void SetSceneNode(rs3::RSceneNode* pSceneNode)
	{ 
		CSceneNodeCommonProperty::SetSceneNode(pSceneNode);

		m_pSubLight =(RSubLightSceneNode*)pSceneNode; 
	}

	void SetColor(DWORD dwColor)	{ m_pSubLight->SetColorVector(ConvertRGBToVector3(dwColor)); }
	DWORD GetColor()				{ return ConvertVector3ToRGB(m_pSubLight->GetColorVector()); }


	static void RegisterReflection()
	{
		const char* strCategory = "���� Light �Ӽ�";

		RegisterProperty<DWORD>("��",&GetColor,&SetColor, 0, 0, "��", strCategory, PE_COLOR);
	}


private:
	RSubLightSceneNode* m_pSubLight;
};