#pragma once

#include <windows.h>

namespace rs3 {

/**
 Trail ����Ʈ�� ���̴� ���� ť -
 �������̽��� CParticleArray�� ����ϴ�.

*/

template <class T>
class CParticleQueue
{
public:
	CParticleQueue()
	{
		m_pPool = NULL;
		m_uSize = 0;

		Init();
	}
	~CParticleQueue()
	{
		Release();
	}

	T* push(const T& value)
	{
		if ( m_uCount == m_uSize )
			return NULL;

		T *ptr = &m_pPool[m_nHeadIndex];
		*ptr = value;

		++m_nHeadIndex;
		m_nHeadIndex%=m_uSize;
		++m_uCount;

		return ptr;
	}

	T* peekTail()
	{
		if (empty())
			return NULL;

		return &m_pPool[m_nTailIndex];
	}

	T* peekHead()
	{
		if (empty())
			return NULL;
		
		int nIndex = (m_nHeadIndex-1);
		if ( nIndex < 0 )
			nIndex += m_uSize;

		return &m_pPool[nIndex];
	}

	void pop()
	{
		if (!empty())
		{
			++m_nTailIndex;
			m_nTailIndex %= m_uSize;
			--m_uCount;
		}
	}

	void pop(unsigned int n)
	{
		if (!empty() && n>0)
		{
			n = min(m_uCount, n);
			
			m_nTailIndex += n;
			m_nTailIndex %= m_uSize;
			m_uCount -= n;
		}
	}

	// count index �� �ֵ��� ��.
	T& operator[](unsigned int uIndex)
	{
		_ASSERT(m_pPool!=NULL);
		_ASSERT(uIndex<m_uCount);
		_ASSERT(uIndex>=0);

		return m_pPool[(m_nTailIndex +uIndex)%m_uSize];
	}

	void resize(unsigned int uSize)
	{
		Release();

		m_pPool = new T[uSize];
		m_uSize = uSize;

		Init();
	}

	unsigned int size()
	{
		return m_uSize;
	}

	unsigned int count()
	{
		return m_uCount;
	}

	bool empty()
	{
		return (m_uCount == 0);
	}

	int beginIndex()
	{
		return m_nHeadIndex;
	}

	int endIndex()
	{
		return m_nTailIndex;
	}

	void clear()
	{
		Init();
	}

protected:
	T* m_pPool;

	unsigned int m_uSize;

	int m_nHeadIndex;
	int m_nTailIndex;
	unsigned int m_uCount;

	void Init()
	{
		m_nHeadIndex = 0;
		m_nTailIndex = 0;
		m_uCount = 0;
	}

	void Release()
	{
		if ( m_pPool )
		{
			delete [] m_pPool;
			m_pPool = NULL;
		}
	}
};

/**

��ƼŬ�� ���� ���Ǵ� Ŭ���� CParticleArray
- ��ü ������ m_pMemPool �� �����, ���� ��Ҹ� ������ �迭 m_ppActive�� �־���.
- m_ppActive�� m_uActiveCount ���� ���Ǵ� ������ �����͸� ��� �ִ�.
- �׷��� ���ؼ�, ������ �� �� ������ ������ ���� ��Ҹ� ����� ä���.

*/

template <typename T> class CParticleArray
{
protected:
	T *m_pMemPool;
	T **m_ppActive;

	unsigned int m_uNum;
	unsigned int m_uActiveCount;

	void Init()
	{
		m_pMemPool = new T[m_uNum];
		if ( m_pMemPool == NULL )
			return;

		m_ppActive = new T*[m_uNum];
		memset(m_ppActive, 0, sizeof(T*)*m_uNum);

		for ( unsigned int ui = 0; ui<m_uNum; ++ui )
			m_ppActive[ui] = &m_pMemPool[ui];

		m_uActiveCount = 0;
	}
	void Release()
	{
		if ( m_ppActive )
		{
			delete [] m_ppActive;
			m_ppActive = NULL;
		}
		if ( m_pMemPool )
		{
			delete [] m_pMemPool;
			m_pMemPool = NULL;
		}

		m_uNum = m_uActiveCount = 0;
	}
public:
	CParticleArray() : m_pMemPool(NULL), m_ppActive(NULL), m_uNum(0), m_uActiveCount(0) {}
	CParticleArray(unsigned int uNum) : m_pMemPool(NULL), m_ppActive(NULL), m_uNum(uNum)
	{
		Init();
	}

	~CParticleArray()
	{
		Release();
	}

	T& operator[](unsigned int uIndex)
	{
		// �� Index���� ���� �ǵ帮�� �ȵ�.
		assert(uIndex<m_uActiveCount);

		return *m_ppActive[uIndex];
	}

	T erase(unsigned int uIndex)
	{
		assert(uIndex<m_uActiveCount);
		assert(m_uActiveCount>0);

		// PackArray�� ���� �����̳�, ���� Point Indexing�� �̿� ���� �� ����ȭ ���״�.
		unsigned int uLastIndex = m_uActiveCount - 1;
		T* ptr = m_ppActive[uIndex];
		m_uActiveCount--;

		if ( uLastIndex == uIndex )
			return *ptr;

		m_ppActive[uIndex] = m_ppActive[uLastIndex];
		m_ppActive[uLastIndex] = ptr;

		return *ptr;
	}

	void resize(unsigned int uNum)
	{
		Release();

		m_uNum = uNum;
		Init();
	}

	void reset()
	{
		m_uActiveCount = 0;
	}

	T* getAddPtr()	// ���� �ϳ� ���̰�, �� ������ �����͸� ��´�.
	{
		if ( m_uActiveCount == m_uNum )
			return NULL;

		T* ptr = m_ppActive[m_uActiveCount];
		++m_uActiveCount;

		return ptr;
	}

	unsigned int count()
	{
		return m_uActiveCount;
	}

	unsigned int size()
	{
		return m_uNum;
	}

	T* getPtr(unsigned int uIndex)
	{
		// �� Index���� ���� �ǵ帮�� �ȵ�.
		assert(uIndex<m_uActiveCount);
		return m_ppActive[uIndex];
	}

	T** getActiveBeginPtr()
	{
		return &m_ppActive[0];
	}

	T** getActiveEndPtr()
	{
		return &m_ppActive[m_uActiveCount];
	}
};

}
