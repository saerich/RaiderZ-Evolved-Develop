#pragma once

// Ư�� actornode �� �ܼ��� ȸ������ �ֱ� ���� ������� controller.
// model ��ǥ��� ��� ȸ���� �����Ѵ�. ���� �ʿ��ϸ� local, world ��ǥ������� Ȯ������

#include <list>
#include "RSceneNodeController.h"

namespace rs3 {

class RS_API RRotationController : public RSceneNodeController
{
	RVector		m_axis;
	float		m_fAngle;
	RMatrix		m_matAffectedTransform;		// ������ ���� ��ȯ�� ��Ƶд�.

public:
	DECLARE_NODE_ID_STRING("RotationController");

	RRotationController(void);
	virtual ~RRotationController(void);

	virtual void Update(RCameraSceneNode* pCamera) override;

	void SetRotation(const RVector& axis, float fAngle);

	const RMatrix& GetAffectedTransform() { return m_matAffectedTransform; }

};

}