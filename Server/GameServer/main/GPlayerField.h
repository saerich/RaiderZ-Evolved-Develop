#pragma once

#include "CSTriggerInfo.h"
#include "GGateInfo.h"
#include "GPlayerFieldEntry.h"

class GEntityActor;
class GEntityPlayer;
class GPlayerFieldListener;
class GPlayerFieldMasterServer;
class GPlayerFieldShared;
class GPlayerFieldDynamic;
class GPlayerFieldLazyGateway;
class GPlayerFieldGateRetryer;

/// �÷��̾��� �ʵ� �̵� ����
class GPlayerField : public MTestMemPool<GPlayerField>
{
public:
	GPlayerField(GEntityPlayer* pOwner);
	~GPlayerField(void);

	void SetLisener(GPlayerFieldListener* pLisener);
	void Update(float fDelta=0.0f);

	bool Gate(int nDestFieldID, vec3 vecDestPos, vec3 vecDestDir, bool bRoute=true, CHANNELID nChannelID = INVALID_CHANNELID);
	bool GateToMarker(int nFieldID, int nMarkerID);
	bool GateToMarker(const MARKER_INFO& refMarker, bool bRouteToMe = true);
	bool GateToActor(GEntityActor* pActor);
	bool GateToSoulBidning();
	bool GateToTrial(int nDynamicFieldGroupID, bool bSingleEnter = false);
	bool GateToInnRoom();
	bool GateToPlayer(wstring strTarPlayerName);
	bool Summon(wstring strTarPlayerName);

	void Warp(vec3 vecDestPos, vec3 vecDestDir, bool bRoute=true);

	MUID GetDynamicFieldGroupUID() { return m_GateInfo.uidDynamicFieldGroup; }
	void SetDynamicFieldGroupUID(const MUID& uidDynamicFieldGroup);
	
	void SetGateInfo(const GGateInfo& GateInfo)		{ m_GateInfo = GateInfo; }
	GGateInfo GetGateInfo(void)						{ return m_GateInfo; }

	GPlayerFieldEntry&			GetFieldEntry()			{ return m_FieldEntry; }
	GPlayerFieldMasterServer&	GetFieldMasterServer()	{ return *m_pFieldMasterServer; }
	GPlayerFieldShared&			GetFieldShared()		{ return *m_pFieldShared; }
	GPlayerFieldDynamic&		GetFieldDynamic()		{ return *m_pFieldDynamic; }
	GPlayerFieldLazyGateway&	GetLazyGateway()		{ return *m_pFieldLazyGateway; }
	GPlayerFieldGateRetryer&	GetFieldGateRetryer()	{ return *m_pFieldGateRetryer; }

	// ���߿� �Լ�: ���� �������� �׻� �����մϴ�. --------------------------------------
	bool GateToRequestField(int nFieldId, vec3 vecDestPos, vec3 vecDestDir = vec3(0, 1, 0));
	bool GateToFirstField(void);
	bool GateAnywhere(void);
	// ------------------------------------------------------------------------------------

private:
	GEntityPlayer*				m_pOwner;				///< ������
	GPlayerFieldListener*		m_pLisener;				///< ������

	GPlayerFieldEntry			m_FieldEntry;			///< �ʵ� ���� ó��
	GPlayerFieldMasterServer*	m_pFieldMasterServer;	///< �����ͼ��� �����Ͽ� �ʵ� �̵� ó��
	GPlayerFieldShared*			m_pFieldShared;			///< ���� �ʵ�, ä�� �̵� ó��
	GPlayerFieldDynamic*		m_pFieldDynamic;		///< ���� �ʵ� �̵� ó��
	GPlayerFieldLazyGateway*	m_pFieldLazyGateway;	///< ���� �̵� ������
	GPlayerFieldGateRetryer*	m_pFieldGateRetryer;	///< �ʵ� �̵� ��õ� ��û��

	GGateInfo					m_GateInfo;				///< ����Ʈ ����

	bool	IsSameField(int nDestFieldID, CHANNELID nChannelID);
	bool	IsDynamicField(int nDestFieldID);

	bool	GateToSharedFIeld(int nFieldID, vec3 vecDestPos, vec3 vecDestDir, CHANNELID nChannelID=INVALID_CHANNELID);

	DYNAMIC_FIELD_TYPE GetDynamicFieldGroupType(MUID uidFieldGroup);
	int GetDynamicFieldGroupID(MUID uidFieldGroup);
};
