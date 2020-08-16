#include "StdAfx.h"
#include "GPlayerSerializer.h"
#include "GPlayerObjectManager.h"
#include "GGlobal.h"
#include "GPlayerObject.h"
#include "GPlayerTalent.h"
#include "GModuleCombat.h"
#include "GModuleBuff.h"
#include "GDBCacheSystem.h"
#include "GPlayerDBCache.h"
#include "GDBCacheFlush.h"
#include "GFieldMgr.h"

GPlayerSerializer::GPlayerSerializer(void)
{
}

GPlayerSerializer::~GPlayerSerializer(void)
{
}

void GPlayerSerializer::SaveFromManager()
{
	// �÷��̾�� �������� �ʴ´�. �߿��� �������� ����صд�.
	GPlayerObjectManager::GPlayerObjectMap mapPlayer = gmgr.pPlayerObjectManager->GetObjects();

	for each (GPlayerObjectManager::GPlayerObjectMap::value_type pair in mapPlayer)
	{		
		Info info;
		MUID uidPlayer = pair.first;
		GEntityPlayer* pPlayer = pair.second->GetEntity();

		//// ��� �ŷ�Ʈ ���� ���
		//GPlayerTalent& pPlayerTalent = pPlayer->GetTalent();
		//info.setLearnedTalents = pPlayer->GetTalent().GetContainer();
		//pPlayerTalent.DeleteAll();

		//// ���� ���� ���� ����
		//pPlayer->GetModuleCombat()->Clear();
		//pPlayer->ReleaseMotionFactor();

		//// �÷��̾� ��� ���� ���
		//pPlayer->GetModuleBuff()->LazyLostAll();
		//pPlayer->GetModuleBuff()->Update(1.0f);		// ���� ������Ʈ �ּ� ƽ 0.1 ms �̻� ������Ʈ�� ���ƾ� ���ŵȴ�.

		//m_mapInfos.insert(make_pair(uidPlayer,info));
//		GPlayerDBCache* pCache = gsys.pDBCacheSystem->FindPlayerCache(uidPlayer);
//		GDBCacheFlush cf(pCache);
//		cf.Post();
//		gsys.pDBManager->Update(0.0f);

		info.cid = pPlayer->GetCID();
		info.uidField = pPlayer->GetFieldUID();
		info.vPos = pPlayer->GetPos();
		info.vDir = pPlayer->GetDir();
		m_mapInfos.insert(make_pair(uidPlayer,info));

		//SAFE_DELETE(pPlayer);
		//m_vecPlayers.push_back(uidPlayer);

		if (!pPlayer)
			continue;

		if (!pPlayer->GetField())
			continue;

		//pPlayer->DeleteMe();
		//pPlayer->GetField()->Update(0.0f);
		//pPlayer->GetField()->RemoveEntity(pPlayer);
		pPlayer->LeaveFromField();
		pair.second->Destroy();
	}
}

void GPlayerSerializer::RestoreAll()
{
	// �÷��̾� �ŷ�Ʈ ���� ����
	GPlayerObjectManager::GPlayerObjectMap mapPlayer = gmgr.pPlayerObjectManager->GetObjects();
	for each (GPlayerObjectManager::GPlayerObjectMap::value_type pair in mapPlayer)
	{
		MUID uidPlayer = pair.first;
		//GEntityPlayer* pPlayer = pair.second->GetEntity();
		
		map<MUID,Info>::iterator it = m_mapInfos.find(uidPlayer);
		if (it == m_mapInfos.end())
		{
			mlog("%s - failed (not found info), program exiting...\n", __FUNCTION__);
			exit(-1);
		}
		Info& info = it->second;
		GPlayerObject* po = pair.second;
		po->m_pMyEntity = new GEntityPlayer();
		po->m_pMyEntity->Create(uidPlayer);
		po->SerializeSelectCharacter(info.cid, true);
		po->m_pMyEntity->RegUIID(po->GetUIID());

		GField* pField = gmgr.pFieldMgr->GetField(info.uidField);
		if (!pField)
		{
			mlog("%s - failed (not found field), program exiting...\n", __FUNCTION__);
			exit(-1);
		}

		//pField->AddEntity(po->m_pMyEntity);
		po->m_pMyEntity->EnterToField(pField, info.vPos, info.vDir);

		//// �ŷ�Ʈ ����
		//for each (int nTalentID in info.setLearnedTalents)
		//{
		//	GPlayerTalent& pPlayerTalent = pPlayer->GetTalent();
		//	pPlayerTalent.Insert(nTalentID, false);		// TP�Ҹ����� ����
		//}

		
	}
}
