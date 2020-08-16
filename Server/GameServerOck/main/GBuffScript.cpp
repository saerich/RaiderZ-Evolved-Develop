#include "StdAfx.h"
#include "GBuffScript.h"
#include "GScriptManager.h"
#include "GScriptUtil.h"
#include "GEntityActor.h"
#include "GNullGlueActor.h"
#include "GBuffInfo.h"
#include "GLuaThisEnchanter.h"
#include "GConst.h"

const wchar_t* pszLuaBuffFuncnames[]={
	L"OnGain",
	L"OnLost",
	L"OnExpired",
};

#define LUA_CALLBACK_FUNCNAME_BUFF_GAIN					pszLuaBuffFuncnames[0]
#define LUA_CALLBACK_FUNCNAME_BUFF_LOST					pszLuaBuffFuncnames[1]
#define LUA_CALLBACK_FUNCNAME_BUFF_EXPIRED				pszLuaBuffFuncnames[2]

void GBuffScript::CallEventScript(MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo, const wstring& strFuncName)
{
	VALID(pTarget);
	VALID(pBuffInfo);

	if (false == pTarget->IsActor()) return;
	// �������� �ʹ� �̺�Ʈ�� ��Ƽ� �ݹ� ó������ �ʴ´�.
	if (pBuffInfo->IsAuraEffectBuff()) 
		return;

	GEntityActor* pTargetActor = ToEntityActor(pTarget);
	GEntityActor* pUserActor = pTargetActor->FindActor(uidUser);

	wstring strTableName = MLocale::ConvAnsiToUTF16(MAKE_LUATABLE_BUFF(pBuffInfo->m_nID));

	// �۷ι� �Լ��� ȣ��
	if (!WLUA->IsExistMemberFunc(MLocale::ConvUTF16ToAnsi(strTableName.c_str()).c_str(), MLocale::ConvUTF16ToAnsi(strFuncName.c_str()).c_str())) return;

	WMEMCALL3(WLUA, MLocale::ConvUTF16ToAnsi(strTableName.c_str()).c_str(), MLocale::ConvUTF16ToAnsi(strFuncName.c_str()).c_str(), NULL, TO_SAFE_LUAPTR(pUserActor), TO_SAFE_LUAPTR(pTargetActor), WNULL); 
}

void GBuffScript::OnGain(MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo)
{
	VALID(pTarget);
	VALID(pBuffInfo);

	CallEventScript(uidUser, ToEntityActor(pTarget), pBuffInfo, LUA_CALLBACK_FUNCNAME_BUFF_GAIN);
}

void GBuffScript::OnDuplicated( MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo )
{
	VALID(pTarget);
	VALID(pBuffInfo);

	CallEventScript(uidUser, ToEntityActor(pTarget), pBuffInfo, LUA_CALLBACK_FUNCNAME_BUFF_GAIN);
}

void GBuffScript::OnLost( MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo, bool bRoute )
{
	VALID(pTarget);
	VALID(pBuffInfo);

	CallEventScript(uidUser, ToEntityActor(pTarget), pBuffInfo, LUA_CALLBACK_FUNCNAME_BUFF_LOST);
}

void GBuffScript::OnExpired(MUID uidUser, GEntitySync* pTarget, GBuffInfo* pBuffInfo)
{
	VALID(pTarget);
	VALID(pBuffInfo);

	CallEventScript(uidUser, ToEntityActor(pTarget), pBuffInfo, LUA_CALLBACK_FUNCNAME_BUFF_EXPIRED);
}

void GBuffScript::MakeTable(int nBuffID)
{
	string strTableName = MAKE_LUATABLE_BUFF(nBuffID);
	MAKE_TABLE(strTableName.c_str());
}

void GBuffScript::RunScript()
{
	string strFileName = MAKE_LUAFILE_BUFF();	

	// ��ũ��Ʈ ��ó��
	string strScript;
	string strScriptFilename = MAKE_LUAFILE(MLocale::ConvUTF16ToAnsi(PATH_SCRIPT).c_str(), strFileName);
	GLuaThisEnchanter enchanterThis;

	for (size_t i=0; i<ArraySize(pszLuaBuffFuncnames); ++i)
		enchanterThis.AddMatchMemFunc(MLocale::ConvUTF16ToAnsi(pszLuaBuffFuncnames[i]));

	if (!enchanterThis.ConvertFromFile(strScriptFilename, strScript))
		return;

	if (strScript.length() == 0)
		return;

	// ��ũ��Ʈ ����
	WRUNSTRING(WLUA, strScript);

	if (IsRunForTest())
	{
		if (GConst::DEBUG_CREATE_CONVERT_LUAFILE)
		{
			// ����׿� ���� ����
			enchanterThis.SaveToFile(strScriptFilename+".convert.txt", strScript);
		}
	}
}