#pragma once

#include "RLODProcessorBase.h"

namespace rs3
{

class RLODMetricInfo;

class RS_API RLODProcessorDistance : public RLODProcessorBase
{
private:
	RLODMetricInfo* m_pLODMetricInfoRef;	/// ��ü������ �����ϴ� ���� �ƴ϶� ������ �����̴�.

public:
	RLODProcessorDistance();
	~RLODProcessorDistance();

public:
	bool SetMetricInfo( RLODMetricInfo* pMetricInfo);

	virtual int	CalculateLOD( RCameraSceneNode* pCamera, const RSphere* pBoundingSphere) override;
};
}