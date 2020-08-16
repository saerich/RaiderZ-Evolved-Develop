#pragma once

#include "MMemPool.h"

class GEntityNPC;
class GEntityPlayer;

class GNPCInteraction : public MMemPool<GNPCInteraction>
{
private:
	GEntityNPC*		m_pOwner;
	set<MUID>		m_setInteractingPlayer;		///< ���ͷ������� �÷��̾�	
	bool			m_bEnable;					///< ���� ���ͷ��� ������ ��������
	MUID			m_nLastInteractPlayerUID;	///< ���������� ���ͷ����� �÷��̾�

	bool			m_bEnabledBeforeCombat;		///< �����ϱ� ���� ���ͷ����� ������ ���� ������

public:
	GNPCInteraction(GEntityNPC* pOwner);

	void Begin(GEntityPlayer* pPlayer);
	void End(GEntityPlayer* pPlayer);

	set<GEntityPlayer*> GetInteractingPlayer();
	bool IsInteractingPlayer(GEntityPlayer* pPlayer);

	GEntityPlayer* GetLastInteractPlayer();

	void Enable();
	void Disable();
	bool IsEnable() { return m_bEnable; }
	bool IsDisable() { return !m_bEnable; }

	void DisableByCombatEnter();
	void EnableByCombatExit();	
};
