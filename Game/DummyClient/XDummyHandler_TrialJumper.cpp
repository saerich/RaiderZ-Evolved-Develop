#include "stdafx.h"
#include "XDummyHandler_TrialJumper.h"
#include "XBirdDummyAgent.h"
#include "XFieldInfo.h"

MCommandResult XDummyHandler_TrialJumper::OnCommand(XBirdDummyAgent* pAgent, MCommand* pCommand)
{
	XBirdClient* pClient = pAgent->GetClient();
	switch(pCommand->GetID())
	{
	case MC_FIELD_START_GAME:		
		{
			MakeFieldList();

			if (pAgent->IsStayTrialField())
			{
				// �����ʵ�� ����
				int nFieldID = pAgent->GetPreStayFieldID();
				vec3 vPos = pAgent->GetLoginFieldStartPos();
				XBIRDPOSTCMD2(pClient, MC_GM_MOVE_REQ, MCommandParameterInt(nFieldID), MCommandParameterVector(vPos));				
			}
			else
			{
				// Ʈ���̾� �ʵ� ����
				int nFieldGroupID = GetRandomFieldGroupID();
				if (nFieldGroupID == INVALID_ID)	return CR_ERROR;

				XBIRDPOSTCMD1(pClient, MC_DYNAMIC_FIELD_ENTER_REQ, MCommandParameterInt(nFieldGroupID));
			}
		}		
		return CR_TRUE;
	}

	return CR_FALSE;
}

void XDummyHandler_TrialJumper::MakeFieldList(void)
{
	if (!m_vecFieldGroupID.empty())	return;

	// FieldGroupID ���� - Ŭ���̾�Ʈ���� FieldGroup ������ �����ϴ� å�Ӱ�ü�� ����.
	// ���� ������������ ���Ǵ� ���� FieldGroup ������ �����Ѵ�.
	set<int> setFieldGroupID;

	CSFieldList& rFieldList = info.field->GetFieldList();
	for(CSFieldList::iterator it = rFieldList.begin(); it != rFieldList.end(); it++)
	{		
		// �ʵ������� �д´�.
		CSFieldFileInfo* pInfo = it->second;
		if (pInfo == NULL)			continue;
			
		int nFieldID = it->first;
		auto_ptr<XFieldInfo> pFieldInfo(info.field->NewFieldInfo(nFieldID));		
		if (pFieldInfo.get() == NULL)		continue;


		// �������� ������ ����, �̵������� FieldGroup�� �����Ѵ�.
		for each(SENSOR_INFO_MAP_BY_ID::value_type pairSensor in pFieldInfo->m_mapSensorByID)
		{		
			SENSOR_INFO& sensorInfo = pairSensor.second;						
			for each(SENSOR_WARP_INFO warpInfo in sensorInfo.vecWarpInfos)
			{
				int nFieldGroupID = warpInfo.nFieldGroupID;
				if (nFieldGroupID == INVALID_ID)	continue;
				
				setFieldGroupID.insert(nFieldGroupID);
			}
		}
	}

	// ���� ����
	m_vecFieldGroupID.resize(setFieldGroupID.size());
	copy(setFieldGroupID.begin(), setFieldGroupID.end(), m_vecFieldGroupID.begin());
}

int XDummyHandler_TrialJumper::GetRandomFieldGroupID(void)
{
	if (m_vecFieldGroupID.empty()) return INVALID_ID;
	
	int nRearIndex = m_vecFieldGroupID.size() - 1;	
	int nRandomIndex = RandomNumber(0, nRearIndex);
	
	return m_vecFieldGroupID[nRandomIndex];
}
