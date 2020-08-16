#pragma once

#include "STransData.h"

/// �ٸ� ������ ���ؼ� �����ϴ� �÷��̾��� ������ ó���ϴ� Ŭ����
class GPlayerAcceptor
{
public:
	enum AcceptorType
	{
		FROM_LOGIN_SERVER = 0,
		FROM_GAME_SERVER,
		FAKE_LOGIN_FROM_GAME_SERVER_TO_LOGIN_SERVER,
	};
protected:
	// ���ӿ� �ʿ��� ��
	uint32		m_nReserveTime;
	MUID		m_uidConnectionKey;
	uint32		m_nEnterKey;

	// account ����
	uint32		m_nAID;
	wstring		m_strUserID;

public:
	GPlayerAcceptor();
	GPlayerAcceptor(MUID uidConnectionKey);
	virtual ~GPlayerAcceptor();

	// getter
	MUID	GetConnectionKey()		{ return m_uidConnectionKey; }
	uint32	GetEnterKey()			{ return m_nEnterKey; }
	uint32	GetReserveTime()		{ return m_nReserveTime; }

	uint32	GetAID()				{ return m_nAID; }
	wstring& GetUserID()				{ return m_strUserID; }
	
	virtual void OnTimeout() = 0;
	virtual AcceptorType GetType() = 0;
};



/*
 TODO: 
  - GPlayerGameStart_AutoRebirther �����͸�
 - GPlayerGameStart_FieldGater �����͸�


 */