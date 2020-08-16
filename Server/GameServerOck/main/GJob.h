#ifndef _GJOB_H
#define _GJOB_H

#include <queue>
#include <list>
using namespace std;

#include "GJobID.h"
#include "GMessage.h"
#include "GTask_Broker.h"
#include "GTaskID.h"

class GModuleAI;
class GJobMgr;
class GEntityNPC; 

struct GJobAttribute
{
	bool bClearJobListWhenCanceled;
	bool bClearJobListWhenTimedout;
	bool bAutoDeletable;

	GJobAttribute()
	{
		 bClearJobListWhenCanceled = false;
		 bClearJobListWhenTimedout = false;
		 bAutoDeletable = true;
	}
};

/// GJob
//		#��Ȱ: ���� Job���� ���� Ŭ����
class GJob 
{
private:
	double				m_fDurationTime;
	MUID				m_uidOwner;
	
	bool UpdateDuration(float fDelta);								///< �� ��ȿ�ð� ���� (��ȿ�ð��� ����Ǹ� true�� ��ȯ)
	GJobResult UpdateTask(float fDelta);

protected:
	friend GJobMgr;

	GTask*				m_pRunningTask;
	GModuleAI*			m_pOwner;
	GTaskBroker			m_TaskBroker;
	bool				m_bSuspended;
	
protected:
	virtual bool OnStart() = 0;							///< Job ���۽�
	virtual GJobResult OnRun(float fDelta);				///< Job ����
	virtual void OnComplete()	{}						///< Job �Ϸ��
	virtual void OnCancel()		{}						///< Job ��ҽ�
	virtual void OnTimeout()	{}						///< Job ����ð� ����
	virtual void OnSuspend();							///< Job �ߴܽ�
	virtual void OnFinished();							///< Job �����
	virtual void OnResume();							///< �ߴܵ� Job �簳��

	virtual void OnTaskComplete(GTaskID nTaskID)		{}	///< �½�ũ �Ϸ��
	virtual void OnTaskCancel(GTaskID nTaskID)			{}	///< �½�ũ ��ҽ�
	bool SetRunningTask(GTask* pTask);

public:
	GJobAttribute		Attr;
public:
	GJob(GModuleAI* pOwner);
	virtual ~GJob();

	bool Start();
	GJobResult Run(float fDelta);
	void Complete();
	void Cancel();
	void TimeOut();

	void SetDurationTime(double fTime);					///< �½�ũ ����ð� ���� (second����)
	double GetDurationTime()							{ return m_fDurationTime; }
	void Suspend();	
	void Resume();	

	virtual	 bool IsInstant()		{ return false; }
	virtual	 bool IsSessionJob()	{ return false; }
			 bool IsSuspended()		{ return m_bSuspended; }
	virtual	bool IsCancelable()		{ return true; }

			 MUID GetOwnerUID();

	GModuleAI* GetOwner() { return m_pOwner; }
	GEntityNPC* GetOwnerNPC();
	const GEntityNPC* GetOwnerNPC() const;
	virtual GJobID GetID() = 0;
	virtual string GetParamString() const = 0;

	virtual const wchar_t* GetJobName() const { return L"noname_job"; }
	const wchar_t* GetTaskName();
	GTask*		GetTask();
};


#define DECLARE_JOB_ID(_ID)	GJobID GetID() { return _ID; }	\
							virtual const wchar_t* GetJobName() const override { return L#_ID; }

#endif