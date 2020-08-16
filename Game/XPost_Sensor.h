#ifndef _XPOST_SENSOR_H
#define _XPOST_SENSOR_H

// ���� ������� ����
inline void XPostSensorTriggerSelection(int nSelection)
{
	XPOSTCMD1(MC_TRIGGER_SENSOR_DESTINATION_SEL, MCommandParameterInt(nSelection));
}

// ��Ƽ�� ���� Ʈ���̾��ʵ� �̵� ����
inline void XPosSensorTriggerFollowWarp(int nSelection)
{
	if (nSelection != 0)	return;
	XPOSTCMD0(MC_TRIGGER_SENSOR_FOLLOW_PARTY_RES);
}

// �����ȿ��� ���ͷ�Ʈ
inline void XPostSensorInteract(vector<uint32>& vecEnteredSensorID)
{
	XPOSTCMD1(MC_TRIGGER_SENSOR_INTERACTION_REQ, MCommandParameterBlob(vecEnteredSensorID));
}

#endif