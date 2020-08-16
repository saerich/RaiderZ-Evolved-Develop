#include "stdafx.h"
#include "GMsgHandler.h"
#include "CSStringFilter.h"
#include "GGlobal.h"
#include "GMsgCommandFacade.h"
#include "CCommandResultTable.h"


GMsgHandler::GMsgHandler()
{
	// do nothing
}

GMsgHandler::~GMsgHandler()
{
	// do nothing
}

bool GMsgHandler::OnPreRequest(const minet::MCommand* pCmd)
{
	// ���� ���� - �߶󳽴�.
	if (CHATTING_LIMIT_SIZE < m_strMsg.length())
	{
		m_strMsg = m_strMsg.substr(0, CHATTING_LIMIT_SIZE);
	}

	// �弳���� ó���� Ŭ���̾�Ʈ���� �մϴ�. aibeast

	// �ý��� ���� ��ü
	m_strMsg = CSStringFilter::RemoveGameSystemCharacter(m_strMsg);

	return true;
}
