#include "stdafx.h"
#include "MiNet.h"
#include "MCommandTable.h"
#include "MCommandDesc.h"
#include "MiNetLogger.h"
#include "MAct.h"

namespace minet {

int	g_nCommandVersion = 0;
MiNetLogger* g_pMiNetLogger = NULL;

bool Init(const int nCommandVersion)
{
	WSADATA	wsaData;

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
	{
		return false;
	}

	g_nCommandVersion = nCommandVersion;
	g_pMiNetLogger = new MiNetLogger();

	MAddSharedCommandTable();

	return true;
}

void MiNetMemPoolRelease()
{

//	MTcpActAccept::release();
}

void Fini()
{
	ClearCommandDescs();
	WSACleanup();
	SAFE_DELETE(g_pMiNetLogger);

	MiNetMemPoolRelease();
}


const int GetCommandVersion()
{ 
	return g_nCommandVersion; 
}

void ClearCommandDescs()
{
	MGetCommandDescMap()->Clear();
}

} // namespace minet


// ��Ʈ
//
// MNetServer�� �ִ��� main �����忡���� ����ϵ��� �Ѵ�.