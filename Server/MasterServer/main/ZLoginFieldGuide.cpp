#include "StdAfx.h"
#include "ZLoginFieldGuide.h"
#include "ZGameServerObjectManager.h"
#include "ZDynamicFieldMaster.h"
#include "ZDynamicField.h"
#include "ZFieldManager.h"
#include "ZFieldInfoManager.h"
#include "ZFieldFinder.h"
#include "ZSharedField.h"
#include "ZSharedFieldMaster.h"


ZLoginFieldGuide::ZLoginFieldGuide(CID nCID, PLAYER_FIELD_INFO& refPlayerFieldInfo)
: m_nCID(nCID)
, m_PlayerFieldInfo(refPlayerFieldInfo)
{
}

ZLoginFieldGuide::~ZLoginFieldGuide()
{
}

bool ZLoginFieldGuide::Search(ZGameServerObject*& poutDestServer, ZField*& poutDestField)
{
	/// ���Ӽ����� �ϳ��� �ִ��� üũ
	if (!CheckExistGameServer())
		return false;

	/// 1. ó�� �α����̸� �����ϴ� �ʵ带 ã�´�.
	if (SearchFirstLoginField(poutDestServer, poutDestField))
		return true;

	/// 2. �����ʵ带 ã�´�.
	if (SearchDynamicField(poutDestServer, poutDestField))
		return true;

	/// 3. �����ʵ带 ã�´�.
	if (SearchSharedField(poutDestServer, poutDestField))
		return true;

	/// 4. ���ܻ�Ȳ �߻�, ���� ��ų �ʵ带 ã�´�.	
	if (SearchExceptionField(poutDestServer, poutDestField))
		return true;

	return false;
}

bool ZLoginFieldGuide::CheckExistGameServer()
{
	if (0 == gmgr.pGameServerObjectManager->GetClientsCount())
	{
		return false;
	}
	return true;
}

bool ZLoginFieldGuide::SearchFirstLoginField(ZGameServerObject*& poutDestServer, ZField*& poutDestField)
{
	/// SharedFieldID �� 0���� ũ�� ��ȿ�� �����ʵ� �������� ó���ž� �մϴ�.
	if (m_PlayerFieldInfo.nSharedFieldID > 0)
		return false;

	ZSharedFieldMaster* pFieldMaster = gmgr.pFieldManager->FindSharedFieldMaster(FIRST_ENTER_FIELD_ID);
	if (NULL == pFieldMaster)
	{
		mlog3("Error! ZLoginFieldGuide::SearchFirstLoginField(), FIRST_ENTER_FIELD_ID is Not Exist! (CID=%d)\n", m_nCID);
		return false;
	}

	poutDestField = pFieldMaster->GetEnterableChannel();
	if (NULL == poutDestField)
		return true;	///< ��õ� ����

	poutDestServer = gmgr.pGameServerObjectManager->FindGameServer(poutDestField->GetGameServerID());
	if (NULL == poutDestServer)
		return false;

	return true;
}

bool ZLoginFieldGuide::SearchDynamicField(ZGameServerObject*& poutDestServer, ZField*& poutDestField)
{
	if (m_PlayerFieldInfo.uidDynamicFieldGroup.IsInvalid())
		return false;

	ZDynamicFieldMaster* pFieldMaster = gmgr.pFieldManager->FindDynamicFieldMaster(m_PlayerFieldInfo.uidDynamicFieldGroup);
	if (NULL == pFieldMaster)
		return false;

	poutDestField = pFieldMaster->FindField(m_PlayerFieldInfo.nDynamicFieldID);
	if (NULL == poutDestField)
		return false;
	
	poutDestServer = gmgr.pGameServerObjectManager->FindGameServer(poutDestField->GetGameServerID());
	if (NULL == poutDestServer)
	{
		mlog3("Error! ZLoginFieldGuide::SearchDynamicField(), DestGameServer is NULL! (GameServerID=%d, CID=%d, FieldGroupUID=%I64u)\n"
			, poutDestField->GetGameServerID(), m_nCID, m_PlayerFieldInfo.uidDynamicFieldGroup.Value);
		return false;
	}

	return true;
}

bool ZLoginFieldGuide::SearchSharedField(ZGameServerObject*& poutDestServer, ZField*& poutDestField)
{
	/// �ʵ� ���� �˻�
	ZFieldInfo* pFieldInfo = gmgr.pFieldInfoManager->Find(m_PlayerFieldInfo.nSharedFieldID);
	if (NULL == pFieldInfo)
	{
		mlog3("Error! ZLoginFieldGuide::SearchSharedField(), FieldInfo is NULL! (CID=%d, FieldID=%d)\n"
			, m_nCID, m_PlayerFieldInfo.nSharedFieldID);
		return false;
	}

	/// ���� ������ ���� �ʵ� ã�´�.
	ZSharedField* poutSharedField = NULL;

	ZFieldFinder fieldFinder;
	if (false == fieldFinder.FindEnterableSharedField(poutSharedField, m_PlayerFieldInfo.nSharedFieldID))
	{
		mlog3("Error! ZLoginFieldGuide::SearchSharedField(), Failed Find Enterable SharedField(id=%d)\n", m_PlayerFieldInfo.nSharedFieldID);
		return false;
	}

	if (NULL == poutSharedField)
		return true;	///< ��õ� ����

	poutDestField = poutSharedField;

	poutDestServer = gmgr.pGameServerObjectManager->FindGameServer(poutDestField->GetGameServerID());
	if (NULL == poutDestServer)
	{
		mlog3("Error! ZLoginFieldGuide::SearchSharedField(), DestGameServer is NULL! (CID=%d, FieldID=%d, ChannelID=%d)\n"
			, poutDestField->GetGameServerID(), m_nCID, poutDestField->GetID(), poutSharedField->GetChannelID());
		return false;
	}

	return true;
}

bool ZLoginFieldGuide::SearchExceptionField(ZGameServerObject*& poutDestServer, ZField*& poutDestField)
{
	// �ϴ� ���� �ʵ�� ���� ��Ų��.

	ZSharedFieldMaster* pFieldMaster = gmgr.pFieldManager->FindSharedFieldMaster(FIRST_ENTER_FIELD_ID);
	if (NULL == pFieldMaster)
		return false;

	poutDestField = pFieldMaster->GetEnterableChannel();
	if (NULL == poutDestField)
		return true;	///< ��õ� ����

	poutDestServer = gmgr.pGameServerObjectManager->FindGameServer(poutDestField->GetGameServerID());
	if (NULL == poutDestServer)
		return false;

	return true;
}
