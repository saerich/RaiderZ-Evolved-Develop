#pragma once

#include "ZGlobal.h"


/// ���� ��ü���� ����ϴ� �Ŵ��� ����
class ZServerManagers : public ZGlobalManager
{
public:
	////
	ZServerManagers();
	virtual ~ZServerManagers();
	void Create();
	void Destroy();
};
