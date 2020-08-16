#pragma once

class XDebugInfoBuilder
{
private:
	wstring		m_strBuf;
	void AddLine(wstring& str);
	wstring _Build_Header();
	wstring _Build_Tail();
	wstring _Build_Server();
	wstring _Build_Field();
	wstring _Build_Character();
public:
	XDebugInfoBuilder()
	{

	}
	~XDebugInfoBuilder()
	{

	}

	void Build();

	wstring& GetInfo() { return m_strBuf; }
	wstring GetInfoForClipboard();
};

/*

#������ ����
#���� ��ġ(�ʵ�, ��ǥ), ���� �ð�, ����, 
#ĳ���� �̸�, ĳ���� ���� , ���� , ĳ���� ���Ľ�

�������� ���
���� �������� ���� ����
ĳ���� ���� ���°�, ��Ƽ ����, ��Ƽ��� ��Ƽ ����(��� ���)

�ػ� , â��� ���� , 
*/