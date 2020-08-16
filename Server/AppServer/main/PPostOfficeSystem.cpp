#include "stdafx.h"
#include "PPostOfficeSystem.h"
#include "PCmdRouter_Mail.h"
#include "SDef.h"
#include "SProxyPlayer.h"
#include "PGameServerObject.h"
#include "PGlobal.h"
#include "PGameServerObjectManager.h"
#include "PBounceMail.h"
#include "PSystem.h"
#include "SProxyPlayerManager.h"
#include "PServer.h"

PPostOfficeSystem::PPostOfficeSystem()
{
	m_pRouter = new PCmdRouter_Mail();
}

PPostOfficeSystem::~PPostOfficeSystem()
{
	SAFE_DELETE(m_pRouter);

	while(!m_queueBounceMail.empty())
	{
		SAFE_DELETE(m_queueBounceMail.front());
		m_queueBounceMail.pop();
	}
}

void PPostOfficeSystem::SetNewRouter(PCmdRouter_Mail* pRouter)
{
	VALID(pRouter != NULL);

	if (m_pRouter != NULL)
	{
		SAFE_DELETE(m_pRouter);
	}

	m_pRouter = pRouter;
}

void PPostOfficeSystem::RouteNewMailToReceiver(const SProxyPlayer* pPlayer, const TD_MAIL_MAILBOX_MAIL_INFO* pInfo)
{
	VALID(pPlayer != NULL);
	VALID(pInfo != NULL);

	
	// �÷��̾ �ִ� ���Ӽ��� ã��
	int nGameServerID = pPlayer->GetGameServerID();
	
	PGameServerObject* pGameServer = gmgr.pGameServerObjectManager->Find(nGameServerID);
	
	if (pGameServer == NULL)
	{
		// ���Ӽ����� ã�� �� ���ٸ�, �뺸���� �ʴ´�. 
		mlog("[Warning] Not find GameServer(ID=%d) for mail notify.", nGameServerID);
		return;
	}


	// �뺸�ϱ�
	MUID uidGameServer = pGameServer->GetUID();
	CID nCID = pPlayer->GetCID();	
	m_pRouter->TransNewMailReq(uidGameServer, nCID, pInfo);
}

void PPostOfficeSystem::RetryNewMailToReceiver(const SProxyPlayer* pPlayer, const TD_MAIL_MAILBOX_MAIL_INFO* pInfo)
{
	VALID(pPlayer != NULL);
	VALID(pInfo != NULL);


	// �ݼ� ���� �غ�
	uint32 nNowTime = gsys.pSystem->GetNowTime();
	CID nCID = pPlayer->GetCID();
	PBounceMail* pMail = new PBounceMail(nNowTime, nCID, pInfo);

	m_queueBounceMail.push(pMail);
}

void PPostOfficeSystem::Update(void)
{
	// �ݼ� ���� �ð� �˻�
	while(!m_queueBounceMail.empty())
	{		
		PBounceMail* pMail = m_queueBounceMail.front();

		// ó�� �ð� �ȉ����� ó������ �ʴ´�.
		uint32 nNowTime = gsys.pSystem->GetNowTime();		

		if (!pMail->IsReady(nNowTime))
		{
			return;
		}


		// ���� ����� ������� �������� �����, pMail�� �����Ѵ�.
		auto_ptr<PBounceMail> mail(pMail);
		m_queueBounceMail.pop();


		// �뺸 �÷��̾ Ȯ���Ѵ�.
		CID nCID = pMail->GetCID();		
		SProxyPlayerManager* pPlayerManager = gsys.pServer->GetPlayerManager();
		SProxyPlayer* pPlayer = pPlayerManager->FindPlayer(nCID);

		if (pPlayer == NULL)
		{
			// �÷��̾�� ������ ������.
			return;
		}


		// ���Ӽ����� Ȯ���Ѵ�.
		int nGameServerID = pPlayer->GetGameServerID();
		PGameServerObject* pGameServer = gmgr.pGameServerObjectManager->Find(nGameServerID);

		if (pGameServer == NULL)
		{
			// ���Ӽ����� �߸��Ǿ���.
			return;
		}		


		// �� �뺸
		MUID uidGameServer = pGameServer->GetUID();
		TD_MAIL_MAILBOX_MAIL_INFO info;		
		pMail->Export(&info);

		m_pRouter->TransNewMailReq(uidGameServer, nCID, &info);
	}
}
