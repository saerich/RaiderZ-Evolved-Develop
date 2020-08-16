#pragma once

#include <fstream>
#include <deque>

// ��ũ��Ʈ ���ڿ��� �߰����� ó���� �ϴ� Ŭ����
class GLuaThisEnchanter
{
public:
	// ��ȯ�� ����Լ��� �߰�
	void AddMatchMemFunc(string strName);

	// this Ű���带 �Լ� ��ó�� ���ڷ� ����
	bool ConvertFromFile(string strFilename, string& outstrBuffer);

	// this Ű���带 �Լ� ��ó�� ���ڷ� ����
	bool ConvertFromStream(string strStream, string& outstrBuffer);

	// this Ű���带 �Լ� ��ó�� ���ڷ� ����
	string ConvertLine(string strLine);

	// ���ڿ��� ���Ϸ� ����
	bool SaveToFile(string strFileName, const string& strText);

private:
	bool IsMatchMemFunc(string strLine);
	bool IsDeclFuncLine(string strLine);
	bool HasComma(string strLine);
	bool HasColon(string strLine);
private:
	// ��ȯ�� ����Լ��� ����
	vector<string>	m_vecMatchMemFuncs;
};
