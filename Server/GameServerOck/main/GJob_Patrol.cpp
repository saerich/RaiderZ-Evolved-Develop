#include "stdafx.h"
#include "GJob_Patrol.h"
#include "GField.h"
#include "GEntityNPC.h"
#include "GModuleAI.h"
#include "GColtMgr.h"
#include "GScriptManager.h"
#include "GScriptGroup_Field.h"

GJob_Patrol::GJob_Patrol(GModuleAI* pmAI, MARKER_LIST& MarkerList, PATROL_TYPE PatrolType) 
: GJob(pmAI)
, m_nCurrMarkerIndex(0)
, m_bReverseOrder(false)
, m_bComplete(false)
, m_bLastDoingIdle(true)
, m_PatrolType(PatrolType)
, m_bUseIdleColt(true)	// �Ϲ� ��Ʈ��, Idle ��Ʈ �����
{
	InsertFromMarkerIDList(MarkerList);
}

GJob_Patrol::GJob_Patrol(GModuleAI* pmAI, const vector<MARKER_INFO>& MarkerList, PATROL_TYPE PatrolType) 
: GJob(pmAI)
, m_MarkerList(MarkerList)
, m_nCurrMarkerIndex(0)
, m_bReverseOrder(false)
, m_bComplete(false)
, m_bLastDoingIdle(true)
, m_PatrolType(PatrolType)
, m_bUseIdleColt(false)	// �ν���Ʈ ��Ʈ��, idle ��Ʈ ����ȵ�
{
}

GJob_Patrol::~GJob_Patrol()
{

}

void GJob_Patrol::InsertFromMarkerIDList( MARKER_LIST& listMarkers )
{
	m_MarkerList.clear();
	
	GField* pField = GetOwnerNPC()->GetField();
	VALID(pField);

	const GFieldInfo* pFieldInfo = pField->GetInfo();
	VALID(pFieldInfo);

	for each (MARKER_ID id in listMarkers)
	{
		const MARKER_INFO* pMarker = pFieldInfo->FindMarker(id);
		if (!pMarker)
		{
			mlog3("invalid marker id(field: %d, npc: %d, marker_id: %d).\n", GetOwnerNPC()->GetFieldID(), GetOwnerNPC()->GetID(), id);
			continue;
		}
		m_MarkerList.push_back(*pMarker);
	}
}


bool 
GJob_Patrol::GetCurrentPatrolMarker(MARKER_INFO& Marker)
{
	if (m_MarkerList.empty())		
		return false;

	if (m_nCurrMarkerIndex >= m_MarkerList.size())
		return false;
	
	if (m_nCurrMarkerIndex < 0) 
		return false;

	Marker = m_MarkerList[m_nCurrMarkerIndex];

	return true;
}

bool GJob_Patrol::IsEndedMarker()
{
	if (m_bReverseOrder)
	{
		return m_nCurrMarkerIndex <= 0;
	}

	return m_nCurrMarkerIndex >= m_MarkerList.size();
}

bool
GJob_Patrol::AdvanceMoveMarker()
{
	if (m_MarkerList.empty())
		return false;
	
	GField* pField = GetOwnerNPC()->GetField();
	if (pField == NULL)
	{
		return false;
	}

	const GFieldInfo* pFieldInfo = pField->GetInfo();
	if (pFieldInfo == NULL)
	{
		return false;
	}

	MARKER_INFO Marker;
	if (!GetCurrentPatrolMarker(Marker))
	{
		_ASSERT(false);
		return false;
	}

	bool bTaskStarted = false;
	if (AT_SKY == GetOwnerNPC()->GetNPCInfo()->nAltitude)
	{
		bTaskStarted = 
			SetRunningTask(m_TaskBroker.New_MoveStraight(GetOwnerNPC(), Marker.vPoint, 0.0f));
	}
	else
	{
		bTaskStarted = 
			SetRunningTask(m_TaskBroker.New_MoveDefinite(GetOwnerNPC(), Marker.vPoint, 0.0f));
	}

	if (!bTaskStarted)
		return false;	// �½�ũ ���� ����

	return true;
}

bool GJob_Patrol::OnStart()
{
	PFI_BLOCK_THISFUNC(1906);

	// ���� ���� �ʱ�ȭ
	m_bComplete = false;

	if (!AdvanceMoveMarker())
		m_bComplete = true;
		
	return true;
}

GJobResult GJob_Patrol::OnRun(float fDelta)
{
	if (m_bComplete) 
	{
		return JR_COMPLETED;
	}

	return JR_RUNNING;
}

void GJob_Patrol::OnTaskComplete(GTaskID nTaskID)
{
	PFI_BLOCK_THISFUNC(9076);

	if (m_bUseIdleColt)
	{
		m_bLastDoingIdle = !m_bLastDoingIdle;
	}
	else
	{
		m_bLastDoingIdle = true;	// �׻� idle ��Ʈ�� ������ ��ó�� ó��
	}

	CallScript();

	if (m_bLastDoingIdle)
	{
		RunAdvance();
	}
	else
	{
		if (!OnIdle())
		{
			RunAdvance();
		}
	}	
}

void GJob_Patrol::OnCancel()
{
	GetOwnerNPC()->RouteMoveStop();
	IncreaseMarkerIndex();
}

bool GJob_Patrol::OnIdle()
{
	GModuleAI* pModuleAI = m_pOwner;
	VALID_RET(pModuleAI, false);

	GTask* pNextTask = pModuleAI->GetColt()->RunColtIdleForJob(GetOwnerNPC());
	if (pNextTask)
	{
		SetRunningTask(pNextTask);
		return true;
	}

	return false;
}

void GJob_Patrol::RunAdvance()
{
	if (!IncreaseMarkerIndex())
		return;

	AdvanceMoveMarker();
}

bool GJob_Patrol::IncreaseMarkerIndex()
{
	if (m_bComplete)
		return false;

	if (m_bReverseOrder)
		m_nCurrMarkerIndex--;
	else
		m_nCurrMarkerIndex++;

	if (IsEndedMarker())
	{
		switch (m_PatrolType)
		{
		case PT_LOOP:
			{
				m_nCurrMarkerIndex = 0;

				return true;
			}break;
		case PT_ONCE:
			{
				// ������ �ε����� ����
				m_nCurrMarkerIndex = m_MarkerList.size()-1; 
				m_bComplete = true;

				return false;
			}break;
		case PT_ONCE_RETURN:
			{
				m_nCurrMarkerIndex = 0;
				m_bComplete = true;

				return true;
			}break;
		case PT_LOOP_BACKORDER:
			{
				m_bReverseOrder = !m_bReverseOrder;

				// �ݴ�� ������ �ε����� �ϳ� �� ������ ����
				if (m_bReverseOrder)
					m_nCurrMarkerIndex = m_MarkerList.size()-2;

				return true;
			}break;
		default:
			DCHECK(false && L"���ǵ��� ���� ��Ʈ�� Ÿ��");
		}
	}
	
	return true;
}

string GJob_Patrol::GetParamString() const
{
	string strRet = "Type: ";
	if (m_PatrolType >= 0 && m_PatrolType < PT_SIZE)
	{
		strRet += PATROL_TYPE_STR[m_PatrolType];
	}
	else
	{
		strRet += "undefined";
	}
	strRet += ", Markers = ";
	
	char buff[512];
	for each (const MARKER_INFO& each in m_MarkerList)
	for (size_t i=0; i<m_MarkerList.size(); ++i)
	{
		const MARKER_INFO& each = m_MarkerList[i];
		_itoa_s((int)each.nID, buff, 10);
		strRet += buff;
		if (i < m_MarkerList.size()-1)
			strRet += ", ";
	}
	return strRet;
}

void GJob_Patrol::CallScript()
{
	MARKER_INFO markerInfo;
	if (!GetCurrentPatrolMarker(markerInfo)) return;	
	GField* pField = GetOwnerNPC()->GetField();
	if (NULL == pField) return;

	GetScriptField()->OnMarkerArrive(pField->GetDelegator(), markerInfo.nID, GetOwnerNPC()->GetDelegator());
}
