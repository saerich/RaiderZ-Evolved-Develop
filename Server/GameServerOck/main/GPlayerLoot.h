#pragma once

class MRegulator;
class GEntityPlayer;
class GEntityNPC;

class GPlayerLoot : public MTestMemPool<GPlayerLoot>
{
public:
	GPlayerLoot(GEntityPlayer*	pOwner);
	~GPlayerLoot();

	void Update(float fDelta);

	void BeginByKillNPC(MUID uidNPC);
	void BeginByInteractNPC(MUID uidNPC, float fTime);
	void BeginByGatherNPC(MUID uidNPC, float fTime, int nLootID);
	void EndLoot();

	GEntityNPC* GetLootingNPC();

	bool IsLootingNPC(const MUID& uidNPC);
	bool IsILootRegulatorActive();

private:
	bool UpdateInteractionLoot(float fDelta);

private:
	GEntityPlayer*	m_pOwner;
	MRegulator*		m_pILootRegulator;	///< IET_LOOT �ҿ�ð� Ÿ�̸�
	MUID			m_LootingNPCUID;	///< �������� NPC UID
	int				m_nLootID;			///< ä�� ��, ��� �������̺�
};
