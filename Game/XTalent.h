#ifndef _XTALENT_H
#define _XTALENT_H

#include "XEffectManager.h"
#include <list>
using namespace std;

class XTalent;
class XTalentInfo;
class XTalentEffectDataMgr;
class XTestFrame;
class XPlayFrame;
class XBackViewCameraSceneNode;
class XTalentEvent;
class XProjectile;

struct XTALENT_TARGET
{
	MUID					uidTarget;

	int						nGroupID;
	int						nCapsuleID;

	vec3					vTargetPos;

	void Init()
	{
		uidTarget			= MUID::ZERO;

		nGroupID			= -1;
		nCapsuleID			= -1;

		vTargetPos			= vec3::ZERO;
	}

	XTALENT_TARGET()
	{
		Init();
	}
};

enum XTalentClassID
{
	TCID_NONE = 0,
	TCID_MELEE,
	TCID_ARCHERY,
	TCID_MAGIC,
	TCID_ITEM,
	TCID_EXTRA_ACTIVE,
	TCID_EXTRA_ACTIVE_GRAPPLE,
	TCID_GUARD,
	TCID_MAGICAREA,
	TCID_EXTRA_ACTIVE_EAT,
};

class XBaseTalent
{
private:
	TALENT_PHASE			m_nPhase;										///< [������]

	bool					m_bActive;
	bool					m_bAllowedActingFromServer;
//	XTalentEffectDataMgr*	m_pTalentEffectDataMgr;

	wstring					m_strMotionName;
	bool					m_bReqActSpell;
	vec3					m_vReservedDirection;
	vec3					m_vProjectileTargetPos;
	vec3					m_vMagicAreaPos;

	vec3					m_vStartPos;
	vec3 					m_vActPos;
	MMatrix					m_matServerStart;

	float					m_fPostDelayRemainTime;

	void					ClearCastingEffect();
	void					CheckRequestSpell();
	void					SetAllowedActingFromServer(bool allow = false)	{ m_bAllowedActingFromServer = allow;	}
//	void					SetTalentEffectDataMgr(XTalentEffectDataMgr* pTalentEffectDataMgr) { m_pTalentEffectDataMgr = pTalentEffectDataMgr;}

	void					ChangePhase(TALENT_PHASE nPhase);		///< [������]
	void					SetInfo(XTalentInfo* pTalentInfo);		///< [������]
	void					OnEnterPhase(TALENT_PHASE nPhase);		///< [������]
	
	bool					IsChangePrepare(XTalentInfo* pTalentInfo);		// Prepare�� �ŷ�Ʈ�ΰ�?

	vec3					GetProjectileStartPos(CSProjectileInfo& csProjectileInfo);
	vec3					GetProjectileStartDir( vec3& vStartPos, MUID uidTarget, int nGroupID /*= -1*/, int nDummyID /*= -1*/ );

	void					UpdateRotationAnimation();

	void					CheckPostDelayTalent(float fDelta);

	// �ŷ�Ʈ �ִϸ��̼� 
	bool					ChangeTalentAnimation(XModuleMotion* pMotion, const wchar_t* szMotionName, const wchar_t* szName, float fMotionSpeedRatio, bool bUpper);
	bool					PushTalentAnimation(XModuleMotion* pMotion, const wchar_t* szMotionName, const wchar_t* szName, float fMotionSpeedRatio, bool bUpper);
	bool					ChangePrepareTalentMotion(XModuleMotion* pMotion, const wchar_t* szName, const wchar_t* szLoopName, MOVING_USE_TYPE nType, float fMotionSpeedRatio, bool& bNeedPushIdleMotion);
	bool					ChangeActTalentMotion(XModuleMotion* pMotion, XModuleAction* pAction, const wchar_t* szName, const wchar_t* szLoopName, MOVING_USE_TYPE nType, float fMotionSpeedRatio, bool& bNeedPushIdleMotion);

protected:
	XObject*				m_pOwner;								///< [������]
	XTalentInfo*			m_pTalentInfo;							///< [������]
	XTalentEvent*			m_pTalentEvent;
	XTALENT_TARGET			m_TalentTarget;
	float					m_fElapsedTime;
	float					m_fActElapsedTime;

	virtual void			OnStart();								///< [������]
	virtual void			OnReCastingTalent(int nTalentID, int nStep = 0)	{}

	virtual void			OnEnterPhasePrepare();
	virtual void			OnEnterPhaseAct();
	virtual void			OnEnterPhasePostDelay();
	virtual void			OnEnterPhaseFinish()		{}			///< [������]
	virtual void			OnEnterPhaseCanceled()		{}			///< [������]
	virtual void			OnEnterPhaseExtraAct()		{}
	virtual void			OnEnterPhaseExtraAct2()		{}
	virtual void			OnEnterPhaseExtraAct3()		{}

	void					PreLoaderTalentEffect(XTalentInfo* pTalentInfo);
	void					ChangeTalentEffect(TALENT_PHASE nPhase);		// �ŷ�Ʈ ����Ʈ ����

	wstring					_GetFullPlayerTalentAniName(const wchar_t* szName);	///< //���� Ÿ�Ժ� �ִϸ��̼��̸� ����.
	virtual void			ChangeTalentMotion(TALENT_PHASE nPhase, const wchar_t* szName, const wchar_t* szLoopName = L"", MOVING_USE_TYPE nType = MUT_NONE);

//	XTalentEffectDataMgr*	GetTalentEffectDataMgr()	{ return m_pTalentEffectDataMgr; }

	/// Act �̺�Ʈ ������ Act()���� �������� ���� ��� ���
	/// - ���� Extra Phase ������ TalentEvent�� �׳� Act�� ����ϱ� ����
	virtual bool				IsNotUseActEventOnAct() { return false; }	
public:
	XBaseTalent(XObject* pOwner);
	virtual ~XBaseTalent(void);

	void						Start(XTalentInfo* pTalentInfo);		///< [������]
	virtual bool				IsNeedTarget();							///< [������]
	virtual XTalentClassID		GetID() { return TCID_NONE; }			///< [������]



	void					Act();
	void					ReCastingTalent(int nTalent, int nStep = 0);
	void					ExtraAct();
	void					ExtraAct2();
	void					ExtraAct3();

	void					SetTarget(MUID uidTarget, int nGroupID, int nDummyID, vec3 vTargetPos);
	bool					Cancel();
	void					RequestActSpell();
	void					ForceTerminate();
	virtual void			OnUpdate(float fDelta);
	virtual void			OnPostUpdate(float fDelta) {}
	void					PostDelayTalent();
	void					FinishTalent();
	void					LaunchProjectile(CSProjectileInfo& csProjectilInfo);
	void					DelayAct(wstring strParam);

	bool					IsActive()					{ return m_bActive;	}
	virtual XEventResult	OnEvent(XEvent& msg);

	XTalentInfo*			GetInfo()					{ return m_pTalentInfo;	}
	TALENT_PHASE			GetPhase()					{ return m_nPhase;		}
	XTALENT_TARGET&			GetTargets()				{ return m_TalentTarget; }
	XTalentEvent*			GetTalentEvent()			{ return m_pTalentEvent; }

	void					SetProjectileTargetPos(vec3 vTargetPos) { m_vProjectileTargetPos = vTargetPos; }
	void					SetServerStartMatrix(MMatrix matStart) { m_matServerStart = matStart; }

	void					SetupTargetProjectile(XProjectile* pProjectile, CSProjectileInfo& csProjectilInfo, const XTALENT_TARGET& TargetInfo);
	void					SetupPosProjectile(XProjectile* pProjectile, CSProjectileInfo& csProjectilInfo, vec3 vTargetPos);

	float					GetElapsedTime() const  { return m_fElapsedTime; }
	float					GetActElapsedTime() const  { return m_fActElapsedTime; }

	bool					GetAllowedActingFromServer() { return m_bAllowedActingFromServer; }

	vec3					GetStartPos() { return m_vStartPos; }
	MMatrix					GetServerStartMatrix() { return m_matServerStart; }
};

#endif // _XTALENT_H