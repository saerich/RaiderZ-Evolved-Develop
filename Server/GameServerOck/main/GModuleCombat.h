#ifndef _GMODULE_COMBAT_H
#define _GMODULE_COMBAT_H

#include "GModule.h"
#include "GTalentInfo.h"
#include "GTalentHitter.h"
#include "CSdef.h"
#include "MMemPool.h"
#include "MTime.h"

enum CCommandResultTable;
class GTalent;
class GEntityActor;
class GTalentHitter;
class GTalentFocusMgr;
class GItem;
class GEntityPlayer;

////////////////////////////////////////////////////////////////

class GInvincibleProcessor
{
private:
	bool		m_bInvincible;
	MRegulator	m_Regulator;
public:
	GInvincibleProcessor();

	bool		IsInvincible();
	void		DoInvincible(float fDurationTime);
	void		Update(float fDelta);
};


class GTalentRangeChecker
{
public:
	bool Check(GTalentInfo* pAttackTalentInfo, const vec3& vAttackerPos, const vec3& vAttackerDir, float fAttackerRadius, const vec3& vTargetPos, float fTargetRadius);
};

/// ��������
class GModuleCombat : public GModule, public MMemPool<GModuleCombat>
{
private:
	friend class GEntityActor;
	DECLARE_ID(GMID_COMBAT);
protected:
	GTalent*				m_pTalent;
	GTalentHitterList		m_TargetHitterList;
	GEntityActor*			m_pOwnerActor;
	GInvincibleProcessor	m_Invincible;						///< ���� ����
	GTalentFocusMgr*		m_pTalentFocusMgr;

	virtual void			OnUpdate(float fDelta);
	void					UpdateActiveTalents(float fDelta);

	bool IsDeletable();

private:
	// ��ų ����
	CCommandResultTable CheckPlayerTalentEnable(GEntityPlayer* pOwnerPlayer, GTalentInfo* pTalentInfo);

public:
	GModuleCombat(GEntity* pOwner);
	virtual ~GModuleCombat(void);

	virtual void			OnInitialized();	///< �ʱ�ȭ�� �� ȣ��
	virtual void			OnFinalized();		///< �������� �� ȣ��

	void Clear();
	// �ŷ�Ʈ ��� ���� ------------------
	CCommandResultTable UseTalent(GTalentInfo* pTalentInfo, TALENT_TARGET_INFO Target, bool bCheckEnabled, bool bGainStress=true);
	CCommandResultTable IsUsableTalent(GTalentInfo* pTalentInfo);
	// �ŷ�Ʈ�� ��� (�ŷ�Ʈ������ cancelable�Ӽ��� ���� ��쿡�� �ش�)
	void CancelTalent(bool bPostCommand=true);
	// �ŷ�Ʈ�� ������ ���
	void CancelTalentForce(bool bPostCommand=true);
	void AddTargetHitter(GTalentHitter* pNewHitter);
	vector<GTalentHitter*> GetTargetHitter( TARGEHITTER_TYPE nType );

	void ActTalent(TALENT_TARGET_INFO Target);
	void DeleteCurTalent();

	void SetInvincible(float fDurationTime)				{ m_Invincible.DoInvincible(fDurationTime); }
	bool IsUsingMovingTalent();
	bool IsUsingNotCancelableTalent();
	bool IsUsingTalent();
	int GetUsingTalentID();
	bool IsNowInvincibleTime();
	bool IsNowSuperarmorTime();
	bool IsNowAvoidTime();

	// Ư�� ��Ŀ���� �ɷ��ִ��� ����
	bool HasFocus(TALENT_FOCUS_TYPE nFocus);
	// ��Ŀ���� �ϳ��� �ɷ��ִ��� ����
	bool HasFocus();
	// ���� ���� ���� ��ȯ
	int GetBerserkBubble();

	// ���� ���� ����


	// for Test
	GTalent* GetTalent()		{ return m_pTalent; }

	bool CheckTalentRange(GEntityActor* pTargetActor, int nTalentID);

	float GetTalentElapsedTime();
	float GetTalentActElapsedTime();
	
};


#endif // _GMODULE_COMBATSTATUS_H