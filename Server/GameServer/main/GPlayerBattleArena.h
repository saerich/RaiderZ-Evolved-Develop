#pragma once

#include "GBattleArena.h"

class GEntityActor;
class GEntityPlayer;

class GPlayerBattleArena : public MTestMemPool<GPlayerBattleArena>
{
public:
	GPlayerBattleArena(GEntityPlayer* pOwner);

	// �����忡 �� �ִ°�?
	bool IsEntered() const;
	// ������ ��⸦ �ϰ� �ִ°�?
	bool IsNowPlaying() const;
	// ������ �������� ���� �ִ°�?
	bool IsNowDisplaySocreboard() const;

	// ���� ��û
	bool RegisterEntry();
	// ���� ���
	bool DeregisterEntry();

	// ������ UID ����
	void SetBattleArenaUID(MUID uid)	{ m_uidBattleArena = uid; }
	MUID GetBattleArenaUID() const		{ return m_uidBattleArena; }

	// ������ ���� ��ȯ
	GBattleArena::Status GetStatus() const;

	// Owner�� �׾����� ȣ��
	void OnDie();

	// Owner�� ������ ��Ȱ�Ǿ����� ȣ��
	void OnRebirthBySelf();
	// Owner�� Ÿ�ο����� ��Ȱ�Ǿ����� ȣ��
	void OnRebirthByOther();
	// onwer�� ���� ������ ���
	void OnDisconnect();

private:
	GEntityPlayer*	m_pOwner;
	MUID			m_uidBattleArena;
};
