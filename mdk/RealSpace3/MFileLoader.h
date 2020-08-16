#ifndef _MFILELOAD_H
#define _MFILELOAD_H

/**
 Helper class �Դϴ�. Tool������ ���ñ� �ٶ��ϴ�. ( fopen���� ���� ������ �а� ���� �κ��� �����Ƿ� ... )
 */
namespace MFileLoad
{
	// 24��Ʈ BMP ������ �ε��Ͽ� ũ�� ������ �����ְ�, BYTE Array�� �����Ѵ�.
	RS_API unsigned char* CreateBitmapFrom24BMPFile(const char *filename, int *width, int *height, unsigned char fillAlpha = 0x00);

	// 8Bit-Raw�� 24��Ʈ BMP ���Ϸ� �����Ѵ�.
	RS_API bool SaveBitmapFrom8bitRawTo24BMPFile(const char *filename, int width, int height, void *pRawdata);

	// 24(32)Bit-Raw�� 24��Ʈ BMP ���Ϸ� �����Ѵ�.
	RS_API bool SaveBitmapFrom24bitRawTo24BMPFile(const char *filename, int width, int height, void *pRawdata);
	
	// (n)Bpp ������ DDS ���Ϸ� �����Ѵ�.
	RS_API bool SaveFromNBitRawToDDSFile(const char *filename, int width, int height, int bpp, void *pRawdata);

	// ARGB(32bit) ����� DDS ������ �о���δ�.
	RS_API BYTE* CreateARGBFromDDS(const char *filename, int *width, int *height);

	/**
	 * 24/32bit ���ϸ� ����ϼ���. (3, 4 Channel �뵵 ) �ٸ� ��Ʈ �� ������ �������� �ʾҽ��ϴ�.
	 * Wrap �� ��Ű�鼭, ���������� malloc/free�� ����ϴ� ���� new �� ���Ҵ��ؼ� ����ϵ��� �߽��ϴ�.
	 */
	
	// SOIL ���̺귯���� �̿��Ͽ�, ������ �д´�. ( bmp, tga, dds, png, jpg ���� )
	RS_API BYTE* SOIL_CreateRawFromFile(const char *filename, int *width, int *height, int *channel);
	
	// SOIL ���̺귯���� �̿��Ͽ� DXT1/5 �������� �����Ѵ�.
	RS_API bool SOIL_SaveRawToDXT(const char *filename, BYTE *pData, int width, int height, int channel);
};

#endif