#include "stdafx.h"

#include "REngine.h"
#include "RSceneManager.h"
#include "RCameraSceneNode.h"
#include "REffectInstance.h"
#include "REnvironmentSet.h"
#include "RShaderCompositeManager.h"

SUITE(Effect)
{
	/* ��ũ������ ���� ������ ����� �޶��� �ӽ÷� ��,2010.06.25 by ignore
	TEST(TestEffect)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();
		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		RVector vPos(0, 0, 0);
		RVector vDir(1,1,0);

		// ����Ʈ
		REffectInstanceSceneNode effectInstance;
		bool bRet = effectInstance.Create("ef_slash_dmg1");
		CHECK(bRet);

		if(!bRet) return;

		REffectSceneNode* pEffect = effectInstance.GetEffectSceneNode();
		pEffect->RemoveFromParent();

		// �ſ� �����ش�
		REngine::GetSceneManager().AttachEffect( pEffect, NULL, NULL );

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK(pCamera);
		pCamera->Set(vPos + vDir * 300 , -vDir);

		// 100 ms ���� �������
		for(int i=0;i<10;i++)
		{
			RUnitTestRunner::GetInstance().UpdateScene(true);
 			RUnitTestRunner::GetInstance().GrabScreenShot(NULL);
		}

		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("Effect");

		// �ν��Ͻ� ���ֱ�, ����Ʈ����
		pEffect->RemoveFromParent();
		delete pEffect;
		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();


	}
	*/

	TEST(TestTrailEffect)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();
		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		RVector vPos(0, 0, 0);
		RVector vDir(0,1,0);

		// ����Ʈ
		REffectInstanceSceneNode effectInstance;
		bool bRet = effectInstance.Create("ef_dt");
		CHECK(bRet);

		REffectSceneNode* pEffect = effectInstance.GetEffectSceneNode();
		pEffect->RemoveFromParent();

		// �ſ� �����ش�
		REngine::GetSceneManager().AttachEffect( pEffect, NULL, NULL );

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK(pCamera);
		pCamera->Set(vPos + vDir * 400 , -vDir);

		// 250 ms ���� �������
		for(int i=0;i<25;i++)
		{
			RUnitTestRunner::GetInstance().UpdateScene(true);
			RUnitTestRunner::GetInstance().GrabScreenShot(NULL);
		}
		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("EffectTrail");

		// �ν��Ͻ� ���ֱ�, ����Ʈ����
		pEffect->RemoveFromParent();
		delete pEffect;
		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
	}

	TEST(TestEffectDistortion)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();
		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		RVector vPos(0, 0, 30);
		RVector vDir(0,1,0);

		// ����Ʈ
		REffectInstanceSceneNode effectInstance;
		bool bRet = effectInstance.Create("ef_breath_stream_02");
		CHECK(bRet);

		REffectSceneNode* pEffect = effectInstance.GetEffectSceneNode();
		pEffect->RemoveFromParent();

		// �ſ� �����ش�
		REngine::GetSceneManager().AttachEffect( pEffect, NULL, NULL );

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK(pCamera);
		pCamera->Set(vPos + vDir * 150 , -vDir);

		//Distortion Ȱ��ȭ
		REngine::GetSceneManager().SetDistortionEnable(true);

		// 800 ms ���� �������
		for(int i=0;i<80;i++)
		{
			RUnitTestRunner::GetInstance().UpdateScene(true);
			RUnitTestRunner::GetInstance().GrabScreenShot(NULL);
		}
		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("EffectDistortion");

		// �ν��Ͻ� ���ֱ�, ����Ʈ����
		pEffect->RemoveFromParent();
		delete pEffect;
		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
	}

	const char *INSTANCING_FILE_NAME = "Login2.zone.xml";
	const char *INSTANCING_ENV_FILE_NAME = "Login2.zone.env.xml";

// ����Ʈ ��ƼŬ �׽�Ʈ�� ���� �� ���� ����� �ٸ���. �켱 �׽�Ʈ ��
// 	TEST(TestSoftParticle)
// 	{
// 		// ���� ����
// 		RUnitTestRunner::GetInstance().ResetEngine();
// 
// 		// �� �ε�
// 		bool bRet = REngine::GetSceneManager().LoadRootSceneFromXML(INSTANCING_FILE_NAME);
// 		CHECK(bRet);
// 
// 		// ī�޶� ����
// 		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
// 		CHECK( pCamera );
// 		pCamera->SetPosition(RVector3( 21742, 18343, 1953)); 
// 		pCamera->SetDirection( RVector( -0.021f, -0.485f, -0.873f) );
// 		pCamera->UpdateTransform();
// 
// 		// �ð� �������
// 		for(int i=0;i<200;i++)
// 		{
// 			RUnitTestRunner::GetInstance().UpdateScene(true);
// 			RUnitTestRunner::GetInstance().GrabScreenShot(NULL);
// 		}
// 
// 		// ��ũ�� ��
// 		RUnitTestRunner::GetInstance().UpdateScene(true);
// 		RUnitTestRunner::GetInstance().GrabScreenShot("TestSoftParticle");
// 	}
}
