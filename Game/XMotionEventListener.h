#pragma once

#include "XMotion.h"

class XActor;
class XMotion;

class XMotionEventListener : public XBaseMotionEventLister
{
public:
	XMotionEventListener() {}
	virtual ~XMotionEventListener() {}
	virtual XMotion* OnPreEnter(XActor* pOwner)	{ return NULL; }	///< �� ������� ������ ��		- NULL�� �ƴ� ������ ��ȯ�ϸ� �� ������� �����
	virtual void OnEnter(XActor* pOwner)		{}					///< �� ������� �ٲ� ��
	virtual void OnLeave(XActor* pOwner)		{}					///< �ٸ� ������� �ٲ� ��
	virtual void OnUpdate(XActor* pOwner)		{}					///< ������Ʈ
};
