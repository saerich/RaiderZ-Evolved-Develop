#pragma once

enum ERROR_MSG_TYPE
{
	EMT_NAME,			// �̸��� ����.
	EMT_LINK,			// ����� ���Ͽ� �̸��� ����.
	EMT_FILE,			// ������ ����.
	EMT_STRING,			// ��Ÿ
};

struct stERRORMSG
{
	ERROR_MSG_TYPE				eTYpe;
	string						strErrorMsg;
};

class VErrorMsg
{
public:
	vector<stERRORMSG>				m_strErrorMsg;

public:
	VErrorMsg();
	virtual ~VErrorMsg();

	void						Add(ERROR_MSG_TYPE eType, string& strMsg);
	void						Add(VErrorMsg& errorMsg);

	void						AddErrorXml(string& strXmlFileName);
	void						AddErrorFile(string& strFileName);

	void						Clear();

	bool						IsError();
};

