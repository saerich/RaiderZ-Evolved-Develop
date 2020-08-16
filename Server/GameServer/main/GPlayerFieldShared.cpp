#include "stdafx.h"
#include "GPlayerFieldMasterServer.h"
#include "GPlayerFieldShared.h"
#include "GPlayerFieldDynamic.h"
#include "GGlobal.h"
#include "GSystem.h"
#include "GEntityPlayer.h"
#include "GPlayerField.h"
#include "GEntityPlayer.h"
#include "GFieldInfoMgr.h"
#include "GFieldMgr.h"
#include "GSharedField.h"
#include "GServerGateRouter.h"
#include "GMasterServerFacade.h"
#include "GConfig.h"
#include "CCommandTable.h"
#include "GCommand.h"
#include "GCommandCenter.h"
#include "SCmdRouter_Login.h"
#include "GPlayerFieldGateRetryer.h"
#include "GClientFieldRouter.h"
#include "CCommandResultTable.h"


GPlayerFieldShared::GPlayerFieldShared(GEntityPlayer* pOwner, GPlayerField* pPlayerField)
: m_nPrepareChannelTime(0)
, m_nPrepareChannelID(INVALID_CHANNELID)
, m_pOwner(pOwner)
, m_pPlayerField(pPlayerField)
{

}

void GPlayerFieldShared::PrepareChangeChannel( CHANNELID nChannelID )
{
	// ���� ���� �ð� ���
	m_nPrepareChannelTime = gsys.pSystem->GetNowTime();
	m_nPrepareChannelID = nChannelID;
	
	// ���� �޼��� ����
	GClientFieldRouter router;
	router.PrepareChangeChannel(m_pOwner, CR_SUCCESS);
}
bool GPlayerFieldShared::IsPreparedChangeChannel()
{
	if (0 != m_nPrepareChannelTime || INVALID_CHANNELID != m_nPrepareChannelID)
		return true;

	return false;
}
void GPlayerFieldShared::ClearPrepareChannelChange()
{
	m_nPrepareChannelTime = 0;
	m_nPrepareChannelID = INVALID_CHANNELID;
}

bool GPlayerFieldShared::GateRequest(int nReqFieldID, int nReqChannelID, vec3 vecDestPos, vec3 vecDestDir)
{
	/// Gate Retryer �� �۵� ���̶�� ó������ �ʴ´�.
	if (m_pPlayerField->GetFieldGateRetryer().IsRunRetryer())
		return false;

	/// �˰� �ִ� �ʵ����� �˻�.
	if (NULL == gmgr.pFieldInfoMgr->Find(nReqFieldID))
	{
		mlog("Error! GPlayerFieldShared::GateRequest(), Not Exist FieldInfo. (AID: %I64d , CID: %d , UID(%u:%u) , id: %d)\n"
			, m_pOwner->GetAID(), m_pOwner->GetCID(), m_pOwner->GetUID().High, m_pOwner->GetUID().Low, nReqFieldID);
		return m_pOwner->FailAndRouteSystemMsg(CR_FAIL_SYSTEM_INVALID_FIELD_ID);
	}

	/// Ŭ���̾�Ʈ�� Gate �غ� Ŀ�ǵ� ����
	SCmdRouter_Login router(gsys.pCommandCenter);
	router.PreparingToLoad(m_pOwner->GetUID(), nReqFieldID);

	if (GConfig::m_bStandAlone)
	{
		// ���� ������ �ʵ尡 ������ ����.
		GSharedField* pSharedField = GetEnterableSharedFIeld(nReqFieldID, nReqChannelID);
		if (NULL == pSharedField) return false;
		
		if (!GateExecute(pSharedField, vecDestPos, vecDestDir))
			return false;
	}
	else
	{
		// ������ ������ ���´�
		if (!m_pPlayerField->GetFieldMasterServer().AskGateSharedField(nReqFieldID, vecDestPos, vecDestDir, nReqChannelID))
			return false;
	}

	return true;
}


GSharedField* GPlayerFieldShared::GetEnterableSharedFIeld(int nFieldID, int nChannelID)
{
	GSharedField* pSharedField = NULL;
	
	// �ڱ� ������ ä�η� ã��, ������ ä�� ������� �ʴ´�; �����̵� �ּ�ȭ�� ����
	pSharedField = gmgr.pFieldMgr->GetSharedField(nFieldID, nChannelID);
	if (NULL == pSharedField)
	{
		pSharedField = gmgr.pFieldMgr->GetEnterableSharedField(nFieldID);
	}

	return pSharedField;
}

bool GPlayerFieldShared::GateExecute(GSharedField* pSharedField, vec3 vecDestPos, vec3 vecDestDir)
{
	RVALID_RET(pSharedField, false);

	m_pPlayerField->GetFieldDynamic().ClearEnterPos();
	if (m_pLisener)
	{
		m_pLisener->OnGateExecute(pSharedField, vecDestPos, vecDestDir);
	}

	return true;
}
