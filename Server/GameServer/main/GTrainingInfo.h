#ifndef _G_TRAINING_INFO_H_
#define _G_TRAINING_INFO_H_

class GTalentInfo;
class GConditionsInfo;

enum TRAINING_TYPE
{
	TRA_NONE = 0,
	TRA_COMBAT,
	TRA_CRAFT,

	TRA_MAX
};

class GTrainingInfo
{
public:
	int				m_nID;

	GDialogInfo*	m_pDialogInfo;
	TRAINING_TYPE	nTrainingType;

	set<GTalentInfo*>					m_setNormalTalent;		///< �ƹ��� ��� �� �ִ� �ŷ�Ʈ
	map<GTalentInfo*, GConditionsInfo*>	m_mapConditionsTalent;	///< ������� �����ϴ� �ڸ� ��� �� �ִ� �ŷ�Ʈ
	
	GConditionsInfo* GetConditionsInfo(GTalentInfo* pTalentInfo);


public:
	GTrainingInfo();
	~GTrainingInfo();
};

#endif//_G_TRAINING_INFO_H_
