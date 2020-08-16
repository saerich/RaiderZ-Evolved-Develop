#pragma once

class GEntityPlayer;
class GEntityActor;
class PLAYER_INFO;
struct MARKER_INFO;

class GRebirthSystem : public MTestMemPool<GRebirthSystem>
{
public:		
	bool RebirthToPlace(GEntityActor* pUser, GEntityPlayer* pTarget, float fRecoverRate);	///< ���ڸ� ��Ȱ	
	bool RebirthToPlaceInBattleArena(GEntityPlayer* pPlayer);								///< ��Ʋ �Ʒ������� ���ڸ� ��Ȱ
	bool AutoRebirthInBattleArena(GEntityPlayer* pPlayer, vec3 vPos, vec3 vDir);			///< ��Ʋ �Ʒ������� �ڵ� ��Ȱ

	bool RebirthToSoulBinding(GEntityPlayer* pPlayer);					///< ������ ��ȥ�� ��ġ�� ��Ȱ		
	bool RebirthToNearSoulBinding(GEntityPlayer* pPlayer);				///< ���� ����� ��ȥ�� ��ġ�� ��Ȱ		

	bool RebirthToCheckPoint(GEntityPlayer* pPlayer);					///< �������� ��ġ�� ��Ȱ
	bool RebirthToQPVP(GEntityPlayer* pPlayer);							///< QPVP��Ȱ�������� ��Ȱ

	bool RebirthForGM(GEntityPlayer* pPlayer);							///< ��� ��ɾ�� ��Ȱ

	void ApplyRebirthRecoverRate(GEntityPlayer* pPlayer, float fRecoverRate);
	void ApplyRebirthBuff(GEntityPlayer* pPlayer, int nBuffID1, int nBuffID2=0, int nBuffTargetLevel=0);

private:
	bool Rebirth(GEntityActor* pUser, GEntityPlayer* pTarget, int nFieldID, vec3 vPos, vec3 vDir, float fRecoverRate, int nBuffID1=0, int nBuffID2=0, int nBuffTargetLevel=0);
	void Route(GEntityPlayer* pPlayer, int nFieldID, vec3 vPos, vec3 vDir);

	const MARKER_INFO* GetNearSoulBindingMarker(GEntityPlayer* pPlayer);
};
