#pragma once

struct CLangNode
{
	wstring strName;		// ����
	wstring strCountry;		// ������
	wstring strLocale;		// �����ϸ�
	wstring strPath;		// ���
};

class CLangInfo: public vector<CLangNode>
{
public:
	CLangInfo();
	virtual ~CLangInfo();

	bool Load(wstring strFileName);
	wstring GetLocale(void) const;

private:
	wstring m_strLocale;
};
