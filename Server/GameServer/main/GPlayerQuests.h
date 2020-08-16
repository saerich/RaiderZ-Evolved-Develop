#ifndef _G_PLAYER_QUESTS_H_
#define _G_PLAYER_QUESTS_H_

#include "CSDef_Quest.h"

class GPlayerQuest;
class GEntityPlayer;

typedef map<int, GPlayerQuest*>	MAP_PLAYER_QUEST;	// ���� �������� ����Ʈ
typedef map<int, int>			MAP_DONE_COUNT;		// �ѹ��̶� �������� �ִ� ����Ʈ�� �Ϸ� Ƚ���� ����ȴ�.
													// ��, �ϷḦ �ѹ��� ������ ������ �Ϸ� Ƚ���� 0�̴�.
													// !!�Ϸ� Ƚ�� ������ϱ�� ��. Ƚ���� �ǹ̰� ����.
													
static const uint8 INVALID_QUSET_SLOT_ID	= 255;

class GPlayerQuests : public MTestMemPool<GPlayerQuests>
{
private:
	MAP_PLAYER_QUEST	m_mapDoingQuest;
	MAP_DONE_COUNT		m_mapDoneCount;
	GEntityPlayer*		m_pOwner;
	bool				m_bSlot[MAX_QUEST_INSTACNE_COUNT];

public:
	GPlayerQuests(GEntityPlayer* pOwner);
	virtual	~GPlayerQuests();

	void				Clear();
	void				ClearDoneCount();
	MAP_PLAYER_QUEST&	GetQuestContainer()		{ return m_mapDoingQuest; }
	MAP_DONE_COUNT&		GetDoneCountContainer()	{ return m_mapDoneCount; }

	bool			Has(int nQuestID);
	GPlayerQuest*	Get(int nQuestID);
	GPlayerQuest*	Insert(const uint8 nSlotID, int nQuestID, const wstring& strAcceptDt, const wstring& strExpiDt);
	void			Cancel(int nQuestID);	
	void			Update(float fDelta);
	void			Done(int nQuestID);

	bool			IsDoingQuest(int nQuestID);
	bool			IsDoingQuest(const vector<int>& vecQuestID);
	bool			IsCompleteQuest(int nQuestID);
	bool			IsDoneQuest(int nQuestID);
	bool			IsDBInsert(int nQuestID);

	int				GetQuestNeedItemQuantity(const vector<int>& vecQuestID, int nItemID);	

	int				GetDoneCount(int nQuestID);

	void			MakeActiveQuestID(vector<int>* pVecQuestID) const;
	uint8			FindAndMarkingEmptySlot();
	void			ResetSlot(const uint8 nSlotID);
	bool			IsValidSlotID(const uint8 nSlotID);
	bool			SlotMarking(const uint8 nSlotID);
};

#endif
