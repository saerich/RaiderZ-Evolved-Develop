#ifndef _XCMD_HANDLER_ENCHANT_H
#define _XCMD_HANDLER_ENCHANT_H

#include "MCommandHandler.h"
using namespace minet;

/// ��üƮ Ŀ�ǵ� ó�� �ڵ鷯
class XCmdHandler_Enchant : public MCommandHandler
{
public:
	XCmdHandler_Enchant(MCommandCommunicator* pCC);

	DECL_CMD_HANDLER(OnEnchantDone);
	DECL_CMD_HANDLER(OnEnchantBroken);
	DECL_CMD_HANDLER(OnEnchantCheckResult);
};

#endif//_XCMD_HANDLER_ENCHANT_H
