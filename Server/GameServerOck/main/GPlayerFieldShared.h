#pragma once

#include "GGateInfo.h"

class GPlayerField;
class GEntityPlayer;
class GPlayerFieldListener;
class GSharedField;

/// �÷��̾��� ä�� �̵� ó��
class GPlayerFieldShared : public MTestMemPool<GPlayerFieldShared>
{
private:
	GPlayerField*			m_pPlayerField;
	GEntityPlayer*			m_pOwner;					///< ������
	GPlayerFieldListener*	m_pLisener;					///< ������

	uint32					m_nPrepareChannelTime;		///< ä�� �����ϱ�� ������ �ð�
	CHANNELID				m_nPrepareChannelID;		///< ������ ä�� ID

	GSharedField*			GetEnterableSharedFIeld(int nReqFieldID, int nReqChannelID);

public:
	GPlayerFieldShared(GEntityPlayer* pOwner, GPlayerField* pPlayerField);
	~GPlayerFieldShared() {}

	void		SetListener(GPlayerFieldListener* pLisener)	{ m_pLisener = pLisener; }

	// ä���� �ٲٱ����� �غ�
	void		PrepareChangeChannel(CHANNELID nChannelID);
	// ä�� �̵� ��
	bool		IsPreparedChangeChannel();
	// ä���̵� ��û �ð� �ʱ�ȭ
	void		ClearPrepareChannelChange();

	// ä���̵� ��û�� �� �ð��� ��ȯ
	uint32		GetPrepareChannelTime()						{ return m_nPrepareChannelTime; }
	// ä���̵� ��û�� �� ä�� ���̵� ��ȯ
	CHANNELID	GetPrepareChannelID()						{ return m_nPrepareChannelID; }

	bool		GateRequest(int nReqFieldID, int nReqChannelID, vec3 vecDestPos, vec3 vecDestDir);
	bool		GateExecute(GSharedField* pField, vec3 vecDestPos, vec3 vecDestDir);

};
