#pragma once

#include "PMPMSConnector.h"

class GPMSObject;

class GPMSSystem : public PMPMSConnector, public MTestMemPool<GPMSSystem>
{
public:
	GPMSSystem(void);
	~GPMSSystem(void);

	// PMS �ݹ� ������Ʈ ���ϱ�
	virtual IPMSObject* GetPMSObject() override;
private:
	GPMSObject* pCallbackObj;
};
