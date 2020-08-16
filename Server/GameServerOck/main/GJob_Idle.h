#ifndef _GJOB_DELAY_H
#define _GJOB_DELAY_H

#include "MMemPool.h"
#include "GJob.h"

/// �ƹ��ϵ� ���ϰ� ������ �½�ũ
class GJob_Idle : public GJob, public MMemPool<GJob_Idle>
{
private:

protected:
	float		m_fIdleTime;

	virtual bool OnStart();
	virtual GJobResult OnRun(float fDelta);
	virtual void OnComplete() {}
	virtual void OnCancel() {}
public:
	DECLARE_JOB_ID(GJOB_IDLE);
	virtual string GetParamString() const override;

	GJob_Idle(GModuleAI* pmAI, float fIdleTime);
	virtual ~GJob_Idle();

	float	GetIdleTime()		{ return m_fIdleTime; }
};







#endif