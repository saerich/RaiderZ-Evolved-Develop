#include "stdafx.h"
#include "GWarpInfoCollector.h"
#include "GWarpInfoChecker.h"
#include "GDef.h"

// SENSOR_WARP_INFO �÷��� ó���� �����ϱ����� ������� 
typedef const vector<SENSOR_WARP_INFO>	vec_warp;
typedef vec_warp::const_iterator		it_warp;


GWarpInfoCollector::GWarpInfoCollector()
{
	// do nothing
}

GWarpInfoCollector::~GWarpInfoCollector()
{
	Clear();
}

GWarpInfoChecker* GWarpInfoCollector::NewWarpInfoChecker(const SENSOR_WARP_INFO& warpInfo)
{
	return new GWarpInfoChecker(warpInfo);
}

GWarpInfoCollector::Result GWarpInfoCollector::CollectAutoWarp(const SENSOR_INFO& sensorInfo, GEntityPlayer* pPlayer)
{	
	// ���� ���� �ʱ�ȭ
	Clear();

	// �ڵ����� ������ �����Ѵ�.
	if (!CollectAutoTrialWarp(sensorInfo, pPlayer))
	{
		CollectAutoSharedWarp(sensorInfo, pPlayer);
	}
	
	// ���� �ʵ��� �������� �׷����� ���´�.
	return ExtractWarpField();
}

GWarpInfoCollector::Result GWarpInfoCollector::CollectManualWarp(const SENSOR_INFO& sensorInfo, GEntityPlayer* pPlayer)
{
	// ���� ���� �ʱ�ȭ
	Clear();

	// �������� ������ �����Ѵ�.
	CollectManualTrialWarp(sensorInfo, pPlayer);
	CollectManualSharedWarp(sensorInfo, pPlayer);

	// ���� �ʵ��� �������� �׷����� ���´�.
	return ExtractWarpField();
}

bool GWarpInfoCollector::CollectAutoTrialWarp(const SENSOR_INFO& sensorInfo, GEntityPlayer* pPlayer)
{
	// �ڵ����� Ʈ���̾� �ʵ� ����
	vec_warp& vecWarpInfo = sensorInfo.vecWarpInfos;
	for(it_warp it = vecWarpInfo.begin(); it != vecWarpInfo.end(); it++)
	{
		auto_ptr<GWarpInfoChecker> autoChecker(NewWarpInfoChecker(*it));
		
		if (!autoChecker->IsAutoTrialWarp())			continue;
		if (!autoChecker->CheckCondition(pPlayer))		continue;

		GWarpInfoChecker::Result checkResult = autoChecker->CheckQuestField(pPlayer);
		if (checkResult == GWarpInfoChecker::FAILURE)	continue;
		
		bool isFullHouse = checkResult == GWarpInfoChecker::FULLHOUSE;
		AddWarpInfoChecker(autoChecker.release(), isFullHouse);
	}

	return !m_vecWarpInfo.empty();
}

bool GWarpInfoCollector::CollectManualTrialWarp(const SENSOR_INFO& sensorInfo, GEntityPlayer* pPlayer)
{
	// �������� Ʈ���̾� �ʵ� ����
	vec_warp& vecWarpInfo = sensorInfo.vecWarpInfos;
	for(it_warp it = vecWarpInfo.begin(); it != vecWarpInfo.end(); it++)
	{
		auto_ptr<GWarpInfoChecker> autoChecker(NewWarpInfoChecker(*it));
		
		if (!autoChecker->IsManualTrialWarp())			continue;
		if (!autoChecker->CheckCondition(pPlayer))		continue;

		GWarpInfoChecker::Result checkResult = autoChecker->CheckQuestField(pPlayer);
		if (checkResult == GWarpInfoChecker::FAILURE)	continue;
		
		bool isFullHouse = checkResult == GWarpInfoChecker::FULLHOUSE;
		AddWarpInfoChecker(autoChecker.release(), isFullHouse);
	}

	return !m_vecWarpInfo.empty();
}

bool GWarpInfoCollector::CollectAutoSharedWarp(const SENSOR_INFO& sensorInfo, GEntityPlayer* pPlayer)
{
	// ���� �ʵ� ����
	vec_warp& vecWarpInfo = sensorInfo.vecWarpInfos;
	for(it_warp it = vecWarpInfo.begin(); it != vecWarpInfo.end(); it++)
	{
		auto_ptr<GWarpInfoChecker> autoChecker(NewWarpInfoChecker(*it));
		
		if (!autoChecker->IsAutoSharedWarp())			continue;
		if (!autoChecker->CheckCondition(pPlayer))		continue;

		AddWarpInfoChecker(autoChecker.release(), false);
	}

	// ����Ʈ ���� �߰� �˻�
	const SENSOR_WARP_INFO& warpInfo = sensorInfo.DefaultWarpInfo;
	auto_ptr<GWarpInfoChecker> autoChecker(NewWarpInfoChecker(warpInfo));

	if (autoChecker->IsAutoSharedWarp())
	{
		AddWarpInfoChecker(autoChecker.release(), false);
	}

	
	return !m_vecWarpInfo.empty();
}

bool GWarpInfoCollector::CollectManualSharedWarp(const SENSOR_INFO& sensorInfo, GEntityPlayer* pPlayer)
{
	// ���� �ʵ� ����
	vec_warp& vecWarpInfo = sensorInfo.vecWarpInfos;
	for(it_warp it = vecWarpInfo.begin(); it != vecWarpInfo.end(); it++)
	{
		auto_ptr<GWarpInfoChecker> autoChecker(NewWarpInfoChecker(*it));
		
		if (!autoChecker->IsManualSharedWarp())			continue;
		if (!autoChecker->CheckCondition(pPlayer))		continue;

		AddWarpInfoChecker(autoChecker.release(), false);
	}

	return !m_vecWarpInfo.empty();
}

GWarpInfoCollector::Result GWarpInfoCollector::ExtractWarpField(void)
{
	// WarpInfo�� �ʵ庰�� ����
	m_vecExtract.clear();

	for(int i = 0; i < (int)m_vecWarpInfo.size(); i++)
	{
		AddExtractField(i);
	}
	
	// ����� �ʵ� ��� ����
	return GetExtractResult();
}

void GWarpInfoCollector::AddWarpInfoChecker(GWarpInfoChecker* pWarpInfoChecker, bool isFullHouse)
{
	VALID(pWarpInfoChecker != NULL);
	m_vecWarpInfo.push_back(pair_warpinfo(pWarpInfoChecker, isFullHouse));
}

void GWarpInfoCollector::Clear(void)
{
	for each(pair_warpinfo warpInfo in m_vecWarpInfo)
	{
		SAFE_DELETE(warpInfo.first);
	}
	
	m_vecWarpInfo.clear();
	m_vecExtract.clear();
}

int GWarpInfoCollector::GetWarpCount(void) const
{
	return m_vecExtract.size();
}

int GWarpInfoCollector::GetFieldGroupID(int nIndex) const
{
	const GWarpInfoChecker* pWarpInfoChecker = GetWarpInfoChecker(nIndex);
	VALID_RET(pWarpInfoChecker != NULL, INVALID_ID);

	return pWarpInfoChecker->GetFieldGroupID();
}

int GWarpInfoCollector::GetFieldID(int nIndex) const
{
	const GWarpInfoChecker* pWarpInfoChecker = GetWarpInfoChecker(nIndex);
	VALID_RET(pWarpInfoChecker != NULL, INVALID_ID);

	return pWarpInfoChecker->GetFieldID();
}

int GWarpInfoCollector::GetMarkerID(int nIndex) const
{	
	const GWarpInfoChecker* pWarpInfoChecker = GetWarpInfoChecker(nIndex);
	VALID_RET(pWarpInfoChecker != NULL, INVALID_ID);

	return pWarpInfoChecker->GetMarkerID();
}

bool GWarpInfoCollector::IsTrialWarp(int nIndex) const
{
	const GWarpInfoChecker* pWarpInfoChecker = GetWarpInfoChecker(nIndex);
	VALID_RET(pWarpInfoChecker != NULL, false);

	return pWarpInfoChecker->IsTrialWarp();
}

bool GWarpInfoCollector::IsSharedWarp(int nIndex) const
{
	const GWarpInfoChecker* pWarpInfoChecker = GetWarpInfoChecker(nIndex);
	VALID_RET(pWarpInfoChecker != NULL, false);

	return pWarpInfoChecker->IsSharedWarp();
}

bool GWarpInfoCollector::IsManualTrialWarp(int nIndex) const
{
	const GWarpInfoChecker* pWarpInfoChecker = GetWarpInfoChecker(nIndex);
	VALID_RET(pWarpInfoChecker != NULL, false);

	return pWarpInfoChecker->IsManualTrialWarp();
}

bool GWarpInfoCollector::IsSingleQuestWarp(int nIndex) const
{
	const GWarpInfoChecker* pWarpInfoChecker = GetWarpInfoChecker(nIndex);
	VALID_RET(pWarpInfoChecker != NULL, false);

	return pWarpInfoChecker->IsSingleQuestWarp();
}

const SENSOR_WARP_INFO* GWarpInfoCollector::GetSensorWarpInfo(int nIndex) const
{
	const GWarpInfoChecker* pWarpInfoChecker = GetWarpInfoChecker(nIndex);
	VALID_RET(pWarpInfoChecker != NULL, NULL);

	return pWarpInfoChecker->GetSensorWarpInfo();
}

GWarpInfoCollector::Result GWarpInfoCollector::RecheckWarp(int nIndex, GEntityPlayer* pPlayer) const
{
	Result recheckResult = NOTHING;

	// ���õ� ���� Ȯ��
	const GWarpInfoChecker* pWarpInfoChecker = GetWarpInfoChecker(nIndex);
	VALID_RET(pWarpInfoChecker != NULL, NOTHING);

	// ���� ��� Field�� ���� ���� ��˻�
	for(vec_warpinfo::const_iterator it = m_vecWarpInfo.begin(); it != m_vecWarpInfo.end(); it++)
	{
		const GWarpInfoChecker* pChecker = it->first;

		if (pWarpInfoChecker->SameFieldWarp(pChecker))
		{
			// ����� �˻�
			if (!pWarpInfoChecker->CheckCondition(pPlayer))	continue;			
			
			// Ʈ���̾� �ʵ尡 �ƴϸ� ����
			if (!pWarpInfoChecker->IsTrialWarp())	return SINGLE;
			
			// Ʈ���̾� �ʵ�� ����Ʈ �˻�
			GWarpInfoChecker::Result checkResult = pWarpInfoChecker->CheckQuestField(pPlayer);
			if (checkResult == GWarpInfoChecker::SUCCESS)	return SINGLE;

			// ������ ���� ��� �˻�.
			if (checkResult == GWarpInfoChecker::FULLHOUSE)
			{
				recheckResult = FULLHOUSE;
				continue;
			}
		}
	}
	
	// �˻� ����� ���� ����
	return recheckResult;
}

int GWarpInfoCollector::GetAllowFollowWarpCount(int nIndex, GEntityPlayer* pPlayer) const
{
	const GWarpInfoChecker* pWarpInfoChecker = GetWarpInfoChecker(nIndex);
	VALID_RET(pWarpInfoChecker != NULL, 0);

	if (!pWarpInfoChecker->IsManualTrialWarp())	return 0;


	// ���� ���� ����Ʈ�� �ο����� Ȯ��
	int nLargestAllowCount = 0;
	for(vec_warpinfo::const_iterator it = m_vecWarpInfo.begin(); it != m_vecWarpInfo.end(); it++)
	{
		const GWarpInfoChecker* pChecker = it->first;

		if (!pWarpInfoChecker->SameFieldWarp(pChecker))	continue;

		// ����� �˻�
		if (!pWarpInfoChecker->CheckCondition(pPlayer))	continue;
					
		// Ʈ���̾� �ʵ�� ����Ʈ �˻�
		GWarpInfoChecker::Result checkResult = pWarpInfoChecker->CheckQuestField(pPlayer);
		if (checkResult != GWarpInfoChecker::SUCCESS)	continue;

		// ���� ū ���� �ο����� ���Ѵ�. ����� �� �ִ� ���� ���� ���� ������ �ڱ��ڽ��� ����
		int nEnterAllowCount = pChecker->GetAllowEnterPlayerCount(pPlayer);		
		nLargestAllowCount = max(nLargestAllowCount, nEnterAllowCount - 1);
	}

	return nLargestAllowCount;
}

void GWarpInfoCollector::AddExtractField(int nWarpInfoIndex)
{
	VALID(0 <= nWarpInfoIndex);
	VALID(nWarpInfoIndex < (int)m_vecWarpInfo.size());

	// ���� �ʵ尡 �ִ� Ȯ���Ѵ�.
	for(vec_extract::iterator it = m_vecExtract.begin(); it != m_vecExtract.end(); it++)
	{		
		int nIndex = *it;
		VALID(0 <= nIndex);
		VALID(nIndex < (int)m_vecWarpInfo.size());

		if (SameFieldChecker(nWarpInfoIndex, nIndex))
		{
			// ���� �ʵ尡 �����̸�, �������� ��ü
			bool isFullHouse = m_vecWarpInfo[nIndex].second;
			if (isFullHouse)
			{
				*it = nWarpInfoIndex;
			}

			// ���� �ʵ�� �߰����� �ʴ´�.
			return;
		}
	}

	// ���� �ʵ尡 ������ �߰�
	m_vecExtract.push_back(nWarpInfoIndex);
}

bool GWarpInfoCollector::SameFieldChecker(int nWarpInfoIndex1, int nWarpInfoIndex2) const
{
	VALID_RET(0 <= nWarpInfoIndex1, false);
	VALID_RET(0 <= nWarpInfoIndex2, false);
	VALID_RET(nWarpInfoIndex1 < (int)m_vecWarpInfo.size(), false);
	VALID_RET(nWarpInfoIndex2 < (int)m_vecWarpInfo.size(), false);

	GWarpInfoChecker* pWarpInfo1 = m_vecWarpInfo[nWarpInfoIndex1].first;
	GWarpInfoChecker* pWarpInfo2 = m_vecWarpInfo[nWarpInfoIndex2].first;
	VALID_RET(pWarpInfo1 != NULL, false);
	VALID_RET(pWarpInfo2 != NULL, false);

	return pWarpInfo1->SameFieldWarp(pWarpInfo2);
}

GWarpInfoCollector::Result GWarpInfoCollector::GetExtractResult(void) const
{
	int nCount = m_vecExtract.size();

	// �ʵ尡 �ΰ� �̻��̸� ����
	if (2 <= nCount)
	{
		return MULTY;
	}

	// �ʵ尡 �Ѱ��̸� �������� ����
	if (1 == nCount)
	{
		int nIndex = m_vecExtract.front();
		VALID_RET(0 <= nIndex, NOTHING);
		VALID_RET(nIndex < (int)m_vecWarpInfo.size(), NOTHING);

		bool isFullHouse = m_vecWarpInfo[nIndex].second;		
		return isFullHouse ? FULLHOUSE : SINGLE;
	}

	// �ʵ� ����
	return NOTHING;
}

const GWarpInfoChecker* GWarpInfoCollector::GetWarpInfoChecker(int nWarpInfoIndex) const
{
	VALID_RET(0 <= nWarpInfoIndex, NULL);
	VALID_RET(nWarpInfoIndex < (int)m_vecWarpInfo.size(), NULL);

	GWarpInfoChecker* pWarpInfoChecker = m_vecWarpInfo[nWarpInfoIndex].first;
	VALID_RET(pWarpInfoChecker != NULL, NULL);

	return pWarpInfoChecker;
}
