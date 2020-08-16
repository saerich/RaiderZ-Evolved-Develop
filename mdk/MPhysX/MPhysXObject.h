#pragma once

#include "MPhysXDef.h"

namespace physx {

// sub-actor Handle
typedef unsigned int MSUBACTORHANDLE;

class MPhysX;
class MPhysXShape;

// MPhysX Primitive Object
class MPhysXObject;
class MPxObjStatic;
class MPxObjDynamic;
class MPxObjRagDoll;
class MPxObjClothed;

//////////////////////////////////////////////////////////////////////////
//
// MPhysXObject
//  - MPhysXManager�� ���� ����
//  - Primitive Object�� Ȯ���ذ��� �������� ������Ʈ�� ������ Ȯ���ϵ��� ����
//
//////////////////////////////////////////////////////////////////////////

class MPhysXObject
{
	friend class MPhysX;

public:

	MPhysXObject();
	virtual ~MPhysXObject();


	//////////////////////////////////////////////////////////////////////////
	// Object Ÿ�� (PO - PhysX Object)
	//
	enum PHYSX_OBJECT_TYPE
	{
		PO_RIGID_STATIC = 0, //�浹 �׷� ������ ������ ���� STATIC_BODY�� enum���� 0���� �����صд�.
		PO_RIGID_DYNAMIC,
		PO_RIGID_RAGDOLL,

		PO_CLOTHED,

		PO_CUSTOM,
		PO_TOTAL_NUM
	};


	//////////////////////////////////////////////////////////////////////////
	// SubActor Ÿ�� (PS : PhysX SubActor)
	//
	enum PHYSX_SUBACTOR_TYPE
	{
		PS_CAPSULE = 0,
		PS_BOX,
		PS_PLANE,
		PS_SPHERE,

		PS_STATIC_TRIANGLE,
		PS_DYNAMIC_CONVEX,

		PS_TOTAL_NUM
	};


	//////////////////////////////////////////////////////////////////////////
	// Joint Ÿ�� (PJ : PhysX Joint), ������Ʈ ���� Shape ���� ���
	//
	enum PHYSX_JOINT_TYPE
	{
		PJ_SPHERICAL = 0,
		PJ_REVOLUTE,

		PJ_TOTAL_NUM
	};

	//////////////////////////////////////////////////////////////////////////
	// MPhysXObject Ÿ�� ����
	//
	inline PHYSX_OBJECT_TYPE	GetPxObjectType(){ return m_eObjectType; }

	
	//////////////////////////////////////////////////////////////////////////
	// MPhysXObject Update
	//
	virtual void				NotifyResultOnSimulated()	= 0;
	virtual const MBox*			GetAreaBox()	= 0;


	//////////////////////////////////////////////////////////////////////////
	// NxActor ���
	//
	NxActor*					GetNxActor(MSUBACTORHANDLE _hHandle){ return m_NxInfos.Get(_hHandle).m_pActor; }


	//////////////////////////////////////////////////////////////////////////
	// Activation & DeActivation
	//
	void						ActivateSubActor(MSUBACTORHANDLE _hSubActor);
	void						ActivateAllSubActors();
	void						DeActiveSubActor(MSUBACTORHANDLE _hSubActor);
	void						DeActiveAllSubActors();


	//////////////////////////////////////////////////////////////////////////
	// Collision Group
	//
	bool						AddToCollisionGroup(const char* _pSzCollsionGroup);
	bool						RemoveFromCollisionGroup(const char* _pSzCollsionGroup);


protected:

	//////////////////////////////////////////////////////////////////////////
	// ������Ʈ Ÿ��
	//
	template< typename T > struct PxType2Id
	{
		static const int	INDEX = PO_CUSTOM;
		static const char*	GetString(){ static const char* pSz = "CUSTOM"; return pSz; }
	};
	template<> struct PxType2Id<MPxObjStatic>
	{
		static const int	INDEX = PO_RIGID_STATIC;
		static const char*	GetString(){ static const char* pSz = "STATIC"; return pSz; }
	};
	template<> struct PxType2Id<MPxObjDynamic>
	{
		static const int	INDEX = PO_RIGID_DYNAMIC;
		static const char*	GetString(){ static const char* pSz = "DYNAMIC"; return pSz; }
	};
	template<> struct PxType2Id<MPxObjRagDoll>
	{
		static const int	INDEX = PO_RIGID_RAGDOLL;
		static const char*	GetString(){ static const char* pSz = "RAGDOLL"; return pSz; }
	};
	template<> struct PxType2Id<MPxObjClothed>
	{
		static const int	INDEX = PO_CLOTHED;
		static const char*	GetString(){ static const char* pSz = "CLOTH"; return pSz; }
	};


	//////////////////////////////////////////////////////////////////////////
	// MPhysXObject �⺻�ɹ�
	//
	PHYSX_OBJECT_TYPE			m_eObjectType;
	MPhysX*						m_pPhysX;

	MMatrix						m_matWorldTransform;
	bool						m_bNeedToUpdateTransform;
	std::set<std::string>		m_setCollsionGroups;
	virtual int					OnRefreshCollisionGroup();


	//////////////////////////////////////////////////////////////////////////
	//
	// Do �迭�� �Լ�. MPhysXObject�� ��ӹ��� ���� ������ Ȱ���Ͽ�
	// AddBody�� GetWorldTransform ���� �Լ��� ����� ����Ѵ�.
	//
	//////////////////////////////////////////////////////////////////////////

	// SubActor Creation
	MSUBACTORHANDLE				DoAddSubActorPlane(const MPlane& _rLocalPlane, float _fDensity);
	MSUBACTORHANDLE				DoAddSubActorBox(const MBox& _rLocalBox, float _fDensity);
	MSUBACTORHANDLE				DoAddSubActorCapsule(const MCapsule& _rLocalCapsule, float _fDensity);
	MSUBACTORHANDLE				DoAddSubActorSphere(const MSphere& _rLocalSphere, float _fDensity);
	MSUBACTORHANDLE				DoAddSubActorSphereCCD(const MSphere& _rLocalSphere, float _fDensity);

	// Transformation
	inline const MMatrix&		DoGetWorldTransform();
	inline void					DoSetWorldTransform( const MMatrix& _rWorldTransform);
	inline void					DoSetSubActorTransform( MSUBACTORHANDLE _hShape, const MMatrix& _rLocalTransform );
	void						DoUpdateTransform();

	// Do Transform �迭�� ����Ѵٸ� ������ ������ �־�� ��
	virtual void				OnDoPreUpdateSubActorTransform()  = 0;
	virtual void				OnDoPostUpdateSubActorTransform() = 0;


	//////////////////////////////////////////////////////////////////////////
	//
	// Nx�迭 ���� ��� InfoŬ����
	// MPhysXObject�� NxActor�� NxBodyDesc���� ������ NX_INFO�� Shape���� ������
	//
	//////////////////////////////////////////////////////////////////////////

	struct NX_INFO
	{
		NX_INFO() : m_pActor(NULL), m_pPhysXShape(NULL)	{}

		NX_INFO( PHYSX_SUBACTOR_TYPE _eSubActorType, const MMatrix& _rTransform, float _fDensity, MPhysXShape* _pPxShape )
		: m_eSubActor(_eSubActorType)
		, m_matLocalTransform(_rTransform)
		, m_fDensity(_fDensity)
		, m_pActor(NULL)
		, m_pPhysXShape(_pPxShape)
		{
		}

		PHYSX_SUBACTOR_TYPE		m_eSubActor;
		MMatrix					m_matLocalTransform;
		float					m_fDensity;	///< �е�
		NxActor*				m_pActor;

		NxBodyDesc				m_BodyDesc;
		MPhysXShape*			m_pPhysXShape;
	};

	MHandle<NX_INFO>			m_NxInfos;

	// NX_INFO ����
	void						CreateNxActorByNxInfo( NX_INFO& _rNxInfo, bool bWaitForAllCreation );
	void						ClearNxInfoAndNxActor(NX_INFO& _rNxInfo, bool bDeleteDesc );

	// Nx�迭 ��ü
	NxScene*					GetNxScene();

	NxActor*					GetNxActorBelongToSubActor(MSUBACTORHANDLE _hSubActor);
	void						ReserveToReleaseNxActor( NxActor* _pActor );

	static float				NX_KINEMATIC_DENSITY(){ return -1.f; }
	static float				NX_STATIC_DENSITY(){ return 0.f; }
};
//----------------------------------------------------------------------------------------------------
inline const MMatrix& MPhysXObject::DoGetWorldTransform()
{
	return m_matWorldTransform;
}
//----------------------------------------------------------------------------------------------------
inline void MPhysXObject::DoSetWorldTransform( const MMatrix& _rWorldTransform)
{
	m_bNeedToUpdateTransform = true;
	m_matWorldTransform = _rWorldTransform;
}
//----------------------------------------------------------------------------------------------------
inline void MPhysXObject::DoSetSubActorTransform( MSUBACTORHANDLE _hShape, const MMatrix& _rLocalTransform )
{
	m_bNeedToUpdateTransform = true;

	NX_INFO& rInfo = m_NxInfos.Get(_hShape);
	rInfo.m_matLocalTransform = _rLocalTransform;
}
//----------------------------------------------------------------------------------------------------
} // namespace physx