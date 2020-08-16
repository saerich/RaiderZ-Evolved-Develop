#include "stdafx.h"
#include "PmLoginFacade.h"
#include "PmLoginTimeChecker.h"
#include "PmAuthenticator.h"
#include "PmLoginStreamParser.h"
#include "PmDef.h"

PmLoginFacade::PmLoginFacade(wstring strServerKey, uint32 nToleranceSeconds)
{
	m_pTimeChecker = new PmLoginTimeChecker(nToleranceSeconds);
	m_pAuthenticator = new PmAuthenticator(strServerKey);
}

PmLoginFacade::~PmLoginFacade()
{
	delete m_pTimeChecker;
	delete m_pAuthenticator;
}

bool PmLoginFacade::Login(wstring strLoginStream, PmUserData* poutUserData)
{
	// �α��� ���ڿ� ��ȿ�� �˻�
	PmLoginStreamParser parser(strLoginStream);

	if (!parser.Parse())
	{
		return false;
	}


	// �α��� ��û �ð� �˻�
	time_t timeLogin = parser.GetTimeStamp();

	time_t timeCurrent;
	time(&timeCurrent);

	if (!m_pTimeChecker->Verify(timeLogin, timeCurrent))
	{
		return false;
	}


	// �α��� �ؽ��ڵ� �˻�
	wstring strHashKeyStream = parser.GetHashKeyStream();
	wstring strHashedValue = parser.GetHashedValue();

	if (!m_pAuthenticator->Verify(strHashedValue, strHashKeyStream))
	{
		return false;
	}	


	// ����� ���� ����
	if (poutUserData != NULL)
	{
		*poutUserData = parser.GetUserData();
	}


	return true;
}
