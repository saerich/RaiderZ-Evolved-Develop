#pragma once

#include "MThread2.h"

namespace cml2 {

/// Future Pattern���� ����ϴ� �����

template <typename ResultT>
class MFutureResult
{
private:
	MCriticalSection		m_csLock;
	MAutoSignalEvent		m_ResultReadySignal;
	bool m_bResultReady;
	ResultT m_Result;
public:
	MFutureResult() :  m_bResultReady(false) {}

	// find whether result can be retrieved yet
	bool IsResultReady() const
	{
		MCriticalSection::Guard lock(m_csLock);
		return m_bResultReady;
	}

	// retrieve result, blocking if not yet available
	ResultT& GetResult()
	{
		MCriticalSection::Guard lock(m_csLock);

		if(m_bResultReady == false)
		{
		  // ������� ���õ� ������ ��ٸ���.
		  m_ResultReadySignal.Wait();
		}
		return m_Result;
	}

	void SetResult(const ResultT& result)
	{
		MCriticalSection::Guard lock(m_csLock);

		m_Result = result;
		m_bResultReady = true;

		// ������� ��ٸ��� ���� �����忡�� �˷��ش�.
		m_ResultReadySignal.Signal();
	}

};
 

} // namespace cml2