#pragma once

#include "XModule.h"

class XModuleEntity;

//////////////////////////////////////////////////////////////////////////
/// ������Ʈ�� ����Ÿ�� ���� - npc, player�� �θ� Ŭ����
class XModuleBaseData : public XModule
{
	DECLARE_ID(XMID_DATA);
protected:
public:
	XModuleBaseData(XObject* pOwner=NULL) {}
	virtual ~XModuleBaseData(void) {}
};

