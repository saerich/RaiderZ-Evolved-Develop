#pragma once

#include "MCommand.h"
#include "GNetServer.h"
#include "SCommandCenter.h"

class GCommandLogger;

/// Ŀ�ǵ� �帧�� �����ϴ� Ŭ����
class GCommandCenter : public SCommandCenter, public MTestMemPool<GCommandCenter>
{
public:
	GCommandCenter(MNetServer* pNetServer);
	~GCommandCenter() {}

	void SetLogger(GCommandLogger* pCommandLogger);

	void RouteToAll(MCommand* pCommand);
	void RouteToChatChannel(MUID uidChannel, MCommand* pCommand);

	void NetDebugString(const wchar_t* szKey, int nParam, const wchar_t* szText);
};
