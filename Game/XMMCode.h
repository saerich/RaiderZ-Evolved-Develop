#pragma once

#include "MMCode.h"


class XMMCode
{
private:
	mmcode::MTagManager		m_TagManager;
	void InitControlCode();
	void InitReplaceChars();
public:
	XMMCode();
	~XMMCode(){}
	void Init();

	mmcode::MTagManager& GetTagManager() { return m_TagManager; }
};


class XMMCodeMgr
{
private:
	XMMCode		m_MMCode;
public:
	XMMCodeMgr()
	{
		m_MMCode.Init();
	}
	static XMMCodeMgr& GetInstance();
	mmcode::MTagManager& GetTagManager() { return m_MMCode.GetTagManager(); }

	// ���ڿ��� ���� �����ڵ常 ����
	void ExecControlCode(const wchar_t* szKey, MUID uidOwner);

	// ġȯ�ڸ� ġȯ�ϰ� �����ڵ���� �����ؼ� ��ȯ. �ٸ� �����ڵ��� ������ ���� �ʴ´�.
	mmcode::MControlCodeTransResult TransControlCodeAndReplaceChars(const wchar_t* szKey, MUID uidOwner);

	//���ڿ��� ���� �ڵ带 �������� ��Ʈ���� ������.
	void TransControlSpliterChars(list<string>& strlist, const char* szKey, MUID uidOwner );
};