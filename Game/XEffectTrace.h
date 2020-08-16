#ifndef _XEFFECT_TRACE_H
#define _XEFFECT_TRACE_H

#include "XObject.h"
#include "XDef.h"
#include "RTimer.h"
#include "RActor.h"
#include "XEffect.h"

using namespace rs3;

class XEffectTrace
{
private:
	RTexture*					m_hTexture;
	float						m_fTraceMaxLength;						// Trace Max ����, 0�� ���� ������ ���°Ŵ�.
	
	vec3						m_vStartPos;

	vector<RLVertex>			m_vecHorizonVertexBuffer;
	vector<RLVertex>			m_vecVerticalVertexBuffer;

	vector<vec3>				m_vecTraceMovePos;

private:
	void UpdateHorizonPolygon();
	void UpdateVerticalPolygon();

public:
	XEffectTrace();
	void Create(vec3 vStartPos, float fTraceMaxLength);
	void Destroy();
	void Update(vec3& vProjectilePos);
	void Render();
};



#endif