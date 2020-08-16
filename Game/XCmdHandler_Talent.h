#ifndef _XCMD_HANDLER_TALENT_H
#define _XCMD_HANDLER_TALENT_H

#include "MCommandHandler.h"
using namespace minet;

/// ������ Ŀ�ǵ� ó�� �ڵ鷯
class XCmdHandler_Talent  : public MCommandHandler
{
private:
protected:	
	DECL_CMD_HANDLER(OnBuffGain);
	DECL_CMD_HANDLER(OnBuffLost);
	DECL_CMD_HANDLER(OnTalentInstantEffectGain);
	DECL_CMD_HANDLER(OnBuffInstantEffectGain);
	DECL_CMD_HANDLER(OnFocusGain);
	DECL_CMD_HANDLER(OnFocusLost);
	DECL_CMD_HANDLER(OnFocusUpdateBubble);
	DECL_CMD_HANDLER(OnBuffHeal);

	DECL_CMD_HANDLER(OnBuffIncrease);
	DECL_CMD_HANDLER(OnBuffDecrease);
		
public:
	XCmdHandler_Talent(MCommandCommunicator* pCC);
};



#endif