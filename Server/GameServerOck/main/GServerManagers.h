#pragma once

#include "GGlobal.h"


/// ���� ��ü���� ����ϴ� �Ŵ��� ����
class GServerManagers : public GGlobalManager
{
public:
	////
	GServerManagers();
	virtual ~GServerManagers();
	void Create();
	void Destroy();
};
