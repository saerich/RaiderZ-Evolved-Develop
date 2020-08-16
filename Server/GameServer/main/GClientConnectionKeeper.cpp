#include "stdafx.h"
#include "GClientConnectionKeeper.h"

GClientConnectionKeeper::GClientConnectionKeeper(MNetClient* pNetClient, wstring strSeverIP, int nPort, SSystem* pSystem, int nTryTick, wstring strNetworkCardIP)
: SClientConnectionKeeper(pNetClient, strSeverIP, nPort, pSystem, nTryTick, strNetworkCardIP)
, m_connectionType(MASTER_SERVER)
{
	// do nothing
}

GClientConnectionKeeper::~GClientConnectionKeeper()
{
	// do nothing
}

void GClientConnectionKeeper::OnDisconnect()
{
	__super::OnDisconnect();

	// �����ͼ����� �ƴϸ�, �������� �õ��Ѵ�. (�����ͼ����� �������� ���������� �����ϱ� ������ �������� �ʿ䰡 ����)
	if (m_connectionType != MASTER_SERVER)
	{
		TryToConnect();
	}
}

void GClientConnectionKeeper::SetConnectionType(CONNECTION_TYPE connectionType)
{
	m_connectionType = connectionType;
}
