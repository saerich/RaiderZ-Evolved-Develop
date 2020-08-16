#ifndef _GCMD_HANDLER_LOG_H
#define _GCMD_HANDLER_LOG_H

#include "MCommandHandler.h"
using namespace minet;

/// ��Ű��� Ŀ�ǵ� �ڵ鷯
class GCmdHandler_Log : public MCommandHandler
{
private:

public:
	GCmdHandler_Log(MCommandCommunicator* pCC);

	DECL_CMD_HANDLER(OnLoginReq);					///< �α���	

	DECL_CMD_HANDLER(OnCharLogReq);	

	DECL_CMD_HANDLER(OnItemAcquireLogReq);
	DECL_CMD_HANDLER(OnItemLostLogReq);
	DECL_CMD_HANDLER(OnItemTradeLogReq);

	DECL_CMD_HANDLER(OnQuestLogReq);
};

#endif
