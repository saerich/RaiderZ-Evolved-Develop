#ifndef _TPROJECTILE_H
#define _TPROJECTILE_H

#pragma once

struct EFFECT_RESULT_DATA;

class TProjectile
{
private:
	TCharacter*		m_pActor;
	CSProjectileInfo*	m_pProjectileInfo;
	CSTalentInfo*	m_pTalentInfo;

	vec3			m_vPos;
	vec3			m_vDir;
	vec3			m_vVelocity;
	vec3			m_vOldPos;

	vec3			m_vStartPos;
	vec3			m_vTargetPos;
	vec3			m_vCalParabola;						// ������ ���� ��ǥ

	vector<EFFECT_RESULT_DATA>	m_EffectDelStorage;

	float			m_fElapsedTime;						// �ð�
	float			m_fAutoDelTime;						// �ڵ� �Ҹ� �ð�
	float			m_fEstimateTime;					// ���߱��� ���� �ð� ����

	bool			m_bCol;

private:
	vec3			GetProjectileStartPos();

	void			SetTargetByProjectileType();

	void			UpdateMoving(float fDelta);
	bool			UpdateCol(vec3& old_pos, vec3& pos);
	void			UpdateEffect();
	
	// �̵� ����
	void			Move_Straight(float fDelta, vec3 * vTargetPos = NULL);		// ����(�߷� ���� ����)
	void			Move_Curve(float fDelta, vec3 * vTargetPos = NULL);			// ����(��� ����)
	void			Move_Parabola(float fDelat, vec3 * vTargetPos = NULL);		// ������

	bool			CheckGPS();

	void			HitEffect();

public:
	TProjectile();
	virtual ~TProjectile();

	bool			LaunchProjectile(CSProjectileInfo& csProjectilInfo, TCharacter* pActor, CSTalentInfo* pInfo);
	void			Update(float fDelta);
	void			DelProjectile();

	bool			IsCol() { return m_bCol; }
};


#endif