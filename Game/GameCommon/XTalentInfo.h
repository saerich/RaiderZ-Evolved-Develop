#pragma once

#include "CSTalentInfo.h"
#include "MTypes.h"
#include "XTalentInfoEnum.h"
#include "XTalentInfoDef.h"
#include "CSTalentInfoHelper.h"
#include "XTalentEffectInfo.h"




class XTalentInfoMgr;

class XTalentInfo : public CSTalentInfo
{
private:
	friend class XTalentInfoMgr;
	using CSTalentInfo::m_szName;		// ���ڿ����̺� �����ϵ��� ���� ���� �����ϴ�.
	using CSTalentInfo::m_szDesc;		// ���ڿ����̺� �����ϵ��� ���� ���� �����ϴ�.

	// ���� ������ xml���� �������� �ʰ�, ���� �����ϴ� �͵��̴�.
	bool					m_bExistCastingFX;
	bool					m_bExistUseFX;

	XTalentEffectDataMgr	m_EffectDataMgr;

public:
	bool					IsExistcastingFX()		{ return m_bExistCastingFX; }
	bool					IsExistUseFX()			{ return m_bExistUseFX; }
	bool					isNestedSpell()			{ return (m_nSkillType == ST_MAGIC); }

	CSProjectileInfo *		GetProjectileInfo();

	vec3					GetSwordHitDir(float fAniTime, tstring strMeshName);
	vec3					GetSwordHitPos(float fAniTime, tstring strMeshName);

	bool					IsPortalTalent();

	const TCHAR*			GetName(void);
	const TCHAR*			GetDesc(void);

public:

	// Sound --------------------------
	tstring					m_szTargetSound;		// Ÿ�� ����(Not Used)
	tstring					m_szTargetLoopingSound;	// Ÿ�� �����Ǵ� ����(Not Used)

	// ����
	MDIRECTION				m_nAttackDirection;				// ���� ���� - MF_BEATEN�� �����Ǿ� ���ȴ�. (Ŭ��)

	bool					m_bFreezeFrame;					// ������ ������ ���� ����

	bool					m_bUIIndividual;				//UI�� ��ø�ؼ� �������� Ż��Ʈ�̴�.

	tstring					m_strGrappledAni;				// ������ ���� �÷��̾� �ִϸ��̼�
	tstring					m_strGrappledBone;				// ������ ���� �÷��̾� ��

	bool					m_bShoveWhileMoving;			// �б� ����, �浹�� ������ �б� ���� : TRUE �б�, FALSE ���ϱ�(Ŭ��)
	TALENT_SHOVE_TYPE		m_eTalentShoveType;				// �ŷ�Ʈ �̵� Ÿ��, ����, �մ�, ����

	float					m_fHitBloodScale;

	XTalentInfo() : CSTalentInfo()
	{
		m_bExistCastingFX = false;
		m_bExistUseFX = false;
		
		m_nAttackDirection = MDIR_FRONT;
		m_bFreezeFrame = false;

		m_bUIIndividual			= false;

		m_bShoveWhileMoving = true;
		m_eTalentShoveType = TST_STOP;

		m_fHitBloodScale = 1.0f;
	}

	virtual void CopyAll(CSTalentInfo* pTalentInfo) override;

	XTalentEffectDataMgr* GetEffectDataMgr() { return &m_EffectDataMgr; }	
};


