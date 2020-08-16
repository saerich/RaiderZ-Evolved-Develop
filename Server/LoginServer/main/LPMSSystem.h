#pragma once

#include "PMPMSConnector.h"

class PMPMSObject;

class LPMSSystem : public PMPMSConnector
{
public:
	LPMSSystem(void);
	~LPMSSystem(void);

	// PMS 콜백 오브젝트 구하기
	virtual IPMSObject* GetPMSObject() override;
private:
	PMPMSObject* pCallbackObj;
};
