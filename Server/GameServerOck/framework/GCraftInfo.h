#ifndef _G_CRAFT_INFO_H_
#define _G_CRAFT_INFO_H_

#include "GFrameworkLib.h"
enum CRAFT_TYPE;

class GFRAMEWORK_API GCraftInfo : public MTestMemPool<GCraftInfo>
{
public:
	int			m_nID;
	int			m_nDialogID;	///< ��µ� ���
	float		m_fBaseMakeMod;	///< �⺻ ����
	int			m_nFactionID;	///< ������ �����ִ� �Ѽ�(������ ����)
	set<int>	m_setRecipeID;	///< ���� ������ ������ ��� 	

	bool IsHaveRecipeID(int nRecipeID);

public:
	GCraftInfo();
	~GCraftInfo();
};

#endif//_G_CRAFT_INFO_H_
