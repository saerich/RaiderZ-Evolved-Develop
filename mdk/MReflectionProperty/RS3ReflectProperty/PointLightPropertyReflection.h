#pragma once

#include "BaseObject.h"
#include "SceneNodeCommonProperty.h"
#include "RPointLightSceneNode.h"

using namespace rs3;

class CPointLightPropertyRef : public CRTTIClass<CPointLightPropertyRef, CSceneNodeCommonProperty,SPID_POINT_LIGHT>
{
public:
	CPointLightPropertyRef() {}

	virtual void SetSceneNode(rs3::RSceneNode* pSceneNode)
	{ 
		CSceneNodeCommonProperty::SetSceneNode(pSceneNode);

		m_pPointLight =(RPointLightSceneNode*)pSceneNode; 

	}

	//------------------------------------------------------------------------------------------------------
	//Setter
	void SetAttenuationStart(float fAttenuStart)	{ m_pPointLight->SetAttenuationStart(fAttenuStart); }
	void SetRadius(float fRadius)					{ m_pPointLight->SetRadius(fRadius); }
	void SetColor(DWORD dwColor)					{ m_pPointLight->SetColorVector(ConvertRGBToVector3(dwColor)); }
	void SetIntensity(float fIntensity)				{ m_pPointLight->m_fApplyIntensity =fIntensity; m_pPointLight->m_fIntensity = fIntensity; }
	void SetAreaRangeX(float fX)					{ RVector3 vArea =m_pPointLight->GetAreaRange(); vArea.x=fX; m_pPointLight->SetAreaRange(vArea); }
	void SetAreaRangeY(float fY)					{ RVector3 vArea =m_pPointLight->GetAreaRange(); vArea.y=fY; m_pPointLight->SetAreaRange(vArea); }
	void SetAreaRangeZ(float fZ)					{ RVector3 vArea =m_pPointLight->GetAreaRange(); vArea.z=fZ; m_pPointLight->SetAreaRange(vArea); }
	//------------------------------------------------------------------------------------------------------
	


	//------------------------------------------------------------------------------------------------------
	//Getter
	float GetAttenuationStart()						{ return m_pPointLight->GetAttenuationStart(); }
	float GetRadius()								{ return m_pPointLight->GetRadius(); }
	DWORD GetColor()								{ return ConvertVector3ToRGB(m_pPointLight->GetColorVector()); }
	float GetIntensity()							{ return m_pPointLight->m_fIntensity; }
	float GetAreaRangeX()							{ return m_pPointLight->GetAreaRange().x; }
	float GetAreaRangeY()							{ return m_pPointLight->GetAreaRange().y; }
	float GetAreaRangeZ()							{ return m_pPointLight->GetAreaRange().z; }
	//------------------------------------------------------------------------------------------------------



	static void RegisterReflection()
	{
		const char* strCategory= "Point Light �Ӽ�";
		{
			RegisterProperty<float>("���ҽ���",&GetAttenuationStart,&SetAttenuationStart, 0, 0, "���� ���� �Ÿ�", strCategory);
			RegisterProperty<float>("����",&GetRadius,&SetRadius, 0, 0, "����", strCategory);
			RegisterProperty<DWORD>("��",&GetColor,&SetColor, 0, 0, "��", strCategory, PE_COLOR);
			RegisterProperty<float>("����",&GetIntensity,&SetIntensity, 0, 0, "����", strCategory);
		}


		const char* strCategoryArea= "Area Light �Ӽ�";
		{
			RegisterProperty<float>("Area Light ����(X)",&GetAreaRangeX,&SetAreaRangeX, 0, 0, "Area Light ����(X)", strCategoryArea);
			RegisterProperty<float>("Area Light ����(Y)",&GetAreaRangeY,&SetAreaRangeY, 0, 0, "Area Light ����(Y)", strCategoryArea);
			RegisterProperty<float>("Area Light ����(Z)",&GetAreaRangeZ,&SetAreaRangeZ, 0, 0, "Area Light ����(Z)", strCategoryArea);
		}
	}

private:
	RPointLightSceneNode* m_pPointLight;
};