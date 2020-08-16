#pragma once

#include "MCollision.h"
#include "RTypes.h"
#include "RSceneNode.h"

namespace rs3 {
class RSceneNode;
class RActor;

class RS_API RActorCollisionObject
{
public:
		RActorCollisionObject();

		MCollision::MCollisionType		m_Type;
		MCollision::MCollisionMeshType	m_MeshType;

		RCapsule						m_Capsule;
		RSphere							m_Sphere;
		RBox							m_Box;

		RSceneNode*						m_pParent;
		RMatrix							m_matTransform;

		string							m_strRelatedNodeNames;	// �浹�� �����ִ� ���ͳ�� �̸�
};


class RS_API RActorCollision : public RSceneNode
{
public:	
		RActorCollision();
		virtual ~RActorCollision();

		vector<RActorCollisionObject>	m_vCollisions;

		void							RenderCollisionObject(MCollision::MCollisionMeshType eType);
		void							RenderCollisionObject();

		virtual void					Add(MCollision* pCollision, RSceneNode* pParent);
		virtual void					Add(MCollision::MCollisionType eCollisionType, MCollision::MCollisionMeshType eMeshType, float fRad, const RVector& b, const RVector& t, const RMatrix& tm,RSceneNode* pParent);
		virtual void					Remove( MCollision* pCollision, RSceneNode* pParent );

		virtual void					Build(RActor* pActor) { return; };	// �ѹ��� �Ҹ������� �Ѵ�.

		virtual void					Update(RActor* pActor);

		virtual void					Destroy();
};

}