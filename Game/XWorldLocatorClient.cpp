#include "stdafx.h"
#include "XWorldLocatorClient.h"
#include "MUDPCommandHandler.h"
#include "CCommandTable_WorldLocator.h"
#include "MiNetHelper.h"


const int CONNECT_LOCATOR_RETRY_TICK_TIME = 5.f;


XWorldLocatorClient::XWorldLocatorClient()
: m_isUpdate(false)
, m_nState(STOP)
{
	m_pCommandHandler->AddHandler(MWC_COMM_WORLD_INFO_RES, OnWorldInfoResponse);
	m_RetryRegulator.SetTickTime(CONNECT_LOCATOR_RETRY_TICK_TIME);
	m_RetryRegulator.Stop();
}

XWorldLocatorClient::~XWorldLocatorClient()
{
	// do nothing
}

bool XWorldLocatorClient::Start(const vector<wstring>& vecIP, int nPort)
{
	// ������ ������ ������ ����
	if (vecIP.empty())	return false;


	// ���� ������ ��û�� �������� �ּҸ� �޾Ƶд�.
	for each(const wstring& strIP in vecIP)
	{
		m_queueIP.push(strIP);
	}
	
	m_nPort = nPort;	


	// ���� ���ε�
	if (!Create(0, 0, 0))
	{
		return false;
	}

	m_nState = START;	


	return true;
}

void XWorldLocatorClient::Update(float fDelta)
{
	switch(m_nState)
	{
	case STOP:		
		break;
	case START:		
		Connect();
		m_RetryRegulator.Start();
		m_nState = RUN;
		break;
	case RUN:
		if (m_isUpdate)
		{
			MCriticalSection::Guard guard(m_csWorldInfoLock);
			gvar.Network.vecWorldInfo = m_vecWorldInfo;

			m_nState = FINISH;
		}
		else if (m_RetryRegulator.IsReady(fDelta))
		{
			if (!Connect())
			{
				// TODO: �����α׸� ���ܾ��Ѵ�. [2/28/2011 praptor]
				m_nState = STOP;
			}
		}
		break;
	case FINISH:
		// TODO: ���������� �޾ƿԴ�. [2/28/2011 praptor]
		if ( global.script)
			global.script->GetGlueGameEvent().OnLoginEvent( "WORLDLIST", "REFRESH");

		m_nState = STOP;
		break;
	}
}

bool XWorldLocatorClient::Connect(void)
{
	// IP����� ������ ����
	if (m_queueIP.empty())	return false;


	// �̹� ���� IP�� �޾ƿ���, ������� �õ��ϱ����ؼ� ����� ������ ������ ������.
	wstring strIP = m_queueIP.front();
	m_queueIP.pop();
	m_queueIP.push(strIP);


	// IP ��ȯ
	int nIP = MiNetHelper::ConvertIPAddress(MLocale::ConvUTF16ToAnsi(strIP).c_str());
	

	// ������ �����ּ� ����
	SetServerAddress(nIP, m_nPort);

	
	// ���� ��� ��û
	MCommand* pCmd = new MCommand();
	pCmd->SetID(MWC_COMM_WORLD_INFO_REQ);

	bool bPostResult = PostCommand(pCmd);
	
	return bPostResult;
}

IMPL_WL_CMD_HANDLER(XWorldLocatorClient, OnWorldInfoResponse)
{
	// UDPSocket �����忡�� ȣ�������, ������ ���� �̴ϴ�.
	XWorldLocatorClient* pThis = static_cast<XWorldLocatorClient*>(pHandler);
	if (pThis == NULL)	return false;

	MCriticalSection::Guard guard(pThis->m_csWorldInfoLock);


	// Ŀ��� �Ľ�
	vector<TD_WORLD_INFO> vecWorldInfo;
	if (!pCommand->GetBlob(vecWorldInfo, 0))	return false;


	// ���� ���� ����
	pThis->m_vecWorldInfo.clear();

	for each(const TD_WORLD_INFO& tdInfo in vecWorldInfo)
	{
		CSWorldInfo info;

		info.nID = tdInfo.nID;
		info.strName = tdInfo.strName;
		info.strIP = tdInfo.strIP;
		info.nType = tdInfo.nType;
		info.nCurrentPlayerCount = tdInfo.nCurrentPlayerCount;
		info.nMaxPlayerCount = tdInfo.nMaxPlayerCount;

		pThis->m_vecWorldInfo.push_back(info);
	}

	pThis->m_isUpdate = true;

	return true;
}
