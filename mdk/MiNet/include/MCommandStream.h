#ifndef MCOMMAND_STREAM_H
#define MCOMMAND_STREAM_H

#include "MiNetPrerequisites.h"
#include "MCommandManager.h"
#include "MDebug.h"
#include "MCRC32.h"
#include "MPacket.h"
#include "MCommandBuilder.h"


namespace minet {

class MPacketCrypter;



/// MCommandStream : �ϰ�������� Stream�� ���۸��ѵ� Command�� �����Ѵ�.
class MINET_API MCommandStream 
{
private:
	static const int		COMMAND_BUFFER_LEN = 32768;

	MCommandManager*		m_pCommandManager;

	char					m_Buffer[COMMAND_BUFFER_LEN];
	int						m_nBufferNext;

	MCommandList			m_CommandList;

	MPacketCrypter*			m_pPacketCrypter;			// ��ȣȭ
	MCommandBuilder			m_CommandBuilder;
private:
	bool					IsBufferEmpty();
	bool					EstimateBufferToCmd();
	void					AddBuffer(char* pBuffer, int nLen);
	int						JustAddBuffer(char* pBuffer, int nLen);
	int						MakeCommand(char* pBuffer, int nBufferLen);
	bool					ReadBuffer( char* pBuffer, int nBufferLen );
	void					Clear();

public:
							MCommandStream(MCommandManager* pCmdMgr);
							~MCommandStream();
	bool					Read(char* pBuffer, int nBufferLen);
	
	MCommand*				GetCommand();

	void					InitCrypt(MPacketCrypter* pPacketCrypter, bool bCheckCommandSerialNumber);
};



} // namespace minet

#endif