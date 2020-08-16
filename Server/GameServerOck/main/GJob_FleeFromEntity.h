#pragma once

#include "MMemPool.h"
#include "GJob.h"

class GJob_FleeFromEntity : public GJob, public MMemPool<GJob_FleeFromEntity>
{
public:
	GJob_FleeFromEntity(GModuleAI* pmAI, MUID& uidTarget);
	virtual ~GJob_FleeFromEntity(void);

	DECLARE_JOB_ID(GJOB_FLEE_FROM_ENTITY);
	virtual string GetParamString() const override;
	
private:
	virtual bool OnStart();
	virtual GJobResult OnRun(float fDelta);
	virtual void OnTaskComplete(GTaskID nTaskID);
	virtual void OnCancel();

	bool IsInvalidTarget();
	bool FleeFromEntity();

	bool GetTarEntityInversePos(vec3& tar_pos);	///< OwnerPos�� �������� �ؼ� TarEntityPos�� �ݴ�Ǵ� ��ġ�� ���Ѵ�.
	bool GetTarEntityPos(vec3& vOutTarPos);

private:
	MUID			m_uidTarget;
};
