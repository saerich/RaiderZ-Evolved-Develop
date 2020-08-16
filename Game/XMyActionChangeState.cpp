#include "stdafx.h"
#include "XMyActionChangeState.h"
#include "XModuleMotion.h"
#include "XModuleMyControl.h"
#include "XModuleBuff.h"
#include "XMyPlayer.h"
#include "XEventID.h"
#include "XPost_Item.h"
#include "XPost_Action.h"
#include "XMyActionIdle.h"
#include "XCharacter.h"

// Change State //////////////////////////////////////////////////////////
bool XMyActionChangeState::CheckEnterable(void* pParam)
{
	// ������ ���� ���� Ȯ��
	XBuffAttribute attrBuff = m_pOwner->GetMyPlayerOwner()->GetModuleBuff()->GetBuffAttribute();
	if ( attrBuff.IsMesmerizable())
		return false;


	// ���� ���´� �ȵ�
	if (m_pOwner->GetMyPlayerOwner()->IsDead())
		return false;


	// ���Ľ� ���� ���� üũ
	int nStanceTo = ( m_pOwner->GetMyPlayerOwner()->IsCurrStanceBattle())  ?  CS_NORMAL : CS_BATTLE;
	if ( m_pOwner->GetMyPlayerOwner()->ValidateChangeStance( (CHAR_STANCE)(nStanceTo)) == false)
		return false;

	// �극��Ŀ������ ���� ������̾ ��� ������ �׼��ΰ�?
	if(m_pOwner->UsableActionState_OverlappedEquipmentItem(GetID()) == false)
		return false;

	return true;
}

void XMyActionChangeState::Enter(void* pParam)
{
	XMyActionState::Enter(pParam);


	// ���Ⱑ ������ idle�� ����
	if ( CheckExistWeapon() == false)
	{
		m_pOwner->DoAction( ACTION_STATE_IDLE);
		return;
	}

	// ���� ����
	m_bStartIdle = false;
	m_bStartRun = false;

	XModuleMotion* pModuleMotion = m_pOwner->GetMyPlayerOwner()->GetModuleMotion();
	if(pModuleMotion)
	{
		if(pModuleMotion->GetCurrMotion() == MOTION_NAME_IDLE)
			m_bStartIdle = true;
		else if(pModuleMotion->GetCurrMotion() == MOTION_NAME_RUN)
			m_bStartRun = true;
	}

	// ���Ľ� ����
	m_nStanceTo = ( m_pOwner->GetMyPlayerOwner()->IsCurrStanceBattle())  ?  CS_NORMAL : CS_BATTLE;
	m_bQuick = ( gvar.MyInfo.Talent.HasExtraPassiveTalent( TEPT_QUICK_DRAW))  ?  true : false;
	m_pOwner->GetMyPlayerOwner()->ChangeStance( m_nStanceTo, m_bQuick);


	// ����
	//XModuleAction* pModuleAction = m_pOwner->GetOwner()->GetModuleAction();
	//if ( pModuleAction)
	//	pModuleAction->StopMove();

	gvar.Game.MyCombatStanceAutoReleaserData.bReserved = false;

	// ������ ���� ���Ľ� ����
	int8 nValue = m_nStanceTo;
	if ( m_bQuick)
		nValue += 2;

	XPostChangeStance(nValue);
}

void XMyActionChangeState::Exit()
{
	XEvent evt( XEVTL_CHANGED_STANCE);
	m_pOwner->Event( evt);


	XMyActionState::Exit();
}

XEventResult XMyActionChangeState::Event(XEvent& msg)
{
	switch (msg.m_nID)
	{
	case XEVTL_UPPER_MOTION_PLAYONCE:
		{
			m_bStartIdle = false;
			m_bStartRun = false;

			XMyActionIdleParam idleParam;
			idleParam.bSetIdleAnimation = false;
			m_pOwner->DoAction(ACTION_STATE_IDLE, &idleParam);
		}
		return MR_TRUE;
	case XEVTL_ON_CHANGE_MOTION:
		{
			// ü���� ����� �Դٸ�... 
			// ��ü �ִϸ��̼��� �Ѱ��� �ƴϴ�.
			// ����������.

			XModuleMotion* pModuleMotion = m_pOwner->GetMyPlayerOwner()->GetModuleMotion();
			if(pModuleMotion)
			{
				wstring strCurrMotion = pModuleMotion->GetCurrMotion();
				if((m_bStartIdle &&
					strCurrMotion == MOTION_NAME_RUN) ||
					(m_bStartRun &&
					strCurrMotion == MOTION_NAME_IDLE) ||
					(m_bStartRun &&
					strCurrMotion == MOTION_NAME_RUN))
				{
					return MR_TRUE;
				}


				// ���ٽ� ���� ����̴�.
				if(CheckChangeStateAnimation(strCurrMotion))
					return MR_FALSE;
			}

			XMyActionIdleParam idleParam;
			idleParam.bSetIdleAnimation = false;
			m_pOwner->DoAction(ACTION_STATE_IDLE, &idleParam);
		}
		return MR_TRUE;
	}

	return XMyActionState::Event(msg);

	
}

bool XMyActionChangeState::CheckExistWeapon()
{
	int nWeaponSet = gvar.MyInfo.EquipmentSlot.GetWeaponSet();


	// ������ �ֹ��Ⱑ �ִ��� Ȯ��
	if ( nWeaponSet == 0)
	{
		XItem* pItemR = gvar.MyInfo.EquipmentSlot.GetItem( ITEMSLOT_RWEAPON);
		if ( pItemR)
			return true;
	}

	// �޼� �ֹ��Ⱑ �ִ��� Ȯ��
	else if ( nWeaponSet == 1)
	{
		XItem* pItemR = gvar.MyInfo.EquipmentSlot.GetItem( ITEMSLOT_RWEAPON2);

		if ( pItemR)
			return true;
	}

	return false;
}

bool XMyActionChangeState::CheckChangeStateAnimation( wstring strMotionName )
{
	if(strMotionName == MOTION_NAME_CHANGE_STANCE_TO_BATTLE2 ||
		strMotionName == MOTION_NAME_CHANGE_STANCE_TO_NORMAL2 ||
		strMotionName == MOTION_NAME_SHIELD_CHANGE_STANCE_TO_BATTLE2 ||
		strMotionName == MOTION_NAME_SHIELD_CHANGE_STANCE_TO_NORMAL2 ||
		strMotionName == MOTION_NAME_CHANGE_STANCE_TO_BATTLE ||
		strMotionName == MOTION_NAME_CHANGE_STANCE_TO_NORMAL ||
		strMotionName == MOTION_NAME_SHIELD_CHANGE_STANCE_TO_BATTLE ||
		strMotionName == MOTION_NAME_SHIELD_CHANGE_STANCE_TO_NORMAL)
		return true;

	return false;
}
