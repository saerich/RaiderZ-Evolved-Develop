#pragma once

#include "MTypes.h"
#include "CSDef.h"
#include "SDef.h"
#include "STypes.h"

#pragma pack(push, old)
#pragma pack(1)


struct TD_SERVER_LIST_NODE
{
	int nServerID;
	uint8 nServerType;
};

//
// �α��μ����� ���� �α��� ó����
//

// ���� �ʵ� ����
struct TD_LOGIN_FIELD_INFO
{
	int		nSharedFieldID;
	int		nDynamicFieldID;
	MUID	uidDynamicFieldGroup;

	TD_LOGIN_FIELD_INFO()
	: nSharedFieldID(0)
	, nDynamicFieldID(0)
	, uidDynamicFieldGroup(MUID::ZERO)
	{}
};

// ���� ���� ����
struct TD_LOGIN_ACCOUNT_INFO
{
	uint32				nEnterKey;							// Client IP Address
	uint32				nAID;								// User DB index
	int32				nCID;
	wchar_t				szUserID[ACCOUNT_NAME_LEN + 1];

	TD_LOGIN_ACCOUNT_INFO()
	: nAID(0)
	, nCID(0)
	, nEnterKey(0)
	{}
};

// ���Ͻ� �÷��̾� ���� ����ȭ ����ü
struct TD_PROXY_PLAYER_INFO
{
	AID		nAID;								// User DB index
	int		nCID;
	MUID	UID;
	wchar_t	szName[ACCOUNT_NAME_LEN + 1];
	int		nGameServerID;
	MUID	uidField;
	MUID	uidParty;
	int		nGuildID;
	int		nFieldID;
	int		nChannelID;		///< �����ʵ��� ��츸 ����.
	MUID	uidFieldGroup;	///< �����ʵ��� ��츸 ����.

	TD_PROXY_PLAYER_INFO()
	: nGameServerID(INVALID_ID)	
	, nGuildID(INVALID_ID)
	, nFieldID(INVALID_ID)
	, nChannelID(INVALID_CHANNELID)
	{
		szName[0] = L'\0';
	}
};


// �Ǹ�
struct TD_PMANG_USER_DATA
{
	wchar_t szUserSerial[PMANG_USER_SERIAL_LEN + 1];
	wchar_t szUserID[PMANG_USER_ID_LEN + 1];
	int8 nRegionCode;
	int8 nAgeCode;
	int8 nGenderCode;
	int16 nYearOfBirth;

	wchar_t szPCBID[PMANG_PCBID_LEN + 1];
	wchar_t szServiceType[PMANG_SERVICE_TYPE_LEN + 1];
	int16 nUserType;
};

#pragma pack(pop, old)
