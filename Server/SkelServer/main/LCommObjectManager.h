#pragma once

#include "SCommObjectManager.h"

/// Client ������
class LCommObjectManager : public SCommObjectManager
{
public:
	LCommObjectManager();
	virtual ~LCommObjectManager();
	virtual SCommObject* NewObject(MUID& uid);
};


