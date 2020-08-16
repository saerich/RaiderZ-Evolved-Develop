#pragma once

#include "MCommandHandler.h"
using namespace minet;

/// ��Ű��� Ŀ�ǵ� �ڵ鷯
class PCmdHandler_Log : public MCommandHandler
{
private:

public:
	PCmdHandler_Log(MCommandCommunicator* pCC);

	DECL_CMD_HANDLER(OnLoginReq);					///< �α���	

	DECL_CMD_HANDLER(OnCharLogReq);	

	DECL_CMD_HANDLER(OnItemAcquireLogReq);
	DECL_CMD_HANDLER(OnItemLostLogReq);
	DECL_CMD_HANDLER(OnItemTradeLogReq);

	DECL_CMD_HANDLER(OnQuestLogReq);
};


