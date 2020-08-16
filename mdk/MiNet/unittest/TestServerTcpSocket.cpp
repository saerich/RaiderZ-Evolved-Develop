#include "stdafx.h"
#include "MSocket.h"
#include "MServerTcpSocket.h"

using namespace minet;

SUITE(ServerTcpSocketTest)
{
	TEST(ServerTcpSocket_IPV4)
	{
		MServerTcpSocket* pSocket = new MServerTcpSocket(NULL, NULL, NULL, NULL);
		assert(NULL!=pSocket);

		CHECK_EQUAL(true, pSocket->Init(AF_INET));
		CHECK_EQUAL(true, pSocket->ReInit());
		CHECK(INVALID_SOCKET != pSocket->GetSocket() && NULL != pSocket->GetSocket());
		MUID id;
		id.Increase();
		pSocket->SetLink(id);
		CHECK_EQUAL(id, pSocket->GetLink());
		pSocket->ReleaseLink();
		CHECK_EQUAL(MUID::ZERO, pSocket->GetLink());

		//todo iocp�� ���?? 
// 		pSocket->Recv();
// 		byte buf[1024] = {0, };
// 		pSocket->Send(buf, 1024);

//		bool Reuse();
// 		bool Disconnect();
// 		void HardDisconnect(MUID uidLink);
// 
// 		void Shutdown(int how = SD_BOTH);

// 
// 		// ������ ��Ŷ�� ���� Ƚ�� �������� ������, �������� ������ ����ٰ� �Ǵ��Ѵ�.
// 		void ResetSendPendingCounter();
// 		bool IncreaseSendPendingCounter();
// 		void DecreaseSendPendingCounter();
// 		void SetSendPendingLimitCount(int nLimitCount);

		
		delete pSocket;
	}
}