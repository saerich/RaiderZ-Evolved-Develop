#pragma once

#include "PmDef.h"
#include "PMPMSObject.h"

class GPMSObject : public PMPMSObject
{
public:
	GPMSObject(PMPMSConnector* pPMS);
	virtual ~GPMSObject(void);

	/*
		Heart Beat Request �� ������ �Ҹ��� �Լ�
		@param1 : sequence index
		@return value : ���� ���� TRUE , ������ FALSE 
	*/
	virtual BOOL OnHeartbeatReq(LONG lIndex) override;

	/*
	���� �޽����� ������ �˸��� �Լ�
	@param1 : ���� Serial Number
	@param2 : ���Ӻ� ī�װ�
	@param3 : ���� �޽��� - NULL Terminate String
	@return value : ���� TRUE , ���� FALSE 	
	*/
	virtual BOOL OnAnnounceReq(DWORD dwSSN, DWORD dwCategoryID, LPCSTR lpszMsg) override;

	/*
	@param1 : (IPMSRegionInfoList* * �� ������ 
	@return value : ������������ ������ TRUE, ������ FALSE ����
	*/
	virtual BOOL OnRegionInfoReq(IPMSRegionInfoList* plstRegionInfo) override;


	virtual BOOL OnStatInfoReq( IPMSStatInfoList* plstStatInfo ) override;

	BOOL OnRegionInfoPCReq(IPMSRegionInfoListPC* plstRegionInfo) override;
	BOOL OnStatInfoPCReq(IPMSStatInfoListPC* plstStatInfo) override;
};
