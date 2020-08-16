#include "stdafx.h"
#include "TProjectile.h"
#include "TEffectCharacterToolInvoker.h"
#include "TEffectCharacterToolSinglyInvoker.h"

vec3 MoveSmoothInTime(vec3 vCurrDir, vec3& vOutDir, vec3 vCurrentPos, vec3 vTargetPos, float fTickTime, float fStartDist, float& fRemainTime)
{
	// ���� ȸ��
	vec3 vDir = vTargetPos - vCurrentPos;
	float dist = vDir.Length();

	float r = (1 - dist / fStartDist);
	if (r < 0.1) r = 0.1f;
	if (r > 1) r = 1.0f;

	vCurrDir.Normalize();
	vDir.Normalize();
	vOutDir = vCurrDir * (1 - r) + vDir * r;
	vOutDir.Normalize();

	if (fRemainTime == 0.0f) fRemainTime = 0.1f;
	float fSpeed = dist * fTickTime / fRemainTime;

	// �̵�
	vec3 vPos = vCurrentPos + (vOutDir * fSpeed);

	fRemainTime -= fTickTime;

	return vPos;
}

vec3 MoveParabolaInTime(vec3 vStartPos, vec3 vTargetPos, vec3 vCurrentPos, vec3& vCalParabola, float fMaxHeight, float fTickTime, float& fRemainTime)
{
	// ���� ��ġ ���ϱ�
	vec3 vDir = vTargetPos - vCalParabola;
	float dist = vDir.Length();
	vDir.Normalize();

	if (fRemainTime <= 0.0f) 
	{
		return vTargetPos;
	}

	float fSpeed = dist * fTickTime / fRemainTime;
	vec3 vPos = vCalParabola + (vDir * fSpeed);
	vCalParabola = vPos;

	// ������ ��ġ ���ϱ�
	float fFullLen = (vTargetPos - vStartPos).Length();
	float fCurLen = (vCalParabola - vStartPos).Length();
	float fAngle = (180.0f * fCurLen) / fFullLen;

	float fDegree = DegreeToRadian(fAngle);
	float fTestSin = sin(DegreeToRadian(fAngle));
	float fTestZ = vPos.z;
	vPos.z += sin(DegreeToRadian(fAngle)) * fMaxHeight;
	//mlog("�׽�Ʈ %f - %f(%f) --> %f - %f\n", fDegree, fTestSin, fTestSin * fMaxHeight, fTestZ, vPos.z);

	fRemainTime -= fTickTime;

	return vPos;
}
//////////////////////////////////////////////////////////////////////////

TProjectile::TProjectile()
{
	m_fElapsedTime	= 0.0f;
	m_fAutoDelTime	= 0.0f;
	m_fEstimateTime	= 0.0f;

	m_bCol			= false;
}

TProjectile::~TProjectile()
{
	DelProjectile();
}

bool TProjectile::LaunchProjectile( CSProjectileInfo& csProjectilInfo, TCharacter* pActor, CSTalentInfo* pInfo )
{
	if(pActor == NULL ||
		pInfo == NULL)
		return false;

	m_pActor = pActor;
	m_pTalentInfo = pInfo;
	m_pProjectileInfo = &csProjectilInfo;

	// �߻� ��ġ ã��
	m_vStartPos	= GetProjectileStartPos();
	m_vPos		= m_vStartPos;
	m_vCalParabola = m_vStartPos;
	m_vOldPos	= m_vStartPos;
	m_vDir		= -m_pActor->GetDirection();
	m_vVelocity	= m_vDir * m_pProjectileInfo->m_fSpeed;

	// Ÿ�� ����
	// �⺻�� ���ۿ��� ���� �Ÿ������̴�.
	m_vTargetPos = m_vStartPos + (m_vDir * 1000.0f);
	SetTargetByProjectileType();

	// �߻� ����Ʈ ����
	TEffectCharacterToolSinglyInvoker fireInvoker;
	fireInvoker.Invoke(m_pProjectileInfo->m_strFireEffectName, m_vStartPos, m_vDir, vec3(vec3::AXISZ));

	// �߻�ü ����Ʈ ����
	TEffectCharacterToolInvoker projectileInvoker;
	projectileInvoker.Invoke(m_pActor, m_pProjectileInfo->m_strEffectName, string(""), string(""), &m_EffectDelStorage);
	UpdateEffect();

	return true;
}

void TProjectile::DelProjectile()
{
	vector<EFFECT_RESULT_DATA>::iterator itProjectileEffect = m_EffectDelStorage.begin();
	while(itProjectileEffect != m_EffectDelStorage.end())
	{
		// �߻�ü ����Ʈ ������Ʈ
		if((*itProjectileEffect).type == CET_MODEL)
		{
			g_pMainApp->m_pEffectManager->DeleteTEffect((*itProjectileEffect).uidEffect);
		}

		itProjectileEffect++;
	}

	m_EffectDelStorage.clear();
}

vec3 TProjectile::GetProjectileStartPos()
{
	vec3 vStartPos;

	m_pActor->GetWorldTransform().TransformVect(m_pProjectileInfo->m_vLocalStartPos, vStartPos);

	return vStartPos;
}

void TProjectile::Update( float fDelta )
{
	m_fElapsedTime += fDelta;
	
	// �̵�
	UpdateMoving(fDelta);

	// �浹 üũ
	if(UpdateCol(m_vOldPos, m_vPos))
	{
		m_bCol = true;
		HitEffect();
		DelProjectile();
	}

	// ����Ʈ ������Ʈ
	UpdateEffect();
}

void TProjectile::UpdateEffect()
{
	vector<EFFECT_RESULT_DATA>::iterator itProjectileEffect = m_EffectDelStorage.begin();
	while(itProjectileEffect != m_EffectDelStorage.end())
	{
		// �߻�ü ����Ʈ ������Ʈ
		if((*itProjectileEffect).type == CET_MODEL)
		{
			MMatrix matResult;
			if(g_pMainApp->m_pEffectManager->SetTransform((*itProjectileEffect).uidEffect, m_vPos, -m_vDir, matResult, (*itProjectileEffect).bTerrainAlign, (*itProjectileEffect).bTerrainPos) == false)
			{
				// �߻�ü ����Ʈ�� �������� ������ �����.
				itProjectileEffect = m_EffectDelStorage.erase(itProjectileEffect);
				continue;
			}
		}

		itProjectileEffect++;
	}
}

void TProjectile::SetTargetByProjectileType()
{
	if(m_pProjectileInfo->m_nType == TPT_MISSILE)
	{

	}

	if (m_pProjectileInfo->m_nType == TPT_HITCAPSULE_GUIDED)
	{
		// ����ĸ����ġ ��������
		int nSegmentIndex = m_pProjectileInfo->m_nSegmentIndexForHitCapsuleType;
		int nCapsuleIndex = m_pProjectileInfo->m_nCapsuleIndexForHitCapsuleType;

		if(nSegmentIndex < 0 ||
			nCapsuleIndex < 0)
			return;

		if((int)m_pTalentInfo->m_HitInfo.m_vSegments.size() > nSegmentIndex)
		{
			if((int)m_pTalentInfo->m_HitInfo.m_vSegments[nSegmentIndex].m_vCapsules.size() > nCapsuleIndex)
			{
				MMatrix matWorld;
				matWorld.SetLocalMatrix(vec3::ZERO, m_pActor->GetDirection(), vec3::AXISZ);
				MCapsule hitCapsule = m_pTalentInfo->m_HitInfo.m_vSegments[nSegmentIndex].m_vCapsules[nCapsuleIndex].Multiply(matWorld);

				m_vTargetPos = hitCapsule.GetCenter();

				m_vDir = m_vTargetPos - m_vStartPos;
				m_vDir.Normalize();

				m_vVelocity = m_vDir * m_pProjectileInfo->m_fSpeed;
			}
		}
	}

	if (m_pProjectileInfo->m_nType == TPT_MISSILE_GUIDED ||	m_pProjectileInfo->m_nType == TPT_HITCAPSULE_GUIDED)
	{
		m_fEstimateTime = 5.0f;

		float len = (m_vStartPos - m_vTargetPos).Length();
		float speed = m_pProjectileInfo->m_fSpeed;

		m_fEstimateTime = (len / speed);

		// �߻�ü Ÿ���� ����, ����ĸ���̸�...
		// ����� Ÿ���� Curve�� ������ �ٲ۴�.
		if(m_pProjectileInfo->m_nVisualType == TPVT_CURVE)
		{
			vec3 vCurveDir = m_vDir;
			vCurveDir.z = 0.0f;
			vCurveDir.Normalize();

			vec3 vRight = CrossProduct(vCurveDir, UP_VECTOR);

			// ����
			int nLeftRight = MMath::RandomNumber(0, 1);
			//float fDegree = MMath::RandomNumber(30.0f, 45.0f);
			float fDegree = 45.0f;
			if(nLeftRight == 1)
			{
				fDegree *= -1.0f;
			}

			// ���� ���ϱ�
			mat4 mat = mat4::IDENTITY;
			mat.SetRotationMatrixAxis(UP_VECTOR, DegreeToRadian(fDegree));
			vCurveDir = vCurveDir * mat;
			vCurveDir.Normalize();

			m_vDir = vCurveDir;
			m_vVelocity = m_vDir * m_pProjectileInfo->m_fSpeed;
		}
	}
}

void TProjectile::UpdateMoving(float fDelta)
{
	m_vOldPos = m_vPos;

	switch(m_pProjectileInfo->m_nType)
	{
	case TPT_MISSILE:
		{
			// Ÿ�� ��ġ�� �������� �߻�
			switch(m_pProjectileInfo->m_nVisualType)
			{
			case TPVT_STRAIGHT:
			case TPVT_CURVE:
				Move_Straight(fDelta);
				break;
			}
			break;
		}
	case TPT_MISSILE_GUIDED:
	case TPT_HITCAPSULE_GUIDED:
		{
			switch(m_pProjectileInfo->m_nVisualType)
			{
			case TPVT_STRAIGHT:
				Move_Straight(fDelta, &m_vTargetPos);
				break;
			case TPVT_CURVE:
				Move_Curve(fDelta, &m_vTargetPos);
				break;
			case TPVT_PARABOLA:
				Move_Parabola(fDelta, &m_vTargetPos);
				break;
			}
			break;
		}
	}
}

void TProjectile::Move_Straight( float fDelta, vec3 * vTargetPos /*= NULL*/ )
{
	// ���� �(�߷� ����)
	vec3 vStartPos	= m_vStartPos;
	vec3 vVelocity	= m_vVelocity;
	float fTime		= m_fElapsedTime;

	if(vTargetPos && (*vTargetPos) != vec3::ZERO)
	{
		// ��ǥ�� �ִٸ�...
		// ���󰣴�.
		vStartPos	= m_vPos;
		vVelocity	= ((*vTargetPos) - m_vPos).Normalize() * m_pProjectileInfo->m_fSpeed;
		fTime		= fDelta;
	}

	m_vPos = CSTalentInfoHelper::MoveStraight(vStartPos, vVelocity, fTime);
}

void TProjectile::Move_Curve( float fDelta, vec3 * vTargetPos /*= NULL*/ )
{
	// ����
	// ��� ����
	if(vTargetPos && (*vTargetPos) != vec3::ZERO)
	{
		vec3 vDir;
		vec3 ndir;
		float fD = (m_vStartPos - (*vTargetPos)).Length();
		m_vPos = MoveSmoothInTime(m_vDir, ndir, m_vPos, (*vTargetPos), fDelta, fD, m_fEstimateTime);
		vDir = ndir;

		m_vDir = vDir;
	}
	else
	{
		// Ÿ���� ���ٸ� ����...
		Move_Straight(fDelta);
	}
}

void TProjectile::Move_Parabola( float fDelat, vec3 * vTargetPos /*= NULL*/ )
{
	if(vTargetPos && (*vTargetPos) != vec3::ZERO)
	{
		m_vPos = MoveParabolaInTime(m_vStartPos, (*vTargetPos), m_vPos, m_vCalParabola, 500.0f, fDelat, m_fEstimateTime);
	}
	else
	{
		Move_Straight(fDelat);
	}
}

bool TProjectile::UpdateCol( vec3& old_pos, vec3& pos )
{
	// �浹 �˻�
	bool bCol = false;

	switch(m_pProjectileInfo->m_nType)
	{
	case TPT_MISSILE:
	case TPT_MISSILE_GUIDED:
		{
			if(CheckGPS())
			{
				return true;
			}

			break;
		}
	case TPT_HITCAPSULE_GUIDED:
		{
			// ����ĸ��
			bool bCheck = true;
			// ���Ͻÿ��� �˻��ص� �ɵ�...
			if(m_pProjectileInfo->m_nVisualType == TPVT_PARABOLA &&
				m_vDir.z > 0.0f)
			{
				bCheck = false;
			}

			if(bCheck && CheckGPS())							// ��ġ üũ
			{
				m_vPos = m_vTargetPos;
				return true;
			}

			break;
		}
	}

	return false;
}

bool TProjectile::CheckGPS()
{
	vec3 vTargetPos = m_vTargetPos;
	if(m_vTargetPos != vec3::ZERO)
	{
		// �������°�?
		vec3 vNoZTargetPos = vTargetPos;
		vec3 vNoZPos = m_vPos;
		vTargetPos.z = vNoZPos.z = 0.0f;

		vec3 vDir = vNoZTargetPos - vNoZPos;
		vDir.Normalize();

		vec3 vNoZDir = m_vDir;
		vNoZDir.z = 0.0f;
		vNoZDir.Normalize();

		float fDot = vNoZDir.DotProduct(vDir);

		if(fDot <= 0.0f)
		{
			return true;
		}
	}

	return false;
}

void TProjectile::HitEffect()
{
	vector<EFFECT_RESULT_DATA> vecHitEffectResult;

	TEffectCharacterToolInvoker projectileInvoker;
	projectileInvoker.Invoke(m_pActor, m_pProjectileInfo->m_strAttackEffectName, string(""), string(""), &vecHitEffectResult);

	// ���� ��ġ�� ����
	for(vector<EFFECT_RESULT_DATA>::iterator it = vecHitEffectResult.begin(); it != vecHitEffectResult.end(); it++)
	{
		if(it->type == CET_MODEL)
		{
			MMatrix matResult;
			g_pMainApp->m_pEffectManager->SetTransform((*it).uidEffect, m_vPos, m_vDir, matResult, it->bTerrainAlign, it->bTerrainPos);
		}
	}
}