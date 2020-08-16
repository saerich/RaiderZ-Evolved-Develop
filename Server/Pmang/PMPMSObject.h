#pragma once

#include "PmLib.h"
#include "IPMSObject.h"

class PMPMSConnector;

class PMANG_API PMPMSObject : public IPMSObject
{
public:
	PMPMSObject(PMPMSConnector* pPMS);
	virtual ~PMPMSObject(void);

	/*
		Heart Beat Request �� ������ �Ҹ��� �Լ�
		@param1 : sequence index
		@return value : ���� ���� TRUE , ������ FALSE 
	*/
	virtual BOOL OnHeartbeatReq(LONG lIndex) override;
	/*
		Order Request �� ������ �Ҹ��� �Լ�
		@param1 : Order �̸� - NULL Terminate String
		@param2 : Order ���� - NULL Terminate String
		@param3 : ����� ���� - �����϶� ����, �����϶� ���� ���� �Է�
		@param4 : ����� ���� ũ�� 
		@param5 : ī�װ� ID
		@return value : ���� TRUE , ���� FALSE 
	*/
	virtual BOOL OnOrderReq(LPCSTR lpszCmdName, LPCSTR lpszCtlVal, LPSTR lpResult, LONG *lpResultLen, DWORD dwSSN, DWORD dwCategoryID) override;
	/*
	���� �޽����� ������ �˸��� �Լ�
	@param1 : ���� Serial Number
	@param2 : ���Ӻ� ī�װ�
	@param3 : ���� �޽��� - NULL Terminate String
	@return value : ���� TRUE , ���� FALSE 	
	*/
	virtual BOOL OnAnnounceReq(DWORD dwSSN, DWORD dwCategoryID, LPCSTR lpszMsg) override;
	/*
	@param1 : IPMSPerformanceInfo * �� ������ 
	@return value : ������������ ������ TRUE, ������ FALSE ����
	*/
	virtual BOOL OnPerformInfoReq(IPMSPerformanceInfo *pvPerform) override;
	/*
	@param1 : (IPMSRegionInfoList* * �� ������ 
	@return value : ������������ ������ TRUE, ������ FALSE ����
	*/
	virtual BOOL OnRegionInfoReq(IPMSRegionInfoList* plstRegionInfo) override;
	/*
	@param1 : IPMSStatInfoList * �� ������ 
	@return value : ����������� ������ TRUE, ������ FALSE ����
	*/
	virtual BOOL OnStatInfoReq(IPMSStatInfoList* plstStatInfo) override;
	/*
	@param1 : (IPMSRegionInfoListPC* * �� ������ 
	@return value : ������������ ������ TRUE, ������ FALSE ����
	*/
	virtual BOOL OnRegionInfoPCReq(IPMSRegionInfoListPC* plstRegionInfo) override;
	/*
	@param1 : IPMSStatInfoListPC * �� ������ 
	@return value : ����������� ������ TRUE, ������ FALSE ����
	*/
	virtual BOOL OnStatInfoPCReq(IPMSStatInfoListPC* plstStatInfo) override;

protected:
	PMPMSConnector* GetPMS();
private:
	PMPMSConnector* m_pPMS;
};
