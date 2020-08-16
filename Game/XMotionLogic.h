#pragma once

#include "XMotion.h"
#include "CSItemData.h"
#include "RSkeleton.h"
#include "XEventID.h"
#include "XMotionTrigger_Checker.h"
#include "XMotionTrigger_Executor.h"

class XGameTransformControllerManager;
class XMotionTimeModifier;
class XNPCInfo;
class XMotionIdleMemory;

class XMotionAniNameMaker
{
private:
public:
	void MakePlayerAniName(wstring& outName, XMotion* pMotion, XMotionTypes curr, int nAniSequence, WEAPON_TYPE nWeaponType, CHAR_STANCE nStance);
	void MakeNPCAniName(wstring& outName, XMotion* pMotion, XMotionTypes curr, int nAniSequence, int nCurrNPCMode, XNPCInfo* pNPCInfo);
};


/// Motion ���� ó�� ��� Ŭ����
class XMotionLogic
{
	struct SCustomAniName
	{
		wstring	m_strMotion;
		wstring	m_strAni;

		SCustomAniName(const wstring& motion, const wstring& ani) : m_strMotion(motion), m_strAni(ani) {}
	};

	struct SMotionQueueNode
	{
		wstring	m_strMotionName;
		float	m_fSpeed;
		float	m_fSpeedRatio;
	};

	class XMotionError
	{
	private:
		wstring					m_strMotionError;
		wstring					m_strAnimationError;
	public:
		void Clear();
		void ClearMotionError();
		void ClearAnimationError();
		void SetMotionError(const wchar_t* szMsg);
		void SetMotionError(wstring& strMsg);
		void SetAnimationError(const wchar_t* szMsg);
		void SetAnimationError(wstring& strMsg);
		wstring GetAnimaionError() { return m_strAnimationError; }
		bool IsExistMotionError();
		bool IsExistAnimationError();
	};
protected:
	XActor*					m_pOwner;
	XGameTransformControllerManager*	m_pAnimationController;
	XMotionTimeModifier*	m_pMotionTimeModifier;
	wstring					m_strAniPostfix;
	bool					m_bUseAniPostfix;
	float					m_fReservedSpeed;
	XMotionError			m_Error;

	XMotionIdleMemory*		m_pMotionIdleMemory;

	static XMotionTrigger_Checker		m_TriggerChecker;
	static XMotionTrigger_Executor		m_TriggerExecutor;

	// event
	XMotion*				OnPreEnter(XMotion* pMotion);
	void					OnEnter(XMotion* pMotion);
	void					OnLeave(XMotion* pMotion);
	void					OnUpdate(XMotion* pMotion);
protected:
	XMotion*				m_pCurrMotion;
	int						m_nCurrAniSequence;
	XMotionTypes			m_nCurrType;
	XMotionTypes			m_nForcedType;			///< �� ���� MT_DEFAULT�� �ƴ� ������ �����Ǿ� ������ CurrType�� ������� �� ��� Ÿ������ ����� �ٲ۴�.
	wstring					m_strLastEvent;
	bool					m_bUseWeaponAni;		///< �÷��̾�� ���⺰ �ִϸ��̼��� �ٸ��� ������
	bool					m_bChanged;
	WEAPON_TYPE				m_nWeaponType;


	bool					m_bPlayDone;
	bool					m_bHoldSequence;

	vector<SCustomAniName>	m_vCustomMotionAniName;
	list<SMotionQueueNode>	m_MotionQueue;

	void AddCustomMotionAniName(const wchar_t* szMotionName, const wchar_t* szAnimationName);
	void MakeCurrAniString( wstring& strOut, XMotionTypes curr );
	void MakeCurrAniStringNoWeapon(wstring& strOut, XMotionTypes curr);

	virtual bool _ChangeMotion(const wchar_t* szMotionName, XMotionTypes nType, bool bForced, bool bPreFrame = false );
	virtual bool SetAnimation(bool bForced=false, bool bPreFrame = false);

	void ResetInfo();
	void ResetAniTime();
	void SetSequence(bool bForce);
	virtual void PlayDone();

	void PlayNextMotion();
	void AnimationError();
	void SendEvent(XEventID nEventID);
	bool CheckPostTrigger(int nSeqIndex);

	void NextSequence( int nNextSeqIndex );

	bool CheckDuplicationNextSequenceAnimation(int nNextSeqIndex);
public:
	XMotionLogic(XActor* pOwner, XGameTransformControllerManager* pAnimationController, XMotionTimeModifier* pMotionTimeModifier, XMotionIdleMemory* pMotionIdleMemory);
	virtual ~XMotionLogic();

	static void BindMotionEvents();

	virtual void Update(float fDelta);

	void OnEvent(const wchar_t* szEvent);

	bool ChangeMotion(const wchar_t* szMotionName, XMotionTypes nType=MT_DEFAULT, bool bForced=false, bool bPreFrame = false);
	bool ChangeMotion(const wchar_t* szMotionName, const wchar_t* szNextMotionName, XMotionTypes nType=MT_DEFAULT, bool bForced=false, bool bPreFrame = false);
	bool ChangeToCustomMotion(const wchar_t* szMotionName, const wchar_t* szAnimationName, bool bForced=false, bool bPreFrame = false);

	void PushMotion(const wchar_t* szMotionName, float fMotionSpeed =1.0f, float fMotionSppedRatio = 1.0f);
	void PushCustomMotion(const wchar_t* szMotionName, const wchar_t* szAnimationName, float fMotionSpeed =1.0f, float fMotionSppedRatio = 1.0f);

	void Reload();
	void RefreshThisMotion();

	void SetAnimationTime(int nAnimationIndex, unsigned int nPlayTime);
	void SetSpeed(float fSpeed);
	float GetSpeed();
	float GetOrgSpeed();
	void SetMotionTime(unsigned int nPlayTime);
	float GetMotionTime();
	void ChangeWeaponAniType(WEAPON_TYPE nWeaponType);	///< ���� ��� Ÿ���� �ܺο��� ���� ��������� �Ѵ�.
	void ChangeStanceType();	// �ʿ� �Ҷ����� ������

	void HoldSequence();
	void HoldRelease();
	bool IsHoldingSequence() { return m_bHoldSequence; }

	void SetAnimationNamePostfix(const wchar_t* szPostfix);
	void ReleaseAnimationNamePostfix();

	void ReserveNextMotionSpeed(float fSpeed);		// ����� ù��° Sequence�� ����ȴ�.

	wstring GetCurrMotion();
	wstring GetCurrSequence();
	XMotionTypes GetCurrMotionType();
	wstring GetCustomMotionAniName(const wstring& szMotionName);

	bool IsCameraLook();
	bool IsCurrMotionLoop();
	bool IsCurrMotionPlayDone();
	virtual bool IsCurrentMotion(wstring szMotionName);
	bool IsPastDeadPostTrigger();

	bool ChangeMotionQueueInfo(wstring strMotionName, wstring strAnimationName, wstring strChangeMotionName, wstring strChangeAnimationName);
};
