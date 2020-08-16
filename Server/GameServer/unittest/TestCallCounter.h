#pragma once

class TestCallCounter
{
public:
	// �Լ� ȣ�� ī���� ��ȯ
	int GetCallCounter(wstring strFuncName);

protected:
	// �Լ� ȣ�� ī���͸� ������Ŵ
	void IncreaseCallCounter(wstring strFuncName);

private:
	typedef map<wstring, int>		MAP_CALLCOUNTER;
	// �׽�Ʈ������ ���̴� �Լ� ȣ�� ī����
	MAP_CALLCOUNTER		m_mapTestCallCounter;
};

#define INCREASE_CALLCOUNTER { this->IncreaseCallCounter(__FUNCTIONW__); }