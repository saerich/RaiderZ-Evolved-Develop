#include "stdafx.h"

#include "Mint4Null.h"
#include "MWidget.h"
#include "Mint.h"

using namespace rs3;




MBitmap* Mint4Null::NewBitmap(const char* szName, const char* szFileName)
{
	// ���� �̸����δ� ����� �ȵǵ���.
	if ( MBitmapManager::IsExist( szName) == true)		return NULL;

	MBitmapNull* pBitmap = new MBitmapNull();
	if ( pBitmap->Create( szName, szFileName) == false)
	{
		delete pBitmap;
		return NULL;
	}

	return pBitmap;
}
