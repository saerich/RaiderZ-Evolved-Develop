#ifndef _XMODULE_ENTITY_H
#define _XMODULE_ENTITY_H

#include <map>
#include <string>

#include "XModule.h"
#include "MTypes.h"
#include "MTypelist.h"
#include "RBackgroundWorkUnit.h"
#include "MPhysX4R3/MPxTransformController.h"


#define CHARACTER_PANE_SCALE	0.6f

using namespace std;
using namespace rs3;
using namespace physx;

class XModuleEntity;
class XNPCInfo;
class XCharacter;

// Caller���
class XObject;
class XActor;
class XBaseTalent;
class XModuleActorControl;
class XCharacterSerializer;
class XGameTransformControllerManager;
class XEntityFader;
class XEntityDirectionInterpolator;

class XVictimFrameFreezer
{
private:
	bool m_bReserve;
	float m_fReserveElapsedTime;
	wstring	m_strCurrAni;

	void FreezeFrame(XObject* pOwner);
public:
	XVictimFrameFreezer() : m_bReserve(false) {}
	void OnHit(XObject* pOwner, const wchar_t* szCurrAni);
	void Update(float fDelta, XObject* pOwner, const wchar_t* szCurrAni);
};

struct stLoopAniEffect
{
	MUID				m_uid;
	wstring				m_string1;
	wstring				m_string2;
	wstring				m_string3;
};

struct stMultiplyColorForBPart 
{
	MRegulator			regMultiplyColor;
	wstring				strActorNodeName;

	stMultiplyColorForBPart()
	{
		regMultiplyColor.Stop();
	}
};

struct XCharExtFeature
{
	int			nMakeUp;
	int			nTattoo;
	float		fTattooScale;
	vec2		vTattooPos;
};




/// @brief ������Ʈ�� �����ϴ� �⺻ ��Ҹ� ����ϴ� ��� - ��ġ, ���, �ִϸ��̼�
class XModuleEntity : public XModule, public RListener
{
private:
	DECLARE_ID(XMID_ENTITY);

	// by pok. �������� �ϴ°͵�
	friend class XThrowMove;		// XThrowMove���� SetPosition�� �������� movable�� MoveForce���� ������ ���ڽ��ϴ�.
	friend class XActionDie;		// ���� ���� ���� XModuleAction�� �ð����� ���ڽ��ϴ�.
	friend class XMyActionGrappled; // ���� SetPostion���� ���� �ٸ����� ���ؼ� ������ ���ڽ��ϴ�.

private:
	vec3				m_vPos;
	vec3				m_vDir;
	vec3				m_vUp;
	vec3				m_vRenderPos;	// ���� ��ġ�� �����ϰ� ��ġ�� ����
	vec3				m_vRenderDir;	// ���� ��ġ�� �����ϰ� ������ ����
	vec3				m_vRenderUp;	// ���� ��ġ�� �����ϰ� ������ ����


	float				m_fMaxVisibility;	// ����ȭ ���� �뵵�� ���. SetVisibility�� �����ص� �� ������ ũ�� �� ������ Visibility�� �����Ѵ�.
	float				m_fScale;		// ���� ������
	XCharacter*			m_pActor;
	XGameTransformControllerManager*	m_pAnimationController;
	bool				m_bLoadingCompleted;
	bool				m_bAlpha2PassRendering;

	XEntityFader*		m_pFader;
	XEntityDirectionInterpolator*		m_pDirectionInterpolator;

	// Loop �ִϿ� Loop ����Ʈ�� ���� ����
	vector<stLoopAniEffect>	m_vLoopAniEffect;

	float				m_fCharacterPaneRadius;
	float				m_fCharacterPaneMinZ;
	bool				m_bInitCharacterPane;
	bool				m_bHasCombatIdleAnimation;

	typedef				vector<MPxTransformController*> VEC_TC;
	VEC_TC				m_vecTransformControllers;
	MPxTcEntity*		m_pTcEntity;
	bool				m_bPauseTransformControl;

	XCharacterSerializer*		m_pSerializer;
	XCharExtFeature*			m_pCharExtFeature;			// ĳ���� Ȯ�� �ܸ� ����

	MRegulator			m_RegMultiplyColor;
	stMultiplyColorForBPart			m_RegMultiplyColorForBParts[BREAKABLEPARTS_MAX_COUNT];
	XVictimFrameFreezer	m_FrameFreezer;

	// ��ġ, ��������� ��ƼƼ(RActor)�� �ݿ�
	void						_SetPosition(const vec3& pos);
	void						_SetDirection(const vec3& dir);
	void						_SetUpVector(const vec3& up);
	void						_SetRenderPosDir( const vec3& rpos, const vec3& rdir);
	void						_SetRenderUpDir( const vec3& rup, const vec3& rdir);
	void						_SetRenderPosition( const vec3& rpos);
	void						_SetRenderDirection( const vec3& rdir);
	void						_UpdateTransform();


	XNPCInfo*					m_pNPCInfo;


	// ���� Ÿ���� �ϰ� �ִ� ������Ʈ�� UID
	MUID						m_uidTarget;
	MUID						m_uidExtraTarget;		// ���, �Ա⿡ ���̴� Ÿ��... ������ ��Ұų� �Ծ����� �˱� ���Ѱ�... (Ŭ���)

	bool						bEntitySoundPlaying;
	bool						m_bInternalWarp;

protected:

	virtual void				OnAdd();
	virtual void				OnRemove();
	virtual void				OnActivate();
	virtual void				OnDeactivate();
	virtual void				OnUpdate(float fDelta);

	virtual void				OnInitialized();
	virtual void				OnFinalized();
	virtual void				OnSubscribeEvent();
	virtual XEventResult		OnEvent(XEvent& msg);

public:
	void						OnLoadingComplete();
	void						CheckLoadingCompleted();
	void						OnWarp();

	XModuleEntity(XObject* pOwner=NULL);
	virtual ~XModuleEntity(void);


	//////////////////////////////////////////////////////////////////////////
	// Transform Controller
	//
	MPxTransformController*		GetTransformController(int nID){ return m_vecTransformControllers[nID]; }
	template<typename T> T*		GetTransformController(){ return static_cast<T*>( GetTransformController(T::GetID()) ); }
	template<typename T> void	SetEnableTransformControl(bool bEnable){m_vecTransformControllers[T::GetID()]->SetEnable(bEnable); }
	void						PauseTransformControl(bool bPause){ m_bPauseTransformControl = bPause; }


	//////////////////////////////////////////////////////////////////////////
	// XEVTL_MOTION_CHANGED_TO_IDLE �޽����� XTalent���� �޾� ModuleEntity�� Direction�� �����Ѵ�.. ��?
	//
	inline void					SetDirectionDelayed( const vec3& dir, const XBaseTalent* _pCaller );


	//////////////////////////////////////////////////////////////////////////
	// ��ġ����
	//
	inline void				SetPosition( const vec3& pos, const XModuleMovable* pCallerMovableModule );
	inline void				SetDirection( const vec3& dir, const XModuleMovable* pCallerMovableModule );
	inline void				SetUpVector( const vec3& up, const XModuleMovable* pCallerMovableModule );

	inline void				SetPosition( const vec3& pos, const XObject* pCallerObject );
	inline void				SetDirection( const vec3& dir, const XObject* pCallerObject );
	inline void				SetUpVector( const vec3& up, const XObject* pCallerObject );

	inline void				SetRenderPosDir( const vec3& rpos, const vec3& rdir);
	inline void				SetRenderUpDir( const vec3& rup, const vec3& rdir);
	inline void				SetRenderPosition( const vec3& rpos);
	inline void				SetRenderDirection( const vec3& rdir);
	void					InitRenderPosDirUp();
	void					SetDirectionSmoothly(const vec3& dir, float fRotateSpeed);
	void					StopDirectionInterpolation();

	void					SetTransform( const MMatrix& mat, const MPxTransformController* pCaller );


	void			SetVisible(bool bVisible);
	void			SetScale(float fScale){ m_fScale = fScale; }

	vec3			GetAnimationPos();
	vec3			GetAnimationDir();

	vec3			GetPosition();
	vec3			GetDirection()		{ return m_vDir; }
	vec3			GetUpvector()		{ return m_vUp; }

	vec3			GetRenderPosition()			{ return m_vRenderPos; }
	vec3			GetRenderDirection()		{ return m_vRenderDir; }

	void			SetTarget( const MUID& target)	{ m_uidTarget = target;		}
	const MUID&		GetTarget()						{ return m_uidTarget;		}
	void			SetExtraTarget( const MUID& target)	{ m_uidExtraTarget = target;		}
	const MUID&		GetExtraTarget()						{ return m_uidExtraTarget;		}

	XCharacter*		GetActor()			{ return m_pActor; }
	float			GetScale()			{ return m_fScale; }
	bool			IsVisible();
	vector<stLoopAniEffect>&	GetAniEffect()	{ return m_vLoopAniEffect; }

	virtual void	OnAnimationEvent(RActor* pActor, const RAnimationEvent* pEvent);
	bool			Pick(RPICKINFO& pickInfo);

	// serialize ���� - �ܺο����� �޽� ���� ��ȯ�� �� �������̽��� �����մϴ�. �ٸ� �������̽��� ������ �����ּ��� - birdkr
	void			SerializeNetPlayer(bool bLoadingAsync = true);		
	void			SerializeMyPlayer(PLAYER_INFO& playerInfo, XMyEquipmentSlot* pEquipmentSlot);			/// PlayerInfo ����� Feature���� Serialize
	void			SerializeNPC(XNPCInfo* pNPCInfo, bool bLoadingAsync = true);
	void			SerializeBPartNPC(XNPCInfo* pNPCInfo, std::wstring strMeshNodeName, std::wstring strActorNodeName, bool bLoadingAsync = true);

	void			DelAllLoopAniEffect();

	void			AddToSceneManager();
	bool			RemoveFromSceneManager();

	//
	bool			IsHaveDummy();
	bool			GetMovingDummyPos(vec3& vAnimPos);
	bool			GetMovingDummyDir(vec3& vAnimDir);

	void			StartFade(float fAlpha, float fTime);
	void			StopFade();
	void			SetVisibility(float fVisibility);
	void			SetMaxVisibility(float fMaxVisibility);
	bool			IsFading();
	float			GetVisibility();
	bool			IsDirectionInterpolatingNow();
	void			StartMultiplyColor();
	void			StartFrameFreeze();
	void			StartMultiplyColorForBParts(wstring strActorNodeName, int nBPartsIndex);

	XGameTransformControllerManager*	GetAnimationController() { return m_pAnimationController; }
	// NPC��
	

	// TEST Sub Material
	void			SetNPCInfoSubMtrl( int NPCID );
	XCharacterSerializer*	GetSerializer()	{ return m_pSerializer; }	// for test

	// Set character extend feature
	void			SetCharExtFeature( const XCharExtFeature* pExtFeature);


	bool IsLoadingCompleted();

	// Face Animation
	void			CreateFaceAnimation(bool bNPC = false);
	void			SetFaceAnimationActive(bool bActive);
	void			SetFaceBlinkAnimationActive(bool bActive);

	bool			IsAnimationUpdateCompleted();
	void			ReSetAnimationUpdateCompleted();

	void			CreateFakeBeatenAnimationController(tstring& strAniName, float fWeight, float fSpeed);
	void			StartFakeBeaten(WEAPON_TYPE nAttackWeaponType, float fCriWeightRate, float fCriSpeedRate);

	#ifdef _DEBUG
	std::set<std::wstring> m_debugInfos;
	#endif
};

//----------------------------------------------------------------------------------------------------
inline void XModuleEntity::SetDirectionDelayed( const vec3& dir, const XBaseTalent* _pCaller )
{
	_SetDirection(dir);
}
//----------------------------------------------------------------------------------------------------
inline void XModuleEntity::SetPosition( const vec3& pos, const XModuleMovable* pCallerMovableModule )
{
	_SetPosition(pos);
}
//----------------------------------------------------------------------------------------------------
inline void XModuleEntity::SetPosition( const vec3& pos, const XObject* pCallerObject )
{
	_SetPosition(pos);
}
//----------------------------------------------------------------------------------------------------
inline void XModuleEntity::SetDirection( const vec3& dir, const XObject* pCallerObject)
{
	_SetDirection(dir);
}
//----------------------------------------------------------------------------------------------------
inline void XModuleEntity::SetDirection( const vec3& dir, const XModuleMovable* pCallerMovableModule )
{
	_SetDirection(dir);
}
//----------------------------------------------------------------------------------------------------
inline void XModuleEntity::SetUpVector( const vec3& up, const XModuleMovable* pCallerMovableModule )
{
	_SetUpVector(up);
}
//----------------------------------------------------------------------------------------------------
inline void XModuleEntity::SetUpVector( const vec3& up, const XObject* pCallerObject )
{
	_SetUpVector(up);
}
//----------------------------------------------------------------------------------------------------
inline void XModuleEntity::SetRenderPosDir( const vec3& rpos, const vec3& rdir)
{
	_SetRenderPosDir( rpos, rdir);
}
//----------------------------------------------------------------------------------------------------
inline void XModuleEntity::SetRenderPosition( const vec3& rpos)
{
	_SetRenderPosition( rpos);
}
//----------------------------------------------------------------------------------------------------
inline void XModuleEntity::SetRenderDirection( const vec3& rdir)
{
	_SetRenderDirection( rdir);
}
//----------------------------------------------------------------------------------------------------
inline void	XModuleEntity::SetRenderUpDir( const vec3& rup, const vec3& rdir)
{
	_SetRenderUpDir(rup, rdir);
}
//----------------------------------------------------------------------------------------------------
#endif // #ifndef _XMODULE_ENTITY_H
