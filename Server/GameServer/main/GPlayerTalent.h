#ifndef _G_PLAYER_TALENT_H_
#define _G_PLAYER_TALENT_H_

#include "GTalentInfo.h"

typedef set<int> SET_TALENTID;
class GEntityPlayer;
class GTalentInfo;


/// �÷��̾ ��� �ŷ�Ʈ��
class GPlayerTalent : public MTestMemPool<GPlayerTalent>
{
private:
	SET_TALENTID	m_setTalentID;			///< �н��� �ŷ�Ʈ
	GEntityPlayer*	m_pOwner;

	int				m_nRemainTP;			///< ���� TP

	MUID			m_nUsedTalentItemUID;	///< ����� �ŷ�Ʈ �������� UID

public:
	GPlayerTalent(GEntityPlayer* pOwner);
	virtual ~GPlayerTalent() {};

	void Insert(int nTalentID, bool bConsumeTP=true);	
	void Insert(GTalentInfo* pTalentInfo, bool bConsumeTP=true);
	void DeleteAll();	

	void InsertWithLowRankTalent(int nTalentID, bool bUseTP=true);	

	void AddTP(int nTP) { m_nRemainTP += nTP; }
	void SetTP(int nTP) { m_nRemainTP = nTP; }
	int GetTP() { return m_nRemainTP; }
	int GetTotalTP();

	SET_TALENTID& GetContainer() { return m_setTalentID; }
	int GetTrainedStyleTP(TALENT_STYLE nStyle);

	bool IsTrainedTalent(GTalentInfo* pTalentInfo);
	bool IsTrainedTalent(int nTalentID);
	bool IsTrainedGatherTalent(int nRank);
	bool IsTrainedTalentLine(int nTalentLine);

	bool IsEmpty();

	void EnableUseItemTalent(MUID nUsedTalentItemUID);
	bool IsEnableUseItemTalent(int nTalentID);
	void DisableUseItemTalent();
	MUID GetUsedTalentItemUID();

	bool IsTPConsumeTalent(int nTalentID);

private:
	/// ä�� �ŷ�Ʈ �н� ���� ����
	class GGatherTalentRank
	{
		set<int> m_setGatherTalentRank;
	public:
		void Insert(int nRank);
		void Delete(int nRank);

		bool HasGatherTalent(int nRank);
	};
	GGatherTalentRank m_GatherTalentRank;

	void Delete(int nTalentID);
};

#endif
