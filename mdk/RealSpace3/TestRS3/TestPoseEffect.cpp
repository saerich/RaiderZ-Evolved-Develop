#include "stdafx.h"

#include "REngine.h"
#include "RSceneManager.h"
#include "REnvironmentSet.h"
#include "RFont.h"
#include "RTextSceneNode.h"

SUITE(PostEffect)
{
	const char *INSTANCING_FILE_NAME = "BirdTown2.zone.xml";
	const char *INSTANCING_ENV_FILE_NAME = "BirdTown2.zone.env.xml";

	RActor* CreateActor( RVector3& _vPos, RVector3& _vDir, RVector3& _vColor )
	{
		RActor* pActor = static_cast< RActor* >( REngine::GetSceneManager().CreateSceneNode( rs3::RSID_ACTOR ) );
		if( NULL == pActor )
			return NULL;

		bool bRet = pActor->Create("goblin_mage.elu");

		// ���Ŵ����� ���̱�
		REngine::GetSceneManager().AddSceneNode(pActor);

		// ���� ���� - ��ǥ
		pActor->SetTransform( _vPos, _vDir );

		RSceneNodeUpdateVisitor visitor;
		pActor->Traverse(&visitor);

		// �ܰ��� �ѱ�
		pActor->EnablePEEdge( _vColor );

		return pActor;
	}

	TEST(TestDOFnBulr)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();

		// �� �ε�
		bool bRet = REngine::GetSceneManager().LoadWorldSceneNodeFromRootXMLFile( REngine::GetSceneManager().GetCurrentRootSceneNode(true), INSTANCING_FILE_NAME);
		CHECK(bRet);

		// ȯ�� ����� ����
		REnvironmentSet* pEnvironmentSet = new REnvironmentSet;

		// ���Ϸκ��� �����
		bRet = pEnvironmentSet->CreateFromFile(INSTANCING_ENV_FILE_NAME, false);
		CHECK(bRet);

		// ���Ŵ����� ���ϱ�
		REngine::GetSceneManager().AddSceneNode(pEnvironmentSet);

		// ȯ�� ����
		bRet = pEnvironmentSet->SetCurrentEnvSet(REnvironment::NIGHT, REnvironment::SUNNY, false, 0.f, false);
		CHECK(bRet);

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK( pCamera );

		pCamera->SetPosition( RVector3( 29395, 28137, 2488));			
		//pCamera->SetDirection( RVector(0.012f, 0.854f, -0.518f) );
		pCamera->SetDirection( RVector(0.012f, 0.854f, -0.518f) );
		pCamera->UpdateViewFrustrum();
		pCamera->UpdateTransform();


		// HDR�� SI �׽�Ʈ���� �������Ƿ� ��ŵ ---------------------------------------------

		// DOF -----------------------------------------------------------------------------
		// ī�޶� ��ó�� �ޱ�
		REngine::GetSceneManager().SetDOFEnable(true);
		REngine::GetSceneManager().SetDOFFocalDist(0);
		REngine::GetSceneManager().SetDOFNearBlurDist( 0);
		REngine::GetSceneManager().SetDOFFarBlurDist( 5500);
		REngine::GetSceneManager().SetDOFCOCSize( 7);
		// ��ũ�� ��
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("TestDOF");
		// ��ó�� ����
		REngine::GetSceneManager().SetDOFEnable(false);

		// SetScreenMotionBlur --------------------------------------------------------------
		// ī�޶� ��ó�� �ޱ�
		REngine::GetSceneManager().SetScreenMotionBlurEnable(true);
		REngine::GetSceneManager().SetScreenMotionBlurActive(true);
		REngine::GetSceneManager().SetScreenMotionBlurSamplerCount(10);
		REngine::GetSceneManager().SetScreenMotionBlurSpeed(9);
		REngine::GetSceneManager().SetScreenMotionBlurBasisByEpicenter();
		// ��ũ�� ��
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("TestScreenMotionBlur");
		// ��ó�� ����
		REngine::GetSceneManager().SetScreenMotionBlurEnable(false);


		// ȯ�� ����� �Ҹ�
		pEnvironmentSet->RemoveFromParent();
		delete pEnvironmentSet;
	}

	TEST(TestPostEffectEdge)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();
		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		// �ܰ��� ����Ʈ ����Ʈ Ȱ��ȭ
		REngine::GetSceneManager().SetPEEdgeEnable( true );
		REngine::GetSceneManager().SetPEQuality( 2, 2 );
		REngine::GetSceneManager().SetPEFactorTickNess( 5.6f, 3.1f, 2.75f );

		RVector vPos(0, 0, 0);
		RVector vDir(0,1,0);

		RActor* pActor	= CreateActor(  RVector(-60.f, 0, 0), -vDir, RVector3(1, 0, 1) );
		RActor* pActor2 = CreateActor(  RVector(60.f, 0, 0), -vDir, RVector3(1, 1, 0) );

		CHECK(pActor);
		CHECK(pActor2);

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK(pCamera);
		pCamera->Set(vPos + vDir * 150 + RVector(0,0,70), -vDir);

		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("TestPostEffectEdge");

		pActor->DisablePEEdge();
		pActor2->DisablePEEdge();

		// �ν��Ͻ� ���ֱ�
		pActor->RemoveFromParent();
		pActor2->RemoveFromParent();

		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
		delete pActor;
		delete pActor2;
	}
}
