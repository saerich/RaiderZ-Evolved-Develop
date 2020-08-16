#ifndef _MCOMMOBJECT_H
#define _MCOMMOBJECT_H

#include "MUID.h"
#include "MCommandManager.h"
#include "MPacketCrypter.h"
#include "MThread2.h"

namespace minet {

class MCommandStream;
class MCommandCommunicator;

/// Ŀ�´������Ϳ��� ������ ���� ��ü. MCommandCommunicator::Connect()�� �Ķ���ͷ� ����.
class MCommObject {
protected:
	MUID					m_uid;

	MCommandStream*			m_pCommandStream;
	MPacketCrypter			m_PacketCrypter;

	DWORD					m_dwUserContext;		// ���� ��ȣ

	char					m_szIP[128];
	int						m_nPort;
	DWORD					m_dwIP;
	bool					m_bAllowed;
public:
	MCommObject(MCommandCommunicator* pCommunicator);
	virtual ~MCommObject();
	void Init(MUID uidServer, MUID uidComm, DWORD dwUserContext, const char* pszIP, int nPort);

	MUID GetUID()			{ return m_uid; }

	MCommandStream*		GetCommandStream()				{ return m_pCommandStream; }

	DWORD GetUserContext()								{ return m_dwUserContext; }
	char* GetIPString()									{ return m_szIP; }
	const char* GetIPString() const						{ return m_szIP; }
	DWORD GetIP()										{ return m_dwIP; }
	int GetPort()										{ return m_nPort; }
	void SetAllowed(bool bAllowed) { m_bAllowed = bAllowed; }
	bool IsAllowed() { return m_bAllowed; }
};


class MCommObjectManager
{
private:
	typedef map<MUID, MCommObject*>		MCommObjectMap;
	typedef map<DWORD, MCommObject*>	MCommObjectKeyMap;

	MCommObjectMap					m_CommObjectMap;
	MCommObjectKeyMap				m_CommObjectKeyMap;
	cml2::MCriticalSection			m_csLock;
public:
	MCommObjectManager();
	~MCommObjectManager();
	void Create();
	void Destroy();
	void Add(MCommObject* pCommObj);
	void Delete(const MUID& uid);
	void Delete(DWORD nKey);
	void DeleteAll();
	int GetCount();
	void Lock();
	void Unlock();
	DWORD GetCommObjectKey(const MUID& uid);
	MUID GetCommObjectUID(const DWORD nClientKey);

	// �����忡 �������� ���� �Լ� - Lock, Unlock�� ���� �ɾ���� �Ѵ�.
	MCommObject* GetCommObjectByKey(DWORD nClientKey);


	/** UID�� ������ ���°� ��� ����.
	MCommObject�� �������°� ������ ����������� ���� ������
	������ MCommObject�� ���� ó���� �������� ����.
	�� ����ؼ� ����ϼ���.
	*/
	MCommObject* GetCommObjectByUID(const MUID& uidComm);
};




} // namespace minet

#endif