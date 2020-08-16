// Ư�� �޸� ���� �迭�� �� ������ �ϳ��� �޸� ���� �ֱ� ���� �Լ���
#ifndef MBLOBARRAY_H
#define MBLOBARRAY_H

/// nOneBlobSize��ŭ nBlobCount������ŭ �迭�� �� �����
void* MMakeBlobArray(unsigned short nOneBlobSize, unsigned short nBlobCount);
/// �� �����
void MEraseBlobArray(void* pBlob);
/// ������ �� ���� ����
void* MGetBlobArrayElement(void* pBlob, int i);
/// ���� ���� ����
unsigned short MGetBlobArrayCount(void* pBlob);
/// ��� ��ü ������ ����
unsigned short MGetBlobArraySize(void* pBlob);
/// Array Block���� ���� �� ó�� ������ ����
void* MGetBlobArrayPointer(void* pBlob);

#endif