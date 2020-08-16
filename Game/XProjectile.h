#ifndef _XPROJECTILE_H
#define _XPROJECTILE_H

#include "XObject.h"
#include "XDef.h"
#include "XEffect.h"
#include "XProjectileEffect.h"

#define AUTO_DELETE_TIME			30.0f

class XProjectile;
class XTalentInfo;
class CSProjectileInfo;
class XTalentEffectDataMgr;
class XTalentEffectInfo;
enum TALENT_EFFECT_TYPE;

using namespace rs3;

enum ProjectileColJudgmentType
{
	PCJT_NONE,
	PCJT_HIT,
	PCJT_DEFENCE
};

class XProjectile : public XObject, public MMemPool<XProjectile>
{
	MDeclareRTTI;
private:
	struct stColJedgment
	{
		ProjectileColJudgmentType	eType;
		XObject*					pTarget;

		stColJedgment(ProjectileColJudgmentType e, XObject* p)
		{
			eType				= e;
			pTarget				= p;
		}
	};

private:
	XModuleEntity*				m_pModuleEntity;
	XProjectileEffect			m_Effect;
	XTalentInfo*				m_pTalentInfo;

	CSProjectileInfo*			m_pProjectileInfo;

	// Ÿ���� ���
	MUID						m_uidAttaker;						// �߻���
	MUID						m_uidTarget;						// Ÿ��
	vec3						m_vTargetPos;						// Ÿ�� ��ǥ
	
	vec3						m_vPos;								// �� ��ǥ
	vec3						m_vOldPos;							// ���� ��ǥ
	vec3						m_vDir;								// �� ����
	vec3						m_vVelocity;						// �ӷ�

	// ��꿡 �ʿ��� ��ǥ
	vec3						m_vStartPos;						// ���� ��ǥ
	vec3						m_vCalParabola;						// ������ ���� ��ǥ

	float						m_fElapsedTime;						// �ð�
	float						m_fAutoDelTime;						// �ڵ� �Ҹ� �ð�
	float						m_fEstimateTime;					// ���߱��� ���� �ð� ����

	bool						m_bColCheck;						// �浹 üũ?
	bool						m_bDelMe;							// ����?
	bool						m_bWaitColJudgment;					// �浹�� ���� �Ǵ�
	bool						m_bColJudgment;

	int							m_nTargetGroupID;					// Ÿ�� �׷� ID
	int							m_nTargetCapsuleID;					// Ÿ�� ���� ID
	ENTITY_TYPEID				m_eAttackerTypeID;					// ������ Ÿ�� ID

	// �浹�� ���� ����
	XObject*					m_pColObject;						// �浹�� ������Ʈ
	vec3						m_vCoPos;							// �浹 ��ġ...
	vec3						m_vColDir;
	ProjectileColJudgmentType	m_ColJudgmentType;

	vector<stColJedgment>		m_vecColJedgment;

protected:
	void						Destroy();

	void						SetAtt(bool bCol, float fAutoDelTime = AUTO_DELETE_TIME);
	void						SetTargetByProjectileType( MMatrix matAttacker );

	bool						CheckTimeOut();
	bool						CheckGPS();
	bool						CheckRange();
	bool						CheckColTerrain(vec3& old_pos, vec3& pos);	///< ���� �浹 üũ
	bool						CheckColObj(vec3& old_pos, vec3& pos);		///< ������Ʈ �浹 üũ
	bool						CheckColBase(vec3& old_pos, vec3& pos, XObject * pObject, float & out_Distance, vec3& out_colPos, bool bDirCheck = false);

	void						UpdateMoving( float fDelta );
	bool						UpdateCol(float fDelta);
	void						UpdateColJedgment();

	vec3						GetTargetPos(MUID uidTarget, int nGroupID = -1, int nDummyID = -1);

	// �߻�ü Ÿ��
	void						Projectile_Missile(float fDelta);
	void						Projectile_Guided(float fDelta);
	void						Projectile_HitCapsule(float fDelta);

	// �̵� ����
	void						Move_Straight(float fDelta, vec3 * vTargetPos = NULL);		// ����(�߷� ���� ����)
	void						Move_Curve(float fDelta, vec3 * vTargetPos = NULL);			// ����(��� ����)
	void						Move_Parabola(float fDelat, vec3 * vTargetPos = NULL);		// ������

	void						SetMoveData(vec3 vPos);				// �̵� ������ �Է�

	void						ShowColEffect(XObject* pColObject, ProjectileColJudgmentType eType, vec3 vColPos, vec3 vColDir);
	void						CheckColJedgment();
	void						ProjectileColJedgment_Missile(ProjectileColJudgmentType eType, XObject* pTarget);

	virtual void				RegisterModules();
	virtual void				UnregisterModules();
	virtual void				OnUpdate(float fDelta);
	virtual bool				OnCreate();
	virtual void				OnDestroy();
	virtual void				OnRender();
public:
	XProjectile(MUID uid);
	virtual ~XProjectile();

	virtual XObjectID				GetType() { return XOBJ_PROJECTILE; }
	virtual bool		Reload();

	// ������ �Է�
	void						Setup(MUID& uidPlayer
										, XTalentInfo* pInfo
										, CSProjectileInfo* pProjectileInfo
										, vec3& vStartPos
										, vec3& velocity
										, vec3 vAttackerStartPos = vec3::ZERO
										, MUID uidTarget = MUID::Invalid()
										, int nGroupID = -1
										, int nCapsuleID = -1);

	void						SetIndex(int nIdx, int nMaxIdx);	// ���ÿ� ������ ������ ���° ���̳� - �ʱ���� ����

	bool						IsEqualProjectile(MUID uidAttacker, int nTalentID);
	bool						IsWaitColJudgment()		{ return m_bWaitColJudgment; }

	XObject *					GetColObject()			{ return m_pColObject; }
	XTalentInfo*				GetTalentInfo()			{ return m_pTalentInfo; }

	void						ProjectileColResult(ProjectileColJudgmentType eType, XObject* pTarget);
};

#endif