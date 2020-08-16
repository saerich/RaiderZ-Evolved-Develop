#include "StdAfx.h"
#include "XExtraActiveTalent.h"
#include "XTalentEvent.h"
#include "XModuleEntity.h"
#include "XModuleActorControl.h"
#include "XGameTransformControllerManager.h"

#ifdef _DEBUG
//#define GRAPLLED_DEBUG_TALENT
//#include "RDebugPassRenderer.h"
#endif

void XExtraActiveTalent::OnEnterPhaseAct()
{
	// �ִϸ��̼� ����
	ChangeTalentMotion(TALENT_PHASE_ACT, m_pTalentInfo->m_szUseAnimation.c_str(), L"", m_pTalentInfo->m_nMovableType);
	// ����Ʈ ����
	ChangeTalentEffect(TALENT_PHASE_ACT);
}

/////////////////////////////////////////////////////////////////////////
void XExtraActiveTalent_Grapple::OnEnterPhasePrepare()
{
	// �ִϸ��̼� ����
	ChangeTalentMotion(TALENT_PHASE_PREPARE, m_pTalentInfo->m_szCastingAnimation.c_str(), m_pTalentInfo->m_szCastingLoopAnimation.c_str());
	// ����Ʈ ����
	ChangeTalentEffect(TALENT_PHASE_PREPARE);
}

void XExtraActiveTalent_Grapple::OnEnterPhaseAct()
{
	// �ִϸ��̼� ����
	ChangeTalentMotion(TALENT_PHASE_ACT, m_pTalentInfo->m_szUseAnimation.c_str());
	// ����Ʈ ����
	ChangeTalentEffect(TALENT_PHASE_ACT);

	StartAttach();
}

void XExtraActiveTalent_Grapple::OnEnterPhaseExtraAct()
{
	// ���� ������... 
	GetGrappledHitInfo();

	// �ִϸ��̼� ����
	ChangeTalentMotion(TALENT_PHASE_EXTRA, m_pTalentInfo->m_szExtraActAnimation.c_str());

	m_pTalentEvent->SetActStart(true);
	StartAttach();
}

void XExtraActiveTalent_Grapple::OnEnterPhaseCanceled()
{
	EndAttach();
}

void XExtraActiveTalent_Grapple::OnEnterPhaseFinish()
{
	EndAttach();
}

void XExtraActiveTalent_Grapple::EndAttach()
{
	XModuleEntity * pModuleEntity = m_pOwner->GetModuleEntity();
	if(pModuleEntity == NULL)
		return;

	//XEntityTransformControlAttacher* pAttacher = pModuleEntity->GetTransformControl<XEntityTransformControlAttacher>();
	//if(pAttacher)
	//{
	//	// ������ ������ Ǯ�� ���ߴٸ�... ���⼭
	//	// ���濡�� Ǯ�ȴٰ� �˸���.
	//	XObject * pTarget = gg.omgr->Find(pAttacher->GetTarget());
	//	if(pTarget)
	//	{
	//		XModuleActorControl * pModuleActorControl = pTarget->GetModuleActorControl();
	//		if(pModuleActorControl)
	//			pModuleActorControl->Stop();
	//	}
	//}
}

void XExtraActiveTalent_Grapple::StartAttach()
{
	// ����� ���� ��� �߰�(�ʿ� �Ҷ����� ����� �մϴ�.)
	// �ִϸ��̼��� ������ �Ǹ� �ʱ�ȭ�� �ǹǷ� �ִϸ��̼��� ������ �ɶ����� ������ ���ش�.
	XModuleEntity* pEntity = m_pOwner->GetModuleEntity();
	if(pEntity)
		pEntity->GetAnimationController()->UseDummyGrip();
}

void XExtraActiveTalent_Grapple::GetGrappledHitInfo()
{
	XTalentInfo* pTalent = GetInfo();
	if(pTalent == NULL ||
		pTalent->m_HitInfo.m_vSegments.size() == 0)
		return;

	int nCurFrame = m_pOwner->GetModuleEntity()->GetAnimationController()->GetAniFrameWithUpdateTime();

	// ���� Throw ����� �ƴ��ε�... ��Ʈ �ð��� �����ٸ�... �پ��...
	m_nGrappledHitFrame = m_pOwner->GetActor()->GetSecToFrame(pTalent->m_HitInfo.m_vSegments[0].m_fCheckTime);
	if(nCurFrame >= m_nGrappledHitFrame)
	{
		return;
	}

	// ��¿�� ���� ���°Ŵ�. 
	// ���� �θ��°��� �ƴϹǷ� ������.
	m_pOwner->GetModuleEntity()->GetAnimationController()->GetActorNodePosByFrame(wstring(GRAPPLED_NPC_GRIP_BONE_NAME), m_nGrappledHitFrame, m_vGrappledPos);

	int nRemainFrame = m_nGrappledHitFrame - nCurFrame;
	m_fGrappledRemainTime = m_pOwner->GetActor()->GetFrameToSec(nRemainFrame);

#ifdef GRAPLLED_DEBUG_TALENT
	vec3 vTest;
	m_pOwner->GetActor()->GetActorNodePos(GRAPPLED_NPC_GRIP_BONE_NAME, vTest, E_TS_WORLD, true);
	RMatrix mat;
	mat.MakeIdentity();
	mat.SetTranslation( vTest);
	REngine::GetSceneManager().GetPassRenderer<RDebugPassRenderer>()->AddDebugBox( mat, 5, 20, D3DCOLOR_XRGB(255, 255, 0));
#endif
}

//////////////////////////////////////////////////////////////////////////
void XExtraActiveTalent_Eat::OnEnterPhaseExtraAct2()
{
	// �ִϸ��̼� ����
	ChangeTalentMotion(TALENT_PHASE_EXTRA2, m_pTalentInfo->m_szExtraActAnimation2.c_str());
	StartAttach();

}

void XExtraActiveTalent_Eat::OnEnterPhaseExtraAct3()
{
	ChangeTalentMotion(TALENT_PHASE_EXTRA3, m_pTalentInfo->m_szExtraActAnimation3.c_str());
	StartAttach();
}