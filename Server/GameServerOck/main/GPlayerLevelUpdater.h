#pragma once

class GEntityPlayer;

class GPlayerLevelUpdater
{
public:
	GPlayerLevelUpdater(GEntityPlayer* pPlayer);	
	void LevelUp(const int nGainXP, const int nNpcID);
	void LevelUpForDBTask(int nLevel, int nExp, int nGainExp);

	void SetLevelForGM(int nNewLevel, int nNewExp);		///< ������ ���� ����	

private:
	GEntityPlayer*		m_pPlayer;
};