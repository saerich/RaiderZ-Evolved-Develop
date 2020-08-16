#pragma once

class LTimeoutHandler;

class LTimeoutWatcher
{
private:
	struct TimeoutInfo
	{
		uint32 m_nTime;
		LTimeoutHandler* m_pHandler;
	};

	typedef map<MUID, TimeoutInfo> WatchMap;

public:
	LTimeoutWatcher();
	virtual ~LTimeoutWatcher();

	void AddPlayer(MUID uidPlayer, LTimeoutHandler* pHandler);
	void RemovePlayer(MUID uidPlayer);
	void UpdateTimeout(void);
	bool IsTimeout(uint32 nTime);

	void SetTimeoutPeriod(uint32 nMilliSeconds);

private:
	WatchMap m_mapPlayerTimeout;
	WatchMap::iterator m_itPlayerTimeout;
	uint32 m_nTimeoutPeriod;
	uint32 m_nTimeOverflowBalance;					// Ÿ�Ӿƿ��� �ڷ��� �����÷ο츦 ����Ű�� ��谪
};
