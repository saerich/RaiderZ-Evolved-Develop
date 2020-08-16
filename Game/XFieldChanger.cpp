#include "stdafx.h"
#include "XFieldChanger.h"
#include "XBaseApplication.h"
#include "XPost_Local.h"
#include "XFieldInfoHelper.h"
#include "XInteractionInfo.h"

void XFieldChanger::Change( int nFieldID, int nChannelID, vec3 vPos, vec3 vDir, int nIntroCutsceneID )
{
	if (gg.game == NULL)
	{
		_ASSERT(PLAY_FRAME_ID != global.app->GetCurrGameFrame());
		XPostLocal_ChangeFrameFrame(PLAY_FRAME_ID);
	}

	if (gvar.Network.IsExistDirectLoginFieldInfo())
	{
		// �ٷ� �α��� �ÿ� ���� ������ �ʱ�ȭ�Ѵ�.
		// �����̵��� �߻�������, �ٽ� ������ ���� �������� �ʿ��ϴ�.
		gvar.Network.ResetDirectLoginFieldInfo();
	}


	bool bIsChangeChannel = false;

	if (gvar.World.GetFieldID() == nFieldID && gvar.World.nChannelID != nChannelID)
		bIsChangeChannel = true;

	gvar.World.SetNextMapInfo(nFieldID, vPos, vDir);
	gvar.World.nChannelID	= nChannelID;
	gvar.World.SetIntroCutSceneID(nIntroCutsceneID);

	XFieldInfoHelper::CheckPointFieldIDReset(nFieldID);

	// 	if(global.script)
	// 		global.script->GetGlueGameEvent().OnDynamicTimeHide();

	if (bIsChangeChannel)
	{
		if ( global.script)		global.script->GetGlueGameEvent().OnGameEvent( "CHANNEL", "CHANGED", nChannelID);
	}

	if (global.app) global.app->MessageToFrame(L"ChangeField");

	//	XPostLocal_OnChangeField(nFieldID, vPos, vDir);
	//gg.game->ChangeMap(nFieldID, vPos, vDir);

	XGetInteractionInfo().stTargetActorInfo.Init();
	if(global.script)	global.script->GetGlueGameEvent().OnEnemyInfoRefresh();

	global.ui->MiniMapInfoRefresh();
}
