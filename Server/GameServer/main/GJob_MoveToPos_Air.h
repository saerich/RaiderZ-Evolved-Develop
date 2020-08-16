#ifndef _GJOB_MOVE_TO_POS_AIR_H_
#define _GJOB_MOVE_TO_POS_AIR_H_

#include "GJob_Movement.h"

/// ���� ��ġ�� ���Ƽ� �̵��ϴ� �ൿ ���
class GJob_MoveToPos_Air : public GJob_Movement
{
private:
	vec3			m_vFinalTarPos;				///< ���� ������
	float			m_fLimitSumOfPathLength;	///< ���� ���������� ���µ� ���ɼ� �ִ� ���� ������ ���Ѱ�
	bool			m_bRun;
	float			m_fSpeed;
	bool			m_bPathFindFailed;
	bool			m_bNowRetreat;

protected:
	virtual bool OnStart();
	virtual GJobResult OnRun(float fDelta);
	virtual bool OnComplete();
	virtual bool OnCancel();
	virtual void OnSuspend();
	virtual void OnResume();

public:
	GJob_MoveToPos_Air(GModuleAI* pmAI, vec3 vTarPos, float fSpeed, bool bRun, bool bNowRetreat, float fLimitSumOfPathLength);
	virtual ~GJob_MoveToPos_Air();
	DECLARE_JOB_ID(GJOB_MOVE_TO_POS_AIR);
	virtual const char* GetJobName() { return "MoveToPosAir"; }
};


#endif //_GJOB_MOVE_TO_POS_AIR_H_