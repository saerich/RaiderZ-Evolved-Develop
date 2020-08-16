#ifndef _XCMD_HANDLER_NPCSHOP_H
#define _XCMD_HANDLER_NPCSHOP_H

#include "MCommandHandler.h"
using namespace minet;

/// ���Ǿ� ���� Ŀ�ǵ� ó�� �ڵ鷯
class XCmdHandler_NPCShop : public MCommandHandler
{
protected:
	DECL_CMD_HANDLER(OnShow);
	DECL_CMD_HANDLER(OnBuy);
	DECL_CMD_HANDLER(OnSell);
	DECL_CMD_HANDLER(OnRepair);
	DECL_CMD_HANDLER(OnSellVeryCommon);

public:
	XCmdHandler_NPCShop(MCommandCommunicator* pCC);
};

#endif //_XCMD_HANDLER_NPCSHOP_H
