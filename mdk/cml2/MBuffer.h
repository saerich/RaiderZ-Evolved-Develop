#pragma once

#include "RLib.h"

class CML2_API MBuffer
{
public:
	MBuffer(void);
	~MBuffer(void);

public:
	//�Ҵ�/����
	void	AllocateBuffer(int Size);
	void	FreeBuffer(void);
	void	SetBuffer(char* pSource,int Size);

	//�ʱ�ȭ
	void	Reset(void);

	//����/�б�
	int		Append(char* pSource,int Size);
	void	Read(char* pDest,int ReadSize);

	//�����ڵ�
	int		GetBufferSize(void)		{	return m_BufferSize;	}	// ���ۿ� �Ҵ�� ��������� ũ��
	int		GetDataSize(void)		{	return m_SavePointer;	}	// ���ۿ� ����� ����Ÿ�� ũ��
	char*	GetBufferPointer(void)	{	return m_pBuffer;		}

public:
	int		GetEmptySize(void)			{	return m_BufferSize - m_SavePointer;			}
	int		GetRemainReadSize(void)		{	return m_SavePointer - m_ReadPointer;	}

private:
	char*	m_pBuffer;
	int		m_BufferSize;
	int		m_SavePointer;	//������ �����ؾ��� ��ġ ( ��ġ�� 0 ���� ���� ), �̰��� 4��� 0~3������ ���ۿ� �ڷᰡ ����Ǿ��ٴ� �ǹ�
	int		m_ReadPointer;	//�б⸦ �����ؾ��� ��ġ ( ��ġ�� 0 ���� ���� ) 
};