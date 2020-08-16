#include "StdAfx.h"
#include "XDummyBot_Dye.h"
#include "XBirdDummyAgent.h"



XDummyBot_Dye::XDummyBot_Dye(XBirdDummyAgent* pAgent, XDummyAgentInfo* pAgentInfo)
: XDummyBot(pAgent, pAgentInfo), m_nPhase(PHASE_INIT), m_nDyeItemID(0), m_nEquipItemID(0), m_bDyeToInvenItem(false)
{
	InitTestItemID();
}

XDummyBot_Dye::~XDummyBot_Dye(void)
{
}

void XDummyBot_Dye::OnRun( float fDelta )
{
	switch (m_nPhase)
	{
	case PHASE_INIT:
		{			
			m_nDyeItemID = XMath::RandomPickUp(m_vecDyeItemID);
			m_nEquipItemID = XMath::RandomPickUp(m_vecEquipItemID);
			m_bDyeToInvenItem = XMath::RandomBool();

			XBIRDPOSTCMD2(m_pAgent->GetClient(), MC_GM_ITEM_GIVE_REQ, MCommandParameterInt(m_nDyeItemID), MCommandParameterInt(1));
			XBIRDPOSTCMD2(m_pAgent->GetClient(), MC_GM_ITEM_GIVE_REQ, MCommandParameterInt(m_nEquipItemID), MCommandParameterInt(1));	

			ChangePhase(PHASE_INIT_DONE);
		}
		break;
	case PHASE_ADDITEM:
		{
			if (m_bDyeToInvenItem)
			{
				ChangePhase(PHASE_DYE_PREPARE_BEGIN);				
			}
			else
			{
				XItem* pEquipItem = m_Inventory.GetItemByID(m_nEquipItemID);
				XBIRDPOSTCMD3(m_pAgent->GetClient(), MC_ITEM_EQUIP_REQ, MCommandParameterInt(SLOTTYPE_INVENTORY), MCommandParameterInt(pEquipItem->m_nSlotID), MCommandParameterChar(static_cast<char>(pEquipItem->m_pItemData->m_nItemSlot)));
				ChangePhase(PHASE_EQUIPITEM);
			}
		}
		break;
	case PHASE_DYE_PREPARE_BEGIN:
		{
			XItem* pEquipItem = NULL;
			if (m_bDyeToInvenItem)
			{
				pEquipItem = m_Inventory.GetItemByID(m_nEquipItemID);
			}
			else
			{
				pEquipItem = m_EquipmentSlot.GetItemByID(m_nEquipItemID);
			}

			XItem* pDyeItem = m_Inventory.GetItemByID(m_nDyeItemID);	

			XBIRDPOSTCMD3(m_pAgent->GetClient(), MC_ITEM_DYE_PREPARE_REQ, MCommandParameterInt(SLOTTYPE_INVENTORY), MCommandParameterInt(pEquipItem->m_nSlotID), MCommandParameterInt(pDyeItem->m_nSlotID));
			ChangePhase(PHASE_DYE_PREPARE);
		}
		break;
	case PHASE_DYE_PREPARE:
		{
			SH_ITEM_SLOT_TYPE nEquipItemSlotType = SLOTTYPE_NONE;
			XItem* pEquipItem = NULL;
			if (m_bDyeToInvenItem)
			{
				pEquipItem = m_Inventory.GetItemByID(m_nEquipItemID);
				nEquipItemSlotType = SLOTTYPE_INVENTORY;
			}
			else
			{
				pEquipItem = m_EquipmentSlot.GetItemByID(m_nEquipItemID);
				nEquipItemSlotType = SLOTTYPE_EQUIP;
			}

			XItem* pDyeItem = m_Inventory.GetItemByID(m_nDyeItemID);			


			XBIRDPOSTCMD3(m_pAgent->GetClient(), MC_ITEM_DYE_REQ, MCommandParameterInt(nEquipItemSlotType), MCommandParameterInt(pEquipItem->m_nSlotID), MCommandParameterInt(pDyeItem->m_nSlotID));
			ChangePhase(PHASE_DYE);
		}
		break;
	case PHASE_FINI:
		{
			XBIRDPOSTCMD0(m_pAgent->GetClient(), MC_GM_CLEAR_INVENTORY_REQ);
		}
		break;
	}
}

minet::MCommandResult XDummyBot_Dye::OnCommand( XBirdDummyAgent* pAgent, MCommand* pCommand )
{
	__super::OnCommand(pAgent, pCommand);

	XBirdClient* pClient = pAgent->GetClient();
	switch(pCommand->GetID())
	{
	case MC_FIELD_CHANGE_FIELD:
		{
			XBIRDPOSTCMD2(pClient, MC_GM_SET_ME_REQ, MCommandParameterWideString(L"grade"), MCommandParameterWideString(L"3"));
			XBIRDPOSTCMD2(pClient, MC_GM_SET_ME_REQ, MCommandParameterWideString(L"level"), MCommandParameterWideString(L"50"));
			XBIRDPOSTCMD0(pClient, MC_GM_GOD_REQ);
		}
		break;
	case MC_ITEM_ADD:
		{
			vector<TD_ITEM_INSERT> vecTDItemInsert;
			if (pCommand->GetBlob(vecTDItemInsert, 0)==false) return CR_ERROR;

			for each(const TD_ITEM_INSERT& tdItemInsert in vecTDItemInsert)
			{
				m_Inventory.InsertItem(tdItemInsert);
			}

			XItem* pDyeItem = m_Inventory.GetItemByID(m_nDyeItemID);
			XItem* pEquipItem = m_Inventory.GetItemByID(m_nEquipItemID);
			if (NULL != pDyeItem && NULL != pEquipItem)
			{
				ChangePhase(PHASE_ADDITEM);
			}
		}
		break;
	case MC_ITEM_REMOVE:
		{
			vector<TD_ITEM_DELETE> vecTDItemDelete;
			if (pCommand->GetBlob(vecTDItemDelete, 0)==false) return CR_ERROR;

			for each(const TD_ITEM_DELETE& tdItemDelete in vecTDItemDelete)
			{
				m_Inventory.PopItem(tdItemDelete.m_nSlotID);
			}

			if (m_Inventory.IsEmpty())
			{
				ChangePhase(PHASE_INIT);
			}
		}
		break;
	case MC_ITEM_EQUIP:
		{
			int nSlotTypeFrom;
			int nSlotIDFrom;
			int8 nSlotIDTo;

			if (pCommand->GetParameter(&nSlotTypeFrom,	0, MPT_INT)==false) return CR_ERROR;
			if (pCommand->GetParameter(&nSlotIDFrom,	1, MPT_INT)==false) return CR_ERROR;
			if (pCommand->GetParameter(&nSlotIDTo,		2, MPT_CHAR)==false) return CR_ERROR;

			XItem* pItem = NULL;
			if (SLOTTYPE_INVENTORY == nSlotTypeFrom)
			{
				// �κ��丮���� ������.
				pItem = m_Inventory.GetItem(nSlotIDFrom);
				if (pItem == NULL)
				{
					return CR_TRUE;
				}

				// ������ ������ �κ��丮���� ����
				m_Inventory.PopItem(nSlotIDFrom);
			}
			else if (SLOTTYPE_EQUIP == nSlotTypeFrom)
			{
				// ���â���� ����
				pItem = m_EquipmentSlot.UnEquipItem(static_cast<SH_ITEM_SLOT>(nSlotIDFrom));
				if (pItem == NULL)
				{
					return CR_TRUE;
				}
			}
			else
			{
				return CR_TRUE;
			}

			// ������ ����
			m_EquipmentSlot.EquipItem(pItem, static_cast<SH_ITEM_SLOT>(nSlotIDTo));

			ChangePhase(PHASE_DYE_PREPARE_BEGIN);
		}
		break;
	case MC_ITEM_EQUIP_SWAPTOINVEN:
		{
			int nEquipSlotIDFrom = 0;
			int nEquipSlotIDTo = 0;
			int nUnequipSlotIDFrom = 0;
			int nUnequipSlotIDTo = 0;

			if (pCommand->GetParameter(&nEquipSlotIDFrom,	0, MPT_INT)==false) return CR_ERROR;
			if (pCommand->GetParameter(&nEquipSlotIDTo,		1, MPT_CHAR)==false) return CR_ERROR;
			if (pCommand->GetParameter(&nUnequipSlotIDFrom,	2, MPT_CHAR)==false) return CR_ERROR;
			if (pCommand->GetParameter(&nUnequipSlotIDTo,	3, MPT_INT)==false) return CR_ERROR;

			// ������ ������ ��񿡼� ������.
			XItem* pUnequipItem = m_EquipmentSlot.UnEquipItem(SH_ITEM_SLOT(nUnequipSlotIDFrom));

			// ������ ������ �κ��丮���� ������.
			XItem* pEquipItem = m_Inventory.GetItem(nEquipSlotIDFrom);
			if (pEquipItem == NULL) return CR_TRUE;

			// ������ ������ �κ��丮���� ����
			m_Inventory.PopItem(nEquipSlotIDFrom);

			// ������ ������ �κ��丮�� �ִ´�.
			if (pUnequipItem != NULL)
			{
				pUnequipItem->m_nSlotID = nUnequipSlotIDTo;
				m_Inventory.PushItem(nUnequipSlotIDTo, pUnequipItem);
			}

			// ������ ������ ����
			m_EquipmentSlot.EquipItem(pEquipItem, static_cast<SH_ITEM_SLOT>(nEquipSlotIDTo));

			ChangePhase(PHASE_DYE_PREPARE_BEGIN);
		}
		break;
	case MC_ITEM_DYE:
		{
			ChangePhase(PHASE_FINI);
		}
		break;
	}

	return CR_TRUE;
}


void XDummyBot_Dye::InitTestItemID()
{
	m_vecEquipItemID.push_back(1090);
	m_vecEquipItemID.push_back(1091);
	m_vecEquipItemID.push_back(1092);
	m_vecEquipItemID.push_back(1093);
	m_vecEquipItemID.push_back(1094);

	m_vecDyeItemID.push_back(20001000);
	/*m_vecDyeItem.push_back(20001001);*/
	m_vecDyeItemID.push_back(20001002);
	m_vecDyeItemID.push_back(20001003);
	m_vecDyeItemID.push_back(20001004);
	m_vecDyeItemID.push_back(20001005);
	/*m_vecDyeItem.push_back(20001006);*/
}

void XDummyBot_Dye::ChangePhase( PHASE nPhase )
{
	m_nPhase = nPhase;
}