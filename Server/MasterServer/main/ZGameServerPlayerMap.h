#pragma once

#include "ZPlayerManager.h"

/// ���� ���� ������Ʈ�� ������ �ִ� �÷��̾� �ν��Ͻ�
/// - ZPlayer �ν��Ͻ��� ZPlayerManager�� ������ �ִ�.
class ZGameServerPlayerMap : public ZBasePlayerManager
{
public:
	ZGameServerPlayerMap() : ZBasePlayerManager() {}
	virtual ~ZGameServerPlayerMap();

	virtual void Add(ZPlayer* pPlayer) override;
	virtual void Clear() override;

};
