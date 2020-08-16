#include "stdafx.h"
#include "XActionDie.h"
#include "XModuleEntity.h"
#include "XModuleCollision.h"
#include "XModuleBuff.h"
#include "XModuleMotion.h"
#include "XModuleAction.h"
#include "XModuleActorControl.h"
#include "XModuleTalent.h"
#include "XGameTransformControllerManager.h"

XActionDie::XActionDie()
{
	m_bStartDie		= false;
	m_bMFDie		= false;
	m_bDieAniEnd	= false;

	m_vDieStartUp	= vec3(0, 0, 0);
	m_vDieStartDir	= vec3(0, 0, 0);
	m_vDieTargetUp	= vec3(0, 0, 0);
	m_vDieTargetDir	= vec3(0, 0, 0);
}

XActionDie::~XActionDie()
{

}

void XActionDie::Init( XObject* pObject )
{
	m_pOwner		= pObject;
}

void XActionDie::StartDie(bool bAni)
{
	// �ִϸ��̼��� ������ �ִϳ����� ���� ó���� �����Ѵ�.
	if(bAni == false)
	{
		DieAniEnd();
		return;
	}

	// �̹� �����ߴٸ�...
	if(m_bStartDie)
		return;

	StartTerrainUpDir();
}

void XActionDie::OnStartDie( ACTION_DIE_STATE eState )
{
	switch(eState)
	{
	case ADS_NORMAL:
	case ADS_MF_NO_ANIMATION:
	case ADS_KNOCKBACK:
		{
			// ���� �ִϸ��̼� ����
			const wchar_t* szDieMotionName = MOTION_NAME_DIE;
			if (m_pOwner->GetEntityType() == ETID_NPC) szDieMotionName = MOTION_NAME_NPC_DIE;

			// �ִϸ��̼� �� �ŷ�Ʈ... ��Ÿ��� ��� �����.
			XModuleAction* pModuleAction = m_pOwner->GetModuleAction();
			if(pModuleAction)
			{
				pModuleAction->StopUpperAni();		// ��ü �ִϸ��̼� ���
			}
			
			XModuleTalent* pModuleTalent = m_pOwner->GetModuleTalent();
			if(pModuleTalent)
			{
				pModuleTalent->Cancel();
			}

			// ���� �ִϸ��̼� ����
			bool bSetAni = true;
			XModuleMotion * pModuleMotion = m_pOwner->GetModuleMotion();
			if(pModuleMotion && pModuleMotion->GetCurrMotion() != szDieMotionName)
			{
				//mlog("���� �ִ� ����\n");
				bSetAni = pModuleMotion->ChangeMotion(szDieMotionName, MT_DEFAULT, true);
			}
			
			StartDie(bSetAni);
			//mlog("��� ���̾��\n");

			break;
		}
		
	case ADS_KNOCKDOWN:
	case ADS_THROWUP:
	case ADS_GRAPPLED:
	case ADS_UPPERED:
		{
			// ��Ǵ��̸�... ����� ���÷� üũ�ؾ���...
			m_bMFDie = true;

			//mlog("��Ǵ��̿���\n");
			StartTerrainUpDir();
			//mlog("��� ���̾��\n");
			break;
		}
	}
}

void XActionDie::OnStartDie( MF_STATE eMFState, bool bAniSet )
{
	// �ִϸ��̼� ������ �Ȱ͸� ������ �ش��Ѵ�.
	// �ִϸ��̼��� ������... �׳� ���� �ִϷ� �����Ѵ�.
	if(bAniSet)
	{
		switch(eMFState)
		{
		case MF_KNOCKBACK:
			{
				OnStartDie(ADS_KNOCKBACK);
				return;
			}

		case MF_THROWUP:
			{
				OnStartDie(ADS_THROWUP);
				return;
			}

		case MF_KNOCKDOWN:
			{
				OnStartDie(ADS_KNOCKDOWN);
				return;
			}

		case MF_UPPERED:
			{
				OnStartDie(ADS_UPPERED);
				return;
			}

		case MF_GRAPPLED:
		case MF_SWALLOWED:
			{
				OnStartDie(ADS_GRAPPLED);
				return;
			}
		}
	}

	OnStartDie(ADS_NORMAL);
}

void XActionDie::OnEndDie()
{
	m_bStartDie		= false;
	m_bMFDie		= false;
	m_bDieAniEnd	= false;

	XModuleEntity * pModuleEntity		= m_pOwner->GetModuleEntity();
	if(pModuleEntity)
	{
		pModuleEntity->_SetUpVector(UP_VECTOR);
		pModuleEntity->_SetRenderDirection( vec3::ZERO);
	}
}

void XActionDie::Update( float fDelta )
{
	if(m_bStartDie && m_bMFDie == false)
	{
		XModuleEntity * pModuleEntity		= m_pOwner->GetModuleEntity();
		if(pModuleEntity == NULL)
		{
			return;
		}

		//mlog("1 %s\n", MLocale::ConvTCHARToAnsi(m_pOwner->GetModuleMotion()->GetCurrMotion().c_str()).c_str());
		//XActor* pThisActor = static_cast<XActor*>(m_pOwner);

		//if(pThisActor == NULL)
		//	return;
		if(m_pOwner == NULL)
			return;

		// ������ �����ӿ� �����ϸ� ó���ϴ� �Լ�
		if(pModuleEntity->GetAnimationController()->IsOncePlayDone())
		{
			if (m_bMFDie == false)
			{
				//mlog("�׳��ε�... ������ ������?");
				DieAniEnd();
			}

			m_bStartDie = false;
			return;
		}

		//------------------------------------------------------------------------
		// �׾������� ���� ��翡 ���� ȸ��
		// �÷��̾ NPC���� �ش�
		//if(pThisActor->IsDead())								// NPC�� �÷��̾ �׾��°�?
		{
			UpdateTerrainUpDir(fDelta);
		}
	}
	else if(m_bMFDie)
	{
		XModuleMotion * pModuleMotion = m_pOwner->GetModuleMotion();
		XModuleEntity * pModuleEntity = m_pOwner->GetModuleEntity();

		if(pModuleMotion == NULL ||
			pModuleEntity == NULL)
			return;

		wstring strDieMotionName = MOTION_NAME_DIE;
		if (m_pOwner->GetEntityType() == ETID_NPC) strDieMotionName = MOTION_NAME_NPC_DIE;

		wstring strIdleMotionName = MOTION_NAME_IDLE;
		if (m_pOwner->GetEntityType() == ETID_NPC) strIdleMotionName = MOTION_NAME_NPC_IDLE;

		//mlog("2 %s\n", MLocale::ConvTCHARToAnsi(pModuleMotion->GetCurrMotion().c_str()).c_str());
		if(pModuleMotion->IsHoldingSequence())
		{
			DoneDie();
			m_bMFDie = false;
		}
		else if(pModuleMotion->GetCurrMotion() == strDieMotionName)
		{
			if(pModuleEntity->GetAnimationController()->IsOncePlayDone())
			{
				//mlog("���� ����̾�����?");
				DieAniEnd();
				m_bMFDie = false;
			}
			else
				UpdateTerrainUpDir(fDelta);
		}
		else if(pModuleMotion->GetCurrMotion() == strIdleMotionName ||
			pModuleMotion->IsPastDeadPostTrigger())
		{
			m_bMFDie = false;
			//mlog("��ַ� �ٽ� �׾��\n");
			OnStartDie(ADS_NORMAL);
		}

		UpdateTerrainUpDir(fDelta);
	}
}

bool XActionDie::CalTerrainUpDir( vec3& pos, vec3& dir, vec3& outUp, vec3& outDir )
{
	XModuleCollision * pModuleCollision = m_pOwner->GetModuleCollision();
	if(pModuleCollision)
	{
		vec3 vStart	= pos + RVector(0,0,100);
		vec3 vEnd	= pos + RVector(0,0,-1000);
		vec3 vOutPos;
		vec3 vOutNor;

		if(pModuleCollision->PickRayToTerrain(vStart, vEnd, vOutPos, vOutNor))
		{
			// ������ ����
			vOutNor.Normalize();

			// ���� ���� ���ϱ�
			vec3 vDir		= dir;
			vec3 vCross		= vDir.CrossProduct(vOutNor);
			vec3 vRealDir	= vOutNor.CrossProduct(vCross);

			outUp			= vOutNor;
			outDir			= vRealDir;
		}
		else
		{
			// ������ ���ٸ�...
			outUp.Set(0.0f, 0.0f, 1.0f);
			outDir.Set(dir);
		}

		return true;
	}

	return false;
}

void XActionDie::DoneDie()
{
	// �̹� �׾� �־���.
	XModuleEntity * pModuleEntity		= m_pOwner->GetModuleEntity();
	if(pModuleEntity)
	{
		vec3 vTerrainUp;
		vec3 vTerrainDir;

		if(CalTerrainUpDir(pModuleEntity->GetPosition(), pModuleEntity->GetDirection(), vTerrainUp, vTerrainDir))
		{
			pModuleEntity->_SetRenderUpDir(vTerrainUp, vTerrainDir);
		}
	}

	DieAniEnd();
}

void XActionDie::DieAniEnd()
{
	XModuleActorControl* pActorControl = m_pOwner->GetModuleActorControl();
	if(pActorControl)
	{
		pActorControl->OnDieAniEnd();
	}

	m_bDieAniEnd = true;
}

bool XActionDie::IsDoingActionDie()
{
	// ���۵� ���ߴ�. ���� ��������̴�.
	if(m_bStartDie == false &&
		m_bDieAniEnd == false)
		return false;

	// �۵����̴�.
	return true;
}

void XActionDie::StartTerrainUpDir()
{
	XModuleEntity * pModuleEntity		= m_pOwner->GetModuleEntity();
	if(pModuleEntity)
	{
		vec3 vTerrainUp;
		vec3 vTerrainDir;

		pModuleEntity->InitRenderPosDirUp();

		if(CalTerrainUpDir(pModuleEntity->GetPosition(), pModuleEntity->GetDirection(), vTerrainUp, vTerrainDir))
		{
			// ó�����ۿ� ���� ���� ����					
			m_vDieStartUp = vec3::AXISZ;
			if(pModuleEntity->GetActor())
				m_vDieStartUp = pModuleEntity->GetActor()->GetWorldUp();

			m_vDieStartDir = pModuleEntity->GetDirection();
			m_vDieTargetUp = vTerrainUp;
			m_vDieTargetDir = vTerrainDir;

			m_bStartDie		= true;
			m_bDieAniEnd	= false;
		}
	}
}

void XActionDie::UpdateTerrainUpDir( float fDelat )
{
	XModuleEntity * pModuleEntity		= m_pOwner->GetModuleEntity();
	if(pModuleEntity == NULL)
		return;

	if(m_pOwner->GetEntityType() == ETID_PLAYER ||
		m_pOwner->GetEntityType() == ETID_NPC)
	{
		// �������� �´ٸ� ������ ���δ�.
		// ������ �Ѵ�.
		// �ð� ������ ����Ѵ�.
		// �� �ִϸ��̼� �ð��� 2/3���� ȸ���� ������.
		if(pModuleEntity->GetAnimationController()->GetAnimation())
		{
			float fCurrentFrameRatio = 1.0f - (pModuleEntity->GetAnimationController()->GetCurFrameToSecond() / (pModuleEntity->GetAnimationController()->GetMaxFrameToSecond() * 2.0f / 3.0f));
			if(fCurrentFrameRatio < 0.0f)
				fCurrentFrameRatio = 0.0f;

			vec3 vCurrentUp = m_vDieStartUp.GetInterpolated(m_vDieTargetUp, fCurrentFrameRatio);
			vec3 vCurrentDir = m_vDieStartDir.GetInterpolated(m_vDieTargetDir, fCurrentFrameRatio);

			// ��, ���� ����
			pModuleEntity->SetRenderUpDir(vCurrentUp, vCurrentDir);
		}
	}
}
