#ifndef _MSYNC_H
#define _MSYNC_H

//#pragma once
#include "RLib.h"

#include <Windows.h>

class CML2_API MCriticalSection 
{
private:
	CRITICAL_SECTION	m_cs;

public:
#ifdef _DEBUG
	MCriticalSection() : m_nCount(0)
	{
		InitializeCriticalSection(&m_cs);
		m_bUsableCS = true;
	}
	~MCriticalSection()
	{
		DeleteCriticalSection(&m_cs);
		m_bUsableCS = false;
	}
#else
	MCriticalSection()	{ InitializeCriticalSection(&m_cs); }
	~MCriticalSection() { DeleteCriticalSection(&m_cs); }
#endif

	void Lock()			{ EnterCriticalSection(&m_cs); }
	void Unlock()		{ LeaveCriticalSection(&m_cs); }

	class Guard
	{
	private:
		MCriticalSection& m_csSyncObject;

		// ��ü�� �����ϸ� �����̹Ƿ�..
		Guard(const Guard& rhs);
		Guard &operator=(const Guard& rhs);

	public:
		explicit Guard(MCriticalSection & cs) : m_csSyncObject(cs)
		{
//			_ASSERT( m_csSyncObject.m_bUsableCS == true );
			m_csSyncObject.Lock();
			#ifdef _DEBUG
			++ m_csSyncObject.m_nCount;
			_ASSERT( m_csSyncObject.m_nCount <= 1);
			#endif

		}
		~Guard()
		{
			#ifdef _DEBUG
			-- m_csSyncObject.m_nCount;
			#endif
			_ASSERT( m_csSyncObject.m_nCount == 0 );
			m_csSyncObject.Unlock();
		}
	};

// by pok
#ifdef _DEBUG
	int  m_nCount;
	bool m_bUsableCS;
#endif

};


class CML2_API MCriticalSectionSpinLock
{
private:
	CRITICAL_SECTION	m_cs;

public:
#ifdef _DEBUG
	MCriticalSectionSpinLock() : m_nCount(0)
	{
		InitializeCriticalSectionAndSpinCount(&m_cs,4000);
		m_bUsableCS = true;
	}
	~MCriticalSectionSpinLock()
	{
		DeleteCriticalSection(&m_cs);
		m_bUsableCS = false;
	}
#else
	MCriticalSectionSpinLock()	{ 
		InitializeCriticalSectionAndSpinCount(&m_cs,4000);
	}
	~MCriticalSectionSpinLock() { DeleteCriticalSection(&m_cs); }
#endif

	void Lock()			{ EnterCriticalSection(&m_cs); }
	void Unlock()		{ LeaveCriticalSection(&m_cs); }

	class Guard
	{
	private:
		MCriticalSectionSpinLock& m_csSyncObject;

		// ��ü�� �����ϸ� �����̹Ƿ�..
		Guard(const Guard& rhs);
		Guard &operator=(const Guard& rhs);

	public:
		explicit Guard(MCriticalSectionSpinLock & cs) : m_csSyncObject(cs)
		{
			//			_ASSERT( m_csSyncObject.m_bUsableCS == true );
			m_csSyncObject.Lock();
#ifdef _DEBUG
			++ m_csSyncObject.m_nCount;
			_ASSERT( m_csSyncObject.m_nCount <= 1);
#endif

		}
		~Guard()
		{
#ifdef _DEBUG
			-- m_csSyncObject.m_nCount;
#endif
			_ASSERT( m_csSyncObject.m_nCount == 0 );
			m_csSyncObject.Unlock();
		}
	};

	// by pok
#ifdef _DEBUG
	int  m_nCount;
	bool m_bUsableCS;
#endif

};

/// Ŭ���� ���� ���� ������ ����ȭ
template <class T>
class MClassLevelLockable
{
private:
	static MCriticalSection		m_stCS;
public:
	class Lock;
	friend class Lock;
	typedef volatile T VolatileType;

	MClassLevelLockable() {}

	class Lock
	{
	public:
		Lock(T& host) { MClassLevelLockable<T>::m_stCS.Lock(); }
		Lock(MClassLevelLockable<T> &) { MClassLevelLockable<T>::m_stCS.Lock(); }
		Lock()	{ MClassLevelLockable<T>::m_stCS.Lock(); }
		~Lock()	{ MClassLevelLockable<T>::m_stCS.Unlock(); }
	};
};

template<class T> MCriticalSection MClassLevelLockable<T>::m_stCS;


class CML2_API MSignalEvent {
	HANDLE		m_hEvent;

public:
	MSignalEvent() { m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL); }
	~MSignalEvent() { if (m_hEvent) { CloseHandle(m_hEvent); m_hEvent = NULL; } }
	HANDLE GetEvent() { return m_hEvent; }
	BOOL SetEvent() { return ::SetEvent(m_hEvent); }
	BOOL ResetEvent() { return ::ResetEvent(m_hEvent); }
};


// ���ø����̼��� ���ÿ� �ΰ��ߴ°��� ���ų� ��ٸ��� �����ִ� Ŭ����
class MSingleRunController {
protected:
	char	m_szAppName[_MAX_DIR];
	HANDLE	m_Mutex;
public:
	//Usage : 	if (g_SingleRunController.Create(true) == false) return FALSE;
	MSingleRunController(const char* pszAppName)	{ 
		m_Mutex = NULL;
		strcpy_s(m_szAppName, pszAppName); 
	}
	virtual ~MSingleRunController()	{ 
		Destroy();
	}	
	bool Create(bool bWaitMode=false) {	// bMode=true(���డ�ɶ����� �����), bMode==false(�ʰ� ����� �ν��Ͻ� �ٷ�����)
		if (bWaitMode == true) {
			int tmTotalWait = 0;
			for( ; ; ) {
				m_Mutex = CreateMutexA(NULL, TRUE, m_szAppName);
				if (GetLastError() == ERROR_ALREADY_EXISTS)	{
					CloseHandle(m_Mutex);

					Sleep(100);
					tmTotalWait += 100;
					if (tmTotalWait >= 5*60*1000) // 5���̻��� ��������
						return false;
				} else {
					return true;
				}
			}
		} else {
			m_Mutex = CreateMutexA(NULL, TRUE, m_szAppName);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
				return false;
			else
				return true;
		}
	}
	void Destroy() {
		if (m_Mutex) {
			ReleaseMutex(m_Mutex);	
			m_Mutex = NULL;
		}
	}
};


#endif