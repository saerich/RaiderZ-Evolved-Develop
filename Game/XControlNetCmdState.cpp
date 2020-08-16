#include "stdafx.h"
#include "XControlNetCmdState.h"

XControlNetCmdState::XControlNetCmdState()
{
	m_vecCmdID.clear();
}

bool XControlNetCmdState::CheckStateUsableCommand( MCommand* pCommand )
{
	if(pCommand == NULL)
		return false;

	// ��ϵȰ��� ���ٸ�... ��� ���
	if(m_vecCmdID.empty())
		return true;

	int nCmdID = pCommand->GetID();

	if(CheckCompareCmdID(nCmdID))
	{
		// ��� �� �� �ִ�.
		return true;
	}

	return false;
}

void XControlNetCmdState::SetUsableCmdID( int nID )
{
	if(CheckCompareCmdID(nID))
		return;

	m_vecCmdID.push_back(nID);
}

bool XControlNetCmdState::CheckCompareCmdID( int nID )
{
	for(vector<int>::iterator it = m_vecCmdID.begin(); it != m_vecCmdID.end(); ++it)
	{
		if((*it) == nID)
		{
			return true;
		}
	}

	return false;
}
