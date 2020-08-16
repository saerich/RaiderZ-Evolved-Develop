#ifndef MCOMMANDMANAGER_H
#define MCOMMANDMANAGER_H

#include "MiNetLib.h"
#include "MThread2.h"
#include "MCommand.h"
#include "MCommandParameter.h"
#include "MCommandHandler.h"
#include <map>
#include <string>
using namespace std;

namespace minet {

typedef list<MCommand*>			MCommandList;		///< Command List

#define DEFAULT_COMMAND_QUEUE_MAX_SIZE			500000		// 50���� �̻� ť�� ���̸� Ŀ�ǵ� �߰����� ����

/// Ŀ�ǵ� �Ŵ���
/// - Ŀ�ǵ� ��ũ���� ����
/// - Ŀ�ǵ� ť ����
/// - �����忡 ������.
class MINET_API MCommandManager
{
protected:
	cml2::MCriticalSection		m_CommandQueueLock;
	MCommandList				m_CommandQueue;			///< Posted Command List
	size_t						m_nMaxQueueSize;
public:
	MCommandManager(void);
	virtual ~MCommandManager(void);
	void Clear();

	/// Ŀ�ǵ� Posting
	bool Post(MCommand* pNew);

	/// ���� ���� �߰��� Ŀ�ǵ带 �о����
	MCommand* GetCommand(void);

	/// Ŀ�ǵ� ť �ִ� ũ�� ����
	void SetMaxQueueSize(size_t nSize)	{ m_nMaxQueueSize = nSize; }

	/// Ŀ�ǵ� ť �ִ� ũ��
	size_t GetMaxQueueSize()			{ return m_nMaxQueueSize; }

	/// �ڵ鷯 ����
	void SetCommandHandler(int nCmdID, MCommandHandler* pHandler, MCommandHanderFunc* fnFunc);

	size_t GetCommandQueueCount(void);

	/// Command Description�� ���� ���� ��Ʈ�� ����
	void GetSyntax(char* szSyntax, int nSyntaxBufferSize, const MCommandDesc* pCD);
};


} // namespace minet

#endif