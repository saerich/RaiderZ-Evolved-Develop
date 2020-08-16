#ifndef _GTASK_COMBAT_MOVE_H
#define _GTASK_COMBAT_MOVE_H


#include "GTask.h"


class GPathFinder;

// #Design Issue: ��� ��ã�� ����(�ΰ����� ����)�ڵ�� Job���� �̵���Ų��.  �� ��쿡 CombatMove�� ������ ��ġ�� ������Ű�� ���� �Ѵ�. (�̸��� �ٲ۴�)
//					�������� �Ǵ��� Job(�ΰ�����)���� ���, �������� ������ Task���� �ٷ��.  ��Ģ�� ������ ��Ű���� �Ѵ�.

/// ������ �̵� 
class GTask_CombatMove : public GTask
{
private:
	float					m_fRotateSpeed;			///< ȸ�� �ӵ�
	vec3					m_vTargetPos;
	vec3					m_vAdvancePos;			///< ���������� �̵��Ǵ� ��ǥ ��ǥ
	int						m_nOldPathCounter;
	bool					m_bOldMoveStopByContext;

private:
	bool		_FindPath();
	void		_MoveStop();
	void		_SyncPos();
	GTaskResult _AdvancedAlongPath(float fDelta, bool& boutMoveStopByContext, bool& boutNewPathLine);
	void		_RouteStatus(bool bMoveStopByContext, bool bNewPathLine);
	void		_RouteDebugInfo();
protected:
	virtual bool OnStart();
	virtual GTaskResult OnRun(float fDelta);
	virtual void OnComplete();
	virtual bool OnCancel();
public:
	DECLARE_TASK_ID(GTASK_COMBAT_MOVE);

	GTask_CombatMove(GEntityNPC* pEntity, vec3 vTargetPos);
	virtual ~GTask_CombatMove();

	virtual const char* GetTaskName() {return "CombatMove";}

	void Reset(vec3 vTarPos);
	vec3 GetTargetPos()			{ return m_vTargetPos; }
};







#endif