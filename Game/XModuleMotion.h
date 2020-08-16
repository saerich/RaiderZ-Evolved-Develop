#ifndef _XMODULE_MOTION_H
#define _XMODULE_MOTION_H

#include "XModule.h"
#include "XMotion.h"
#include "CSItemData.h"
#include "RSkeleton.h"
#include "XMotionIdleMemory.h"

class XMotionLogic;
class XUpperMotionLogic;



/// @brief ������Ʈ���� �ִϸ��̼��� �����ϴ� ���
/// @author �����
class XModuleMotion : public XModule, public MMemPool<XModuleMotion>
{

	DECLARE_ID(XMID_MOTION);
private:
	XModuleEntity*			m_pModuleEntity;
	XMotionIdleMemory		m_MotionIdleMemory;

protected:
	XMotionLogic*			m_pMotionLogic;
	XUpperMotionLogic*		m_pUpperMotionLogic;


private:
	virtual void			OnUpdate(float fDelta);
	virtual XEventResult	OnEvent(XEvent& msg);
	virtual void			OnSubscribeEvent();
	virtual void			OnInitialized();

public:
	bool ChangeMotion(const wchar_t* szMotionName, XMotionTypes nType=MT_DEFAULT, bool bForced=false, bool bForceIdle=false );	///< ��� ����
	bool ChangeMotion(const wchar_t* szMotionName, const wchar_t* szNextMotionName, XMotionTypes nType=MT_DEFAULT, bool bForced=false );	///< ��� ����
	bool ChangeToCustomMotion(const wchar_t* szMotionName, const wchar_t* szAnimationName, bool bForced=false);	///< Ư���� ������� ����(��ų ��)
	void RefreshThisMotion();

	XMotionTypes GetCurrMotionType();


	// ������ --
	XModuleMotion(XObject* pOwner=NULL);		///< ������
	virtual ~XModuleMotion(void);				///< �Ҹ���

	void PushMotion(const wchar_t* szMotionName, float fMotionSpeed =1.0f, float fMotionSppedRatio = 1.0f, bool bForceIdle=false);									///< ��� ����. ����� ť�� �ִ´�.
	void PushCustomMotion(const wchar_t* szMotionName, const wchar_t* szAnimationName, float fMotionSpeed =1.0f, float fMotionSppedRatio = 1.0f);	///< Ư���� ������� ����(��ų ��)

	void SetPause(bool bPause);
	void ChangeWeaponAniType(WEAPON_TYPE nWeaponType);	///< ���� ��� Ÿ���� �ܺο��� ���� ��������� �Ѵ�.	//--
	void ChangeStanceType(); // �ʿ��� ���� ������.
	void SetAnimationTime(int nAnimationIndex, unsigned int nPlayTime);		///< �ش� �ε��� �ִϸ��̼��� �ӵ��� �����Ѵ�.
	void SetMotionTime(unsigned int nPlayTime);		///< ��ü ��� �÷���Ÿ��
	float GetMotionTime();
	void SetSpeed(float fSpeed);		///< ��� ���ǵ� ����
	float GetSpeed();					///< ��� ���ǵ� ����
	float GetOrgSpeed();

	wstring GetCurrMotion();
	wstring GetCurrSequence();
	wstring GetCustomMotionAniName(const wstring& szMotionName);

	bool IsCameraLook();
	bool IsCurrMotionLoop();
	bool IsCurrMotionPlayDone();
	bool IsCurrentMotion(wstring szMotionName);
	bool IsPastDeadPostTrigger();

	void HoldSequence( bool bHold);
	bool IsHoldingSequence();

	void SetAnimationNamePostfix(const wchar_t* szPostfix);
	void ReleaseAnimationNamePostfix();

	void ReserveNextMotionSpeed(float fSpeed);

	void Reload();

	//////////////////////////////////////////////////////////////////////////
	// ��ü �ִϸ��̼�
	bool ChangeUpperMotion(const wchar_t* szMotionName, XMotionTypes nType=MT_DEFAULT, bool bForced=false );	///< ��� ����
	bool ChangeUpperMotion(const wchar_t* szMotionName, const wchar_t* szNextMotionName, XMotionTypes nType=MT_DEFAULT, bool bForced=false );	///< ��� ����
	bool ChangeToCustomUpperMotion(const wchar_t* szMotionName, const wchar_t* szAnimationName, bool bForced=false);	///< Ư���� ������� ����(��ų ��)
	
	void SetUpperAnimationTime(int nAnimationIndex, unsigned int nPlayTime);		///< �ش� �ε��� �ִϸ��̼��� �ӵ��� �����Ѵ�.

	void StopUpperMotion();

	wstring GetCurrUpperMotion();
	bool IsCurrentUpperMotion(wstring szMotionName);

	void SetUpperSpeed(float fSpeed);		///< ��� ���ǵ� ����

	//////////////////////////////////////////////////////////////////////////
	// Memory
	void SetIdleMemoryMotion(int nBuffID, wstring strMotion, XMotionTypes nType=MT_DEFAULT, bool bForced=false);
	void SetIdleMemoryCustomMotion(int nBuffID, wstring strMotion, wstring strAnimationName, bool bForced=false);
	void RemoveIdleMemory(int nBuffID);
	void RemoveAllIdleMemory();

};


#endif // _XMODULE_MOTION_H