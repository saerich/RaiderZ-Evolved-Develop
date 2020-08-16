#ifndef _MMONITORMEMPOOL
#define _MMONITORMEMPOOL


// CML�� �ִ� MemPool.h�� ����.
// MMonitor������Ʈ������ ����ؾ� ��.
// MMonitor�� �ٸ� ������Ʈ�� �������̾�� �ؼ� ���� ������. - by SungE 2007-06-07




#include "Windows.h"
#include "Winbase.h"
// #include "MDebug.h"
#include "assert.h"


namespace mmonitor {


#define InitMonitorMemPool(T)		MMonitorMemPool<T>::_InitCS()
#define UninitMonitorMemPool(T)		MMonitorMemPool<T>::_DeleteCS();
#define ReleaseMonitorMemPool(T)	MMonitorMemPool<T>::_Release();

template< typename T >
class MMonitorMemPool
{
protected:
	static T*	m_list;
	T*			m_next;

	// Multi Thread���� �޸𸮸� ��ȣ�ϱ� ����
	static CRITICAL_SECTION m_csLock;

public:
	static void	_InitCS()
	{
		InitializeCriticalSection( &m_csLock );
	}

	static void	_DeleteCS()
	{
		DeleteCriticalSection( &m_csLock );
	}
	static void	_Release();

public:
	static void* operator new( size_t size_ );
	static void  operator delete( void* deadObject_, size_t size_ );
public:
	MMonitorMemPool(void)	{};
	~MMonitorMemPool(void)	{};
};

// new
template<typename T>
void* MMonitorMemPool<T>::operator new( size_t size_ )
{
	T* instance;
	EnterCriticalSection( &m_csLock );

	if( m_list != NULL )
	{
		instance	= m_list;
		m_list	= m_list->m_next;
	}
	else
	{
		instance = (T*)::operator new(size_);
	}

	LeaveCriticalSection( &m_csLock );

#ifdef _DEBUG
	if(size_ != sizeof(*instance))
		assert(0);
#endif

	return instance;
}

// delete
template<typename T>
void MMonitorMemPool<T>::operator delete( void* deadObject_, size_t size_ )
{
	EnterCriticalSection( &m_csLock );

	((T*)deadObject_)->m_next	= m_list;
	m_list	= (T*)deadObject_;

	LeaveCriticalSection( &m_csLock );
}

// Release
// ���� �Ҵ�Ǿ� ���� ���� �޸� ����~~
template<typename T>
void MMonitorMemPool<T>::_Release()
{
	// KeeperManager(PatchInterface)�� �������� Ŭ���̾�Ʈ�� ������ ����.
	//  2���̻��� Ŭ���̾�Ʈ�� ����ɶ� �̺κ��� �ߺ� ȣ���� �Ǿ�, 
	//  ��ȿ���� ���� m_csLock�� ũ��Ƽ�ü����� �����Ͽ� ���ܰ� �߻���.
	//  ����Ʈ�� �ش� �ּҰ� '0'�� �ƴҰ�츸 �����ϵ��� ����. - by �߱���.
	if( NULL != m_list ) 
	{
		EnterCriticalSection( &m_csLock );

		T* pInstace		= m_list;
		while( pInstace != NULL )
		{
			pInstace	= m_list->m_next;
			::operator delete( m_list );
			m_list	= pInstace;
		}

		LeaveCriticalSection( &m_csLock );
	}
}

template<typename T> CRITICAL_SECTION MMonitorMemPool<T>::m_csLock;
template<typename T> T* MMonitorMemPool<T>::m_list;


////////////////////////////////////////////////////////////////////////////
// ������ ��� ���ϴ�~
////////////////////////////////////////////////////////////////////////////

template < typename T >
class MMonitorMemPoolSm
{
protected:
	static T*	m_list;
	T*			m_next;

public:
	static void* operator new( size_t size_ );
	static void  operator delete( void* deadObject_, size_t size_ );

	static void _Release();

public:
	MMonitorMemPoolSm(void) {};
	~MMonitorMemPoolSm(void) {};
};


template<typename T>
void* MMonitorMemPoolSm<T>::operator new( size_t size_ )
{
	T* instance;

	if( m_list != NULL ) {
		instance = m_list;
		m_list = m_list->m_next;
	} else {
		instance = (T*)::operator new(size_);
	}

#ifdef _DEBUG
	if(size_ != sizeof(*instance))
		assert(0);
#endif

	return instance;
}

template<typename T>
void MMonitorMemPoolSm<T>::operator delete( void* deadObject_, size_t size_ )
{
	((T*)deadObject_)->m_next = m_list;
	m_list	= (T*)deadObject_;
}



// ���� �Ҵ�Ǿ� ���� ���� �޸� ����~~
template<typename T>
void MMonitorMemPoolSm<T>::_Release()
{
	T* pInstace	= m_list;
	while( pInstace != NULL ) {
		pInstace = m_list->m_next;
		::operator delete( m_list );
		m_list = pInstace;
	}
}

/////////////////////////////////////////////////////////////////////////

template<typename T> T* MMonitorMemPoolSm<T>::m_list;

}

#endif