#pragma once

// Ư�� scenenode �� target�� ���� ȸ������ �ֱ� ���� ������� controller.

#include "RSceneNodeController.h"

namespace rs3 {

class RS_API RAlignController : public RSceneNodeController
{
public:
	DECLARE_NODE_ID_STRING("AlignController");

	RAlignController(void);
	virtual ~RAlignController(void);

	virtual void Update(RCameraSceneNode* pCamera) override;

};

}