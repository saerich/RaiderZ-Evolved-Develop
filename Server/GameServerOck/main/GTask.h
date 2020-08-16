#ifndef _GTASK_H
#define _GTASK_H

#include <queue>
#include <list>
using namespace std;

#include "GTaskID.h"
#include "GMessage.h"

class GEntityNPC;
class GJob;

/// GJob
/// - ��Ȱ: ���� Task���� ���� Ŭ����
/// - ����: GEntitNPC, GNPCInfo
class GTask
{
	friend GJob;
private:
	GJob*				m_pParent;
protected:
	GEntityNPC*			m_pOwner;
		
	virtual bool OnStart() = 0;							///< �½�ũ ���۽�
	virtual GTaskResult OnRun(float fDelta) = 0;		///< �½�ũ ����
	virtual void OnComplete() = 0;						///< �׽�ũ �Ϸ��
	virtual void OnCancel() = 0;						///< �½�ũ ��ҽ�
	virtual void OnSuspend()	{}						///< �½�ũ �ߴܽ�
	virtual void OnResume()		{}						///< �ߴܵ� �׽�ũ �簳��
public:
	GTask(GEntityNPC* pOwner);
	virtual ~GTask() {}
	virtual GMessageResult	OnMessage(GMessage& msg)	{ return MR_FALSE; }

	bool Start();
	GTaskResult Run(float fDelta);
	void Complete();
	void Cancel();
	GEntityNPC* GetOwner() { return m_pOwner; }
	virtual GTaskID GetID() = 0;
	virtual const wchar_t* GetTaskName() = 0;

	virtual bool Reset(GTask* pNewTask);				///< ret: ���µƴ��� ����
};


#define DECLARE_TASK_ID(_ID)	GTaskID GetID() { return _ID; }

#endif