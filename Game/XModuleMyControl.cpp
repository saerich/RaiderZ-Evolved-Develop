#include "stdafx.h"
#include "XModuleMyControl.h"
#include "XModuleMovable.h"
#include "XModuleEntity.h"
#include "XModuleAction.h"
#include "XModuleTalent.h"
#include "XModuleCollision.h"
#include "XModuleEffect.h"
#include "XModulePost.h"
#include "XModuleBuff.h"
#include "XModuleMessageQueue.h"
#include "XMyPlayer.h"
#include "XController.h"
#include "XEventID.h"
#include "XGame.h"
#include "XWorld.h"
#include "CSItemHelper.h"
#include "XActionKeyDef.h"
#include "XSpecialActionTransition.h"
#include "XMyActionNormalAttack.h"
#include "XMyActionBow.h"
#include "XMyActionGuard.h"
#include "XMyActionHit.h"
#include "XMyActionUseTalent.h"
#include "XMyActionIdle.h"
#include "XMyActionJump.h"
#include "XMyActionFalling.h"
#include "XMyActionChangeState.h"
#include "XMyActionSwitchingWeapon.h"
#include "XMyActionHit.h"
#include "XMyActionDie.h"
#include "XMyActionUseItem.h"
#include "XMyActionHold.h"
#include "XMyActionLootingItem.h"
#include "XMyActionRebirth.h"
#include "XMyActionGrappled.h"
#include "XMyActionMapObj.h"
#include "XMyActionChangeChannel.h"
#include "XMyActionQuestInteraction.h"
#include "XMyActionDeactivative.h"
#include "XMyActionSwallowed.h"
#include "XMyActionSit.h"
#include "XMyActionGesture.h"
#include "XMyActionNPCInteraction.h"
#include "XMyActionEnchant.h"
#include "XMyActionDye.h"
#include "XTalentInfoMgr.h"
#include "XTalentHelper.h"
#include "XEffectInvoker.h"
#include "XTalentHitParam.h"
#include "XMouseMove.h"
#include "XStrings.h"
#include "XPlayerPVP.h"
#include "XCameraManager.h"
#include "XSensorController.h"
#include "XSensorManager.h"
#include "XLightEffect.h"
#include "CSMotionFactorHelper.h"
#include "XInteractionInfo.h"
#include "XGameTransformControllerManager.h"
#include "XTargetIndicator.h"

#include "XPost_Trade.h"
#include "XPost_Guild.h"
#include "XPost_Action.h"
#include "XPost_Duel.h"
#include "XChallengerQuestExitChecker.h"

#include "XHelp.h"
#include "XNetDebuger.h"

bool XModuleMyControl::CheckSpecialActionKey( SPECIAL_ACTION_COMMAND& eCmd )
{
	eCmd		 = (SPECIAL_ACTION_COMMAND)gg.controller->CheckSequenceKeyCommand();

	// ���콺�� ������ ���� ���� üũ�Ѵ�.
	if(IsCurrentGuardEnabled())
		eCmd = SPECIAL_KEY_GUARD;

	if (eCmd == SPECIAL_KEY_NONE)
		return false;

	return m_pActionFSM->OnSpecialAction( eCmd );
}

bool XModuleMyControl::CheckUsableSpecialActionKey(XEvent& msg)
{
	// �ŷ�Ʈ ��Ʈ�� ��쿡�� ����� �׼� Ű�� ���� �ȵȴ�.
	if(msg.m_nID == XEVTD_TALENT_HIT)
		return false;

	return true;
}

bool XModuleMyControl::DoAction(ACTION_STATE nState, void* pParam)
{
	if ((GetCurrentActionStateID() == ACTION_STATE_HIT) && (nState == ACTION_STATE_HIT))
	{
		m_pActionFSM->GetCurrentState()->Exit();
		if (m_pActionFSM->GetCurrentState()->CheckEnterable(pParam))
			m_pActionFSM->GetCurrentState()->Enter(pParam);
		else
			return DoAction(ACTION_STATE_IDLE, pParam);
	}
	else
		return m_pActionFSM->ChangeState(nState, pParam);

	return true;
}

bool XModuleMyControl::DoAction(ACTION_STATE nState, int nParam)
{
	return DoAction(nState, &nParam);
}

XMyPlayer* XModuleMyControl::GetMyPlayerOwner()
{
	return m_pOwner->AsMyPlayer();
}

void XModuleMyControl::InitStates()
{
	XMyActionState* pState;

	pState = new XMyActionIdle(this);			m_pActionFSM->AddState(pState);
	pState = new XMyActionJump(this);			m_pActionFSM->AddState(pState);
	pState = new XMyActionFalling(this);		m_pActionFSM->AddState(pState);
	pState = new XMyActionChangeState(this);	m_pActionFSM->AddState(pState);
	pState = new XMyActionHit(this);			m_pActionFSM->AddState(pState);
	pState = new XMyActionGuard(this);			m_pActionFSM->AddState(pState);
	pState = new XMyActionNormalAttack(this);	m_pActionFSM->AddState(pState);
	pState = new XMyActionUseTalent(this);		m_pActionFSM->AddState(pState);
	pState = new XMyActionBow(this);			m_pActionFSM->AddState(pState);
	pState = new XMyActionUseItem(this);		m_pActionFSM->AddState(pState);
	pState = new XMyActionSwitchingWeapon(this);m_pActionFSM->AddState(pState);
	pState = new XMyActionLootingItem(this);	m_pActionFSM->AddState(pState);
	pState = new XMyActionRebirth(this);		m_pActionFSM->AddState(pState);
	pState = new XMyActionDie(this);			m_pActionFSM->AddState(pState);
	pState = new XMyActionHold(this);			m_pActionFSM->AddState(pState);
	pState = new XMyActionGrappled(this);		m_pActionFSM->AddState(pState);
	pState = new XMyActionMapObj(this);			m_pActionFSM->AddState(pState);
	pState = new XMyActionChangeChannel(this);	m_pActionFSM->AddState(pState);
	pState = new XMyActionQuestInteraction(this);	m_pActionFSM->AddState(pState);
	pState = new XMyActionDeactivative(this);	m_pActionFSM->AddState(pState);
	pState = new XMyActionSwallowed(this);		m_pActionFSM->AddState(pState);
	pState = new XMyActionSit(this);			m_pActionFSM->AddState(pState);
	pState = new XMyActionGesture(this);		m_pActionFSM->AddState(pState);
	pState = new XMyActionNPCInteraction(this);	m_pActionFSM->AddState(pState);
	pState = new XMyActionEnchant(this);		m_pActionFSM->AddState(pState);
	pState = new XMyActionDye(this);			m_pActionFSM->AddState(pState);

	// �⺻ ���� ����
	m_pActionFSM->ChangeState(ACTION_STATE_IDLE);
}


/////////////////////////////////////////////////////////////////////////////////////////////
XModuleMyControl::XModuleMyControl(XObject* pOwner)
: XModuleActorControl(pOwner)
, m_bOnTargetObject(false)
, m_bCancelAction(false)
, m_pMouseMove(new XMouseMove(pOwner))
//, m_uidCharacterLightEffect(MUID::ZERO)
{
	_ASSERT(MIsExactlyClass(XMyPlayer, pOwner));

	m_pAttackChecker = new XSpecialActionTransChecker(GetMyPlayerOwner(), &gvar.MyInfo.Talent);
	m_pAttackChecker->Init();

	m_bPaused = false;
	m_bAutoRun = false;
	m_bAutoRunKeyRelease = true;
	m_bRestoreCamera = false;
	m_nMovementFlags = 0;

	m_nCancelParam = -1;

	m_pActionFSM = new XMyActionFSM(this);

	m_TouchSensorCheckRegulator.SetTickTime(0.1f);

	m_vCharacterFrontMoveDir		= -vec3::AXISY;

	m_bDamageRepeat			= false;
	m_bFallingStart = false;
	m_bCurrentFreeLook = false;

	m_bMouseAutoMove = false;
	m_vMouseAutoMove = vec3::ZERO;

	m_pTargetIndicator	= new XTargetIndicator(pOwner);

	m_pChallengerQuestExitChecker = NULL;

	m_pOverlappedEquipmentController = new XMyBPartWeaponController;
}

XModuleMyControl::~XModuleMyControl(void)
{
	SAFE_DELETE(m_pChallengerQuestExitChecker);
	SAFE_DELETE(m_pTargetIndicator);

	m_pActionFSM->DeleteAll();
	SAFE_DELETE(m_pActionFSM);
	SAFE_DELETE(m_pAttackChecker);
	SAFE_DELETE(m_pMouseMove);
}

void XModuleMyControl::OnInitialized()
{
	XModuleActorControl::OnInitialized();
	InitStates();

	m_pModulePost = m_pOwner->GetModulePost();
	_ASSERT(m_pModulePost);

	//CreateCharacterLightEffect();
}

void XModuleMyControl::OnSubscribeEvent()
{
	XModuleActorControl::OnSubscribeEvent();

	Subscribe(XEVTL_VIRTUAL_KEY_PRESSED);
	Subscribe(XEVTL_VIRTUAL_KEY_PRESSEDONCE);
	Subscribe(XEVTL_VIRTUAL_KEY_RELEASED);
	Subscribe(XEVTD_TALENT_HIT);

	// MyActionState���� �޴� �޼��� --
	Subscribe(XEVTL_ON_MOTION_EVENT);
	Subscribe(XEVTL_ON_CHANGE_MOTION);
	Subscribe(XEVTL_ON_CHANGE_ANIMATION);

	Subscribe(XEVTL_MOTION_PLAYONCE);
	Subscribe(XEVTL_UPPER_MOTION_PLAYONCE);
	
	Subscribe(XEVTL_ON_RELEASE_TALENT_KEY);

	Subscribe(XEVTL_ON_CHANGED_MOTION_TO_IDLE);
	Subscribe(XEVTL_JUMP_FAILED);

	Subscribe(XEVTL_TALENT_START);
	Subscribe(XEVTL_TALENT_CANCELED);
	Subscribe(XEVTL_TALENT_ACT_SPELL);
	Subscribe(XEVTL_TALENT_RECASTING);

	Subscribe(XEVTL_USE_ITEM);
	//Subscribe(XEVTL_HOLD_KEYABLE);

	Subscribe(XEVTL_MYACTION_SWITCHING_WEAPON_BEGIN);
	Subscribe(XEVTL_MYACTION_SWITCHING_WEAPON_START);
	Subscribe(XEVTL_MYACTION_USE_TALENT);
	Subscribe(XEVTL_MYACTION_NPC_INTERACTION_END);

	Subscribe(XEVTL_LOOT_END_READY);
	Subscribe(XEVTL_LOOT_END);
	Subscribe(XEVTL_LOOT_GETITEM);
	Subscribe(XEVTL_LOOT_GETALLITEM);

	Subscribe(XEVTL_CHANGE_STANCE);
	Subscribe(XEVTL_CHANGED_STANCE);
	
	Subscribe(XEVTL_MYACTION_RELEASE_ACTION_HOLD);

	Subscribe(XEVTL_THROW);
	Subscribe(XEVTL_GRAPPLED_TERRAIN_COL);

	Subscribe(XEVTL_CRAFTING_REQ);	

	Subscribe(XEVTL_EXPCEPTION_HOLD);

	Subscribe(XEVTD_END_MF_STUN);
	Subscribe(XEVTD_END_MF_KNOCKDOWN);

	Subscribe(XEVTL_GUARD_KNOCKBACK);
	Subscribe(XEVTL_GUARDED);
	Subscribe(XEVTL_GUARD_FAILED);

	Subscribe(XEVTL_INTERACTION_CLICK);
	Subscribe(XEVTL_MOUSE_MOVE_CLICK);

	Subscribe(XEVTL_SWALLOWED_DISAPPEAR);

	Subscribe(XEVTL_ENCHANT_ITEM_SELECT);
	Subscribe(XEVTL_ENCHANT_START);
	Subscribe(XEVTL_ENCHANT_CANCEL);
	Subscribe(XEVTL_ENCHANT_CHECK_REQ);
	Subscribe(XEVTL_ENCHANT_CHECK_SUCCESS);
	Subscribe(XEVTL_ENCHANT_RESULT);

	Subscribe(XEVTL_DYE_ITEM_SELECT);
	Subscribe(XEVTL_DYE_START_ITEM);
	Subscribe(XEVTL_DYE_END_ITEM);
	Subscribe(XEVTL_DYE_CANCEL);

	Subscribe(XEVTL_CAMERA_PVP_VIEW_CLEAR);

	Subscribe(XEVTL_DIE_ANI_END);
	Subscribe(XEVTD_ACTION_SLEEP);
}

XEventResult XModuleMyControl::OnEvent(XEvent& msg)
{
	if (XModuleActorControl::OnEvent(msg) == MR_TRUE) return MR_TRUE;

	if (CheckActionEventAcceptable(&msg) == false)	
	{
		if (msg.m_nID == XEVTL_VIRTUAL_KEY_PRESSED ||
			msg.m_nID == XEVTL_VIRTUAL_KEY_RELEASED ) return MR_IGNORED;
	}

	// key sequence process
	SPECIAL_ACTION_COMMAND eCmd		= SPECIAL_KEY_NONE;
	if (CheckUsableSpecialActionKey(msg) && CheckSpecialActionKey( eCmd ))
	{
		global.ui->GetHelpMgr()->SpecialActionCmd( eCmd );
		return MR_TRUE;
	}

	// Helper
	global.ui->GetHelpMgr()->MyControlEvent( msg );

	// ActionFSM event process
	if (m_pActionFSM->Event(msg) == MR_TRUE) return MR_TRUE;

	switch (msg.m_nID)
	{
	case XEVTL_VIRTUAL_KEY_PRESSED:
		{
			XVirtualKey nVirtualKey = *static_cast<XVirtualKey*>(msg.m_pData);
			switch (nVirtualKey)
			{
			case VKEY_ACTION:
//			case VKEY_GUARD:
				{
					m_bAutoRun=false;
					m_bMouseAutoMove = false;
				}
				break;
			case VKEY_AUTO_RUN:
				{
					if(GetCurrentActionStateID() != ACTION_STATE_IDLE || 
						m_bAutoRunKeyRelease == false)
						break;

					m_bAutoRun = !m_bAutoRun;
					m_bMouseAutoMove = false;
					m_bAutoRunKeyRelease = false;
					if (m_bAutoRun)
					{
						SyncPlayerDirToCameraDir();
						m_bCurrentFreeLook = false;
					}
					else
					{
						m_pModuleAction->Stop();
					}
				}
				break;
			case VKEY_LOCKTARGET:
				{
					if (XGetInteractionInfo().stTargetActorInfo.bLockOnTarget)
					{
						m_pTargetIndicator->Init();
						XGetInteractionInfo().SetTargetUIID(0);
					}
					else
					{
						UIID uiidSearch = XGetInteractionInfo().GetSearchUIID();

						XNetDebuger::PostAIMonitorLockTarget(uiidSearch);				// ��� ����̸鼭 ���� ������ �������� ������ ������

						XObject* pObject = gg.omgr->FindActor_UIID(uiidSearch);
						if (pObject && !pObject->AsActor()->IsDead())
							XGetInteractionInfo().SetTargetUIID(uiidSearch);
						else
							return MR_TRUE;
					}

					XGetInteractionInfo().stTargetActorInfo.bLockOnTarget = !XGetInteractionInfo().stTargetActorInfo.bLockOnTarget;
					XGetInteractionInfo().stTargetActorInfo.nLockOnTime = timeGetTime();

					if(global.script)
					{
						global.script->GetGlueGameEvent().OnTargetInfoRefresh();
						global.script->GetGlueGameEvent().OnEnemyInfoRefresh();
					}
				}
				break;
			};
		}
		return MR_TRUE;
	case XEVTL_VIRTUAL_KEY_RELEASED:
		{
			XVirtualKey nVirtualKey = *static_cast<XVirtualKey*>(msg.m_pData);
			switch (nVirtualKey)
			{
			case VKEY_FORWARD:
			case VKEY_BACKWARD:
			case VKEY_LEFT:
			case VKEY_RIGHT:
				{
					if (GetCurrentActionStateID() == ACTION_STATE_IDLE) 
					{
						if (m_bAutoRun == false && m_bMouseAutoMove == false && 
							GetCurrMotionType() == MT_DEFAULT)
						{
							m_pModuleAction->Stop();
						}
					}
				}
				break;
			case VKEY_TOGGLE_FREECAMERA:
				{
					if (gvar.MyInfo.ChrInfo.nPlayerGrade >= PLAYER_GRADE_DEVELOPER)
					{
						m_bCurrentFreeLook = !m_bCurrentFreeLook;
					}					
				}
				break;
			case VKEY_AUTO_RUN:
				{
					m_bAutoRunKeyRelease = true;
				}
				break;
			}
		}
		return MR_TRUE;
	case XEVTD_TALENT_HIT:
		{
			HitByTalent(static_cast<XTalentHitParam*>(msg.m_pData));
			SAFE_DELETE(msg.m_pData);
		}
		return MR_TRUE;

	case XEVTL_ON_CHANGED_MOTION_TO_IDLE:
		{
			DoActionIdle();
			break;
		}
	case XEVTL_CHANGE_STANCE:
		{
			if ( m_pModuleAction)
				m_pModuleAction->StopMove();

			DoAction( ACTION_STATE_CHANGE_STANCE);
			break;
		}
	case XEVTL_CHANGED_STANCE:
		{
			if ( m_ChangeWeaponInfo.IsEmpty() == false)
				ChangeWeapon();

			break;
		}
	case XEVTL_JUMP_FAILED:
		{
			if (GetCurrentActionStateID() == ACTION_STATE_JUMP)
			{
				DoActionIdle();
			}
			break;
		}
	case XEVTL_TALENT_CANCELED:
		{
			if (GetCurrentActionStateID() == ACTION_STATE_USE_TALENT ||
				GetCurrentActionStateID() == ACTION_STATE_USE_ITEM ||
				GetCurrentActionStateID() == ACTION_STATE_NORMAL_ATTACK)
			{
				DoActionIdle();
			}
			break;
		}
	case XEVTL_EXPCEPTION_HOLD:
		{
			Stop();
		}
		break;
	case XEVTL_INTERACTION_CLICK:
		{
			MPoint ptCursorPos = *(static_cast<MPoint*>(msg.m_pData));
		}
		break;
	case XEVTL_MOUSE_MOVE_CLICK:
		{
			MPoint ptCursorPos = *(static_cast<MPoint*>(msg.m_pData));
			MouseMoveClick(ptCursorPos);
		}
		break;
	};

	return MR_FALSE;
}

XMotionTypes XModuleMyControl::GetCurrMotionType()
{
	XMotionTypes nMotionType = MT_DEFAULT;

	if ( global.ui->IsMouseVisible() == false)
	{
		if ( gg.controller->IsVirtualKeyPressedAsync( VKEY_FORWARD))		nMotionType = MT_FORWARD;
		else if ( gg.controller->IsVirtualKeyPressedAsync(VKEY_BACKWARD))	nMotionType = MT_BACKWARD;
		else if ( gg.controller->IsVirtualKeyPressedAsync( VKEY_LEFT))		nMotionType = MT_LEFT;
		else if ( gg.controller->IsVirtualKeyPressedAsync( VKEY_RIGHT))		nMotionType = MT_RIGHT;
	}

	return nMotionType;
}

//////////////////////////////////////////////////////////////////////////
void XModuleMyControl::OnUpdate(float fDelta)
{
	PFC_THISFUNC;

	PFI_BLOCK_THISFUNC(304);
	XModuleActorControl::OnUpdate(fDelta);

	_UpdateMyCharacterAlpha();
}

bool XModuleMyControl::OnUpdateCheckUpdatable()
{
	return (gg.omgr != NULL);
}

void XModuleMyControl::OnUpdateState( float fDelta )
{
	PFC_THISFUNC;

	// Ż��Ʈ ���� ������Ʈ
	if ( m_pModuleTalent->IsActive())		
	{
		int nID = -1;
		if(m_pModuleTalent->GetInfo())
		{
			nID = m_pModuleTalent->GetInfo()->m_nID;
		}

		m_pModuleMovable->TriggerTalentActive(nID);
	}
	else									m_pModuleMovable->TriggerTalentDeActive();

	// Falling ���� üũ
	UpdateStateFalling();


	m_pActionFSM->Update(fDelta);

	XModuleActorControl::OnUpdateState(fDelta);
}


void XModuleMyControl::OnUpdateCommand( float fDelta )
{
	PFC_THISFUNC;

	// ī�޶� �������� �̵�
	vec3 dir(gg.controller->GetBackViewCameraDir());

	// ����Ʈ ��ġ(�ӽ� ����)
	//_UpdateLightEffectPos(dir);

	// ���� �̵����� ������Ʈ �� ī�޶� ����
	_UpdateFrontDirAndSetCamera( fDelta, dir);

	// ĳ���� ���� ������Ʈ
	_UpdateMyDir( fDelta, dir);

	// �� ĳ���� �̵� ������Ʈ
	bool bKeyAction = _UpdateMyMovement(fDelta, m_pModuleEntity, dir);

	// post packet
	m_pModulePost->UpdateMovement(fDelta, m_pModuleEntity->GetPosition(), dir, bKeyAction, m_nMovementFlags);

	// ��輱 ���
	_UpdateRestrictedArea(fDelta);
	
	// post key event
	if (GetCurrentActionStateID() == ACTION_STATE_IDLE)
	{
		queue<XVirtualKey>& q = gg.controller->GetReservedKey();
		while (!q.empty())
		{
			XEvent evt;
			evt.m_nID = XEVTL_VIRTUAL_KEY_PRESSED;
			XVirtualKey key = q.back();
			evt.m_pData = &key;
			Event(evt);

			q.pop();
		}
	}
}

void XModuleMyControl::OnUpdateSpecial( float fDelta )
{
	PFC_THISFUNC;

	_UpdateTouchSensorTrigger(fDelta);

	if(m_pTargetIndicator)	m_pTargetIndicator->OnUpdate();

	m_MapSignAreaChecker.UpdateTouchAreaTrigger(m_pOwner, fDelta);

	if (m_pChallengerQuestExitChecker)
		m_pChallengerQuestExitChecker->Update(fDelta);
}

void XModuleMyControl::OnPostUpdate(float fDelta)
{
	PFC_THISFUNC;

	m_pActionFSM->PostUpdate(fDelta);

	XModuleActorControl::OnPostUpdate(fDelta);
}

//////////////////////////////////////////////////////////////////////////

bool XModuleMyControl::_UpdateMyMovement(float fDelta, XModuleEntity* pModuleEntity, vec3& vDir)
{
	if ( m_bPaused == true)	return false;

	if (gg.controller->IsMainBackViewCamera() == false) return false;


	// ������ ���� �̵� �������� Ȯ��
	XBuffAttribute attrBuff = m_pModuleBuff->GetBuffAttribute();
	if ( attrBuff.IsMovable() == false)
		return false;

		
	unsigned int nTempMovementFlags = m_nMovementFlags;				// Ű ��ȯ�� ���� üũ
	m_nMovementFlags = 0;

	const int nActionFSMStateID = GetCurrentActionStateID();
	const XMovableFactor& movableFactor = m_pModuleMovable->GetMovableFactor();

	if (nActionFSMStateID != ACTION_STATE_IDLE &&
		nActionFSMStateID != ACTION_STATE_CHANGE_STANCE &&
		nActionFSMStateID != ACTION_STATE_WEAPON_CHANGE) 
	{
		bool bReturn = true;
		if (nActionFSMStateID == ACTION_STATE_HIT) m_nMovementFlags |= MOVEMENTFLAG_MOTION_FACTOR;
		else if(nActionFSMStateID == ACTION_STATE_GUARD) m_nMovementFlags |= MOVEMENTFLAG_GUARD;

		if (movableFactor.IsFalling()) 
		{
			m_nMovementFlags |= MOVEMENTFLAG_FALLING;
		}
		if( nActionFSMStateID == ACTION_STATE_JUMP)
		{
			if(m_pActionFSM->OnSpecialAction(SPECIAL_KEY_NONE) == false)
				m_nMovementFlags |= MOVEMENTFLAG_JUMPING;
			else
				bReturn = false;
		}

		if (nActionFSMStateID == ACTION_STATE_USE_TALENT) 
		{
			if(m_pActionFSM->OnSpecialAction(SPECIAL_KEY_NONE) == false)
				m_nMovementFlags |= MOVEMENTFLAG_USING_TALENT;
			else 
				bReturn = false;
		}

		if((nActionFSMStateID == ACTION_STATE_NORMAL_ATTACK && CheckAttackReturnAnimation()))
			bReturn = false;

		if(bReturn)
		{
			m_pModuleAction->InitPrevDir();
			return false;
		}
	}

	bool bMoveKeyPress = false;
	vec3 dir = vDir;

	// ���䷱�ΰ�?
	if (m_bAutoRun)
	{
		bMoveKeyPress = true;
		m_nMovementFlags = MOVEMENTFLAG_FORWARD;
	}

	// ���콺 �̵��ΰ�?
	if(m_bMouseAutoMove)
	{
		CheckMouseMoveState();

		if (CheckActionEventAcceptable() == false &&
			global.ui->IsMouseCursorToggleKeyDown() == false )
			return false;
	}
	// �ؿ����ʹ� �׼� �̺�Ʈ(Ű ��ǲ ��) ���źҰ����Ҷ��� �����ع�����. ���� flag�� ��Ȱ��ȭ�϶����� ��������� �ϴ� �͵�.
	else if (CheckActionEventAcceptable() == false &&
		global.ui->IsMouseCursorToggleKeyDown() == false )
	{
		float fSpeed = GetMyPlayerOwner()->GetMoveSpeed(m_nMovementFlags);
		XMovableGoParam* pGoParam = new XMovableGoParam(XMovableGoParam::E_GO_DIRECTION);
		XMovableFactor::SInterpolation param = m_pModuleMovable->GetMovableFactor().INTERPOLATION;
		pGoParam->Set( param.m_vTargetPos, dir, m_fSpeed, param.m_fRotateSpeed, BOOST_NONE, m_nMovementFlags , true );
		pGoParam->SetKeyPress( bMoveKeyPress);
		m_pModuleAction->UpdateMyMovementAction( fDelta, pGoParam);
		delete pGoParam;

		return false;
	}


	if (gg.controller->IsVirtualKeyPressedAsync(VKEY_FORWARD))
	{
		m_bAutoRun = false;
		m_bMouseAutoMove = false;
		bMoveKeyPress = true;
		m_nMovementFlags = MOVEMENTFLAG_FORWARD;
	}
	else if (gg.controller->IsVirtualKeyPressedAsync(VKEY_BACKWARD))
	{
		m_bAutoRun = false;
		m_bMouseAutoMove = false;
		bMoveKeyPress = true;
		m_nMovementFlags = MOVEMENTFLAG_BACKWARD;

		dir = -vDir;
	}

	if (gg.controller->IsVirtualKeyPressedAsync(VKEY_LEFT))
	{
		m_bMouseAutoMove = false;
		vec3 pos = pModuleEntity->GetPosition();
		vec3 left = -CrossProduct(vDir, UP_VECTOR);
		left.z = 0.0f;
		left.Normalize();

		if (bMoveKeyPress)
		{
			dir = dir + left;
			dir.Normalize();
			m_nMovementFlags |= MOVEMENTFLAG_LEFT;
		}
		else
		{
			dir = left;
			m_nMovementFlags = MOVEMENTFLAG_LEFT;
			bMoveKeyPress = true;
		}
	}
	else if (gg.controller->IsVirtualKeyPressedAsync(VKEY_RIGHT))
	{
		m_bMouseAutoMove = false;
		vec3 right = CrossProduct(vDir, UP_VECTOR);
		right.z = 0.0f;
		right.Normalize();

		if (bMoveKeyPress)
		{
			dir = dir + right;
			dir.Normalize();
			m_nMovementFlags |= MOVEMENTFLAG_RIGHT;
		}
		else
		{
			dir = right;
			m_nMovementFlags = MOVEMENTFLAG_RIGHT;
			bMoveKeyPress = true;
		}
	}


	// ���ǵ� ���
	m_fSpeed = GetMyPlayerOwner()->GetMoveSpeed(m_nMovementFlags);

	// ����׽� ����Ʈ Ű�� ������ �̵� �ӵ��� ������.
#ifdef _DEBUG
	if (global.ui->IsDebugSpeedUpKeyDown())
		m_fSpeed *= 5.0f;
#endif

	if(m_bMouseAutoMove &&
		bMoveKeyPress == false)
	{
		return true;
	}

	static bool bLastMoving = bMoveKeyPress;
	if (bMoveKeyPress==false && bLastMoving==true)
	{
		if (nActionFSMStateID == ACTION_STATE_IDLE) 
		{
			if (m_pModuleAction->GetCurrentMotionName() == MOTION_NAME_RUN)
			{
				if (GetCurrMotionType() == MT_DEFAULT)
				{
					m_pModuleAction->Stop();
				}
			}
		}
	}
	bLastMoving = bMoveKeyPress;

	XMovableGoParam* pGoParam = new XMovableGoParam(XMovableGoParam::E_GO_DIRECTION);
	XMovableFactor::SInterpolation param = m_pModuleMovable->GetMovableFactor().INTERPOLATION;
	pGoParam->Set( param.m_vTargetPos, dir, m_fSpeed, param.m_fRotateSpeed, BOOST_NONE, m_nMovementFlags , true );
	pGoParam->SetKeyPress( bMoveKeyPress);
	m_pModuleAction->UpdateMyMovementAction( fDelta, pGoParam);
	delete pGoParam;

	if( nTempMovementFlags != m_nMovementFlags)
	{
		return true;
	}

	return false;
}


void XModuleMyControl::_UpdateTouchSensorTrigger(float fDelta)
{
	PFC_THISFUNC;

	if (!m_TouchSensorCheckRegulator.IsReady(fDelta))		return;
	if (gg.currentgamestate->GetWorld()->IsNowLoading())	return;

	XFieldInfo* pFieldInfo = gg.currentgamestate->GetWorld()->GetInfo();
	if (pFieldInfo==NULL) return;

	vec3 vMyPos = m_pModuleEntity->GetPosition();
	m_pSensorController->Update(pFieldInfo, vMyPos);
}

//////////////////////////////////////////////////////////////////////////

void XModuleMyControl::OnHit(XTalentHitParam* pHitInfo)
{
	m_HitController.SetHitData(pHitInfo);
	XModuleActorControl::OnHit(pHitInfo);
}

void XModuleMyControl::OnDie( MF_STATE nMFState /*= MF_NONE*/, bool bSetAni /*= true*/ )
{
	m_bAutoRun = false;
	m_bMouseAutoMove = false;

	GetMyPlayerOwner()->OnDie();

	DoForceCameraLock();

	DoActionDie();
}

void XModuleMyControl::DoActionDie()
{
	if (m_bReservedHitByTalent == false)
	{
		DoAction(ACTION_STATE_DIE);
	}
}

void XModuleMyControl::Rebirth()
{
	GetMyPlayerOwner()->OnRebirth();

	m_bAutoRun = false;
	m_bMouseAutoMove = false;

	// ���� �׼ǿ� ���� �����͸� �ʱ�ȭ�Ѵ�.
	if(m_pOwner->GetModuleAction())
	{
		m_pOwner->GetModuleAction()->EndDie();
	}


	// �̵� ���� ���� �ʱ�ȭ
	if ( m_pModuleMovable)
		m_pModuleMovable->TriggerDie( false);
}

void XModuleMyControl::OnRebirth()
{
	Rebirth();

	DoAction(ACTION_STATE_REBIRTH);
}

void XModuleMyControl::OnRebirthSoulBinding()
{
	Rebirth();

	DoActionIdle();
}

void XModuleMyControl::HitByTalent(XTalentHitParam * pHitInfo)
{
	m_HitController.DelHitData();

	CheckReservedHitByTalent();

	XObject* pAttackObject = gg.omgr->FindActor_UIID(pHitInfo->nAttackerUIID);
	if (pAttackObject && pAttackObject->GetModuleTalent())
	{
		pAttackObject->GetModuleTalent()->OnHitTarget(pHitInfo->nTalentID);
	}

	m_bAutoRun					= false;
	m_bDamageRepeat				= false;
	m_bMouseAutoMove			= false;

	bool bDieCheck				= false;

	//if(m_pOwner->GetUID() == XGetMyUID())
	//	mlog("��� ���� %s(%d)\n", MLocale::ConvTCHARToAnsi(MF_STATE_NAME[pHitInfo->nMFState]).c_str(), pHitInfo->nMFWeight);

	// �׾��ٸ�.. �ǰ��� �������� �ʴ´�.
	//if(m_DeathController.IsDeadActionImmediatelyOnHit(GetMyPlayerOwner(), pHitInfo->nMFState))
	//{
	//	OnDie();
	//}
	//else 
	{
		// �ŷ�Ʈ ��� üũ		
		if(m_pModuleTalent->IsActivate() && m_pModuleTalent->GetInfo() &&
			(m_pModuleTalent->GetInfo()->m_nSkillType == ST_SOCIAL ||																	// �Ҽ� �ŷ�Ʈ ������̸� ��Ұ� �ǰ�
			(m_pModuleTalent->GetInfo()->m_bCancelable && XTalentHelper::IsTalentCancelableMotionFactor(pHitInfo->nMFState)) ))			// �ּ� ������ �ŷ�Ʈ üũ
			m_pModuleTalent->Cancel();
	

		if(pHitInfo->nMFState != MF_GRAPPLED &&
			pHitInfo->nMFState != MF_SWALLOWED)
		{
			if (!CSMotionFactorHelper::IsNoneOrFakes(pHitInfo->nMFState))
			{
				if (pHitInfo->nMFWeight > 0)
				{
					DoAction(ACTION_STATE_HIT, pHitInfo);
					bDieCheck = true;
				}
			}
			// MF_NONE�϶� �ŷ�Ʈ ������̶��... ������͸� ���Ѵ�. 
			else if (m_pModuleTalent->IsActive() == false || pHitInfo->nMFState == MF_FAKE_KNOCKBACK)
			{
				stMotionFactorResult rtMotionFactor;
				stServerKnockbackVictimInfo knockbackInfo(pHitInfo->vServerKnockbackDir, pHitInfo->vServerKnockbackVictimPos, pHitInfo->vServerKnockbackVictimTargetPos);
				m_pModuleAction->ActMF(pHitInfo->nMFState, pHitInfo->nMFWeight, pHitInfo->nAttackerUIID, knockbackInfo, pHitInfo->nTalentID, rtMotionFactor);

				// ���� ó��...
				if(GetMyPlayerOwner()->IsDead())
				{
					OnDie(pHitInfo->nMFState, rtMotionFactor.bUseMFAnimation);
					bDieCheck = true;
				}
			}
		}
		else
		{
			if(pHitInfo->nMFState == MF_GRAPPLED)
			{
				// ��� ����̶��...
				_DamageRepeatInfo * pDamageRepeatInfo = InitDamageRepeat(pHitInfo);
				if(StartGrappled(pDamageRepeatInfo) == false)
					SAFE_DELETE(pDamageRepeatInfo);
			}
			else if(pHitInfo->nMFState == MF_SWALLOWED)
			{
				// �Ա� ����̶��...
				_DamageRepeatInfo * pDamageRepeatInfo = InitDamageRepeat(pHitInfo);
				if(StartSwallowed(pDamageRepeatInfo) == false)
					SAFE_DELETE(pDamageRepeatInfo);
			}

			return;
		}
	}

	// ���� ó��...
	if(bDieCheck == false && GetMyPlayerOwner()->IsDead())
	{
		OnDie(pHitInfo->nMFState);
	}

	// �ǰݹ� ������ �����ֱ�
	_DamageInfo damageInfo;
	damageInfo.nDamage				= pHitInfo->nDamage;
	damageInfo.nFlags				= pHitInfo->nFlags;
	damageInfo.nMFState				= pHitInfo->nMFState;
	damageInfo.nAttackedTalentID	= pHitInfo->nTalentID;
	damageInfo.nAttackerUIID		= pHitInfo->nAttackerUIID;
	damageInfo.bSecondaryEffect		= pHitInfo->bSecondaryEffect;

	ShowHitAndDamage(&damageInfo, pAttackObject);
	FakeBeatenAndSetMultiplyColor(pHitInfo);

	BirdTest_ShowDelay(pHitInfo);
}

void XModuleMyControl::SetCancelAction(int nParam)
{
	m_bCancelAction = true;
	m_nCancelParam = nParam;
	m_nLastCancelActionTime = XGetNowTime();

	XEvent evt;
	evt.m_nID = XEVTL_ON_CAN_CANCEL_ACTION;
	Event( evt);
}

void XModuleMyControl::DoActionUseItem( int nSlotID )
{
	bool bRet = DoAction(ACTION_STATE_USE_ITEM, &nSlotID);
	//_ASSERT(bRet);
}

void XModuleMyControl::DoActionUseTalent(int nTalentID)
{
	XEvent evt;
	evt.m_nID = XEVTL_MYACTION_USE_TALENT;
	evt.m_pData = (void*)&nTalentID;
	Event( evt);
}

void XModuleMyControl::DoActionEquipItem( SH_ITEM_SLOT nSlot, MUID uidItemID)
{
	// ! XItem::m_uidID ���ŵǾ�, slotID�θ� ������ �� �ֽ��ϴ�. - praptor, 2010.03.03
	// ! ������� �ʴ� �Լ��� �ǴܵǾ� ���忡�� ������, ���� �ּ�ó���մϴ�. - praptor, 2010.03.03

	//// ���� ���¶��
	//if ( GetMyPlayerOwner()->GetStance() == CS_BATTLE)
	//{
	//	// ���� ����ϴ� ����� ��ü�Ϸ��� ���� ������ ������ Ȯ��
	//	int nWeaponSet = gvar.MyInfo.EquipmentSlot.GetWeaponSet();
	//	if ( (nSlot == ITEMSLOT_RWEAPON  &&  nWeaponSet == 0)  ||  (nSlot == ITEMSLOT_RWEAPON2  &&  nWeaponSet == 1))
	//	{
	//		// ���� ���� ���� ����
	//		m_ChangeWeaponInfo.Set( nSlot, uidItemID);

	//		// ���Ľ� ����
	//		DoAction( ACTION_STATE_CHANGE_STANCE);

	//		return;
	//	}
	//}

	//m_pModulePost->PostEquipItem( nSlot, uidItemID);
}

// cur �������� tar �������� ���� ª�� ȸ���ϴ� ���� ���Ѵ�
float GetLowerDiffAngle( float cur, float tar)
{
	static const float _2PI = MMath::PI * 2.0f;

	float fDiff1 = tar - cur;
	float fDiff2 = (fDiff1 > 0.0f) ? (fDiff1 - _2PI) : (fDiff1 + _2PI);
	float ang = ( abs( fDiff1) < abs( fDiff2)) ? fDiff1 : fDiff2;

	return ang;
}

void XModuleMyControl::_UpdateMyDir( float fDelta, const vec3& dir )
{
	if ( m_bPaused == true)
		return;

	if ( gg.controller->IsMainBackViewCamera() == false)
		return;

	if ( m_bOnTargetObject == true)
		return;

	if ( GetMyPlayerOwner()->IsDead())
		return;


	// ������ ���� ȸ�� �������� Ȯ��
	XBuffAttribute attrBuff = m_pModuleBuff->GetBuffAttribute();
	if ( attrBuff.IsMovable() == false)
		return;

	// ���� ���¿����� ȸ�� ����...
	if(CheckGuardKnockback())
		return;


	vec3 vMyDir = Normalize( m_pOwner->GetDirection());
	float fTarDir = dir.GetAngleXY();
	float fMyDir = vMyDir.GetAngleXY();


	// �밢�� ���� �̵��� ���� ȸ���� ����
	static const float fLimitRotateAngleA = MMath::PI / 8.0f;
	static const float fLimitRotateAngleB = MMath::PI / 8.0f + 0.3f;
	static const float fRotSpeed = 7.0f;

	if ( ( (m_nMovementFlags & MOVEMENTFLAG_FORWARD)  &&  (m_nMovementFlags & MOVEMENTFLAG_LEFT))  ||
		( (m_nMovementFlags & MOVEMENTFLAG_BACKWARD)  &&  (m_nMovementFlags & MOVEMENTFLAG_RIGHT)) )
	{
		fTarDir += fLimitRotateAngleA;
	}
	else if ( ( (m_nMovementFlags & MOVEMENTFLAG_FORWARD)   &&  (m_nMovementFlags & MOVEMENTFLAG_RIGHT))  ||
		( (m_nMovementFlags & MOVEMENTFLAG_BACKWARD)  &&  (m_nMovementFlags & MOVEMENTFLAG_LEFT)) )
	{
		fTarDir -= fLimitRotateAngleA;
	}


	// ī�޶� ȸ���� ���� ĳ���� ȸ��
	float fDiff = GetLowerDiffAngle( fMyDir, fTarDir);
	if ( abs( fDiff) < 0.01f)
		return;

	float fRot = 0.0f;
	if ( fDiff > 0.0f)			fRot = min( fDiff,  fRotSpeed * fDelta);
	else if ( fDiff < 0.0f)		fRot = max( fDiff, -fRotSpeed * fDelta);

	float _diff = fDiff - fRot;
 	if ( _diff > fLimitRotateAngleB)		fRot += _diff - fLimitRotateAngleB;
	else if ( _diff < -fLimitRotateAngleB)	fRot += _diff + fLimitRotateAngleB;


	// ȸ�� ����
	vec3 vNextDir;
	vNextDir.x = m_pOwner->GetDirection().x * cos( fRot) - m_pOwner->GetDirection().y * sin( fRot);
	vNextDir.y = m_pOwner->GetDirection().y * cos( fRot) + m_pOwner->GetDirection().x * sin( fRot);
	vNextDir.z = 0.0f;
	Normalize( vNextDir);

	m_pOwner->SetDirection( vNextDir);
}

void XModuleMyControl::_UpdateFrontDirAndSetCamera( float fDelta, vec3 & dir)
{
	//////////////////////////////////////////////////////////////////////////
	// ��Ʈ��
	// ��Ʈ���� �ƴ϶� ��밡���Ѱ�?
	if(m_bCurrentFreeLook)
	{
		m_bCurrentFreeLook = false;
		if(m_bAutoRun ||																	// ���䷱�϶�
			m_bMouseAutoMove ||																// ���콺 �̵���
			(GetCurrentActionStateID() == ACTION_STATE_IDLE && m_nMovementFlags == 0))		// �̵��� ���� IDLE�϶�
		{
			m_bCurrentFreeLook = true;
		}
	}
		
	// ��Ʈ�� ���� �� ���콺 �̵�
	if (global.ui)
	{
		/*if(global.ui->IsFreeCameraKeyDown())
		{
			// ��Ʈ�� Ű�� ���콺 �̵��� ������� �۵�
			m_bCurrentFreeLook = !m_bCurrentFreeLook;
		}
		else*/ if(m_bMouseAutoMove)
			m_bCurrentFreeLook = true;
	}

	// ī�޶� ������� ����
	gg.controller->SetFreeLookCamera(m_bCurrentFreeLook);

	//////////////////////////////////////////////////////////////////////////
	// ����
	bool bMotionCameraLook = m_pModuleAction->IsCameraLook();
	if(!bMotionCameraLook || m_bCurrentFreeLook ||
		gg.controller->IsMainBackViewCamera() == false)		// BackViewCamera�� �ƴ϶��...
	{
		// ĳ���� �̵� ���� ������ ����
		dir = m_vCharacterFrontMoveDir;
	}
	else
	{
		// ī�޶� �������� ����
		m_vCharacterFrontMoveDir = dir;
	}

	XMath::NormalizeZ0(dir);
}

void XModuleMyControl::SyncPlayerDirToCameraDir()
{
	if (global.ui)
	{
		if (global.ui->IsFreeCameraKeyDown()) return;
	}

	// ī�޶� �������� �̵�
	vec3 vCamDir;
	if (global.camera->IsPrimaryCameraType(CAMERA_GRAPPLED))
	{
		// grapple camera�� �۵����϶��� ���� ī�޶��� ������ ĳ������ �������� �����Ѵ�.
		// Ticket #1930 ���ù���
		vCamDir = global.camera->GetPrimaryCamera()->GetDirection();
	}
	else
	{
		vCamDir = gg.controller->GetBackViewCameraDir();
	}

	vCamDir.z = 0.0f;
	vCamDir.Normalize();
	m_pOwner->SetDirection(vCamDir);
	m_vCharacterFrontMoveDir = vCamDir;
}


// CancelAction �׼��� ���ο� �ŷ�Ʈ�� ���ų� ���� �ð��� ������ Ǯ����.
void XModuleMyControl::ReleaseCancelAction()
{
	m_bCancelAction = false;
	m_nCancelParam = -1;
}

bool XModuleMyControl::CheckActionEventAcceptable( XEvent* pEvent /*= NULL*/ )
{
	//	if (global.ui->IsFocusMainInput() == false) return false;
	if (CheckActionEventAcceptableByMouseVisible(pEvent) == false &&
		global.ui->IsMouseVisible()) return false;

	return true;
}

void XModuleMyControl::OnSwitchingWeaponBegin()
{
	// ���� ü���� ����
	XEvent msg1(XEVTL_MYACTION_SWITCHING_WEAPON_BEGIN);
	m_pActionFSM->Event(msg1);
}

void XModuleMyControl::OnSwitchingWeapon( int8 nWeaponSet )
{
	// ���� ü����
	//mlog("����Ī %d\n", nWeaponSet);
	XEvent msg2(XEVTL_MYACTION_SWITCHING_WEAPON_START, &nWeaponSet);
	if(m_pActionFSM->Event(msg2) == MR_FALSE)
	{
		// �̺�Ʈ�� �������� ������... ���� �ٸ� ���·� �ν�
		// �ٸ� �׼��� ������ �ʰ� �׳� ���⸸ �ٲ۴�.
		gvar.Game.pMyPlayer->SwitchingWeaponSet(nWeaponSet);
	}
}

void XModuleMyControl::OnLootEndReady()
{
	// ���� ���� �غ�
	XEvent msg(XEVTL_LOOT_END_READY);
	m_pActionFSM->Event(msg);
}

void XModuleMyControl::OnHitLooting()
{
	// �������϶� ������...
	XEvent msg(XEVTL_LOOT_END_READY);
	if(m_pActionFSM->Event(msg) == MR_FALSE)
	{
		// ���� ���°� �ƴ϶��...
		// â�̶� �ݴ´�.
		if(global.script)
		{
			global.script->GetGlueGameEvent().OnLootingEnd();
		}
	}
}

bool XModuleMyControl::IsCanStopMouseToggle()
{
	// ���콺 ����� ������ ĳ���� ���� �� �� �ִ� �����ΰ�?
	int nStateID = m_pActionFSM->GetCurrentState()->GetID();
	if(GetMyPlayerOwner()->IsDead() == false &&				// ���� ���°� �ƴ϶��...
		nStateID== ACTION_STATE_IDLE &&						// ���� FSM ���¿� ���� ���
		m_bAutoRun == false)
	{
		return true;
	}

	return false;
}

bool XModuleMyControl::IsCanCancelAllMotion()
{
	return (m_bCancelAction && m_nCancelParam == CANCEL_NEXT_ATTACK_PARAM_ALL);
}

bool XModuleMyControl::IsCanCancelUpper()
{
	return m_bCancelAction && (m_nCancelParam == CANCEL_NEXT_ATTACK_PARAM_UPPERED);
}

void XModuleMyControl::_UpdateReleaseCancelAction()
{
	const unsigned int CANCEL_ACTION_RELEASE_TIME = 1000;	// 1�� �� Ǯ��
	unsigned int nNowTime = XGetNowTime();
	if (nNowTime - m_nLastCancelActionTime > CANCEL_ACTION_RELEASE_TIME)
	{
		ReleaseCancelAction();
	}
}

void XModuleMyControl::InitStand()
{
	// �ʱ⿡ ���ִ� Animation�� ���� ��������� ������.
	XModuleAction * pModuleAction = m_pOwner->GetModuleAction();
	if(pModuleAction)
	{
		pModuleAction->Stop();
	}
}

void XModuleMyControl::OnWarp( const vec3& vPos, const vec3& vDir )
{
	XModuleActorControl::OnWarp(vPos, vDir);

	// ĳ���� ���� ����, �𵨻Ӹ� �ƴ϶� �ڵ忡�� ���̴� ������� �ٲ�� �Ѵ�.
	m_vCharacterFrontMoveDir = vDir;

	if(gg.controller) 
	{
		gg.controller->SetEnabled(true);
		gg.controller->ClearKeyBuffer();

		InitStand();
		InitAutoMove();
		
		m_pSensorController->Clear();	// �����ϸ� ���� ��� �ִ� �� �ʱ�ȭ

		// ī�޶� ��ġ�� �������� ���� ��ġ��.
		vec3 vCameraDir = gg.controller->GetBackViewCameraDir();
		vCameraDir.x = vDir.x;
		vCameraDir.y = vDir.y;
		vCameraDir.Normalize();
		gg.controller->SetBackViewCameraDir(vCameraDir);
	}
}

void XModuleMyControl::StopRun()
{
	SetAutoRun(false);

	XModuleAction* pModuleAction = GetOwner()->GetModuleAction();
	if ( pModuleAction) 
		pModuleAction->Stop();

	m_nMovementFlags = 0;

	m_pModulePost->PostMoveStop(GetOwner()->GetPosition());
}

void XModuleMyControl::SetPause( bool bPause)
{
	if ( m_bPaused == bPause)
		return;
	
	m_bPaused = bPause;


	if ( bPause == true)
	{
		StopRun();
//		DoAction( ACTION_STATE_IDLE);
	}
	else if ( bPause == false)
	{
	}

	if ( m_pModuleAction)
		m_pModuleAction->SetPause( bPause);
}

bool XModuleMyControl::ChangeWeapon()
{
	if ( m_ChangeWeaponInfo.IsEmpty())
		return false;


	// ���� ����
	m_pModulePost->PostEquipItem(m_ChangeWeaponInfo.m_nSlot, m_ChangeWeaponInfo.m_uidWeaponID);
	m_ChangeWeaponInfo.Clear();


	// ���Ľ� �ٽ� ����
	XEvent evt( XEVTL_CHANGE_STANCE);
	m_pOwner->PostDelayEvent( evt, 500);


	return true;
}

void XModuleMyControl::DoActionLootingItem(MUID& uidNPC, float fLootTime)
{
	gvar.MyInfo.LootingItem.fTime = fLootTime;
	DoAction(ACTION_STATE_LOOTINGITEM, &uidNPC);
}

void XModuleMyControl::OnLootEnd()
{
	XEvent msg(XEVTL_LOOT_END);
	m_pActionFSM->Event(msg);
}

void XModuleMyControl::OnLootItemStart()
{
	if (GetCurrentActionStateID() != ACTION_STATE_LOOTINGITEM) return;

	XMyActionLootingItem* pActionState = static_cast<XMyActionLootingItem*>(m_pActionFSM->GetCurrentState());
	pActionState->LootingStart();
}

void XModuleMyControl::DoActionIdle()
{
	DoAction(ACTION_STATE_IDLE);
}

// TODO : �ϴ� �����ؼ� ����Ϸ��� ���� ������µ� �Ʒ����� �ʿ���������� �Ǵ�
//			�ʿ�������� ����ϴ�.
void XModuleMyControl::DoActionIdleWithSpecialState()
{
	if (GetCurrentActionStateID() == ACTION_STATE_MAPOBJ_INTERACTION	||
		GetCurrentActionStateID() == ACTION_STATE_CHANGE_CHANNEL		||
		GetCurrentActionStateID() == ACTION_STATE_QUEST_INTERACTION		||
		GetCurrentActionStateID() == ACTION_STATE_DEACTIVATIVE			||
		GetCurrentActionStateID() == ACTION_STATE_HOLD)
	{
		DoActionIdle();
	}
}

void XModuleMyControl::DoInnBeginSleep()
{
	if (GetCurrentActionStateID() != ACTION_STATE_MAPOBJ_INTERACTION) return;

	XMyActionMapObj* pActionState = static_cast<XMyActionMapObj*>(m_pActionFSM->GetCurrentState());
	pActionState->BeginSleep();
}

void XModuleMyControl::DoInnEndSleep()
{
	if (GetCurrentActionStateID() != ACTION_STATE_MAPOBJ_INTERACTION) return;

	XMyActionMapObj* pActionState = static_cast<XMyActionMapObj*>(m_pActionFSM->GetCurrentState());
	pActionState->EndSleep();
}

bool XModuleMyControl::IsCurrentGuardEnabled()
{
	// ���� �׽�Ʈ
	if(gvar.Debug.bForceGuard)
		return true;

	// ���� �� �� �ִ� �����ΰ�?
	if (CheckActionEventAcceptable() == false &&
		global.ui->IsMouseCursorToggleKeyDown() == false )
		return false;

	if (GetMyPlayerOwner()->GetStance() != CS_BATTLE) return false;

	// ���尡 ������ ���� Ÿ���ΰ�?
	WEAPON_TYPE nWeaponType = GetMyPlayerOwner()->GetCurrWeaponType();

	if (CSItemHelper::IsGuardableWeaponType(nWeaponType) == false)
	{
		return false;
	}

	// Ű üũ
	bool bKeyPressed = false;

	switch (nWeaponType)
	{
	case WEAPON_1H_SLASH:
	case WEAPON_1H_BLUNT:
	case WEAPON_TWO_HANDED:
	case WEAPON_STAFF:
	case WEAPON_2H_BLUNT:
		{
			bKeyPressed = (gg.controller->IsVirtualKeyPressedAsync(VKEY_GUARD) || 
							(gg.controller->IsVirtualKeyPressedAsync(VKEY_ACTION) && gg.controller->IsVirtualKeyPressedAsync(VKEY_ACTION2)));
		}
		break;
	default:
		_ASSERT(0);
	}

	if (bKeyPressed == false) return false;

	return true;
}

void XModuleMyControl::CheckSwim( bool bIn )
{
	// �������� ��Ŷ�� ������ �ϴ� �������� üũ
	XMyPlayer* pMyPlayer = GetMyPlayerOwner();

	if(bIn)
	{
		// ���󿡼� �������� ù ���Խ�...
		if(pMyPlayer->IsSwim() == false)
		{
			XPostSwim(true);
		}
	}
	else
	{
		// ���ӿ��� �������� ù ���Խ�...
		if(pMyPlayer->IsSwim() == true)
		{
			XPostSwim(false);
		}
	}

	XModuleActorControl::CheckSwim(bIn);
}

void XModuleMyControl::OnTalentInstantEffectGain( XTalentInfo* pTalentInfo)
{
	int nMyHP = gvar.MyInfo.GetHP();

	gvar.MyInfo.Talent.GainInstantEffect(pTalentInfo);

	int nGainHp = gvar.MyInfo.GetHP() - nMyHP;
	XModuleEffect* pModuleEffect = m_pOwner->GetModuleEffect();
	if(pModuleEffect && nGainHp > 0)
	{
		XCaptionEffectEventData* pEventData = new XCaptionEffectEventData;
		pEventData->m_nParam1 = nGainHp;
		pModuleEffect->OnEffectEvent(XEFTEVT_EFFECT_CAPTION_HEAL, pEventData);
	}

	// ����Ʈ ȿ��...
	XModuleActorControl::OnTalentInstantEffectGain(pTalentInfo);
}

void XModuleMyControl::DoActionChangeChannel()
{
	DoAction( ACTION_STATE_CHANGE_CHANNEL);
}

void XModuleMyControl::FallingDamage()
{
	float fFallingHeight = m_pModuleMovable->GetMovableFactor().GENERAL_STATE.m_fFallingStartHeight - m_pModuleMovable->GetMovableFactor().GENERAL_STATE.m_fFallingEndHeight;

	if (fFallingHeight < 0.f)	return;
	if (fFallingHeight < 600.f) return;

	bool bColWater = false;

	XModuleCollision* pModuleCollision = m_pOwner->GetModuleCollision();
	if (pModuleCollision != NULL) 
	{
		vec3 vPos = m_pOwner->GetPosition();
		vec3 vOut;
		bColWater = pModuleCollision->PickTerrainWater(vPos, vOut);
	}

	//mlog("FallingDamage(%d, %f) \n", bColWater, fFallingHeight);

	XPostFallingEnd(bColWater, fFallingHeight);

	m_pModuleMovable->InitMovableFactorFalling();
}

void XModuleMyControl::DoActionQuestInteraction()
{
	DoAction( ACTION_STATE_QUEST_INTERACTION);
}

void XModuleMyControl::UpdateStateFalling()
{
	if(m_pModuleMovable->GetMovableFactor().GENERAL_STATE.m_bFallingStartRecording)
	{
		// Falling ���۽�...
		if(m_bFallingStart == false)
		{
			// Falling �ִϸ��̼�
			if(GetCurrentActionStateID() == ACTION_STATE_IDLE)
			{
				DoAction(ACTION_STATE_FALLING, IsAutoRun());
			}

			m_bFallingStart = true;
		}
	}
	else if(m_bFallingStart)
	{
		FallingDamage();
		m_bFallingStart = false;
	}
}


void XModuleMyControl::DoActionDeactivate( const int nBuffID, const wchar_t* szUseAni)
{
	stDeactiveData deactiveData;
	deactiveData.nBuffID = nBuffID;
	deactiveData.strBuffAnimation = szUseAni;
	DoAction( ACTION_STATE_DEACTIVATIVE, &deactiveData);
}


bool XModuleMyControl::IsCurrentGuard()
{
	if(m_pActionFSM->GetCurrentStateID() == ACTION_STATE_GUARD)
		return true;
	
	return false;
}

void XModuleMyControl::OnGuardKnockBack(vec3& vDir)
{
	// ���� ���� �׼� ���¶��... �˹��̶�� ������ �˷��� ���带 Ǭ��.
	XEvent msg(XEVTL_GUARD_KNOCKBACK);
	m_pActionFSM->Event(msg);

	XModuleActorControl::OnGuardKnockBack(vDir);
}

bool XModuleMyControl::IsPostPosSync()
{
	XMyActionState* pCurrentActionState = GetCurrentActionState();

	// ��ũ ���������� ������ �ϴ� �͵�
	if((pCurrentActionState &&
		((pCurrentActionState->GetID() == ACTION_STATE_GUARD &&	IsCurrentMotionGuardKnockback() == true) ||		// ���� �˹�� ��ǿ����� ��ũ ���������� ������.
		pCurrentActionState->GetID() == ACTION_STATE_CHANGE_STANCE ||											// ���� ü������... 
		pCurrentActionState->GetID() == ACTION_STATE_JUMP)) ||
		m_pModuleMovable->GetMovableFactor().IsMoveDummyNode())													// ���� ��忡 ���� �������̸�...
	{	
		return true;
	}

	return false;
}

void XModuleMyControl::DoGrappledEnd()
{
	if (GetCurrentActionStateID() != ACTION_STATE_GRAPPLED) return;

	XMyActionGrappled* pActionState = static_cast<XMyActionGrappled*>(m_pActionFSM->GetCurrentState());
	pActionState->FinishGrappled();
}

void XModuleMyControl::OnHitByBuff( TD_BUFF_HIT* pBuffHitInfo )
{
	// �ŷ�Ʈ ���
	if (m_pModuleTalent->IsActive())
	{
		if ( m_pModuleTalent->GetInfo()->m_bCancelable && 
			XTalentHelper::IsTalentCancelableMotionFactor((MF_STATE)pBuffHitInfo->nMFState) )
		{
			// ������̴� Ż��Ʈ �������
			m_pModuleTalent->Cancel();
		}
	}

	// ��� ����
	XTalentHitParam * pHitInfo			= new XTalentHitParam();

	pHitInfo->nAttackerUIID				= UIID_INVALID;
	pHitInfo->vServerKnockbackDir		= vec3::ZERO;
	pHitInfo->vServerKnockbackVictimPos = vec3::ZERO;
	pHitInfo->nTalentID			= 0;
	pHitInfo->nMFState					= MF_STATE(pBuffHitInfo->nMFState);
	pHitInfo->nMFWeight					= pBuffHitInfo->nMFWeight;

	HitByTalent(pHitInfo);

	SAFE_DELETE(pHitInfo);
}

bool XModuleMyControl::StartGrappled( _DamageRepeatInfo* pInfo )
{
	if ( GetCurrentActionStateID() == ACTION_STATE_GRAPPLED)
	{
		XEvent evt( XEVTL_GRAPPLED_NEW_START, (void*)( pInfo));
		m_pActionFSM->Event( evt);
	}
	else
	{
		DoAction(ACTION_STATE_GRAPPLED, (void*)( pInfo));

		// Ȯ��
		if ( GetCurrentActionStateID() != ACTION_STATE_GRAPPLED)
		{
			return false;
		}
	}

	m_bDamageRepeat = true;

	return true;
}

bool XModuleMyControl::StartSwallowed( _DamageRepeatInfo* pInfo )
{
	if ( GetCurrentActionStateID() == ACTION_STATE_SWALLOWED)
	{
		XEvent evt( XEVTL_SWALLOWED_NEW_START, (void*)( pInfo));
		m_pActionFSM->Event( evt);
	}
	else
	{
		DoAction(ACTION_STATE_SWALLOWED, (void*)( pInfo));

		// Ȯ��
		if ( GetCurrentActionStateID() != ACTION_STATE_SWALLOWED)
		{
			return false;
		}
	}

	m_bDamageRepeat = true;

	return true;
}

void XModuleMyControl::OnGuarded()
{
	// ���� ���� �׼� ���¶��... ���忡���� ������ �˷��� ó������
	XEvent msg(XEVTL_GUARDED);
	m_pActionFSM->Event(msg);
}

void XModuleMyControl::UseRefillPotion()
{
	XItem* pItem = gvar.MyInfo.Inventory.GetItemByID(XCONST::REFILL_POTION_HP_ITEMID);
	if( pItem == NULL)
	{
		pItem = gvar.MyInfo.Inventory.GetItemByID(XCONST::REFILL_POTION_SP_ITEMID);
		if( pItem == NULL)
		{
			pItem = gvar.MyInfo.Inventory.GetItemByID(XCONST::REFILL_POTION_MP_ITEMID);
			if( pItem == NULL)	return;
		}
	}

	gvar.Game.pMyPlayer->GetModuleMyControl()->DoActionUseItem(pItem->m_nSlotID);
}

void XModuleMyControl::InitFallingStartZ()
{
	m_pModuleMovable->InitMovableFactorFalling();
}

void XModuleMyControl::_UpdateMyCharacterAlpha()
{
	if (global.camera == NULL || global.camera->GetCamera() == NULL) return;
	if (gg.controller->IsMainBackViewCamera())
	{
		vec3 vCameraPos = global.camera->GetCamera()->GetPosition();
		vec3 vCharacterPos = m_pModuleEntity->GetPosition();

		vCharacterPos.z += XCONST::CAMERA_TARGET_HEIGHT;

		const float fCameraDist = vCameraPos.DistanceTo(vCharacterPos);

		m_CharacterAlphaMgr.Check(fCameraDist, m_pModuleEntity);
	}
	//else if(global.camera->IsPrimaryCameraType(CAMERA_INTERACTION))
	//{
	//	// ���ͷ����� npc�� ī�޶� ���̿� ������ ����ó��
	//	XObject* pNPC = gg.omgr->Find(XGetInteractionInfo().InteractionTargetUID);
	//	if(pNPC == NULL)
	//		return;

	//	//vec3 vCameraPos = global.camera->GetCamera()->GetPosition();
	//	vec3 vNpcPos = pNPC->GetPosition();
	//	vNpcPos.z += XCONST::CAMERA_TARGET_HEIGHT;
	//	vec3 vCharacterPos = m_pModuleEntity->GetPosition();
	//	vCharacterPos.z += XCONST::CAMERA_TARGET_HEIGHT;

	//	//float fCameraDistToNPC = vCameraPos.DistanceTo(vNpcPos);
	//	//float fCameraDistToPlayer = vCameraPos.DistanceTo(vCameraPos);
	//	//if(fCameraDistToNPC > fCameraDistToPlayer)
	//	//	fCameraDistToPlayer = m_CharacterAlphaMgr.CONST_ALPHA_VALUE() - 10.0f;
	//	float fPlayerDistToNPC = vCharacterPos.DistanceTo(vNpcPos);
	//	if(60.0f > fPlayerDistToNPC)
	//		fPlayerDistToNPC = m_CharacterAlphaMgr.CONST_ALPHA_VALUE() - 10.0f;
	//		
	//	m_CharacterAlphaMgr.Check(fPlayerDistToNPC, m_pModuleEntity);
	//}
}

void XModuleMyControl::DoActionGesture( const wchar_t* szMotionName )
{
	DoAction(ACTION_STATE_GESTURE, (void*)szMotionName);
}

void XModuleMyControl::OnGuardFailed()
{
	XEvent msg(XEVTL_GUARD_FAILED);
	m_pActionFSM->Event(msg);
}

void XModuleMyControl::SetMouseMove( vec3& vMovePos )
{
	if(GetMyPlayerOwner()->IsDead())
		return;

	if(m_bMouseAutoMove == false)
	{
		// �ϴ� ����
		Stop();
	}

	// �׼� ���� üũ
	const int nActionFSMStateID = GetCurrentActionStateID();
	if(nActionFSMStateID == ACTION_STATE_HOLD)
		return;

	// �̵� ����
	vec3 vMoveDir = (vMovePos - m_pOwner->GetPosition());
	float fMoveLen = vMoveDir.Length();

	m_vCharacterFrontMoveDir = vMoveDir.Normalize();

	// �̵� �÷���(����)
	m_nMovementFlags = MOVEMENTFLAG_FORWARD;

	// �̵� �ð�
	float fMoveSpeed = GetMyPlayerOwner()->GetMoveSpeed( m_nMovementFlags);
	float fMoveTime = fMoveLen / fMoveSpeed + 3.0f;

	// �̵� ������
	XMovableGoParam* pGoParam = new XMovableGoParam(XMovableGoParam::E_GO_MOUSE_MOVE);
	XMovableFactor::SInterpolation param = m_pModuleMovable->GetMovableFactor().INTERPOLATION;
	pGoParam->SetMoveCheckType(MCT2_MOUSE_MOVE);
	pGoParam->Set(vMovePos, m_vCharacterFrontMoveDir, fMoveSpeed, 200.0f, BOOST_NONE, m_nMovementFlags & 0x000F, true, fMoveTime);

	m_pModuleAction->SetMyMovementActionByMouseMove(pGoParam);

	m_bMouseAutoMove = true;
	m_vMouseAutoMove = vMovePos;

	delete pGoParam;
}

void XModuleMyControl::MouseMoveClick( MPoint& pt )
{
	// �׼� ���� �˻�
	const int nActionFSMStateID = GetCurrentActionStateID();
	if (nActionFSMStateID != ACTION_STATE_IDLE ||
		CheckMouseMovable() == false) 
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	vec3 vMovePos = vec3::ZERO;
	if(m_pMouseMove->OnClick(pt.x, pt.y, vMovePos) && vMovePos != vec3::ZERO)
	{
		m_bAutoRun = false;
		SetMouseMove(vMovePos);
	}
}

void XModuleMyControl::ChangeIndicatorEffectPos()
{
	m_pTargetIndicator->ChangeIndicatorEffectPos();  
}

void XModuleMyControl::SetTargetIndicatorToggle( bool bShow )
{
	m_pTargetIndicator->SetShow(bShow);
}

bool XModuleMyControl::IsHelpMessage()
{
	unsigned long ltime = timeGetTime();
	if (XGetInteractionInfo().stTargetActorInfo.nLockOnTime + TARGET_INFO_REFRESH_TIME > ltime)
	{
		return true;
	}

	return false;
}

bool XModuleMyControl::IsCurrentGrappled()
{
	if(m_pActionFSM->GetCurrentStateID() == ACTION_STATE_GRAPPLED ||
		m_pActionFSM->GetCurrentStateID() == ACTION_STATE_SWALLOWED)
		return true;

	return false;
}

void XModuleMyControl::CheckMouseMoveState()
{
	m_nMovementFlags = MOVEMENTFLAG_FORWARD;

	const XMovableFactor& movableFactor = m_pModuleMovable->GetMovableFactor();

	// ���� �ߴ°�? �ƴ� �浹�ΰ�?
	if(movableFactor.INTERPOLATION.m_bArrived == true ||
		movableFactor.MOUSE_MOVE.m_bColEnd == true)
	{
		InitMouseMove();
		m_nMovementFlags = 0;
	}
	else if (m_pModuleAction->GetCurrentMotionName() == MOTION_NAME_IDLE &&
		m_vMouseAutoMove != vec3::ZERO)
	{
		// �߰��� �����Ϸ� Idle�� �Ǿ��ٸ�...
		SetMouseMove(m_vMouseAutoMove);
	}
	else
	{
		// �ӵ� üũ
		float fMoveSpeed = GetMyPlayerOwner()->GetMoveSpeed( m_nMovementFlags);

		if(fMoveSpeed != movableFactor.INTERPOLATION.m_fMoveSpeed)
		{
			SetMouseMove(m_vMouseAutoMove);
		}
	}
}

void XModuleMyControl::OnUseTalent( int nTalentID, vec3& vPos, vec3& vDir )
{
	XTalentInfo* pTalentInfo = XTalentHelper::FindTalentInfo( nTalentID, m_pOwner);
	if ( pTalentInfo && pTalentInfo->m_nMovableType == MUT_NONE)
	{
		InitMouseMove();
		m_bAutoRun = false;
	}

	XModuleActorControl::OnUseTalent(nTalentID, vPos, vDir);
}

bool XModuleMyControl::CheckActionEventAcceptableByMouseVisible( XEvent* pEvent /*= NULL*/ )
{
	// ���콺 �����Ͱ� Ȱ��ȭ �Ǿ� �ִµ�... ��� ������ �׼� �̺�Ʈ
	// ����(VKEY_FORWARD, VKEY_BACKWARD, VKEY_LEFT, VKEY_RIGHT, VKEY_ACTION, VKEY_JUMP, VKEY_ACTION2) �� �׼���... �˾Ƽ� ����Ѵ�.

	if (pEvent == NULL ||
		pEvent->m_pData == NULL)
		return false;

	XVirtualKey nVirtualKey = *static_cast<XVirtualKey*>(pEvent->m_pData);
	if(nVirtualKey == VKEY_CHANGE_WEAPON ||
		nVirtualKey == VKEY_SIT ||
		nVirtualKey == VKEY_CHANGE_STANCE ||
		nVirtualKey == VKEY_NPCINTERACTION	||
		nVirtualKey == VKEY_PCINTERACTION ||
		nVirtualKey == VKEY_FORWARD ||
		nVirtualKey == VKEY_BACKWARD ||
		nVirtualKey == VKEY_LEFT ||
		nVirtualKey == VKEY_RIGHT)
		return true;

	return false;
}

void XModuleMyControl::InitAutoMove()
{
	InitMouseMove();
	m_bAutoRun = false;
}

bool XModuleMyControl::IsMoving()
{
	return ( m_nMovementFlags == MT_DEFAULT  ?  false : true);
}

bool XModuleMyControl::IsJumping()
{
	return ( MOVEMENTFLAG_JUMPING & m_nMovementFlags ? true : false );
}

void XModuleMyControl::Init_ForChangeField()
{
	InitStand();
	InitAutoMove();

	m_bCurrentFreeLook = false;
	m_bFallingStart = false;

	m_pSensorController->Clear();

	m_pModuleBuff->Init_ForChangeField();

	DoForceCameraLock();
}

void XModuleMyControl::Final_ForChangeField()
{
	m_pSensorController->Clear();

	SetPause( true);
	DoGrappledEnd();
	InitFallingStartZ();
}

void XModuleMyControl::RequestCancel_PreInteraction()
{
	if (gvar.Game.bTradeReq == true)
	{
		XPostTrade_StartAccept(false);
		gvar.Game.bTradeReq = false;
		global.script->GetGlueGameEvent().OnGameEvent( "TRADE", "CANCELED");
	}
	else if (gvar.Game.bPartyReq == true)
	{
		XPostPartyRequestInviteQuestionRespond(false);
		gvar.Game.bPartyReq = false;
		global.script->GetGlueGameEvent().OnGameEvent( "PARTY", "CANCELED");
	}
	else if (gvar.Game.bGuildReq == true)
	{
		XPostGuild_Invite_Res(false);
		gvar.Game.bGuildReq = false;
		global.script->GetGlueGameEvent().OnGameEvent( "GUILD", "CANCELED");
	}
	else if (gvar.Game.bDuelReq == true)
	{
		XPostDuel_QuestionRespond( false);
		gvar.Game.bDuelReq = false;
		global.script->GetGlueGameEvent().OnGameEvent( "DUEL", "CANCELED");
	}
}

//void XModuleMyControl::_UpdateLightEffectPos(vec3 vCameraDir)
//{
//	if(m_uidCharacterLightEffect == MUID::ZERO)
//		return;
//
//	if(vCameraDir == vec3::ZERO)
//		return;
//
//	XLightEffect* pCharacterLightEffect = global.emgr->GetXLightEffect(m_uidCharacterLightEffect);
//
//	if(pCharacterLightEffect == NULL)
//	{
//		if(CreateCharacterLightEffect() == false)
//			return;
//
//		pCharacterLightEffect = global.emgr->GetXLightEffect(m_uidCharacterLightEffect);
//	}
//
//	vec3 vLightEffectPos = m_pOwner->GetPosition();
//	vLightEffectPos.z += XCONST::CAMERA_TARGET_HEIGHT;
//
//	vLightEffectPos += -vCameraDir * XCONST::CONST_CAMERA_LIGHT_EFFECT_DISTANCE;
//
//	MMatrix matWorld;
//	matWorld.SetLocalMatrix(vLightEffectPos, vec3::AXISY, vec3::AXISZ);
//
//	pCharacterLightEffect->SetWorldMatrix(matWorld);
//	pCharacterLightEffect->SetRadius(XCONST::CONST_CAMERA_LIGHT_EFFECT_RADIUS);
//	pCharacterLightEffect->SetColorRGB(XCONST::CONST_CAMERA_LIGHT_EFFECT_COLOR);
//}

//bool XModuleMyControl::CreateCharacterLightEffect()
//{
//	if(XCONST::CONST_CAMERA_LIGHT_EFFECT_ENABLE == false)
//		return false;
//
//	m_uidCharacterLightEffect = global.emgr->AddXLightEffect(NULL, wstring(L""), XCONST::CONST_CAMERA_LIGHT_EFFECT_RADIUS, XCONST::CONST_CAMERA_LIGHT_EFFECT_COLOR, 0.0f, false);
//
//	return (m_uidCharacterLightEffect != MUID::ZERO);
//}

void XModuleMyControl::_UpdateRestrictedArea( float fDelta )
{
	if(gg.currentgamestate->GetWorld() && gg.currentgamestate->GetWorld()->GetInfo())
	{
		CSFieldFileInfo* pFieldFileInfo = info.field->GetFieldList().Find(gg.currentgamestate->GetWorld()->GetInfo()->m_nFieldID);
		m_RestrictedAreaChecker._UpdateRestrictedArea(fDelta, m_pModuleEntity->GetPosition(), gg.currentgamestate->GetWorld()->GetInfo(), pFieldFileInfo);
	}
}

void XModuleMyControl::OnHeal( int nAmount, UIID nUserUIID /*= UIID_INVALID*/ )
{
	gvar.MyInfo.SetHP(gvar.MyInfo.GetHP() + nAmount);

	XModuleActorControl::OnHeal(nAmount, nUserUIID);
}

void XModuleMyControl::InitSensorController(XGameState* pState)
{
	if (pState && pState->GetSensorManager())
	{
		m_pSensorController = pState->GetSensorManager()->GetController();
	}
	else
	{
		m_pSensorController = NULL;
	}
}

void XModuleMyControl::StartChallengerQuestExitTimer()
{
	if (m_pChallengerQuestExitChecker == NULL)
		m_pChallengerQuestExitChecker = new XChallengerQuestExitChecker;

	m_pChallengerQuestExitChecker->Start();
}

void XModuleMyControl::StopChallengerQuestExitTimer()
{
	if (m_pChallengerQuestExitChecker)
	{
		m_pChallengerQuestExitChecker->Stop();
		SAFE_DELETE(m_pChallengerQuestExitChecker);
	}
}

void XModuleMyControl::DoActionInteraction( MUID uidTarge )
{
	DoAction(ACTION_STATE_INTERACTION, &uidTarge);
}

bool XModuleMyControl::CheckAttackReturnAnimation()
{
	wstring strCurMotion = m_pOwner->GetModuleMotion()->GetCurrMotion();
	wstring strCurUpperMotion = m_pOwner->GetModuleMotion()->GetCurrUpperMotion();

	if(strCurMotion == MOTION_NAME_ATTACK_1_RETURN ||
		strCurMotion == MOTION_NAME_ATTACK_2_RETURN ||
		strCurMotion == MOTION_NAME_ATTACK_3_RETURN ||
		strCurMotion == MOTION_NAME_ATTACK_4_RETURN ||
		strCurMotion == MOTION_NAME_ATTACK_5_RETURN ||
		strCurMotion == MOTION_NAME_ATTACK_1F_RETURN ||
		strCurMotion == MOTION_NAME_ATTACK_SWORDDRAWING_RETURN ||
		strCurUpperMotion == MOTION_NAME_ATTACK_1_RETURN ||
		strCurUpperMotion == MOTION_NAME_ATTACK_2_RETURN ||
		strCurUpperMotion == MOTION_NAME_ATTACK_3_RETURN ||
		strCurUpperMotion == MOTION_NAME_ATTACK_4_RETURN ||
		strCurUpperMotion == MOTION_NAME_ATTACK_5_RETURN ||
		strCurUpperMotion == MOTION_NAME_ATTACK_1F_RETURN ||
		strCurUpperMotion == MOTION_NAME_ATTACK_SWORDDRAWING_RETURN)
	{
		return true;
	}

	return false;
}

void XModuleMyControl::HitByBuff( XTalentHitParam * pHitInfo )
{
	if(pHitInfo)
		HitByTalent(pHitInfo);
}

bool XModuleMyControl::DoActionEnchantUseItem( int nSlotID )
{
	return DoAction(ACTION_STATE_ENCHANT, &nSlotID);
}

void XModuleMyControl::DoActionMapObjInteraction(MUID uidSearch)
{
	DoAction(ACTION_STATE_MAPOBJ_INTERACTION, (void*)&uidSearch);
}

void XModuleMyControl::DoCameraLock( MUID& uidTarget, wstring& strBoneName, float& fDurationTime )
{
	if ( global.camera->IsPrimaryCameraType( CAMERA_MAIN_BACKVIEW))
	{
		XCameraBackViewController* pBackViewCamera = global.camera->GetCameraController<XCameraBackViewController>();
		if (pBackViewCamera)
		{
			pBackViewCamera->StartCameraLock(uidTarget, strBoneName, fDurationTime);
		}
	}
}

void XModuleMyControl::DoForceCameraLock()
{
	XCameraBackViewController* pBackViewCamera = global.camera->GetCameraController<XCameraBackViewController>();
	if (global.camera->IsPrimaryCameraType( CAMERA_MAIN_BACKVIEW) && pBackViewCamera)
	{
		pBackViewCamera->EndCameraLock();
	}
}

bool XModuleMyControl::CheckMouseMovable()
{
	// �����ϼ� ���� �������� üũ
	XBuffAttribute attrBuff = m_pModuleBuff->GetBuffAttribute();
	if ( attrBuff.IsMovable() == false)
		return false;

	return true;
}

void XModuleMyControl::DoActionSitDown()
{
	DoAction(ACTION_STATE_SIT, true);
}

void XModuleMyControl::DoActionSitRise()
{
	XEvent msg(XEVTL_SIT_RISE);
	m_pOwner->Event(msg);
}

void XModuleMyControl::SetAFK( bool bAFK )
{
	XMyPlayer* pMyPlayer = GetMyPlayerOwner();
	if (pMyPlayer->IsDead()) return;

	pMyPlayer->SetAFK(bAFK);

	if (bAFK)
	{
		DoActionSitDown();

		pMyPlayer->SetCharacterPane_Player();
		global.ui->OnSystemMsg(XGetStr(L"SMSG_AWAY_FROM_KEYBOARD_BEGIN"));
	}
	else
	{
		pMyPlayer->SetCharacterPane_Player();
		global.ui->OnSystemMsg(XGetStr(L"SMSG_AWAY_FROM_KEYBOARD_END"));
	}
}

bool XModuleMyControl::CheckGuardKnockback()
{
	// ���� ���� �˹� �����ΰ�?
	if(GetCurrentActionStateID() != ACTION_STATE_GUARD)
		return false;

	XMyActionGuard* pActionState = static_cast<XMyActionGuard*>(m_pActionFSM->GetCurrentState());
	return pActionState->IsGuardKnockback();
}

bool XModuleMyControl::IsSendablePositionPost()
{
	// �ǰ� ��� ������ ������ ��ġ ������ ������ �Ǵ��� ���ϴ�.
	// ������Ͱ� ������ ������ ���ÿ�.
	vector<XEvent> vecEvent;
	m_pOwner->GetModuleMessageQueue()->GetEventQueueNode(XEVTD_TALENT_HIT, vecEvent);

	for(vector<XEvent>::iterator it = vecEvent.begin(); it != vecEvent.end(); ++it)
	{
		XTalentHitParam* pHitParam = static_cast<XTalentHitParam*>(it->m_pData);
		if(pHitParam && !CSMotionFactorHelper::IsNoneOrFakes(pHitParam->nMFState))
		{
			return false;
		}
	}

	return true;
}

void XModuleMyControl::ClearLockOnTarget( const UIID& uidObj )
{
	if( false == XGetInteractionInfo().stTargetActorInfo.bLockOnTarget )
		return ;

	if( uidObj != XGetInteractionInfo().GetTargetUIID() )
		return ;

	m_pTargetIndicator->Init();
	XGetInteractionInfo().SetTargetUIID(0);
	XGetInteractionInfo().stTargetActorInfo.bLockOnTarget = false;
	XGetInteractionInfo().stTargetActorInfo.nLockOnTime = timeGetTime();

	if(global.script)
	{
		global.script->GetGlueGameEvent().OnTargetInfoRefresh();
		global.script->GetGlueGameEvent().OnEnemyInfoRefresh();
	}
}