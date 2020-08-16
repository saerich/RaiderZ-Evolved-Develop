#pragma once

#include "MUID.h"

class ZPlayer;
class ZField;

struct PmUserData;

class ZPlayerLogic
{
protected:
public:
	ZPlayerLogic() {}
	virtual ~ZPlayerLogic() {}
	
	bool AddPlayer(const MUID& uidPlayer, AID nAID, const wstring& strUserID/*, PmUserData* pPmangUserData=NULL*/);
	void DeletePlayer(AID nAID);

	bool OnSelectChar(AID nAID, MUID uidPlayer);
	bool OnInWorld(MUID uidRequestGameServer, AID nAID, int nCID, MUID uidPlayer, const std::wstring& strPlayerName, int nGID);
	void LeaveGameServer(AID nAID);
	
	// Field ���� ����
	void PrepareGateField(ZPlayer* pPlayer, ZField* pField);
	void CancelPrepareGateField(ZPlayer* pPlayer);
	void InField(MUID uidRequestGameServer, CID nCID, MUID uidField, int nFieldID);
	void OutField(ZPlayer* pPlayer, MUID uidField);

	// Guild ���� ����
	void JoinGuild(CID cid, int nGID);
	void LeaveGuild(CID cid);

	void AllMemberDeleteInLoginServer();
	void AllMemberDeleteInGameServer(int nGameServerID);

private:
	bool InWorld_Check(ZPlayer* pPlayer);

};