#include "stdafx.h"

#include "REngine.h"
#include "RSceneManager.h"
#include "RActor.h"
#include "RMesh.h"
#include "RMeshMgr.h"
#include "RCameraSceneNode.h"
#include "REnvironmentSet.h"

#include "RActorController.h"
#include "RAnimationController.h"

SUITE(Actor)
{
	TEST(TestLoadActorSimple)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();
		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		// �ν��Ͻ� ����
		RActor* pActor = static_cast< RActor* >( REngine::GetSceneManager().CreateSceneNode( rs3::RSID_ACTOR ) );
		CHECK(pActor);

		bool bRet = pActor->Create("goblin_Mage.elu");
		CHECK(bRet);

		// ���Ŵ����� ���̱�
		REngine::GetSceneManager().AddSceneNode(pActor);

		RVector vPos(0, 0, 0);
		RVector vDir(0,1,0);

		// ���� ���� - �ִϸ��̼� / ��ǥ
		pActor->SetTransform( vPos, -vDir );
		pActor->SetAnimation( "idle" );

		RSceneNodeUpdateVisitor visitor;
		pActor->Traverse(&visitor);

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK(pCamera);
		pCamera->Set(vPos + vDir * 150 + RVector(0,0,70), -vDir);

		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("LoadActorSimple");

		// �ν��Ͻ� ���ֱ�
		pActor->RemoveFromParent();
		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
		delete pActor;
	}

	TEST(TestLoadActorCustom)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();
		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		// �ν��Ͻ� ����
		RActor* pActor = new RActor;
		CHECK(pActor);

		// �ڿ�����
		RMesh* pMesh = REngine::GetSceneManager().CreateResource<RMesh>("goblin_Mage.elu");
		bool bRet = pMesh->BuildAll();
		CHECK(bRet);

		// �ڿ����κ��� �ν��Ͻ� ä���
		bRet = pActor->Create(pMesh);
		CHECK(bRet);

		// ���Ŵ����� ���̱�
		REngine::GetSceneManager().AddSceneNode(pActor);

		RVector vPos(0, 0, 0);
		RVector vDir(0,1,0);

		// ���� ���� - �ִϸ��̼� / ��ǥ
		pActor->SetAnimation( "idle" );
		pActor->SetTransform( vPos, -vDir );
		RSceneNodeUpdateVisitor visitor;
		pActor->Traverse(&visitor);
		//pActor->Update();

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK(pCamera);
		pCamera->Set(vPos + vDir * 150 + RVector(0,0,70), -vDir);

		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("LoadActorCustom");

		// �ν��Ͻ� ���ֱ�, �ν��Ͻ��� �������� �ڿ��� ���� ��������.
		pActor->RemoveFromParent();
		delete pActor;

		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
	}

	TEST(TestActorAlpha2PassRedering)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();
		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		// �ν��Ͻ� ����
		const int instanceNum = 3;
		const float offset = 25.0f;

		// �ν��Ͻ� ����
		RActor* pActor[instanceNum];

		RVector vPos(0, 0, 0);
		RVector vDir(0, 1, 0);
		RSceneNodeUpdateVisitor visitor;

		for( int i = 0; i < instanceNum; ++i )
		{
			pActor[i] = static_cast< RActor* >( REngine::GetSceneManager().CreateSceneNode( rs3::RSID_ACTOR ) );
			CHECK(pActor[i]);

			// �ڿ����κ��� �ε�
			bool bRet = pActor[i]->Create("goblin_Mage.elu");
			CHECK(bRet);

			// ���� 2�н� ������ ����.
			if( i & 1 )
			{
			}
			else
			{
				pActor[i]->SetAlpha2PassRendering(true);
				pActor[i]->SetVisibility( 0.5f );
			}

			// ���Ŵ����� ���̱�
			REngine::GetSceneManager().AddSceneNode(pActor[i]);

			// ���� ���� - �ִϸ��̼� / ��ǥ
			pActor[i]->SetAnimation( "idle" );
			pActor[i]->SetTransform( vPos + RVector( i * offset, i * -offset, 0.f ), -vDir );
			pActor[i]->Traverse(&visitor);
			//pActor[i]->Update();
		}

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK(pCamera);
		pCamera->Set(vPos + vDir * 150 + RVector(offset,0,70), -vDir);

		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("Alpha2PassRendering");

		// �ν��Ͻ� ���ֱ�
		for( int i = 0; i < instanceNum; ++i )
		{
			pActor[i]->RemoveFromParent();
			delete pActor[i];
		}
		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
	}

	TEST(TestLoadActorGhostFX)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();
		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		// �ν��Ͻ� ����
		RActor* pActor = new RActor;
		CHECK(pActor);

		// �ڿ�����
		RMesh* pMesh = REngine::GetSceneManager().CreateResource<RMesh>("ghost_knight.elu");
		bool bRet = pMesh->BuildAll();
		CHECK(bRet);

		// �ڿ����κ��� �ν��Ͻ� ä���
		bRet = pActor->Create(pMesh);
		CHECK(bRet);

		// ���Ŵ����� ���̱�
		REngine::GetSceneManager().AddSceneNode(pActor);

		RVector vPos(0, 0, 0);
		RVector vDir(0,1,0);

		// ���� ���� - �ִϸ��̼� / ��ǥ
		pActor->SetAnimation( "idle" );
		pActor->SetTransform( vPos, -vDir );
		RSceneNodeUpdateVisitor visitor;
		pActor->Traverse(&visitor);
		//pActor->Update();

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK(pCamera);
		pCamera->Set(vPos + vDir * 200 + RVector(0,0,100), -vDir);

		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("LoadActorGhostFX");

		// �ν��Ͻ� ���ֱ�, �ν��Ͻ��� �������� �ڿ��� ���� ��������.
		pActor->RemoveFromParent();
		delete pActor;
		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
	}

	TEST(TestAlign)
	{
		// ��Ģ�߿� Align �� AlignZ �� �ߵǴ��� �׽�Ʈ.
		// ��ũ������ ��ҿ� �޸� �Һ� ������, ������ Align �������� AlignZ �̴�.

		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();
		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		// �ν��Ͻ� ����
		RActor* pActor = new RActor;
		CHECK(pActor);

		// �ڿ�����
		RMesh* pMesh = REngine::GetSceneManager().CreateResource<RMesh>("test_align.elu");
		bool bRet = pMesh->BuildAll();
		CHECK(bRet);

		// �ڿ����κ��� �ν��Ͻ� ä���
		bRet = pActor->Create(pMesh);
		CHECK(bRet);

		// ���Ŵ����� ���̱�
		REngine::GetSceneManager().AddSceneNode(pActor);

		RVector vPos(0, 0, 0);
		RVector vDir(0,1,1);

		// �ִϸ��̼��� �ɶ� �׽�Ʈ�� �ʿ��ϸ� �߰�����
// 		// ���� ���� - �ִϸ��̼� / ��ǥ
// 		pActor->SetAnimation( "idle" );
// 		pActor->SetTransform( vPos, -vDir );

		RSceneNodeUpdateVisitor v;
		pActor->Traverse( &v );

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK(pCamera);
		pCamera->Set(vPos + vDir * 100 + RVector(0,0,100), -vDir);

		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("ActorAlign");

		// �ν��Ͻ� ���ֱ�, �ν��Ͻ��� �������� �ڿ��� ���� ��������.
		pActor->RemoveFromParent();
		delete pActor;
		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();

	}

	TEST(TestActorAdditive)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();
		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		// �ν��Ͻ� ����
		RActor* pActor = static_cast< RActor* >( REngine::GetSceneManager().CreateSceneNode( rs3::RSID_ACTOR ) );
		CHECK(pActor);

		// �ڿ����κ��� �ε�
		bool bRet = pActor->Create("ritz_sign_adventurer_01.elu");
		CHECK(bRet);

		// ���Ŵ����� ���̱�
		REngine::GetSceneManager().AddSceneNode(pActor);

		RVector vPos(0, 0, 0);
		RVector vDir(0,1,0);
		RVector vDir2(1,0,0);

		// ���� ���� - �ִϸ��̼� / ��ǥ
		pActor->SetTransform( vPos, -vDir2 );
		pActor->SetAnimation( "idle" );

		RSceneNodeUpdateVisitor visitor;
		pActor->Traverse(&visitor);

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK(pCamera);
		pCamera->Set(vPos + vDir * 700 + RVector(0,0,300), -vDir);

		REngine::GetSceneManager().SetHDREnable(true);
		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("ActorAdditive");

		// �ν��Ͻ� ���ֱ�
		pActor->RemoveFromParent();
		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
		delete pActor;
	}

	TEST(TestActorSelfIllumination)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();
		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		// �ν��Ͻ� ����
		RActor* pActor = static_cast< RActor* >( REngine::GetSceneManager().CreateSceneNode( rs3::RSID_ACTOR ) );
		CHECK(pActor);

		// �ڿ����κ��� �ε�
		bool bRet = pActor->Create("cerberus.elu");
		CHECK(bRet);

		// ���Ŵ����� ���̱�
		REngine::GetSceneManager().AddSceneNode(pActor);

		RVector vPos(0, 0, 0);
		RVector vDir(0,1,0);

		// ���� ���� - �ִϸ��̼� / ��ǥ
		pActor->SetTransform( vPos, -vDir );
		pActor->SetAnimation( "idle" );

		RSceneNodeUpdateVisitor visitor;
		pActor->Traverse(&visitor);

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK(pCamera);
		pCamera->Set(vPos + vDir * 700 + RVector(0,0,300), -vDir);

		REngine::GetSceneManager().SetHDREnable(true);
		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("ActorSelfIllumination");

		// �ν��Ͻ� ���ֱ�
		pActor->RemoveFromParent();
		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
		delete pActor;
	}

	TEST(TestMultiVertexAnimation)	// ���� �޽��� ����ϴ� �ٸ� �������� ���ؽ� �ִϸ��̼� �׽�Ʈ
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();
		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		RVector vPos(0, 0, 0);
		RVector vDir(0,1,0);

		RActor* pActor[2];
		for( int i = 0; i < 2; ++ i )
		{
			// �ν��Ͻ� ����
			pActor[i] = static_cast< RActor* >( REngine::GetSceneManager().CreateSceneNode( rs3::RSID_ACTOR ) );
			CHECK(pActor[i]);
		
			// �ڿ����κ��� �ε�
			bool bRet = pActor[i]->Create("goblin_bomb.elu");
			CHECK(bRet);

			// ���Ŵ����� ���̱�
			REngine::GetSceneManager().AddSceneNode(pActor[i]);

			pActor[i]->SetAnimation( "bomb" );
		}

		pActor[0]->SetTransform(vPos + RVector( -100.0f, 0.0f, 0.0f), vDir);
		pActor[1]->SetTransform(vPos + RVector(  100.0f, 0.0f, 0.0f), vDir);

		pActor[0]->GetAnimationController().SetFrame(0);
		pActor[1]->GetAnimationController().SetFrame(1000);

		RSceneNodeUpdateVisitor visitor;
		pActor[0]->Traverse(&visitor);
		pActor[1]->Traverse(&visitor);

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK(pCamera);
		pCamera->Set(vPos + vDir * 500 + RVector(0,0,0), -vDir);

		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("TestMultiVertexAnimation");

		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();

		for( int i = 0; i < 2; ++i )
		{
			pActor[i]->RemoveFromParent();
			delete pActor[i];
		}
	}

	TEST(TestActorDiffusemapBlend)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();

		RActor* pActor[3];
		// �ν��Ͻ� ����

		for(int i=0;i<3;i++)
		{
			pActor[i] = static_cast< RActor* >( REngine::GetSceneManager().CreateSceneNode( rs3::RSID_ACTOR ) );
			CHECK( pActor[i]);

			// �ڿ����κ��� �ε�
			bool bRet = pActor[i]->Create("sphere_tiling.elu");
			CHECK(bRet);

			// ���Ŵ����� ���̱�
			REngine::GetSceneManager().AddSceneNode(pActor[i]);

			RVector vPos(0, 0, 0);
			RVector vinstancePos[3] = { RVector( -180, 0, 0), RVector( 0, 0, 0), RVector( 180, 0, 0)};
			RVector vDir(0,1,0);

			// ���� ���� - �ִϸ��̼� / ��ǥ
			pActor[i]->SetTransform( vinstancePos[i], -vDir );
			pActor[i]->SetAnimation( "idle" );

			RSceneNodeUpdateVisitor visitor;
			pActor[i]->Traverse(&visitor);

			// ī�޶� ����
			RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
			CHECK(pCamera);
			pCamera->Set(vPos + vDir * 400 + RVector(0,0,0), -vDir);

			switch( i)
			{
			case 0:
				break;
			case 1:
				pActor[i]->ChangeMatetial( 0, 0, 1, 1.0f);
				break;
			case 2:
				pActor[i]->ChangeMatetial( 0, 0, 1, 0);
				break;
			}
		}

		for(int j=0;j<50;j++)
		{
			RUnitTestRunner::GetInstance().UpdateScene(true);
			RUnitTestRunner::GetInstance().GrabScreenShot(NULL);
		}

		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("ActorDiffusemapBlend");

		for(int i=0;i<3;i++)
		{
			// �ν��Ͻ� ���ֱ�
			pActor[i]->RemoveFromParent();
			delete pActor[i];
		}
	}
}
