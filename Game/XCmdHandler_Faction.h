#pragma once

#include "MCommandHandler.h"
using namespace minet;

/// �Ѽ� Ŀ�ǵ� ó�� �ڵ鷯
class XCmdHandler_Faction : public MCommandHandler
{
public:
	XCmdHandler_Faction(MCommandCommunicator* pCC);

	DECL_CMD_HANDLER(OnIncrease);
	DECL_CMD_HANDLER(OnDecrease);
};
