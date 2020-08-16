#include "StdAfx.h"
#include "XInteraction.h"
#include "XInteractionInfo.h"
#include "XSensorController.h"
#include "XGameState.h"
#include "XSensorManager.h"
#include "XPost_Comm.h"
#include "XPost_Interaction.h"
#include "XPost_Sensor.h"
#include "XMyPlayer.h"
#include "XNonPlayer.h"
#include "XGatherMSG.h"
#include "XModuleMyControl.h"
#include "XInteractionHelper.h"
#include "XInteractor.h"
#include "XMouseInteractor.h"

XInteraction::XInteraction(XModuleMyControl* pOwner)
: m_pOwner(pOwner)
{
	m_pInteractor = new XInteractor(m_pOwner->GetOwner());
	m_pMouseInteractor = new XMouseInteractor(pOwner->GetOwner());
}

XInteraction::~XInteraction(void)
{
	SAFE_DELETE(m_pMouseInteractor);
	SAFE_DELETE(m_pInteractor);

	m_pOwner = NULL;
}

MUID XInteraction::GetInteractionSearchUID()
{
	return XGetInteractionInfo().GetInteractionSearchUID();
}

bool XInteraction::Interact()
{
	if (GetInteractionSearchUID().IsValid() )
	{
		XObject* pObj = gg.omgr->Find(GetInteractionSearchUID());
		if( pObj == NULL) return false;

		if( pObj->GetEntityType() == ETID_NPC )
		{
			return InteractWithNPC(pObj);
		}
		else if( pObj->GetEntityType() == ETID_PLAYER )
		{
			return InteractWithPlayer(GetInteractionSearchUID());
		}
	}
	else
	{
		XSensorController* pSensorController = gg.currentgamestate->GetSensorManager()->GetController();
		vector<uint32> vecEnteredSensorID = pSensorController->GetEnteredSensorID();
		if (!vecEnteredSensorID.empty())
		{
			XPostSensorInteract(vecEnteredSensorID);
			return true;
		}
	}

	/// ä�� �Ұ� ���� �޼��� ���
	XGatherMSG msg;
	if (msg.Check(XGetInteractionInfo().GetSearchUIID()))
	{
		return true;
	}

	return false;
}

bool XInteraction::InteractWithPlayer( MUID uidTarget )
{
	//�������� ��뿡�� ���ͷ��� �ɸ��� �ʵ���
	if (CheckVSPlayer() == false) return false;

	XPostInteraction_Interaction(uidTarget, m_pOwner->GetMyPlayerOwner()->GetPosition(), m_pOwner->GetMyPlayerOwner()->GetDirection());

	// ���ͷ����� �Ͼ�� ���� uid �� �ѱ�.
	XGetInteractionInfo().InteractionTargetUID = uidTarget;

	if (global.script)
	{
		global.script->GetGlueGameEvent().OnGameEvent( "INTERACTION_PC", "BEGIN");
	}

	//���ͷ����� �Ͼ�� ������ ����
	m_pOwner->StopRun();
	m_pOwner->DoActionHold();

	return true;
}

bool XInteraction::InteractWithNPC( XObject* pObj )
{
	if (pObj == NULL) return false;

	XNonPlayer* pTargetNPC = AsNPC(pObj);
	if (XInteractionHelper::IsInteractableNPC(pTargetNPC) == true)
	{
		m_pOwner->RequestCancel_PreInteraction();

		if (pTargetNPC->GetIConState() == NIS_ITEM_USABLE)
		{
			// ���������ͷ����̸� ������ ���
			if(pTargetNPC->GetInfo()->nIItemID != 0)
			{
				if(gvar.MyInfo.Inventory.IsHaveItem(pTargetNPC->GetInfo()->nIItemID))
				{
					XItem* pItem = gvar.MyInfo.Inventory.GetItemByID(pTargetNPC->GetInfo()->nIItemID);
					if (pItem == NULL) return false;

					XModuleMyControl* pModuleMyControl = gvar.Game.pMyPlayer->GetModuleMyControl();
					if ( pModuleMyControl == NULL)		return false;

					pModuleMyControl->DoActionUseItem( pItem->m_nSlotID);
				}
			}
		}
		else
		{
			XPostInteraction_Interaction(GetInteractionSearchUID(), m_pOwner->GetMyPlayerOwner()->GetPosition(), m_pOwner->GetMyPlayerOwner()->GetDirection());
		}

		return true;
	}
	else
	{
		if (pTargetNPC->IsLootable())
		{
			Loot(pTargetNPC);
			return true;
		}
	}

	XGetInteractionInfo().SetInteractionSearchUID(MUID::ZERO);
	return false;
}

bool XInteraction::CheckVSPlayer()
{
	MUID uidTarget = GetInteractionSearchUID();

	if (uidTarget == MUID::ZERO) return true;

	//�������� ��뿡�� ���ͷ��� �ɸ��� �ʵ���
	if (uidTarget == XGetInteractionInfo().VSPlayerUID) return false;

	XMyPlayer* pMyPlayer = gvar.Game.pMyPlayer;
	if (pMyPlayer == NULL) return false;

	XPlayer* pPlayer = gg.omgr->FindPlayer(uidTarget);
	if (pPlayer)
	{
		if(pPlayer->IsAlly(pMyPlayer) == false) return false;
	}

	return true;
}

void XInteraction::Loot( XNonPlayer* pTargetNPC )
{
	if(pTargetNPC && pTargetNPC->IsLootable())
	{
		m_pOwner->StopRun();
		XPostLoot_Begin(pTargetNPC->GetUID());
	}
}

bool XInteraction::Interact_WithClickingMessageBox()
{
	bool bClickPopupMessageBoxResult = false;
	// ���࿡ �˾� �޼����� ������ �˾� �޼��� Ŭ��
	if (global.script)
	{
		bClickPopupMessageBoxResult = global.script->GetGlueGameEvent().OnGameEvent( "INTERACTION_NPC", "PREINTERACT");
	}

	if (bClickPopupMessageBoxResult)
	{
		return true;
	}

	bool ret = Interact();
	if (ret) return true;

	return false;
}

bool XInteraction::PCInteract()
{
	XObject* pTarObj = NULL;

	if (GetInteractionSearchUID().IsValid() )
	{
		pTarObj = gg.omgr->Find(GetInteractionSearchUID());
		if( pTarObj && pTarObj->GetEntityType() != ETID_PLAYER )
		{
			pTarObj = NULL;
		}
	}

	if (!pTarObj)
	{
		pTarObj = m_pInteractor->FindInteractionPC();
	}

	if (pTarObj)
	{
		if( pTarObj->GetEntityType() == ETID_PLAYER )
		{
			return InteractWithPlayer(pTarObj->GetUID());
		}
	}

	return true;
}

void XInteraction::Update( float fDelta )
{
	m_pInteractor->Update(fDelta);
	m_pMouseInteractor->Update();
}

XEventResult XInteraction::Event( XEvent& msg )
{
	switch (msg.m_nID)
	{
	case XEVTL_VIRTUAL_KEY_PRESSED:
		{
			bool bRet = false;
			XVirtualKey nVirtualKey = *static_cast<XVirtualKey*>(msg.m_pData);

			switch (nVirtualKey)
			{
			//case VKEY_ACTION:
			//	{
			//		if (m_pOwner->GetMyPlayerOwner()->IsCurrStanceBattle() == false)
			//		{
			//			bRet = Interact();
			//		}
			//	}
			//	break;
			case VKEY_NPCINTERACTION:		// eŰ
				{
					bRet = Interact_WithClickingMessageBox();
				}
				break;
			case VKEY_PCINTERACTION:
				{
					bRet = PCInteract();
				}
				break;
			};

			return (bRet) ? MR_TRUE : MR_FALSE;
		}
		break;
	case XEVTL_MOUSE_MOVE_CLICK:
		{
			MPoint ptCursorPos = *(static_cast<MPoint*>(msg.m_pData));
			if(m_pMouseInteractor->OnClick(ptCursorPos.x, ptCursorPos.y))
			{
				bool bRet = Interact_WithClickingMessageBox();
				return (bRet) ? MR_TRUE : MR_FALSE;
			}
		}
		break;
	}

	return MR_FALSE;
}
