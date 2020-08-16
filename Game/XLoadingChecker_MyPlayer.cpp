#include "stdafx.h"
#include "XLoadingChecker_MyPlayer.h"
#include "XMyPlayer.h"
#include "XGame.h"
#include "XGlobal.h"
#include "XModuleEntity.h"
#include "XCharacter.h"

bool XLoadingChecker_MyPlayer::CheckLoaded()
{
	if (gg.game == NULL) return true;
	if (gg.omgr == NULL) return true;

	XMyPlayer* pMyPlayer = gg.omgr->FindMyPlayer();
	if (pMyPlayer == NULL) return true;

	XModuleEntity* pEntity = pMyPlayer->GetModuleEntity();
	_ASSERT(pEntity);

	// XCharacter ������ ������Ʈ ���־�� ĳ���͸��ҽ� �ε��� Ȯ���Ҽ� �ִ�.
	XCharacter* pChar = pEntity->GetActor();
	_ASSERT(pChar);
	pChar->UpdatePartsNode();

	// Entity�κ��� �ε��� �� �Ǿ����� Ȯ���ϱ�
	pEntity->CheckLoadingCompleted();
	if (pEntity->IsLoadingCompleted() == false) return false;

	return true;
}