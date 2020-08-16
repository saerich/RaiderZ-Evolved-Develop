#include "stdafx.h"
#include "XGameTransformControllerManager.h"
#include "XGlobal.h"
#include "XSystem.h"

#include "XFaceAnimationTC.h"
#include "XBaseAnimationTC.h"
#include "XUpperAnimationTC.h"
#include "XOverlayAnimationTC.h"
#include "XFakeBeatenAnimationTC.h"

#include "XCameraManager.h"

#include "RMesh.h"

XGameTransformControllerManager::XGameTransformControllerManager( XCharacter* pCharacter, MPxAniLookUpTC* pAniLookUpTransformController )
: XTransformControllerManager(pCharacter)
, m_pAniLookUpTransformController(pAniLookUpTransformController)
, m_fElapsedTime(0.0f)
, m_bFreezingFrame(false)
, m_fFreezingFrame(0.0f)
, m_fFreezingFrameDelta(0.0f)
{
	if(pCharacter == NULL)
		return;

	CreateAnimationTC(pCharacter);
}

XGameTransformControllerManager::~XGameTransformControllerManager()
{

}

bool XGameTransformControllerManager::SetAnimation( wstring& strAniName, bool bForce/*=false*/, bool bSameFrame /*= false*/)
{
	m_bFreezingFrame = false;
	m_fElapsedTime = 0.0f;

	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation == NULL)
		return false;

	// �����׽�Ʈ ��������..
	if (global.system->IsResourceLoading() == false) 
	{
		pBaseAnimation->SetAniName(strAniName);
		return true;
	}

	// Animation�� ���õǾ� ó��
	if (m_pAniLookUpTransformController)
	{
		string outAniName = MLocale::ConvUTF16ToAnsi(strAniName.c_str());
		bool bHooked = m_pAniLookUpTransformController->HookOnAnimationSetting(outAniName);
		strAniName = MLocale::ConvAnsiToUTF16(outAniName.c_str());

		if(bHooked)
			return true;
	}

	bool bResult = pBaseAnimation->SetAnimation(strAniName, bForce, bSameFrame);
	if(bResult)
	{
		// �������� �ִϸ��̼� ����
		XOverlayAnimationTC* pOverlayAnimation = GetTransformController<XOverlayAnimationTC>();
		if(pOverlayAnimation)
			pOverlayAnimation->SetAnimation(strAniName);

		// ���̽� �ִϸ��̼� ���� - �ִϸ��̼� ����� ���� �ִϸ��̼ǿ��� ����� ���̽� �ִϸ��̼� ����(������...)
		XFaceAnimationTC* pFaceAnimation = GetTransformController<XFaceAnimationTC>();
		if(pFaceAnimation)
			pFaceAnimation->EndAllFaceLoopAnimation();
	}

	return bResult;
}

void XGameTransformControllerManager::Update( float fDelta )
{
	m_fElapsedTime += fDelta;

	CheckFreezingFrame(fDelta);

	// PreUpdate�� ������... �ؾ��Ѵ�.
	XTransformControllerManager::PreUpdate(fDelta);

	// ������Ʈ ������ �þ߿� �ֳ�?
	if(CheckCameraViewFrustum() == false)
	{
		m_pActor->SetDoNotAnimationUpdated(true);
		return;
	}

	// �ִϸ��̼� ������Ʈ
	m_pActor->SetDoNotAnimationUpdated(false);
	XTransformControllerManager::Update(fDelta);

	XTransformControllerManager::PostUpdate(fDelta);
}

void XGameTransformControllerManager::CloseAnimationEvent()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		pBaseAnimation->CloseEvent();
}

void XGameTransformControllerManager::FreezeFrame( float fKeepTime )
{
	if (m_bFreezingFrame) return;

	m_bFreezingFrame = true;
	m_fFreezingFrameDelta = 0.0f;
	m_fFreezingFrame = fKeepTime;

	PauseAnimation();
}

void XGameTransformControllerManager::CheckFreezingFrame( float fDelta )
{
	if (m_bFreezingFrame)
	{
		m_fFreezingFrameDelta += fDelta;

		if (m_fFreezingFrameDelta >= m_fFreezingFrame)
		{
			m_bFreezingFrame = false;

			uint32 nFrame = m_pActor->GetSecToFrame(m_fElapsedTime);
//			SetAnimationFrame(nFrame);
			PlayAnimation();
		}
	}
}

bool XGameTransformControllerManager::SetUpperAnimation( wstring strUpperAniName )
{
	if( m_pActor->GetUsable() == false )
	{
		return false;
	}

	// �⺻ �ִϸ��̼� �߰�
	XUpperAnimationTC* pTc = Add<XUpperAnimationTC>();
	if(pTc == NULL)
	{
		return false;
	}

	pTc->SetEnable(true);

	return pTc->SetAnimation(strUpperAniName);
}

bool XGameTransformControllerManager::IsUpperAnimationOncePlayDone()
{
	XUpperAnimationTC* pUpperAnimation = GetTransformController<XUpperAnimationTC>();
	if(pUpperAnimation)
		return pUpperAnimation->IsOncePlayDone();

	return false;
}

bool XGameTransformControllerManager::IsUpperAnimationPlayDone()
{
	XUpperAnimationTC* pUpperAnimation = GetTransformController<XUpperAnimationTC>();
	if(pUpperAnimation)
		return pUpperAnimation->IsPlayDone();

	return false;
}

bool XGameTransformControllerManager::IsUpperAnimationLoop()
{
	XUpperAnimationTC* pUpperAnimation = GetTransformController<XUpperAnimationTC>();
	if(pUpperAnimation)
		return pUpperAnimation->IsLoop();

	return false;
}

void XGameTransformControllerManager::InitUpperAnimation()
{
	XUpperAnimationTC* pUpperAnimation = GetTransformController<XUpperAnimationTC>();
	if(pUpperAnimation)
		return pUpperAnimation->Init();
}

void XGameTransformControllerManager::SetFaceAnimationActive( bool bActive )
{
	XFaceAnimationTC* pFaceAnimation = GetTransformController<XFaceAnimationTC>();
	if(pFaceAnimation)
		pFaceAnimation->SetEnable(bActive);
}

void XGameTransformControllerManager::SetFaceAnimationNPC( bool bNPC )
{
	XFaceAnimationTC * pTC = Add<XFaceAnimationTC>();
	if(pTC == NULL)
	{
		return;
	}

	pTC->SetEnable(true);
	pTC->SetNPC(bNPC);
}

void XGameTransformControllerManager::OnLoadingComplete()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		pBaseAnimation->CheckReservedAnimation();

	XOverlayAnimationTC* pOverlayAnimation = GetTransformController<XOverlayAnimationTC>();
	if(pOverlayAnimation)
		pOverlayAnimation->CheckReservedAnimation();
}

float XGameTransformControllerManager::GetAnimationSpeed()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		return pBaseAnimation->GetAnimationSpeed();

	return 0.0f;
}

void XGameTransformControllerManager::SetAnimationSpeed( float fSpeed )
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		pBaseAnimation->SetAnimationSpeed(fSpeed);
}

void XGameTransformControllerManager::SetAnimationFrame( unsigned int nFrame )
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		pBaseAnimation->SetAnimationFrame(nFrame);
}

float XGameTransformControllerManager::GetMaxFrameToSecond()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		return pBaseAnimation->GetMaxFrameToSecond();

	return 0.0f;
}

float XGameTransformControllerManager::GetCurFrameToSecond()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		return pBaseAnimation->GetCurFrameToSecond();

	return 0.0f;
}

void XGameTransformControllerManager::PauseAnimation()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		pBaseAnimation->PauseAnimation();

	XOverlayAnimationTC* pOverlayAnimation = GetTransformController<XOverlayAnimationTC>();
	if(pOverlayAnimation)
		pOverlayAnimation->OverlayAnimationPause();
}

void XGameTransformControllerManager::StopAnimation()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		pBaseAnimation->StopAnimation();

	XOverlayAnimationTC* pOverlayAnimation = GetTransformController<XOverlayAnimationTC>();
	if(pOverlayAnimation)
		pOverlayAnimation->OverlayAnimationStop();
}

void XGameTransformControllerManager::PlayAnimation()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		pBaseAnimation->PlayAnimation();

	XOverlayAnimationTC* pOverlayAnimation = GetTransformController<XOverlayAnimationTC>();
	if(pOverlayAnimation)
		pOverlayAnimation->OverlayAnimationPlay();
}

wstring XGameTransformControllerManager::GetAnimationName()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		return pBaseAnimation->GetAnimationName();

	return L"";
}

int XGameTransformControllerManager::GetAniFrame()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		return pBaseAnimation->GetAniFrame();

	return 0;
}

int XGameTransformControllerManager::GetAniMaxFrame()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		return pBaseAnimation->GetAniMaxFrame();

	return 0;
}

bool XGameTransformControllerManager::IsAnimationLoop()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		return pBaseAnimation->IsAnimationLoop();

	return false;
}

bool XGameTransformControllerManager::IsPlayDone()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		return pBaseAnimation->IsPlayDone();

	return false;
}

bool XGameTransformControllerManager::IsOncePlayDone()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		return pBaseAnimation->IsOncePlayDone();

	return false;
}

void XGameTransformControllerManager::CreateAnimationTC( XCharacter* pCharacter )
{
	// �⺻ �ִϸ��̼� �߰�
	XBaseAnimationTC* pTc = Add<XBaseAnimationTC>();
	if(pTc == NULL)
	{
		return ;
	}

	pTc->SetEnable(true);
	pCharacter->SetAnimationConroller(pTc);

	// �������� �ִϸ��̼� �߰�
	XOverlayAnimationTC* pOverTc = Add<XOverlayAnimationTC>();
	if(pOverTc == NULL)
		return;

	pOverTc->SetEnable(true);
}

RAnimation* XGameTransformControllerManager::GetAnimation()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		return pBaseAnimation->GetAnimation();

	return NULL;
}

void XGameTransformControllerManager::UseDummyGrip()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation == NULL)
		return;

	pBaseAnimation->CheckActiveActorNode(wstring(GRAPPLED_NPC_GRIP_BONE_NAME));
	pBaseAnimation->CheckActiveActorNode(wstring(GRAPPLED_NPC_THROW_BONE_NAME));
}

void XGameTransformControllerManager::UseAttachActorNode( wstring strActorNodeName )
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation == NULL)
		return;

	pBaseAnimation->CheckActiveActorNode(strActorNodeName);
}

void XGameTransformControllerManager::SetUpperFrame( int nFrame )
{
	XUpperAnimationTC* pUpperAnimation = GetTransformController<XUpperAnimationTC>();
	if(pUpperAnimation)
		pUpperAnimation->SetAnimationFrame(nFrame);

}

int XGameTransformControllerManager::GetUpperFrame()
{
	XUpperAnimationTC* pUpperAnimation = GetTransformController<XUpperAnimationTC>();
	if(pUpperAnimation)
		return pUpperAnimation->GetAniFrame();

	return 0;
}

wstring XGameTransformControllerManager::GetPreAnimationName()
{
	// Ư���� ��쿡 ���̴� �̴ϴ�.
	// ���� ó���� ���� �ִϸ��̼ǿ� ���� ������ �˱� ���Ѱ̴ϴ�.
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		return pBaseAnimation->GetPreAnimaionName();

	return L"";
}

bool XGameTransformControllerManager::GetActorNodePosByFrame( wstring& strNodeName, unsigned int nFrame, vec3& vOut )
{
	// �Ժη� ���� ������.
	// ��¿�� ������...
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation == NULL)
		return false;

	return pBaseAnimation->GetActorNodePosByFrame(strNodeName, nFrame, vOut);
}

int XGameTransformControllerManager::GetAniFrameWithUpdateTime()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		return pBaseAnimation->GetAniFrameWithUpdateTime();

	return 0;
}

int XGameTransformControllerManager::GetPreAniFrame()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		return pBaseAnimation->GetPreAniFrame();

	return 0;
}

float XGameTransformControllerManager::GetPreAniFrameToSec(unsigned int nFrame)
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		return pBaseAnimation->GetPreAniFrameToSec(nFrame);
	
	return 0.0f;
}

void XGameTransformControllerManager::InitFakeBeatenAnimation(tstring& strAniName, float fWeight, float fSpeed)
{
	XFakeBeatenAnimationTC * pTC = Add<XFakeBeatenAnimationTC>();
	pTC->Init(strAniName, fWeight, fSpeed);
}

bool XGameTransformControllerManager::StartFakeBeaten( const TCHAR* szCurAni, const float fWeightRate, const float fSpeedRate, const TCHAR* szAniPrefix/*=NULL*/ )
{
	XFakeBeatenAnimationTC* pTC = GetTransformController<XFakeBeatenAnimationTC>();
	if (pTC)
	{
		return pTC->StartBeaten(szCurAni, fWeightRate, fSpeedRate, szAniPrefix);
	}

	return false;
}

bool XGameTransformControllerManager::CheckCameraViewFrustum()
{
	// ī�޶� ������ ������ ������Ʈ ����
	// ī�޶� ������ ������ �׳� ���
	if(global.camera == NULL ||
		global.camera->GetCurrentCameraController() == NULL ||
		global.camera->GetCurrentCameraController()->GetSceneNode() == NULL)
		return true;


	return global.camera->GetCurrentCameraController()->GetSceneNode()->GetViewFrustum().TestSphere(m_pActor->GetSphere());
}

void XGameTransformControllerManager::StopUpperAnimation()
{
	XUpperAnimationTC* pUpperAnimation = GetTransformController<XUpperAnimationTC>();
	if(pUpperAnimation)
		pUpperAnimation->Stop();
}

bool XGameTransformControllerManager::IsAnimationUsable()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		return pBaseAnimation->IsAnimationUsable();

	return false;
}

vector<ANIMATION_EVENT>* XGameTransformControllerManager::GetAnimationEvent()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		return pBaseAnimation->GetAnimationEvent();

	return NULL;
}

void XGameTransformControllerManager::StartFaceAnimation( wstring& strAniName, bool bEye )
{
	XFaceAnimationTC* pFaceAnimation = GetTransformController<XFaceAnimationTC>();
	if(pFaceAnimation)
		pFaceAnimation->StartFaceAnimation(strAniName,bEye);
}

void XGameTransformControllerManager::EndFaceAnimation( wstring& strAniName, float fEndBledTime )
{
	XFaceAnimationTC* pFaceAnimation = GetTransformController<XFaceAnimationTC>();
	if(pFaceAnimation)
		pFaceAnimation->EndFaceAnimation(strAniName, fEndBledTime);
}

void XGameTransformControllerManager::SetUpperAnimationSpeed( float fSpeed )
{
	XUpperAnimationTC* pUpperAnimation = GetTransformController<XUpperAnimationTC>();
	if(pUpperAnimation)
		pUpperAnimation->SetAnimationSpeed(fSpeed);
}

float XGameTransformControllerManager::GetUpperAnimationSpeed()
{
	XUpperAnimationTC* pUpperAnimation = GetTransformController<XUpperAnimationTC>();
	if(pUpperAnimation)
		return pUpperAnimation->GetAnimationSpeed();

	return 0.0f;
}

int XGameTransformControllerManager::GetUpperAniMaxFrame()
{
	XUpperAnimationTC* pUpperAnimation = GetTransformController<XUpperAnimationTC>();
	if(pUpperAnimation)
		return pUpperAnimation->GetAniMaxFrame();

	return 0;
}

float XGameTransformControllerManager::GetUpperMaxFrameToSecond()
{
	XUpperAnimationTC* pUpperAnimation = GetTransformController<XUpperAnimationTC>();
	if(pUpperAnimation)
		return pUpperAnimation->GetMaxFrameToSecond();

	return 0.0f;
}

void XGameTransformControllerManager::ResetAniamtionSpeed()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		pBaseAnimation->ResetAniamtionSpeed();
}

float XGameTransformControllerManager::GetOrgAniamtionSpeed()
{
	XBaseAnimationTC* pBaseAnimation = GetTransformController<XBaseAnimationTC>();
	if(pBaseAnimation)
		return pBaseAnimation->GetOrgAniamtionSpeed();

	return 1.0f;
}

void XGameTransformControllerManager::SetFaceBlinkAnimationActive( bool bActive )
{
	XFaceAnimationTC* pFaceAnimation = GetTransformController<XFaceAnimationTC>();
	if(pFaceAnimation)
		pFaceAnimation->SetEnableBlink(bActive);
}
