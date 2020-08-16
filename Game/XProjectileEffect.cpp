#include "stdafx.h"
#include "XProjectileEffect.h"
#include "XProjectile.h"
#include "XTalentEffectInfo.h"
#include "XEffectInvoker.h"
#include "XRealSound.h"

//#include "REffectInstance.h"
#include "REffectSceneNode.h"

XProjectileEffect::XProjectileEffect() : m_pTalentEffectInfo(NULL)/*, m_pTalentInfo(NULL)*/, m_pOwner(NULL), m_bGround(false), m_bTrace(false), m_fTraceLength(0.0f), XTalentEffect()
{
	m_EffectStorage.clear();

//	m_pChannel = NULL;			<<<rsound3
}

XProjectileEffect::~XProjectileEffect()
{
	Destroy();
}

// TODO : ���� ������ �÷��� ���۰� �� ������ �̵���Ű�鼭 
//		: ä�ο� set3DAttributes ���� ��� ���� ���ָ� �ȴ�.
void XProjectileEffect::Create( XObject* pProjectile, XTalentInfo* pInfo, CSProjectileInfo* pProjectileInfo )
{
	if (global.emgr == NULL || pProjectileInfo == NULL) return;

	m_pOwner			= pProjectile;

	//CreateTrace(vPos);
	//XEffectSinglyInvoker effectInvoker;
	//effectInvoker.Invoke(pProjectileInfo->m_strEffectName, (vec3)vec3::ZERO, (vec3)vec3::AXISY, (vec3)vec3::AXISZ, &m_EffectStorage);
	XEffectInvoker effectInvoker;
	effectInvoker.Invoke(pProjectile, pProjectileInfo->m_strEffectName, wstring(), wstring(), &m_EffectStorage);

	// �߻� ����Ʈ
	XEffectSinglyInvoker fireEffectInvoker;
	fireEffectInvoker.Invoke(pProjectileInfo->m_strFireEffectName, m_pOwner->GetPosition(), m_pOwner->GetDirection(), (vec3)vec3::AXISZ);
}

void XProjectileEffect::Destroy()
{
	// �߻�ü ����Ʈ ����
	vector<EFFECT_RESULT_DATA>::iterator itProjectileEffect = m_EffectStorage.begin();
	while(itProjectileEffect != m_EffectStorage.end())
	{
		if((*itProjectileEffect).type == CET_MODEL) 
		{
			global.emgr->DeleteXEffect((*itProjectileEffect).uidEffect);
		}
		else if((*itProjectileEffect).type == CET_SOUND)
		{
			if ( global.sound)
				global.sound->StopDynamic3D( (*itProjectileEffect).uidEffect);
		}

		itProjectileEffect++;
	}

	m_EffectStorage.clear();

	if (m_bTrace)
		m_EffectTrace.Destroy();
}

void XProjectileEffect::Update( vec3& pos, vec3& dir )
{
	if(m_pOwner == NULL)
		return;

	//vector<EFFECT_RESULT_DATA>::iterator itProjectileEffect = m_EffectStorage.begin();
	//while(itProjectileEffect != m_EffectStorage.end())
	//{
	//	// �߻�ü ����Ʈ ������Ʈ
	//	// ����� SoundMgr���� �ڵ����� ���ش�.
	//	if((*itProjectileEffect).type == CET_MODEL)
	//	{
	//		MMatrix matResult;
	//		vec3 vUp = vec3::AXISZ;
	//		if(global.emgr->SetTransform((*itProjectileEffect).uidEffect, pos, -dir, vUp, matResult, (*itProjectileEffect).bTerrainAlign, (*itProjectileEffect).bTerrainPos) == false)
	//		{
	//			// �߻�ü ����Ʈ�� �������� ������ �����.
	//			itProjectileEffect = m_EffectStorage.erase(itProjectileEffect);
	//			continue;
	//		}
	//	}

	//	itProjectileEffect++;
	//}

	// Trace ������
	if(m_bTrace)
	{
		m_EffectTrace.Update(pos);
	}
}

void XProjectileEffect::CreateGroundEffect( vec3& vPos, CSProjectileInfo* pProjectileInfo )
{
	// ���� �ǰ��϶�
	if(pProjectileInfo)
	{
		XEffectSinglyInvoker effectInvoker;
		vec3 vUp = vec3::AXISZ;
		effectInvoker.Invoke(pProjectileInfo->m_strHitGroundEffectName, vPos, m_pOwner->GetDirection(), vUp);
	}
}


void XProjectileEffect::RenderTrace()
{
	if(m_bTrace)
		m_EffectTrace.Render();

}

void XProjectileEffect::CreateTrace( vec3& vStartPos )
{
	m_bTrace = true;
	m_EffectTrace.Create(vStartPos, m_fTraceLength);
}

void XProjectileEffect::CreateHitDefenceEffect( XObject* pObj, wstring strProjectileColEffectName, vec3 vPos, vec3 vDir )
{
	EFFECT_POST_TRANSFORM postData;
	postData.matWorldMatrix.SetLocalMatrix(vPos, vDir, vec3::AXISZ);

	vector<EFFECT_RESULT_DATA> vecDefenceEffectResult;

	if(pObj)
	{
		XEffectInvoker effectInvoker;
		effectInvoker.Invoke(pObj, strProjectileColEffectName, wstring(), wstring(), &vecDefenceEffectResult, m_pOwner, &postData);
	}
	else 
	{
		XEffectSinglyInvoker effectInvoker;
		effectInvoker.Invoke(strProjectileColEffectName, vPos, vDir, vec3(vec3::AXISZ), &vecDefenceEffectResult);
	}
}

wstring XProjectileEffect::GetProjectileColEffect( CSProjectileInfo* pProjectileInfo, ProjectileColJudgmentType eType )
{
	if(pProjectileInfo == NULL)
		return L"";

	// ���� ����Ʈ �̸� ��������
	if(eType == PCJT_HIT)
		return pProjectileInfo->m_strAttackEffectName;
	else if(eType == PCJT_DEFENCE)
		return pProjectileInfo->m_strDefenceEffectName;

	return L"";
}
