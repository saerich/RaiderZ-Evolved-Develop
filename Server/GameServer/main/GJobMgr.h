#ifndef _GJOB_MGR_H
#define _GJOB_MGR_H

#include "GJobID.h"
#include "GMessage.h"

class GEntityNPC;
class GJob;
class GTask;
class GModuleAI;

/// GJobMgr
//		#��Ȱ: AI�� ���� �ο����� Job�� ó���Ѵ�.
//				Job�� Task���� ���� ��ǥ�� �ϼ��Ѵ�.
//		#����: TaskMgr�� ���� ������� �ʴ´�.  �ʿ��ϴٸ� ModuleJob�� ���� �̿��Ѵ�.
class GJobMgr : public MTestMemPool<GJobMgr>
{
	friend GModuleAI;
public:
	GJobMgr(GModuleAI* pOwner=NULL);
	~GJobMgr();

	void Clear(bool bCancelCurJob=true);
	void Push(GJob* pJob);
	void PushFront(GJob* pJob);
	void Cancel();						///< ���� �½�ũ�� ����Ѵ�.
	GJobResult Run(float fDelta);

	// �� Ŭ���� ����� ����
	void EnableClearJob(bool bEnable)	{ m_bEnableClearJob = bEnable; }

	void CancelCurrJob();

	// interface functions
	bool IsEmpty() { return (m_pCurrJob==NULL && m_Jobs.empty()); }
	int GetCount() { return (int)m_Jobs.size(); }
	GJob* GetCurrJob();
	GJob* GetJob(size_t nIndex);
	GTask* GetCurrTask();
	GJobID GetCurrJobID();
	deque<GJob*> GetJobs() const;
	void Suspend();
	void Resume();

private:
	bool IsFinished(GJobResult nResult);
	void OnJobFinished(GJob* pJob, GJobResult nResult );
	void PostMessage(GMessage& m);

protected:
	bool PopJob();
	bool PopAndStartJob();
	void CompleteCurrJob();
	void TimeoutCurrJob();
private:
	list<GJob*>				m_Jobs;
	GJob*					m_pCurrJob;
	bool					m_bEnableClearJob;
	bool					m_bLazyDeleteCurJob;
	GModuleAI*				m_pOwner;
};

#endif // _XTASK_MGR_H
