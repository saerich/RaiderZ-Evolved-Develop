#pragma once

#include "RActor.h"
#include "RClothActorNode.h"
#include "MMemPool.h"

namespace rs3 
{
	class RClothWorldActorNode : public RClothActorNode, public MMemPool<RClothWorldActorNode>
	{
	public:
		RClothWorldActorNode(void);
		virtual ~RClothWorldActorNode(void);

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

	//--[ RClothWorldActorNode�� �Լ� ]
	protected:
		bool  		intersectWithPartitionedOBB( ROBB &obbForWorldShape );
		void		skinningAttachingPoints(void);
		void		warpCloth(void);

	private:
		RBox		m_clothAABBForDebugRender;
		bool		m_bIntersectWithPartitionedOBBForDebugRender;


	};
}//rs3