#ifndef _TTALENT_EFFECT_VIEWER_H
#define _TTALENT_EFFECT_VIEWER_H

#include <Windows.h>
#include <MMSystem.h>
#include <shlwapi.h>
#include <math.h>
#include <algorithm>

#include "MainApp.h"
#include "XTalentInfo.h"
#include "XTalentEffectInfo.h"

using namespace System;
using namespace System::Windows::Forms;

class TTalentEffectMgr;

#define EFFECTYPE_STRING_CAST_EFFECT						"ĳ��Ʈ"
#define EFFECTYPE_STRING_USE_EFFECT							"���"
#define EFFECTYPE_STRING_ATTACK_EFFECT						"�ǰ�"
#define EFFECTYPE_STRING_MISS_EFFECT						"����"
#define EFFECTYPE_STRING_SHOCK_EFFECT						"���"
#define EFFECTYPE_STRING_DURATION_EFFECT					"����"
#define EFFECTYPE_STRING_DEFENSE_EFFECT						"���"
#define EFFECTYPE_STRING_CAST_END_EFFECT					"ĳ��Ʈ ����"
#define EFFECTYPE_STRING_NOT								"���ùٶ�"

#define IMPACTPOSITION_STRING_HIT_POS						"���� ��ġ"
#define IMPACTPOSITION_STRING_BONE_POS						"�� ��ġ"
#define IMPACTPOSITION_STRING_SHOCK_GROUND_POS				"���� ��ġ(����)"
#define IMPACTPOSITION_STRING_TALENT_HIT_CAPSUL_POS			"���� ��ġ(���)"
#define IMPACTPOSITION_STRING_MODEL_POS						"�� ��ġ"

#define EFFECT_DIRECTION_NONE								"���⼺ ����"
#define EFFECT_DIRECTION_FRONT_USER							"����Ʈ����� ����"
#define EFFECT_DIRECTION_SWORD_DIR							"Į ����"

class TTalentEffectViewer
{
private:
	TTalentEffectMgr *	m_pEffectMgr;

	TALENT_EFFECT_TYPE	m_eTalentEffectType;


private:
	PropertyGrid^		GetEffectViewerPropertGrid();

	// ���ڿ� �Է�
	void				SetTextEffectType(string& strBuffer, TALENT_EFFECT_TYPE nEffectType);
	void				SetTextImpactPositionList(string& strBuffer, IMPACTPOS nImapctPos);

	TALENT_EFFECT_TYPE	GetEffectType(string& strBuffer);
	IMPACTPOS			GetEffectImpactPositionList(string& strBuffer);

	// ĳ��Ʈ
	bool				SetTalentEffectCastToProperty(XTalentEffectInfo * pTalentEffectInfo);
	bool				SetPropertyToTalentEffectCast(XTalentEffectInfo * pTalentEffectInfo);
	// ���
	bool				SetTalentEffectUseToProperty(XTalentEffectInfo * pTalentEffectInfo);
	bool				SetPropertyToTalentEffectUse(XTalentEffectInfo * pTalentEffectInfo);
	// �ǰ�
	bool				SetTalentEffectAttackToProperty(XTalentEffectInfo * pTalentEffectInfo);
	bool				SetPropertyToTalentEffectAttack(XTalentEffectInfo * pTalentEffectInfo);
	// ����
	bool				SetTalentEffectMissToProperty(XTalentEffectInfo * pTalentEffectInfo);
	bool				SetPropertyToTalentEffectMiss(XTalentEffectInfo * pTalentEffectInfo);
	// ���
	bool				SetTalentEffectShockToProperty(XTalentEffectInfo * pTalentEffectInfo);
	bool				SetPropertyToTalentEffectShock(XTalentEffectInfo * pTalentEffectInfo);
	// ����
	bool				SetTalentEffectDurationToProperty(XTalentEffectInfo * pTalentEffectInfo);
	bool				SetPropertyToTalentEffectDuration(XTalentEffectInfo * pTalentEffectInfo);
	// ���
	bool				SetTalentEffectDefenseToProperty(XTalentEffectInfo * pTalentEffectInfo);
	bool				SetPropertyToTalentEffectDefense(XTalentEffectInfo * pTalentEffectInfo);
	// ĳ��Ʈ ����
	bool				SetTalentEffectCastEndToProperty(XTalentEffectInfo * pTalentEffectInfo);
	bool				SetPropertyToTalentEffectCastEnd(XTalentEffectInfo * pTalentEffectInfo);


	bool	(TTalentEffectViewer::*TalentEffectFuncToProperty[8])(XTalentEffectInfo * pTalentEffectInfo);
	bool	(TTalentEffectViewer::*TalentEffectFuncToTalentEffectInfo[8])(XTalentEffectInfo * pTalentEffectInfo);
	//------------------------------------------------------------------------

public:
	TTalentEffectViewer();
	virtual ~TTalentEffectViewer() {}

	void				SetEffectManager(TTalentEffectMgr * pMgr)		{ m_pEffectMgr = pMgr; }

	TALENT_EFFECT_TYPE	GetCurrentEffectType(){ return m_eTalentEffectType; }

	//------------------------------------------------------------------------
	// �ŷ�Ʈ ����Ʈ �Ӽ�â
	void				SetEffectInfoFromPropertyGrid(XTalentEffectInfo* pEffectInfo);
	void				SetPropertyGridFromEffectInfo(XTalentEffectInfo* pEffectInfo);

	//------------------------------------------------------------------------
	// ��Ʈ ����Ʈ ��ġ ���� �Ӽ�â
	void				SetHitEffectPosDirInfoFromPropertyGrid(XTalentHitEffectPosDir* pHitEffectPosDirInfo);
	void				SetPropertyGridFromHitEffectPosDirInfo(XTalentHitEffectPosDir* pHitEffectPosDirInfo);
};


#endif