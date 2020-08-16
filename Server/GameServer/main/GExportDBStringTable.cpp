#include "stdafx.h"
#include "GExportDBStringTable.h"
#include "CStringResMgr.h"
#include "CLangInfo.h"

GExportDBStringTable::GExportDBStringTable()
: m_pTable(NULL)
{
	// do nothing
}

GExportDBStringTable::~GExportDBStringTable()
{
	SAFE_DELETE(m_pTable);
}

bool GExportDBStringTable::Load(wstring strLocale)
{
	// ����
	if (m_pTable == NULL)
	{
		m_pTable = new CStringResMgr();
	}


	// Ŭ���̾�Ʈ ���� ���� - Runtime�� ������ ����
	CLangNode node;
	node.strLocale = strLocale;
	node.strPath = L"..\\..\\Data\\system\\lang\\" + strLocale + L"\\";


	// SQL Export���� �����ϴ� �̸� ����
	vector<wstring> fieldList;
	
	fieldList.push_back(L"name_xitem.xml");
	fieldList.push_back(L"name_quest.xml");
	fieldList.push_back(L"name_talent.xml");

	
	// �б� - �����ϸ� ���ڿ� ���̺��� ���鹮�ڿ��� ��ȯ
	if (!m_pTable->Init(node, fieldList))
	{
		mlog("Failed to loading files for sql export string tables.");
		return false;
	}


	return true;
}

wstring GExportDBStringTable::GetString(wstring strKey) const
{
	if (m_pTable == NULL)	return L"";

	return m_pTable->GetString(strKey);
}
