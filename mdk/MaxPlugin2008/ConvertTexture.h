#pragma once

/*
dds �� ��ȯ�� mipmap ���� , dxtn ���� ����

*/

/// targetFormat ���� ��ȯ�Ѵ�
bool ConvertTexture(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 pSrcTexture, D3DFORMAT targetFormat, const char* szOutput);

/// �Ϲ� = targetPainFormat , ���İ� �ִ°�� targetAlphaFormat ���� ��ȯ�Ѵ�
bool ConvertTexture(LPDIRECT3DDEVICE9 device, const char* szSrcFileName, D3DFORMAT targetPlainFormat, D3DFORMAT targetAlphaFormat, const char* szOutput);
