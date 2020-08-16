#ifndef _XACTION_DIE_H
#define _XACTION_DIE_H

enum ACTION_DIE_STATE
{
	ADS_NORMAL,
	ADS_GAMESTART,

	// ��� ���� ����
	ADS_KNOCKBACK,
	ADS_KNOCKDOWN,
	ADS_THROWUP,
	ADS_GRAPPLED,
	ADS_UPPERED,
	ADS_MF_NO_ANIMATION,

	ADS_MAX
};

class XActionDie
{
private:
	XObject *			m_pOwner;

	bool				m_bStartDie;					// �״� �ִϸ��̼� ����
	bool				m_bMFDie;
	bool				m_bDieAniEnd;

	vec3				m_vDieStartUp;					// ���� djq
	vec3				m_vDieStartDir;					// ���� ����
	vec3				m_vDieTargetUp;					// ���� ��
	vec3				m_vDieTargetDir;				// ���� ����

private:
	bool				CalTerrainUpDir(vec3& pos, vec3& dir, vec3& outUp, vec3& outDir);

	void				StartDie(bool bAni);
	void				DieAniEnd();

	void				StartTerrainUpDir();
	void				UpdateTerrainUpDir(float fDelat);

public:
	XActionDie();
	virtual ~XActionDie();

	void				Init(XObject* pObject);
	void				OnStartDie(ACTION_DIE_STATE eState);
	void				OnStartDie(MF_STATE eMFState, bool bAniSet);
	void				OnEndDie();
	void				Update(float fDelta);

	void				DoneDie();

	bool				IsDieAniEnd() { return m_bDieAniEnd; }
	bool				IsDoingActionDie();
};

#endif // _XACTION_DIE_H