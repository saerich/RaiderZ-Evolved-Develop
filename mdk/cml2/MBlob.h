#ifndef _MBLOB_H
#define _MBLOB_H

#pragma warning( disable : 4251 )

#include <vector>
#include "RLib.h"
using namespace std;

class CML2_API MBlob
{
private:
	unsigned short			m_nTypeSize;	///< Type size�� 0xffff�� ���� �� ����.
	unsigned short			m_nBlobCount;	///< Blob Count�� 0xffff�� ���� �� ����.
	vector<unsigned char>	m_vecData;		///< m_vecBlob���� ���� �����͸� ����.
public:
	MBlob(void);
	/// nTypeSizeũ��� nBlobCount������ŭ �迭�� �� �����
	MBlob(const void* pData, unsigned short nTypeSize, unsigned short nBlobCount);
		
	int SetData(const char* pData, const int nDataSize);	///< Data�� Blob�� ����
	int GetData(char* pBuffer, const int nBufferSize);		///< Blob������ data�� ����

	/// �� ������ ����
	inline int GetBlobSize();

	/// ���� ���� ����
	inline int GetBlobCount();

	/// �� ��ü ������ ����
	int GetBlobTotalSize();

	/// ������ �� ���� ����
	void* GetBlobElement(int i);

	/// ���� ���Ҵ����� ������ �ֱ�
	bool SetBlobElement(int i, const void* pData, const int nTypeSize);

	/// Array Block���� ���� �� ó�� ������ ����
	void* GetBlobPointer();

	void Clear();
};

inline int MBlob::GetBlobSize()
{
	return m_nTypeSize;
}

inline int MBlob::GetBlobCount()
{
	return m_nBlobCount;
}

#endif // _MBLOB_H