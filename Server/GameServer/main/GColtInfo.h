#pragma once

#include "CSChrInfo.h" // for NPC_MODE_MAX

#include <string>
#include <vector>

enum COLT_CHECK_TYPE
{
	AST_NONE = 0,
	AST_HP,
	AST_EN,
	AST_SP,
	AST_UNBREAKPART,
	AST_BREAKPART,
	AST_DAMAGE,
	AST_MOTIONFACTOR,
	AST_DISTANCE,
	AST_ANGLE,
	AST_MODE,
	AST_ENEMY_HP,
	AST_ENEMY_MOTIONFACTOR,
	AST_STRESS,
	AST_HIT_CAPSULE,	// NPC�� �浹 ĸ�� ����
	AST_USED_TALENT,	// ������� �ŷ�Ʈ
	AST_GAINED_BUFF,	// ��� �ִ� ����
	AST_ENEMY_GAINED_BUFF,	// ��� �ִ� ����
	AST_REAR,			// ��ǥ �ڿ� ��ġ�� ���
	AST_FRONT,			// ��ǥ �տ� ��ġ�� ���
	AST_RAGE,			// �г�ġ
	AST_DEAD,			// �׾�����
	AST_COMBAT,			// ����������
	AST_PEACE,			// ������������
	AST_SIZE,
};

enum COLT_ACTION_TYPE
{
	CAT_NONE = 0,
	CAT_NOTHING,
	CAT_GROUP,			// �׷� �׼� (�ƹ��ϵ� ���� ����)
	CAT_TALENT,			// �ŷ�Ʈ�� ���
	CAT_LUAFUNC,		// ��� �Լ��� ȣ��
	CAT_BREAKPART,		// ��Ʈ �ı�
	CAT_DAMAGE,
	CAT_MOTIONFACTOR,	// ��� ���� ��ȭ
	CAT_FLEE,			// ����
	CAT_YELL,			// ��ħ (�����û)
	CAT_GUARD,			// ���
	CAT_ADJUST,			// �Ĺ� �⺻ �Ӽ� ����
	CAT_DISTANCE,		// Ÿ������ ���� �Ÿ��� ����
	CAT_FOLLOW,			// Ÿ���� �i�ư���
	CAT_MOVE,			// ������ġ�� Ÿ�� ������ ������ �ش��ϴ� �������� �̵�
	CAT_CHANGE_HIT_CAPSULE,	// NPC�� �浹 ĸ�� ����
	CAT_SAY,				// ��ȭ: ���ϱ�
	CAT_BALLOON,				// ��ȭ: ��ǳ��
	CAT_SHOUT,				// ��ȭ: ��ġ��
	CAT_NARRATION,			// ��ȭ: �����̼�
	CAT_AGGRO,				// ��׷� ����
	CAT_FACETO,

	CAT_SIZE,
};


#define MAX_AI_INFO_PARAM 4
#define INVALID_AI_ACTION_COUNT	-1

struct GColtAction;

// Colt �׼ǿ� ���� �ν��Ͻ� ����, ��� NPC ��ü���� ������ �����Ǿ��ϴ� �������� ����ü
class GColtActionInst
{
	friend GColtActionInst;
public:
	GColtActionInst();
	GColtActionInst(const GColtAction& action);

	// ���� ��� �������� ����
	bool IsEnableCycleTime() const;

	// ����Ŭ Ÿ���� ��밡���ϰ� ����
	void SetEnableCycleTime();

	// ����Ŭ Ÿ�� �ʱ�ȭ
	void ResetCycleTime(float fDelaySecond);

	// Duration �ð��� �������� ����
	bool IsInfinityTime();
	
	// ��� Ƚ�� ����
	void IncreaseUseCount();

	// ������ ���� ���� ����
	vector<GColtActionInst>& GetInnerInfos();

	// Ȯ�� ��ȯ
	int GetRate() const;
	
	// ������ Ȯ������ ����
	void SetRate(int nRate); 

	// ���ӽð� ��ȯ
	float GetDurationTime() const;

	// ����� Ƚ�� ��ȯ
	int  GetUseCount() const;

	// ����Ŭ Ÿ�� ��ȯ
	DWORD GetCycleTime() const;

	// DurationTime ��뿩��
	bool IsModifiedDurationTime() const;

	// ��Ʈ���� �޴��� ����
	bool IsGainStress() const;

	bool operator==(const GColtActionInst& other) const;

private:
	int				m_nModifiedRate;		// GColtActionInfo::nRate�� ������� ����� ���� Rate
	int				m_nCurCount;			// �ش� �׼��� ���� Ƚ��
	DWORD			m_dwCycleTime;			// ������ �׼Ǳ��� ����ؾ��� �ð�
	float			m_fDurationTime;		// �ش� �׼��� ���ӵǴ� �ð�
	bool			m_bModifyDurationTime;	// DurationTime ��뿩��
	bool			m_bGainStress;			// �ŷ�Ʈ�� �����Ҷ� ��Ʈ������ ������ ���� (NPC ����)
	vector<GColtActionInst>  m_vecInnerInfos;
};

// Colt �׼�, üũ�� ���� NPC�� �ؾ��� �ൿ�� ���� ����
struct GColtAction
{
	COLT_ACTION_TYPE	nType;			// �׼� Ÿ��
	vector<wstring>	qParams;		// �׼� �Ķ����
	int				nRate;			// ���õ� Ȯ�� (������)
	int				nMaxCount;		// �ش� �׼��� ���� �ִ� ���Ƚ�� (0 ���ϰ� �Ǹ� NPC�� �� �̻� �ش� �׼��� �������� �ʴ´�)
	float			fCycleTime;		// ������ �׼Ǳ��� ����ؾ��� �ð�
	float			fDurationTime;	// �ش� �׼��� ���ӵǴ� �ð�
	bool			bGainStress;	// �ŷ�Ʈ�� �����Ҷ� ��Ʈ������ ������ ���� (NPC ����)
	bool			bAutoRun;		// Rate�� ������� ������ ����Ǵ��� ����
	bool			bModifyDurationTime;	// DurationTime�� �����ߴ��� ����

	vector<GColtAction>  vecSeqActions;

	GColtAction();
	
	bool IsPickable( const GColtActionInst& inst ) const;
	bool IsExpiredCount(const GColtActionInst& inst) const;

	bool operator==(const GColtAction& other) const
	{
		return nType == other.nType && 
			qParams == other.qParams &&
			nRate == other.nRate &&
			nMaxCount == other.nMaxCount &&
			fCycleTime == other.fCycleTime &&
			fDurationTime == other.fDurationTime &&
			bGainStress == other.bGainStress &&
			bAutoRun == other.bAutoRun &&
			bModifyDurationTime == other.bModifyDurationTime;
	}
};

typedef vector<pair<bool,int> >	COLT_PARAM_DATA;


// Colt üũ
struct GColtCheck : public MTestMemPool<GColtCheck>
{
	COLT_CHECK_TYPE			nType;				// üũ Ÿ��
	COLT_PARAM_DATA			vecParams;			// ���� �Ķ����: MAX_AI_INFO_PARAM�� �ִ� ������ �������� first�� ��뿩��, second�� ���� ��
	vector<GColtAction>		vecActions;			// �ش� üũ�� ������ �����Ǹ� ������ �׼ǵ��� ���� (rate ������� �ϳ��� �����ϸ� ����)
	vector<GColtAction>		vecAutoRunActions;	// �ش� üũ�� ������ �����Ǹ� ������ �׼ǵ��� ���� (qAction�� ������ ���䷱ �׼��� ������ ��� ���� ���� ���� ����)
	vector<GColtCheck*>		vecChecks;			// ��ø üũ: ���� üũ�� ������ �����Ǵ� ���¿��� ��ø�Ǵ� üũ�� ���ǵǾ� ������ ���� üũ�� ���õǸ� ��ø üũ��� ������� �Ѿ��.
	GColtCheck*				pCheckDefault;		// ��ø üũ�� �⺻ üũ
	int8					nPickRate;

	GColtCheck();
	~GColtCheck();

public:
	// �׻� �˻縦 ���� ����
	bool CheckActionConditions() const;

	// ���� üũ���� �ִ��� ����
	bool HasInnerChecks() const;
	// ���� üũ���� �ִ��� ����
	bool HasActions() const;
	// auto run �� �ִ��� ����
	bool HasAutoActions() const;

	// �ʱ�ȭ
	void Clear();

	// ����
	void Copy(GColtCheck& new_check) const;
};

// Colt ����ü
struct GColtGroupInfo
{
	struct CombatAttr
	{
		// �ٿ������ ���� �ŷ�Ʈ ID
		int	nDownAttackTalentID;	
		// �¸��� �����ϴ� �ŷ�Ʈ ID
		int	nVictoryTalentID;		

		CombatAttr();

		void Clear();
	};

	CombatAttr attrCombat[NPC_MODE_MAX];
	// ���� üũ����Ʈ
	vector<GColtCheck*>	vecCombatChecks[NPC_MODE_MAX];
	// �ǰݽ� ���� üũ����Ʈ
	vector<GColtCheck*>	vecReactionChecks;	
	// ����Ʈ �ʱ�߻� üũ����Ʈ (1ȸ �߻�)
	vector<GColtCheck*>	vecAggroChecks;
	// ����Ʈ �ʱ�߻� üũ����Ʈ (������ 1ȸ �߻�)
	vector<GColtCheck*>	vecPrologue;
	// �ױ� ������ �߻� (������ 1ȸ �߻�)
	vector<GColtCheck*>	vecEpilogue;
	// ��Ʈ���� �߻� üũ����Ʈ
	vector<GColtCheck*>	vecStressChecks;	
	// ���� üũ����Ʈ
	vector<GColtCheck*>	vecIdleChecks;		

	GColtCheck			checkCombatDefault[NPC_MODE_MAX];
	GColtCheck			checkIdleDefault;
	GColtCheck			checkAggroDefault;
	GColtCheck			checkPrologueDefault;
	GColtCheck			checkEpilogueDefault;

	GColtGroupInfo();
	~GColtGroupInfo();

	void Clear();

	// ����
	void Copy(GColtGroupInfo& new_info);
private:
	void _EachClear(vector<GColtCheck*>& vecCheckinfos);
	void _EachClone( const vector<GColtCheck*>& source, vector<GColtCheck*>& dest );
	
};
