#pragma once

#include "RAnimationNode.h"
#include "RActorNode.h"

namespace rs3 {

class RActorNode;
class RAnimationNode;
class RAnimationController;

class RS_API RAnimationControllerNode
{
	friend RAnimationController;

	const char*		m_szName;	// for debug

//	RAnimationController*		m_pBase;		///< base animation controller
	RAnimationControllerNode*	m_pParent;		///< �ִϸ��̼��� �θ�

	RAnimationNode* m_pAniNode;					///< ����� animation node
	RActorNode*		m_pActorNode;				///< ����� actornode
	float			m_fBoneImportance;			///< ���� �߿䵵

	unsigned int	m_nPosKeyIterator;
	unsigned int	m_nRotKeyIterator;
	unsigned int	m_nScaleKeyIterator;
	unsigned int	m_nVisKeyIterator;


	bool			m_bUpdateAnimation;		// �ִϸ��̼� ������Ʈ ����

	float			m_fVisibility;			// �ִϸ��̼��� ���� ��
	RVector			m_currentScale;			// �ִϸ��̼��� �����ϰ�
	RQuaternion		m_currentRotation;		// �ִϸ��̼��� ȸ����
	RVector			m_currentTranslation;	// �ִϸ��̼��� �̵���
	RMatrix			m_matCurrent;			// ���ϸ��̼ǿ��� ������ local Matrix �� ( = ȸ�� + �̵� )

	RMatrix			m_matResult;			// ���ϸ��̼ǿ��� ������ ���� Matrix ��. world matrix �� ������ ��������.

// 	RQuaternion		m_baseRotation;			// �⺻ȸ����
// 	RVector			m_baseTranslation;		// �⺻�̵���
	RMatrix			m_matBase;				// ���ϸ��̼��� ���� ��� ����ϰ� �ڽ��� ù ������ ���� ��Ʈ����,

public:
	RAnimationControllerNode();
	virtual ~RAnimationControllerNode(void);

	RAnimationNode* GetAnimationNode();
	void SetAnimationNode(RAnimationNode* pNode);

	void Disconnect();

	void SetEnableAnimationUpdate(bool bEnable)
	{
		m_bUpdateAnimation = bEnable;
	}

	void Update(unsigned int nFrame);
	void UpdateVertexAnimation(unsigned int nFrame);
	void UpdateActorNode();

	const char* GetNodeName();
	RActorNode* GetNode();

	const RQuaternion& GetCurrentRotation();
	const RVector& GetCurrentTranslation();
	const RVector& GetCurrentScale();

	const RQuaternion& GetResultRotation();
	const RVector& GetResultTranslation();

	float GetVisibility();

	bool HasVisibilityAnimation();

	RQuaternion	GetRotationAt(DWORD nFrame);
	RVector		GetTranslationAt(DWORD nFrame);
	RVector		GetScaleAt(DWORD nFrame);

	void		GetLocalTransformAt(DWORD nFrame, RMatrix& outMatrix);
};

//////////////////////////////////////////////////////////////////////////
// �ζ��� �Լ���
inline const char* RAnimationControllerNode::GetNodeName()				{ return m_pAniNode->GetName(); }
inline const RQuaternion& RAnimationControllerNode::GetCurrentRotation()	{ return m_currentRotation; }
inline const RVector& RAnimationControllerNode::GetCurrentTranslation() { return m_currentTranslation; }
inline const RVector& RAnimationControllerNode::GetCurrentScale()		{ return m_currentScale; }
inline float RAnimationControllerNode::GetVisibility()					{ return m_fVisibility; }
inline RAnimationNode* RAnimationControllerNode::GetAnimationNode()		{ return m_pAniNode; }
inline void RAnimationControllerNode::UpdateActorNode()
{
	if(m_bUpdateAnimation && m_pActorNode)
	{
		m_pActorNode->SetVisibility( GetVisibility() );

		m_pActorNode->SetRotation( GetCurrentRotation() );
		m_pActorNode->SetScale( GetCurrentScale() );
		m_pActorNode->SetPosition( GetCurrentTranslation() );

		m_pActorNode->UpdateAllParentDependentValues();
		m_pActorNode->UpdateFinalResultTransforms();
	}
}
}