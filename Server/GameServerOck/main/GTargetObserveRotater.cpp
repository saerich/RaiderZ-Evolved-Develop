#include "StdAfx.h"
#include "GTargetObserveRotater.h"
#include "GEntityNPC.h"
#include "GMath.h"
#include "GModuleAI.h"
#include "GTask.h"
#include "GGlobal.h"
#include "GTalentInfoMgr.h"
#include "GTask_Broker.h"
#include "GJobMgr.h"

GTargetObserveRotater::GTargetObserveRotater(void)
{
}

GTargetObserveRotater::~GTargetObserveRotater(void)
{
}

bool GTargetObserveRotater::IsNeedRotate(GEntityNPC* pOwnerNPC)
{	
	VALID_RET(pOwnerNPC, false);

	// Ÿ���� �ִ��� Ȯ��
	GEntity* pTargetEntity = pOwnerNPC->GetTargetActor();
	if ( pTargetEntity == NULL)
	{
		return false;
	}


	// ȸ�� ������ ������ �ִ��� Ȯ��
	if (!HasRotateTalent(pOwnerNPC))
		return false;

	if(pOwnerNPC->GetNPCInfo()->nAltitude == AT_SKY)
		return false;	// ���߿� �������� ȸ�� �ŷ�Ʈ ��� ����

	// ���� ȸ���� �� �� �ִ� �������� Ȯ��
	GJobMgr* pJobMgr = pOwnerNPC->GetJobMgr();	
	GTask* pTask = pJobMgr->GetCurrTask();
	if ( pTask   &&   pTask->GetID() != GTASK_IDLE   &&   pTask->GetID() != GTASK_MOVE   &&   pTask->GetID() != GTASK_COMBAT_MOVE)
	{
		return false;
	}


	// ȸ�� ���� Ȯ��
	vec3 _dir = pTargetEntity->GetPos() - pOwnerNPC->GetPos();
	GMath::NormalizeZ0( _dir);
	float fAngle = pOwnerNPC->GetDir().AngleToXY( _dir);

	GNPCInfo* pInfo = pOwnerNPC->GetNPCInfo();

	if ( fAngle > -pInfo->m_fRotationActAngle  &&  fAngle < pInfo->m_fRotationActAngle)
	{
		return false;	
	}

	return true;
}

GTask* GTargetObserveRotater::GetRotateTask( GEntityNPC* pOwnerNPC )
{
	VALID_RET(pOwnerNPC, false);
	
	// Ÿ���� �ִ��� Ȯ��
	GEntity* pTargetEntity = pOwnerNPC->GetTargetActor();
	if ( pTargetEntity == NULL)
		return NULL;

	// ȸ�� ������ ������ �ִ��� Ȯ��
	GNPCInfo* pInfo = pOwnerNPC->GetNPCInfo();
	if ( (pInfo == NULL) || (pInfo->m_nLRotationID == 0) || (pInfo->m_nLRotationID == 0))
		return NULL;

	vec3 _dir = pTargetEntity->GetPos() - pOwnerNPC->GetPos();
	GMath::NormalizeZ0( _dir);
	float fAngle = pOwnerNPC->GetDir().AngleToXY( _dir);

	// ȸ���� Ż��Ʈ ����
	unsigned int nTalentID;
	float fRotAngle;

	if ( fAngle > pInfo->m_fRotationActAngle)
	{
		nTalentID = pInfo->m_nLRotationID;
		fRotAngle = pInfo->m_fRotationAngle;
	}
	else if ( fAngle < -pInfo->m_fRotationActAngle)
	{
		nTalentID = pInfo->m_nRRotationID;
		fRotAngle = -pInfo->m_fRotationAngle;
	}
	else
		return NULL;	// ȸ�����Ҷ��� �����ϱ�� ��


	// NPC ȸ��`
	if ( fRotAngle > -1000.0f  &&  fRotAngle < 1000.0f)
	{
		_dir.x = pOwnerNPC->GetDir().x * cos( fRotAngle) - pOwnerNPC->GetDir().y * sin( fRotAngle);
		_dir.y = pOwnerNPC->GetDir().y * cos( fRotAngle) + pOwnerNPC->GetDir().x * sin( fRotAngle);

		GMath::NormalizeZ0( _dir);
	}

	pOwnerNPC->SetDir( _dir);
	pOwnerNPC->SetFacingDir( _dir);

	GTalentInfo* pTalentInfo = gmgr.pTalentInfoMgr->Find(nTalentID);
	if (pTalentInfo == NULL)	
		return	NULL;

	GTaskBroker	task_broker;
	return task_broker.New_UseTalent(pOwnerNPC, pTalentInfo, MUID::Invalid(), false, false);
}

bool GTargetObserveRotater::HasRotateTalent( GEntityNPC* pOwnerNPC )
{
	GNPCInfo* pInfo = pOwnerNPC->GetNPCInfo();
	if ( (pInfo == NULL) || (pInfo->m_nLRotationID == 0) || (pInfo->m_nLRotationID == 0))
	{
		return false;
	}

	return true;
}