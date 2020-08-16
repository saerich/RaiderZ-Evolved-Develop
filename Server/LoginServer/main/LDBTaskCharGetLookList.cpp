#include "stdafx.h"
#include "LDBTaskCharGetLookList.h"
#include "LDBTaskGlobal.h"
#include "LPlayerObject.h"
#include "LPlayerObjectManager.h"
#include "SCmdRouter_Login.h"
#include "LCommandCenter.h"

LDBTaskCharGetLookList::LDBTaskCharGetLookList() 
: LDBAsyncTask(SDBT_DBTYPE_GAMEDB, SDBTID_CHARGETLOOKLIST, 2)
{
	m_commonBody.SetCharCount(0);
}

LDBTaskCharGetLookList::~LDBTaskCharGetLookList()
{

}

void LDBTaskCharGetLookList::OnExecute(mdb::MDatabase& rfDB)
{
	if (!GetSimpleInfo(rfDB))
		return;

	if (!GetEquipInfo(rfDB))
		return;

	SetTaskSuccess();
}

bool LDBTaskCharGetLookList::GetSimpleInfo( mdb::MDatabase& rfDB )
{
	mdb::MDBRecordSet rs(&rfDB);

	if (!ExecuteW(rs, GetSQLW(CHAR_SIMPLE_INFO)))
		return false;

	m_commonBody.GetCharSimpleInfo(rs);

	return true;
}

bool LDBTaskCharGetLookList::GetEquipInfo( mdb::MDatabase& rfDB )
{
	mdb::MDBRecordSet rs(&rfDB);

	if (!ExecuteW(rs, GetSQLW(CHAR_EQUIPMENT_INFO)))
		return false;

	return m_commonBody.GetCharEquipmentInfo(rs);
}

void LDBTaskCharGetLookList::Input(const MUID& uidPlayer)
{
	m_commonBody.SetPlayerUID(uidPlayer);	
}

mdb::MDB_THRTASK_RESULT LDBTaskCharGetLookList::_OnCompleted()
{
	Completer completer(m_commonBody);
	completer.Do();

	return mdb::MDBTR_SUCESS;
}

LDBTaskCharGetLookList::Completer::Completer(SCommonBodyCharGetLookList& commonBody)
: m_commonBody(commonBody)
{

}

void LDBTaskCharGetLookList::Completer::Do()
{
	// ��û�� Ȯ��
	MUID uidPlayer = m_commonBody.GetPlayerUID();
	LPlayerObject* pPlayerObject = gmgr.pPlayerObjectManager->GetPlayer(uidPlayer);
	if (NULL == pPlayerObject)
	{
		return;
	}


	// ĳ���� ��� ����
	vector<TD_AccountCharInfo> vecAccountCharInfo;	
	pPlayerObject->ClearCharFieldInfo();	

	for (int i = 0; i < m_commonBody.GetCharCount(); ++i)
	{
		// �� ����
		TD_AccountCharInfo tdAccountCharInfo;

		m_commonBody.SetAccountCharInfo(i, &tdAccountCharInfo);
		vecAccountCharInfo.push_back(tdAccountCharInfo);


		// �ʵ� ����
		SAccountCharFieldInfo accountCharFieldInfo;

		m_commonBody.SetAccountCharFieldInfo(i, &accountCharFieldInfo);
		pPlayerObject->AddCharFieldInfo(accountCharFieldInfo);
	}


	// ĳ���� ��� ����
	SCmdRouter_Login cmdRouter(gsys.pCommandCenter);
	cmdRouter.ResponseAccountCharList(uidPlayer, vecAccountCharInfo);
}
