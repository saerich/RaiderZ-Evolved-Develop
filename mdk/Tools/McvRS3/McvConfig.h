#pragma once

#include <string>

using namespace rs3;

class McvConfig
{
public:
	McvConfig(void);
	~McvConfig(void);

	bool LoadConfig(const string& strFileName);

	static McvConfig& GetInstance();

	float	m_fFarZ;
	string	m_strRegistryRoot;
	string	m_strLightAni;		// ����Ʈ ���� ���ϸ��̼� �̸������Ͽ� �ش� ���ϸ��̼� ���� ���� �ϰ� �Ѵ�.
};
