#include "StdAfx.h"
#include "XMyActionBow.h"
#include "XModuleMyControl.h"
#include "XMyPlayer.h"
#include "XEventID.h"
#include "XModuleTalent.h"
#include "XTargetSticky.h"
#include "XModuleEntity.h"
#include "XModuleCollision.h"
#include "XModuleAction.h"
#include "XModuleBuff.h"
#include "XCameraManager.h"
#include "XGameTransformControllerManager.h"
#include "XModulePost.h"
#include "XTalentInfoMgr.h"
#include "XTalentHelper.h"
#include "XPost_Action.h"

XMyActionBow::XMyActionBow(XModuleMyControl* pOwner)
: XMyActionState(pOwner)
{
	m_bShooted			= false;
	m_nAimingTime		= 0;
	m_nPreparingTime	= 0;
	m_pTarget			= new XTargetSticky(pOwner->GetOwner());
}

XMyActionBow::~XMyActionBow(void)
{
	SAFE_DELETE(m_pTarget);
}

bool XMyActionBow::CheckEnterable(void* pParam)
{
	// ������ ���� ���� Ȯ��
	XBuffAttribute attrBuff = m_pOwner->GetMyPlayerOwner()->GetModuleBuff()->GetBuffAttribute();
	if ( attrBuff.IsMesmerizable())
		return false;


	if (m_pOwner->GetMyPlayerOwner()->IsDead()) return false;
	if (!m_pOwner->GetMyPlayerOwner()->IsCurrStanceBattle()) return false;

	if (AmIGod()) return true;

	XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo(PLAYER_NORMAL_ATTACK_ARROW_TALENT_ID);
	if (pTalentInfo == NULL || pTalentInfo->m_nSTACost > gvar.MyInfo.ChrStatus.nSTA) return false;

	return true;
}

void XMyActionBow::Exit()
{
	m_pOwner->RemoveSpineController();
	m_pTarget->ExitTarget();

	XMyActionState::Exit();
}

void XMyActionBow::Enter(void* pParam)
{
	XMyActionState::Enter(pParam);

	if( !m_pOwner->GetOwner()->IsModuleEntity()) return;

	XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo(PLAYER_NORMAL_ATTACK_ARROW_TALENT_ID);
	if (pTalentInfo == NULL) return;

	XModuleAction* pModuleAction = m_pOwner->GetOwner()->GetModuleAction();
	if (!pModuleAction) return;

	XModuleEntity* pModuleEntity = m_pOwner->GetOwner()->GetModuleEntity();

	pModuleAction->StopMove(false);

	XModulePost* pModulePost = m_pOwner->GetOwner()->GetModulePost();

	pModulePost->PostMoveStop(m_pOwner->GetOwner()->GetPosition());

	XPostAttackPrepare();
	// ���� �غ� �޼��� �۽�
	//vec3 vCameraDir = gg.controller->GetBackViewCameraDir();
	//XPostUseTalent(PLAYER_NORMAL_ATTACK_ARROW_TALENT_ID, vCameraDir);

	m_bShooted		= false;
	m_nAimingTime	= XGetNowTime();
	m_nPreparingTime= (int)(1000.0f * pModuleEntity->GetAnimationController()->GetAniMaxFrame() / float(ANIMATION_FRAME_PER_SEC));

	m_pTarget->InitTarget(true);

	m_pTarget->SetTargetInfo(pTalentInfo->m_fRange, pTalentInfo->m_nENCost, pTalentInfo->m_EffectInfo.m_nRelation);

	m_pOwner->UseTalent(PLAYER_NORMAL_ATTACK_ARROW_TALENT_ID);
}

XEventResult XMyActionBow::Event(XEvent &msg)
{
	if (msg.m_nID == XEVTL_VIRTUAL_KEY_RELEASED)
	{
		XVirtualKey nVirtualKey = *static_cast<XVirtualKey*>(msg.m_pData);
		if( nVirtualKey == VKEY_ACTION)
		{
			if(m_bShooted == false)
			{
				if (IsDonePreparing())
				{
					if(global.ui)
					{
						global.ui->TargetUIEnd();
					}

					doShoot();
				}
				else
				{
					doCancel();
				}

				return MR_TRUE;
			}
		}
	}

	return XMyActionState::Event(msg);
}

void XMyActionBow::Update(float fDelta)
{
	// �غ�
	if( !m_bShooted )
		doReady();


}

void XMyActionBow::doReady()
{
	//�̺κ��� ��....
	XBaseCameraSceneNode* pCameraNode = global.camera->GetCamera();
	if (pCameraNode && m_pOwner->GetOwner()->IsModuleEntity())
	{
		vec3 dir = pCameraNode->GetDirection();
		vec3 pos = pCameraNode->GetPosition();

		vec3 vdir = dir;
		dir.z = 0.0f;
		dir.Normalize();

		// TODO: ������� ������ ���ô�.
		//if (!m_bShooted)
		m_pOwner->ActorRotation(vdir);
	}

	// Ÿ�� üũ
	if (m_pOwner->GetOwner()->IsModuleEntity())
	{
		m_pTarget->UpdateTarget();
		//m_pOwner->GetTalentSpellTarget();
	}
}


void XMyActionBow::doShoot()
{
	XBaseCameraSceneNode* pCameraNode = global.camera->GetCamera();
	if (pCameraNode == NULL) return;

	XModuleTalent* pModuleTalent = m_pOwner->GetOwner()->GetModuleTalent();

	if(pModuleTalent == NULL)
		return;

	if (pModuleTalent->GetTalent() == NULL) return;
	MUID uidTarget	= 0;
	int nGroupID	= -1;
	int nDummyID	= -1;

	// Ÿ�� ��������
	stTargetID* pTargetInfo = m_pTarget->GetTargetInfo();
	if (pTargetInfo)
	{
		XObject* pTargetObject = gg.omgr->FindObject(pTargetInfo->uidTarget);
		if(pTargetObject && pTargetObject->IsModuleEntity())
		{
			// Ÿ�� ���� ����
			uidTarget	= pTargetInfo->uidTarget;
			nGroupID	= pTargetInfo->nTargetGroupID;
			nDummyID	= pTargetInfo->nTargetCapsuleID;

			if (pCameraNode && m_pOwner->GetOwner()->IsModuleEntity())
			{
				XModuleCollision* pTargetCollision = pTargetObject->GetModuleCollision();
				if(pTargetCollision == NULL)
					return;

				vec3 campos			= pCameraNode->GetPosition();
				vec3 tar_center		= pTargetCollision->GetColObjectCenter();

				// �׷�� ���� ID�� �ִٸ�... Ÿ�� ��ġ�� ������Ʈ�� �ƴ϶� ĸ���̴�.
				if(nGroupID >= 0 &&nDummyID >= 0)
				{
					vec3 vCollCpasuleCenter = pTargetCollision->GetHitEffectProjectileTargetPos(nGroupID, nDummyID);
					if(vCollCpasuleCenter != vec3::ZERO)
					{
						tar_center = vCollCpasuleCenter;
					}
				}
				vec3 dir			= tar_center - pCameraNode->GetPosition();
				dir.Normalize();

				float fAngleDiff = pCameraNode->GetDirection().AngleBetween(dir);
				if (fAngleDiff > 0.02f)
				{
					pCameraNode->SetDirection(dir);

					dir		= tar_center - m_pOwner->GetOwner()->GetPosition();
					dir.z	= 0;
					dir.Normalize();
					m_pOwner->GetOwner()->SetDirection(dir);
				}
			}
		}
	}
	else
	{
		return;
	}

	vec3 vArrowStartPos;
	vec3 vDir;

	vArrowStartPos	= m_pOwner->GetOwner()->GetPosition();
	vDir			= m_pOwner->GetOwner()->GetDirection();

	// Ÿ�� �Է�
	pModuleTalent->SetTarget(uidTarget, nGroupID, nDummyID);

	// �κ� ���� Ÿ���̸�...
	if(nGroupID >= 0 &&
		nDummyID >= 0)
	{
		XPostAttackArcheryDetail(vArrowStartPos, vDir, uidTarget, nGroupID, nDummyID);
	}
	else
	{
		XPostAttackArchery(vArrowStartPos, vDir, uidTarget);
	}

	m_bShooted = true;
}

void XMyActionBow::doCancel()
{
	if( !m_bShooted )
		XPostAttackCancel();

	m_pOwner->DoActionIdle();
}

bool XMyActionBow::IsDonePreparing()
{
	uint32 nNowTime = XGetNowTime();
	if (nNowTime - m_nAimingTime > m_nPreparingTime) return true;
	return false;
}
