#pragma once

#include "MMath.h"
#include "RDevice.h"

namespace rs3 {

// ���� ��ǥ�� ��ũ�� ��ǥ�� ��ȯ�� �ִ� ��. ģ���� DX �Լ��� ����Ͽ���.
RS_API void GetScreenFromWorld(MVector2 *pScreen, const MVector3 *position, RDevice *pDevice);
RS_API void GetScreenFromWorld(MVector2 *pScreen, const MVector3 *position, const RViewport *pViewport, const MMatrix *pWorldMatrix, const MMatrix *pViewMatrix, const MMatrix *pProjectionMatrix);
RS_API void GetScreenFromWorld(MVector3 *pScreen, const MVector3 *position, const RViewport *pViewport, const MMatrix *pWorldMatrix, const MMatrix *pViewMatrix, const MMatrix *pProjectionMatrix);

}