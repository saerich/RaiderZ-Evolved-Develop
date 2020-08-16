#include "StdAfx.h"
#include "XFaceAnimationTC.h"

#define PLAYER_BLINK_ANIMATION_NAME	"none_blink"
#define NPC_BLINK_ANIMATION_NAME	"none_blink"
#define BLINK_ANIMATION_BONE_NAME	"Bip01 Head"
#define FACE_ANIMATION_BONE_NAME	"Bip01 Head"
#define EYE_ANIMATION_BONE_NAME		"Dummy_eyes"

XFaceAnimationTC::XFaceAnimationTC( XCharacter* pCharacter )
:XTransformController(pCharacter)
{
	if(m_pActor == NULL)
		return;

	m_pBlinkAnimationController = new XRAnimationController(RSNCTRL_CUSTOMUPDATE, RSNCTRL_CUSTOMUPDATE);
	m_pActor->AddController(m_pBlinkAnimationController);
	
	m_fBlinkElapsed = 0.0f;
	m_fBlinkStartTime = 0.0f;

	m_bNPC = false;
	m_bEnableBlink = true;
}

XFaceAnimationTC::~XFaceAnimationTC( void )
{
	SAFE_DELETE(m_pBlinkAnimationController);
	DelAllAnimationController(m_vecFaceAnimationController);
	DelAllAnimationController(m_vecEyeAnimationController);
}

void XFaceAnimationTC::Update( float fDelta )
{
	BlinkPlay(fDelta);

	FaceAnimationEndPlay(fDelta);

	//////////////////////////////////////////////////////////////////////////
	// Update

	// Bink �ִϸ��̼��� �⺻���� �Ѵ�.
	m_pBlinkAnimationController->OnUpdate();

	// ���̽� �ִϸ��̼� ������Ʈ
	UpdateAnimationController(m_vecFaceAnimationController);

	// ���̽� �ִϸ��̼��� �������... Blink Animation �ڵ����� �Ѿ��.
	if(m_vecFaceAnimationController.empty())
	{
		m_pBlinkAnimationController->Play();
	}

	// �� �ִϸ��̼��� ���� �������� �Ѵ�.
	if(m_vecEyeAnimationController.empty() == false)
	{
		UpdateAnimationController(m_vecEyeAnimationController);
	}
}

void XFaceAnimationTC::UpdateAnimationController( FaceAnimationList& animationList )
{
	FaceAnimationList::iterator it = animationList.begin();
	while(it != animationList.end())
	{
		it->pFaceAnimationController->OnUpdate();
		//if(it->pFaceAnimationController->IsPlayDone())
		//{
		//	it->pFaceAnimationController->Reset();
		//	m_pActor->RemoveController(it->pFaceAnimationController);
		//	SAFE_DELETE(it->pFaceAnimationController);
		//	it = animationList.erase(it);
		//	continue;
		//}

		++it;
	}
}

void XFaceAnimationTC::BlinkPlay(float fDelta)
{
	// ���̽� �ִϸ��̼��� �۵����̸� BlinkAnimation�� �۵����� �ʴ´�.
	// ������ Blink�� ����� ���� �ʴ´ٸ�...
	if(m_vecFaceAnimationController.empty() == false || m_bEnableBlink == false)
		return;

	m_fBlinkElapsed += fDelta;

	if(m_fBlinkElapsed < m_fBlinkStartTime)
		return;

	if( m_pActor->GetUsable() == false )
		return;

	RActorNodeSelection nodeSelection;
	if(nodeSelection.SelectNodeAndChildren(m_pActor, BLINK_ANIMATION_BONE_NAME, true) == false)
		return ;

	RPLAYINFO playInfo;
	playInfo.blendMode = RBLENDMODE_BLEND;
	playInfo.pNodeSet = &nodeSelection;

	string strAnimationName = PLAYER_BLINK_ANIMATION_NAME;
	if(m_bNPC)
		strAnimationName = NPC_BLINK_ANIMATION_NAME;

	m_pBlinkAnimationController->SetAnimation( strAnimationName.c_str(), &playInfo );
	m_pBlinkAnimationController->Play();

	m_fBlinkElapsed = 0.0f;
	m_fBlinkStartTime = MMath::RandomNumber(3.0f, 5.0f);
}

void XFaceAnimationTC::PreUpdate( float fDelta )
{
	if(m_vecFaceAnimationController.empty())
	{
		m_pBlinkAnimationController->OnPreUpdate();
	}
	else
	{	
		for(FaceAnimationList::iterator it = m_vecFaceAnimationController.begin(); it != m_vecFaceAnimationController.end(); ++it)
			it->pFaceAnimationController->OnPreUpdate();

		for(FaceAnimationList::iterator it = m_vecEyeAnimationController.begin(); it != m_vecEyeAnimationController.end(); ++it)
			it->pFaceAnimationController->OnPreUpdate();
	}
}

void XFaceAnimationTC::StartFaceAnimation( tstring& strAniName, bool bEye )
{
	// �غ� �ȵȻ��¿����� ����Ҽ� ����.
	ANIMATION_CHECK_STATE eState = CheckAnimationUsable(strAniName);
	if( eState != AS_LOADED)
		return;

	bool bResult = false;

	if(bEye)
	{
		bResult = SetFaceAnimation(strAniName, EYE_ANIMATION_BONE_NAME, bEye, m_vecEyeAnimationController);
	}
	else
	{
		bResult = SetFaceAnimation(strAniName, FACE_ANIMATION_BONE_NAME, bEye, m_vecFaceAnimationController);
	}

	if(bResult == false)
		return;

	m_pBlinkAnimationController->Stop();
	m_pBlinkAnimationController->OnUpdate();
}

void XFaceAnimationTC::EndFaceAnimation( tstring& strAniName, float fEndBledTime )
{
	EndAnimationController(m_vecFaceAnimationController, strAniName, fEndBledTime);
	EndAnimationController(m_vecEyeAnimationController, strAniName, fEndBledTime);
}

void XFaceAnimationTC::FaceAnimationEndPlay( float fDelta )
{
	UpdateEndAnimationController(m_vecFaceAnimationController, fDelta);
	UpdateEndAnimationController(m_vecEyeAnimationController, fDelta);

	// ���̽� �ִϸ��̼��� �������... Blink Animation �ڵ����� �Ѿ��.
	if(m_vecFaceAnimationController.empty())
	{
		m_pBlinkAnimationController->Play();
	}
}

void XFaceAnimationTC::UpdateEndAnimationController( FaceAnimationList& animationList, float fDelta )
{
	FaceAnimationList::iterator it = animationList.begin();
	while(it != animationList.end())
	{
		if(it->fFaceEndElapsed > 0.0f)
		{
			it->fFaceEndElapsed -= fDelta;
			float fWeight = it->fFaceEndElapsed / it->fFaceEndTime;

			if (fWeight <= 0.0f)	// ���� �Ϸ�
			{
				it->pFaceAnimationController->Reset();
				m_pActor->RemoveController(it->pFaceAnimationController);
				SAFE_DELETE(it->pFaceAnimationController);
				it = animationList.erase(it);
				continue;
			}
			else
			{
				it->pFaceAnimationController->SetWeight(fWeight);
				it->pFaceAnimationController->SetBlendMode(RBLENDMODE_BLEND);
			}
		}

		++it;
	}
}

bool XFaceAnimationTC::SetFaceAnimation( tstring& strAniName, string strNodeName, bool bEye, FaceAnimationList& animationList )
{
	RActorNodeSelection nodeSelection;
	if(nodeSelection.SelectNodeAndChildren(m_pActor, strNodeName.c_str(), true) == false)
		return false;

	XRAnimationController *	pFaceAnimationController = new XRAnimationController(RSNCTRL_CUSTOMUPDATE, RSNCTRL_CUSTOMUPDATE);
	m_pActor->AddController(pFaceAnimationController);

	RPLAYINFO playInfo;
	playInfo.blendMode = RBLENDMODE_OVERWRITE;
	playInfo.pNodeSet = &nodeSelection;

	pFaceAnimationController->SetAnimation( MLocale::ConvTCHARToAnsi(strAniName.c_str()).c_str(), &playInfo );
	pFaceAnimationController->Play();

	stFaceAnimationList stFaceAnimation;
	stFaceAnimation.strAniName = strAniName;
	stFaceAnimation.pFaceAnimationController = pFaceAnimationController;
	stFaceAnimation.bEye = bEye;

	animationList.push_back(stFaceAnimation);

	return true;
}

void XFaceAnimationTC::DelAllAnimationController( FaceAnimationList& animationList )
{
	for(FaceAnimationList::iterator it = animationList.begin(); it != animationList.end(); ++it)
	{
		SAFE_DELETE(it->pFaceAnimationController);
	}

	animationList.clear();
}

void XFaceAnimationTC::EndAnimationController( FaceAnimationList& animationList, tstring& strAniName, float fEndBledTime )
{
	for(FaceAnimationList::iterator it = animationList.begin(); it != animationList.end(); ++it)
	{
		if(it->strAniName == strAniName)
		{
			// ���� Ÿ���� 0�ʶ��...
			if(fEndBledTime == 0.0f)
			{
				it->pFaceAnimationController->Reset();
				m_pActor->RemoveController(it->pFaceAnimationController);
				SAFE_DELETE(it->pFaceAnimationController);
				it = animationList.erase(it);
			}
			else
			{
				it->fFaceEndElapsed = fEndBledTime;
				it->fFaceEndTime = fEndBledTime;
			}

			return;
		}
	}
}

void XFaceAnimationTC::EndAllFaceLoopAnimation()
{
	DelAllAnimationController(m_vecFaceAnimationController);
	DelAllAnimationController(m_vecEyeAnimationController);
}
