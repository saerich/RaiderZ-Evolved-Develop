#pragma once

/// ���� Request Ŀ�ǵ带 �ݺ��ؼ� ������ ���� �����ϱ� ���� Ŭ����
class XRequestCommandFilter
{
private:
	set<int>			m_RequestCommandFilter;			// ���͸��� ��ü Request Command ����
	map<int, int>		m_FilterMap;					// response command id�� Key�̴�.
	set<int>			m_CurrentSendRequestCommands;	// ���� ���͸����� Request Command
public:
	XRequestCommandFilter() {}
	~XRequestCommandFilter() {}
	void AddFilter(int nRequestCommandID, int nResponseCommandID);

	bool CheckRequestEnable(int nRequestCommandID);
	void OnResponse(int nResponseCommandID);
	void ClearSendRequestCommands();
};


