#include "stdafx.h"
#include "LTimeoutWatcher.h"
#include "LTimeoutHandler.h"
#include "LGlobal.h"
#include "LSystem.h"

LTimeoutWatcher::LTimeoutWatcher()
: m_nTimeoutPeriod(0)
, m_nTimeOverflowBalance(UINT_MAX)
{
	m_itPlayerTimeout = m_mapPlayerTimeout.end();
}

LTimeoutWatcher::~LTimeoutWatcher()
{
	// do nothing
}

void LTimeoutWatcher::AddPlayer(MUID uidPlayer, LTimeoutHandler* pHandler)
{	
	uint32 nTime = gsys.pSystem->GetNowTime();

	TimeoutInfo timeoutInfo;
	timeoutInfo.m_nTime = nTime + m_nTimeoutPeriod;
	timeoutInfo.m_pHandler = pHandler;

	m_mapPlayerTimeout.insert(make_pair(uidPlayer, timeoutInfo));
}

void LTimeoutWatcher::RemovePlayer(MUID uidPlayer)
{
	WatchMap::iterator it = m_mapPlayerTimeout.find(uidPlayer);

	if (it == m_mapPlayerTimeout.end())
	{
		return;
	}

	if (it == m_itPlayerTimeout)
	{
		m_itPlayerTimeout = m_mapPlayerTimeout.erase(it);
	}
	else
	{
		m_mapPlayerTimeout.erase(it);
	}
}

void LTimeoutWatcher::UpdateTimeout(void)
{
	// ������ �˻����� �ʴ´�.
	if (m_mapPlayerTimeout.empty())
	{
		return;
	}


	// �˻� ��������, ó������ �ٽ��Ѵ�.
	if (m_itPlayerTimeout == m_mapPlayerTimeout.end())
	{
		m_itPlayerTimeout = m_mapPlayerTimeout.begin();
	}


	// �ѹ��� �� �÷��̾ �˻��Ѵ�.
	TimeoutInfo timeoutInfo = m_itPlayerTimeout->second;	
	if (IsTimeout(timeoutInfo.m_nTime))
	{	
		MUID uidPlayer = m_itPlayerTimeout->first;

		// ���� ��, ��������. LServer���� RemovePlayer()�� ȣ��Ǿ� m_itPlayerTimeOut�� ����� ���ۿ� ����.
		m_itPlayerTimeout = m_mapPlayerTimeout.erase(m_itPlayerTimeout);
		
		if (timeoutInfo.m_pHandler != NULL)
		{
			timeoutInfo.m_pHandler->OnTimeout(uidPlayer);			
		}		
	}
	else
	{
		m_itPlayerTimeout++;
	}
}

bool LTimeoutWatcher::IsTimeout(uint32 nTime)
{
	uint32 nCurrentTime = gsys.pSystem->GetNowTime();			

	// timeGetTime �����÷ο� ��������, �����÷ο츦 ����Ͽ� ����Ѵ�.	
	if (m_nTimeOverflowBalance < nCurrentTime)
	{
		uint32 nOverflowGuardTime = nCurrentTime + m_nTimeoutPeriod;
		return nOverflowGuardTime < nTime && nTime <= nCurrentTime;
	}
	else
	{
		return nTime <= nCurrentTime;
	}
}

void LTimeoutWatcher::SetTimeoutPeriod(uint32 nMilliSeconds)
{
	m_nTimeoutPeriod = nMilliSeconds;
	m_nTimeOverflowBalance = UINT_MAX - nMilliSeconds;
}
