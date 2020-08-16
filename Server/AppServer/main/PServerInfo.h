#pragma once

#include "PDef.h"

/// ���� ����
enum PServerStat
{
	SERVER_STAT_INACTIVE = 0,
	SERVER_STAT_ACTIVE,			///< ������
	SERVER_STAT_INCHECK,		///< ���ӺҰ�. ���� ���� ��
	SERVER_STAT_ERROR,			///< ���ӺҰ�. ��Ÿ
};

/// ���� ����
struct PServerInfo
{
	// ���� ����
	wstring			strServerName;		///< ���� �̸�
	PServerStat		nStat;				///< ���� ����
	uint32			nMaxRecvBPS;
	uint32			nMaxSendBPS;
	LONGLONG		nTotalRecvBPS;
	LONGLONG		nTotalSendBPS;
	LONGLONG		nFrameIndex;

	// ���� ����
	int				nFPS;					///< FPS
	int				nCurrGameServerCount;	///< ���� ������ �������� ���� ���� ��
	uint32			nRecvBPS;				///< �ʴ� ���� �뷮
	uint32			nSendBPS;				///< �ʴ� ���� �뷮
	uint32			nRecvCPS;				///< �ʴ� ���� Ŀ�ǵ� ��
	uint32			nSendCPS;				///< �ʴ� ���� Ŀ�ǵ� ��
	uint32			nLocalCPS;				///< �ʴ� ���� Ŀ�ǵ� ��

	// �׽�Ʈ ����

	PServerInfo()
	{
		strServerName = DEFAULT_SERVER_NAME;
		nStat = SERVER_STAT_INACTIVE;
		nCurrGameServerCount = 0;
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
