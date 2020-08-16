#ifndef _GSERVER_INFO_H
#define _GSERVER_INFO_H

#include "GDef.h"

/// ���� ����
enum GServerStat
{
	SERVER_STAT_INACTIVE = 0,
	SERVER_STAT_ACTIVE,		///< ������
	SERVER_STAT_FULL,		///< ���ӺҰ�. ����� �ʰ�
	SERVER_STAT_INCHECK,	///< ���ӺҰ�. ���� ���� ��
	SERVER_STAT_ERROR,		///< ���ӺҰ�. ��Ÿ
};

/// ���� ����
struct GServerInfo
{
	// ���� ����
	wstring			strServerName;		///< ���� �̸�
	GServerStat		nStat;				///< ���� ����
	int				nMaxPlayer;			///< ������ ���ÿ� ������ �ִ��
	int				nMaxPlayerCapacity;	///< ������ ���� ������ �� �ִ� ��
	uint32			nMaxRecvBPS;
	uint32			nMaxSendBPS;
	LONGLONG		nFrameIndex;

	// ���� ����
	int				nFPS;					///< FPS
	int				nNowPlayer;				///< ���� ������ ������ �ִ� ����� ��
	int				nLinkCount;

	int64			nTotalRecvBytes;		///< �� ���� �뷮
	int64			nTotalSendBytes;		///< �� ���� �뷮

	int				nDisconnCountAll;
	int				nDisconnCountByDup;
	int				nDisconnCountByCheat;


	uint32			nRecvBPS;				///< �ʴ� ���� �뷮
	uint32			nSendBPS;				///< �ʴ� ���� �뷮
	uint32			nRecvCPS;				///< �ʴ� ���� Ŀ�ǵ� ��
	uint32			nSendCPS;				///< �ʴ� ���� Ŀ�ǵ� ��
	uint32			nLocalCPS;				///< �ʴ� ���� Ŀ�ǵ� ��
	uint32			nAsyncDBTask;			///< ���� ��� �۾���.
	uint32			nAsyncDBTotalTask;		///< ���ݱ��� �� �� ��� �۾���
	uint32			nMaxAsyncDBTask;		///< �ִ� ��� �۾���.
	uint32			nAsyncTaskCount;		///< ó���� AsyncTask ����

	// �׽�Ʈ ����
	bool			bDamageTest;			///< ������ �׽�Ʈ������

	GServerInfo()
	{
		strServerName = DEFAULT_SERVER_NAME;
		nStat = SERVER_STAT_INACTIVE;
		nMaxPlayerCapacity = DEFAULT_MAX_PLAYER;
		nMaxPlayer = 0;
		nNowPlayer = 0;
		nMaxRecvBPS = 0;
		nMaxSendBPS = 0;
		nFrameIndex = 0;
		nFPS = 0;
		nTotalRecvBytes = 0;
		nTotalSendBytes = 0;

		ResetDisconnCount();

		nRecvBPS = 0;
		nSendBPS = 0;
		nRecvCPS = 0;
		nSendCPS = 0;
		nLocalCPS = 0;
		bDamageTest = false;
		nAsyncDBTask = 0;
		nMaxAsyncDBTask = 0;
		nAsyncDBTotalTask = 0;
		nLinkCount = 0;
		nAsyncTaskCount = 0;
	}

	void ResetDisconnCount()
	{
		nDisconnCountAll = 0;
		nDisconnCountByDup = 0;
		nDisconnCountByCheat = 0;
	}

};


#endif