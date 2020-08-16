#include "stdafx.h"
#include "XProjectile.h"
#include "XGame.h"
#include "XModuleCollision.h"
#include "XModuleEntity.h"
#include "XModuleTalent.h"
#include "XModuleActorControl.h"
#include "XModuleEffect.h"
#include "XWorld.h"
#include "XTalentInfo.h"

MImplementRTTI(XProjectile, XObject);

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
// class XProjectile
XProjectile::XProjectile(MUID uid)
: XObject(uid)
, m_pProjectileInfo(NULL)
{
	m_vVelocity			= vec3::ZERO;
	m_vPos				= vec3::ZERO;
	m_vOldPos			= vec3::ZERO;
	m_vDir				= vec3::ZERO;
	m_vTargetPos		= vec3::ZERO;

	m_vStartPos			= vec3::ZERO;
	m_vCalParabola		= vec3::ZERO;

	m_fElapsedTime		= 0.0f;
	m_fAutoDelTime		= AUTO_DELETE_TIME;

	m_bColCheck			= true;				// �浹 ó���� �⺻
	m_bDelMe			= false;
	m_bWaitColJudgment	= false;

	m_pModuleEntity		= new XModuleEntity(this);

	m_nTypeID			= ETID_PROJECTILE;
	m_eAttackerTypeID	= ETID_ENTITY;

	m_uidTarget			= MUID::ZERO;
	m_nTargetGroupID	= -1;
	m_nTargetCapsuleID	= -1;

	m_pColObject		= NULL;
	m_vCoPos			= vec3::ZERO;
	m_vColDir			= vec3::ZERO;
	m_bColJudgment		= false;
	m_ColJudgmentType	= PCJT_NONE;
}

XProjectile::~XProjectile()
{
	Destroy();

	SAFE_DELETE(m_pModuleEntity);

	m_vecColJedgment.clear();
}

void XProjectile::RegisterModules()
{
	XObject::RegisterModules();

	// Initialize ����
	if(m_pModuleEntity)
	{
		m_pModuleEntity->Initialize();
		m_Modules.AddModule(m_pModuleEntity, true);
	}
}

void XProjectile::UnregisterModules()
{
	// Remove ����..
	if(m_pModuleEntity)
	{
		m_Modules.RemoveModule(m_pModuleEntity);
		m_pModuleEntity->Finalize();
	}

	XObject::UnregisterModules();
}

void XProjectile::Destroy()
{
	m_Effect.Destroy();
}

void XProjectile::OnUpdate(float fDelta)
{ 
	UpdateColJedgment();

	m_fElapsedTime += fDelta;

	UpdateMoving(fDelta);

	// �浹 üũ
	bool bCol = UpdateCol(fDelta);
	if(bCol)
	{
		// �浹�� ���� �Ǵ��� ��ٸ���.
		m_bWaitColJudgment = true;

		CheckColJedgment();

		// �Ǵ��� ��ٸ�����... �ð� ���ٴ� �׳� �����.
		if(CheckTimeOut())
			m_bDelMe = true;
	}

	// �̰� �����ϴ� ���̴�.
	if(m_bDelMe)
		DeleteMe();

	// �浹�ϰų� �����ϴ� ���̸� ����Ʈ�� ���ش�.
	if(bCol || m_bDelMe)
		m_Effect.Destroy();

	m_Effect.Update(m_vPos, m_vDir);
}


void XProjectile::SetMoveData(vec3 vPos)
{
	vec3 vOldPos		= m_vPos;
	vec3 dir			= vPos - vOldPos;
	dir.Normalize();

	// ��� ��ǥ �Է�
	m_vPos				= vPos;
	m_vOldPos			= vOldPos;
	m_vDir				= dir;

	SetPosition(m_vPos);
	SetDirection(m_vDir);
}

//----------------------------------------------------------------------------//
// �̵� ����
void XProjectile::UpdateMoving( float fDelta )
{
	switch(m_pProjectileInfo->m_nType)
	{
	case TPT_MISSILE:
		{
			// ����
			Projectile_Missile(fDelta);
			break;
		}
	case TPT_MISSILE_GUIDED:
		{
			// ����
			Projectile_Guided(fDelta);
			break;
		}
	case TPT_HITCAPSULE_GUIDED:
		{
			// ����ĸ��
			Projectile_HitCapsule(fDelta);
			break;
		}
	}
}

void XProjectile::Projectile_Missile( float fDelta )
{
	// Ÿ�� ��ġ�� �������� �߻�
	switch(m_pProjectileInfo->m_nVisualType)
	{
	case TPVT_STRAIGHT:
	case TPVT_CURVE:
		Move_Straight(fDelta);
		break;
	}
}

void XProjectile::Projectile_Guided( float fDelta )
{
	// Ÿ�ٿ��� ������ ���󰡱�(�߰��� �浹 ó�� ����.)
	// Ÿ���� �־�� �Ѵ�.
	// Ÿ�� üũ
	XObject* pTarget = gg.omgr->Find(m_uidTarget);
	if(pTarget == NULL)
	{
		// �������.
		m_bDelMe = true;
		return;
	}

	// ���� �̵�ó��
	vec3 vTargetPos = GetTargetPos(m_uidTarget, m_nTargetGroupID, m_nTargetCapsuleID);
	switch(m_pProjectileInfo->m_nVisualType)
	{
	case TPVT_STRAIGHT:
		Move_Straight(fDelta, &vTargetPos);
		break;
	case TPVT_CURVE:
		Move_Curve(fDelta, &vTargetPos);
		break;
	case TPVT_PARABOLA:
		Move_Parabola(fDelta, &vTargetPos);
		break;
	}
}

void XProjectile::Projectile_HitCapsule( float fDelta )
{
	// �ŷ�Ʈ ���� ĸ�� ��ġ�� �߻�(�߰��� �浹 ó�� ����.)
	switch(m_pProjectileInfo->m_nVisualType)
	{
	case TPVT_STRAIGHT:
		Move_Straight(fDelta, &m_vTargetPos);
	case TPVT_CURVE:
		Move_Curve(fDelta, &m_vTargetPos);
		break;
	case TPVT_PARABOLA:
		Move_Parabola(fDelta, &m_vTargetPos);
		break;
	}
}

void XProjectile::Move_Straight( float fDelta, vec3 * vTargetPos /*= NULL*/ )
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

	vec3 vPos = CSTalentInfoHelper::MoveStraight(vStartPos, vVelocity, fTime);
	
	SetMoveData(vPos);
}

void XProjectile::Move_Curve(float fDelta, vec3 * vTargetPos /*= NULL*/)
{
	// ����
	// ��� ����
	if(vTargetPos && (*vTargetPos) != vec3::ZERO)
	{
		vec3 vDir;
		vec3 ndir;
		float fD = (m_vStartPos - (*vTargetPos)).Length();
		vec3 vPos = MoveSmoothInTime(m_vDir, ndir, m_vPos, (*vTargetPos), fDelta, fD, m_fEstimateTime);
		vDir = ndir;

		SetMoveData(vPos);
		m_vDir = vDir;
	}
	else
	{
		// Ÿ���� ���ٸ� ����...
		Move_Straight(fDelta);
	}
}

void XProjectile::Move_Parabola( float fDelat, vec3 * vTargetPos /*= NULL*/ )
{
	if(vTargetPos && (*vTargetPos) != vec3::ZERO)
	{
		vec3 vPos = MoveParabolaInTime(m_vStartPos, (*vTargetPos), m_vPos, m_vCalParabola, 500.0f, fDelat, m_fEstimateTime);

		SetMoveData(vPos);
	}
	else
	{
		Move_Straight(fDelat);
	}
}

// �̵� ����--
//----------------------------------------------------------------------------//

bool XProjectile::UpdateCol(float fDelta)
{
	// �浹 �˻�
	bool bCol = false;
	if(m_bColCheck)
	{
		switch(m_pProjectileInfo->m_nType)
		{
		case TPT_MISSILE:
			{
				// ����
				if(CheckRange() ||									// ��Ÿ� üũ
					CheckColObj(m_vOldPos, m_vPos) ||				// ������Ʈ üũ
					CheckColTerrain(m_vOldPos, m_vPos))				// ���� üũ
				{
					bCol = true;
				}
				break;
			}
		case TPT_MISSILE_GUIDED:
			{
				// ����
				if(CheckColObj(m_vOldPos, m_vPos) ||				// ������Ʈ üũ
					CheckGPS())										// ��ġ üũ
				{
					bCol = true;
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
					bCol				= true;
					
					// ��Ʈ ĸ���� ���� ������ ������� �ǰ� ����Ʈ�� ���´�.
					m_bColJudgment		= true;
					m_ColJudgmentType	= PCJT_HIT;
				}

				break;
			}
		}
	}

	return bCol;
}

#define PROJECT_RANGE 3000.0f	// 30����

bool XProjectile::CheckRange()
{
	// ��Ÿ� üũ
	// ��Ÿ��� �Ѿ�� ���� �ʽ��ϴ�.
	// �����ϰ� �浹 ó�� �մϴ�.
	vec3 vMove = m_vPos - m_vStartPos;
	float fRange = vMove.Length();

	if(fRange >= m_pProjectileInfo->m_fRange)
	{
		m_uidTarget = MUID::ZERO;
		return true;
	}

	return false;
}

bool XProjectile::CheckTimeOut()
{
	// Ÿ�� �ƿ� üũ
	if(m_fElapsedTime >= m_fAutoDelTime)
	{
		return true;
	}

	return false;
}

bool XProjectile::CheckGPS()
{
	vec3 vTargetPos = vec3::ZERO;
	// �߻��ڷ� ���� �ǰݿ� ���� ������ �����´�.
	if(m_uidTarget.IsValid())
	{
		XObject* pTarget = gg.omgr->Find(m_uidTarget);

		if(pTarget == NULL)
		{
			// Ÿ���� ���ٸ�... ����
			return true;
		}

		vTargetPos = pTarget->GetPosition();
	}
	else if(m_vTargetPos != vec3::ZERO)
	{
		vTargetPos = m_vTargetPos;
	}
	else
	{
		// ��ǥ������ ���� üũ ���� �ʴ´�.
		return false;
	}

	if(vTargetPos != vec3::ZERO)
	{
		// �������°�?
		vec3 vNoZTargetPos = vTargetPos;
		vec3 vNoZPos = m_vPos;
		vNoZTargetPos.z = vNoZPos.z = 0.0f;
		
		vec3 vDir = vNoZTargetPos - vNoZPos;
		vDir.Normalize();

		vec3 vNoZDir = m_vDir;
		vNoZDir.z = 0.0f;
		vNoZDir.Normalize();

		float fDot = vNoZDir.DotProduct(vDir);

		if(fDot < 0.0f)
		{
			m_pColObject = NULL;
			m_vCoPos = vTargetPos;
			m_vColDir = m_vDir;
			return true;
		}
	}

	return false;
}

bool XProjectile::CheckColTerrain(vec3& old_pos, vec3& pos)
{
	if (old_pos != pos)
	{
		vec3 vOutPos, vOutNormal;

		if(gg.currentgamestate->GetWorld()->PickCollision(old_pos, pos, &vOutPos, &vOutNormal) == true)
		{
			m_Effect.CreateGroundEffect(vOutPos, m_pProjectileInfo);
			SetMoveData(vOutPos);

			// �⺻������ �浹�� �߻�ü�� ������ �ȵȴ�. �������� �����̴�.
			// �׷��Ƿ� �����浹�� �����ǰ� �Ѵ�.
			m_bDelMe = true;
			return true;
		}
	}

	return false;
}

bool XProjectile::CheckColObj(vec3& old_pos, vec3& pos)
{
	// ������Ʈ �浹
	if (old_pos != pos)
	{
		XObject * pColObject	= NULL;
		bool bCol				= false;
		float fDistance			= 0.0f;
		vec3 vOutPos;
		vec3 dir				= pos - old_pos;
		dir.Normalize();


		// Ÿ���� ������ �ִٸ�...
		// Ÿ�ٵ��� BaseColCheck���� ������ ���ڿ� true�� �־��ּ���(����)
		XObject* pTarget = gg.omgr->Find(m_uidTarget);
		if(pTarget)
		{
			// Ÿ�ٿ� ���� �浹 �˻�
			if(CheckColBase(old_pos, pos, pTarget, fDistance, vOutPos, true))
			{
				bCol		= true;
				pColObject	= pTarget;
			}
		}
		else
		{
			// ��� ������Ʈ �浹 �˻�
			float fMinDistance = 99999.0f;
			for (XObjectMap::iterator itor = gg.omgr->BeginItor();itor != gg.omgr->EndItor(); ++itor)
			{
				XObject* pObj = (*itor).second;
				if(pObj->GetUID() == m_uidAttaker ||
					pObj->GetEntityType() == m_eAttackerTypeID ||					// Ÿ������ �ƴ� �߻�ü�� ���� ĳ���ͳ����� �浹ó���� ���Ѵ�.
					IsActorDead(pObj))											// ���� ���� �н�
				{
					continue;
				}

				if(CheckColBase(old_pos, pos, pObj, fDistance, vOutPos) &&
					fDistance < fMinDistance)
				{
					bCol			= true;
					pColObject		= pObj;

					fMinDistance	= fDistance;
				}
			}

			fDistance = fMinDistance;
		}

		// �浹 ������... ����Ʈ��
		if(bCol == true)
		{
			m_pColObject = pColObject;
			m_vCoPos = vOutPos;
			m_vColDir = m_vDir;
			SetMoveData(old_pos);
			return true;
		}
	}

	return false;
}

bool XProjectile::CheckColBase(vec3& old_pos, vec3& pos, XObject * pObject, float & out_Distance, vec3& out_colPos, bool bDirCheck /* = false */)
{
	vec3 tar_center		= GetTargetPos(pObject->GetUID(), m_nTargetGroupID, m_nTargetCapsuleID);
	XModuleCollision* pModuleCollision = pObject->GetModuleCollision();
	if(pModuleCollision == NULL ||
		tar_center == vec3::ZERO)
	{
		return false;
	}

	out_Distance		= 0.0f;
	vec3 vImpactPos		= vec3::ZERO;
	vec3 vImpactDir		= vec3::ZERO;
	vec3 dir			= pos - old_pos;
	vec3 tar_dir		= tar_center - pos;
	float fLen			= dir.Length();
	
	dir.Normalize();
	tar_dir.Normalize();

	// �浹 �غ�
	MCapsule capsuleCollision(old_pos, pos, m_pProjectileInfo->m_fHitRadius);

	// �浹 �˻�
	if(pModuleCollision->ColCapsuleToObject(capsuleCollision, capsuleCollision, pos - old_pos, vImpactPos, vImpactDir, out_Distance))
	{
		out_colPos = vImpactPos;
		return true;
	}

	if(bDirCheck)
	{
		float fDist = dir.DotProduct(tar_dir);
		if(fDist < 0.0f)
		{
			// ������.
			out_Distance	= (tar_center - pos).Length();
			out_colPos		= tar_center;
			return true;
		}
	}

	return false;
}

bool XProjectile::OnCreate()
{
	return true;
}

void XProjectile::OnDestroy()
{
	Destroy();
}

void XProjectile::Setup( MUID& uidPlayer 
						, XTalentInfo* pInfo
						, CSProjectileInfo* pProjectileInfo 
						, vec3& vStartPos 
						, vec3& velocity 
						, vec3 vAttackerStartPos /*= vec3::ZERO*/
						, MUID uidTarget /*= MUID::Invalid() */
						, int nGroupID /*= -1 */
						, int nCapsuleID /*= -1*/)
{
	if(pInfo == NULL ||
		pProjectileInfo == NULL)
	{
		// �ŷ�Ʈ ���� �� �ʿ�... ������ �������~~
		_ASSERT(pInfo != NULL);
		return;
	}

	m_pTalentInfo			= pInfo;
	m_pProjectileInfo		= pProjectileInfo;

	m_uidTarget				= uidTarget;
	m_uidAttaker			= uidPlayer;
	m_nTargetGroupID		= nGroupID;
	m_nTargetCapsuleID		= nCapsuleID;

	m_vStartPos				= vStartPos;
	m_vCalParabola			= vStartPos;
	m_vPos = m_vOldPos		= m_vStartPos;
	m_vDir					= velocity;
	m_vDir.Normalize(); 

	m_vVelocity				= velocity;

	// Entity�� ��ǥ ����
	SetPosition(m_vPos);
	SetDirection(m_vDir);

	XObject* pAttacker = gg.omgr->Find(uidPlayer);
	if(pAttacker)
	{
		m_eAttackerTypeID	= pAttacker->GetEntityType();
	}
	else
	{
		DeleteMe();
		return;
	}

	// �߻�ü Ÿ�� ����
	MMatrix matAttackerStart;
	matAttackerStart.SetLocalMatrix(pAttacker->GetPosition(), -pAttacker->GetDirection(), pAttacker->GetUpvector());
	if(vAttackerStartPos != vec3::ZERO)
		matAttackerStart.SetTranslation(vAttackerStartPos);

	SetTargetByProjectileType(matAttackerStart);

	// ����Ʈ ����
	m_Effect.Create(this, pInfo, m_pProjectileInfo);
	m_Effect.Update(m_vPos, m_vDir);

	// ���� Ÿ�Կ� ���� �Ӽ��� �ٸ��� �ش�.
	// ȭ���� Trace�� �����Ѵ�.
	// [12/12/2007 isnara] �κ� ����
	if(pInfo->m_nSkillType == ST_ARCHERY ||
		pInfo->m_nSkillType == ST_MAGIC)
	{
		SetAtt(true);
	}
}

void XProjectile::SetAtt( bool bCol, float fAutoDelTime /*= AUTO_DELETE_TIME*/ )
{
	m_bColCheck			= bCol;					// �浹
	m_fAutoDelTime		= fAutoDelTime;			// �ڵ� �Ҹ� �ð�
}

void XProjectile::SetIndex(int nIdx, int nMaxIdx)
{
	float fMinAngle, fMaxAngle;

	fMinAngle = MMath::DegreeToRadian(-20.0f);
	fMaxAngle = MMath::DegreeToRadian(20.0f);

	float fAngle = fMinAngle + (fMaxAngle - fMinAngle) * ((float)nIdx / (float)nMaxIdx);

	if (nMaxIdx <= 0) fAngle = 0.0f;
	if (m_pProjectileInfo &&
		m_pProjectileInfo->m_nType == TPT_MISSILE_GUIDED)
	{
		RMatrix zrot;
		zrot.SetRotationZ(fAngle);
		zrot.TransformVect(m_vDir);
		m_vDir.z += 0.2f;
		m_vDir.Normalize();
	}
}

void XProjectile::OnRender()
{
	m_Effect.RenderTrace();
}

vec3 XProjectile::GetTargetPos( MUID uidTarget, int nGroupID /*= -1*/, int nDummyID /*= -1*/ )
{
	XObject* pTarget = gg.omgr->Find(uidTarget);
	if(pTarget)
	{
		XModuleCollision* pTargetModuleCollision = pTarget->GetModuleCollision();
		if(pTargetModuleCollision)
		{
			return pTargetModuleCollision->GetHitEffectProjectileTargetPos(nGroupID, nDummyID);
		}
	}

	return vec3::ZERO;
}

void XProjectile::SetTargetByProjectileType( MMatrix matAttacker )
{
	if(m_pProjectileInfo->m_nType == TPT_MISSILE)
	{
		// �̻��� Ÿ���� TargetUID�� ���� �ʽ��ϴ�.
		m_uidTarget = MUID::ZERO;
	}

	if (m_pProjectileInfo->m_nType == TPT_HITCAPSULE_GUIDED)
	{
		// ����ĸ����ġ ��������
		int nSegmentIndex = m_pProjectileInfo->m_nSegmentIndexForHitCapsuleType;
		int nCapsuleIndex = m_pProjectileInfo->m_nCapsuleIndexForHitCapsuleType;

		if(nSegmentIndex < 0 ||
			nCapsuleIndex < 0)
			return;

		if(m_pTalentInfo && (int)m_pTalentInfo->m_HitInfo.m_vSegments.size() > nSegmentIndex)
		{
			if((int)m_pTalentInfo->m_HitInfo.m_vSegments[nSegmentIndex].m_vCapsules.size() > nCapsuleIndex)
			{
				MCapsule hitCapsule = m_pTalentInfo->m_HitInfo.m_vSegments[nSegmentIndex].m_vCapsules[nCapsuleIndex].Multiply(matAttacker);

				m_vTargetPos = hitCapsule.GetCenter();

				m_vDir = m_vTargetPos - m_vStartPos;
				m_vDir.Normalize();

				m_vVelocity = m_vDir * m_pProjectileInfo->m_fSpeed;

				m_uidTarget.SetZero();
			}
		}
	}

	if (m_pProjectileInfo->m_nType == TPT_MISSILE_GUIDED ||	m_pProjectileInfo->m_nType == TPT_HITCAPSULE_GUIDED)
	{
		m_fEstimateTime = 5.0f;
		vec3 vTargetPos = m_vTargetPos;
		XObject* pTarget = gg.omgr->Find(m_uidTarget);
		if(pTarget)
		{
			m_pColObject = pTarget;			// Ÿ���� �ִٴ°�... Ÿ���� �߻�ü�̴�. �׷��Ƿ� �浹 ������Ʈ�� �̹� ������ �ִ�.
			vTargetPos = pTarget->GetPosition();
		}

		float len = (m_vStartPos - vTargetPos).Length();
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

void XProjectile::ProjectileColResult( ProjectileColJudgmentType eType, XObject* pTarget )
{
	if(pTarget == NULL)
		return;

	// �̹� �浹�߳�?
	if(IsWaitColJudgment())
	{
		if(pTarget != m_pColObject)
		{
			// ���.. �ٸ���...
			// �׷��ٸ� �浹 ��ġ�� ������ �� �������� �����Ѵ�.
			//stColJedgment colJedgment;
			//colJedgment.eType = eType;
			//colJedgment.pTarget = pTarget;
			//m_vecColJedgment.push_back(colJedgment);
			m_vecColJedgment.push_back(stColJedgment(eType, pTarget));
			return;
		}

		ShowColEffect(m_pColObject, eType, m_vCoPos, m_vColDir);
		DeleteMe();
		//mlog("�̹� �浹�� �߻�ü\n");
		return;
	}

	// �浹�� ���� ���¶��...
	// Ÿ�����̳� �̻������̳�?
	if(m_uidTarget == MUID::ZERO)
	{
		// ��Ʈ ĸ��
		// �߻�ü�� ��Ʈĸ���̶�� ���� ����Ʈ�� �ʿ������? �ϴ� �ǵ�����.
		if(m_pProjectileInfo->m_nType == TPT_HITCAPSULE_GUIDED)
			return;

		// �̻���
		// �ٷ� ����ؼ� ó���Ѵ�.
		//stColJedgment colJedgment;
		//colJedgment.eType = eType;
		//colJedgment.pTarget = pTarget;
		m_vecColJedgment.push_back(stColJedgment(eType, pTarget));
		//mlog("�̻��� Ÿ��\n");
	}
	else
	{
		// Ÿ��
		// Ÿ������ ������ �浹�� �ǰ� ó�� �Ǿ� �����Ƿ� ������ �����ش�.
		m_bColJudgment = true;
		//mlog("Ÿ���� Ÿ��\n");
	}

	m_ColJudgmentType = eType;
} 

bool XProjectile::IsEqualProjectile( MUID uidAttacker, int nTalentID )
{
	if(m_uidAttaker == uidAttacker &&
		m_pTalentInfo &&
		m_pTalentInfo->m_nID == nTalentID)
	{
		return true;
	}

	return false;
}

void XProjectile::ShowColEffect( XObject* pColObject, ProjectileColJudgmentType eType, vec3 vColPos, vec3 vColDir )
{
	// ���� ����Ʈ ���
	m_Effect.CreateHitDefenceEffect(pColObject, m_Effect.GetProjectileColEffect(m_pProjectileInfo, eType), vColPos, vColDir);
}

void XProjectile::CheckColJedgment()
{
	// �̹� �浹 ���� ������ �Դٸ�...
	if(m_bColJudgment)
	{
		// ����Ʈ�� �����ش�.
		ShowColEffect(m_pColObject, m_ColJudgmentType, m_vCoPos, m_vColDir);

		// ������.
		DeleteMe();
	}
}

void XProjectile::ProjectileColJedgment_Missile( ProjectileColJudgmentType eType, XObject* pTarget )
{
	XModuleCollision* pModuleCollision = pTarget->GetModuleCollision();

	// �浹 �˻�
	vec3 vStartPos = m_vPos;
	XObject * pOwner = gg.omgr->Find(m_uidAttaker);
	if(pOwner)
	{
		pOwner->GetWorldTransform().TransformVect(m_pProjectileInfo->m_vLocalStartPos, vStartPos);
	}

	vec3 vTargetPos = pTarget->GetPosition();
	vTargetPos.z = vStartPos.z;

	vec3 vNearestPos = MMath::GetNearestPointOnRay(vTargetPos, vStartPos, m_vDir);

	ShowColEffect(pTarget, eType, vNearestPos, m_vDir);
	DeleteMe();
}

void XProjectile::UpdateColJedgment()
{
	for(vector<stColJedgment>::iterator it = m_vecColJedgment.begin(); it != m_vecColJedgment.end(); ++it)
	{
		ProjectileColJedgment_Missile(it->eType, it->pTarget);
	}

	m_vecColJedgment.clear();
}

bool XProjectile::Reload()
{
	if (m_pTalentInfo)
	{
		m_pTalentInfo = NULL;
		DeleteMe();
	}

	return XObject::Reload();
}
