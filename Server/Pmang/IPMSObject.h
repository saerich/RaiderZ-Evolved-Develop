
#pragma once

#ifndef __IPMSOBJECT_H_
#define __IPMSOBJECT_H_


typedef std::vector<LONG> VLONG;
/*
OnRegionInfoReq �Լ��� �Ҹ��� ������ ����� ���� ������ PMSConn�� ���� HA�� �����ϱ� ���� �����͸� �����ϴ� Interface 
@param1 : ���� Serial Number
@param2 : ���Ӻ� ī�װ�
@param3 : ����� ���������� ������ �ִ� Vector<LONG> �Ϲ������� UserInfo[i][j][k] Array�� �Ǿ� ���� ��� (i, j, k) �϶� (0,0,0) (0,0,1) ~ (0,1,0) ~ (1,0.0) �� ������ �Է� �ϸ� �ȴ�.
@return value ����
*/
interface IPMSRegionInfoList
{
	virtual void AddRegionInfo(DWORD dwSSN, DWORD dwCategoryID, VLONG* pvRegionStat) = 0;
// by truewise - 2006.05.09 
	virtual void AddRegionInfo2(DWORD dwSSN, DWORD dwCategoryID, LONG pvRegionStat[238]) = 0;
	virtual void AddRegionInfo238(DWORD dwSSN, DWORD dwCategoryID, LONG pvRegionStat[238]) = 0;
	virtual void AddRegionInfo272(DWORD dwSSN, DWORD dwCategoryID, LONG pvRegionStat[272]) = 0;
};

// by truewise - 2007.03.22 for PC�� ������ 
interface IPMSRegionInfoListPC
{
	virtual void AddRegionInfoPC(DWORD dwSSN, DWORD dwCategoryID, VLONG* pvRegionStat) = 0;
	virtual void AddRegionInfoPC238(DWORD dwSSN, DWORD dwCategoryID, LONG pvRegionStat[238]) = 0;
	virtual void AddRegionInfoPC272(DWORD dwSSN, DWORD dwCategoryID, LONG pvRegionStat[272]) = 0;
};

/*
IPMSStatInfoList ���� ���� ������ ��� ������ PMSConn�� �̿��Ͽ� �����ϱ� ���� �����͸� ���� �ϴ� Interface
@param1 : ���� Serial Number
@param2 : ���Ӻ� ī�װ�
@param3 : @param1, @param2�� ������ �����ϴ� ����� ��
@param4 : ���� ���� ������ �����Ǿ� �ְ� @param1, @param2�� ������ �����ϴ� Channel ��
@param5 : ���� ���� ������ �����Ǿ� �ְ� @param1, @param2�� ������ �����ϴ� ���� �� ��
@param6 : Optional �� ������
@return value ����
*/
interface IPMSStatInfoList
{
	virtual void AddStatInfoList( DWORD dwSSN, DWORD dwCategory, DWORD dwCU,
        	DWORD dwSession, 	DWORD dwChannelCnt, DWORD dwRoomCnt, LPCSTR pszOptionInfo) = 0;
};

// by truewise - 2007.03.22 for PC�� ������ 
interface IPMSStatInfoListPC
{
	virtual void AddStatInfoListPC( DWORD dwSSN, DWORD dwCategory, DWORD dwCU,
        	DWORD dwSession, 	DWORD dwChannelCnt, DWORD dwRoomCnt, LPCSTR pszOptionInfo) = 0;
};

/*
IPMSPerformanceInfo���� ���� ������ ���� ������ PMSConn�� �̿��Ͽ� �����ϱ� ���� �����͸� ���� �ϴ� Interface
@param1 : ���� ������ ���� ������ ��� �ִ� long type�� vector pointer
@return value ����
*/

interface IPMSPerformanceInfo
{
	virtual void AddPerformanceInfo(VLONG* pvPerformance) = 0;
// by truewise - 2006.05.09 
	virtual void AddPerformanceInfo2(LONG pvPerformance[10]) = 0;
};

/*
IPMSObject interface
PMSConn���� ȣ���� ���Ͽ� ������ Call Back Object Interface
*/
interface IPMSObject
{
public:
	/*
		Heart Beat Request �� ������ �Ҹ��� �Լ�
		@param1 : sequence index
		@return value : ���� ���� TRUE , ������ FALSE 
	*/
	virtual BOOL OnHeartbeatReq(LONG lIndex) = 0;
	/*
		Order Request �� ������ �Ҹ��� �Լ�
		@param1 : Order �̸� - NULL Terminate String
		@param2 : Order ���� - NULL Terminate String
		@param3 : ����� ���� - �����϶� ����, �����϶� ���� ���� �Է�
		@param4 : ����� ���� ũ�� 
		@param5 : ī�װ� ID
		@return value : ���� TRUE , ���� FALSE 
	*/
	virtual BOOL OnOrderReq(LPCSTR lpszCmdName, LPCSTR lpszCtlVal, LPSTR lpResult, LONG *lpResultLen, DWORD dwSSN, DWORD dwCategoryID) = 0;
	/*
	���� �޽����� ������ �˸��� �Լ�
	@param1 : ���� Serial Number
	@param2 : ���Ӻ� ī�װ�
	@param3 : ���� �޽��� - NULL Terminate String
	@return value : ���� TRUE , ���� FALSE 	
	*/
	virtual BOOL OnAnnounceReq(DWORD dwSSN, DWORD dwCategoryID, LPCSTR lpszMsg) = 0;
	/*
	@param1 : IPMSPerformanceInfo * �� ������ 
	@return value : ������������ ������ TRUE, ������ FALSE ����
	*/
	virtual BOOL OnPerformInfoReq(IPMSPerformanceInfo *pvPerform) = 0;
	/*
	@param1 : (IPMSRegionInfoList* * �� ������ 
	@return value : ������������ ������ TRUE, ������ FALSE ����
	*/
	virtual BOOL OnRegionInfoReq(IPMSRegionInfoList* plstRegionInfo) = 0;
	/*
	@param1 : IPMSStatInfoList * �� ������ 
	@return value : ����������� ������ TRUE, ������ FALSE ����
	*/
	virtual BOOL OnStatInfoReq(IPMSStatInfoList* plstStatInfo) = 0;
	/*
	@param1 : (IPMSRegionInfoListPC* * �� ������ 
	@return value : ������������ ������ TRUE, ������ FALSE ����
	*/
	virtual BOOL OnRegionInfoPCReq(IPMSRegionInfoListPC* plstRegionInfo) = 0;
	/*
	@param1 : IPMSStatInfoListPC * �� ������ 
	@return value : ����������� ������ TRUE, ������ FALSE ����
	*/
	virtual BOOL OnStatInfoPCReq(IPMSStatInfoListPC* plstStatInfo) = 0;
};



#endif