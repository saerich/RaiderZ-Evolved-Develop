#pragma once

#include "XDef.h"
#include "XEffect.h"
#include "XEffectTrace.h"
#include "XTalentEffect.h"

#define AUTO_DELETE_TIME			30.0f

class XProjectile;
class XTalentEffectDataMgr;
class XTalentEffectInfo;
enum ProjectileColJudgmentType;

using namespace rs3;

class XProjectileEffect : public XTalentEffect
{
private:
	XTalentEffectDataMgr*		m_pTalentEffectInfo;
	//XTalentInfo*				m_pTalentInfo;
	XObject*					m_pOwner;

	bool						m_bGround;							// ���
	bool						m_bTrace;							// Trace �ѷ�?

	float						m_fTraceLength;						// Trace ����
	XEffectTrace				m_EffectTrace;

	vector<EFFECT_RESULT_DATA>	m_EffectStorage;

//	RealSoundChannel*			m_pChannel;		<<<rsound3
private:

public:
	XProjectileEffect();
	~XProjectileEffect();

	void						Create( XObject* pProjectile, XTalentInfo* pInfo, CSProjectileInfo* pProjectileInfo );
	void						CreateTrace(vec3& vStartPos);
	void						CreateHitDefenceEffect(XObject* pObj, wstring strProjectileColEffectName, vec3 vPos, vec3 vDir);
	void						CreateGroundEffect(vec3& vPos, CSProjectileInfo* pProjectileInfo);

	void						Destroy();

	void						Update(vec3& pos, vec3& dir);

	void						RenderTrace();

	wstring						GetProjectileColEffect(CSProjectileInfo* pProjectileInfo, ProjectileColJudgmentType eType);
};
