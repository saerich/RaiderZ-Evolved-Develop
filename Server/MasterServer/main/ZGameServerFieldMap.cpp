#include "stdafx.h"
#include "ZGameServerFieldMap.h"
#include "ZSharedField.h"
#include "ZDynamicFieldMaster.h"

void ZGameServerFieldMap::AddSharedField(ZSharedField* pSharedField)
{
	pSharedField->AddRef();

	ZBaseFieldManager::AddSharedField(pSharedField);
}

void ZGameServerFieldMap::DelSharedField(MUID uidSharedField)
{
	ZBaseFieldManager::DelSharedField(uidSharedField); ///< �̰����� Drop() ȣ��
}

void ZGameServerFieldMap::AddDynamicFieldMaster(ZDynamicFieldMaster* pDynamicFieldMaster)
{
	pDynamicFieldMaster->AddRef();

	ZBaseFieldManager::AddDynamicFieldMaster(pDynamicFieldMaster);
}

void ZGameServerFieldMap::DelDynamicFieldMaster(MUID uidDynamicFieldMaster)
{
	ZBaseFieldManager::DelDynamicFieldMaster(uidDynamicFieldMaster); ///< �̰����� Drop() ȣ��
}

ZGameServerFieldMap::~ZGameServerFieldMap()
{
	
}
