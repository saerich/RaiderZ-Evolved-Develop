#include "stdafx.h"
#include "ZSmartFieldAllocator.h"
#include "ZFieldManager.h"
#include "ZFieldInfoManager.h"
#include "STransData_M2G.h"
#include "ZGameServerObjectManager.h"
#include "ZGameServerObject.h"
#include "ZDynamicFieldMaster.h"
#include "ZConst.h"
#include "ZFitnessCalculator.h"
#include "ZSharedFieldMaster.h"
#include "ZSharedField.h"
#include "CSDef.h"
#include "ZFieldFinder.h"


ZSharedField* ZSmartFieldAllocator::AllocNewChannel(ZSharedFieldMaster* pFieldMaster)
{
	if (NULL == pFieldMaster) return NULL;
	
	// �ʵ� Ȯ��
	int nFieldID = pFieldMaster->GetFieldID();

	ZFieldInfo* pFieldInfo = gmgr.pFieldInfoManager->Find(nFieldID);
	if (NULL == pFieldInfo)	return NULL;
	
	int nChannelID = pFieldMaster->GetAllocableChannelID();
	
	// ä���� ����� ���Ӽ��� Ž��.
	ZFieldFinder finder;
	ZGameServerObject* pTargetGameServer = NULL;
	pTargetGameServer = finder.FindGameServer_HadReservedChannel(nFieldID, nChannelID);
	if (NULL == pTargetGameServer)
	{
		// ����� ä���� �ƴϸ� ���� ����ؼ� ������ ���� ����.
		pTargetGameServer = GetRecommendedGameServer_ToCreateSharedField(pFieldMaster);
		if (NULL == pTargetGameServer) return NULL;
	}
	
	bool bIsDefault = false;
	ZSharedField* pNewField = gmgr.pFieldManager->CreateSharedField(nFieldID, nChannelID, pTargetGameServer->GetID(), bIsDefault, true);
	if (NULL == pNewField) return NULL;

	// GameServerObject�� FieldMap���� �߰��Ѵ�.
	pTargetGameServer->GetFieldMap().AddSharedField(pNewField);

	return pNewField;
}

ZGameServerObject* ZSmartFieldAllocator::AllocNewDynamicFieldGroup(MUID uidRequestPlayer, int nFieldGroupID, DYNAMIC_FIELD_TYPE eType, MUID* outuidCreatedFieldGroup)
{
	if (NULL == gmgr.pFieldInfoManager->FindGroupInfo(nFieldGroupID))
		return NULL;

	ZGameServerObject* pRecommendedGameServer = NULL;
	
	if (ZConst::ALLOC_DYNAMIC_SERVER_ANOTHER_SERVER)
	{
		ZPlayer* pRequestPlayer = gmgr.pPlayerManager->FindByUID(uidRequestPlayer);
		if (NULL == pRequestPlayer) return NULL;

		pRecommendedGameServer = gmgr.pGameServerObjectManager->FindAnotherGameServer(pRequestPlayer->GetGameServerID());
		if (NULL == pRecommendedGameServer) return NULL;
	}
	else
	{
		pRecommendedGameServer = GetRecommendedGameServer_ToCreateFieldGroup(uidRequestPlayer);
		if (NULL == pRecommendedGameServer) return NULL;
	}

	int nOwnerServerID = pRecommendedGameServer->GetID();
	ZDynamicFieldMaster* pNewFieldGroup = m_pFieldManager->CreateDynamicFieldMaster(nFieldGroupID, nOwnerServerID, eType, uidRequestPlayer, true);
	if (NULL == pNewFieldGroup) return NULL;

	pRecommendedGameServer->GetFieldMap().AddDynamicFieldMaster(pNewFieldGroup);

	*outuidCreatedFieldGroup = pNewFieldGroup->GetUID();

	return pRecommendedGameServer;
}

ZGameServerObject* ZSmartFieldAllocator::GetRecommendedGameServer_ToCreateSharedField(ZSharedFieldMaster* pFieldMaster)
{
	if (NULL == pFieldMaster) return NULL;

	ZGameServerObject* pRequestGameServer = NULL;
	ZGameServerObject* pRecommendedGameServer = NULL;
	int nRecommandedGameServerValue = 0;
	int nItorGameServerValue = 0;

	ZGameServerObjectManager::ZServerObjectIDMap::iterator itor = gmgr.pGameServerObjectManager->GetGameServerObjectIDMap().begin();
	for (; itor != gmgr.pGameServerObjectManager->GetGameServerObjectIDMap().end(); ++itor)
	{
		ZGameServerObject* pItorGameServer = (*itor).second;

		nItorGameServerValue = ZFitnessCalculator::CalcToCreateChannel(pItorGameServer);

		if (nRecommandedGameServerValue < nItorGameServerValue)
		{
			nRecommandedGameServerValue = nItorGameServerValue;
			pRecommendedGameServer = pItorGameServer;
		}
	}

	if (nRecommandedGameServerValue == 0)
		return NULL;

	return pRecommendedGameServer;
}

ZGameServerObject* ZSmartFieldAllocator::GetRecommendedGameServer_ToCreateFieldGroup(MUID uidRequestPlayer)
{
	ZPlayer* pRequestPlayer = gmgr.pPlayerManager->FindByUID(uidRequestPlayer);
	if (NULL == pRequestPlayer) return NULL;

	ZGameServerObject* pRequestGameServer = gmgr.pGameServerObjectManager->FindGameServer(pRequestPlayer->GetGameServerID());
	if (NULL == pRequestGameServer) return NULL;
	
	ZGameServerObject* pRecommendedGameServer = pRequestGameServer;
	int nRecommandedGameServerValue = ZFitnessCalculator::CalcToCreateFieldGroup(pRequestGameServer, ZConst::FITNESS_ADDITIONAL_VALUE_TO_ALLOC_FIELD);
	int nItorGameServerValue;

	ZGameServerObjectManager::ZServerObjectIDMap::iterator itor = gmgr.pGameServerObjectManager->GetGameServerObjectIDMap().begin();
	for (; itor != gmgr.pGameServerObjectManager->GetGameServerObjectIDMap().end(); ++itor)
	{
		ZGameServerObject* pItorGameServer = (*itor).second;

		if (pRecommendedGameServer->GetUID() == pItorGameServer->GetUID())
			continue;

		nItorGameServerValue = ZFitnessCalculator::CalcToCreateFieldGroup(pItorGameServer);

		if (nRecommandedGameServerValue < nItorGameServerValue)
		{
			nRecommandedGameServerValue = nItorGameServerValue;
			pRecommendedGameServer = pItorGameServer;
		}
	}

	if (nRecommandedGameServerValue == 0)
		return NULL;

	return pRecommendedGameServer;
}