#pragma once

#include "XMotionEventListener.h"

class XMotionEventLister_NPC_IDLE : public XMotionEventListener
{
public:
	XMotionEventLister_NPC_IDLE() {}
	virtual ~XMotionEventLister_NPC_IDLE() {}
	virtual XMotion* OnPreEnter(XActor* pOwner);	///< �� ������� ������ ��		- false�� ��ȯ�ϸ� �� ������� ������ ��ҵ�
};
