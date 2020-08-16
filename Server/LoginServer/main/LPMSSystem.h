#pragma once

#include "PMPMSConnector.h"

class PMPMSObject;

class LPMSSystem : public PMPMSConnector
{
public:
	LPMSSystem(void);
	~LPMSSystem(void);

	// PMS �ݹ� ������Ʈ ���ϱ�
	virtual IPMSObject* GetPMSObject() override;
private:
	PMPMSObject* pCallbackObj;
};
