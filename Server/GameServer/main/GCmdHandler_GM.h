#ifndef _GCMD_HANDLER_GM_H
#define _GCMD_HANDLER_GM_H

#include "MCommandHandler.h"
using namespace minet;

class GEntityPlayer;

/// ���� ���� Ŀ�ǵ� �ڵ鷯
class GCmdHandler_GM : public MCommandHandler, public MTestMemPool<GCmdHandler_GM>
{
private:
	static bool IsDeveloper(GEntityPlayer* pEntityPlayer);
	static bool IsDeveloper(MCommand* pCmd);
	static bool IsGM(GEntityPlayer* pEntityPlayer);
	static bool IsGM(MCommand* pCmd);
public:
	GCmdHandler_GM(MCommandCommunicator* pCC);

	DECL_CMD_HANDLER(OnRequestSpawn);					///< NPC ���� ��û
	DECL_CMD_HANDLER(OnRequestDespawn);					///< NPC ���� ��û
	DECL_CMD_HANDLER(OnRequestDespawnByID);				///< NPCID���� ���� ��û
	DECL_CMD_HANDLER(OnRequestGMAIRunning);				///< AI ���� ���ϰ�
	DECL_CMD_HANDLER(OnRequestNPCAIRunning);			///< AI ���� ���ϰ�
	DECL_CMD_HANDLER(OnRequestGMAICommand);				///< AI���� ����Ѵ�
	DECL_CMD_HANDLER(OnRequestGMAISetMonitorNPC);		///< AI���� �ŷ�Ʈ ����϶�� ���
	DECL_CMD_HANDLER(OnRequestDebugString);				///< Debug String
	DECL_CMD_HANDLER(OnRequestGMGetPlayerUID);			///< UID ��û
	DECL_CMD_HANDLER(OnRequestReportTargetEntity);		///< UID ��û
	DECL_CMD_HANDLER(OnRequestGMMove);
	DECL_CMD_HANDLER(OnRequestGMMoveToMySpot);
	DECL_CMD_HANDLER(OnRequestGMMoveToPlayer);
	DECL_CMD_HANDLER(OnRequestGMMoveToNPC);
	DECL_CMD_HANDLER(OnRequestGMSummon);
	DECL_CMD_HANDLER(OnRequestGMRebirth);
	DECL_CMD_HANDLER(OnRequestGMGod);
	DECL_CMD_HANDLER(OnRequestGMGhost);
	DECL_CMD_HANDLER(OnRequestGMEnableICheck);
	DECL_CMD_HANDLER(OnRequestGMDisableICheck);
	DECL_CMD_HANDLER(OnRequestGMChangeWeather);
	DECL_CMD_HANDLER(OnRequestGMChangeTime);	
	DECL_CMD_HANDLER(OnRequestGMItemGive);
	DECL_CMD_HANDLER(OnRequestGMClearInventory);
	DECL_CMD_HANDLER(OnRequestGMKillEntity);
	DECL_CMD_HANDLER(OnRequestGMRangeKillEntity);
	DECL_CMD_HANDLER(OnRequestGMSetMe);
	DECL_CMD_HANDLER(OnRequestGMSetNPC);
	DECL_CMD_HANDLER(OnRequestGMSetItem);

	DECL_CMD_HANDLER(OnRequestGMQuestGive);
	DECL_CMD_HANDLER(OnRequestGMQuestComplete);
	DECL_CMD_HANDLER(OnRequestGMQuestReset);
	DECL_CMD_HANDLER(OnRequestGMQuestResetAll);
	DECL_CMD_HANDLER(OnRequestGMQuestVar);
	DECL_CMD_HANDLER(OnRequestGMQuestFail);
	DECL_CMD_HANDLER(OnRequestGMQuestReward);


	DECL_CMD_HANDLER(OnRequestGMInsertBuff);
	DECL_CMD_HANDLER(OnRequestGMDeleteBuff);
	DECL_CMD_HANDLER(OnRequestGMResetCoolTime);

	DECL_CMD_HANDLER(OnRequestGMShowEnemyUID);

	DECL_CMD_HANDLER(OnRequestGMBreakPart);	
	DECL_CMD_HANDLER(OnRequestGMRangeBreakPart);
	
	DECL_CMD_HANDLER(OnRequestGMLogCRTInsert);
	DECL_CMD_HANDLER(OnRequestGMLogCRTDelete);

	DECL_CMD_HANDLER(OnRequestGMQueryMultiLogin);

	DECL_CMD_HANDLER(OnRequestGMFactionIncrease);
	DECL_CMD_HANDLER(OnRequestGMFactionDecrease);
	DECL_CMD_HANDLER(OnRequestGMFactionReset);

	DECL_CMD_HANDLER(OnRequestGMChangeServerMode);
	DECL_CMD_HANDLER(OnRequestGMResetExpoCharacters);

	DECL_CMD_HANDLER(OnRequestStartCommandProfile);
	DECL_CMD_HANDLER(OnRequestEndCommandProfile);

	DECL_CMD_HANDLER(OnRequestServerDump);
	DECL_CMD_HANDLER(OnRequestGMDye);
	DECL_CMD_HANDLER(OnRequestGMRegen);

	DECL_CMD_HANDLER(OnRequestDebugEchoReq);
};


#endif