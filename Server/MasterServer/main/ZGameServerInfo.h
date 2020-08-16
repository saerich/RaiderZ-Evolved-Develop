#pragma once

#include "ZFieldInfo.h"

class ZGameServerFieldInfo
{
public:
	int m_nFieldID;
	int m_nZoneID;
	ZFieldType	m_nFieldType;
	ZGameServerFieldInfo() : m_nFieldID(0), m_nZoneID(0), m_nFieldType(FT_SHARED) 
	{

	}
};

/// ���� ������ �Ҵ��ϴ� �ʵ� ����
struct ZManagedSharedField
{
	int				nFieldID;
	vector<int>		vecChannelID;			///< ���Ӽ��� ���� ��� �����ؾ��� ä�ε�.
	vector<int>		vecReservedChannelID;	///< ä���� Ȯ���� �� ������ ä�ε�.
	ZManagedSharedField() : nFieldID(0) {}
};

/// ���� ���� ����
class ZGameServerInfo
{
public:
	int								m_nID;							///< ���� ID
	vector<ZManagedSharedField>		m_vecManagedSharedField;		///< �Ҵ�� ���� �ʵ�


	ZGameServerInfo() : m_nID(0) 
	{

	}
};



