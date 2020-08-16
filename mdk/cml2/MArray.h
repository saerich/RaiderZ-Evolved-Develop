#ifndef _MARRAY_H
#define _MARRAY_H

// 2006.3.2 dubble
// std::vector �� ������ clear �ÿ� capacity �� �ʱ�ȭ ���� �ʴ´�.

// thread �� safe���� �����Ƿ� ��뿡 ���Ǹ� ����.

#include <cassert>

using namespace std;

#define INITIAL_SIZE		16			// �ʱ� ���� ����

template <class _TYPE>

class MArray {

protected:
	_TYPE *pArray;					// ���� �޸� ��
	unsigned int m_nCapasity;		// ��ü ��������� ��
	unsigned int m_nCount;			// �������� ����ִ� ������ ��

public:
	MArray(){
		m_nCount = m_nCapasity = 0;
		pArray = NULL;

		if(INITIAL_SIZE) Reserve(INITIAL_SIZE);
	}
	
	~MArray(){
		free(pArray);
	}

	_TYPE &operator [] (const unsigned int index) const {
		return pArray[index];
	}

	_TYPE & Get(const unsigned int index) const {
		return pArray[index];
	}

	_TYPE *GetMArray() const { return pArray; }
	
	unsigned int GetCount() const { return m_nCount; }
	unsigned int GetCapasity() const { return m_nCapasity; }

	unsigned int Add(const _TYPE object){
		if (m_nCount >= m_nCapasity){
			Reserve(m_nCapasity*2);
		}

		pArray[m_nCount++] = object;
		return m_nCount-1;
	}

//	void Remove(const unsigned int index)	// �� �ʿ��ϸ� ����� �����ϸ� �������ô�

	void Reserve(const unsigned int newCapasity){
		assert(newCapasity>=m_nCapasity);
		m_nCapasity = newCapasity;
		pArray = (_TYPE *) realloc(pArray, m_nCapasity * sizeof(_TYPE));		
	}

	void Clear(){ m_nCount = 0; }
};


#endif