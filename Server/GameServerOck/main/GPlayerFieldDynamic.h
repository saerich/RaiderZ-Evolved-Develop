#pragma once

#include "CSTriggerInfo.h"


class GEntityPlayer;
class GPlayerField;
class GDynamicFieldMaster;
class GPlayerFieldListener;
enum DYNAMIC_FIELD_TYPE;
struct GATE_INFO_DYNAMIC;

/// �÷��̾��� ���� �ʵ� �̵� ó��
class GPlayerFieldDynamic
{
protected:
	GPlayerField*			m_pPlayerField;
	GEntityPlayer*			m_pOwner;					///< ������
	GPlayerFieldListener*	m_pLisener;					///< ������

	CHANNELID				m_EnterChannelID;	///< �����ʵ忡 �����ϱ� �� �����ʵ��� ä��ID
	MARKER_INFO				m_EnterPos;			///< �����ʵ� ��������
	MARKER_INFO				m_Checkpoint;		///< �����ʵ忡�� �׾��� �� ��Ȱ�Ǵ� üũ����Ʈ

	MUID					GetEnterableDynamicFieldGroup(DYNAMIC_FIELD_TYPE eType, int nFieldGroupID, bool bIsSingleEnter);

	MUID					GetFieldGroupWhenReconnect();
	MUID					FindEnterableTrialField(int nFieldGroupID, bool bIsSingleEnter);
	MUID					FindEnterableBattleArena(int nFieldGroupID);

	bool					IsExistLocalServer(MUID uidFieldGroup);
	bool					IsValidFieldGroupWith(MUID uidDestFieldGroup, DYNAMIC_FIELD_TYPE eType, int nFieldGroupID);

	virtual void			_EndGateExecute(GDynamicFieldMaster* pFieldGroup) {}

public:
	GPlayerFieldDynamic(GEntityPlayer* pOwner, GPlayerField* pPlayerField);
	~GPlayerFieldDynamic() {}

	void SetListener(GPlayerFieldListener* pLisener)			{ m_pLisener = pLisener; }

	// �����ʵ� �������� ����
	void SetEnterPos(const MARKER_INFO& EnterPos, CHANNELID nEnterChannelID=INVALID_CHANNELID);
	void ClearEnterPos();
	// �����ʵ� ��Ȱ���� ����
	void SetCheckpoint(const MARKER_INFO& markerCheckpoint)		{ m_Checkpoint = markerCheckpoint; }
	void ClearCheckpoint();

	// �����ʵ� �������� ��ȯ
	const MARKER_INFO& GetEnterPos() const						{ return m_EnterPos; }
	int GetEnterPosChannelID()									{ return m_EnterChannelID; }
	// �����ʵ� ��Ȱ���� ��ȯ
	const MARKER_INFO& GetCheckpoint()							{ return m_Checkpoint; }

	// �����ʵ� ������������ �̵�
	bool GateToEnterPos(int nDynamicFieldID);
	// �����ʵ� ��Ȱ�������� �̵�
	bool GateToCheckPoint();

	// ���� �ʵ� �̵� ��û
	bool GateRequest(const GATE_INFO_DYNAMIC* pGateInfo);
	virtual bool GateRequest(DYNAMIC_FIELD_TYPE eType, int nFieldGroupID, int nFieldID, vec3 vecDestPos, vec3 vecDestDir, bool bSingleEnter=false);

	/// ���� �̵� ó��
	bool GateExecute(MUID uidFieldGroup, int nFieldID, vec3 vecDestPos, vec3 vecDestDir);
};
