#pragma once

#include "MUID.h"
#include "MSync.h"
#include "ServerCommonLib.h"

class SCommObject;

/// Client ������
class SCOMMON_API SCommObjectManager
{
public:
	typedef map<MUID, SCommObject*> SCommObjectMap;
protected:
	SCommObjectMap			m_ClientObjectMap;
	MCriticalSection		m_Lock;
public:
	SCommObjectManager();
	virtual ~SCommObjectManager();
	virtual SCommObject* NewObject(MUID& uid);
	virtual void AddObject(SCommObject* pClient);
	virtual void DeleteObject(MUID uidClient);
	virtual void DeleteAllObject();
	virtual void Clear();

	int  GetClientsCount() { return (int)m_ClientObjectMap.size(); }

	// �����忡 �������� ���� �Լ� - Lock, Unlock�� ����ؾ��Ѵ�.
	void Lock();
	void Unlock();
	SCommObject* GetClient(const MUID& uidClient);			// �����忡 �������� �ʴ�.
	SCommObjectMap& GetObjects()	{ return m_ClientObjectMap; }	
};
