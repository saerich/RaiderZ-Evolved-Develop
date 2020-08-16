#pragma once

struct TargetActorInfo
{
	UIID			m_nUIID;		// ��ǥ ��� UIID
	int				m_nMaxHP;		// �ִ� hp
	uint8			m_nHPPercent;	// hp(�ۼ�Ʈ)
	uint8			m_nLevel;		// npc level
	bool			m_bComabt;		// ������
	NPC_ICON_TYPE	m_nNPCIConType;	// �ο������� ���ؼ� �����ش�. ������ ���ñ�ȹ �Ƚ��Ǹ� �׶� ����

	unsigned long	m_time;			// ���� ���� �ð�

	TargetActorInfo(const TD_TARGET_INFO& tdInfo)
	{
		this->Copy(tdInfo);
	}

	TargetActorInfo(const TD_ENEMY_INFO& tdInfo)
	{
		this->Copy(tdInfo);
	}

	void Copy(const TD_TARGET_INFO& tdInfo)
	{
		m_nUIID		= tdInfo.m_nUIID;
		m_nMaxHP	= tdInfo.m_nMaxHP;
		m_nHPPercent= tdInfo.m_nHPPercent;
		m_nLevel	= tdInfo.m_nLevel;
		m_bComabt	= tdInfo.m_bComabt;
		m_nNPCIConType = tdInfo.m_nNPCIConType;
		m_time		= timeGetTime();
	}

	void Copy(const TD_ENEMY_INFO& tdInfo)
	{
		m_nUIID		= tdInfo.nUIID;
		m_nMaxHP	= tdInfo.nMaxHP;
		m_nHPPercent= tdInfo.nHPPercent;
		m_nLevel	= tdInfo.nLevel;
		m_bComabt	= false;		//�Ϻη� ���� �������̸� false
		m_nNPCIConType = NIT_NONE;
		m_time		= timeGetTime();
	}
};

class XTargetActorInfo
{
public:
	UIID			nMouseSearchUIID;		// ���콺 Ÿ�� UIID

	UIID			nSearchUIID;			// Ÿ�� UIID

	UIID			nTargetUIID;			// Ÿ�� ���� UIID
	bool			bLockOnTarget;			// Ÿ�� ����
	unsigned long	nLockOnTime;			// ���� �� �ð�

	unsigned long	nPostReqTime;			/// Ÿ������ ��û�� �ð�

private:
	map<UIID, TargetActorInfo*>		m_ActorMap;

	void	Add( const UIID nUIID, TD_ENEMY_INFO& tdInfo );
	void	Clear();

public:
	void	Init();
	void	Add( const UIID nUIID, TD_TARGET_INFO& tdInfo );
	bool	Find( const UIID nUIID );
	bool	Find_Time( const UIID nUIID );
	TargetActorInfo* Get( const UIID nUIID );
	void	AddEnemyList();

	bool	IsPostTime();
};
