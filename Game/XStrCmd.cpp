#include "stdafx.h"
#include "XStrCmd.h"

int IsSpace(int c)
{
	if (c < 0) return 0;
	if (c > 127) return 0;
	return isspace(c);
}

XStrCmd::XStrCmd(int nID, const wchar_t* szName, int nMinArgs, int nMaxArgs, const wchar_t* szUsage, const wchar_t* szHelp)
{
	m_nID = nID;

	m_fnProc = NULL;
	m_nMinArgs = nMinArgs;
	m_nMaxArgs = nMaxArgs;

	wcscpy_s(m_szName, szName);
	wcscpy_s(m_szUsage, szUsage);
	wcscpy_s(m_szHelp, szHelp);
}

bool XStrCmd::OnProc(const wchar_t* line, const int argc, wchar_t **const argv)
{
	if (m_fnProc != NULL)
	{
		return (*m_fnProc)(line, argc, argv);
	}
	return false;
}



XStrCmd* XStrCmdManager::MakeArgv(wchar_t* szLine, XStrCmdArgvInfo* pAI)
{
	int nLen = (int)wcslen(szLine);
	if (nLen >= 2048) NULL;

	wchar_t szBuffer[2048];
	wcscpy_s(szBuffer, szLine);

	// ���ڿ� ���� �����̽� ����
	for (int pos = nLen-1; pos >= 0; pos--)
	{
		if (IsSpace(szBuffer[pos])) szBuffer[pos] = '\0';
		else break;
	}


	XStrCmd* pCmd = NULL;

	int c;
	wchar_t* scp;
	wchar_t* dcp;
	wchar_t* dlim;
	wchar_t* arg;

	int nArgcMax, nCmdArgcMax;
	nCmdArgcMax = nArgcMax = (sizeof(pAI->cargv) / sizeof(wchar_t*));
	

	scp = szBuffer;
	dcp = pAI->argbuf;
	dlim = dcp + sizeof(pAI->argbuf) - 1;

	for (pAI->cargc = 0; pAI->cargc < nArgcMax; )
	{
		for ( ; ; scp++)
		{
			c = *scp;
			if (IsSpace(c)) continue;

			if ( (c == '\0') || (c == '\n') )
			{
				pAI->cargv[pAI->cargc] = NULL;
				return pCmd;
			}
			break;
		}
		arg = dcp;
		pAI->cargv[pAI->cargc] = arg;
		(pAI->cargc)++;

		for ( ; ; )
		{
			c = *scp;
			if ( (c == '\0') || (c == '\n')) break;

			// ������ ���ڴ� ������ ���ڿ� ������������ ���̴�.
			if (pAI->cargc != nCmdArgcMax)
			{
				if (IsSpace(c)) break;
			}
			scp++;

			// ���� ����ǥ ó��
			if (c == '\'') 
			{
				for ( ; ; ) 
				{
					c = *scp++;

//					 ����ǥ�� �ϳ��� ������ �� ����
					if (c == '\0') return NULL;

					if (c == '\'') break;

//					 �ʹ� ���
					if (dcp >= dlim) return NULL;

					*dcp++ = c;
				}
			} 
			else if (c == '"')	// ū����ǥó��
			{	
				for ( ; ; ) 
				{
					c = *scp++;
					if (c == '\0') return NULL;
					if (c == '"') break;

					if (dcp >= dlim) return NULL;
					*dcp++ = c;
				}
			}
            else 
			{
				if (dcp >= dlim) return NULL;
				*dcp++ = c;
			}
		}

		*dcp++ = '\0';

		if (pAI->cargc == 1)
		{
			pCmd = GetCommandByName(pAI->cargv[0]);
			if (pCmd != NULL) 
			{
				if (pCmd->GetMaxArgs() != ARGVNoMin)
				{
					nCmdArgcMax = pCmd->GetMaxArgs() + 1;
				}
			}
			else
			{
				return NULL;
			}
		}
	}
	
	return NULL;
}


void XStrCmdManager::AddCommand(int nID, const wchar_t* szName, XStrCmdProc* fnProc, 
								int nMinArgs, int nMaxArgs, const wchar_t* szUsage, const wchar_t* szHelp)
{
	wchar_t szLwrName[256];
	wcscpy_s(szLwrName, szName);
	_wcslwr(szLwrName);

	if (m_CmdMap.find(wstring(szLwrName)) != m_CmdMap.end())
	{
		_ASSERTE(0 && "�ܼ� Ŀ�ǵ尡 �ߺ���");
		return;
	}

	XStrCmd* pCmd = new XStrCmd(nID, szLwrName, nMinArgs, nMaxArgs, szUsage, szHelp);
	
	pCmd->SetProc(fnProc);
	m_CmdMap.insert(XStrCmdMap::value_type(szLwrName, pCmd));
}

bool XStrCmdManager::DoCommand(wchar_t* szLine)
{
	if ((szLine == 0) || (szLine[0] == 0)) return false;
	
	XStrCmdArgvInfo ai;
	memset(&ai, 0, sizeof(XStrCmdArgvInfo));

	XStrCmd* pCmd = MakeArgv(szLine, &ai);
	if (pCmd != NULL)
	{
		return pCmd->OnProc(szLine, ai.cargc, ai.cargv);
	}

	return false;
}

XStrCmdManager::XStrCmdManager()
{

}

XStrCmdManager::~XStrCmdManager()
{
	Clear();
}

void XStrCmdManager::Clear()
{
	while (!m_CmdMap.empty())
	{
		XStrCmdMap::iterator i = m_CmdMap.begin();
		delete (*i).second;
		m_CmdMap.erase(i);
	}
	m_AliasMap.clear();
}

XStrCmd* XStrCmdManager::GetCommandByName(const wchar_t* szName)
{
	wchar_t szLwrName[256];
	wcscpy_s(szLwrName, szName);
	_wcslwr(szLwrName);


	wchar_t szCmdName[256] = L"";

	// alias���� ���� ã�´�.
	map<std::wstring, std::wstring>::iterator itorAlias = m_AliasMap.find(wstring(szLwrName));
	if (itorAlias != m_AliasMap.end())
	{
		wcscpy_s(szCmdName, ((*itorAlias).second).c_str());
	}
	else
	{
		wcscpy_s(szCmdName, szLwrName);
	}

	// ������ ã��
	XStrCmdMap::iterator pos = m_CmdMap.find(wstring(szCmdName));
	if (pos != m_CmdMap.end())
	{
		XStrCmd* pCmd = (*pos).second;
		return pCmd;
	}
	else
	{
		return NULL;
	}
}

XStrCmd* XStrCmdManager::GetCommandByID(int nID)
{
	for (XStrCmdMap::iterator itor = m_CmdMap.begin(); itor != m_CmdMap.end(); ++itor)
	{
		XStrCmd* pChatCmd = (*itor).second;
		if (pChatCmd->GetID() == nID) return pChatCmd;
	}
	
	return NULL;
}

void XStrCmdManager::AddAlias(const wchar_t* szNewCmdName, const wchar_t* szTarCmdName)
{
	wchar_t szLwrName[256];
	wcscpy_s(szLwrName, szNewCmdName);
	_wcslwr(szLwrName);

	m_AliasMap.insert(map<std::wstring, std::wstring>::value_type(std::wstring(szLwrName), std::wstring(szTarCmdName)));
}






