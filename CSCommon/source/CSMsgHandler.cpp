#include "stdafx.h"
#include "CSMsgHandler.h"

CSMsgHandler::CSMsgHandler()
{
	// do nothing
}

CSMsgHandler::~CSMsgHandler()
{
	// do nothing
}

bool CSMsgHandler::Request(const minet::MCommand* pCmd)
{
	if (!GetCommandParameter(pCmd))	return false;

	if (OnPreRequest(pCmd))
	{
		return OnRequest(pCmd);
	}

	return false;
}

bool CSMsgHandler::Response(const minet::MCommand* pCmd)
{
	if (!GetCommandParameter(pCmd))	return false;

	if (OnPreResponse(pCmd))
	{
		return OnResponse(pCmd);
	}

	return false;
}

bool CSMsgHandler::GetCommandParameter(const minet::MCommand* pCmd)
{
	if (pCmd == NULL)	return false;
	
	// Ŀ�ǵ� �б�
	int nType;
	tstring strMsg;
	int nAdditional;
	
	if (!pCmd->GetParameter(&nType, 0, minet::MPT_INT))			return false;
	if (!pCmd->GetParameter(strMsg, 1, minet::MPT_WSTR))		return false;
	if (!pCmd->GetParameter(&nAdditional, 4, minet::MPT_INT))	return false;

	// ��� �޽���Ÿ�� Ȯ��
	if (nType != GetHandlerType())						return false;	

	// �� ����
	m_uidCommandSender = pCmd->GetSenderUID();
	m_eType = (CSMsgType)nType;
	m_strMsg = strMsg;
	m_nAdditional = nAdditional;

	return true;
}

bool CSMsgHandler::OnPreRequest(const minet::MCommand* pCmd)
{
	// ��
	return true;
}

bool CSMsgHandler::OnPreResponse(const minet::MCommand* pCmd)
{
	// ��
	return true;
}
