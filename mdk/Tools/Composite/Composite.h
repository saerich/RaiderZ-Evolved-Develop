#pragma once

/*
= ���� =
���� ����ũ �� : ������ ������ �ϴ� �ؽ��ķ�, 255�̸� ���� �ȼ��̰�,
0 �̸� �������� ���ɴϴ�.

= ���� =
���� ����ũ�ʰ� ���� �׽�Ʈ�� ���� ����� ��� �ϳ��� �ؽ��ķ� ��Ÿ���� ����
0~255 �� ���İ��� ������ ( �⺻�� 8 ) ���� �����ϰ�,
0~246 �� ���� ����ũ������ ���� 247~255 �� ���� �׽�Ʈ�� ���� �������� ���ϰ�,
�̸� �����ִ� ���α׷��̴�.  

= �˷��� ������ =
parts ������ ���� �ȼ��̴� �κ��� �����׽�Ʈ�� ���ٶ� ��谡 �ڿ������쳪 
parts ������ ���̴� �κ��� �������� ���ؼ� ��ŭ  ��迡 ���� diffuse ���� ������� ������ �ִ�.

�ΰ����� ������� dds �� ��ȯ�� mipmap ���� , dxtn ���� ������ �ִ�

*/

// �����׽�Ʈ�� ���� �Ҵ��� ����,, ���� 16���� ���������� �ʿ��ϴٸ� �ɼ����� ���������ϵ��� ����
const int REMAP_ALPHA_RANGE = 16;

// diffuse map �� parts �� ��� �ϳ��� ������ ����� ( �� ���� ���� )
bool MakeCompositeMap(LPDIRECT3DDEVICE9 device, const char* szDiffuseMapName, const char* szPartsColorMap, const char* szOutput);

/// targetFormat ���� ��ȯ�Ѵ�
bool ConvertTexture(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 pSrcTexture, D3DFORMAT targetFormat, const char* szOutput);

/// �Ϲ� = targetPainFormat , ���İ� �ִ°�� targetAlphaFormat ���� ��ȯ�Ѵ�
bool ConvertTexture(LPDIRECT3DDEVICE9 device, const char* szSrcFileName, D3DFORMAT targetPlainFormat, D3DFORMAT targetAlphaFormat, const char* szOutput);

/// ���� ���� �׽�Ʈ�� ����ϸ� parts color �� ���� �����Ƿ� ���ĸ� 0~255 -> (255-REMAP_ALPHA_RANGE)~255 �� ���������ִ� ���
bool RemapAlphaAndConvertTexture(LPDIRECT3DDEVICE9 device, const char* szSrcFileName, D3DFORMAT targetFormat, const char* szOutput);
