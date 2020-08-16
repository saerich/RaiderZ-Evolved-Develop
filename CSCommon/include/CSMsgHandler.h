#pragma once

#include "CSDef.h"
#include "CSMsgCommandHelper.h"

class CSMsgHandler
{
public:
	CSMsgHandler();
	virtual ~CSMsgHandler();

	bool Request(const minet::MCommand* pCmd);
	bool Response(const minet::MCommand* pCmd);
	bool GetCommandParameter(const minet::MCommand* pCmd);

	virtual bool OnPreRequest(const minet::MCommand* pCmd);		// �弳���͸� �� �޽��� ��ó����
	virtual bool OnPreResponse(const minet::MCommand* pCmd);	// ������ ó��

	virtual bool OnRequest(const minet::MCommand* pCmd) = 0;
	virtual bool OnResponse(const minet::MCommand* pCmd) = 0;	
	virtual CSMsgType GetHandlerType(void) = 0;

protected:
	CSMsgCommandHelper m_msgHelper;
	
	MUID m_uidCommandSender;
	CSMsgType m_eType;
	tstring m_strMsg;
	int m_nAdditional;	
};
