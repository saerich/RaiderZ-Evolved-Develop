#pragma once

#include "MCommandHandler.h"
using namespace minet;

/// ��Ű��� Ŀ�ǵ� �ڵ鷯
class LCmdHandler_Master : public MCommandHandler
{
private:

public:
	LCmdHandler_Master(MCommandCommunicator* pCC);

	DECL_CMD_HANDLER(OnNetClear);
	DECL_CMD_HANDLER(OnRequestLogin);
};

