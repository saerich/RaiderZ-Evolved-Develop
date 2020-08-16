#ifndef _X_PLAYER_QUEST_H_
#define _X_PLAYER_QUEST_H_

class XQuestInfo;
class XQObjectiveInfo;

// �÷��̾� ����Ʈ�� ����(DB������ ���)
enum PLAYER_QUEST_STATE
{
	PQS_NONE		= 0,
	PQS_NOEXIST		= 1,	// ����Ʈ�� ���� ����
	PQS_EXIST		= 2,	// ����Ʈ�� �޾����� ���� ��Ȱ��ȭ ����
	PQS_COMPLETED	= 3,	// ����Ʈ�� �޾Ұ� Ȱ��ȭ ����

	PQS_MAX
};

/// �÷��̾��� ����Ʈ ���� ����
// TODO : �׳� CSQObjectiveInfo �� ��� ������ �ȵǳ���? [3/18/2008 madduck]
struct XPlayerQObjective
{
public:
	XQObjectiveInfo* m_pQObjectiveInfo;

	int nProgress;
	bool bComplete;

	XPlayerQObjective(XQObjectiveInfo* pQObjectiveInfo);
};

typedef map<int, XPlayerQObjective*>	MAP_PLAYERQOBJECTIVE;


/// �÷��̾��� �������� ����Ʈ ����
class XPlayerQuest
{
public:
	XQuestInfo*		m_pQuestInfo;

	MAP_PLAYERQOBJECTIVE	m_mapPlayerQObjective;		///< ����Ʈ�� ���� �޼� ����
	QUEST_STATE_TYPE		m_nState;
	uint32	m_nAcceptTime;								///< ����Ʈ ���� �ð�
	uint32	m_nLimitTime;
	bool	bChallengerQuest;

public:
	XPlayerQuest(XQuestInfo* pQuestInfo);
	virtual	~XPlayerQuest();

	void Clear();

	XPlayerQObjective* Get(int nObjectiveID);
	bool UpdateObjective(int nObjectiveID, int nProgress, bool bCompleted);	///< ���� �޼� ���¸� ���� �Ѵ�.
	uint32 GetRemainTime();
	bool	IsTimeAttackType()
	{
		if (m_nLimitTime == NO_LIMIT_TIME) return false;

		return true;
	}
	bool IsComplete()
	{
		return (QST_COMPLETE == m_nState);
	}
	bool IsFail()
	{
		return (QST_FAIL == m_nState);
	}
};

typedef map<int, XPlayerQuest*>		MAP_PLAYERQUEST;

/// �÷��̾ ������ �ִ� ����Ʈ�� ���µ�
class XPlayerQuests
{
public:
	MAP_PLAYERQUEST		m_mapPlayerQuest;
	void			Delete(int nQuestID);

public:
	XPlayerQuests()	{}
	virtual	~XPlayerQuests();

	void			Clear();

	XPlayerQuest*	Get(int nQuestID);
	bool			GetIndex( size_t nIndex, XPlayerQuest** ppOutQuest);

	size_t			GetCount() const		{ return m_mapPlayerQuest.size();	}

	XPlayerQuest*	Insert(int nQuestID);
	void			Cancel(int nQuestID);
	void			Reward(int nQuestID);
	
	bool			IsPlayerQuest(int nQuestID);
	const wchar_t*		GetQuestIDList(wstring& szText);
};


#endif//_X_PLAYER_QUEST_H_