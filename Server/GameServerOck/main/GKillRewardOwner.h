#pragma once

class GKillRewardOwner
{
public:
	struct PARTYUID_CID
	{
		PARTYUID_CID(int nCID, MUID nPartyUID)
		: m_nCID(nCID), m_nPartyUID(nPartyUID) {}
		int		m_nCID;
		MUID	m_nPartyUID;
	};

	typedef std::tr1::unordered_map<int, int> MAP_CANDIDATE_CID;
	typedef map<int, PARTYUID_CID> MAP_CANDIDATE_RANK;

public:
	GKillRewardOwner();
	~GKillRewardOwner(void);

	void GetBeneficiaryAndParty(vector<int>& outvecBeneficiaryCID, MUID& outnPartyUID);
	void InsertBeneficiaryAndParty(const vector<int>& vecBeneficiaryCID, const MUID& nPartyUID);
	bool IsEmpty();	

protected:
	void Insert(int nCID, MUID nPartyUID);
	void Delete(int nCID);
	void Delete(const vector<int>& vecCID);

	virtual void Clear();

	int GetRewardOwnerCID();
	bool IsCandidate(int nCID);	

private:
	int NewCandidateRank();	
	MUID GetRewardOwnerPastPartyUID();
	

private:
	MAP_CANDIDATE_RANK	m_mapCandidateRank;	///< ���� ������ �ĺ� key : Rank, value : CID
	MAP_CANDIDATE_CID	m_mapCandidateCID;	///< ���� ������ �ĺ� key : CID, value : Rank	

	int	m_nCanditateRank;
};
