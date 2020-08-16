#pragma once

#include "MCommandHandler.h"
using namespace minet;

/// ���� Ŀ�ǵ� ó�� �ڵ鷯
class XCmdHandler_BattleArena : public MCommandHandler
{
public:
	XCmdHandler_BattleArena(MCommandCommunicator* pCC);

	DECL_CMD_HANDLER(OnRegister);
	DECL_CMD_HANDLER(OnDeregister);
	DECL_CMD_HANDLER(OnMatched);
	DECL_CMD_HANDLER(OnCounting);
	DECL_CMD_HANDLER(OnGameStart);
	DECL_CMD_HANDLER(OnReadyDeathmatch);
	DECL_CMD_HANDLER(OnScoreboardDeathmatch);
	DECL_CMD_HANDLER(OnUpdateDeathmatch);
	DECL_CMD_HANDLER(OnRejoinDeathmatch);
	DECL_CMD_HANDLER(OnFinish);
	DECL_CMD_HANDLER(OnDie);	
	DECL_CMD_HANDLER(OnKillInfo);
};
