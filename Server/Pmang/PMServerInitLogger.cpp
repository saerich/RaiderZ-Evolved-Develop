#include "StdAfx.h"
#include "PMServerInitLogger.h"

SERVER_INIT_RESULT PmServerInitLogger::m_nResult = SERVERINIT_OK;

SERVER_INIT_RESULT PmServerInitLogger::GetResult()
{
	return m_nResult;
}

void PmServerInitLogger::SetResult( SERVER_INIT_RESULT result )
{
	m_nResult = result;
}

