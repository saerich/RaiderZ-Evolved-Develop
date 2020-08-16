#include "stdafx.h"
#include "GSQLExporter.h"
#include "GGlobal.h"
#include "GDBManager.h"



GSQLExporter::GSQLExporter()
{

}

GSQLExporter::~GSQLExporter()
{

}

bool GSQLExporter::ExportToDB( const std::vector<std::wstring>& vSQL )
{
	if (NULL == gsys.pDBManager)
		return false;

	for (size_t i = 0; i < vSQL.size(); ++i)
	{
		gsys.pDBManager->Execute(vSQL[i].c_str());
	}

	return true;
}

bool GSQLExporter::ExportToFile( const wstring& strFile, const std::vector<std::wstring>& vSQL )
{
	FILE* pFile;

	// �����ڵ� ���� ��, ������ ĳ���ͼ¿� ���� ���ڸ� ǥ������ ���ϴ� ������ ���ϱ� ���� �������� ���ϴ�.
	if (0 != _wfopen_s(&pFile, strFile.c_str(), L"wb"))
	{		
		return false;
	}


	// �����ڵ� BOM ����
	BYTE bom[2] = {0xFF, 0xFE};		// UTF-16, little-endian	
	fwrite(bom, sizeof(BYTE), 2, pFile);	


	// ���� ����
	for (size_t i = 0; i < vSQL.size(); ++i)
	{
		// �ؽ�Ʈ ��忡�� �����ϴ� \n -> \r\n �ڵ������� �������� �������� �ʱ⶧���� ������ȯ�Ѵ�.
		CString str = vSQL[i].c_str();
		str.Replace(L"\n", L"\r\n");

		size_t nWriteLenth = fwrite(str.GetString(), sizeof(wchar_t), str.GetLength(), pFile);

		if (str.GetLength() != nWriteLenth)
		{
			fclose(pFile);
			return false;
		}
	}

	fclose(pFile);
	return true;
}
