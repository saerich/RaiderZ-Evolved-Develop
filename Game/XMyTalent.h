#ifndef _XMY_TALENT_H
#define _XMY_TALENT_H

#include "CSChrInfo.h"
#include "XTalentInfo.h"
#include "XBuffInfo.h"
#include "XModEffector.h"
#include <set>
#include "CSTalentInfo.h"
#include "XTalentInfoMgr.h"
using namespace std;

class XMyPassiveTalent
{
private:
	bool	m_bHasExtraPassiveTalent[TEPT_MAX];		///< Ư���� �нú� �ŷ�Ʈ�� �����ϰ� �ִ��� ����
	int		m_nExtraPassiveTalentLevel[TEPT_MAX];
	XModEffector			m_ModEffector;

public:
	XMyPassiveTalent();
	void GainEffect(XTalentInfo* pTalentInfo);
	void LostEffect(XTalentInfo* pTalentInfo);
	bool HasExtraPassiveTalent(TALENT_EXTRA_PASSIVE_TYPE nType)
	{
		if (true == info.talent->IsGivedExtraPassive(nType))
			return true;

		return m_bHasExtraPassiveTalent[nType];
	}
	bool HasExtraPassiveTalent(TALENT_EXTRA_PASSIVE_TYPE nType, int nExtraPassiveLevel)
	{
		if (true == info.talent->IsGivedExtraPassive(nType, nExtraPassiveLevel))
			return true;

		if (m_bHasExtraPassiveTalent[nType] == true && m_nExtraPassiveTalentLevel[nType] >= nExtraPassiveLevel)
			return true;

		return false;
	}
	int GetExtraPassiveLevel(TALENT_EXTRA_PASSIVE_TYPE nType)	
	{ 
		return m_nExtraPassiveTalentLevel[nType]; 
	}
};


// ��Ÿ�� ���� ����
enum XCOOLTIME_STATE
{
	XCS_END				= 0,		// Ż��Ʈ ��� ����
	XCS_REQ_USE_TALENT,				// Ż��Ʈ ��� ��û �� : C -> S
	XCS_CASTING,					// Ż��Ʈ ĳ���� �� : C
	XCS_REQ_ACT_TALENT,				// Ż��Ʈ ��� ��û �� : C -> S
	XCS_USE_TALENT,					// Ż��Ʈ ��� �� : S -> C
	XCS_TIMEOUT,					// Ż��Ʈ cool time �ð� ����
};

// ��Ÿ�� ����
struct XCOOLTIME_UNIT
{
	XCOOLTIME_STATE		nState;
	float				fCoolTime;
	float				fElapsedTime;

	XCOOLTIME_UNIT() : nState( XCS_END), fCoolTime( 0.0f), fElapsedTime( 0.0f)		{}
};

// ��Ÿ�� �޴���
class XCoolTimeMgr
{
private:
	map< int, XCOOLTIME_UNIT>	m_mapCoolTime;

public:
	XCoolTimeMgr();
	virtual ~XCoolTimeMgr();

	void Update( float fDelta);

	void Clear();

	bool IsCoolDown( int nTalentID);					// Ż��Ʈ�� ����� �� ������ ���� Ȯ��
	float GetRemainedTime(int nTalentID);				// ���� �ð��� ����
	float GetRemainedTimeRatio(int nTalentID);			// ���� �ð��� ������ ����

	void RequestUseTalent( int nTalentID);				// Ż��Ʈ ��� ��û : C -> S
	void CastingTalent( int nTalentID);					// Ż��Ʈ ĳ���� : C
	void RequestActTalent( int nTalentID);				// Ż��Ʈ ��� ��û : C -> S
	void UseTalent( int nTalentID);						// Ż��Ʈ ��� : S -> C
	void Canceled( int nTalentID);						// Ż��Ʈ ��� : S -> C

	void SetRemained( int nTalentID, float fRemainedTime);		// �̹� �����ϴ� Ż��Ʈ�� ���� cool time �ð� ���� : S -> C

protected:
	void _add_cooltime( int nTalentID, XCOOLTIME_STATE nState, float fCoolTime, float fElapsedTime =0.0f);
	bool IsToggleTalent(int nTalentID);
	bool IsToggleTalent(XTalentInfo* pTalentInfo);

private:
	vector<int> GetRelativeTalentsID(CSTalentInfo* pTalentInfo);
};



typedef set<int> SET_TALENTID;

class XMyTalent
{
private:
	/// ä�� �ŷ�Ʈ �н� ���� ����
	class XGatherTalentRank
	{
		set<int> m_setGatherTalentRank;
	public:
		void Insert(int nRank);
		void Delete(int nRank);

		bool HasGatherTalent(int nRank);
	};
	XGatherTalentRank m_GatherTalentRank;

private:
	XMyPassiveTalent		m_Passive;
	SET_TALENTID			m_setTalentID;			///< �Ϲ������� ��� �ŷ�Ʈ
	int						m_nRemainTP;			///< ����Ҽ� �ִ� TP

	SET_TALENTID			m_setStyleTalentID[9];		///< 
	XModEffector			m_ModEffector;

	SET_TALENTID			m_setSocialTalentID;	///< �Ҽ��ŷ�Ʈ ID
	SET_TALENTID			m_setBuffTalentID;		///< ���� �ɷ����� ��� �Ҽ� �ִ� �ŷ�Ʈ ID

public:
	XCoolTimeMgr		CoolTime;
	

	XMyTalent()
	{
		Init();
	}

	void Init()
	{
		m_nRemainTP = 0;

		InitTalent();
	}

	void InitTalent()
	{
		m_setTalentID.clear();

		for (int i=0; i< 7; i++)
		{
			m_setStyleTalentID[i].clear();
		}
	}

	void GainInstantEffect(XTalentInfo* pTalentInfo);
	void GainMaintainEffect(XTalentInfo* pTalentInfo);

	void LostMaintainEffect(XTalentInfo* pTalentInfo);
	void LostMaintainEffect(int nTalentID);

	bool HasExtraPassiveTalent(TALENT_EXTRA_PASSIVE_TYPE nType)	{ return m_Passive.HasExtraPassiveTalent(nType); }
	bool HasExtraPassiveTalent(TALENT_EXTRA_PASSIVE_TYPE nType, int nExtraPassiveLevel)	{ return m_Passive.HasExtraPassiveTalent(nType, nExtraPassiveLevel); }
	int GetExtraPassiveLevel(TALENT_EXTRA_PASSIVE_TYPE nType)	{ return m_Passive.GetExtraPassiveLevel(nType); }

	bool Train(XTalentInfo* pTalentInfo, bool bConsumeTP=true);
	void Untrain(int nTalentID);
	void UntrainAll();

	void SetSocialTalentID(int nID);

	void GetAllTalentID(vector<int>& outvecTalentID);

	int GetTP() { return m_nRemainTP; }
	void SetTP(int nTP) { m_nRemainTP = nTP; }
	void AddTP(int nTP) { m_nRemainTP += nTP; }

	bool IsLearnedTalent(int nTalentID);
	bool IsLearnedTalent(XTalentInfo* pTalentInfo);
	bool IsLearnedTalentLine(int nTalentLine);
	bool IsLearnedGatherTalent(int nRank);

	void GetHighestRankListOfTalent(vector<XTalentInfo*>& outvecTalentID);
	int GetHighestRankByTalentLine( int nTalentID);

	int GetUsedTP()							{ return m_setTalentID.size();}
	int GetStyleUsedTP(TALENT_STYLE nStyle)
	{
		if (nStyle >= TALENT_STYLE_MAX || nStyle <= TALENT_STYLE_NONE)
		{
			return 0;
		}

		const SET_TALENTID& setStyleTalentID = m_setStyleTalentID[nStyle];

		int nStyleUsedTP = 0;
		for each (int nID in setStyleTalentID)
		{
			CSTalentInfo* pTalentInfo = info.talent->Find(nID);
			if (NULL == pTalentInfo) continue;
			if (true == pTalentInfo->m_bByLicense) continue;

			nStyleUsedTP++;
		}

		return nStyleUsedTP;
	}

	size_t GetTalentCount() const					{ return m_setTalentID.size();			}
	int GetTalentID( int index);

	int GetEmotionalTalentCount() const;
	int GetEmotionalTalentID( int index);

	void SetBuffTalentID(int arrPalettes[]);
	void ReSetBuffTalentID();
	bool IsUsableBuffTalentID(int nID);
	int GetBuffTalentCount()						{ return m_setBuffTalentID.size(); }
};

#endif // _XMY_TALENT_H