#pragma once


#include "RActorPassRenderer.h"



namespace rs3 {


	class RS_API RPostEffectEdgePassRender : public RActorPassRenderer
	{
	public:
		RPostEffectEdgePassRender();
		virtual ~RPostEffectEdgePassRender() {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Pass Renderer ID ����
		//
		DECLARE_PASS_RENDERER_ID( RRP_POSTEFFECT_EDGE );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// update / rendering
		//
		virtual void		RenderMStageOnPass( RCameraSceneNode* pCurrentCamera, SCENE_SET* pRenderingList, RRENDERINGMETHOD renderMethod) override;

	protected:
		virtual void		UpdateShaderIDForMStage(RSCID& rscComponents, bool bInstancing, bool bNoiseRef, bool bBlendingMaterial, bool bMultiplyColor) override;
	};

}