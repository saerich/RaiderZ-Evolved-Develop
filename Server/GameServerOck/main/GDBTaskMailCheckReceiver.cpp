#include "stdafx.h"
#include "GDBTaskMailCheckReceiver.h"
#include "GGlobal.h"
#include "GPlayerObjectManager.h"
#include "GMailSystem.h"
#include "GDBManager.h"
#include "GConst.h"
#include "GMsgCommandFacade.h"

GDBTaskMailCheckReceiver::GDBTaskMailCheckReceiver(const MUID& uidReqPlayer)
: GDBAsyncTask(uidReqPlayer, SDBT_DBTYPE_GAMEDB, SDBTID_MAIL_CHECK_RCVR)
{
	m_Data.m_uidPlayer	= uidReqPlayer;
}

GDBTaskMailCheckReceiver::~GDBTaskMailCheckReceiver()
{
}

void GDBTaskMailCheckReceiver::Input(const GDBT_MAIL_WRITE& data)
{
	m_Data.m_mailWrite = data;
}

mdb::MDB_THRTASK_RESULT GDBTaskMailCheckReceiver::RouteResult(CCommandResultTable nResult)
{
	GEntityPlayer* pPlayer = gmgr.pPlayerObjectManager->GetEntity(m_Data.m_uidPlayer);

	if (pPlayer != NULL)
	{
		gsys.pMailSystem->RouteMailPostRes(pPlayer, nResult);
	}

	return mdb::MDBTR_SUCESS;
}

void GDBTaskMailCheckReceiver::OnExecute(mdb::MDatabase& rfDB)
{
	mdb::MDBRecordSet rs(&rfDB);
	if (!ExecuteW(rs, GetSQLW(SQL_MAIL_CHECK_RCVR)))
		return;

	m_Result.m_nErrorNo = rs.Field("ERRNO").AsInt();	
}

mdb::MDB_THRTASK_RESULT GDBTaskMailCheckReceiver::_OnCompleted()
{
	switch(m_Result.m_nErrorNo)
	{
	case 0:	return MailWrite();
	case 1:	return RouteResult(CR_FAIL_MAIL_POST_NOT_FIND_USER);	// ���� �̸�
	case 2:
		if (GConst::TEST_MAIL_POST_TO_ONESELF)
		{
			return MailWrite();			
		}
		else
		{
			return RouteResult(CR_FAIL_MAIL_POST_SELF);				// ���� ĳ����
		}
		break;
	case 3:	return RouteResult(CR_FAIL_MAIL_POST_NOT_SEND_GM);		// �Ϲ� ������ GM���� ����
	case 4:	return RouteResult(CR_FAIL_MAIL_POST_FULL_BOX);			// ���� ��������
	default:
		return RouteResult(CR_FAIL);
	}

	return mdb::MDBTR_SUCESS;
}

mdb::MDB_THRTASK_RESULT	GDBTaskMailCheckReceiver::_OnFailed()
{
	// Ŭ���̾�Ʈ�� ���� �뺸
	return RouteResult(CR_FAIL_MAIL_POST_DB_CHECK_FAIL);
}

mdb::MDB_THRTASK_RESULT GDBTaskMailCheckReceiver::MailWrite(void)
{
	if (!gsys.pDBManager->MailWrite(m_Data.m_uidPlayer, m_Data.m_mailWrite))
	{
		return RouteResult(CR_FAIL_MAIL_POST_DB_ERROR);
	}	

	return mdb::MDBTR_SUCESS;
}
