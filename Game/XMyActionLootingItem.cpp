#include "stdafx.h"
#include "XMyActionLootingItem.h"
#include "XModuleMyControl.h"
#include "XModuleCollision.h"
#include "XModuleAction.h"
#include "XModuleBuff.h"
#include "XMyPlayer.h"
#include "XNonPlayer.h"
#include "XEventID.h"
#include "XPost_Loot.h"
#include "XInteractionInfo.h"

#include "RSceneManager.h"
#include "RMeshMgr.h"
#include "XCharacter.h"
#include "RMeshNode.h"

XMyActionLootingItem::XMyActionLootingItem(XModuleMyControl* pOwner)
: XMyActionState(pOwner) 
, m_pGatherNode(NULL)
{

}

bool XMyActionLootingItem::CheckEnterable(void* pParam)
{
	// ������ ���� ���� Ȯ��
	XBuffAttribute attrBuff = m_pOwner->GetMyPlayerOwner()->GetModuleBuff()->GetBuffAttribute();
	if ( attrBuff.IsMesmerizable())
		return false;

	MUID targetUID = (pParam) ? *(MUID*)pParam : 0;
	if (targetUID == MUID::ZERO) return false;

	Init();

	StopMove();

	m_TargetNPCUID = targetUID;
	XGetInteractionInfo().InteractionTargetUID = m_TargetNPCUID;

	return true;
}

void XMyActionLootingItem::StopMove()
{
	XModuleAction* pModuleAction = m_pOwner->GetOwner()->GetModuleAction();
	if (!pModuleAction) return;

	pModuleAction->StopMove();
}

void XMyActionLootingItem::Init()
{
	m_nLootingItemState		= LOOTINGITEM_NONE;
	m_TargetNPCUID			= MUID::ZERO;
	m_bLootCancel			= false;
	m_bSendLootEnd			= false;
	m_bGather				= false;
}

void XMyActionLootingItem::Enter(void* pParam)
{
	XMyActionState::Enter(pParam);	//�α׿�.

	m_pOwner->InitAutoMove();
	LootingReady();
}

void XMyActionLootingItem::Exit()
{
	// ���� ���� ó���� �ȵŰ� ���ᰡ �Ǵ� �Ŷ��... UI â�� �ݴ´�.
	if(m_nLootingItemState != LOOTINGITEM_END)
	{
		// ���� �޼����� ������ �ʾҴٸ�...
		if(m_bSendLootEnd == false)
		{
			m_bLootCancel	= true;
			m_bSendLootEnd	= true;
			XPostLoot_End();
		}

		if(global.script)
		{
			global.script->GetGlueGameEvent().OnLootingEnd();
		}
	}

	if(global.script)
	{
		global.script->GetGlueGameEvent().OnGameEvent( "ACTIONPROGRESSBAR", "END");
	}

	m_TargetNPCUID = MUID::ZERO;
	XGetInteractionInfo().InteractionTargetUID	= MUID::ZERO;;

	XMyActionState::Exit();
}

XEventResult XMyActionLootingItem::Event(XEvent& msg)
{
	switch(msg.m_nID)
	{
	case XEVTD_TALENT_HIT:
		{
			if (m_bSendLootEnd == false)
			{
				XPostLoot_End();
				m_bSendLootEnd	= true;
			}
		}
		break;
	case XEVTL_LOOT_END:
		{
			m_bSendLootEnd = true;
			LootingEnd();
		}
		break;

	case XEVTL_LOOT_END_READY:
		{
			// �������� ���� �޼����� ������.
			if (m_bSendLootEnd == false)
			{
				XPostLoot_End();
				m_bSendLootEnd = true;
			}
		}
		break;

	case XEVTL_LOOT_GETITEM:
		{
			GetItems(msg);
		}
		break;

	case XEVTL_LOOT_GETALLITEM:
		{
			GetItems(msg);

			size_t _size = gvar.MyInfo.LootingItem.GetSize();
			vector<MUID>* vecDropItemUID = ((vector<MUID>*)msg.m_pData);
			size_t _size2 = vecDropItemUID->size();
			if (_size != _size2)
			{
				XPostLoot_End();
			}
		}
		break;

	case XEVTL_VIRTUAL_KEY_PRESSED:
		{
			if(m_bLootCancel == false)
			{
				// �̵�Ű�� �����ٸ� ���� ����
				XVirtualKey nVirtualKey = *static_cast<XVirtualKey*>(msg.m_pData);
				switch (nVirtualKey)
				{
				case VKEY_FORWARD:
				case VKEY_BACKWARD:
				case VKEY_LEFT:
				case VKEY_RIGHT:
					{
						m_bLootCancel = true;

						if(m_bSendLootEnd == false)
						{
							XPostLoot_End();
							m_bSendLootEnd = true;
						}
					}
					break;
				}
			}
		}
		break;
	}

	return XMyActionState::Event(msg); // return MR_FALSE; 
}

void XMyActionLootingItem::LootingReady()
{
	XNonPlayer* pTargetNPC = FindNpc();
	if(NULL == pTargetNPC ) return;

	XNPCInfo* pNPCInfo = pTargetNPC->GetInfo();
	if (pNPCInfo == NULL) return;

	wstring aniname = GetAnimationName(pNPCInfo);
	if(m_pOwner->GetOwner()->GetModuleAction()->ChangeLootingMotion(aniname.c_str()) == false)
	{
		//���� ����
		m_pOwner->DoActionIdle();
		return;
	}

	SetGatherModel(pNPCInfo);

	m_nLootingItemState = LOOTINGITEM_READY;

	if(pTargetNPC->IsLootable())
	{
		XPostLoot_SeeItem(m_TargetNPCUID);
	}
	else
	{
		if(global.script)
		{
 			if(0.0f < gvar.MyInfo.LootingItem.fTime)
 			{
 				global.script->GetGlueGameEvent().OnGameEvent( "ACTIONPROGRESSBAR", "BEGIN", MAPT_LOOTING_ITEM, gvar.MyInfo.LootingItem.fTime * 1000.0f);
 			}
		}
	}
}

void XMyActionLootingItem::LootingStart()
{
	if (m_bGather)
	{
		SetNextMotion();
	}

	m_nLootingItemState = LOOTINGITEM_START;

	if(global.script)
	{
		global.script->GetGlueGameEvent().OnGameEvent( "ACTIONPROGRESSBAR", "END");
		global.script->GetGlueGameEvent().OnLootingBegin();
	}
}

void XMyActionLootingItem::LootingEnd()
{
	//�� ����
	RemoveGatherModel();

	if(global.script)
	{
		global.script->GetGlueGameEvent().OnLootingEnd();
	}

	// ������ �ִϸ� ������ ���¸� Ǭ �ִϸ� �����ص� �ȴ�.
	if( m_nLootingItemState >= LOOTINGITEM_READY &&
		m_nLootingItemState <= LOOTINGITEM_START &&
		m_bGather == false)
	{
		SetNextMotion();
	}
	else
	{
		XModuleMotion* pModuleMotion = m_pOwner->GetOwner()->GetModuleMotion();
		if (pModuleMotion)
		{
			pModuleMotion->ChangeMotion(L"none_gather_up", MOTION_NAME_IDLE);
		}

		Exit();
	}

	m_nLootingItemState = LOOTINGITEM_END;
	gvar.MyInfo.LootingItem.fTime = 0.f;
	gvar.MyInfo.LootingItem.Clear();
}

void XMyActionLootingItem::SetLootingState( LOOTINGITEM_STATE nState )
{
	m_nLootingItemState = nState;
}

wstring XMyActionLootingItem::GetAnimationName( XNPCInfo* pNPCInfo )
{
	if (pNPCInfo == NULL) return L"";
	if (gvar.MyInfo.LootingItem.fTime == 0.f) return MOTION_NAME_LOOTING_ITEM;	///< ä���ð��� ������ �ٷ� ����

	if (pNPCInfo->nGatherType != GATHER_NONE &&
		pNPCInfo->nGatherType != GATHER_LUMBER)
	{
		m_bGather = true;
	}

	switch (pNPCInfo->nGatherType)
	{
	case GATHER_MINE:		// ä��
		return L"Mining";
	case GATHER_HERB:		// �Ĺ�
		return L"Gathering";
	case GATHER_RELIC:		// �߱�
		return L"Lockpicking";
	case GATHER_LUMBER:		// ���� ä��
		break;
	}

	return	pNPCInfo->strInteractionLootAni;
}

void XMyActionLootingItem::SetNextMotion()
{
	XEvent evt;
	evt.m_nID = XEVTL_ON_MOTION_EVENT;
	evt.m_pData = MOTION_EVENT_NEXT_MOTION;
	m_pOwner->GetOwner()->Event(evt);
}

void XMyActionLootingItem::SetGatherModel( XNPCInfo* pNPCInfo )
{
	if (pNPCInfo == NULL) return;
	if (gvar.MyInfo.LootingItem.fTime == 0.f) return;	///< ä���ð��� ������ �ٷ� ����

	const char* szName;
	const char* szNodeName;
	const char* szParentNodeName;

	switch (pNPCInfo->nGatherType)
	{
	case GATHER_MINE:		// ä��
		{
			szName = "pickaxe.elu";
			szNodeName = NODE_PICKAXE;
			szParentNodeName = DUMMY_PICKAXE;
		}
		break;
	case GATHER_HERB:		// �Ĺ�
		{
			szName = "trowel.elu";
			szNodeName = NODE_SHOVEL;
			szParentNodeName = DUMMY_SHOVEL;
		}
		break;
	default:
		return;
	}

	RMesh* pMesh = REngine::GetSceneManager().CreateResource< RMesh >( szName );
	bool bRet = pMesh->BuildAll();

	if( !bRet )
	{
		REngine::GetSceneManager().ReleaseResource( pMesh );
		return;
	}
	RMeshNode* pMeshNode = pMesh->GetNode(szNodeName);
	if(!pMeshNode)
	{
		REngine::GetSceneManager().ReleaseResource( pMesh );
		return;
	}

	RActorNode* pParentNode = m_pOwner->GetOwner()->GetActor()->GetActorNode(szParentNodeName);
	m_pGatherNode = m_pOwner->GetOwner()->GetActor()->AddMeshNode(pMeshNode,pParentNode);
}

void XMyActionLootingItem::RemoveGatherModel()
{
	XNonPlayer* pTargetNPC = FindNpc();
	if(NULL == pTargetNPC ) return;

	XNPCInfo* pNPCInfo = pTargetNPC->GetInfo();
	if (pNPCInfo == NULL) return;

	switch (pNPCInfo->nGatherType)
	{
	case GATHER_MINE:		// ä��
	case GATHER_HERB:		// �Ĺ�
		if(m_pGatherNode)
		{
			RMesh* pMesh = m_pGatherNode->m_pMeshNode->m_pBaseMesh;

			m_pOwner->GetOwner()->GetActor()->DeleteActorNode(m_pGatherNode->GetNodeName().c_str());
			REngine::GetSceneManager().ReleaseResource(pMesh);

			m_pGatherNode = NULL;
		}
		break;
	}
}

void XMyActionLootingItem::GetItems(XEvent &msg )
{
	if( m_TargetNPCUID == MUID::ZERO ||
		m_TargetNPCUID != XGetInteractionInfo().InteractionTargetUID)
	{
		_ASSERT(0);
	}

	vector<MUID>* vecDropItemUID = ((vector<MUID>*)msg.m_pData);
	if (vecDropItemUID->empty()) return;

	XPostLoot_LootItem(m_TargetNPCUID, *vecDropItemUID);
}

XNonPlayer* XMyActionLootingItem::FindNpc()
{
	XNonPlayer* pTargetNPC = gg.omgr->FindNPC(m_TargetNPCUID);
	if(NULL == pTargetNPC)
	{
		pTargetNPC = (XNonPlayer*)gg.omgr->FindBPartNPC(m_TargetNPCUID);
	}

	return pTargetNPC;
}
