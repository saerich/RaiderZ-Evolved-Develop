#pragma once

#include "RActorNode.h"
#include "MMemPool.h"

namespace rs3 {

///	RActorNodePhysique �� CPU physique
class RS_API RActorNodePhysique : public RActorNode
{
	MDeclareRTTI;

protected:
	unsigned int					m_nLastUpdatedFrame;	///< �� ������ ���� �ߺ�������Ʈ�� ���� ���� ����

	//bool							m_bResetVertexAgainstMatrix;	// ���̽� ��Ʈ���� Inv�� ���������� 

public:
	explicit RActorNodePhysique(ACTOR_NODE_TYPE eActorNodeType);
	virtual ~RActorNodePhysique(void);

	virtual void					UpdateTransformAndVertex();
	virtual void					ResetTransform();

	/// cpu �� physique �� ó���ϴ� ���
	int								UpdatePoint_SoftPhysiqueAnimation();

	virtual void					RenderNormal(DWORD dwColor);	// ����� ���� : ���
	virtual void					RenderWire(DWORD dwColor);		// ����� ���� : ���̾�

	virtual void					RenderPrimitive(int index);		// �����̾� ���ߵ� ���ؽ��� DP
	virtual void					RenderAllNodePrimitive();

	virtual bool					OnPick(RPICKINFO& pickInfo);

	virtual bool					Validate();

	//virtual bool					ResetVertexAgainstMatrix();		// ������ ���ؽ��� ���̽� ��Ʈ���� Inv�� ������ ����� ������ �ִ´�. 

};

class RS_API RActorNodePhysiqueGPU : public RActorNodePhysique, public MMemPool<RActorNodePhysiqueGPU, 50000>
{
	friend RActor;

	MDeclareRTTI;

public:
	RActorNodePhysiqueGPU(void);
	virtual ~RActorNodePhysiqueGPU(void);

	virtual void					SetShaderTransformConstant(const RMatrix& matView, const RMatrix& matViewProj);

	virtual void					RenderPrimitive(int index);		// �����̾� ���ߵ� ���ؽ��� DP
	virtual void					RenderAllNodePrimitive();
};

}