#include "stdafx.h"

#include "REngine.h"
#include "RSceneManager.h"
#include "RCameraSceneNode.h"

#include "RActor.h"
#include "REffectInstance.h"

SUITE(AttachEffect)
{
	TEST(TestAttachEffectToSceneNode)
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
		pActor->SetAnimation( "idle" );
		pActor->SetTransform( vPos, -vDir );
		RSceneNodeUpdateVisitor visitor;
		pActor->Traverse(&visitor);
		//pActor->Update();

		// ����Ʈ
		REffectInstanceSceneNode effectInstance;
		bRet = effectInstance.Create("ef_head_icon_q02");
		CHECK(bRet);

		REffectSceneNode* pEffect = effectInstance.GetEffectSceneNode();
		pEffect->RemoveFromParent();

		// ����Ʈ ������
		RMatrix matTransOffset;
		matTransOffset.MakeIdentity();
		matTransOffset.SetTranslation( RVector(0,0,120) );

		// ���Ŵ����� ���� ���Ϳ� ����Ʈ ���̱�
		REngine::GetSceneManager().AttachEffect(pEffect, pActor, NULL, matTransOffset);

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK(pCamera);
		pCamera->Set(vPos + vDir * 150 + RVector(0,0,70), -vDir);

		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("AttachEffectToSceneNode");

		// �ν��Ͻ� ���ֱ�, ����Ʈ����
		pEffect->RemoveFromParent();
		delete pEffect;
		pActor->RemoveFromParent();
		delete pActor;
		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
	}

	TEST(TestAttachLoopEffectToSceneNode)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();
		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();

		// �ν��Ͻ� ����
		RActor* pActor = static_cast< RActor* >( REngine::GetSceneManager().CreateSceneNode( rs3::RSID_ACTOR ) );
		CHECK(pActor);

		// �ڿ����κ��� �ε�
		bool bRet = pActor->Create("goblin_Mage.elu");
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

		// ����Ʈ
		REffectInstanceSceneNode effectInstance;
		bRet = effectInstance.Create("ef_head_icon_l00");
		CHECK(bRet);

		REffectSceneNode* pEffect = effectInstance.GetEffectSceneNode();
		pEffect->RemoveFromParent();

		// ����Ʈ ������
		RMatrix matTransOffset;
		matTransOffset.MakeIdentity();
		matTransOffset.SetTranslation( RVector(0,0,120) );

		// ���Ŵ����� ���� ���Ϳ� ����Ʈ ���̱�
		REngine::GetSceneManager().AttachEffect(pEffect, pActor, NULL, matTransOffset);

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK(pCamera);
		pCamera->Set(vPos + vDir * 150 + RVector(0,0,70), -vDir);

		// loop �� �� �Ǵ��� �������� �ð��� 3�� ���� �������, �׷��� ������ ������ loop ��������
		for(int i=0;i<300;i++)
		{
			if(pEffect)
				REffectSceneNode::ControlEffectSceneNodeLife(&pEffect);	// app �� ������
			RUnitTestRunner::GetInstance().UpdateScene(true);
			RUnitTestRunner::GetInstance().GrabScreenShot(NULL);
		}

		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("AttachLoopEffectToSceneNode");

		if(pEffect)
		{
			// �ν��Ͻ� ���ֱ�, ����Ʈ����
			pEffect->RemoveFromParent();
			delete pEffect;
		}

		pActor->RemoveFromParent();
		delete pActor;
		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
	}

	TEST(TestAttachEffectToActorNode)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();
		// ����Ʈ ����
		RUnitTestRunner::GetInstance().AddDirectionalLight();
		// ����Ʈ ���ֱ�
		RUnitTestRunner::GetInstance().DelDirectionalLight();
	}
}
