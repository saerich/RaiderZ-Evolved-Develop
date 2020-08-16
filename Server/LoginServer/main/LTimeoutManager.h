#pragma once

class LTimeoutHandler;
class LTimeoutWatcher;
class LMoveServerTimeoutHandler;

class LTimeoutManager
{
public:
	LTimeoutManager();
	virtual ~LTimeoutManager();

	void InitWatcher(void);
	void Update(void);	
	void StopAllWatching(MUID uidPlayer);

	void StartWatchingSelMyCharTimeout(MUID uidPlayer, LTimeoutHandler* pHandler);
	void StopWatchingSelMyCharTimeout(MUID uidPlayer);
	void StartWatchingMoveServerTimeout(MUID uidPlayer);
	void StopWatchingMoveServerTimeout(MUID uidPlayer);

private:
	LTimeoutWatcher*			m_pSelMyCharWatcher;		///< �÷��̾� ĳ���� ���� Ÿ�Ӿƿ� ó����
	LTimeoutWatcher*			m_pMoveServerWatcher;		///< �÷��̾� �����̵� �������� Ÿ�Ӿƿ� ������
	LMoveServerTimeoutHandler*	m_pMoveServerTimeoutHandler;///< �ٸ� ������ �����̵Ǵ� ��û/���� Ŀ�ǵ� Ÿ�Ӿƿ� ������
};
