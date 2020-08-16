#include "stdafx.h"
#include "XMyActionUseTalent.h"
#include "XModuleMyControl.h"
#include "XModuleAction.h"
#include "XModuleTalent.h"
#include "XModulePost.h"
#include "XEventID.h"
#include "XModuleMotion.h"
#include "XModuleEntity.h"
#include "XModuleBuff.h"
#include "XMyPlayer.h"
#include "XSystem.h"
#include "XCameraManager.h"
#include "XController.h"
#include "XTalentInfoMgr.h"
#include "XTalentHelper.h"
#include "XTargetSticky.h"
#include "XTargetGround.h"
#include "XMyActionIdle.h"
#include "XStrings.h"
#include "XPost_Action.h"
#include "XModuleEffect.h"
#include "XSoundResMgr.h"
#include "CSEffectInfo.h"
#include "XSpecialActionTransition.h"

// Use Talent //////////////////////////////////////////////////////////
XMyActionUseTalent::XMyActionUseTalent(XModuleMyControl* pOwner) : XMyActionState(pOwner), m_bIsImmediateTalent(false)
{
	m_pTarget			= NULL;
	m_pUseTarget		= NULL;

	m_nTalentStep		= 0;
	m_nCurrentTalentID	= 0;
	m_nUseTalentState	= UTS_NONE;

	m_fCastingTime		= 0.0f;

	m_bTargetingTalent	= false;
	m_bCastingEndEffectDone	 = false;
}

XMyActionUseTalent::~XMyActionUseTalent()
{
	SAFE_DELETE(m_pTarget);
	m_pUseTarget		= NULL;
}

bool XMyActionUseTalent::CheckEnterable(void* pParam)
{
	int nTalentID = (pParam) ? *(int*)pParam : 0;
	if (nTalentID == 0)
	{
		_ASSERT(0);
		return false;
	}

	XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo(nTalentID, m_pOwner->GetOwner());
	if (pTalentInfo == NULL) return false;

	// ������ ���� ���� Ȯ��
	if (CheckBuff(nTalentID) == false)
	{
		return false;
	}

	if (m_pOwner->GetMyPlayerOwner()->IsDead() && pTalentInfo->m_bUsableAtDead == false) return false;

	if ( ValidateUse(pTalentInfo) == false ) return false;

	if (AmIGod())
	{
		return true;
	}

	if (pTalentInfo->IsPassiveSkill())
	{
		_ASSERT(0);
		return false;
	}

	// ��Ÿ�� üũ
	if ( gvar.MyInfo.Talent.CoolTime.IsCoolDown( nTalentID))
	{
		wstring strMsg = CSFormatString::Format( XGetStr( L"TALENT_NOTUSEYET"), FSParam(pTalentInfo->GetName()));

		global.ui->OnPresentationLower(strMsg, PRESENTATION_ICON_EXCLAMATION, true);

		return false;
	}

	// ���¹̳� üũ
	if(CheckTalentCost(pTalentInfo) == false)
	{
		wstring strMsg = XGetStr(L"SMSG_HAVE_NO_STA");
		global.ui->OnPresentationLower(strMsg, PRESENTATION_ICON_EXCLAMATION, true);

		return false;
	}

	m_nUseTalentState = UTS_CHECK;

	m_bTargetingTalent = false;

	return true;
}

void XMyActionUseTalent::Update(float fDelta)
{
	if(m_pUseTarget)
	{
		m_pUseTarget->UpdateTarget();
	}

	// ĳ���� Ÿ��
	CheckCastingTime(fDelta);
}

//Ż��Ʈ ���.
void XMyActionUseTalent::Enter(void* pParam)
{
	XMyActionState::Enter(pParam);

	m_nTalentStep			= 0;
	m_nTalentID				= (pParam) ? *(int*)pParam : 0;
	m_nCurrentTalentID		= m_nTalentID;
	m_pUseTarget			= NULL;

	XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo(m_nTalentID, m_pOwner->GetOwner());
	if (pTalentInfo == NULL) return;
	
	m_bIsImmediateTalent = CSTalentInfoHelper::IsImmediateTalent(pTalentInfo);

	// �̵��ϸ鼭 ����� �����Ѱ�?
	if(m_pOwner->IsMouseMove() == false || 
		CheckEnterTalentMovableType(pTalentInfo) == false)
	{
		m_pOwner->SyncPlayerDirToCameraDir();

		if( m_pOwner->GetOwner()->IsModuleEntity())
		{
			XModuleAction* pModuleAction = m_pOwner->GetOwner()->GetModuleAction();
			if (!pModuleAction) return;

			pModuleAction->StopMove(false);

			XModulePost* pModulePost = m_pOwner->GetOwner()->GetModulePost();
			pModulePost->PostMoveStop(m_pOwner->GetOwner()->GetPosition());
		}
	}
	
	m_nUseTalentState		= UTS_ENTER;

	PostUseTalent();


	// ĳ������ ���� �ŷ�Ʈ�� ��� ���
	if(HasCastingTime(pTalentInfo) == false)
	{
		m_pOwner->GetOwner()->GetModuleTalent()->Start(m_nTalentID);

		// �Ҽ��� ä��â���� �Ѹ���.
		if(pTalentInfo->m_nSkillType == ST_SOCIAL)
		{
			global.ui->OnChatMsg(MT_SOCIAL, pTalentInfo->GetDesc());
		}
	}

	// ��Ÿ�� ����
//	if ( pTalentInfo->m_nSkillType != ST_MAGIC  ||  pTalentInfo->m_fCastingTime < 0.001f)
//	{
//		gvar.MyInfo.Talent.Cooldown.OnUseTalent( pTalentInfo);
//	}
}

void XMyActionUseTalent::Exit()
{
	// ���� ������ε� ������ �Ǵ��� üũ
	if(m_nUseTalentState == UTS_ENTER ||
		m_nUseTalentState == UTS_START)
	{
		// �׷��ٸ� ��Ҹ� �������� �˸���.
		CancleTalent();
	}

	m_nUseTalentState	= UTS_END;
	m_nTalentStep		= 0;
	m_fCastingTime		= 0.0f;

 	if (global.script)
 	{
 		global.script->GetGlueGameEvent().OnActTalentEnd();
 	}

	if(global.script)
	{
		global.script->GetGlueGameEvent().OnGameEvent( "ACTIONPROGRESSBAR", "END");
	}

	XModuleTalent* pModuleTalent = m_pOwner->GetOwner()->GetModuleTalent();
	if( pModuleTalent)
	{
		pModuleTalent->ForceTerminate();
	}

	if(m_pUseTarget)
	{
		m_pUseTarget->ExitTarget();
	}

	//XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo(m_nTalentID, m_pOwner->GetOwner());
	//if (pTalentInfo && pTalentInfo->m_nSkillType != ST_MAGIC)
	//{
	//	gg.controller->ReleaseLimitDegreeOnFreeLook();
	//}

	SAFE_DELETE(m_pTarget);
	m_pUseTarget = NULL;

	XMyActionState::Exit();

	m_nTalentID			= -1;
	m_nCurrentTalentID	= 0;
}

XEventResult XMyActionUseTalent::Event(XEvent& msg)
{
	if (msg.m_nID == XEVTL_ON_RELEASE_TALENT_KEY)
	{
	}
	else if (msg.m_nID == XEVTL_VIRTUAL_KEY_PRESSED)
	{
		XVirtualKey nVirtualKey = *static_cast<XVirtualKey*>(msg.m_pData);
		switch (nVirtualKey)
		{
		case VKEY_JUMP:
		case VKEY_FORWARD:
		case VKEY_BACKWARD:
		case VKEY_LEFT:
		case VKEY_RIGHT:
			{
				CancleTalent();
				CancleSocialTalent();
			}
			break;
		case VKEY_ACTION:
			{
				if(m_bTargetingTalent && CheckActSpell())
					return MR_TRUE;
			}
			break;
		} // switch
	}
	else if(msg.m_nID == XEVTL_TALENT_START)
	{
		// �ŷ�Ʈ ������ �ߴ�.
		// ���⼭ Ÿ�ټ��� ��Ÿ���� ��°�... �ܰ躰�� Ÿ�� ����� �ٸ� �� �ֱ� �����̴�.
		XTalentInfo* pTalentInfo	= static_cast<XTalentInfo*>(msg.m_pData);
		if(pTalentInfo == NULL)
			return MR_FALSE;

		TalentStart(pTalentInfo);

		m_nUseTalentState = UTS_START;
	}
	else if(msg.m_nID == XEVTL_TALENT_CANCELED)
	{
		// ������ ���� ĵ���� �޾Ҵ�.
		// XModuleMyControl �̺�Ʈ���� idle ó���Ѵ�.
		m_nUseTalentState	= UTS_CANCEL;
	}
	else if(msg.m_nID == XEVTL_TALENT_ACT_SPELL)
	{
 		if (global.script)
 		{
 			global.script->GetGlueGameEvent().OnActTalentEnd();
 		}

		m_nUseTalentState = UTS_END;
	}
	else if(msg.m_nID == XEVTL_TALENT_RECASTING)
	{
		XTalentInfo* pTalentInfo	= static_cast<XTalentInfo*>(msg.m_pData);

		m_nUseTalentState = UTS_START;
	}
	else if(msg.m_nID == XEVTL_UPPER_MOTION_PLAYONCE)
	{
		XMyActionIdleParam idleParam;
		idleParam.bSetIdleAnimation = false;
		m_pOwner->DoAction(ACTION_STATE_IDLE, &idleParam);
	}
	else if(msg.m_nID == XEVTL_ON_CHANGED_MOTION_TO_IDLE)
	{
		XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo(m_nTalentID, m_pOwner->GetOwner());

		if(pTalentInfo->m_nMovableType != MUT_NONE && m_pOwner->GetOwner()->GetModuleTalent()->IsActive())
			return MR_TRUE;
	}
	else if(msg.m_nID == XEVTL_MOUSE_MOVE_CLICK)
	{
		if(m_pUseTarget && (m_pUseTarget->GetTargetType() == XTT_GROUND))
		{
			if(m_bTargetingTalent && CheckActSpell())
				return MR_TRUE;
		}
	}

	return XMyActionState::Event(msg);
}

bool XMyActionUseTalent::ValidateUse( XTalentInfo* pTalentInfo )
{
	// �ŷ�Ʈ ��� ���̶��...
	if(m_nUseTalentState != UTS_END &&
		m_nUseTalentState != UTS_NONE)
	{
		return false;
	}

	XTalentAvailable talentAvailable;
	if (talentAvailable.IsAvailable(pTalentInfo) == false)
	{
		XSoundResMgr::GetInstance()->PlayTalentResultSound(talentAvailable.GetResult());		
		return false;
	}

	// �÷��̾��� ��� ���Ľ� üũ
	XPlayer* pMyPlayer = m_pOwner->GetMyPlayerOwner();
	if (CheckBitSet(pTalentInfo->m_nEnabledStanceFlag, pMyPlayer->GetStance()) == false)
	{
		return false;
	}

	// ���¹̳� üũ�� XMyActionUseTalent�� �ִ�. �̰͵鵵 ���� üũ�Ҳ��� ������ ������. - birdkr(2007-06-19)

	// ��Ŀ�� �䱸 �ŷ�Ʈ�̸� �ش� ������ ���� �ִ��� Ȯ��
	if ( pTalentInfo->m_nRequireFocus == TFT_NONE)
		return true;

	int nFocusID = BUFF_FOCUS_ID[ pTalentInfo->m_nRequireFocus];
	if ( gvar.MyInfo.BuffList.IsValidTime( nFocusID))
		return true;

	// �극��Ŀ������ ���� ������̾ ��� ������ �ŷ�Ʈ�ΰ�?
	if(m_pOwner->IsOverlappedEquipmentControllerWork() &&
		gvar.MyInfo.Talent.IsUsableBuffTalentID(pTalentInfo->m_nID))
	{
		return true;
	}

	return false;
}

void XMyActionUseTalent::ShowCastingTimeUI( XTalentInfo* pTalentInfo )
{
	if(pTalentInfo == NULL)
		return;

	if(pTalentInfo->m_fCastingTime)
	{
 		if(global.script &&	IsUseTalent(pTalentInfo))
 		{
 			global.script->GetGlueGameEvent().OnActTalentStart(pTalentInfo->m_fCastingTime*1000);
 		}

		m_fCastingTime = pTalentInfo->m_fCastingTime;
		m_bCastingEndEffectDone	 = false;
	}
}

bool XMyActionUseTalent::IsUseTalent( XTalentInfo* pTalentInfo )
{
	// �ŷ�Ʈ�� ���� EN�� �ִ°�?
	if(pTalentInfo->m_nENCost > gvar.MyInfo.ChrStatus.nEN) 
	{		
		return false;
	}

	return true;
}

void XMyActionUseTalent::CancleTalent()
{
	XModuleTalent* pModuleTalent = m_pOwner->GetOwner()->GetModuleTalent();
	if (pModuleTalent)
	{
		if (pModuleTalent->GetPhase() == TALENT_PHASE_PREPARE && (m_nUseTalentState == UTS_ENTER || m_nUseTalentState == UTS_START))
		{
			XPostCancelTalent();			// �������� ��� �޽����� ���ƿ��� ������..
		}
	}
}

bool XMyActionUseTalent::ActSpell()
{
	XModuleTalent* pModuleTalent = m_pOwner->GetOwner()->GetModuleTalent();
	if (pModuleTalent)
	{
		pModuleTalent->RequestActSpell();

		// ������ ȭ���̳� ���� ���ư��� ������ ī�޶� �������� �Ͽ�����...
		// ������ ĳ���� �������� �ص� �ȴ�.
		// ������ Ȱ�� ��� �����Ŀ� ���� �ٲ�� ����.
		vec3 dir = m_pOwner->GetOwner()->GetDirection();

		TD_TALENT_TARGET_DETAIL TarInfo;
		stTargetID* pTargetInfo = NULL;
		if(m_pUseTarget)
		{
			pTargetInfo = m_pUseTarget->GetTargetInfo();
			if(pTargetInfo)
			{
				if(pTargetInfo->uidTarget != MUID::ZERO)
				{
					TarInfo.uidTarget			= pTargetInfo->uidTarget;
					TarInfo.nCapsuleGroupIndex	= pTargetInfo->nTargetGroupID;
					TarInfo.nCapsuleIndex		= pTargetInfo->nTargetCapsuleID;

					XPostActTalentWithHitCapsule(m_nTalentID, dir, TarInfo);
					return true;
				}
				else if(pTargetInfo->vTargetPos != vec3::ZERO)
				{
					XPostActTalentWithGround(m_nTalentID, dir, pTargetInfo->vTargetPos);
					return true;
				}
			}
		}
		else
		{
			// Ÿ���� ���ٸ� Ÿ���� �� �ڽ��̴�.
			XPostActTalent(m_nTalentID, dir, m_pOwner->GetOwner()->GetUID());
			return true;
		}

		// Ÿ���� ���ٸ�... �߻縦 ���� �ʴ´�.
		CancleTalent();
		// https://martini.neowiz.com/issues/2921
		MEvent::bLButton = false;
	}

	return false;
}

bool XMyActionUseTalent::CheckTalentCost( XTalentInfo* pTalentInfo )
{
	// ���¹̳� üũ
	// ���� ���õ� �ŷ�Ʈ üũ
	XTalentInfo* pSelectTalentInfo = NULL;

	pSelectTalentInfo = pTalentInfo;

	if(pSelectTalentInfo)
	{
		if (pSelectTalentInfo->m_nSTACost > gvar.MyInfo.ChrStatus.nSTA) return false;
		if (pSelectTalentInfo->m_nENCost > gvar.MyInfo.ChrStatus.nEN) return false;
	}

	return true;
}

void XMyActionUseTalent::TalentStart( XTalentInfo* pTalentInfo )
{
	// �׷� �ŷ�Ʈ UI �����ֱ�
	ShowCastingTimeUI(pTalentInfo);

	m_nCurrentTalentID = pTalentInfo->m_nID;


	// Ÿ���� �ʿ��� �ŷ�Ʈ �ΰ�?
	if(pTalentInfo->m_EffectInfo.m_nPoint != CSEffectInfo::POINT_TARGET &&
		(pTalentInfo->m_eTargetingType != TTT_GROUND || pTalentInfo->m_EffectInfo.m_nPoint != CSEffectInfo::POINT_AREAPOINT))
		return;

	// Ÿ�� ����
	CreateTarget(pTalentInfo);	
	
	m_bTargetingTalent = true;
}

void XMyActionUseTalent::CheckCastingTime(float fDelta)
{
	if(m_fCastingTime > 0.0f)
	{
		m_fCastingTime -= fDelta;		

		if(m_bTargetingTalent == false)
			CheckActSpell();

		if(m_bCastingEndEffectDone == false &&
			m_fCastingTime <= 0.1f)
			CastingEndEffect();
	}
}

bool XMyActionUseTalent::CheckActSpell()
{
	if(m_nUseTalentState == UTS_START && m_fCastingTime <= 0.1f) /* �����̸� �����Ͽ� ���� ���� ��� */
	{
		XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo(m_nTalentID, m_pOwner->GetOwner());

		// Act ������ �Ǵ°�?
		if ( pTalentInfo )
		{
			// ĳ������������Ʈ �ѷȳ�?
			if(m_bCastingEndEffectDone == false)
				CastingEndEffect();

			// �������� �߻� ��û
			if(ActSpell())
				m_nUseTalentState = UTS_ACT;

			m_bTargetingTalent = false;

			if(m_pUseTarget)
				m_pUseTarget->ExitTarget();

			m_pUseTarget = NULL;
			SAFE_DELETE(m_pTarget);

			return true;
		}
	}

	return false;
}

bool XMyActionUseTalent::HasCastingTime( XTalentInfo* pTalentInfo )
{
	return (pTalentInfo->m_fCastingTime > 0.0f);
}

void XMyActionUseTalent::PostUseTalent()
{
	XPostUseTalent(m_nTalentID, 
				m_pOwner->GetOwner()->GetDirection(), 
				m_pOwner->GetOwner()->GetPosition());
}

bool XMyActionUseTalent::CheckBuff( int nTalentID )
{
	XBuffAttribute attrBuff = m_pOwner->GetMyPlayerOwner()->GetModuleBuff()->GetBuffAttribute();
	if ( attrBuff.IsUsableTalent( nTalentID) == false)
		return false;

	return true;
}

bool XMyActionUseTalent::OnSpecialAction( SPECIAL_ACTION_COMMAND nSpecialActionCommand )
{
	XModuleTalent* pModuleTalent = m_pOwner->GetOwner()->GetModuleTalent();
	if (pModuleTalent && pModuleTalent->GetPhase() == TALENT_PHASE_PREPARE && m_nUseTalentState == UTS_START)
	{
		// ȸ�� �߳�?
		XSpecialActionTransChecker* pAttackChecker = m_pOwner->m_pAttackChecker;
		TALENT_NORMAL_ATTACK_TYPE nNextAttackType = pAttackChecker->Trans(nSpecialActionCommand);

		switch (nNextAttackType)
		{
		case NA_DODGE_FRONT:
		case NA_DODGE_BACK:
		case NA_DODGE_LEFT:
		case NA_DODGE_RIGHT:
			{
				CancleTalent();
				m_pOwner->DoAction(ACTION_STATE_NORMAL_ATTACK, nNextAttackType);
				return true;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// �̵� �ŷ�Ʈ?
	XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo(m_nTalentID, m_pOwner->GetOwner());
	
	if(pTalentInfo->m_nMovableType == MUT_NONE)
		return false;

	// �̵��ϴ� �ſ����� true ó��... ����� Ű�� ����� �Ѵ�.
	if(nSpecialActionCommand == SPECIAL_KEY_NONE)
		return true;

	return false;
}

void XMyActionUseTalent::CreateTarget( XTalentInfo* pTalentInfo )
{
	if(m_pTarget)
		SAFE_DELETE(m_pTarget);

	m_pUseTarget = NULL;

	// Ÿ�� ����
	float fRange = pTalentInfo->m_fRange;
	int	nEnCost = pTalentInfo->m_nENCost;
	CSEffectInfo::RELATION nRelation = pTalentInfo->m_EffectInfo.m_nRelation;

	// Ÿ�� ����
	if(pTalentInfo->m_eTargetingType == TTT_STICKY)
	{
		m_pTarget = new XTargetSticky(m_pOwner->GetOwner());

	}
	else if(pTalentInfo->m_eTargetingType == TTT_GROUND)
	{
		m_pTarget = new XTargetGround(m_pOwner->GetOwner());

		float fRaduis = 0.0f;
		XBuffInfo * pBuffInfo = info.buff->Get(pTalentInfo->m_nExtraActiveParam1);
		if(pBuffInfo && fRaduis < pBuffInfo->m_TriggeredBuff.infoEffect.m_fRadius)
			fRaduis = pBuffInfo->m_TriggeredBuff.infoEffect.m_fRadius;
		else
		{
			// ���� ������ ������... �� ������ ���ϴ�. 
			fRaduis = pTalentInfo->m_Buff1.nRange;

			nRelation = pTalentInfo->m_Buff1.EffectInfo.m_nRelation;
		}

		static_cast<XTargetGround*>(m_pTarget)->CreateGroundTarget(fRaduis);
	}

	//------------------------------------------------------------------------
	// Ÿ�� ���� ����
	m_pUseTarget		= m_pTarget;
	m_pUseTarget->SetTargetInfo(fRange, nEnCost, nRelation);

	if(pTalentInfo->m_eTargetingType != TTT_GROUND)
		m_pUseTarget->InitTarget(true);

}

void XMyActionUseTalent::CastingEndEffect()
{
	XModuleEffect * pModuleEffect = m_pOwner->GetOwner()->GetModuleEffect();
	XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo(m_nCurrentTalentID, m_pOwner->GetOwner());
	if (pTalentInfo == NULL) return;
	if(pModuleEffect)
	{
		XTalentEffectEventData * pCastEndEffectData = new XTalentEffectEventData;
		pCastEndEffectData->Set(pTalentInfo, CAST_END_EFFECT, m_pOwner->GetOwner(), vec3(0.0f, 0.0f, 0.0f), NULL, true);
		pModuleEffect->OnEffectEvent(XEFTEVT_EFFECT_TALENT, pCastEndEffectData);
	}

	m_bCastingEndEffectDone = true;
}

void XMyActionUseTalent::CancleSocialTalent()
{
	// �Ҽ� �ִϸ��̼��̶��...
	XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo(m_nTalentID, m_pOwner->GetOwner());
	if(pTalentInfo && pTalentInfo->m_nSkillType == ST_SOCIAL)
	{
		m_pOwner->DoActionIdle();
	}
}

bool XMyActionUseTalent::CheckEnterTalentMovableType( XTalentInfo* pTalentInfo )
{
	// �ŷ�Ʈ ���� �̵�Ÿ�� üũ
	// false�� ���� ó���ؾ� �Ѵ�.
	if(pTalentInfo == NULL)
		return false;

	if(pTalentInfo->m_nMovableType == MUT_NONE)
		return false;
	if(pTalentInfo->m_fCastingTime > 0.0f &&
		pTalentInfo->m_nMovableType == MUT_USE)
		return false;

	return true;
}

bool XMyActionUseTalent::CheckExitable( int nNextStateID )
{
	if(m_nUseTalentState == UTS_ENTER)
	{
		XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo(m_nTalentID, m_pOwner->GetOwner());
		if (pTalentInfo == NULL) return true;

		if(nNextStateID == ACTION_STATE_IDLE)
		{
			// ĳ������ ���� �ŷ�Ʈ�� ���� �������� ���� ���� ��Ȳ�̶��...
			if(pTalentInfo->m_fCastingTime > 0.0f)
			{
				// ������ ����.
				return false;
			}
		}
	}

	return true;
}
