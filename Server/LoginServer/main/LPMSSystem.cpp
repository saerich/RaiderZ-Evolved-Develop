#include "stdafx.h"
#include "LPMSSystem.h"
#include "PMServerInitLogger.h"
#include "LPMSObject.h"
#include "LConfig.h"

LPMSSystem::LPMSSystem()
{
	SetGameInfo(LConfig::m_SSN, LConfig::m_nWorldID);
	pCallbackObj = new LPMSObject(this);
}

LPMSSystem::~LPMSSystem()
{
	SAFE_DELETE(pCallbackObj);
}

IPMSObject* LPMSSystem::GetPMSObject()
{
	return pCallbackObj;
}
