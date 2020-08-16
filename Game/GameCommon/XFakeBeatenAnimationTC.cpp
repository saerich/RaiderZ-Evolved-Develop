#include "stdafx.h"
#include "XFakeBeatenAnimationTC.h"
#include "MLocale.h"


XFakeBeatenAnimationTC::XFakeBeatenAnimationTC( XCharacter* pCharacter )
:XTransformController(pCharacter)
, m_pCurrentReActionInfo(NULL)
, m_fWeaponVisibility(1.0f)
{
	if(m_pActor == NULL)
		return;

	m_pRAnimationController = new XRAnimationController(RSNCTRL_CUSTOMUPDATE, RSNCTRL_CUSTOMUPDATE);
	m_pActor->AddController(m_pRAnimationController);
}

XFakeBeatenAnimationTC::~XFakeBeatenAnimationTC( void )
{
	SAFE_DELETE(m_pRAnimationController);
}

void XFakeBeatenAnimationTC::Update( float fDelta )
{
	if(m_pRAnimationController->IsPlayDone() == false)
	{
		CheckWeaponVisibility();

		m_pRAnimationController->OnUpdate();
	}
}

void XFakeBeatenAnimationTC::Stop()
{

}

bool XFakeBeatenAnimationTC::StartBeaten( const TCHAR* szCurAni, const float fWeightRate, const float fSpeedRate, const TCHAR* szAniPrefix/*=NULL*/ )
{
	m_pCurrentReActionInfo = NULL;

	// �غ� �ȵȻ��¿����� ����ũ��ư�� ����Ҽ� ����.
	ANIMATION_CHECK_STATE eState = CheckAnimationUsable(tstring(szCurAni));
	if( eState != AS_LOADED)
		return false;

	m_fWeaponVisibility = 1.0f;

	XReActionAnimationInfo* pInfo = m_pActor->GetReActionAnimationInfo();
	ReAction_DATA* pReActionData = &m_ReActionDefalutInfo;
	bool bUseDefalut = true;

	if(pInfo)
	{
		ReAction_DATA* pGetReActionData = pInfo->Get(szCurAni);
		if(pGetReActionData)
		{
			pReActionData = pGetReActionData;
			bUseDefalut = false;

			// ���׼� �ִϸ��̼��� ���ٸ� ����Ʈ�� ���ش�.
			if(pGetReActionData->strReActionName.empty())
				pReActionData->strReActionName = m_ReActionDefalutInfo.strReActionName;
		}
	}

	if(pReActionData->bDisable)
		return false;

	m_pCurrentReActionInfo = pReActionData;

	RPLAYINFO playInfo;
	playInfo.blendMode = RBLENDMODE_BLEND;
	playInfo.fPlaySpeed = pReActionData->fSpeed * fSpeedRate;
	playInfo.fWeight = pReActionData->fWeight * fWeightRate;
	if(playInfo.fWeight > 1.0f)
		playInfo.fWeight = 1.0f;
	playInfo.fBlendIn = 0.3f;
	playInfo.fBlendOut = 0.7f;

	tstring strAni;
	if (szAniPrefix && bUseDefalut)
	{
		strAni = szAniPrefix + pReActionData->strReActionName;
	}
	else
	{
		strAni = pReActionData->strReActionName;
	}

	bool bUseUpperAnimation = m_pRAnimationController->SetAnimation( MLocale::ConvTCHARToAnsi(strAni.c_str()).c_str(), &playInfo );
	if(bUseUpperAnimation == false)
	{
		// ���ξ� ���� �ѹ���...
		bUseUpperAnimation = m_pRAnimationController->SetAnimation( MLocale::ConvTCHARToAnsi(pReActionData->strReActionName.c_str()).c_str(), &playInfo );
		if(bUseUpperAnimation == false)
		{
			dlog("���׼� �ִϸ��̼� ���� %s or %s\n", MLocale::ConvTCHARToAnsi(strAni.c_str()).c_str(), MLocale::ConvTCHARToAnsi(pReActionData->strReActionName.c_str()).c_str());
			return false;
		}
	}


	m_pRAnimationController->Play();
	m_pRAnimationController->SetSpeed(playInfo.fPlaySpeed);

	return true;
}

void XFakeBeatenAnimationTC::Init( tstring& strAniName, float fWeight, float fSpeed )
{
	if (fWeight <= 0.0f) return;

	m_pRAnimationController->SetEventActive(false);

	m_ReActionDefalutInfo.strReActionName	= strAniName;
	m_ReActionDefalutInfo.fWeight			= fWeight;
	m_ReActionDefalutInfo.fSpeed			= fSpeed;

	SetEnable(true);
}

void XFakeBeatenAnimationTC::PreUpdate( float fDelta )
{
	if(m_pRAnimationController->IsPlayDone() == false)
		m_pRAnimationController->OnPreUpdate();
}

void XFakeBeatenAnimationTC::PostUpdate( float fDelta )
{
	if(m_pRAnimationController->IsPlayDone() == false)
	{
		// ���� ���� ������ �Ǿ� �ִٸ�...
		if(m_pCurrentReActionInfo && m_pCurrentReActionInfo->strWeaponBoneName.empty() == false)
		{
			RActorNode* pActorNode = m_pActor->GetActorNode(MLocale::ConvTCHARToAnsi(m_pCurrentReActionInfo->strWeaponBoneName.c_str()).c_str());
			if(pActorNode == NULL)
				return;

			RAnimationControllerNode* pControllerNode = m_pRAnimationController->GetControllerNode(m_pCurrentReActionInfo->strWeaponBoneName);
			if(pControllerNode)
			{
				pActorNode->SetVisibility(m_fWeaponVisibility);
			}
		}
	}
}

void XFakeBeatenAnimationTC::CheckWeaponVisibility()
{
	// ���� �� visibility �� ��������
	if(m_pCurrentReActionInfo && m_pCurrentReActionInfo->strWeaponBoneName.empty() == false)
	{
		RActorNode* pActorNode = m_pActor->GetActorNode(MLocale::ConvTCHARToAnsi(m_pCurrentReActionInfo->strWeaponBoneName.c_str()).c_str());

		if(pActorNode)
		{
			m_fWeaponVisibility = pActorNode->GetVisibility();
		}
	}
}
