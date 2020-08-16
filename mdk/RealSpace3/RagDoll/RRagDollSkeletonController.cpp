#include "stdafx.h"
#include "RRagDollSkeletonController.h"
#include "RSimpleBoxSceneNode.h"
#include "RSimpleCylinderSceneNode.h"
#include "RSimpleSphereSceneNode.h"
#include "RSceneManager.h"
#include "RActor.h"

namespace rs3 {

RRagDollSkeletonController::RRagDollSkeletonController(const NewtonWorld* pNWorld, NewtonApplyForceAndTorque callBackNewtonApplyForceAndTorque) 
							: RActorController(), m_pRagDoll(NULL), bones(NULL), m_bShowBones(false)
{
	m_callBackNewtonApplyForceAndTorque = callBackNewtonApplyForceAndTorque;
	nWorld = pNWorld;
//	BuildRagDoll();
}

RRagDollSkeletonController::~RRagDollSkeletonController(void)
{
	DestroyRagDoll();
}

/// note: cylinder Ÿ���� ��� ���̸� ������� BIPID �� �ڽ����� ����ξ���.
/// �ʱ�ȭ �Ҷ� �θ�� ��ȯ�Ѵ�.

// TODO: ���������� ������ ���̰ų� ��Ʋ���°Ϳ����� ���������� �ʿ�.
struct m_pRagDollBoneDefine {
	RAGDOLLBONETYPE boneType;
	float	fLimitConeAngle;	// radian
	float	fMaxTwistAngle;		// radian
	int		parentIndex;
	RBIPID	refBipID;
	DWORD	dwColor;
} boneDefine[] = {
	{	RAGDOLLBONETYPE_BOX , 0.05f, 0.05f, -1, RBIPID_PELVIS },		// 0 pelvis
	{	RAGDOLLBONETYPE_BOX , 0.05f, 0.05f, 0, RBIPID_SPINE },			// 1 spine
	{	RAGDOLLBONETYPE_BOX , 0.05f, 0.05f, 1, RBIPID_SPINE1 },			// 2 spine1
	{	RAGDOLLBONETYPE_BOX , 0.05f, 0.05f, 2, RBIPID_SPINE2 },			// 3 spine2
	{	RAGDOLLBONETYPE_CYLINDER , 0.05f, 0.05f, 3, RBIPID_HEAD },		// 4 neck

	{	RAGDOLLBONETYPE_SPHERE, 0.15f, 0.05f, 4, RBIPID_HEAD },			// 5 head
	// �Ǹ��� Ÿ���� ��� ���̸� ������� �ڽ����� �����ִ�.
	{	RAGDOLLBONETYPE_CYLINDER , 0.01f, 0.05f, 4, RBIPID_RUPPERARM },	// 6 right clavicle
	{	RAGDOLLBONETYPE_CYLINDER , 0.25f, 0.05f, 6, RBIPID_RFOREARM },	// 7 right upper arm
	{	RAGDOLLBONETYPE_CYLINDER , 0.25f, 0.05f, 7, RBIPID_RHAND },		// 8 right fore arm
	{	RAGDOLLBONETYPE_CYLINDER , 0.01f, 0.05f, 4, RBIPID_LUPPERARM },	// 9 left clavicle

	{	RAGDOLLBONETYPE_CYLINDER , 0.25f, 0.05f, 9, RBIPID_LFOREARM },	// 10 left upper arm
	{	RAGDOLLBONETYPE_CYLINDER , 0.25f, 0.05f, 10, RBIPID_LHAND },	// 11 left fore arm
	{	RAGDOLLBONETYPE_CYLINDER , 0.25f, 0.05f, 1, RBIPID_RCALF },		// 12 right upper leg
	{	RAGDOLLBONETYPE_CYLINDER , 0.25f, 0.05f, 12, RBIPID_RFOOT },	// 13 right lower leg
	{	RAGDOLLBONETYPE_CYLINDER , 0.25f, 0.05f, 1, RBIPID_LCALF },		// 14 left upper leg

	{	RAGDOLLBONETYPE_CYLINDER , 0.25f, 0.05f, 14, RBIPID_LFOOT },	// 15 left lower leg
};

const int RAGDOLL_BONE_COUNT = elementsOf(boneDefine);

struct ragDollBones : public m_pRagDollBoneDefine {
    RVector	boneSize;
	RMatrix boneMatrix;			// local matrix
	RMatrix boneBaseMatrix;		// �θ� �������ִ� ���
	int		ID;
	NewtonRagDollBone* pRagDollBone;
	NewtonBody*	pBody;
	RSceneNode* pSceneNode;
};

// TODO: GetParentBoneLength() �� ����
float RRagDollSkeletonController::GetParentBoneLength( RBIPID bipId)
{
	if(bipId==RBIPID_UNKNOWN) return 0;

	RActorNode *pActorNode = m_pActor->GetActorNode(bipId);
	RMatrix matLocal = pActorNode->m_pMeshNode->m_matLocal;
	return (RVector(0,0,0) * matLocal).Length();
}

void RRagDollSkeletonController::BuildRagDoll()
{
	_ASSERT(m_pRagDoll==NULL);

	bones = new ragDollBones[RAGDOLL_BONE_COUNT];

	for(int i=0;i<RAGDOLL_BONE_COUNT;i++) 
	{
		memcpy(&bones[i],&boneDefine[i],sizeof(m_pRagDollBoneDefine));
		bones[i].dwColor = 0xffffffff;
		bones[i].ID = i;
		bones[i].fMaxTwistAngle = .01f;
	}

//	RVector m_pRagDollPosition = RVector(0,0,80);
//	RVector m_pRagDollRotation = RVector(0,-110,180);

	const float headSize = 10.f;
	RVector spineSize = RVector(1.f*headSize,1.5f*headSize,1.5f*headSize);

	// ���̹� transform �� ����
	for(int i=0;i<RAGDOLL_BONE_COUNT;i++) 
	{
		ragDollBones& bone = bones[i];

		switch(bone.boneType) {
		case RAGDOLLBONETYPE_BOX : 
			bone.boneSize = spineSize;break;
		case RAGDOLLBONETYPE_CYLINDER : 
		{
			bone.boneSize.y = bone.boneSize.z = headSize*.8f;
			bone.boneSize.x = GetParentBoneLength(bone.refBipID) - bone.boneSize.y*.5f; 
			bone.refBipID = RSkeleton::GetBipParent(bone.refBipID);	// �Ǹ��� Ÿ���� ��� ���̸� ������� �ڽ����� �����ִ�.
		}break;
		case RAGDOLLBONETYPE_SPHERE :
			bone.boneSize = RVector(headSize,headSize,headSize);
			break;
		}

		if(i==0) {	// root �϶�
			m_matRoot = m_pActor->GetActorNode(RBIPID_ROOT)->GetLocalTransform();
//					GetCurrentTransform(RBIPID_ROOT);
			RMatrix matRoot;
//			matRoot = GetCurrentTransform(RBIPID_PELVIS) * m_pActor->GetWorldTransform();
			matRoot = m_pActor->GetActorNode(RBIPID_PELVIS)->GetWorldTransform();
//			matRoot.MakeIdentity();
//			matRoot.SetRotationDegrees(RVector(0,0,-90));
			bone.boneMatrix = matRoot;
		}
		else
		{
//			bone.boneMatrix = GetCurrentTransform(bone.refBipID);
			bone.boneMatrix = m_pActor->GetActorNode(bone.refBipID)->GetLocalTransform();
//			bone.boneMatrix = m_pSkeleton->GetLocalMatrix(bone.refBipID);
		}


		bone.boneBaseMatrix = bone.boneMatrix;
		if(bone.parentIndex!=-1)
			bone.boneBaseMatrix = bone.boneBaseMatrix * bones[bone.parentIndex].boneBaseMatrix;
	}



	//create a ragdoll container
	m_pRagDoll = NewtonCreateRagDoll(nWorld);
	// begin adding bones to the rad doll
	NewtonRagDollBegin(m_pRagDoll);
	// set the force function
	NewtonRagDollSetForceAndTorqueCallback(m_pRagDoll, m_callBackNewtonApplyForceAndTorque);

	// Build ragdoll
	for(int i=0;i<RAGDOLL_BONE_COUNT;i++) 
	{
		ragDollBones& bone = bones[i];

		NewtonCollision* collider;
		RSceneNode* pBoneSceneNode = NULL;

		// type�� ���� collider�� scenenode�� �����
		switch(bone.boneType) {
		case RAGDOLLBONETYPE_BOX : 
			collider = NewtonCreateBox(nWorld,bone.boneSize.x,bone.boneSize.y,bone.boneSize.z,NULL);
			pBoneSceneNode = new RSimpleBoxSceneNode;
			{
				RBoundingBox box;
				box.vmin = -.5f*bone.boneSize;
				box.vmax = .5f*bone.boneSize;
				((RSimpleBoxSceneNode*)pBoneSceneNode)->SetBox(box);
				((RSimpleBoxSceneNode*)pBoneSceneNode)->SetColor(0xffffff00);
			}
			break;
		case RAGDOLLBONETYPE_CYLINDER: 
			collider = NewtonCreateCapsule(nWorld,.5f * bone.boneSize.y,bone.boneSize.x,NULL);
			pBoneSceneNode = new RSimpleCylinderSceneNode;
			((RSimpleCylinderSceneNode*)pBoneSceneNode)->m_fRadius = .5f * bone.boneSize.y;
			((RSimpleCylinderSceneNode*)pBoneSceneNode)->m_fHeight = bone.boneSize.x;
			((RSimpleCylinderSceneNode*)pBoneSceneNode)->m_dwColor = bone.dwColor;
			break;
		case RAGDOLLBONETYPE_SPHERE: 
			collider = NewtonCreateSphere(nWorld,.5f * bone.boneSize.x,.5f*bone.boneSize.y,.5f*bone.boneSize.z,NULL);
			pBoneSceneNode = new RSimpleSphereSceneNode;
			((RSimpleSphereSceneNode*)pBoneSceneNode)->m_fRadius = .5f * bone.boneSize.x;
			break;
		}

		if(pBoneSceneNode)
		{
//			if(i!=5 && i!=6)
			REngine::GetSceneManager().AddSceneNode(pBoneSceneNode);
			bone.pSceneNode = pBoneSceneNode;
			pBoneSceneNode->SetVisible(m_bShowBones);
		}

		NewtonRagDollBone* pParentBone = 
			bone.parentIndex>=0 ? bones[bone.parentIndex].pRagDollBone : NULL;

		NewtonRagDollBone* pBone = NewtonRagDollAddBone(m_pRagDoll, pParentBone, pBoneSceneNode, 10, 
			bone.boneMatrix, collider, bone.boneSize);
		NewtonRagDollBoneSetID(pBone, bone.ID);
		bone.pRagDollBone = pBone;
		// Release collider
		NewtonReleaseCollision(nWorld, collider);

		// Set limits for the joints
		// The x axis is usually the box main axis
		RVector coneDir(1,0,0);
		// The z axis is usually the box plane
		RVector LateralDir(0, 0, 1);
		// The axis limits are specifiied in global space, rotate by the joint matrix
		bone.boneMatrix.TransformNormal(coneDir);
		bone.boneMatrix.TransformNormal(LateralDir);
		// Set the limis
		NewtonRagDollBoneSetLimits(pBone, coneDir, -MMath::PI*bone.fLimitConeAngle, MMath::PI*bone.fLimitConeAngle, MMath::PI*bone.fMaxTwistAngle, LateralDir, 0, 0);
		bone.pBody = NewtonRagDollBoneGetBody(pBone);

		NewtonBodySetUserData (bone.pBody, pBoneSceneNode);
//		NewtonBodySetTransformCallback (bone.pBody, PhysicsSetTransform);

	}
}

void RRagDollSkeletonController::DestroyRagDoll()
{
	if(!m_pRagDoll) return;

	for(int i=0;i<RAGDOLL_BONE_COUNT;i++) {
		bones[i].pSceneNode->RemoveFromParent();
		SAFE_DELETE(bones[i].pSceneNode);
	}
	NewtonDestroyRagDoll(nWorld,m_pRagDoll);
	m_pRagDoll = NULL;
	SAFE_DELETE_ARRAY(bones);
}

void RRagDollSkeletonController::OnAdd()
{
	m_pActor->SetAnimation((RAnimation*)NULL);
	if(m_pRagDoll)
		DestroyRagDoll();

	BuildRagDoll();
}

void RRagDollSkeletonController::OnRemove()
{
	DestroyRagDoll();
}


void RRagDollSkeletonController::Update()
{
	/*
	RBIPID m_pRagDollBoneTable[elementsOf(bones)] = {
		RBIPID_PELVIS,
		RBIPID_SPINE2,
		RBIPID_HEAD,
		RBIPID_LUPPERARM,
		RBIPID_LFOREARM,
		RBIPID_RUPPERARM,
		RBIPID_RFOREARM,
		RBIPID_LTHIGH,
		RBIPID_LCALF,
		RBIPID_RTHIGH,
		RBIPID_RCALF,
	};
	*/

	for(int i=0;i<RAGDOLL_BONE_COUNT;i++) 
	{
		ragDollBones& bone = bones[i];
		
		RMatrix matBone;
		NewtonBodyGetMatrix(bone.pBody,matBone);
		bone.pSceneNode->SetTransform(matBone);

		RMatrix matLocal;
		
		if(bone.parentIndex==-1)
		{	// Root Node
			RMatrix matRoot = m_pActor->GetWorldTransform();
			RMatrix matRootInv;
			matRoot.GetInverse(&matRootInv);
			// max���� pelvis�� root���� ��ġ�º������ʰ� ȸ������ �������ִ�.
			SetCurrent(RBIPID_ROOT,matBone * matRootInv);	// bip-root node
			matLocal.MakeIdentity();						// bip-pelvis
		}else		
		{	// �θ�κ����� local ��ȯ�� ���Ѵ�
			RMatrix matParentBone;
			NewtonBodyGetMatrix(bones[bone.parentIndex].pBody,matParentBone);

			/*
			if(bones[bone.parentIndex].boneType==RAGDOLLBONETYPE_CYLINDER) {
				RMatrix matEndPoint;
				matEndPoint.MakeIdentity();
				matEndPoint.SetTranslation(RVector(+.5f*bones[bone.parentIndex].boneSize.x,0,0));
				matParentBone = matEndPoint * matParentBone ;
			}
			*/
			RMatrix invParent;
			matParentBone.GetInverse(&invParent);
			matLocal = matBone * invParent;
		}

//		if(i==13) {
		// bone �� ������ ���� �߰��̱� ������ parent �� ���������� ��ȯ
		if(bone.boneType==RAGDOLLBONETYPE_CYLINDER) {
			RMatrix matEndPoint;
			matEndPoint.MakeIdentity();
			matEndPoint.SetTranslation(RVector(-.5f*bone.boneSize.x,0,0));
			matLocal = matEndPoint * matLocal ;

			// �θ� cylinder �� ��� ���� ���� �θ��� �θ��� ����� ���� ������
			// �θ��� ������ ��ŭ �̵��Ѵ�.
			if(bones[bone.parentIndex].boneType==RAGDOLLBONETYPE_CYLINDER) {
				matEndPoint.MakeIdentity();
				matEndPoint.SetTranslation(.5f*RVector(bones[bone.parentIndex].boneSize.x,0,0));
				matLocal = matLocal * matEndPoint;
			}
		}
//		}

//		SetCurrentMatrix(bone.refBipID, matLocal);
		SetCurrent(bone.refBipID, matLocal);
	}
}

void RRagDollSkeletonController::SetCurrent(RBIPID bipId,const RMatrix& matTransform)
{
	RQuaternion quatRotation = matTransform;
	RVector		vecPosition = RVector(matTransform._41,matTransform._42,matTransform._43);
//	SetCurrentRotation(bipId,quatRotation);
//	SetCurrentTranslation(bipId,vecPosition);

//	SetCurrentTransform(bipId,matTransform);

	m_pActor->GetActorNode(bipId)->SetRotation( quatRotation );
	m_pActor->GetActorNode(bipId)->SetPosition( vecPosition );

}


void RRagDollSkeletonController::ShowBones(bool bShow)
{
	m_bShowBones = bShow;
	if(!bones) return;
	for(int i=0;i<RAGDOLL_BONE_COUNT;i++) 
	{
		ragDollBones& bone = bones[i];
		if(bone.pSceneNode)
			bone.pSceneNode->SetVisible(bShow);
	}
}


}