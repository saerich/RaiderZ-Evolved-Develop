#pragma once

#include "CSCommonLib.h"
#include "CSTalentInfo.h"
#include "CSTalentInfoDef.h"

typedef map<TCHAR, CSTalentInfo*>	TalentRankMap;
typedef map<int, TalentRankMap>		TalentMaxRankMap;
typedef map<int, vector<int>>		TalentLineMap;
typedef map<TALENT_EXTRA_PASSIVE_TYPE, int> GivedExtraPassiveMap;	


class CSCOMMON_API CSTalentInfoMgr : public map<int , CSTalentInfo* > 
{
protected:
	TalentMaxRankMap		m_mapTalentLine;
	TalentLineMap			m_mapLineTalents;
	TalentLineMap			m_mapCooltimeLineTalents;
	GivedExtraPassiveMap	m_mapGivedExtraPassive;		// ������ �־��� �ŷ�Ʈ�� ���� ȹ���� ����Ʈ���нú�
private:	
	CSTalentInfoMgr(CSTalentInfoMgr& other);
protected:
	void CopyHitFromReference( CSTalentInfo* pTalentInfo );
private:
	void ClassifyGivedTalents(bool bNeedTraining, TALENT_EXTRA_PASSIVE_TYPE nExtraPassive, int nExtraPassiveParam);
public:
	CSTalentInfoMgr() {}
	virtual ~CSTalentInfoMgr() {}
	virtual void Cooking();		// Loading�� TalentInfo�� �߰����� ���� �ۼ�
	void _SortHitInfos();
	void _CopyHitInfoRef();

	void Insert(CSTalentInfo* pTalentInfo);
	void Clear();

	// ���� �迭�� �ŷ�Ʈ�� ���� ��ȯ
	vector<int>	GetLineTalents(int nTalentLine) const;
	vector<int>	GetCooltimeLineTalents(int nTalentLine) const;

	// bModeExactly�� false�� �ϸ� mode�� ���������� �⺻ �ŷ�Ʈ�� ��ȯ��
	CSTalentInfo* Find(int nID, int nMode=0, bool bModeExactly=false);	

	bool Exist(int nID);
	bool Exist( int nID, int nMode );

	bool IsGivedExtraPassive(TALENT_EXTRA_PASSIVE_TYPE nPassiveType);
	bool IsGivedExtraPassive(TALENT_EXTRA_PASSIVE_TYPE nPassiveType, int nExtraPassiveLevel);
};


