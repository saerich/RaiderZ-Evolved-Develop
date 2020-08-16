#include "StdAfx.h"

#pragma push_macro("min")
#pragma push_macro("max")
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include "NxPhysics.h"

#include "NxController.h"
#include "NxControllerManager.h"

#pragma pop_macro("min")
#pragma pop_macro("max")

#include "RActor.h"
#include "RPhysXRagDollBuilder.h"

#include "REngine.h"
#include "RSceneManager.h"
#include "RDebugPassRenderer.h"
#include "RMeshNode.h"

namespace rs3 {

//////////////////////////////////////////////////////////////////////////
//
// RagDoll Factor
//
//////////////////////////////////////////////////////////////////////////

bool RPhysXRagDollFactor::LoadRagDollFactorFromContext( MXmlElement* pRagDollElement, const char* pSzContextName )
{
	_ASSERT(pRagDollElement != NULL);

	bool bRet = false;
	std::string strWantedName(pSzContextName);
	MXmlElement* pContextElement = pRagDollElement->FirstChildElement(RAG_ELEMENT_CONTEXT);
	while ( pContextElement )
	{
		if ( strWantedName == pContextElement->Attribute(RAG_ATTR_CONTEXT) )
		{
			LoadRagDollFactorFromContext(pContextElement);
			bRet = true;
			break;
		}
		pContextElement = pContextElement->NextSiblingElement(RAG_ELEMENT_CONTEXT);
	}

	return bRet;
}

void RPhysXRagDollFactor::LoadRagDollFactorFromContext( MXmlElement* pContextElement )
{
	// move or set func
	std::string strProperty;
	_Contents(strProperty, pContextElement->FirstChildElement("UsingMoveFunc"));
	if (strProperty == "true")
	{
		bUsingMoveFunc = true;
	}

	// density
	_Contents(&fDensity, pContextElement->FirstChildElement("Density"));

	// elbow
	MXmlElement* pElement = pContextElement->FirstChildElement("Elbow");
	_Attribute(&fElbowLimitRatio, pElement, "limit_ratio");
	_Attribute(&fElbowSpring, pElement, "spring");
	_Attribute(&fElbowDamping, pElement, "damping");

	// knee
	pElement = pContextElement->FirstChildElement("Knee");
	_Attribute(&fKneeLimitRatio, pElement, "limit_ratio");
	_Attribute(&fKneeSpring, pElement, "spring");
	_Attribute(&fKneeDamping, pElement, "damping");

	// shoulder
	pElement = pContextElement->FirstChildElement("Shoulder");
	_Attribute(&fShoulderLimitRatio, pElement, "limit_ratio");
	_Attribute(&fShoulderSpring, pElement, "spring");
	_Attribute(&fShoulderDamping, pElement, "damping");
	_Attribute(&fShoulderTwistSpring, pElement, "twist_spring");
	_Attribute(&fShoulderTwistDamping, pElement, "twist_damping");
	_Attribute(&fShoulderSwingSpring, pElement, "swing_spring");
	_Attribute(&fShoulderSwingDamping, pElement, "swing_damping");

	// Hip
	pElement = pContextElement->FirstChildElement("Hip");
	_Attribute(&fHipLimitRatio, pElement, "limit_ratio");
	_Attribute(&fHipSpring, pElement, "spring");
	_Attribute(&fHipDamping, pElement, "damping");
	_Attribute(&fHipTwistSpring, pElement, "twist_spring");
	_Attribute(&fHipTwistDamping, pElement, "twist_damping");
	_Attribute(&fHipSwingSpring, pElement, "swing_spring");
	_Attribute(&fHipSwingDamping, pElement, "swing_damping");

	// Vert
	pElement = pContextElement->FirstChildElement("Vert");
	_Attribute(&fVertLimitRatio, pElement, "limit_ratio");
	_Attribute(&fVertSpring, pElement, "spring");
	_Attribute(&fVertDamping, pElement, "damping");
	_Attribute(&fVertTwistSpring, pElement, "twist_spring");
	_Attribute(&fVertTwistDamping, pElement, "twist_damping");
	_Attribute(&fVertSwingSpring, pElement, "swing_spring");
	_Attribute(&fVertSwingDamping, pElement, "swing_damping");
}

void RPhysXRagDollFactor::ChangeRagDollControllerFromFactor( RPhysXActorController* pController )
{
	if (NULL == pController)
		return;

	NxSphericalJointDesc spDesc;
	NxSphericalJoint* pSpJoint = NULL;

	#define _SET_SP_JOINT( _id, _preFix, _limitRatio )	{			\
		pSpJoint = dynamic_cast<NxSphericalJoint*>(pController->GetNxJointFromResourceMap(RPhysXRagDollBuilder::##_id##));	\
		if (pSpJoint) {												\
			pSpJoint->saveToDesc(spDesc);							\
			spDesc.jointSpring.spring = f##_preFix##Spring;			\
			spDesc.jointSpring.damper = f##_preFix##Damping;		\
			spDesc.swingSpring.spring = f##_preFix##SwingSpring;	\
			spDesc.swingSpring.damper = f##_preFix##SwingDamping;	\
			spDesc.twistSpring.spring = f##_preFix##TwistSpring;	\
			spDesc.twistSpring.damper = f##_preFix##TwistDamping;	\
			spDesc.swingLimit.value = NxPi * _limitRatio;			\
			pSpJoint->loadFromDesc(spDesc);							\
		}\
	}

	// ô�߰迭
	_SET_SP_JOINT(E_VERT_1, Vert, fVertLimitRatio);
	_SET_SP_JOINT(E_VERT_2, Vert, fVertLimitRatio);

	// ��� / ������
	_SET_SP_JOINT(E_LEFT_SHOULDER, Shoulder, fShoulderLimitRatio);
	_SET_SP_JOINT(E_RIGHT_SHOULDER, Shoulder, fShoulderLimitRatio);
	_SET_SP_JOINT(E_LEFT_HIP, Hip, fHipLimitRatio );
	_SET_SP_JOINT(E_RIGHT_HIP, Hip, fHipLimitRatio );


	NxRevoluteJointDesc rvDesc;
	NxRevoluteJoint* pRvJoint = NULL;

	#define _SET_RV_JOINT( _id, _preFix )	{						\
		pRvJoint = dynamic_cast<NxRevoluteJoint*>(pController->GetNxJointFromResourceMap(RPhysXRagDollBuilder::##_id##));	\
		if (pRvJoint) {												\
			pRvJoint->saveToDesc(rvDesc);							\
			rvDesc.spring.spring = f##_preFix##Spring;				\
			rvDesc.spring.damper = f##_preFix##Damping;				\
			if(rvDesc.limit.low.value < 0 ){rvDesc.limit.low.value = -NxPi * f##_preFix##LimitRatio;}	\
			else{ rvDesc.limit.high.value = NxPi * f##_preFix##LimitRatio; }							\
			pRvJoint->loadFromDesc(rvDesc);							\
		}\
	}

	// �Ȳ�ġ / ����
	_SET_RV_JOINT(E_LEFT_ELBOW, Elbow);
	_SET_RV_JOINT(E_RIGHT_ELBOW, Elbow);
	_SET_RV_JOINT(E_LEFT_KNEE, Knee);
	_SET_RV_JOINT(E_RIGHT_KNEE, Knee);

	// Density ����
	pController->SetResourceNxActorDensity(fDensity);

	// move func ����
	pController->SetEnableMoveFuncInKinematic(bUsingMoveFunc);
}

//////////////////////////////////////////////////////////////////////////
//
// Custom Controller
//
//////////////////////////////////////////////////////////////////////////
class RPhysXAnkleRightController : public RPhysXActorNodeCustomController
{
public:
	RPhysXAnkleRightController(){}
	virtual ~RPhysXAnkleRightController(){}

	virtual void OnCustomControl(RActor* pActor, NxActor* pNxActor, NxJoint* pNxJoint, NxMat34& rResultMat)
	{
		NxU32 nShapes = pNxActor->getNbShapes();
		_ASSERT(nShapes == 1);

		NxShape*const* shapes = pNxActor->getShapes();
		float fHeight = static_cast<NxCapsuleShape*>( shapes[0] )->getHeight();
		NxMat33 matPose = pNxActor->getGlobalOrientation();

		NxVec3 vNxUp = matPose.getColumn(0);
		RVector vUp(vNxUp.x, vNxUp.y, vNxUp.z);
		vUp.Normalize();
		vUp = vUp * fHeight;
		vNxUp.set(vUp.x, vUp.y, vUp.z);

		NxMat33 matRotX;
		matRotX.rotX(NxPi*0.5f);
		RActorNode* pActorNodeTwist = pActor->GetActorNode("Bip01 R ForeTwist");
		if ( NULL != pActorNodeTwist )
		{
			RVector vScale;
			RVector vPos;
			RQuaternion qOrientForeArm;
			RMatrix matT;

			RActorNode* pActorNodeForeArm = pActor->GetActorNode("Bip01 R Forearm");
			matT = pActorNodeForeArm->GetLocalTransform();
			matT.Decompose(vScale, vPos, qOrientForeArm);

			pActorNodeTwist->SetRotation( qOrientForeArm );
		}

		rResultMat.M.multiply(matPose, matRotX);
		rResultMat.t = pNxActor->getGlobalPosition() + vNxUp;
	}
};

class RPhysXAnkleLeftController : public RPhysXActorNodeCustomController
{
public:
	RPhysXAnkleLeftController(){}
	virtual ~RPhysXAnkleLeftController(){}

	virtual void OnCustomControl(RActor* pActor, NxActor* pNxActor, NxJoint* pNxJoint, NxMat34& rResultMat)
	{
		NxU32 nShapes = pNxActor->getNbShapes();
		_ASSERT(nShapes == 1);

		NxShape*const* shapes = pNxActor->getShapes();
		float fHeight = static_cast<NxCapsuleShape*>( shapes[0] )->getHeight();
		NxMat33 matPose = pNxActor->getGlobalOrientation();

		NxVec3 vNxUp = matPose.getColumn(0);
		RVector vUp(vNxUp.x, vNxUp.y, vNxUp.z);
		vUp.Normalize();
		vUp = vUp * fHeight;
		vNxUp.set(vUp.x, vUp.y, vUp.z);

		NxMat33 matRotX;
		matRotX.rotX(NxPi*(-0.5f));
		RActorNode* pActorNodeTwist = pActor->GetActorNode("Bip01 L ForeTwist");
		if ( NULL != pActorNodeTwist )
		{
			RVector vScale;
			RVector vPos;
			RQuaternion qOrientForeArm;
			RMatrix matT;

			RActorNode* pActorNodeForeArm = pActor->GetActorNode("Bip01 L Forearm");
			matT = pActorNodeForeArm->GetLocalTransform();
			matT.Decompose(vScale, vPos, qOrientForeArm);

			pActorNodeTwist->SetRotation( qOrientForeArm );
		}

		rResultMat.M.multiply(matPose, matRotX);
		rResultMat.t = pNxActor->getGlobalPosition() + vNxUp;
	}
};

//////////////////////////////////////////////////////////////////////////
//
// Controller Builder for RagDoll
//
//////////////////////////////////////////////////////////////////////////


NxSphericalJoint* CreateBodySphericalJoint( NxScene* scene, NxActor* a0, NxActor* a1, const NxVec3& globalAnchor, const NxVec3& globalAxis, float fSwingLimit)
{
	NxSphericalJointDesc sphericalDesc;
	sphericalDesc.actor[0] = a0;
	sphericalDesc.actor[1] = a1;
	sphericalDesc.setGlobalAnchor(globalAnchor);
	sphericalDesc.setGlobalAxis(globalAxis);

	sphericalDesc.flags |= NX_SJF_TWIST_LIMIT_ENABLED;
	sphericalDesc.twistLimit.low.value = -(NxReal)0.025*NxPi;
	sphericalDesc.twistLimit.low.hardness = 0.5;
	sphericalDesc.twistLimit.low.restitution = 0.5;
	sphericalDesc.twistLimit.high.value = (NxReal)0.025*NxPi;
	sphericalDesc.twistLimit.high.hardness = 0.5;
	sphericalDesc.twistLimit.high.restitution = 0.5;

	sphericalDesc.flags |= NX_SJF_SWING_LIMIT_ENABLED;
	//sphericalDesc.swingLimit.value = (NxReal)0.25*NxPi;
	sphericalDesc.swingLimit.value = (NxReal)fSwingLimit*NxPi;
	sphericalDesc.swingLimit.hardness = 0.5;
	sphericalDesc.swingLimit.restitution = 0.5;

	sphericalDesc.flags |= NX_SJF_TWIST_SPRING_ENABLED;
	//sphericalDesc.twistSpring.spring = 0.5;
	//sphericalDesc.twistSpring.damper = 1;

	sphericalDesc.twistSpring.spring = 100;
	sphericalDesc.twistSpring.damper = 500;

	sphericalDesc.flags |= NX_SJF_SWING_SPRING_ENABLED;
	//sphericalDesc.swingSpring.spring = 0.5;
	//sphericalDesc.swingSpring.damper = 1;

	sphericalDesc.swingSpring.spring = 100;
	sphericalDesc.swingSpring.damper = 500;

	sphericalDesc.flags |= NX_SJF_JOINT_SPRING_ENABLED;
	sphericalDesc.jointSpring.spring = 1000;
	sphericalDesc.jointSpring.damper = 5000;

	//// ���� �⺻ ��
	//sphericalDesc.projectionDistance = (NxReal)0.15;
	//sphericalDesc.projectionMode = NX_JPM_POINT_MINDIST;

	//sphericalDesc.projectionDistance = 1.f;
	//sphericalDesc.projectionDistance = 0.5f;
	sphericalDesc.projectionDistance = 0.7f;
	sphericalDesc.projectionMode = NX_JPM_POINT_MINDIST;

	return (NxSphericalJoint*)scene->createJoint(sphericalDesc);
}

NxRevoluteJoint* CreateRevoluteJoint( NxScene* scene, NxActor* a0, NxActor* a1, const NxVec3& globalAnchor, const NxVec3& globalAxis, bool bBottomLimit, float fJointLimit )
{
	NxRevoluteJointDesc revDesc;
	revDesc.actor[0] = a0;
	revDesc.actor[1] = a1;
	revDesc.setGlobalAnchor(globalAnchor);
	revDesc.setGlobalAxis(globalAxis);

	revDesc.flags |= NX_RJF_LIMIT_ENABLED;
	if (bBottomLimit)
	{
		revDesc.limit.high.value = 0;
		revDesc.limit.low.value = -NxPi * fJointLimit;
	}
	else
	{
		revDesc.limit.high.value = NxPi * fJointLimit;
		revDesc.limit.low.value = 0;
	}

	revDesc.flags |= NX_RJF_SPRING_ENABLED;
	revDesc.spring.spring = 1;
	revDesc.spring.damper = 5;

	//revDesc.projectionDistance = 1.f;
	//revDesc.projectionDistance = 0.5f;
	revDesc.projectionDistance = 0.7f;
	revDesc.projectionMode = NX_JPM_POINT_MINDIST;

	return (NxRevoluteJoint*)scene->createJoint(revDesc);
}

NxActor* CreateCapsuleBone( NxScene* scene, const NxVec3& actorpos, const NxReal height, const NxReal radius, const NxReal density)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	bodyDesc.solverIterationCount = 12;

	// The actor has one shape, a capsule
	NxCapsuleShapeDesc capsuleDesc;
	capsuleDesc.height = height;
	capsuleDesc.radius = radius;

	//capsuleDesc.localPose.t.set(0, 0, -height/2);
	// �����ִ� ĸ�� �����ֱ�
	NxMat33 matRot;
	matRot.rotZ(NxPi*0.5f);
	capsuleDesc.localPose.M = matRot;

	actorDesc.shapes.pushBack(&capsuleDesc);

	if (density)
	{
		actorDesc.body = &bodyDesc;
		actorDesc.density = density;
	}
	else
	{
		actorDesc.body = NULL;
	}
	actorDesc.globalPose.t = actorpos;
	return scene->createActor(actorDesc);
}

NxActor* CreateSphereBone( NxScene* scene, const NxVec3& pos, const NxMat33& rot, const NxReal radius, const NxReal density )
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	bodyDesc.solverIterationCount = 12;

	// The actor has one shape, a sphere
	NxSphereShapeDesc sphereDesc;
	sphereDesc.radius = radius;
	//sphereDesc.localPose.M = rot;
	actorDesc.shapes.pushBack(&sphereDesc);

	actorDesc.body = &bodyDesc;
	actorDesc.density = density;
	actorDesc.globalPose.t = pos;

	actorDesc.globalPose.M = rot;

	return scene->createActor(actorDesc);
}

RPhysXRagDollBuilder::RPhysXRagDollBuilder()
{
	m_fDensity = 1.f;
	m_fCapsuleRatio = 4.f;

	m_fKneeJointLimit = 0.7f;
	m_fElbowJointLimit = 0.3f;
	m_fHipJointLimit = 0.2f;
	m_fShoulderJointLimit = 0.25f;

	m_fSphereSize[E_SPINE2]	= 11.f;
	m_fSphereSize[E_SPINE1]	= 10.f;
	m_fSphereSize[E_PELVIS]	= 12.f;


	//////////////////////////////////////////////////////////////////////////
	// Sphere
	//
	// �Ӹ� / ���� / ���
	m_aryRefBip[E_SPINE2]			= RBIPID_SPINE2;
	m_aryRefBip[E_SPINE1]			= RBIPID_SPINE1;
	m_aryRefBip[E_PELVIS]			= RBIPID_PELVIS;


	//////////////////////////////////////////////////////////////////////////
	// Capsule, ĸ���� ���κ�, ��, �ڽ��� bip�� �������Ŀ� �缳�� ���ش�.
	//
	// ��� / �Ϲ�
	m_aryRefBip[E_LEFT_UPPERARM]	= RBIPID_LFOREARM;
	m_aryRefBip[E_RIGHT_UPPERARM]	= RBIPID_RFOREARM;

	m_aryRefBip[E_LEFT_FOREARM]		= RBIPID_LHAND;
	m_aryRefBip[E_RIGHT_FOREARM]	= RBIPID_RHAND;

	// ����� / ���Ƹ�
	m_aryRefBip[E_LEFT_THIGH]		= RBIPID_LCALF;
	m_aryRefBip[E_RIGHT_THIGH]		= RBIPID_RCALF;

	m_aryRefBip[E_LEFT_CALF]		= RBIPID_LFOOT;
	m_aryRefBip[E_RIGHT_CALF]		= RBIPID_RFOOT;
}

bool RPhysXRagDollBuilder::Build( RPhysXActorController* pPhysXController )
{
	bool bResult = true;

	// ���ҽ� ���忡 �ʿ��� ��ü��
	RActor* pActor = GetActor(pPhysXController);
	NxScene* pNxScene = GetNxScene(pPhysXController);
	_ASSERT(pActor);
	_ASSERT(pNxScene);

	// ���ҽ��� �����ϱ����� ������ �͵�
	//pActor->Pause();
	pActor->Stop();


	//////////////////////////////////////////////////////////////////////////
	//
	// Build NxActor
	//
	//////////////////////////////////////////////////////////////////////////

	RVector vPos, vScale;
	RQuaternion qOrient;
	NxQuat qNxOrient;
	float fScale = pActor->GetScale().x;

	NxActor* aryNxActor[E_BONE_NUM] = {0, };
	RBIPID aryBipd[E_BONE_NUM];

	for(int i=0;i<E_BONE_NUM;i++)
	{
		NxActor* pNxActor = NULL;
		RBIPID eBipd;
		DWORD dwAttr = 0;

		// Sphere bone --
		if (i >=0 && i <= E_PELVIS )
		{
			eBipd = m_aryRefBip[i];
			RActorNode* pActorNode = pActor->GetActorNode( RSkeleton::GetBipName(eBipd) );
			if (NULL == pActorNode)
				return false;

			const RMatrix& mat = pActorNode->GetWorldTransform();
			vPos.Set(mat._41, mat._42, mat._43);

			NxMat33 rot;
			rot(0, 0) = mat._11;
			rot(1, 0) = mat._12;
			rot(2, 0) = mat._13;

			rot(0, 1) = mat._21;
			rot(1, 1) = mat._22;
			rot(2, 1) = mat._23;

			rot(0, 2) = mat._31;
			rot(1, 2) = mat._32;
			rot(2, 2) = mat._33;

			pNxActor = CreateSphereBone(pNxScene, NxVec3(vPos.x, vPos.y, vPos.z), rot, m_fSphereSize[i] * fScale, m_fDensity);
		}

		// Capsule bone --
		else
		{
			//  NxActor ���� ����
			float fRadius = m_fCapsuleRatio * fScale;
			float fHeight = (GetCapsuleLengthFromChildBip( pActor, m_aryRefBip[i] ) - (fRadius * 2) ) * fScale;

			eBipd = RSkeleton::GetBipParent(m_aryRefBip[i]);	// �Ǹ��� Ÿ���� ��� ���̸� ������� �ڽ����� �����ִ�.
			const RMatrix& mat = pActor->GetActorNode( RSkeleton::GetBipName(eBipd) )->GetWorldTransform();
			mat.Decompose(vScale, vPos, qOrient);
			qNxOrient.setXYZW( qOrient.x, qOrient.y, qOrient.z, qOrient.w);

			RVector vOffSetForCapsule(mat._11, mat._12, mat._13);
			vOffSetForCapsule.Normalize();
			vOffSetForCapsule *= (fHeight/2 + fRadius);
			vPos += vOffSetForCapsule;

			// NxActor ����
			pNxActor = CreateCapsuleBone(pNxScene, NxVec3(vPos.x, vPos.y, vPos.z), fHeight, fRadius, m_fDensity);
			if ( NULL != pNxActor )
			{
				pNxActor->setGlobalOrientationQuat( qNxOrient );
			}
		}

		aryNxActor[i] = pNxActor;
		aryBipd[i] = eBipd;

		AddNxActorResource(pPhysXController, i, pNxActor);
		if (NULL == pNxActor)
		{
			bResult = false;
		}
		else
		{
			//pNxActor->setSleepLinearVelocity(1000);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	//
	// Build NxJoint
	//
	//////////////////////////////////////////////////////////////////////////

	RVector vActorNodePos;
	RVector vActorNodeUp;

	NxVec3 vNxPos;
	NxVec3 vNxUpAxis;

	NxJoint* aryJoint[E_JOINT_NUM] = {0, };
	for(int i=0;i<E_JOINT_NUM;i++)
	{
		NxJoint* pJoint = NULL;
		RMatrix matRot;

		int nUpActor = i;
		int nBottomActor = i + 1;

		float fSwingLimit = 0.25f; // ô�� swing limit ratio
		float fRevolueLimit = 1;

		bool bBottomLimit = true;
		if (i == E_LEFT_SHOULDER || i == E_RIGHT_SHOULDER)
		{
			nUpActor = E_SPINE2;
			fSwingLimit = m_fShoulderJointLimit;
		}
		else if ( i == E_LEFT_HIP || i == E_RIGHT_HIP )
		{
			nUpActor = E_PELVIS;
			nBottomActor = i + (E_LEFT_THIGH - E_LEFT_HIP);
			fSwingLimit = m_fHipJointLimit;
		}
		else if (i == E_LEFT_ELBOW || i == E_RIGHT_ELBOW)
		{
			nUpActor = i + ( E_LEFT_UPPERARM - E_LEFT_ELBOW );
			nBottomActor = nUpActor + 2;
			matRot.MakeIdentity();

			fRevolueLimit = m_fElbowJointLimit;

			if (E_RIGHT_ELBOW == i)
			{
				bBottomLimit = false;
			}
		}
		else if ( i == E_LEFT_KNEE || i == E_RIGHT_KNEE )
		{
			nUpActor = i + ( E_LEFT_THIGH - E_LEFT_KNEE );
			nBottomActor = nUpActor + 2;
			matRot.SetRotationZ(MMath::HALFPI);

			fRevolueLimit = m_fKneeJointLimit;
		}

		RBIPID eBip = aryBipd[nBottomActor];
		const RMatrix& mat = pActor->GetActorNode( RSkeleton::GetBipName(eBip) )->GetWorldTransform();
		vActorNodePos.Set(mat._41, mat._42, mat._43);

		// Spherical Joint
		if (i >=0 && i <= E_RIGHT_HIP )
		{
			vActorNodeUp.Set(mat._11, mat._12, mat._13);
			vActorNodeUp.Normalize();
			vActorNodePos += ( m_fSphereSize[nBottomActor] * vActorNodeUp );

			vNxUpAxis.set(-vActorNodeUp.x, -vActorNodeUp.y, -vActorNodeUp.z);
			vNxPos.set(vActorNodePos.x, vActorNodePos.y, vActorNodePos.z);

			pJoint = CreateBodySphericalJoint(pNxScene, aryNxActor[nUpActor], aryNxActor[nBottomActor], vNxPos, vNxUpAxis, fSwingLimit);
		}

		// revolute Joint, �Ȳ�ġ / ����
		else
		{
			vActorNodeUp.Set(mat._21, mat._22, mat._23);
			matRot.TransformVect(vActorNodeUp);
			vActorNodeUp.Normalize();

			vNxUpAxis.set(vActorNodeUp.x, vActorNodeUp.y, vActorNodeUp.z);
			vNxPos.set(vActorNodePos.x, vActorNodePos.y, vActorNodePos.z);

			pJoint = CreateRevoluteJoint(pNxScene, aryNxActor[nUpActor],aryNxActor[nBottomActor],vNxPos,vNxUpAxis, bBottomLimit, fRevolueLimit);
		}

		AddNxJointResource(pPhysXController, i, pJoint);
		aryJoint[i] = pJoint;
		if (NULL == pJoint)
		{
			bResult = false;
		}
	}


	//////////////////////////////////////////////////////////////////////////
	//
	// Add Update Info
	//
	//////////////////////////////////////////////////////////////////////////

	// root, pelvis�� ���� Update�ȴ�.
	DWORD dwAttr = RPhysXActorController::APPLY_ACTOR_POS | RPhysXActorController::APPLY_ACTOR_AXIS | RPhysXActorController::ADD_HIERARCHY_ATTR;
	PushBackNxUpdateInfo(pPhysXController, RSkeleton::GetBipName(RBIPID_ROOT), aryNxActor[E_PELVIS], NULL, dwAttr | RPhysXActorController::ADD_BIP_ROOT_ATTR );

	// pelvis
	PushBackNxUpdateInfo(pPhysXController, RSkeleton::GetBipName(RBIPID_PELVIS), aryNxActor[E_PELVIS], NULL, dwAttr | RPhysXActorController::ADD_PELVIS_ATTR );

	// ��ü, pelvis - spine ���� �����Ǿ� ����
	dwAttr = RPhysXActorController::APPLY_JOINT_POS | RPhysXActorController::APPLY_ACTOR_AXIS | RPhysXActorController::ADD_HIERARCHY_ATTR;
	PushBackNxUpdateInfo(pPhysXController, RSkeleton::GetBipName(aryBipd[E_LEFT_THIGH]), aryNxActor[E_LEFT_THIGH], aryJoint[E_LEFT_HIP], dwAttr);
	PushBackNxUpdateInfo(pPhysXController, RSkeleton::GetBipName(aryBipd[E_LEFT_CALF]), aryNxActor[E_LEFT_CALF], aryJoint[E_LEFT_KNEE], dwAttr);

	PushBackNxUpdateInfo(pPhysXController, RSkeleton::GetBipName(aryBipd[E_RIGHT_THIGH]), aryNxActor[E_RIGHT_THIGH], aryJoint[E_RIGHT_HIP], dwAttr);
	PushBackNxUpdateInfo(pPhysXController, RSkeleton::GetBipName(aryBipd[E_RIGHT_CALF]), aryNxActor[E_RIGHT_CALF], aryJoint[E_RIGHT_KNEE], dwAttr);

	// ��ü - spine1 - spine2 �� �����Ǿ� ����
	dwAttr = RPhysXActorController::APPLY_ACTOR_POS | RPhysXActorController::APPLY_ACTOR_AXIS | RPhysXActorController::ADD_HIERARCHY_ATTR;
	PushBackNxUpdateInfo(pPhysXController, RSkeleton::GetBipName(aryBipd[E_SPINE1]), aryNxActor[E_SPINE1], NULL, dwAttr);
	PushBackNxUpdateInfo(pPhysXController, RSkeleton::GetBipName(aryBipd[E_SPINE2]), aryNxActor[E_SPINE2], NULL, dwAttr);

	// ��
	dwAttr = RPhysXActorController::APPLY_JOINT_POS | RPhysXActorController::APPLY_ACTOR_AXIS | RPhysXActorController::ADD_HIERARCHY_ATTR;
	PushBackNxUpdateInfo(pPhysXController, RSkeleton::GetBipName(aryBipd[E_LEFT_UPPERARM]), aryNxActor[E_LEFT_UPPERARM], aryJoint[E_LEFT_SHOULDER], dwAttr);
	PushBackNxUpdateInfo(pPhysXController, RSkeleton::GetBipName(aryBipd[E_LEFT_FOREARM]), aryNxActor[E_LEFT_FOREARM], aryJoint[E_LEFT_ELBOW], dwAttr);

	PushBackNxUpdateInfo(pPhysXController, RSkeleton::GetBipName(aryBipd[E_RIGHT_UPPERARM]), aryNxActor[E_RIGHT_UPPERARM], aryJoint[E_RIGHT_SHOULDER], dwAttr);
	PushBackNxUpdateInfo(pPhysXController, RSkeleton::GetBipName(aryBipd[E_RIGHT_FOREARM]), aryNxActor[E_RIGHT_FOREARM], aryJoint[E_RIGHT_ELBOW], dwAttr);

	// ��
	dwAttr = RPhysXActorController::ADD_HIERARCHY_ATTR | RPhysXActorController::ADD_CUSTOMCONTROL_DATA;
	PushBackNxUpdateInfo(pPhysXController, RSkeleton::GetBipName(RBIPID_LHAND), aryNxActor[E_LEFT_FOREARM], NULL,  dwAttr, NULL, new RPhysXAnkleLeftController);
	PushBackNxUpdateInfo(pPhysXController, RSkeleton::GetBipName(RBIPID_RHAND), aryNxActor[E_RIGHT_FOREARM], NULL, dwAttr, NULL, new RPhysXAnkleRightController);

	// twist
	dwAttr = RPhysXActorController::ADD_HIERARCHY_ATTR | RPhysXActorController::ADD_LINKEDNODE_DATA;
	PushBackNxUpdateInfo(pPhysXController, "Bip01 LThighTwist", NULL, NULL, dwAttr, RSkeleton::GetBipName(aryBipd[E_LEFT_THIGH]) );
	PushBackNxUpdateInfo(pPhysXController, "Bip01 RThighTwist", NULL, NULL, dwAttr, RSkeleton::GetBipName(aryBipd[E_RIGHT_THIGH]) );
	PushBackNxUpdateInfo(pPhysXController, "Bip01 LUpArmTwist", NULL, NULL, dwAttr, RSkeleton::GetBipName(aryBipd[E_LEFT_UPPERARM]) );
	PushBackNxUpdateInfo(pPhysXController, "Bip01 RUpArmTwist", NULL, NULL, dwAttr, RSkeleton::GetBipName(aryBipd[E_RIGHT_UPPERARM]) );
	// ������ȣ��
	PushBackNxUpdateInfo(pPhysXController, "Dummy04", NULL, NULL, dwAttr, RSkeleton::GetBipName(aryBipd[E_RIGHT_CALF]) );
	PushBackNxUpdateInfo(pPhysXController, "Dummy03", NULL, NULL, dwAttr, RSkeleton::GetBipName(aryBipd[E_LEFT_CALF]) );
	// ��� ��
	PushBackNxUpdateInfo(pPhysXController, "Dummy06", NULL, NULL, dwAttr, RSkeleton::GetBipName(aryBipd[E_RIGHT_UPPERARM]) );
	PushBackNxUpdateInfo(pPhysXController, "Dummy05", NULL, NULL, dwAttr, RSkeleton::GetBipName(aryBipd[E_LEFT_UPPERARM]) );
	// �ٺ���ȣ��
	PushBackNxUpdateInfo(pPhysXController, "Dummy02", NULL, NULL, dwAttr, RSkeleton::GetBipName(aryBipd[E_RIGHT_FOREARM]) );
	PushBackNxUpdateInfo(pPhysXController, "Dummy01", NULL, NULL, dwAttr, RSkeleton::GetBipName(aryBipd[E_LEFT_FOREARM]) );

	// Set Pelivs Inverse Transform to 'Hierarchy Root Inverse' and Update Map Refresh
	RActorNode* pPelvis = pActor->GetActorNode( RSkeleton::GetBipName(RBIPID_PELVIS) );
	if ( pPelvis )
	{
		RMatrix matInv;
		pPelvis->GetResultTransform().GetInverse(&matInv);
		SetHierarchyRootInvTransform(pPhysXController, matInv);
	}
	RefreshNxUpdateInfoMap(pPhysXController);
	return bResult;
}

float RPhysXRagDollBuilder::GetCapsuleLengthFromChildBip( RActor* pActor, RBIPID bipId )
{
	_ASSERT( pActor != NULL);
	_ASSERT( bipId != RBIPID_UNKNOWN);

	RActorNode* pActorNode = pActor->GetActorNode( RSkeleton::GetBipName(bipId) );
	RMatrix matLocal = pActorNode->m_pMeshNode->m_matLocal;
	return (RVector::ZERO * matLocal).Length();
}

}
