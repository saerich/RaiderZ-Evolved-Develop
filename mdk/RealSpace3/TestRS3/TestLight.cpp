#include "stdafx.h"

#include "REngine.h"
#include "RSceneManager.h"
#include "REnvironmentSet.h"
#include "RPointLightSceneNode.h"
#include "RSpotLightSceneNode.h"

SUITE(ETC)
{
	const char *ZONE_FILE_NAME = "Login.zone.xml";
	const char *ZONE_ENV_FILE_NAME = "Login.zone.env.xml";

	TEST(TestLight)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();

		// �� �ε�
		bool bRet = REngine::GetSceneManager().LoadWorldSceneNodeFromRootXMLFile( REngine::GetSceneManager().GetCurrentRootSceneNode(true),ZONE_FILE_NAME);
		CHECK(bRet);

		// ȯ�� ����� ����
		REnvironmentSet* pEnvironmentSet = new REnvironmentSet;

		// ���Ϸκ��� �����
		bRet = pEnvironmentSet->CreateFromFile(ZONE_ENV_FILE_NAME, false);
		CHECK(bRet);

		// ���Ŵ����� ���ϱ�
		REngine::GetSceneManager().AddSceneNode(pEnvironmentSet);

		// ȯ�� ����
		bRet = pEnvironmentSet->SetCurrentEnvSet(REnvironment::DAWN, REnvironment::SUNNY, false, 0.f, false);
		CHECK(bRet);
		REngine::GetSceneManager().UseLighting(true);

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK( pCamera );
		pCamera->SetPosition(RVector3(17000.0f, 6300.0f, 800.0f)); // �� ������ ������
		pCamera->SetDirection( RVector(-0.46f, 0.78f, -0.4f) );

		//Point Light ����
		RPointLightSceneNode PointLight;
		PointLight.SetPosition(16000.0f, 7000.0f, 100.0f);
		PointLight.SetRadius(1000.0f);
		PointLight.SetColorRGB(0xFFFF0000);
		PointLight.m_fApplyIntensity =2;
		PointLight.SetAreaRange(RVector3(500, 10, 10));
		PointLight.UpdateTransform();

		//Spot Light ����
		RSpotLightSceneNode SpotLight;
		SpotLight.SetTransform(RVector3(17000.0f, 7500.0f, 600.0f), RVector3(0,0,-1), RVector3(0,1,0));
		SpotLight.SetSpotLightTexture("batman-logo.jpg");
		SpotLight.m_fApplyIntensity =2;
		SpotLight.UpdateTransform();
		RSceneNodeUpdateVisitor visitor;
		SpotLight.Traverse(&visitor);
		

		// ���Ŵ����� ���̱�
		REngine::GetSceneManager().AddSceneNode(&PointLight);
		REngine::GetSceneManager().AddSceneNode(&SpotLight);


		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("TestLight");

		// Light ���ֱ�
		PointLight.RemoveFromParent();
		SpotLight.RemoveFromParent();
		
		// ȯ�� ����� �Ҹ�
		pEnvironmentSet->RemoveFromParent();
		delete pEnvironmentSet;
	}
}
