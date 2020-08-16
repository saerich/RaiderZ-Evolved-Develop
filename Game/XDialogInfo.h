#pragma once

class XDialogInfo
{
private:
	int								m_nSayTextID;	// ��ȭID
	vector<TD_DIALOG_SELECT_NODE>	m_vecDialogSelectNode;

	wstring							m_strSay;	//��ȯ��
public:
	XDialogInfo();

	void Clear();

	MWLua::table GetDialogSelectTable();

	int&							GetSayTextID()	{ return m_nSayTextID; }
	vector<TD_DIALOG_SELECT_NODE>&	GetList()	{ return m_vecDialogSelectNode; }

	const wchar_t* GetSayString();
};
