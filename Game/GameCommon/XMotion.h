#pragma once

#include "CSDef.h"
#include "CSStrings.h"
#include "MXml.h"
#include "XAnim.h"
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

#include "CSItemData.h"
#include "XMotionTrigger.h"

class XMotion;
class XMotionMgr;
class XModuleMotion;


enum XMotionTypes
{
	MT_DEFAULT = 0,
	MT_FORWARD,
	MT_BACKWARD,
	MT_LEFT,
	MT_RIGHT,
	MT_COUNT
};

class XMotionAniSequence
{
	friend class XMotionMgr;
	friend class XMotion;
private:
	bool			m_bWeaponAni;
	vector<tstring>	m_WeaponAniName;		// �����̶� ������ �ϱ����� �̸� �ִϸ��̼� �̸��� ������ �ִ´�.
	TCHAR			m_szAniName[128];
	TCHAR			szEvent[64];
	bool			bExistEvent;
	float			m_fSpeed;
	float			m_fTimeOut;			///< �� ���� �ð��� ������ ���� �������� �Ѿ��.
	int				m_nFrameOver;		///< �� ���� �������� ������ ���� �������� �Ѿ��.

	XMotionTrigger	m_PostTrigger;
public:
	XMotionAniSequence();
	void SetAniName(const TCHAR* szAniName, bool bWeaponAni);
	bool IsExistEvent()			{ return bExistEvent; }
	const TCHAR* GetEventName()	{ return szEvent; }
	const TCHAR* GetAniName() { return m_szAniName; }
	const TCHAR* GetAniName(WEAPON_TYPE nWeaponType);
	int GetFrameOver() const { return m_nFrameOver; }
	float GetSpeed() const		{ return m_fSpeed; }
	float GetTimeOut() const	{ return m_fTimeOut; }
	XMotionTrigger* GetPostTrigger() { return &m_PostTrigger; }
};

class XBaseMotionEventLister
{
public:
	XBaseMotionEventLister() {}
	virtual ~XBaseMotionEventLister() {}
};

/// ��� - �ִϸ��̼��� ���� ����(�� ������ �̷�� �ִϸ��̼��� ����)
class XMotion
{
	friend class XMotionMgr;
public:
	enum XMotionStance
	{
		MS_NORMAL = 0,	// normal ���Ľ���
		MS_BATTLE,		// combat ���Ľ���
		MS_ALL,			// ��� ���Ľ� 
		MS_MAX
	};
private:
	tstring							m_strName;
	XMotionStance					m_nStanceType;

	bool							m_bLoop;
	bool							m_bCancelEnabled;		///< ĵ�� ���� ����
//	bool							m_bCameraLook;			///< �̰��� true�̸� �� ������϶��� ������ ī�޶� �������� ��ĳ���� ������ �����Ѵ�.

	bool							m_bCameraLook[MS_MAX];	///< �̰��� true�̸� �� ������϶��� ������ ī�޶� �������� ��ĳ���� ������ �����Ѵ�.

	bool							m_bCustomAni;			///< ��������� ���۾��ϰ� �ٸ� ������� �ִϸ��̼� �Ǵ� ���
	bool							m_bWeaponAni;			//< ������� �ִϸ��̼��� �Ұ��ΰ�?

	bool							m_bExistTrigger;		///< Ʈ���Ű� �ִ��� ����

	bool							m_bAddPostfix;			///< �ִϸ��̼� �̸��� ��Ȳ�� ���� ���̾ ������ ����
	bool							m_bAutoPrefix;			///< �ִϸ��̼� �̸��� ��Ȳ�� ���� ���ξ� ������ ����

	void CheckTrigger();
	void SetCameraLook(bool b);

	XBaseMotionEventLister*			m_pEventLister;
public:
	vector<XMotionAniSequence*>		m_vecAniSequence[MT_COUNT];

	XMotion();
	virtual ~XMotion();
	void Cook();
	tstring& GetName()				{ return m_strName; }

	XMotionStance GetStanceType()	{ return m_nStanceType; }
	XMotionAniSequence*	GetAniSequence(XMotionTypes nMotionType, int index);
	XBaseMotionEventLister* GetEventListener() { return m_pEventLister; }
	void SetEventListener(XBaseMotionEventLister* pListener) { m_pEventLister = pListener; }

	bool IsCameraLook()									{ return m_bCameraLook[MS_ALL]; }
	bool IsCameraLook(XMotionStance nStance)			{ return m_bCameraLook[nStance]; }
	bool IsCancelEnabled()			{ return m_bCancelEnabled; }
	bool IsLoop()					{ return m_bLoop; }
	bool IsCustom()					{ return m_bCustomAni; }
	bool IsWeaponAni()				{ return m_bWeaponAni; }
	bool IsExistTrigger()			{ return m_bExistTrigger; }
	bool IsAddPostfix()				{ return m_bAddPostfix; }
	bool IsAutoPrefix()				{ return m_bAutoPrefix; }
};

