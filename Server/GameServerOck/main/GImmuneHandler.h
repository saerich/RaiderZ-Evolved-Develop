#pragma once

#include "GImmuneInfo.h"

enum DAMAGE_ATTRIB;
enum TALENT_SKILL_TYPE;
enum BUFF_PASSIVE_EXTRA_ATTRIB_;

class GImmuneHandler
{
public:
	GImmuneHandler(void);
	~GImmuneHandler(void);

	// �ʱ�ȭ
	void Init(immune::INFO info);

	// ������Ÿ�Կ� ���� �鿪����
	bool IsDamageTypeImmune(DAMAGE_ATTRIB nDamageType) const;
	// ��ųŸ�Կ� ���� �鿪����
	bool IsSkillTypeImmune( TALENT_SKILL_TYPE nSkillType ) const;
	// ����Ÿ�Կ� ���� �鿪����
	bool IsBuffImmune(BUFF_PASSIVE_EXTRA_ATTRIB_ nBuffExtraType) const;
	// Ŀ����Ÿ�Կ� ���� �鿪����
	bool IsImmune(immune::CUSTOM_TYPE nCustomType) const;
private:
	bool			m_bInit;
	immune::INFO	m_Info;
};
