#ifndef _XPOST_GM_H
#define _XPOST_GM_H

#include "XNetClient.h"
#include "XCommandTable.h"


// NPC ����
inline void XPostGM_Spawn(int nNPCID, int nCount, vec3 vSpawnPos)
{
	XPOSTCMD3(MC_GM_REQUEST_SPAWN, MCommandParameterInt(nNPCID), MCommandParameterInt(nCount), MCommandParameterVector(vSpawnPos));
}

// NPC ����
inline void XPostGM_Despawn(float fRange)
{
	XPOSTCMD1(MC_GM_REQUEST_DESPAWN, MCommandParameterFloat(fRange));
}

// NPCID�� ����
inline void XPostGM_DespawnByID(int nNPCID, float fRange)
{
	XPOSTCMD2(MC_GM_DESPAWNBYID_REQ, MCommandParameterInt(nNPCID), MCommandParameterFloat(fRange));
}

// ���� ���� �̵� ��û
inline void XPostGM_Move(int nFieldID, vec3& pos)
{
	XPOSTCMD2(MC_GM_MOVE_REQ, MCommandParameterInt(nFieldID), MCommandParameterVector(pos));
}

// PC���Է� �̵� ��û
inline void XPostGM_MoveToPlayer(const wchar_t* szPlayerID)
{
	XPOSTCMD1(MC_GM_MOVE_TO_PLAYER_REQ, MCommandParameterWideString(szPlayerID));
}

// NPC���Է� �̵� ��û
inline void XPostGM_MoveToNPC(int nNPCID, int nFieldID)
{
	XPOSTCMD2(MC_GM_MOVE_TO_NPC_REQ, MCommandParameterInt(nNPCID), MCommandParameterInt(nFieldID));
}

// PC�� ��ȯ ��û
inline void XPostGM_Summon(const wchar_t* szPlayerID)
{
	XPOSTCMD1(MC_GM_SUMMON_REQ, MCommandParameterWideString(szPlayerID));
}

// �� ����ġ�� �̵� ��û
inline void XPostGMMoveToMySpot()
{
	XPOSTCMD0(MC_GM_MOVE_TO_MYSPOT_REQ);
}

// ��Ȱ ��û
inline void XPostGMRebirth(const vec3& vPos)
{
	XPOSTCMD1(MC_GM_REBIRTH_REQ, MCommandParameterVector(vPos));
}

// ����
inline void XPostGMGod()
{
	XPOSTCMD0(MC_GM_GOD_REQ);
}

inline void XPostGMGhost()
{
	XPOSTCMD0(MC_GM_GHOST_REQ);
}

inline void XPostGM_GameGuardEnable(bool bEnable)
{
	XPOSTCMD1(MC_GG_AUTH_ENABLE, MCommandParameterBool(bEnable));
}

// ���� ���� ��û
inline void XPostGMChangeWeather(int nWeatherType)
{
	XPOSTCMD1(MC_GM_CHANGE_WEATHER_REQ, MCommandParameterInt(nWeatherType));
}

// �ð� ���� ��û
inline void XPostGMChangeTime(int nTimeType)
{
	XPOSTCMD1(MC_GM_CHANGE_TIME_REQ, MCommandParameterInt(nTimeType));
}

// ����Ʈ ���� ��û
inline void XPostGMGiveQuest(int nQuestID)
{
	XPOSTCMD1(MC_GM_QUEST_GIVE_REQ, MCommandParameterInt(nQuestID));
}

inline void XPostGM_QuestReset()
{
	XPOSTCMD0(MC_GM_QUEST_RESET_REQ);
}

inline void XPostGM_QuestComplete(int nQuestID)
{
	XPOSTCMD1(MC_GM_QUEST_COMPLETE_REQ, MCommandParameterInt(nQuestID));
}

inline void XPostGM_QuestReward(int nQuestID)
{
	XPOSTCMD1(MC_GM_QUEST_REWARD_REQ, MCommandParameterInt(nQuestID));
}

inline void XPostGM_QuestVar(int nQuestID)
{
	XPOSTCMD1(MC_GM_QUEST_VAR_REQ, MCommandParameterInt(nQuestID));
}

inline void XPostGM_QuestFail(int nQuestID)
{
	XPOSTCMD1(MC_GM_QUEST_FAIL_REQ, MCommandParameterInt(nQuestID));
}

inline void XPostRequestGiveItem(int nItemID, int nQuantity)
{
	XPOSTCMD2(MC_GM_ITEM_GIVE_REQ, MCommandParameterInt(nItemID), MCommandParameterInt(nQuantity));
}

inline void XPostGMAIRunning()
{
#ifndef _PUBLISH
	XPOSTCMD0(MC_GM_AI_RUNNING_REQ);
#endif
}

inline void XPostGMAICommand(MUID uidTarget, const wchar_t* szCommand, const wchar_t* szParam)
{
#ifdef _PUBLISH
	return;
#endif

	XPOSTCMD3(MC_GM_AI_USE_TALENT_REQ, MCommandParameterUID(uidTarget), MCommandParameterWideString(szCommand), MCommandParameterWideString(szParam));
}

inline void XPostGMToggleNPCAI(MUID uidTargetNPC)
{
#ifdef _PUBLISH
	return;
#endif

	XPOSTCMD1(MC_GM_NPC_AI_RUNNING_REQ, MCommandParameterUID(uidTargetNPC));
}

inline void XPostGMAISetMonitorNPC(UIID nNPCUIID)
{
#ifdef _PUBLISH
	return;
#endif

	XPOSTCMD1(MC_GM_AI_SET_MONITOR_TARGET, MCommandParameterUShort(nNPCUIID));
}

inline void XPostGMReportTargetEntity(MUID uidTarget)
{
#ifdef _PUBLISH
	return;
#endif

	XPOSTCMD1(MC_GM_REPORT_TARGET_ENTITY, MCommandParameterUID(uidTarget));
}

inline void XPostDebugString(const wchar_t* szKey, int nParam1, const wchar_t* szText)
{
	XPOSTCMD4(MC_REQUEST_DEBUG_STRING, MCommandParameterWideString(szKey), MCommandParameterInt(nParam1), MCommandParameterWideString(szText), MCommandParameterBlob());
}

inline void XPostGMSetMe(const wchar_t* szType, const wchar_t* szValue)
{
	XPOSTCMD2(MC_GM_SET_ME_REQ, MCommandParameterWideString(szType), MCommandParameterWideString(szValue));
}

inline void XPostGMSetNPC(const wchar_t* szType, const wchar_t* szValue)
{
	XPOSTCMD2(MC_GM_SET_NPC_REQ, MCommandParameterWideString(szType), MCommandParameterWideString(szValue));
}

inline void XPostGMSetItem(const wchar_t* szType, int nInvenSlotID, int nValue)
{
#ifdef _PUBLISH
	return;
#endif

	XPOSTCMD3(MC_GM_SET_ITEM_REQ, MCommandParameterWideString(szType), MCommandParameterInt(nInvenSlotID), MCommandParameterInt(nValue));
}

inline void XPostGM_InsertBuff(int nBuffID, float fDurationTime, float fPeriodTime)
{
	XPOSTCMD3(MC_GM_INSERT_BUFF_REQ, MCommandParameterInt(nBuffID), MCommandParameterFloat(fDurationTime), MCommandParameterFloat(fPeriodTime));
}

inline void XPostGM_DeleteBuff(int nBuffID)
{
	XPOSTCMD1(MC_GM_DELETE_BUFF_REQ, MCommandParameterInt(nBuffID));
}

inline void XPostGM_ResetCoolTime()
{
	XPOSTCMD0(MC_GM_RESET_COOLTIME_REQ);
}

inline void XPostGM_ShowEnemyUID()
{
	XPOSTCMD0(MC_GM_SHOW_ENEMY_UID_REQ);
}

inline void XPostGM_BreakPart(MUID uidNPC, uint8 nBPartID)
{
	XPOSTCMD2(MC_GM_BREAK_PART_REQ, MCommandParameterUID(uidNPC), MCommandParameterUChar(nBPartID));
}

inline void XPostGM_RangeBreakPart(float fRange)
{
	XPOSTCMD1(MC_GM_RANGE_BREAK_PART_REQ, MCommandParameterFloat(fRange));
}

inline void XPostGM_LogCRTInsert(int nCRT)
{
	XPOSTCMD1(MC_GM_LOG_CRT_INSERT_REQ, MCommandParameterInt(nCRT));
}

inline void XPostGM_LogCRTDelete(int nCRT)
{
	XPOSTCMD1(MC_GM_LOG_CRT_DELETE_REQ, MCommandParameterInt(nCRT));
}

inline void XPostGM_FactionIncrease(uint8 nFactionID, uint16 nQuantity)
{
	XPOSTCMD2(MC_GM_FACTION_INCREASE_REQ, MCommandParameterUChar(nFactionID), MCommandParameterUShort(nQuantity));
}

inline void XPostGM_FactionDecrease(uint8 nFactionID, uint16 nQuantity)
{
	XPOSTCMD2(MC_GM_FACTION_DECREASE_REQ, MCommandParameterUChar(nFactionID), MCommandParameterUShort(nQuantity));
}

inline void XPostGM_ChangeServerMode(int nServerMode)
{
	XPOSTCMD1(MC_GM_CHANGE_SERVER_MODE_REQ, MCommandParameterInt(nServerMode));
}

inline void XPostGM_ResetExpoCharacters()
{
	XPOSTCMD0(MC_GM_RESET_EXPO_CHARACTERS_REQ);
}

inline void XPostGM_StartCommandProfile()
{
	XPOSTCMD0(MC_DEBUG_START_COMMAND_PROFILE);
}

inline void XPostGM_EndCommandProfile()
{
	XPOSTCMD0(MC_DEBUG_END_COMMAND_PROFILE);
}

inline void XPostGM_RequestServerDump(const wchar_t* szID)
{
	XPOSTCMD1(MC_GM_SERVER_DUMP_REQ, MCommandParameterWideString(szID));
}

inline void XPostGM_ClearInventory()
{
	XPOSTCMD0(MC_GM_CLEAR_INVENTORY_REQ);
}

inline void XPostGM_Regen()
{
	XPOSTCMD0(MC_GM_REGEN_REQ);
}

inline void XPostGM_Dye(SH_ITEM_SLOT_TYPE nSlotType, int nSlotID, int nColor)
{
	XPOSTCMD3(MC_GM_DYE_REQ, MCommandParameterInt(nSlotType), MCommandParameterInt(nSlotID), MCommandParameterInt(nColor));
}

#endif