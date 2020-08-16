#pragma once

#include "GGateInfo.h"

class GEntityPlayer;
class GPlayerField;
struct MARKER_INFO;
class GEntityActor;

class GPlayerFieldLazyGateway : public MTestMemPool<GPlayerFieldLazyGateway>
{
public:
	GPlayerFieldLazyGateway(GEntityPlayer* pOwner, GPlayerField* pPlayerField);
	~GPlayerFieldLazyGateway();

	bool IsScheduled() { return m_bScheduled; }
	void ProcessScheduledGate();

	// Lazily �Լ� ��. Update() ����Ÿ�ӿ� ������.
	bool GateToMarker(int nFieldID, int nMarkerID);
	void GateToMarker(const MARKER_INFO* Marker);
	bool Gate(int nDestFieldID, vec3 vecDestPos, vec3 vecDestDir);
	bool GateToActor(GEntityActor* pActor);
	bool GateToTrial(int nDynamicFieldGroupID, bool bSingleEnter);
	bool GateToInnRoom();

private:
	GEntityPlayer*	m_pOwner;		///< ������
	GPlayerField*	m_pPlayerField;

	bool			m_bScheduled;

	void			SetSchedule(bool bScheduled)	{ m_bScheduled = bScheduled; }

	/// Lazy Gate Info
	struct LAZY_GATE_INFO
	{
		int		m_nFieldID;
		vec3	m_vPos;
		vec3	m_vDir;

		/// Dynamic Field �� ���� ���
		DYNAMIC_FIELD_TYPE m_eType;
		int		m_nFieldGroupID;
		bool	m_bSingleEnter;

		LAZY_GATE_INFO()
		{
			Reset();
		}
		void Reset()
		{
			m_nFieldID = INVALID_ID;
			m_nFieldGroupID = INVALID_ID;
			m_bSingleEnter = false;
		}
		void Set(int nFieldID, vec3 vPos, vec3 vDir)
		{
			m_nFieldID = nFieldID;
			m_vPos = vPos;
			m_vDir = vDir;
		}
		void Set(DYNAMIC_FIELD_TYPE eType, int nFieldGroupID, int nFieldID, vec3 vPos, vec3 vDir, bool bSingleEnter)
		{
			m_eType = eType;
			m_nFieldGroupID = nFieldGroupID;
			m_nFieldID = nFieldID;
			m_vPos = vPos;
			m_vDir = vDir;
			m_bSingleEnter = bSingleEnter;
		}
	};
	LAZY_GATE_INFO	m_tdLazyGateInfo;

};
