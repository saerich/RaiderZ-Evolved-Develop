#ifndef _GJOB_MOVE_TO_POS_H
#define _GJOB_MOVE_TO_POS_H

#include "MMemPool.h"
#include "GJob.h"

/// ���� ��ġ�� �̵��ϴ� �ൿ ���
class GJob_MoveToPos : public GJob, public MMemPool<GJob_MoveToPos>
{
private:
	vec3			m_vTarPos;					///< ���� ������
	vec3			m_vTarDir;					///< ���� ������
	float			m_fLimitSumOfPathLength;	///< ���� ���������� ���µ� ���ɼ� �ִ� ���� ������ ���Ѱ�
	unsigned long	m_nMarkerID;				///< ���� �������� ��Ŀ�϶� ���

private:
	void CallScript();

protected:
	virtual bool OnStart();
	virtual GJobResult OnRun(float fDelta);
	virtual void OnComplete();	

public:
	DECLARE_JOB_ID(GJOB_MOVE_TO_POS);
	virtual string GetParamString() const override;

	GJob_MoveToPos(GModuleAI* pmAI, vec3 vTarPos, vec3 vTarDir, float fLimitSumOfPathLength = FLT_MAX, unsigned long nMarkerID=0);
	virtual ~GJob_MoveToPos();

	// for test
	vec3	GetTarPos()		{ return m_vTarPos; }
};


#endif // _GJOB_MOVE_TO_POS_H