#pragma once

namespace rs3 {

class RBackgroundWorker;
class RBackgroundJobSafeQueue;

//////////////////////////////////////////////////////////////////////////
//
// ��׶��� �۾��� ����
// Ƽ���� ������ �ùٸ� �۾����� ��Ŀ�����尡 �Ǵ��Ѵ�.
//
class RS_API RBackgroundWorkUnit
{
	friend RBackgroundWorker;
	friend RBackgroundJobSafeQueue;
public:
	enum WorkState
	{
		WS_INVALID,				// �۾� ����
		WS_SLEEPING,			// ������
		WS_RUNNING,				// ���� ��
		WS_FINISHED				// �۾� �Ϸ�
	};
	WorkState				GetState() { return m_nState; }

	void					UpdateCompleteWork();			///< ���ξ����忡�� ȣ��
	void					BackgroundWork();				///< ��׶��� ��Ŀ�����忡�� �۾�
	void					WorkInTheForeground();			///< ���׶��忡�� ��� �ε��� ����

	inline bool				IsWorkable();					///< ��� ������ WorkUnit���� ����
	inline bool				IsBackgroundWork();				///< ��׶���� ����Ǵ��� ����
	inline bool				IsFinished();					///< �۾��� ����Ǿ����� ����
	inline bool				IsSleeping();

	virtual const char*		GetName()			{ return "base"; }	///< WorkUnit �̸� ��ȯ
	virtual int				GetDebuggingID() { return 0; }	///< ������. - �׽�Ʈ�� ���� �� ���ŵ�

	void					InvalidateWork();
	void					InvalidateWorkWithNoLock();

protected:
	RBackgroundWorkUnit();
	virtual ~RBackgroundWorkUnit();
	void					SetState(WorkState nState) { m_nState = nState; }

	RBackgroundWorker*		m_pBackgroundWorker;
	WorkState				m_nState;

			void			SetPriority(int priority)	{	m_pTicket->m_nPriority = priority;}
	virtual int				CalcPriority()			= 0;	///< ��׶��� �۾��� �켱����
	virtual void			OnBackgroundWork()		= 0;	///< ��׶��� ��Ŀ�����忡�� �۾�
	virtual void			OnUpdateCompleteWork()	= 0;	///< ���ξ����忡�� ȣ��

	void					FinishWork();					///< �۾��� �ùٸ��� �Ϸ�Ǿ�����, OnUpdateCompleteWork������ ȣ������� �Ѵ�.
	
	inline void				SetRecursiveChainedFlag();		///< BackgroundWork, UpdateComplete�� ������ �����Ϸ� �� �� �����.
private:
	class BG_TICKET
	{
	private:
		bool						m_bExpired;
		RBackgroundWorkUnit*		m_pBackgroundWorkUnit;
		BG_TICKET() : m_bExpired(false), m_nPriority(0), m_pBackgroundWorkUnit(NULL) {}
	public:
		BG_TICKET(RBackgroundWorkUnit* pWorkUnit) : m_bExpired(false), m_nPriority(0), m_pBackgroundWorkUnit(pWorkUnit) {}
		~BG_TICKET(){}
		void InvalidateWork() 
		{ 
			m_bExpired = true;
			m_pBackgroundWorkUnit = NULL; 
		}

		int							m_nPriority;
		RBackgroundWorkUnit*		GetWorkUnit() { return m_pBackgroundWorkUnit; }
		bool IsExpired() { return m_bExpired; }
	};
	BG_TICKET*				m_pTicket;
	bool					m_bRecursiveChained;
	bool					m_bBackgroundWorkFinished;
};
//----------------------------------------------------------------------------------------------------
inline bool RBackgroundWorkUnit::IsWorkable()
{
	return m_pTicket != NULL;
}
inline bool RBackgroundWorkUnit::IsBackgroundWork() 
{ 
	return m_pBackgroundWorker != NULL; 
}

inline void RBackgroundWorkUnit::SetRecursiveChainedFlag()
{
	m_bRecursiveChained = true;
}

inline bool RBackgroundWorkUnit::IsFinished()
{
	return (m_nState == WS_FINISHED);
}


//----------------------------------------------------------------------------------------------------
}
