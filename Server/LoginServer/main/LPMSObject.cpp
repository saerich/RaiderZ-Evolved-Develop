#include "StdAfx.h"
#include "LPMSObject.h"
#include "PMPMSConnector.h"
#include "PmRegionStat.h"
#include "MLocale.h"

LPMSObject::LPMSObject(PMPMSConnector* pPMS)
: PMPMSObject(pPMS)
{
}

LPMSObject::~LPMSObject(void)
{
}

BOOL LPMSObject::OnRegionInfoReq( IPMSRegionInfoList* plstRegionInfo )
{
	VALID_RET(plstRegionInfo, FALSE);

	DWORD nSSN = GetPMS()->GetSSN();
	DWORD nCategory = GetPMS()->GetCategory();
	VLONG* vRegionStat = GetPMS()->GetRegionStat()->GetRegionStat();

	plstRegionInfo->AddRegionInfo(nSSN, nCategory, vRegionStat);

	return TRUE;
}

BOOL LPMSObject::OnStatInfoReq( IPMSStatInfoList* plstStatInfo )
{
	DWORD nSSN = GetPMS()->GetSSN();
	DWORD nCategory = GetPMS()->GetCategory();
	DWORD nCU = GetPMS()->GetRegionStat()->GetTotalUserCount();

	plstStatInfo->AddStatInfoList(nSSN, nCategory, nCU, 0, 0, 0, NULL);

	return TRUE;
}

BOOL LPMSObject::OnRegionInfoPCReq(IPMSRegionInfoListPC* plstRegionInfo)
{
	VALID_RET(plstRegionInfo, FALSE);

	DWORD nSSN = GetPMS()->GetSSN();
	DWORD nCategory = GetPMS()->GetCategory();
	VLONG* vRegionStat = GetPMS()->GetRegionStat()->GetRegionStatPC();

	plstRegionInfo->AddRegionInfoPC(nSSN, nCategory, vRegionStat);

	return TRUE;
}

BOOL LPMSObject::OnStatInfoPCReq(IPMSStatInfoListPC* plstStatInfo)
{
	DWORD nSSN = GetPMS()->GetSSN();
	
	map<int, int>& mapTotalUserCountPC = GetPMS()->GetRegionStat()->GetTotalUserCountPC();

	for each(const pair<int, int>& pairPCCafeCount in mapTotalUserCountPC)
	{
		DWORD nCategory = pairPCCafeCount.first;
		DWORD nCU = pairPCCafeCount.second;

		plstStatInfo->AddStatInfoListPC(nSSN, nCategory, nCU, 0, 0, 0, NULL);
	}

	return TRUE;
}
