#include "stdafx.h"
#include "XSensorInfo.h"


XSensorInfo::XSensorInfo()
: nQuestID(0)
{
	static_cast<void>(strGroupName.empty());
}

XSensorInfo::~XSensorInfo()
{
	static_cast<void>(strGroupName.empty());
}
