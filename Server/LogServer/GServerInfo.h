#ifndef _GSERVER_INFO_H
#define _GSERVER_INFO_H

#include "GDef.h"

/// ���� ����
enum GServerStat
{
	SERVER_STAT_ACTIVE = 0,		///< ������
	SERVER_STAT_FULL,		///< ���ӺҰ�. ����� �ʰ�
	SERVER_STAT_INCHECK,	///< ���ӺҰ�. ���� ���� ��
	SERVER_STAT_ERROR,		///< ���ӺҰ�. ��Ÿ
};

/// ���� ����
struct GServerInfo
{
	// ���� ����
	string			strServerName;		///< ���� �̸�
	GServerStat		nStat;				///< ���� ����
	int				nMaxPlayer;			///< ������ ���ÿ� ������ �ִ��
	int				nMaxPlayerCapacity;	///< ������ ���� ������ �� �ִ� ��
	uint32			nMaxRecvBPS;
	uint32			nMaxSendBPS;
	LONGLONG		nTotalRecvBPS;
	LONGLONG		nTotalSendBPS;
	LONGLONG		nFrameIndex;

	// ���� ����
	int				nFPS;					///< FPS
	int				nNowPlayer;				///< ���� ������ ������ �ִ� ����� ��
	uint32			nRecvBPS;				///< �ʴ� ���� �뷮
	uint32			nSendBPS;				///< �ʴ� ���� �뷮
	uint32			nRecvCPS;				///< �ʴ� ���� Ŀ�ǵ� ��
	uint32			nSendCPS;				///< �ʴ� ���� Ŀ�ǵ� ��
	uint32			nLocalCPS;				///< �ʴ� ���� Ŀ�ǵ� ��

	// �׽�Ʈ ����
	bool			bDamageTest;			///< ������ �׽�Ʈ������

	GServerInfo()
	{
		strServerName = DEFAULT_SERVER_NAME;
		nStat = SERVER_STAT_ACTIVE;
		nMaxPlayerCapacity = DEFAULT_MAX_PLAYER;
		nMaxPlayer = 0;
		nNowPlayer = 0;
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
		bDamageTest = false;
	}
};


#endif