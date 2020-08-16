#ifndef _GTASK_DELAY_H
#define _GTASK_DELAY_H


#include "GTask.h"
#include "MTime.h"

/// �ƹ��ϵ� ���ϰ� ������ �½�ũ
class GTask_Idle : public GTask, public MTestMemPool<GTask_Idle>
{
private:

protected:
	MRegulator	m_rgrWaiter;

	virtual bool OnStart();
	virtual GTaskResult OnRun(float fDelta);
	virtual void OnComplete() {}
	virtual void OnCancel() {}
public:
	DECLARE_TASK_ID(GTASK_IDLE);

	GTask_Idle(GEntityNPC* pEntity, float fIdleTime);
	virtual ~GTask_Idle();
	virtual const wchar_t* GetTaskName() { return L"Idle"; }
};







#endif