#pragma once

#include "ZDef.h"

/// ���� ����
enum ZServerStat
{
	SERVER_STAT_INACTIVE = 0,
	SERVER_STAT_ACTIVE,			///< ������
	SERVER_STAT_INCHECK,		///< ���ӺҰ�. ���� ���� ��
	SERVER_STAT_ERROR,			///< ���ӺҰ�. ��Ÿ
};

/// ���� ����
struct ZServerInfo
{
	// ���� ����
	wstring			strServerName;		///< ���� �̸�
	ZServerStat		nStat;				///< ���� ����
	uint32			nMaxRecvBPS;
	uint32			nMaxSendBPS;
	LONGLONG		nTotalRecvBPS;
	LONGLONG		nTotalSendBPS;
	LONGLONG		nFrameIndex;

	int				nMaxRequiredLoginServerCount;	///< ���� �޾ƾ��� �α��� ���� ��
	int				nMaxRequiredGameServerCount;	///< ���� �޾ƾ��� ���� ���� ��
	int				nMaxRequiredAppServerCount;		///< ���� �޾ƾ��� �� ���� ��

	int				nSharedFieldInfoCount;	///< ���� �ʵ� ���� ����

	// ���� ����
	int				nFPS;						///< FPS
	int				nIsLoginServerConnected;	///< ���� ������ ���� ���� �α��� ���� ���� (0: Disconnected , 1: Connected)
	int				nCurrGameServerCount;		///< ���� ������ ���� ���� ���� ���� ��
	int				nCurrAppServerCount;		///< ���� ������ ���� ���� �� ���� ��
	int				nCurrPlayerCount;			///< ���� ���忡 ���� ���� �÷��̾� ��
	uint32			nRecvBPS;					///< �ʴ� ���� �뷮
	uint32			nSendBPS;					///< �ʴ� ���� �뷮
	uint32			nRecvCPS;					///< �ʴ� ���� Ŀ�ǵ� ��
	uint32			nSendCPS;					///< �ʴ� ���� Ŀ�ǵ� ��
	uint32			nLocalCPS;					///< �ʴ� ���� Ŀ�ǵ� ��

	int				nAllSharedFieldCount;		///< ��ü ���� �ʵ�(ä��) ����

	// �׽�Ʈ ����

	ZServerInfo()
	{
		strServerName = DEFAULT_SERVER_NAME;
		nStat = SERVER_STAT_INACTIVE;

		nMaxRequiredLoginServerCount = 0;
		nMaxRequiredGameServerCount = 0;
		nMaxRequiredAppServerCount = 0;

		nIsLoginServerConnected = 0;
		nCurrGameServerCount = 0;
		nCurrAppServerCount = 0;

		nCurrPlayerCount = 0;

		nSharedFieldInfoCount = 0;
		nAllSharedFieldCount = 0;
		
		nMaxRecvBPS = 0;
		nMaxSendBPS = 0;
		nTotalRecvBPS = 0;
		nTotalSendBPS = 0;
		nFrameIndex = 0;
		nFPS = 0;
		nRecvBPS = 0;
		nSendBPS = 0;
		nRecvCPS = 0;
		nSendCPS = 0;
		nLocalCPS = 0;
	}

};


