#ifndef _XPOST_GUILD_H
#define _XPOST_GUILD_H

#include "XNetClient.h"
#include "XCommandTable.h"

// ��� ����
inline void XPostGuild_Create(const wchar_t* szName)
{
	XPOSTCMD1(MC_GUILD_CREATE_REQ, MCommandParameterWideString(szName));
}

// ��� �ʴ� ��û
inline void XPostGuild_Invite_Req(const MUID& uidGuest)
{
	XPOSTCMD1(MC_GUILD_INVITE_REQ, MCommandParameterUID(uidGuest));
}

// ��� �ʴ� ����
inline void XPostGuild_Invite_Res(bool bAccept)
{
	XPOSTCMD1(MC_GUILD_INVITE_RES, MCommandParameterBool(bAccept));
}

// ��� Ż��
inline void XPostGuild_Leave()
{
	XPOSTCMD0(MC_GUILD_LEAVE_REQ);
}

// ��� ����
inline void XPostGuild_Kick(const wchar_t* szTargetName)
{
	XPOSTCMD1(MC_GUILD_KICK_REQ, MCommandParameterWideString(szTargetName));
}

// ��� ��ü
inline void XPostGuild_Destroy()
{
	XPOSTCMD0(MC_GUILD_DESTROY_REQ);
}

// �渶 ����
inline void XPostGuild_ChangeMaster(const wchar_t* szNewMasterName)
{
	XPOSTCMD1(MC_GUILD_CHANGE_MASTER_REQ, MCommandParameterWideString(szNewMasterName));
}

// ��� ������ �Ա� ��û
inline void XPostGuild_DepositStorageMoney(int nMoney)
{
	XPOSTCMD1(MC_GUILD_DEPOSIT_STORAGEMONEY_REQ, MCommandParameterInt(nMoney));
}

// ��� ������ ���� ��û
inline void XPostGuild_WithdrawStorageMoney(int nMoney)
{
	XPOSTCMD1(MC_GUILD_WITHDRAW_STORAGEMONEY_REQ, MCommandParameterInt(nMoney));
}

#endif