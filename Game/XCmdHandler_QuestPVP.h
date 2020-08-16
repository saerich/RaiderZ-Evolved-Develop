#pragma once

#include "MCommandHandler.h"
using namespace minet;

/// ����ƮPVP ���� Ŀ�ǵ� ó��
class XCmdHandler_QuestPVP : public MCommandHandler
{
public:
	XCmdHandler_QuestPVP(MCommandCommunicator* pCC);

	DECL_CMD_HANDLER(OnTeamCount);
	DECL_CMD_HANDLER(OnEnterPvpZone);
	DECL_CMD_HANDLER(OnBeginEvent);
	DECL_CMD_HANDLER(OnEndEvent);
	DECL_CMD_HANDLER(OnMarkNPCPos);
	DECL_CMD_HANDLER(OnUnmarkNPCPos);	
};
