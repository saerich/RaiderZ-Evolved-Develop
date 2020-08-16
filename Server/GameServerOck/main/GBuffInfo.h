#pragma once

#include "CSBuffInfo.h"
#include "CSTalentInfoEnum.h"


class GBuffInfoMgr;

class GBuffInfo : public CSBuffInfo, public MTestMemPool<GBuffInfo>
{
private:
	friend class GBuffInfoMgr;
public:
	GBuffInfo(); 
	// ������͸� ��ȭ��Ű���� ����
	bool			IsModified_MF() const;
	bool			IsExistModValue() const	{ return m_bExistModValue; }
	bool			IsFocusBuff() const		{ return m_bFocusBuff; }
	bool			HasUseTalent() const	{ return m_nTalentID != INVALID_TALENT_ID; }

	bool 			IsInvincibility() const;
	bool 			IsInvisibilityToNPC() const;
	bool 			IsInvisibility() const;
	TALENT_CATEGORY IsDisableTalent() const;
	bool			IsMesmerize() const;
	bool			IsCancelable() const;
	bool			IsDieBuff() const;
	bool			IsDespawnOnceBuff() const;
	bool			IsDespawnEternalBuff() const;
	bool			IsAbsoluteGuard() const;
	bool			IsInstantEffect() const;
	bool			IsDisableQuestPVPDrop() const;
	bool			HasHealEffect() const; 
	
	void			Cooking();

	float			GetInvisibilityPower();				

private:
	void			InitSuffix();
public:
	// ���� ���� �ð�
	float					m_fDuration;	
	// ���� �ֱ�ȿ�� �ð�
	float					m_fPeriod;	
	// ������̾�� �ִ��� ����
	bool					m_bExistModValue;		
	// ���� ȿ���� ���� �� �ִ� ����
	CSTalentConditionInfo	m_Condition;
	// ���� ȿ���� ���� �� �ִ� ���
	CSEffectInfo			m_EffectInfo;
	// ������� �׷� ���� ȿ��
	MOTION_FACTOR_GROUP		m_MotionFactorGroup;	
	// ƽ�� �������(HP), 0�̸� ����������
	int						m_nHPCost;
	// ƽ�� �������(EN), 0�̸� ����������
	int						m_nENCost;
	// ƽ�� �������(STA), 0�̸� ����������
	int						m_nSTACost;
	// ��Ŀ�� �������� ����
	bool					m_bFocusBuff;
	// ȿ�� ���� �� ����� �ŷ�Ʈ
	int						m_nTalentID;
	// ����Ʈ ��ȭ��
	int						m_nHateAdj;
	// ����Ʈ ����ġ
	float					m_fHateFactor;
	// DB�� �������� ����
	bool					m_bSaveToDB;
	// ���� ���� ������Ʈ�� ��Ŷ ���ۿ���
	bool					m_bUpdateRoute;
};

/// ��ų ���� ������
class GBuffInfoMgr : public CSBuffInfoMgr, public MTestMemPool<GBuffInfoMgr>
{
private:
	void					ParseBuff(MXmlElement* pElement, MXml* pXml);
	void					ParseIsFocusBuff(GBuffInfo* pBuffInfo);

public:
	virtual ~GBuffInfoMgr();

	bool					Load(const wchar_t* szFileName);
	void					Clear();
	GBuffInfo*				Get(int nID);
	bool					Exist(int nID);
	void					Insert(GBuffInfo* pBuffInfo );
};	
