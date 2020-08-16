#ifndef MSHAREDCOMMANDTABLE_H
#define MSHAREDCOMMANDTABLE_H

#include "MiNetLib.h"

namespace minet {

class MCommandManager;


enum MCommandTable
{
// �⺻ Ŀ�ǵ� ����Ʈ
	MC_LOCAL_VERSION				= 1,		///< (A) Ŀ�ǵ� ���� Ȯ��

	MC_LOCAL_NET_CONNECT			= 101,		///< (C) Ŀ�´������Ϳ� ����
	MC_LOCAL_NET_CONNECT_WITH_NIC	= 102,		///< (C) Ŀ�´������Ϳ� ���� (������ ��Ʈ��ī�� ���)
	MC_LOCAL_NET_DISCONNECT			= 103,		///< (C) Ŀ�´������Ϳ� ���� ����
	MC_LOCAL_NET_ONCONNECT			= 104,		///< (C) Ŀ�´������Ϳ� ����Ǿ��� ��
	MC_LOCAL_NET_ONDISCONNECT		= 105,		///< (A) Ŀ�´������Ϳ� ������ ������ ��
	MC_LOCAL_NET_ONDISCONNECT_HARD	= 106,		///< (A) Ŀ�´������Ϳ� ������ ������ ��
	MC_LOCAL_NET_CHECKPING			= 107,		///< (A) Ŀ�´������� �����ӵ� �˻�
	MC_LOCAL_NET_ONERROR			= 108,		///< (A) Ŀ�´������Ϳ� ���� ����
	MC_LOCAL_NET_ONACCEPT			= 109,		///< (S) Ŀ�´������Ͱ� �������� ��
	MC_LOCAL_NET_CLEAR				= 110,		///< (S) Ŀ�´������Ϳ� ���� ���� �� �����ڿ� ó��

	MC_NET_REPLY_CONNECT			= 111,		///< (S->C) ���ῡ ���� ����. �̶� Ŭ���̾�Ʈ�� �Ҵ�� UID�� �޾ƿ´�.
	MC_NET_REALLOC_CLIENT_UID		= 112,		///< (S->C) Ŭ���̾�Ʈ�� UID ���Ҵ�

	MC_NET_PING						= 121,		///< Ŀ�´������� �����ӵ��˻� ��û
	MC_NET_PONG						= 122,		///< Ŀ�´������� �����ӵ��˻� ����
	MC_NET_KEEP_ALIVE				= 123,		///< (C->S) ���� ����
};


/// MINET���� ����ϴ� Ŀ�ǵ� ���
MINET_API void MAddSharedCommandTable();

#define SET_COMMAND(_a)				MCommand* pTempCmd = _a;	\
									int nParamIndex = 0


#define GET_PARAM(_a, _b)			if(pTempCmd->GetParameter(_a, nParamIndex++, MPT_##_b##, sizeof(_a)) == false)	\
											return CR_ERROR;														\

#define GET_PARAM_STR(_a)			if(pTempCmd->GetParameter(_a, nParamIndex++, MPT_STR) == false)	\
											return CR_ERROR;										\

#define GET_PARAM_WSTR(_a)			if(pTempCmd->GetParameter(_a, nParamIndex++, MPT_WSTR) == false) \
											return CR_ERROR;										 \


#define GET_PARAM_BLOB()			pTempCmd->GetParameter(nParamIndex)->GetPointer();						\
									if(pTempCmd->GetParameter(nParamIndex++)->GetType()!=MPT_BLOB) return
} // namespace minet

#endif