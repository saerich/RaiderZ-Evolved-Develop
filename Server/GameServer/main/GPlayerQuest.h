#ifndef _G_PLAYER_QUEST_H_
#define _G_PLAYER_QUEST_H_

class GPlayerQObjective;
class GGlueQuest;
class GNullGlueQuest;
class GQuestInfo;
class GEntityPlayer;

enum QUEST_STATE_TYPE;

typedef map<int, GPlayerQObjective*>	MAP_PLAYERQOBJECTIVE;

class GPlayerQuest : public MTestMemPool<GPlayerQuest>
{
private:
	GQuestInfo*		m_pQuestInfo;
	GEntityPlayer*	m_pOwner;

	uint8	m_nSlotID;
	MAP_PLAYERQOBJECTIVE	m_mapPlayerQObjective;		///< ����Ʈ�� ���� �޼� ����
	int		m_nVar;										///< ����Ʈ ������������ ���Ǵ� ����
	QUEST_STATE_TYPE		m_nState;					///< ����Ʈ ����
	uint32	m_nAcceptTime;								///< ����Ʈ ���� �ð�
	uint32	m_nLimitTime;								///< ����Ʈ �Ϸ� ���� �ð�
	wstring m_strAcceptDt;								///< ����Ʈ ���� �ð�.
	wstring m_strExpiDt;								///< ����Ʈ ���� �ð�.

	GGlueQuest*	m_pDelegator;

public:
	typedef GNullGlueQuest NullType;

	GPlayerQuest();										///< Glue�� ���ε� �ϱ� ���� ������
	GPlayerQuest(GQuestInfo* pQuestInfo, GEntityPlayer* pOwner, const uint8 nSlotID, uint32 nAcceptTime, const wstring& strAcceptDt, const wstring& strExpiDt);
	virtual	~GPlayerQuest();

	void Clear();
	MAP_PLAYERQOBJECTIVE& GetQObjectiveContainer() { return m_mapPlayerQObjective; }
	GQuestInfo*	 GetInfo()					{ return m_pQuestInfo; }
	GEntityPlayer* GetOwner()				{ return m_pOwner; }
	void Update(float fDelta);

	GPlayerQObjective* Get(int nObjectiveID);
	
	void UpdateQuestVar(int nVar);
	int  GetQuestVar() { return m_nVar; }

	void UpdateQObjective(int nObjectiveID, int nProgress, bool& bChangeToComplete, bool& bChageToIncomplete);	///< ���� �޼� ���¸� ���� �Ѵ�.
	bool IsQObjectiveUpdatableVar(int nObjectiveID, int nVar, int& nUpdateProgress);							///< ���� �޼� ���¸� ���� ��ų���� ������?

	void AllQObjectiveComplete();
	bool IsAllQObjectiveComplete();	

	void Fail();
	
	void Complete();
	void Incomplete();

	bool IsComplete();
	bool IsFail();
	bool IsActive();

	int GetQuestNeedItemQuantity(int nItemID);
	vector<pair<int,INT>> GetQuestNeedItem();

	uint32	GetAcceptTime() { return m_nAcceptTime; }
	uint32	GetLimitTime() { return m_nLimitTime; }	
	QUEST_STATE_TYPE GetState();
	
	void CreateDelegator();	
	GGlueQuest* GetDelegator(void);

	uint8 GetSlotID();
	wstring& GetAcceptDt();	
	wstring& GetExpiDt();
};



#endif//_G_PLAYER_QUEST_H_