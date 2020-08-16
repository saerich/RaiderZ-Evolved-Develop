#pragma once

#include "RActor.h"
#include "RClothActorNode.h"

namespace rs3 
{
	class RClothLocalActorNode : public RClothActorNode, public MMemPool<RClothLocalActorNode>
	{
	public:
		RClothLocalActorNode(void);
		virtual ~RClothLocalActorNode(void);

	//--[ RActorNode������ �Լ� ]
	public:
		virtual bool	OnCreateDeviceDependentPart(void);	//�������̵�
		virtual void	RenderPrimitive(int index);			//�������̵�
		virtual void	SetShaderTransformConstant(const RMatrix& matView, const RMatrix& matViewProj);

	//--[ RClothActorNode������ �Լ� ]
	protected:
		virtual void	RenderCollision(int index);

	//--[ RClothSimulator�� �Լ� ]
	protected:
		virtual void		resolveCollision(void);
		virtual	void		simulate(float timeStep);	

	//--[ RClothLocalActorNode �� �Լ� ]
	protected:
		void		skinningAttachingPoints(void);
		
		void		virtuallyTranslate(void);
		void		getVirtualTranslation(RVector *pVirtualTranslation);
		void		translateMovableClothParticles(RVector *pVirtualTranslation);

	private:
		RBox		m_clothAABBForDebugRender;

	};
}//rs3