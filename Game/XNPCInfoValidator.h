#pragma once

#include "XBaseValidator.h"

class XNPCInfo;

/// �ŷ�Ʈ ���� ����
class XNPCInfoValidator : public XBaseValidator
{
private:
	bool CheckTitle(XNPCInfo* pNPCInfo);
public:
	virtual bool Check();
};
