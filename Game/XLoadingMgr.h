#pragma once

#include "XLoadingChecker.h"

class XLoadingStarter;
class XLoadingCompleter;

/// �ε����� �̺�Ʈ ID
enum XLoadingEventID
{
	XLOADING_EVT_ON_FIELD_START_GAME = 1		//  �����κ��� MC_FIELD_START_GAME Ŀ�ǵ� ����
};

/// �ε� ������
class XLoadingMgr
{
public:
	enum LoadingState
	{
		STATE_READY = 0,
		STATE_STARTED,
		STATE_WORLD_LOADED,
		STATE_COMPLETED
	};
protected:
	LoadingState					m_nState;
	XLoadingStarter*				m_pStarter;
	XLoadingCompleter*				m_pCompleter;
	vector<XLoadingChecker*>		m_vecCheckers;

	void WorldLoadingFinish();
	void Complete();					///< �ε��� �� �Ϸ�Ǹ� ȣ��
public:
	XLoadingMgr(XLoadingStarter* pStarter = NULL, XLoadingCompleter* pCompleter = NULL);
	virtual ~XLoadingMgr();

	void AddChecker(XLoadingChecker* pChecker);			///< �ε��� �ٵǾ������� üũ�ϴ� Ŭ���� �߰�
	void ClearCheckers();
	void Start();										///< ������ �� ȣ��
	bool CheckCompleted();								///< �ε��� �Ϸ�Ǿ����� ���� Ȯ��
	void Event(XEvent& evt);

	bool IsLoading();									///< ù��° �ε����� ����
	LoadingState GetState() { return m_nState; }
};