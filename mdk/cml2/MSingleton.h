#ifndef _MSINGLETON_H
#define _MSINGLETON_H

#include <cassert>

template <typename T> class MSingleton
{
    static T* m_pSingleton;

public:
    MSingleton( void )
    {
        assert( !m_pSingleton );

		/* 
		���߻���� �޾����� �����Ͱ� �ٸ��� ���� ������ �ذ��� ��Ŭ�� ���ø�

		gpg �� ���� ���, VS6 ���Ϲ�����
        int offset = (int)((T*)1) - (int)(MSingleton <T>*)((T*)1);
        m_pSingleton = (T*)((int)this + offset);
		*/
        
		// �̰��� ǥ�ؿ� ����� ����ε�.
        m_pSingleton = static_cast<T *>(this); 

	}
   ~MSingleton( void )
        {  assert( m_pSingleton );  m_pSingleton = 0;  }
    static T& GetInstance( void )
        {  assert( m_pSingleton );  return ( *m_pSingleton );  }
    static T* GetInstancePtr( void )
        {  return ( m_pSingleton );  }
};

template <typename T> T* MSingleton <T>::m_pSingleton = 0;

//////////////////////////////////////////////////////////////////////////
// _num ������ŭ �ν��Ͻ��� �����ϴ� �̱���
//
template <typename T, int _num = 1> class MSingletonRestricted
{
	static int m_nCount;

public:
	MSingletonRestricted( void )
	{
		++m_nCount;
		assert( m_nCount <= _num );
	}
	~MSingletonRestricted( void )
	{
		--m_nCount;
		assert( m_nCount >= 0 );
	}
};

template <typename T, int _num > int MSingletonRestricted<T, _num>::m_nCount = 0;


#endif