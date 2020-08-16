#pragma once
#include "RSceneNode.h"

// ���ִϸ��̼��� instance

namespace rs3 {

enum RBIPID
{
	RBIPID_UNKNOWN = -1,
	RBIPID_ROOT,
	RBIPID_PELVIS,
	  RBIPID_SPINE ,
		RBIPID_LTHIGH,
		  RBIPID_LCALF,
			RBIPID_LFOOT ,
			  RBIPID_LTOE0 ,
		RBIPID_RTHIGH,
		  RBIPID_RCALF,
			RBIPID_RFOOT ,
			  RBIPID_RTOE0 ,
		RBIPID_SPINE1 ,
		  RBIPID_SPINE2 ,
			RBIPID_NECK ,
			  RBIPID_HEAD,
			  RBIPID_LCLAVICLE,
				RBIPID_LUPPERARM ,
				  RBIPID_LFOREARM ,
					RBIPID_LHAND,
			  RBIPID_RCLAVICLE,
				RBIPID_RUPPERARM ,
				  RBIPID_RFOREARM ,
					RBIPID_RHAND,
	// �޼հ��� parent = RBIPID_LHAND
					RBIPID_LFINGER0,
					  RBIPID_LFINGER01,
						RBIPID_LFINGER02,
					RBIPID_LFINGER1,
					  RBIPID_LFINGER11,
						RBIPID_LFINGER12,
					RBIPID_LFINGER2,
					  RBIPID_LFINGER21,
						RBIPID_LFINGER22,
	// �����հ��� parent = RBIPID_RHAND
					RBIPID_RFINGER0,
					  RBIPID_RFINGER01,
						RBIPID_RFINGER02,
					RBIPID_RFINGER1,
					  RBIPID_RFINGER11,
						RBIPID_RFINGER12,
					RBIPID_RFINGER2,
					  RBIPID_RFINGER21,
						RBIPID_RFINGER22,
	
	// Ponytail parent = RBIPID_HEAD
	RBIPID_PONYTAIL1,
	  RBIPID_PONYTAIL11,
	RBIPID_PONYTAIL2,
	  RBIPID_PONYTAIL21,

	// ����
	RBIPID_TAIL,
	RBIPID_TAIL1,
	RBIPID_TAIL2,
	RBIPID_TAIL3,
	
	// ��, ��Ģ ����� �ʿ�
	RBIPID_EYE,
	RBIPID_LIP,

	// ����
	RBIPID_PROP1,

	RBIPID_END
};

class RMesh;
class RAnimation;
class RAnimationNode;
class RSkeletonController;
class RBlendAnimationController;
class RAnimationFrameInfo;

typedef list<RSkeletonController*> RSKELETONCONTROLLERLIST;

//	class RSkeletonBone �� �ִ°� ������ ?
class RS_API RSkeleton : public RSceneNode
{
	/*
	friend RSkeletonController;

protected:
	RANIMATIONTRANSITIONTYPE	m_transitionType;
//	RBlendAnimationController*	m_pTransitionController;	// �ִϸ��̼��� ��ȯ�� ���� ���� ��Ʈ�ѷ��� �׻� �ϳ� �������ִ�.

	RAnimation*	m_pAnimation;
	RAnimationNode*	m_pAnimationNodeTable[RBIPID_END];
	RSKELETONCONTROLLERLIST m_controllers;

	int	m_nFrame;
	int m_nMaxFrame;

	bool		m_bUse[RBIPID_END];				// ����ϴ��� ����
	RMatrix		m_matLocal[RBIPID_END];			// �θ���� ��ǥ�迡�� �ڱ���� ��ǥ����� ��ȯ. ���� �⺻ ��ġ�� ������ ����ִ�
	RQuaternion	m_localRotation[RBIPID_END];	// m_matLocal �� ȸ������
	RVector		m_localPosition[RBIPID_END];	// m_matLocal �� �̵�����
	RMatrix		m_matBase[RBIPID_END];			// local��ǥ�迡�� max static frame���� ���� ��ȯ ( = �θ�κ��� �������� local )
	RMatrix		m_matInvBase[RBIPID_END];		// base�� inverse

	// �ð����� ���ϴ� ���� ���� ��ȯ��ĵ�
	RQuaternion	m_currentRotation[RBIPID_END];	// ������ ȸ����
	RVector		m_currentPosition[RBIPID_END];	// ������ �̵���
	RMatrix		m_matCurrent[RBIPID_END];		// ���� �������� ��ȯ	= m_quatRotation,m_vecPosition �� ����� ���
	RMatrix		m_matResult[RBIPID_END];		// parent�κ��� ������ ������ ������ȯ���

public:
	RSkeleton(void);
	~RSkeleton(void);

	void OnUpdate();
	void OnRender();

	void CreateBone(RMesh* pMesh);
	void SetAnimation(RAnimation* pAnimation);
	void SetAnimation(RAnimation* pAnimation, RAnimationFrameInfo& frameInfo, RANIMATIONTRANSITIONTYPE transType = RAT_NONE);

	void UpdateBone();
	void UpdateBoneFinal();
*/
	RSkeleton(void) {}
public:
	static RBIPID GetBipID(const char* szName);
	static RBIPID GetBipParent(RBIPID id);
	static const char* GetBipName(RBIPID id);
	static float GetBaseBoneImportance(RBIPID id);

/*

	void AddController(RSkeletonController* pController);
	bool RemoveController(RSkeletonController* pController);

	const RMatrix& GetBaseTransform(RBIPID bipID);
	const RMatrix& GetResultTransform(RBIPID bipID);

	const RMatrix& GetCurrentTransform(RBIPID bipID);
	const RQuaternion& GetCurrentRotation(RBIPID bipID);
	const RVector&	GetCurrentPosition(RBIPID bipID);

	const RMatrix& GetLocalTransform(RBIPID bipID);
	const RQuaternion& GetLocalRotation(RBIPID bipID);
	const RVector&	GetLocalPosition(RBIPID bipID);

// �ӽú�����
	DWORD		m_dwBoneColor[RBIPID_END];	// ����׿� �������Ҷ� ����

	RANIMATIONTRANSITIONTYPE GetAnimationTransitionStatus();
*/
};

/*
//////////////////////////////////////////////////////

inline const RMatrix& RSkeleton::GetBaseTransform(RBIPID bipID) { return m_matBase[bipID]; }
inline const RMatrix& RSkeleton::GetResultTransform(RBIPID bipID) { return m_matResult[bipID]; }

inline const RMatrix& RSkeleton::GetCurrentTransform(RBIPID bipID) { return m_matCurrent[bipID]; }
inline const RQuaternion& RSkeleton::GetCurrentRotation(RBIPID bipID) { return m_currentRotation[bipID]; }
inline const RVector& RSkeleton::GetCurrentPosition(RBIPID bipID) { return m_currentPosition[bipID]; }

inline const RMatrix& RSkeleton::GetLocalTransform(RBIPID bipID) { return m_matLocal[bipID]; }
inline const RQuaternion& RSkeleton::GetLocalRotation(RBIPID bipID) { return m_localRotation[bipID]; }
inline const RVector& RSkeleton::GetLocalPosition(RBIPID bipID) { return m_localPosition[bipID]; }
*/

}
