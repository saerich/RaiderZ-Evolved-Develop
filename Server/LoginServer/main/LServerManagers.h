#pragma once

#include "LGlobal.h"


/// ���� ��ü���� ����ϴ� �Ŵ��� ����
class LServerManagers : public LGlobalManager
{
public:
	////
	LServerManagers();
	virtual ~LServerManagers();
	void Create();
	void Destroy();
};
