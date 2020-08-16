#include "stdafx.h"
#include "XGlueGamePartyFunction.h"
#include "XCommonFunction.h"
#include "XInteractionInfo.h"
#include "XPost_Action.h"
#include "XPost_Msg.h"

//��Ƽ�ʴ�
void XGlueGamePartyFunction::PartyInviteReq()
{
	if (XGetInteractionInfo().InteractionTargetUID ==  gvar.MyInfo.MyUID)
	{
		global.ui->OnSystemMsg(L"�ڱ� �ڽſ��� ��Ƽ�� ��û�� �� �����ϴ�.");
	}
	else
	{
		XPostPartyRequestInvite(XGetInteractionInfo().InteractionTargetUID);
	
		wstring strPlayerName = XCommonFunction::GetPlayerName(XGetInteractionInfo().GetInteractionSearchUID());
		global.ui->OnSystemMsg(L"{0}�Բ� ��Ƽ�� ��û �Ͽ����ϴ�.", FSParam(strPlayerName));
	}
}

void XGlueGamePartyFunction::PartyRequiredAnswer( bool bAccept )
{
	//XPostPartyRequestInviteQuestionRespond(bAccept);

	//char text[1024]="";
	//sprintf_s(text, "��Ƽ�ʴ븦 �������� �ʾҽ��ϴ�.");
}

void XGlueGamePartyFunction::PartyConfirmRequire()
{
	//��Ƽ���Կ�û
}

void XGlueGamePartyFunction::PartyLeave()
{
	//�켱 �ڱⰡ Ż���ϴ� �ɷ�.
	XPostPartyRequestLeave();
}

void XGlueGamePartyFunction::PartyFired(const char* szuid)
{
	if( szuid == NULL) return;

	MUID uid = _atoi64(szuid);
	XPostPartyRequestKick(uid);
}

void XGlueGamePartyFunction::PartyDivisionRule()
{

}

void XGlueGamePartyFunction::PartyMessage()
{
	wchar_t* pszText = {L"���� �־����ϴ�.�۾���"};
	XPostMsg_Party(pszText);
}

//��Ƽ��û
void XGlueGamePartyFunction::PartyDemand()
{
	//��Ƽ�� uid�� �ƴ϶� ��Ƽ�� uid�� ������ �Ѵ�.
	// �������̽����� ��Ƽ����� �����ϰ� �׸���� ��Ƽ�� �����ؼ�
	// ��Ƽuid�� ������.
	//XPostPartyRequestInviteForMe();
}

void XGlueGamePartyFunction::PartyDemandAnswer(bool bAccept)
{
	XPostPartyRequestInviteForMeQuestionRespond(bAccept);
}

__int64 XGlueGamePartyFunction::GetPartyLeaderUID()
{
	return gvar.MyInfo.Party.GetLeader().Value;
}

bool XGlueGamePartyFunction::IsPartyMemberOffline(const char* szuid)
{
	if( szuid == NULL) return NULL;

	MUID uid = _atoi64(szuid);
	return gvar.MyInfo.Party.IsPartyMemberOffline(uid);
}

bool XGlueGamePartyFunction::IsPartyMemberDead(const char* szuid)
{
	if( szuid == NULL) return NULL;

	MUID uid = _atoi64(szuid);
	return gvar.MyInfo.Party.IsPartyMemberDead(uid);
}

const char* XGlueGamePartyFunction::GetPartyMemberName(const char* szuid)
{
	if( szuid == NULL) return NULL;

	MUID uid = _atoi64(szuid);
	
	gvar.Temp.szTemp = MLocale::ConvUTF16ToAnsi(gvar.MyInfo.Party.GetPartyMemberName(uid));

	return gvar.Temp.szTemp.c_str();
}

int XGlueGamePartyFunction::GetPartyMemberHP( const char* szuid )
{
	if( szuid == NULL) return NULL;

	MUID uid = _atoi64(szuid);
	return gvar.MyInfo.Party.GetPartyMemberHP(uid);
}

int XGlueGamePartyFunction::GetPartyMemberEN( const char* szuid )
{
	if( szuid == NULL) return NULL;

	MUID uid = _atoi64(szuid);
	return gvar.MyInfo.Party.GetPartyMemberEN(uid);
}

int XGlueGamePartyFunction::GetPartyMemberSTA( const char* szuid )
{
	if( szuid == NULL) return NULL;

	MUID uid = _atoi64(szuid);
	return gvar.MyInfo.Party.GetPartyMemberSTA(uid);
}

const char* XGlueGamePartyFunction::GetPartyMemberLevel(const char* szuid)
{
	if( szuid == NULL) return NULL;

	MUID uid = _atoi64(szuid);
	return "None";//gvar.MyInfo.Party.GetPartyMemberLevel(uid);
}

const char* XGlueGamePartyFunction::GetPartyMemberClass(const char* szuid)
{
	if( szuid == NULL) return NULL;

	MUID uid = _atoi64(szuid);
	return "None";//gvar.MyInfo.Party.GetPartyMemberLevel(uid);
}

bool XGlueGamePartyFunction::IsPartyMemberHightlight( const char* szuid )
{
	if( szuid == NULL) return NULL;

	MUID uid = _atoi64(szuid);
	return gvar.MyInfo.Party.IsPartyMemberHightlight(uid);
}



