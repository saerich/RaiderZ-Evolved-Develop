#include "stdafx.h"
#include "MCommandStream.h"
#include "MPacketCrypter.h"

namespace minet {


MCommandStream::MCommandStream(MCommandManager*	pCmdMgr) 
{	
	m_pCommandManager = pCmdMgr;
	m_pPacketCrypter = NULL;

	m_nBufferNext = 0;
}

MCommandStream::~MCommandStream() 
{
	Clear();
}

bool MCommandStream::IsBufferEmpty() 
{
	if (m_nBufferNext == 0) return true;
	else return false;
}

bool MCommandStream::EstimateBufferToCmd() 
{
	if (m_nBufferNext < sizeof(MPacketHeader)) return false;

	MPacketHeader* pPacket = (MPacketHeader*)m_Buffer;
	if (m_nBufferNext < pPacket->nSize) return false;

	return true;
}

void MCommandStream::AddBuffer(char* pBuffer, int nLen) 
{
	if (nLen <= 0) return;
	if ((m_nBufferNext + nLen) >= COMMAND_BUFFER_LEN) 
	{
		_ASSERT(FALSE);
		return;
	}
	CopyMemory(m_Buffer+m_nBufferNext, pBuffer, nLen);
	m_nBufferNext += nLen;
}

int MCommandStream::JustAddBuffer( char* pBuffer, int nLen )
{
	if (nLen <= 0) return 0;
	
	int nRemainLen = nLen;

	// ��� ����⿡�� �����ϸ� �� ���Ѵ�.
	if (m_nBufferNext + nLen < sizeof(MPacketHeader))
	{		
		AddBuffer(pBuffer, nLen);
		return 0;
	}

	// ����� ���ٸ�, ����� �����.		
	if (m_nBufferNext < sizeof(MPacketHeader))
	{
		int nLackSize = sizeof(MPacketHeader) - m_nBufferNext;
		
		AddBuffer(pBuffer, nLackSize);
		nRemainLen -= nLackSize;
	}
	
	// Ŀ�ǵ� ����⿡ �����ϸ� �� ���Ѵ�.
	MPacketHeader* pPacket = (MPacketHeader*)m_Buffer;
	int nReadedLen = nLen - nRemainLen;

	if (m_nBufferNext + nRemainLen < pPacket->nSize)
	{
		AddBuffer(pBuffer + nReadedLen, nRemainLen);
		return 0;
	}
	
	// Ŀ�ǵ� �ϳ��� ���Ѵ�.
	int nLackSize = pPacket->nSize - m_nBufferNext;
	AddBuffer(pBuffer + nReadedLen, nLackSize);
	nRemainLen -= nLackSize;

	return nRemainLen;
}

int MCommandStream::MakeCommand(char* pBuffer, int nBufferLen) 
{
	return m_CommandBuilder.BuildCommand(pBuffer, nBufferLen, m_CommandList, m_pPacketCrypter);
}

void MCommandStream::Clear()
{
	if (!m_CommandList.empty())
	{
		for (MCommandList::iterator itorCmd = m_CommandList.begin(); itorCmd != m_CommandList.end(); ++itorCmd)
		{
			MCommand* pCmd = (*itorCmd);
			SAFE_DELETE(pCmd);
		}
		m_CommandList.clear();
	}

}

bool MCommandStream::ReadBuffer( char* pBuffer, int nBufferLen )
{
	MPacketHeader* pPacket = (MPacketHeader*)pBuffer;

	if ( (nBufferLen < sizeof(MPacketHeader)) || (nBufferLen < pPacket->nSize) ) 
	{	
		AddBuffer(pBuffer, nBufferLen);	// ���� Buffer�� ����
	} 
	else 
	{	// Build Command --------------
		int nSpareData = MakeCommand(pBuffer, nBufferLen);			// Arg�� �Ѿ�� �ܺι��ۿ��� �ٷ� Cmd����
		if (nSpareData > 0) 
		{
			AddBuffer(pBuffer+(nBufferLen-nSpareData), nSpareData);	// �����κ� ���� Buffer�� ����(���ι���ó�����)
		}
		else if (nSpareData < 0) 
		{
			return false;
		}
	}

	return true;
}

bool MCommandStream::Read(char* pBuffer, int nBufferLen) 
{
	if (IsBufferEmpty() == true)  
	{
		return ReadBuffer(pBuffer, nBufferLen);
	} 
	else 
	{
		// ����� �κ� ��Ŷ�� ������ �����ŭ�� ���Ѵ�.
		int nRemainLen = JustAddBuffer(pBuffer, nBufferLen);

		if (EstimateBufferToCmd())
		{
			// ���ۿ��� Ŀ�ǵ� ����; �� �Ѱ��з��� ��� �ִ�.
			int nSpareData = MakeCommand(m_Buffer, m_nBufferNext);
			_ASSERT(nSpareData == 0);

			if (0 <= nSpareData)
			{			
				m_nBufferNext = 0;
			}
			else
			{
				return false;
			}

			// ���� �����Ͱ� ���� ������ �߰��� �д´�.
			if (0 < nRemainLen)
			{
				int nUnreadedBegin = nBufferLen - nRemainLen;
				return ReadBuffer(pBuffer + nUnreadedBegin, nRemainLen);
			}
		}
	}

	return true;
}

MCommand* MCommandStream::GetCommand() 
{
	MCommandList::iterator itorCmd = m_CommandList.begin();
	if (itorCmd != m_CommandList.end()) 
	{
		MCommand* pCmd = (*itorCmd);
		m_CommandList.pop_front();
		return (pCmd);
	} 
	else 
	{
		return NULL;
	}
}


void MCommandStream::InitCrypt(MPacketCrypter* pPacketCrypter, bool bCheckCommandSerialNumber)
{
	m_pPacketCrypter = pPacketCrypter;
	m_CommandBuilder.SetSerialCheckerActive(bCheckCommandSerialNumber);
}

} // namespace minet

