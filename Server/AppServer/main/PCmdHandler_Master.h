#pragma once

#include "MCommandHandler.h"
using namespace minet;

/// ��Ű��� Ŀ�ǵ� �ڵ鷯
class PCmdHandler_Master : public MCommandHandler
{
private:

public:
	PCmdHandler_Master(MCommandCommunicator* pCC);

	DECL_CMD_HANDLER(OnNetClear);
	DECL_CMD_HANDLER(OnRequestLogin);
};

