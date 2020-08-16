#include "stdafx.h"

SUITE(ETC)
{
	TEST(TestMaterialDecal)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();

		// �ν��Ͻ� ����
		RActor* pActor = static_cast< RActor* >( REngine::GetSceneManager().CreateSceneNode( rs3::RSID_ACTOR ) );
		CHECK(pActor);

		// �ڿ����κ��� �ε�
		bool bRet = pActor->Create("hf_face_01.elu");
		CHECK(bRet);

		// ���Ŵ����� ���̱�
		REngine::GetSceneManager().AddSceneNode(pActor);

		RVector vPos(0, 0, 0);
		RVector vDir(0,1,0);

		// ���� ���� - �ִϸ��̼� / ��ǥ
		pActor->SetTransform( vPos, -vDir );

		//Material Decal ����
		RTextureDesc desc;
		desc.TextureName = "scream.dds";
		desc.UVTransform.SetScale(RVector2(4,4));
		desc.UVTransform.SetOffset(RVector2(-0.3f,0.3f));
		desc.textureAddressU =RTADDRESS_WRAP;
		desc.textureAddressV =RTADDRESS_WRAP;

		pActor->GetMaterialDecal()->AddDecalLayer("at_face", &desc, NULL);


		RSceneNodeUpdateVisitor visitor;
		pActor->Traverse(&visitor);

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK(pCamera);
		pCamera->Set(vPos + vDir * 30 + RVector(0,0,160), -vDir);

		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("MaterialDecal");

		// �ν��Ͻ� ���ֱ�
		pActor->RemoveFromParent();

		delete pActor;
	}

	TEST(TestMaterialDecalMaskMap)
	{
		// ���� ����
		RUnitTestRunner::GetInstance().ResetEngine();

		// �ν��Ͻ� ����
		RActor* pActor = static_cast< RActor* >( REngine::GetSceneManager().CreateSceneNode( rs3::RSID_ACTOR ) );
		CHECK(pActor);

		// �ڿ����κ��� �ε�
		bool bRet = pActor->Create("hf_face_01.elu");
		CHECK(bRet);

		// ���Ŵ����� ���̱�
		REngine::GetSceneManager().AddSceneNode(pActor);

		RVector vPos(0, 0, 0);
		RVector vDir(0,1,0);

		// ���� ���� - �ִϸ��̼� / ��ǥ
		pActor->SetTransform( vPos, -vDir );

		//Material Decal ����
		RTextureDesc DiffuseDesc;
		DiffuseDesc.TextureName = "red256.jpg";

		RTextureDesc MaskDesc;
		MaskDesc.TextureName = "scream.dds";
		MaskDesc.UVTransform.SetScale(RVector2(4,4));
		MaskDesc.UVTransform.SetOffset(RVector2(-0.3f,0.3f));
		MaskDesc.textureAddressU =RTADDRESS_WRAP;
		MaskDesc.textureAddressV =RTADDRESS_WRAP;

		pActor->GetMaterialDecal()->AddDecalLayer("at_face", &DiffuseDesc, &MaskDesc);


		RSceneNodeUpdateVisitor visitor;
		pActor->Traverse(&visitor);

		// ī�޶� ����
		RCameraSceneNode* pCamera = REngine::GetSceneManager().GetPrimaryCamera();
		CHECK(pCamera);
		pCamera->Set(vPos + vDir * 30 + RVector(0,0,160), -vDir);

		// ��ũ����
		RUnitTestRunner::GetInstance().UpdateScene(true);
		RUnitTestRunner::GetInstance().GrabScreenShot("MaterialDecalMasking");

		// �ν��Ͻ� ���ֱ�
		pActor->RemoveFromParent();

		delete pActor;
	}
}