#pragma once

#include "MCommand.h"
#include "LNetServer.h"
#include "SCommandCenter.h"

/// Ŀ�ǵ� �帧�� �����ϴ� Ŭ����
class LCommandCenter : public SCommandCenter
{
protected:
public:
	LCommandCenter(minet::MNetServer* netserver, SCommObjectManager* pCommObjectManager);
	virtual ~LCommandCenter() {}

};



