#include "stdafx.h"
#include "MBlobArray.h"
#include <memory.h>

void* MMakeBlobArray(unsigned short nOneBlobSize, unsigned short nBlobCount)
{
	// nBlobCount����ŭ Blob�� �ΰ� ����(ī��Ʈ)�� ���� ���� ���, �� ó���� �ΰ� ����(ī��Ʈ)�� nOneBlobSize�� nBlobCount���� �ִ´�.
	unsigned char* pBlob = new unsigned char[sizeof(nOneBlobSize)+sizeof(nBlobCount)+nOneBlobSize*nBlobCount];
	memcpy(pBlob, &nOneBlobSize, sizeof(nOneBlobSize));
	memcpy(pBlob+sizeof(nBlobCount), &nBlobCount, sizeof(nOneBlobSize));
	return pBlob;
}

void MEraseBlobArray(void* pBlob)
{
	// MBlob�� �� �Լ��� ������� ����. - By SungE 2007-09-11

	delete[] pBlob;
}

void* MGetBlobArrayElement(void* pBlob, int i)
{
	// MBlob�� �� �Լ��� ������� ����. - By SungE 2007-09-11

	unsigned short nBlobCount = 0;
	unsigned short nOneBlobSize = 0;
	memcpy(&nOneBlobSize, (unsigned char*)pBlob, sizeof(nOneBlobSize));
	memcpy(&nBlobCount, (unsigned char*)pBlob+sizeof(nOneBlobSize), sizeof(nBlobCount));
	if(i<0 || i>=nBlobCount) return 0;

	return ((unsigned char*)pBlob+sizeof(unsigned short)*2+nOneBlobSize*i);
}

unsigned short MGetBlobArrayCount(void* pBlob)
{
	// MBlob�� �� �Լ��� ������� ����. - By SungE 2007-09-11

	unsigned short nBlobCount;
	memcpy(&nBlobCount, (unsigned char*)pBlob+sizeof(unsigned short), sizeof(nBlobCount));
	return nBlobCount;
}

unsigned short MGetBlobArraySize(void* pBlob)
{
	// MBlob�� �� �Լ��� ������� ����. - By SungE 2007-09-11

	unsigned short nBlobCount, nOneBlobSize;
	memcpy(&nOneBlobSize, (unsigned char*)pBlob, sizeof(nOneBlobSize));
	memcpy(&nBlobCount, (unsigned char*)pBlob+sizeof(unsigned short), sizeof(nBlobCount));

	return (nOneBlobSize*nBlobCount+sizeof(unsigned short)*2);
}

void* MGetBlobArrayPointer(void* pBlob)
{
	// MBlob�� �� �Լ��� ������� ����. - By SungE 2007-09-11

	return ((unsigned char*)pBlob+sizeof(unsigned short)*2);
}