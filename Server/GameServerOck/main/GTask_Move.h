#pragma once 

#include "GTask.h"
#include "GMoveStrategy.h"

// Design Issue: ��� ��ã�� ����(�ΰ����� ����)�ڵ�� Job���� �̵���Ų��.  �� ��쿡 CombatMove�� ������ ��ġ�� ������Ű�� ���� �Ѵ�. (�̸��� �ٲ۴�)
//					�������� �Ǵ��� Job(�ΰ�����)���� ���, �������� ������ Task���� �ٷ��.  ��Ģ�� ������ ��Ű���� �Ѵ�.

class GModuleMovable;


class GTask_Move : public GTask, public MTestMemPool<GTask_Move>
{
public:
	DECLARE_TASK_ID(GTASK_MOVE);

	GTask_Move(GEntityNPC* pEntity, vec3 vTargetPos, float fLeastGap, GTaskMoveStrategy nStrategyType=MS_STRAIGHT);
	virtual ~GTask_Move();

	virtual const wchar_t* GetTaskName();

	vec3				GetGoalPos()			{ return m_vGoalPos; }

	// �������� ������ ���� ����
	void SetFinishDir(vec3 vFinishDir);

private:
	virtual bool OnStart();
	virtual GTaskResult OnRun(float fDelta);
	virtual void OnComplete();
	virtual void OnCancel();
	virtual void OnSuspend();
	virtual void OnResume();
	virtual bool Reset(GTask* pNewTask);

private:
	GModuleMovable* GetMovable() const;

private:
public:
	vec3				m_vGoalPos;
	float				m_fLeastGap;
	MovingWay			m_nWay;
	int					m_nChangeWayCounter;
	GTaskMoveStrategy	m_nStrategyType;
	vec3				m_vFinishDir;	// �������� ������ ����
};



