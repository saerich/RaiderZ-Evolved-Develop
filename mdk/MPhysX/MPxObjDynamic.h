#pragma once

#include "MPhysXObject.h"

namespace physx {

//////////////////////////////////////////////////////////////////////////
//
// MPxObjDynamic
//  - Dynamic Object (������ SubActors�� ������ ������Ʈ)
//  - Dynamic NxActor�� �����ϸ� ��� �浹 �� ���� �ùķ��̼���
//    MPhysX�� m_pScene�� �˾Ƽ� ó���Ѵ�.
//
//////////////////////////////////////////////////////////////////////////

class MPhysXCollisionReport;
class MPxObjDynamic : public MPhysXObject
{
public:
	MPxObjDynamic();
	virtual ~MPxObjDynamic();

	//////////////////////////////////////////////////////////////////////////
	// Dynamic Object Entity
	//
	class DynamicObjectEntity
	{
	public:

		// Collision Group
		int						GetCollisionGroup(){ return m_nCollisionGroup; }
		void					SetCollisionGroup(int _nCollisionGroup){ m_nCollisionGroup = _nCollisionGroup; }

	protected:
								DynamicObjectEntity(){ m_pOwnerDynamicObject = NULL; m_nCollisionGroup = 0; }
		virtual					~DynamicObjectEntity(){ _ASSERT(m_pOwnerDynamicObject == NULL); }

		// ������ �ʿ��� ����
		virtual int				GetSubActorCount()	= 0;
		virtual bool			GetSubActorInfo(int _nIndex, std::string& _rShapeName, std::vector<NxVec3>& _refVertices, std::vector<NxU32>& _refFaces, MMatrix& _rMatLocal, bool& _rIsKinematic) = 0;

		// �����ÿ� (SubActor ������ ����)
		virtual void			OnSubActorAdded( int _nSubActorIndex, MSUBACTORHANDLE _hAddedSubActorHandle ) = 0;
		virtual void			OnActivatedSubActorsAll(MMatrix& _rOutEntityTM) = 0;
		virtual void			OnDeActivatedSubActorsAll() = 0;

		// SubActor �ùķ��̼� ����
		virtual void			OnSubActorSimulated( NxActor* pNxActor ) = 0;
		virtual void			OnSubActorCollisionStarted(NxActor* pNxActor) = 0;


		// MPhysX ���� Ȱ��ȭ�� ���� AreaBox
		virtual const MBox&		GetAreaBox() = 0;

		// �ùķ���Ʈ �� �Ŀ�
		virtual void			OnSimulated() = 0;

		// �ڵ�� ���� NxActor ������
		NxActor* GetNxActorFromHandle( MSUBACTORHANDLE _hHandle )
		{
			if (NULL == m_pOwnerDynamicObject)
				return NULL;

			return m_pOwnerDynamicObject->GetNxActor(_hHandle);
		}

	private:
		friend					MPxObjDynamic;
		friend					MPhysXCollisionReport;
		MPxObjDynamic*			m_pOwnerDynamicObject;
		int						m_nCollisionGroup;
	};


	//////////////////////////////////////////////////////////////////////////
	// Dynamic Object Public Method
	//
	void						RegisterDynamicObjectEntity(DynamicObjectEntity* pEntity);
	void						UnRegisterDynamicObjectEntity();

	bool						AddSubActorFromEntity();
	void						ActivateSubActorsAll();
	void						DeActivateSubActorsAll();

	void						ApplyForce(const MVector3& _rForce, const MVector3& _rForceWorldPos);

	virtual void				NotifyResultOnSimulated();
	void						SetEnableObservingResultOnSimulated(bool bEnable){ m_bObservingResultOnSimulated = bEnable; }

	virtual const MBox*			GetAreaBox();
	void						SetEnableUsingAreaBox(bool bEnalbe){ m_bUsingAreaBox = bEnalbe; }

protected:

	virtual void				OnDoPreUpdateSubActorTransform() {}
	virtual void				OnDoPostUpdateSubActorTransform(){}

	bool						m_bActive;
	DynamicObjectEntity*		m_pDynamicObjectEntity;
	MBox						m_aabb;

	bool						m_bObservingResultOnSimulated;
	bool						m_bUsingAreaBox;
};

} // namespace physx
