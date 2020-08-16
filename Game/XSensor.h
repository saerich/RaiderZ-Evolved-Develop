#pragma once

#include "XSensorInfo.h"

/// ���ӿ��� Ȱ��ȭ�� ���� �ν��Ͻ�
class XSensor
{
protected:
	XSensorInfo*		m_pSensorInfo;
	unsigned long		m_nID;
	GATE_TYPE			m_nGateType;
	MUID				m_uidEffect;

	void CreateSensorEffect(SENSOR_INFO* pInfo, GATE_TYPE nGateType);
	void DeleteSensorEffect();

public:
	XSensor(unsigned long nSensorID, XSensorInfo* pInfo, GATE_TYPE nGateType);
	~XSensor();
	void Refresh(GATE_TYPE nGateType);

	bool	IsEnteredSensor(vec3 vPos);

	virtual SENSOR_RESULT OnUpdate( bool bEnter, vec3 vPos );
	virtual bool OnEnter(vec3 vPos);
	virtual bool OnLeave(vec3 vPos);

	unsigned long GetID() { return m_nID; }
	XSensorInfo* GetInfo()	{ return m_pSensorInfo; }
	MUID GetEffectUID() { return m_uidEffect; }
};

