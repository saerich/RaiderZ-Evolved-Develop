#ifndef _CS_RECIPE_INFO_H
#define _CS_RECIPE_INFO_H

#include "CSCommonLib.h"

#define MAX_RECIPE_MATERIAL_TYPE 5

struct RecipeMaterial
{
	RecipeMaterial(int nItemID, int nAmount)
	: m_nItemID(nItemID), m_nAmount(nAmount)
	{

	}

	int m_nItemID;
	int m_nAmount;
};

class CSCOMMON_API CSRecipeInfo
{
public:
	int			m_nID;
	int			m_nProductItemID;			///< ���� ������
	int			m_nAmount;					///< ���귮
	vector<RecipeMaterial> m_vecMaterial;	///< ��� ������
	int			m_nPrice;					///< ���꿡 �ʿ��� ���� ��. ������ �ǹ�
	int			m_nConditionID;				///< �̾������� �����ϱ� ���� ����

	set<int>	m_setMaterialItemID;		///< Ư�� �������� ��� ���������� Ȯ���Ҷ� ���

public:
	CSRecipeInfo();
	~CSRecipeInfo();

	bool	IsMaterialItemID(int nItemID);
	int		GetMaterialItemAmount(int nItemID);
};


#endif//_CS_RECIPE_INFO_H