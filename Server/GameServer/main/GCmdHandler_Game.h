#ifndef _GCMD_HANDLER_GAME_H
#define _GCMD_HANDLER_GAME_H

#include "MCommandHandler.h"
using namespace minet;

/// ���Ӱ��� Ŀ�ǵ� �ڵ鷯 - �ϴ� �̰��� �����δٰ� �ʹ� �������� �и�����.
class GCmdHandler_Game : public MCommandHandler, public MTestMemPool<GCmdHandler_Game>
{
private:

public:
	GCmdHandler_Game(MCommandCommunicator* pCC);

	
};


#endif