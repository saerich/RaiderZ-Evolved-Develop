#include "stdafx.h"
#include "XMyActionGuard.h"
#include "XModuleMyControl.h"
#include "XModuleAction.h"
#include "XModuleBuff.h"
#include "XModuleMovable.h"
#include "XObject.h"
#include "XEventID.h"
#include "XMyPlayer.h"
#include "XController.h"
#include "XSpecialActionTransition.h"
#include "XTalentAvailable.h"
#include "XStrings.h"
#include "XPost_Action.h"


// Guard Action State //////////////////////////////////////////////////////////
bool XMyActionGuard::CheckEnterable(void* pParam)
{
	// ������ ���� ���� Ȯ��
	XBuffAttribute attrBuff = m_pOwner->GetMyPlayerOwner()->GetModuleBuff()->GetBuffAttribute();
	int nTalentID = CSTalentInfoHelper::NormalGuardTalentID( gvar.MyInfo.EquipmentSlot.GetCurrWeaponType(), 0, gvar.MyInfo.EquipmentSlot.IsEquipShield());
 	if ( attrBuff.IsUsableTalent( nTalentID) == false)
 		return false;


	// Disabled ������ �ɸ� ���¿����� ���� Ű �޺� �Է��� ���� ����
	m_bEnableAtkReserve = attrBuff.IsAttackableMelee() ? true : false;


	if (m_pOwner->GetMyPlayerOwner()->IsDead()) return false;
	if (!m_pOwner->GetMyPlayerOwner()->IsCurrStanceBattle()) return false;

	// ����� �˹�� ���带 ���Ѵ�.
	XModuleMovable* pModuleMovable = m_pOwner->GetOwner()->GetModuleMovable();
	if(pModuleMovable &&
		pModuleMovable->GetMovableFactor().KNOCK_BACK.m_bKnockback == true)
		return false;
	
	// ���� �˹�� ���带 ���� ���Ѵ�.
	if(m_pOwner->IsCurrentMotionGuardKnockback()) return false;

	// Ű ���� üũ
	if(pParam)
	{
		bool bKeyReleased = *(bool*)pParam;
		if(bKeyReleased)
			m_bShowErrorMsg = false;
	}

	// �ŷ�Ʈ ��� �����ϴ��� üũ
	XTalentAvailable talentAvailable;
	if(talentAvailable.IsAvailable(nTalentID) == false)
	{
		XTalentAvailable::Anailable_Result nResult = talentAvailable.GetResult();
		if (nResult == XTalentAvailable::RESULT_FALSE_STA && m_bShowErrorMsg == false)
		{
			wstring strMsg = XGetStr(L"SMSG_HAVE_NO_STA");
			global.ui->OnPresentationLower(strMsg, PRESENTATION_ICON_EXCLAMATION, true);
			m_bShowErrorMsg = true;
		}

		return false;
	}

	// �극��Ŀ������ ���� ������̾ ��� ������ �׼��ΰ�?
	if(m_pOwner->UsableActionState_OverlappedEquipmentItem(GetID()) == false)
		return false;

	return true;
}
void XMyActionGuard::Enter(void* pParam)
{
	XMyActionState::Enter(pParam);

	XModuleAction* pModuleAction = m_pOwner->GetOwner()->GetModuleAction();
	if (!pModuleAction) return;

	m_AttackReserver.Release();

	pModuleAction->StopUpperAni();
	pModuleAction->ActionGuard();

	m_bReleased = false;
	m_bGuardKnockback = false;
	m_bIdle = false;
	m_bShowErrorMsg = false;

	m_nStartTime = XGetNowTime();
	
	XPostGuard();
}

void XMyActionGuard::Exit()
{
	if(m_bReleased == false)
		doRelease();

	XMyActionState::Exit();
}

void XMyActionGuard::Update(float fDelta)
{
	if (m_bReleased) return;

	if (m_pOwner->IsCurrentGuardEnabled() == false &&
		m_bGuardKnockback == false)
	{
		doRelease();
	}
}

XEventResult XMyActionGuard::Event(XEvent& msg)
{
	switch (msg.m_nID)
	{
	case XEVTL_VIRTUAL_KEY_PRESSEDONCE:
		{
			XVirtualKey nVirtualKey = *static_cast<XVirtualKey*>(msg.m_pData);
			switch (nVirtualKey)
			{
			case VKEY_ACTION:
				{
					if(m_bGuardKnockback == false)
					{
						doRelease();

						if ( m_bEnableAtkReserve)
							m_AttackReserver.Reserve( NA_NORMAL1);
					}

					return MR_TRUE;
				}
				break;
			};
		}
		break;
	case XEVTL_GUARD_KNOCKBACK:
		{
			m_bGuardKnockback = true;
		}
		break;
	case XEVTL_GUARDED:
		{
			m_bGuardKnockback = false;
			guarded();
		}
		break;
	case XEVTL_ON_CHANGED_MOTION_TO_IDLE:
		{
			m_bIdle = true;
			m_bGuardKnockback = false;

			if ( m_bEnableAtkReserve  &&  m_AttackReserver.IsReserved())
				m_pOwner->DoAction(ACTION_STATE_NORMAL_ATTACK, m_AttackReserver.GetReservedAttackType());
			else
				m_pOwner->DoActionIdle();
			return MR_TRUE;
		}
		break;
	case XEVTL_GUARD_FAILED:
		{
			// ���� ����
			ActionRelease();
		}
		break;
	case XEVTL_MYACTION_USE_TALENT:
		{
			int nTalentID = *(int*)(msg.m_pData);

			if (CheckAvailableTalentOnGuard(nTalentID))
			{
				m_pOwner->DoAction(ACTION_STATE_USE_TALENT, nTalentID);

				return MR_TRUE;
			}
		}
		break;
	}
	return XMyActionState::Event(msg);
}

void XMyActionGuard::doRelease()
{
	XPostGuardReleased();

	m_bReleased = true;
	
	ActionRelease();
}

bool XMyActionGuard::OnSpecialAction( SPECIAL_ACTION_COMMAND nSpecialActionCommand )
{
	// �˹�ÿ��� �۵��ϸ� �ȵȴ�.
	if(m_bGuardKnockback)
		return false;

	XSpecialActionTransChecker* pAttackChecker = m_pOwner->m_pAttackChecker;
	TALENT_NORMAL_ATTACK_TYPE nNextAttackType = pAttackChecker->Trans(nSpecialActionCommand);

	if (pAttackChecker->CheckConst(NA_NA, nNextAttackType) == false) return false;

	// ����Ű ����
	switch (nNextAttackType)
	{
	case NA_ADVANCE:
	case NA_UPPER:
		{
			doRelease();

			if ( m_bEnableAtkReserve)
				m_AttackReserver.Reserve( nNextAttackType);
		}
		break;
	}

	if (m_pOwner->IsCanCancelAllMotion())
	{
		if (nNextAttackType == NA_COUNTER_ATTACK)
		{
			m_pOwner->DoAction(ACTION_STATE_NORMAL_ATTACK, nNextAttackType);
			return true;
		}
	}

	return false;
}

bool XMyActionGuard::CheckExitable( int nNextStateID )
{
	// ���⼭�� ������ �ִ��� üũ�� �ƴ϶�
	// ���� ������Ʈ�� ��������... �˰� �ű⿡ ���� ó���Ѵ�.
	if(nNextStateID == ACTION_STATE_HIT)
	{
		m_bReleased = true;

		XModuleAction* pModuleAction = m_pOwner->GetOwner()->GetModuleAction();
		if (pModuleAction)
		{
			pModuleAction->ActionGuardRelease();
		}
	}

	return true;
}

void XMyActionGuard::guarded()
{
	// ���� �˹����̶�� �ִϸ��̼��� �߰��� ���� �ʿ䰡 ����.
	if(m_bGuardKnockback)
		return;

	// ����
	XModuleAction* pModuleAction = m_pOwner->GetOwner()->GetModuleAction();
	if (pModuleAction)
	{
		pModuleAction->Guarded(!m_bReleased);
	}
}

void XMyActionGuard::ActionRelease()
{
	if(m_pOwner->GetMyPlayerOwner()->IsDead() ||
		m_bIdle)
		return;

	XModuleAction* pModuleAction = m_pOwner->GetOwner()->GetModuleAction();
	if (!pModuleAction) return;

	pModuleAction->ActionGuardRelease();
}

bool XMyActionGuard::CheckAvailableTalentOnGuard( int nTalentID )
{
	XTalentInfo* pTalentInfo = info.talent->Get(nTalentID);	
	if (!pTalentInfo) return false;

	return pTalentInfo->m_bAvailableOnGuard;
}
