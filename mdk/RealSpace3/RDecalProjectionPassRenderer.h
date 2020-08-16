#pragma once

#include <map>
#include "RPassRenderer.h"

namespace rs3 {

class RDecalProjectionPassRenderer : public RPassRenderer
{
public:
	RDecalProjectionPassRenderer(void);
	virtual ~RDecalProjectionPassRenderer(void);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pass Renderer ID ����
	//
	DECLARE_PASS_RENDERER_ID( RRP_DECAL_PROJECTION );

	virtual void RenderOnPass( RCameraSceneNode* pCurrentCamera,SCENE_SET* pRenderingList, RRENDERINGMETHOD eMothod) override;
	virtual void RenderMStageOnPass( RCameraSceneNode* pCurrentCamera,SCENE_SET* pRenderingList, RRENDERINGMETHOD renderMethod) override;

};

}