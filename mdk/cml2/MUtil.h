#ifndef _MUTIL_H
#define _MUTIL_H

#include "MSync.h"
#include <vector>
using namespace std;

// 10�������� ���� ������
#define SetBitSet(sets, item)		(sets |= (1 << item))
#define ClearBitSet(sets, item)		(sets &= ~(1 << item))
#define CheckBitSet(sets, item)		(sets & (1 << item))

/* ����
enum Item
{
	Foo1 = 1,
	Foo2 = 2,
	FooMax
};

unsigned long int sets = 0;
SetBitSet(sets, Foo1);
if (CheckBitSet(sets, Foo1)) extr;

*/


/** ��Ƽ�����忡 ������ ��������

	- �����(2006/02/22)
*/
class MRingBuffer
{
private:
	static const int	DEFAULT_RINGBUFFER_SIZE = 8192;		///< �⺻ ����ũ��� 8k�̴�.

	char*				m_pBuf;								///< ���� ����
	char*				m_pStartData;						///< ������ ���� ��ġ
	char*				m_pEndBuf;							///< ���� �� ��ġ
	char*				m_pCursor;							///< ������ �� ��ġ
	int					m_nBufSize;							///< ���� ������
	int					m_nDataSize;						///< ���ۿ� �����ϴ� ������ ������

	MCriticalSection	m_csLock;							///< �����忡 �����ϱ� ���ؼ�
public:
						MRingBuffer();						///< �⺻ ����ũ��� ����
						MRingBuffer(int nBufSize);			///< ���� ũ�⸦ ���Ͽ� ����
	virtual				~MRingBuffer();						///< �Ҹ���
	void				Reserve(int nBufSize);				///< ����ũ�⸦ ���Ҵ�. ���۾ȿ� �ִ� �����ʹ� �ʱ�ȭ�ȴ�.
	bool				Push(void* pBuf, int nSize);		///< ���ۿ� �����͸� �ִ´�.
	bool				Pop(void* pBuf, int nSize);			///< ���ۿ��� �����͸� ����.
	bool				ReadFront(void* pBuf, int nSize);	///< ������ ������ �պκ��� �д´�. ���� ����(����).
	int					GetSize();							///< ���ۿ� ����ִ� ������ ũ�⸦ ��ȯ�Ѵ�.
	int					GetCapacity();						///< ���� ��ü ũ�⸦ ��ȯ�Ѵ�.
};




// ����Ʈ ����
//		:��Ŷ ������������ �����Ǿ���
//
//  [7/25/2006 pyo] ����.

#include "MUID.h"
#include <string>
using namespace std;

#define DEFAULT_BYTE_BUFFER_SIZE 512

class MByteBuffer
{
private:
	char*		m_Buf;
	size_t		m_nMaxBufSize,
		m_nWritePos,
		m_nReadPos;
public:
	MByteBuffer(size_t nSize=0):m_Buf(0), m_nMaxBufSize(nSize), m_nWritePos(0), m_nReadPos(0) { this->SetMaxSize(); }
	~MByteBuffer() {}

	void SetMaxSize(size_t nSize=0);

	char*	GetBuffer();
	char*&	GetBufferRef();
	void	SetBuffer(char* buf);

	void	WritePos(size_t pos);
	size_t	WritePos();
	void	ReadPos(size_t pos);
	size_t	ReadPos();

	int WriteChar(const char data);
	int WriteInt(const int data);
	int WriteUInt(const unsigned int data);
	int WriteShort(const short data);
	int WriteUShort(const unsigned short data);
	int WriteLong(const long data);
	int WriteULong(const unsigned long data);
	int WriteFloat(const float data);
	int WriteDouble(const double data);
	int WriteString(const string data);
	int WriteUID(const MUID data);
	int WriteData(const void* data, size_t size);

	int ReadChar(char& data);
	int ReadInt(int& data);
	int ReadUInt(unsigned int& data);
	int ReadShort(short& data);
	int ReadUShort(unsigned short& data);
	int ReadLong(long& data);
	int ReadULong(unsigned long& data);
	int ReadFloat(float& data);
	int ReadDouble(double& data);
	int ReadString(string& data);
	int ReadUID(MUID& data);
	int ReadData(void* data, size_t size);

	friend int operator<<(MByteBuffer& lhs, char data);
	friend int operator<<(MByteBuffer& lhs, int data);
	friend int operator<<(MByteBuffer& lhs, unsigned int data);
	friend int operator<<(MByteBuffer& lhs, short data);
	friend int operator<<(MByteBuffer& lhs, unsigned short data);
	friend int operator<<(MByteBuffer& lhs, long data);
	friend int operator<<(MByteBuffer& lhs, unsigned long data);
	friend int operator<<(MByteBuffer& lhs, float data);
	friend int operator<<(MByteBuffer& lhs, double data);
	friend int operator<<(MByteBuffer& lhs, string data);
	friend int operator<<(MByteBuffer& lhs, MUID data);

	friend int operator>>(MByteBuffer& lhs, char& data);
	friend int operator>>(MByteBuffer& lhs, int& data);
	friend int operator>>(MByteBuffer& lhs, unsigned int& data);
	friend int operator>>(MByteBuffer& lhs, short& data);
	friend int operator>>(MByteBuffer& lhs, unsigned short& data);
	friend int operator>>(MByteBuffer& lhs, long& data);
	friend int operator>>(MByteBuffer& lhs, unsigned long& data);
	friend int operator>>(MByteBuffer& lhs, float& data);
	friend int operator>>(MByteBuffer& lhs, string& data);
	friend int operator>>(MByteBuffer& lhs, MUID& data);

private:
	void AutoSetMaxSizeImpl(size_t nSize);
};

bool SplitStrIP( const string& strIP, vector<BYTE>& vIP );

/// Win32 Windows ���α׷����� �ܼ��� ���� ����
void RedirectIOToConsole();

// Ÿ���� ���̰� �;��
#define scast static_cast
#define rcast reinterpret_cast
#define ccast const_cast
#define dcast dynamic_cast

// �ڵ� ���� �����
template <class T>
class AutoDelete
{
public:
	AutoDelete(T* const obj) : m_obj(obj), m_delete(true) {}
	~AutoDelete() { if (true == m_delete) delete m_obj; }

	void Cancel() { m_delete = false; }

private:
	bool		m_delete;
	T* const	m_obj;
};

#endif