#pragma once

namespace immune
{
	enum CUSTOM_TYPE
	{
		IMMUNE_DRAIN = 0,
		IMMUNE_DISPEL,
		IMMUNE_MAX,
	};

	static const wchar_t* CUSTOM_TYPE_STR[] = 
	{
		L"DRAIN",
		L"DISPEL",
	};

	struct INFO
	{
		// ������ ���� �鿪 ��Ʈ��
		uint32			nImmuneBuff;		
		// ������Ÿ�Կ� ���� �鿪 ��Ʈ��
		uint32			nImmuneDamageType;	
		// ��ųŸ�Կ� ���� �鿪 ��Ʈ��
		uint32			nImmuneSkillType;	
		// Ŀ����Ÿ�Կ� ���� �鿪 ��Ʈ��
		int32			nImmuneCustomType;	

		INFO()
		{
			nImmuneBuff = 0L;
			nImmuneDamageType = 0L;
			nImmuneSkillType = 0L;
			nImmuneCustomType = 0L;
		}
	};
} // namespace immune

