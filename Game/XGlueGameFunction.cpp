#include "stdafx.h"
#include "XGlueGameFunction.h"
#include "XChatCmds.h"
#include "XPost_Local.h"
#include "XEventID.h"
#include "XMyPlayer.h"
#include "XModuleMyControl.h"
#include "XPost_GM.h"
#include "XMMCode.h"
#include "XStrings.h"
#include "XNonPlayer.h"
#include "XModuleEffect.h"
#include "XConsoleCmds.h"
#include "XFeedbackSystem.h"
#include "XNetwork.h"
#include "XSystem.h"
#include "XInteractionInfo.h"
#include "XPost_Msg.h"
#include "XPost_Comm.h"


#include "XCommonFunction.h"
#include "XGlueGameFieldFunction.h"
#include "XGlueGameQuestFunction.h"
#include "XGlueGamePartyFunction.h"
#include "XGlueGameCraftFunction.h"
#include "XGlueGameInteractionFunction.h"
#include "XGlueGameTradeFunction.h"
#include "XGlueGameLootFunction.h"
#include "XGlueGameItemFunction.h"
#include "XGlueGameGuildFunction.h"
#include "XGlueGameDuelFunction.h"
#include "XGlueGameRepairFunction.h"

void XGlueGameFunction::DeclGlueFunc( MWLua* pLua )
{
	MWLua::class_<XGlueGameFunction>(pLua->GetState(), "_shGame")
			
		// ����Ʈ
		.def("GetQuestLevel",			&XGlueGameQuestFunction::GetQuestLevel)
		.def("GetQuestLimitPersonnel",	&XGlueGameQuestFunction::GetQuestLimitPersonnel)
		//.def("GetQuestName",			&XGlueGameQuestFunction::GetQuestName)
		//.def("GetQuestDesc",			&XGlueGameQuestFunction::GetQuestDesc)
		//.def("IsQuestShareable",		&XGlueGameQuestFunction::IsQuestManualShare)
		//.def("GetCountQuestObjectives",	&XGlueGameQuestFunction::GetCountQuestObjectives)
		//.def("GetQuestObjectiveAchive",	&XGlueGameQuestFunction::GetQuestObjectiveAchive)
		.def("GetQuestObjectiveID",		&XGlueGameQuestFunction::GetQuestObjectiveID)
		//.def("GetQuestObjectiveDesc",	&XGlueGameQuestFunction::GetQuestObjectiveDesc)
		.def("GetQuestObjectiveZoneID",	&XGlueGameQuestFunction::GetQuestObjectiveZoneID)
		.def("GetZoneID",				&XGlueGameQuestFunction::GetZoneID)
		.def("GiveUpQuest",				&XGlueGameQuestFunction::GiveUpQuest)
		//.def("ShareQuest",				&XGlueGameQuestFunction::ShareQuest)
		.def("QuestAccept",				&XGlueGameQuestFunction::QuestAccept)
		.def("QuestReject",				&XGlueGameQuestFunction::QuestReject)		
		.def("QuestReward",				&XGlueGameQuestFunction::QuestReward)
		.def("GetRewardCount",			&XGlueGameQuestFunction::GetRewardCount)
		.def("GetRewardTypeNParam",		&XGlueGameQuestFunction::GetRewardTypeNParam)
		.def("GetSelectableRewardCount",&XGlueGameQuestFunction::GetSelectableRewardCount)		
		.def("GetSelectableRewardTypeNParam", &XGlueGameQuestFunction::GetSelectableRewardTypeNParam)
		.def("QuestAbandonDestroyItem", &XGlueGameQuestFunction::QuestAbandonDestroyItem)
		.def("QuestObjectivePos",		&XGlueGameQuestFunction::QuestObjectivePos)
		.def("QuestInteractionCancelReq", &XGlueGameQuestFunction::QuestInteractionCancelReq)

		

		//Ʈ���̵�
		.def("TradeStartReq",			&XGlueGameTradeFunction::TradeStartReq)
		.def("TradeRequiredAnswer",		&XGlueGameTradeFunction::TradeRequiredAnswer)
		.def("TradeItemListUp",			&XGlueGameTradeFunction::TradeItemListUp)
		.def("TradeItemListDel",		&XGlueGameTradeFunction::TradeItemListDel)
		.def("TradePutUpSilver",			&XGlueGameTradeFunction::TradePutUpSilver)
		.def("TradeConfirmRequire",		&XGlueGameTradeFunction::TradeConfirmRequire)
		.def("TradeCancel",				&XGlueGameTradeFunction::TradeCancel)

		// ��Ƽ
		.def("PartyInviteReq",			&XGlueGamePartyFunction::PartyInviteReq)	
		.def("PartyRequiredAnswer",		&XGlueGamePartyFunction::PartyRequiredAnswer)	
		.def("PartyLeave",				&XGlueGamePartyFunction::PartyLeave)
		.def("PartyFired",				&XGlueGamePartyFunction::PartyFired)
		.def("PartyDivisionRule",		&XGlueGamePartyFunction::PartyDivisionRule)		
		.def("PartyMessage",			&XGlueGamePartyFunction::PartyMessage)		
		.def("PartyDemand",				&XGlueGamePartyFunction::PartyDemand)	
		.def("PartyDemandAnswer",		&XGlueGamePartyFunction::PartyDemandAnswer)	
		.def("GetPartyLeaderUID",		&XGlueGamePartyFunction::GetPartyLeaderUID)	
		.def("GetPartyMemberName",		&XGlueGamePartyFunction::GetPartyMemberName)	
		.def("IsPartyMemberDead",		&XGlueGamePartyFunction::IsPartyMemberDead)	
		.def("IsPartyMemberOffline",	&XGlueGamePartyFunction::IsPartyMemberOffline)	
		.def("IsPartyMemberHightlight",	&XGlueGamePartyFunction::IsPartyMemberHightlight)	
		.def("GetPartyMemberHP",		&XGlueGamePartyFunction::GetPartyMemberHP)	
		.def("GetPartyMemberEN",		&XGlueGamePartyFunction::GetPartyMemberEN)	
		.def("GetPartyMemberSTA",		&XGlueGamePartyFunction::GetPartyMemberSTA)	
		.def("GetPartyMemberLevel",		&XGlueGamePartyFunction::GetPartyMemberLevel)	
		.def("GetPartyMemberClass",		&XGlueGamePartyFunction::GetPartyMemberClass)	

		// NPC Interaction
		.def("InteractionElementReq",	&XGlueGameInteractionFunction::InteractionElementReq)
		.def("InteractionEndRequest",	&XGlueGameInteractionFunction::InteractionEndRequest)

		//ũ����Ʈ
		.def("GetItemIDWithRecipe",		&XGlueGameCraftFunction::GetItemIDWithRecipe)
		//.def("GetRecipeRank",			&XGlueGameCraftFunction::GetRecipeRank)
		//.def("GetOptionToolOfRecipe",	&XGlueGameCraftFunction::GetOptionToolOfRecipe)
		.def("GetMatOfRecipe",			&XGlueGameCraftFunction::GetMatOfRecipe)
		//.def("GetSubMatOfRecipe",		&XGlueGameCraftFunction::GetSubMatOfRecipe)
		.def("GetCraftableItemQuantity",&XGlueGameCraftFunction::GetCraftableItemQuantity)
		//.def("GetCraftableItemRate",	&XGlueGameCraftFunction::GetCraftableItemRate)
		.def("StartCraftingItem",		&XGlueGameCraftFunction::StartCraftingItem)
		.def("CraftingCancelReq",		&XGlueGameCraftFunction::CraftingCancelReq)

		//loot
		.def("LootItem",				&XGlueGameLootFunction::LootItem)
		.def("LootEndRequest",			&XGlueGameLootFunction::LootEndRequest)

		//�ʵ�, ä��
		.def("ReqChannelList",			&XGlueGameFieldFunction::ReqChannelList)
		.def("ChangeChannel",			&XGlueGameFieldFunction::ChangeChannel)
		.def("ChangeChannelCancel",		&XGlueGameFieldFunction::ChangeChannelCancel)

		//����
		//.def("ChangeWeather",			&XGlueGameFieldFunction::ChangeWeather)
		//.def("ChangeCurrentWeather",	&XGlueGameFieldFunction::ChangeCurrentWeather)
		//.def("ChangeCustomWeather",		&XGlueGameFieldFunction::ChangeCustomWeather)

		//.def("Spawn",				&XGlueGameFieldFunction::Spawn)
		//.def("SpawnByID",			&XGlueGameFieldFunction::SpawnByID)
		//.def("DespawnByID",			&XGlueGameFieldFunction::DespawnByID)
		//.def("GetSpawnNPC",			&XGlueGameFieldFunction::GetSpawnNPC)
		//.def("EnableSensor",		&XGlueGameFieldFunction::EnableSensor)
		//.def("DisableSensor",		&XGlueGameFieldFunction::DisableSensor)
		//.def("CheckSensor",			&XGlueGameFieldFunction::CheckSensor)
		//.def("GetNPCCount",			&XGlueGameFieldFunction::GetNPCCount)
		//.def("SetTimer",			&XGlueGameFieldFunction::SetTimer)
		//.def("KillTimer",			&XGlueGameFieldFunction::KillTimer)
		//.def("GetMarkerPos",		&XGlueGameFieldFunction::GetMarkerPos)
		//.def("GetMarkerDir",		&XGlueGameFieldFunction::GetMarkerDir)
		//.def("GetID",				&XGlueGameFieldFunction::GetID)
		//.def("IsNull",				&XGlueGameFieldFunction::IsNull)

		//������
		.def("GetItemName",				&XGlueGameItemFunction::GetItemName)
		.def("GetItemPrice",			&XGlueGameItemFunction::GetItemPrice)
		.def("GetItemType",				&XGlueGameItemFunction::GetItemType)
		.def("GetItemEquipmentType",	&XGlueGameItemFunction::GetItemEquipmentType)
		.def("GetItemAP",				&XGlueGameItemFunction::GetItemAP)
		.def("GetItemSlotType",			&XGlueGameItemFunction::GetItemSlotType)
		.def("GetItemStackSize",		&XGlueGameItemFunction::GetItemStackSize)
		.def("GetItemMaxDurability",	&XGlueGameItemFunction::GetItemMaxDurability)		
		.def("GetItemIconName",			&XGlueGameItemFunction::GetItemIconName)
		
		.def("UseItem",					&XGlueGameItemFunction::UseItem)
		.def("DropItem",				&XGlueGameItemFunction::DropItem)

		//����
		.def("BuyItem",					&XGlueGameItemFunction::BuyItem)
		.def("SellItem",				&XGlueGameItemFunction::SellItem)

		// ä��
		.def("ChatGlobal",				&XGlueGameFunction::ChatGlobal)

		// ��ȭ
		.def("DialogButtonSel",			&XGlueGameFunction::DialogButtonSel)
		.def("XGetString",				&XGlueGameFunction::XGetString)
		.def("XGetString2",				&XGlueGameFunction::XGetString2)
		.def("UnHoldMyCharacter",		&XGlueGameFunction::UnHoldMyCharacter)

		// �α���

		// ���� ����

		// ��Ʈ��ũ
		.def("Connect",					&XGlueGameFunction::Connect)
		.def("Disconnect",				&XGlueGameFunction::Disconnect)
	
		//����
		.def("GetPlayerID",				&XGlueGameFunction::GetPlayerID)
		.def("GetInteractivePlayerName",&XGlueGameFunction::GetInteractivePlayerName)
		.def("GetDynamicFieldRemainTime", &XGlueGameFunction::GetDynamicFieldRemainTime)

		//��Ȱ
		.def("RequireRebirth",			&XGlueGameFunction::RequireRebirth)

		// Buff
		.def("GetBuffName",				&XGlueGameFunction::GetBuffName)
		.def("IsBuff",					&XGlueGameFunction::IsBuff)

		//�̴ϸ�
		.def("GameFieldTime",			&XGlueGameFunction::GameTime)
		.def("GameFieldWeather",		&XGlueGameFunction::GameWeather)

		//��Ʈ
		.def("GetStringWidth",			&XGlueGameFunction::GetStringWidth)		

		//����
		.def("ReqRepairItemList",		&XGlueGameRepairFunction::ReqRepairItemList)
		.def("RepairAllItem",			&XGlueGameRepairFunction::RepairAllItem)
		.def("RepairItem",				&XGlueGameRepairFunction::RepairItem)

		//����
		.def("DuelRequest",				&XGlueGameDuelFunction::DuelRequest)
		.def("DuelRequiredAnswer",		&XGlueGameDuelFunction::DuelRequiredAnswer)

		//���
		.def("GuildDestroy",			&XGlueGameGuildFunction::GuildDestroy)
		.def("GuildInviteQ",			&XGlueGameGuildFunction::GuildInviteQ)
		.def("GuildRequiredAnswer",		&XGlueGameGuildFunction::GuildRequiredAnswer)

		// �ǵ��
		.def("SendFeedback",			&XGlueGameFunction::SendFeedback)
		.def("ReloadFeedback",			&XGlueGameFunction::ReloadFeedback)
		;
}

void XGlueGameFunction::ChatGlobal(const char* szMsg)
{
	bool bDone = false;

	// �ϴ� ���� ä�� ��ɾ� ����
	if (XChatCmds::Input(MLocale::ConvAnsiToUTF16(szMsg).c_str()))
	{
		bDone = true;
	}

	// '@'�� �����ϸ� �ܼ� ��ɾ� ����
	if (bDone==false && strlen(szMsg) >= 2)
	{
		if (szMsg[0] == '@')
		{
			if (XConsoleCmds::Input(MLocale::ConvAnsiToUTF16(&szMsg[1]).c_str()))
			{
				bDone = true;
			}
		}
	}

	if (bDone==false && szMsg[0] != 0)
	{
		XPostMsg_Say(MLocale::ConvAnsiToUTF16(szMsg).c_str());
	}


	// ��Ŀ�� ����
	// �ϴ� ���� ������ ��Ʈ�� ����� �� �̺�Ʈ ������ �ȴٸ� ��ũ��Ʈ�� �Ű��� �����Դϴ�. - birdkr
//	if (global.ui) global.ui->SetFocusMainInput();
}

void XGlueGameFunction::Connect(const char* szLoginID, const char* szPassword, const char* szIP)
{
	// TODO : �ϴ� �׽�Ʈ�� ��Ʒ� ������ ����.
	XCommonFunction::Connect(MLocale::ConvAnsiToUTF16(szLoginID).c_str(), MLocale::ConvAnsiToUTF16(szPassword).c_str(), MLocale::ConvAnsiToUTF16(szIP).c_str());
}

void XGlueGameFunction::Disconnect()
{
	if (global.net) global.net->PostDisconnect();
}

void XGlueGameFunction::DialogButtonSel(int nSel)
{
	XPostRequestDialogSelect(nSel);
}

const char* XGlueGameFunction::XGetString(const char* szSrcString )
{
	if (szSrcString == NULL) return NULL;

	XStringResMgr::GetInstance()->TransKeyStr(m_strStringBuffer, MLocale::ConvAnsiToUTF16(szSrcString).c_str());

	gvar.Temp.szTemp = MLocale::ConvUTF16ToAnsi(m_strStringBuffer.c_str());

	return gvar.Temp.szTemp.c_str();
}

const char* XGlueGameFunction::XGetString2(const char* szSrcString )
{
	if (szSrcString == NULL) return NULL;

	wstring strBuf;
	XStringResMgr::GetInstance()->TransKeyStr(strBuf, MLocale::ConvAnsiToUTF16(szSrcString).c_str());

	// �ʿ���� �����ڵ�� �����Ѵ�.
	mmcode::MControlCodeTransformer transformer(MLocale::ConvUTF16ToAnsi(strBuf.c_str()), XMMCodeMgr::GetInstance().GetTagManager());
	mmcode::MControlCodeTransResult result = transformer.transform();
	m_strStringBuffer = MLocale::ConvAnsiToUTF16(result.m_strOutput.c_str());

	gvar.Temp.szTemp = MLocale::ConvUTF16ToAnsi(m_strStringBuffer.c_str());
	return gvar.Temp.szTemp.c_str();
}

void XGlueGameFunction::UnHoldMyCharacter()
{
	XModuleMyControl* pModuleMyControl = gvar.Game.pMyPlayer->GetModuleMyControl();
	if (NULL == pModuleMyControl) return;
	if( ACTION_STATE_HOLD != pModuleMyControl->GetCurrentActionStateID()) return;

	XEvent msg;
	msg.m_nID = XEVTL_MYACTION_RELEASE_ACTION_HOLD;
	gg.omgr->PostEvent(gvar.MyInfo.MyUID, msg);
}

const char* XGlueGameFunction::GetInteractivePlayerName()
{
	return GetPlayerID(XGetInteractionInfo().GetInteractionSearchUID());
}

void XGlueGameFunction::RequireRebirth( int nSelect )
{
	//switch (nSelect)
	//{
	//case 1:
	//	{
	//		XPostChar_Rebirth(REBT_SOULBINDING);
	//	}
	//	break;
	//case 2:
	//	{
	//		XPostChar_Rebirth(REBT_CHECKPOINT);
	//	}
	//	break;
	//case 3:
	//	{
	//		XItem* pItem = gvar.MyInfo.Inventory.GetUsableSelfRebirthItem();
	//		if (NULL == pItem) return;

	//		gvar.Game.pMyPlayer->GetModuleMyControl()->DoActionUseItem(pItem->m_nSlotID);
	//	}
	//	break;
	//default:
	//	{
	//		gvar.Game.bRebirth = false;	//��Ȱ���ҷ�...
	//	}
	//	break;
	//}
}

const char* XGlueGameFunction::GetBuffName( int nBuffID )
{
	XBuffInfo * pBuffInfo = info.buff->Get(nBuffID);
	if(pBuffInfo)
	{
		gvar.Temp.szTemp = MLocale::ConvUTF16ToAnsi(pBuffInfo->GetName());
		return gvar.Temp.szTemp.c_str();
	}

	return NULL;
}

bool XGlueGameFunction::IsBuff(int nBuffID)
{
	XBuffInfo * pBuffInfo = info.buff->Get(nBuffID);
	if(pBuffInfo)
		return (pBuffInfo->m_nType == BUFT_BUFF) ? true : false;

	return false;
}

unsigned int XGlueGameFunction::GetDynamicFieldRemainTime()
{
	if (gvar.World.nDynamicFieldExpiredTime >= global.system->GetTimer().GetNowGlobalTime())
	{
		return gvar.World.nDynamicFieldExpiredTime - global.system->GetTimer().GetNowGlobalTime();
	}

	return 0;
}

int XGlueGameFunction::GameTime()
{
	//return XStrings::GameTime(gvar.World.nGameTime);
	return gvar.World.nGameTime;
}

int XGlueGameFunction::GameWeather()
{
	//return XStrings::Weather(gvar.World.nWeather);
	return gvar.World.nWeather;
}

const char* XGlueGameFunction::GetPlayerID( MUID uid )
{
	gvar.Temp.szTemp = MLocale::ConvUTF16ToAnsi(XCommonFunction::GetPlayerName(uid));
	return gvar.Temp.szTemp.c_str();
}

int XGlueGameFunction::GetStringWidth( const char* szWidgetName, const char* szMsg )
{
	MWidget* pWidget = (MFrame*)global.mint->FindWidget(szWidgetName);
	if (pWidget== NULL) return 0;
	
//	MFont* pFont = pWidget->GetFont();
	MFont* pFont = MFontManager::Get( NULL);
	if (pFont == NULL) return 0;

	return pFont->GetWidth(szMsg);
}

void XGlueGameFunction::SendFeedback()
{
	// textarea�� Text�� ���� �� ��� �ϴ� �̷��� �ϵ��ڵ�����. - birdkr
	MWidget* pFeedbackWidget = global.mint->FindWidget("FeedbackWindow_TextArea");
	if (pFeedbackWidget == NULL) return;
//	MTextArea* pFeedbackTextArea = dynamic_cast<MTextArea*>(pFeedbackWidget);
// 	if (pFeedbackTextArea == NULL) return;
// 
// 	wstring strText;
// 	for (int i = 0; i < pFeedbackTextArea->GetLineCount(); i++)
// 	{
// 		strText += pFeedbackTextArea->GetTextLine(i);
// 		strText += "\n";
// 	}


// 	if (global.feedback)
// 	{
// 		global.feedback->SendFeedback(strText.c_str());
// 	}
}

void XGlueGameFunction::ReloadFeedback()
{
	if (global.feedback == NULL) return;

	global.feedback->ReloadFeedbackFromWeb();
}