#pragma once

#include "BaseObject.h"
#include "SceneNodeCommonProperty.h"
#include "RPointLightSceneNode.h"

using namespace rs3;

class CActorPropertyRef : public CRTTIClass<CActorPropertyRef, CSceneNodeCommonProperty,0x03>
{
public:
	CActorPropertyRef() {}

	virtual void SetSceneNode(rs3::RSceneNode* pSceneNode)
	{ 
		CSceneNodeCommonProperty::SetSceneNode(pSceneNode);

		m_pActor =(RActor*)pSceneNode; 

	}

	//------------------------------------------------------------------------------------------------------
	//Setter
	void SetCollisionCreation(bool bEnable)			{ m_pActor->SetCollisionCreationEnable(bEnable); }
	void SetAutoAnimation(bool bEnable)				
	{ 
		m_pActor->m_bAutoAnimationEnable = bEnable;
		if (bEnable)
			m_pActor->LoadAutoAnimation();
		else
			m_pActor->Stop();
	}
	void SetShadowReceiver(bool bEnable)			{ m_pActor->SetShadowReceiver(bEnable); }
	//------------------------------------------------------------------------------------------------------



	//------------------------------------------------------------------------------------------------------
	//Getter
	bool GetCollisionCreation()						{ return m_pActor->GetCollisionCreationEnable(); }
	bool GetAutoAnimation()							{ return m_pActor->m_bAutoAnimationEnable; }
	bool GetShadowReceiver()						{ return m_pActor->IsShadowReceiver(); }
	//------------------------------------------------------------------------------------------------------



	static void RegisterReflection()
	{
		RegisterProperty<bool>("�浹����",&GetCollisionCreation,&SetCollisionCreation, 0, 0, "�浹 �޽� ����", "Actor �Ӽ�");
		RegisterProperty<bool>("�ִϸ��̼�",&GetAutoAnimation,&SetAutoAnimation, 0, 0, "�ִϸ��̼�", "Actor �Ӽ�");
		RegisterProperty<bool>("Shadow Receiver",&GetShadowReceiver,&SetShadowReceiver, 0, 0, "Shadow Receiver", "Actor �Ӽ�");
	}

private:
	RActor* m_pActor;
};