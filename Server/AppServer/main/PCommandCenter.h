#pragma once

#include "MCommand.h"
#include "PNetServer.h"
#include "SCommandCenter.h"
#include "STypes.h"

class SCommObjectManager;
class PChatChannel;


/// Ŀ�ǵ� �帧�� �����ϴ� Ŭ����
class PCommandCenter : public SCommandCenter
{
public:
	PCommandCenter(minet::MNetServer* netserver);
	virtual ~PCommandCenter() {}

	void RouteToAll(MCommand* pCommand);
	void RouteToChannel(PChatChannel* pChannel, minet::MCommand* pCommand, CID cidIgnorePlayer = 0);

};



