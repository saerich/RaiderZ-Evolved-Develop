#pragma once

#include "GField.h"
#include "MMemPool.h"

class GAutoPartyMgr;

//////////////////////////////////////////////////////////////////////////
// 
// GSharedField
//
//////////////////////////////////////////////////////////////////////////

class GSharedField : public GField
{
public:
	GSharedField(const MUID& uid, GWeatherMgr* pWeatherMgr);
	virtual ~GSharedField();

	// �����ʵ��� ����
	virtual bool		IsSharedField() override { return true; }
	// ��ȿ�� �����ʵ����� ����
	bool				IsInvalid() const;
	// ������ �����ʵ����� ���� (���� �ȵ�)
	bool				IsDefaultChannel() const;

	// ä�ξ��̵� ����
	void				SetChannelID(int id)	{ m_nChannelID = id; }
	// ä�ξ��̵� ��ȯ
	int					GetChannelID()	const	{ return m_nChannelID; }

	// ���� ������ �÷��̾� �ִ��ο�
	int					GetCapacity();

	// �ʵ忡 ������ �� �ִ��� ����
	virtual bool		IsEnterable();

	// ä���̵� ���� - StandAlone Mode ����
	void				ReserveChangeChannel_ForStandAlone(MUID uidPlayer);
	// ä���̵� ������� - StandAlone Mode ����
	void				CancelChangeChannel_ForStandAlone(MUID uidPlayer);
	// ä���̵� ������ �Ǿ��ִ��� ���� - StandAlone Mode ����
	bool				HasReservation_ForStandAlone( MUID uidPlayer );

	// �ڵ���Ƽ ������ ��ȯ
	GAutoPartyMgr*	 GetAutoPartyMgr() const { return m_pAutoPartyMgr; }
	
	virtual void		Update(float fDelta) override;
private:
	// ������ ���� �� (������ ������ �Ǿ��� ���)
	void DestroySelf();

	// ��ƼƼ�� ���ŵɶ� ȣ��
	virtual void RemoveEntity(GEntity* pEntity) override;

	// ��ƼƼ�� ������
	virtual void OnEntityLeaved(GEntity* pEntity);
	
private:
	// ä�� ���̵�
	CHANNELID m_nChannelID;
	GAutoPartyMgr*	m_pAutoPartyMgr;
};

class GSharedFieldMLeaf : public GSharedField, public MMemPool<GSharedFieldMLeaf>
{
public:
	GSharedFieldMLeaf(const MUID& uid, GWeatherMgr* pWeatherMgr)
		: GSharedField(uid, pWeatherMgr) {}
	virtual ~GSharedFieldMLeaf() {}
};

// Field�� SharedField�� �ٲ���
GSharedField*	ToSharedField(GField* pField);		
// Field�� ä�ξ��̵� ��ȯ, ä���� �� �� ���� ����� INVALID_CHANNELID ��ȯ
CHANNELID		GetChannelID(GField* pField);