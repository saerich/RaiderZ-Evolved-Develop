#ifndef _MPACKET_H
#define _MPACKET_H

#include "MCommand.h"
#include "MCommandManager.h"

namespace minet {


#define MAX_PACKET_SIZE			32768		// �ִ� ��Ŷ ũ��� 32k	

#define MSG_COMMAND				0x0A
#define MSGID_RAWCOMMAND		0x1A		// ��ȣȭ �ȵ� Ŀ�ǵ�
#define MSGID_COMMAND			0x29		// ��ȣȭ�� Ŀ�ǵ�


class MPacketCrypter;

#pragma pack(1)

struct MPacketHeader
{
	unsigned char	nMsg;
	unsigned short	nSize;
	unsigned short	nCheckSum;		// üũ��. ������ 4bit�� �ø��� ��ȣ üũ

	MPacketHeader() { nMsg=MSG_COMMAND; nSize=0; nCheckSum=0; }
};

struct MCommandMsg : public MPacketHeader
{
	char	Buffer[1];
};

#pragma pack()

// Tiny CheckSum for MCommandMsg
inline unsigned short MBuildCheckSum(MPacketHeader* pPacket, int nPacketSize)
{
	int nStartOffset = sizeof(MPacketHeader);
	BYTE* pBulk = (BYTE*)pPacket;

	unsigned long nCheckSum = 0;
	for (int i=nStartOffset; i<nPacketSize; i++) 
	{
		nCheckSum += pBulk[i];
	}
	nCheckSum -= (pPacket->nMsg + nPacketSize);
	unsigned short nShortCheckSum = HIWORD(nCheckSum) + LOWORD(nCheckSum);
	return nShortCheckSum;
}

inline unsigned short MBuildCheckSumAndSerialNumber(MPacketHeader* pPacket, int nPacketSize, uint8 nSerialNumber)
{
	unsigned short nRet = MBuildCheckSum(pPacket, nPacketSize);

	// ������ 4bit�� �ø��� ��ȣ üũ�� ���
	nRet = (nRet & 0xFFF0) | (nSerialNumber & 0x000F);
	return nRet;
}

inline bool IsEqualCheckSum(unsigned short nSrcCheckSum, MPacketHeader* pPacket, int nPacketSize)
{
	unsigned short nTarCheckSum = MBuildCheckSum(pPacket, nPacketSize);

	return ((nSrcCheckSum & 0xFFF0) == (nTarCheckSum & 0xFFF0));
}

inline uint8 GetPacketSerialNumberFromChecksum(unsigned short nPacketCheckSum)
{
	return (nPacketCheckSum & 0x000F);
}


inline int CalcPacketSize(MCommand* pCmd)
{
	return (sizeof(MPacketHeader) + pCmd->GetSize());
}

} // namespace minet

#endif
