#pragma once

#include "GJob.h"
#include "MTime.h"
#include "MMemPool.h"

class GEntityActor;

/// ���� ��ƼƼ ��ġ�� �̵��ϴ� �ൿ ���
class GJob_MoveToEntity : public GJob, public MMemPool<GJob_MoveToEntity>
{
private:
	float			m_fLeastGap;				///< findpath���� ����ϴ� �ּ� �Ÿ�
	MUID			m_uidTargetEntity;
	bool			m_bInifity;					// ��� �Ѿƴٴϱ�
	MRegulator		m_FindPathRegulator;
	vec3			m_vLastTarEntityPos;		///< ���������� findpath �������� Ÿ�� ��ƼƼ ��ġ
	vec3			m_vLastTarEntityDir;		///< ���������� findpath �������� Ÿ�� ��ƼƼ ����
private:
	virtual bool		OnStart();
	virtual	GJobResult	OnRun(float fDelta);
	virtual void		OnTaskComplete(GTaskID nTaskID);
	virtual void		OnCancel();
	virtual void		OnResume();
	virtual void		OnComplete();
	virtual void		OnTimeout();						///< Job ����ð� ����

	bool			IsTargetMoved( vec3 vCurTarEntityPos );
	bool			MoveToEntity();
	bool			IsArriveTarEntity(const vec3& vTarEntityPos, float fDelta=0.0f);			
	bool			CheckFindPathTick(vec3 vCurTarEntityPos, float fDelta);
	GEntityActor*	GetTargetEntity();
public:
	DECLARE_JOB_ID(GJOB_MOVE_TO_ENTITY);
	virtual string GetParamString() const override;

	GJob_MoveToEntity(GModuleAI* pmAI, MUID uidTargetEntity, float fLeastDistance=0.0f, bool bSetInifity=false);
	virtual ~GJob_MoveToEntity();

	bool			GetTargetEntityPos(vec3& voutTarPos);
	bool			GetTargetEntityDir(vec3& voutTarDir);
	float			GetLeastDistance()						{ return m_fLeastGap; }			

};

