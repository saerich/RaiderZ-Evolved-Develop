#pragma once

#include "PGlobal.h"


/// ���� ��ü���� ����ϴ� �Ŵ��� ����
class PServerManagers : public PGlobalManager
{
public:
	////
	PServerManagers();
	virtual ~PServerManagers();
	void Create();
	void Destroy();
};
