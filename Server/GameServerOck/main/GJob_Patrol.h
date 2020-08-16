#ifndef _GJOB_PATROL_H
#define _GJOB_PATROL_H

#include "MMemPool.h"
#include "GJob.h"
#include "CSTriggerInfo.h"

class GJob_Patrol : public GJob, public MMemPool<GJob_Patrol>
{
private:
	vector<MARKER_INFO> 	m_MarkerList;
	size_t			m_nCurrMarkerIndex;
	bool			m_bReverseOrder;
	bool 			m_bComplete;
	bool			m_bLastDoingIdle;
	PATROL_TYPE		m_PatrolType;
	bool			m_bUseIdleColt;
private:
	// ��Ŀ �ε����� ���������� �Դ��� ����
	bool IsEndedMarker();

	// ��Ŀ ����Ʈ �߰�
	void InsertFromMarkerIDList(MARKER_LIST& listMarkers);
	
	// �̵��� ��Ŀ �ε��� ����, ��ȯ���� �̵��� ��� �������� ����
	bool IncreaseMarkerIndex();
protected:
	virtual bool OnStart();
	virtual GJobResult OnRun(float fDelta);
	virtual void OnTaskComplete(GTaskID nTaskID);
	virtual void OnCancel();
			bool OnIdle();
public:
	DECLARE_JOB_ID(GJOB_PATROL);
	virtual string GetParamString() const override;

	GJob_Patrol(GModuleAI* pmAI, MARKER_LIST& MarkerList, PATROL_TYPE PatrolType);
	GJob_Patrol(GModuleAI* pmAI, const vector<MARKER_INFO>& MarkerList, PATROL_TYPE PatrolType);
	virtual ~GJob_Patrol();

	bool GetCurrentPatrolMarker(MARKER_INFO& Marker);
	bool AdvanceMoveMarker();
	void RunAdvance();
	void CallScript();
};


#endif // _GJOB_MOVE_TO_POS_H