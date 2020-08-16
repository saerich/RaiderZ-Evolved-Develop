#pragma once

#include "BaseObject.h"
#include "SceneNodeCommonProperty.h"
#include "RSpotLightSceneNode.h"

using namespace rs3;

class CSpotLightPropertyRef : public CRTTIClass<CSpotLightPropertyRef, CSceneNodeCommonProperty,SPID_SPOT_LIGHT>
{
public:
	CSpotLightPropertyRef() {}

	virtual void SetSceneNode(rs3::RSceneNode* pSceneNode)
	{ 
		CSceneNodeCommonProperty::SetSceneNode(pSceneNode);

		m_pSpotLight =(RSpotLightSceneNode*)pSceneNode; 
	}


	//------------------------------------------------------------------------------------------------------
	//Setter
	void SetMapFileName(const char* strFilePath)	{ m_pSpotLight->SetSpotLightTexture(strFilePath); }
	void SetFar(float fFar)							{ m_pSpotLight->SetFar(fFar); }
	void SetFov(float fFov)							{ m_pSpotLight->SetFov(fFov); }
	void SetAttenuationStart(float fAttenuStart)	{ m_pSpotLight->SetAttenuationStart(fAttenuStart); }
	void SetColor(DWORD dwColor)					{ m_pSpotLight->SetColorVector(ConvertRGBToVector3(dwColor)); }
	void SetIntensity(float fIntensity)				{ m_pSpotLight->m_fApplyIntensity =fIntensity; m_pSpotLight->m_fIntensity = fIntensity; }
	void SetShadowCast(bool bShadowCast)			{ m_pSpotLight->SetShadowCast(bShadowCast); }
	void SetUserRenderMinArea(bool bEnable)			{ m_pSpotLight->SetUseRenderMinArea(bEnable); }
	void SetRenderMinArea(float fMinArea)			{ m_pSpotLight->SetRenderMinArea(fMinArea); }
	//------------------------------------------------------------------------------------------------------



	//------------------------------------------------------------------------------------------------------
	//Getter
	const char* GetMapFileName()					{ return m_pSpotLight->GetSpotLightTextureFileName().c_str(); }
	float GetFar()									{ return m_pSpotLight->GetFar(); }
	float GetFov()									{ return m_pSpotLight->GetFov(); }
	float GetAttenuationStart()						{ return m_pSpotLight->GetAttenuationStart(); }
	DWORD GetColor()								{ return ConvertVector3ToRGB(m_pSpotLight->GetColorVector()); }
	float GetIntensity()							{ return m_pSpotLight->m_fIntensity; }
	bool GetShadowCast()							{ return m_pSpotLight->GetShadowCast(); }
	bool GetUserRenderMinArea()						{ return m_pSpotLight->GetUseRenderMinArea(); }
	float GetRenderMinArea()						{ return m_pSpotLight->GetRenderMinArea(); }
	//------------------------------------------------------------------------------------------------------



	static void RegisterReflection()
	{
		const char* strCategory = "Spot Light �Ӽ�";
		{
			RegisterProperty<const char*>("������ ����",&GetMapFileName,&SetMapFileName, 0, 0, "������ ����", strCategory, PE_FILENAME);
			RegisterProperty<float>("����",&GetFar,&SetFar, 0, 0, "����", strCategory);
			RegisterProperty<float>("����",&GetFov,&SetFov, 0, 0, "����", strCategory);
			RegisterProperty<float>("�������",&GetAttenuationStart,&SetAttenuationStart, 0, 0, "���� ���� �Ÿ�", strCategory);
			RegisterProperty<DWORD>("��",&GetColor,&SetColor, 0, 0, "��", strCategory, PE_COLOR);
			RegisterProperty<float>("����",&GetIntensity,&SetIntensity, 0, 0, "����", strCategory);
		}


		const char* strCategoryShadow = "�׸��� �Ӽ�";
		{
			RegisterProperty<bool>("�׸��� ����",&GetShadowCast,&SetShadowCast, 0, 0, "�׸��� ���� ����", strCategoryShadow);
			RegisterProperty<bool>("�׸��� ���� �ּҳ��� ���",&GetUserRenderMinArea,&SetUserRenderMinArea, 0, 0, "�׸��� ���� �ּҳ��� ��� ����", strCategoryShadow);
			RegisterProperty<float>("�׸��� ���� �ּҳ���(%)",&GetRenderMinArea,&SetRenderMinArea, 0, 0, "Light �����󿡼� ������ ���� ������Ʈ�� �׸��ڰ� �������� ����", strCategoryShadow);
		}
	}


private:
	RSpotLightSceneNode* m_pSpotLight;
};