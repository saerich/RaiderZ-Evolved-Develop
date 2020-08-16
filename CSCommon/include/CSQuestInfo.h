#ifndef _CS_QUEST_INFO_H
#define _CS_QUEST_INFO_H

#include <string>
#include "MTstring.h"
#include <vector>
#include "MTypes.h"
#include "CSCommonLib.h"

using namespace std;
enum QUEST_SHARE_TYPE;
enum QUEST_TRIAL_TYPE;
enum QUEST_REWARD_TYPE;
enum QOBJECTIVE_TYPE;

class CSQObjectiveInfo;
struct QUEST_REWARD;

typedef vector<QUEST_REWARD>		VEC_QUEST_REWARD;
typedef map<int, CSQObjectiveInfo*>	MAP_QOBJECTIVEINFO;

/// ����Ʈ ����
class CSCOMMON_API CSQuestInfo
{
public:
	CSQuestInfo();
	virtual ~CSQuestInfo();	

	bool Insert(CSQObjectiveInfo* pQObjectiveInfo);
	CSQObjectiveInfo* Get(int nQObjectiveID);	

	bool IsTrialQuest();
	bool IsEscortQuest();
	bool IsEmptyQObjective();
	bool IsAutoParty()			{ return bAutoParty;}
	bool CheckRequireLevel_ForChallengerQuest(int nLevel) const;

	vector<CSQObjectiveInfo*> GetQObjectiveInfo(QOBJECTIVE_TYPE nType);

	vector<CSQObjectiveInfo*> GetAllQObjectiveInfo();
	vector<CSQObjectiveInfo*> GetActQObjectiveInfo();
	vector<CSQObjectiveInfo*> GetEscortQObjectiveInfo();
	vector<CSQObjectiveInfo*> GetCollectQObjectiveInfo();	

	int GetQObejctiveInfoCount();

public:
	int			nID;				///< ����Ʈ ID	
	tstring		strTitle;			///< ����Ʈ ����
	tstring		strDesc;			///< ����Ʈ ����
	tstring		strCategory;		///< �������̽��� ���� ���
	int			nTimeLimit;			///< ���� ���ѽð� (�ʴ���)
	int			nLevel;				///< ����Ʈ ����
	bool		bRepeatable;		///< �ݺ� ������ �������� ����
	bool		bCancelable;		///< ��� �������� ����
	QUEST_SHARE_TYPE		nShareType;			///< ���� Ÿ��
	int			nLimitPersonnel;				///< ���� �ο���

	QUEST_TRIAL_TYPE	nTrialType;			///< Ʈ���̾� Ÿ��	
	int					nTrialFieldGroupID;	///< Ʈ���̾� ����Ʈ�� �����ϴ� �ʵ�׷�	

	int				nGiveDialogID;			///< ����Ʈ�� �� �� ����ϴ� ���̾�α�
	int				nIncompleteDialogID;	///< ����Ʈ�� ���� �Ϸ����� �������� ����ϴ� ���̾�α�	
	int				nShareConditionsID;		///< ����Ʈ ������ �������� üũ�ϴ� �����

	int					nRewardDialogID;			///< ���� ����ϴ� ���̾�α�	
	int					nRewardNPCID;				///< �������ִ� NPCID
	int					nRewardFieldID;				///< ���� �ʵ� ID
	vec3				vRewardPos;					///< ���� ��ǥ
	VEC_QUEST_REWARD	vecReward;					///< ����Ʈ ����
	VEC_QUEST_REWARD	vecSelectableReward;		///< ���� ������ ����Ʈ ����

	vector<int>			vecDestroyItemID;			///< ����Ʈ �Ϸ�� ��ҽ� �ı��Ǵ� ������	

	int					nPartyFieldID;				///< �ڵ���Ƽ��Ī ����Ʈ�� �ʵ�ID
	int					nPartySensorID;				///< �ڵ���Ƽ��Ī ����Ʈ�� ����ID
	int					nAutoPartyMemberLimit;		///< �ڵ���Ƽ �ּ��ο�

	bool				bAutoParty;					///< �ڵ���Ƽ��Ī�� �Ǵ� ����Ʈ���� ����
	bool				bPublicVar;					///< var ����� ��Ƽ���� ����z
	bool				bValidation;				///< ResourceValidator ��ȿ���˻縦 ����, ���� ���� - �⺻�� true


	int			nRequiredMinLevelForChallengerQuest;	///< ������ ����Ʈ ������ �ʿ� �ּҷ���
	int			nRequiredMaxLevelForChallengerQuest;	///< ������ ����Ʈ ������ �ʿ� �ִ뷹��
	int			nRequiredConditionIDForChallengerQuest;	///< ������ ����Ʈ ������ �ʿ� ����


protected:
	MAP_QOBJECTIVEINFO	mapQObjectiveInfo;	///< ����Ʈ ����
};

/// ����Ʈ ����
struct QUEST_REWARD
{
public:
	QUEST_REWARD_TYPE	nType;
	int					nParam1;
	int					nParam2;

public:
	QUEST_REWARD();

	QUEST_REWARD(QUEST_REWARD_TYPE	nType, int nParam1, int	nParam2);
	QUEST_REWARD(QUEST_REWARD_TYPE	nType, int nParam1);
};


#endif // _CS_QUEST_INFO_H
