#include "stdafx.h"
#include "GEntityActor.h"
#include "GEntityNPC.h"
#include "MockField.h"
#include "GCommonFixture.h"
#include "GGlobal.h"
#include "GPlayerSystem.h"
#include "GConst.h"
#include "GTestForward.h"

SUITE(EntityPlayer)
{
	//TEST_FIXTURE(CommonFixture_FN, RebirthAftereffectBuff)
	//{
	//	test.buff.NewBuffInfo(GConst::REBIRTH_AFTEREFFECT_BUFFID);
	//	test.buff.NewBuffInfo(GConst::INVINCIBLE_BUFF_ID);
	//	GEntityPlayer* pPlayer = GUTHelper::NewEntityPlayer(m_pField, vec3(0.0f, 1000.0f, 0.0f));
	//	pPlayer->SetPlayerGrade(PLAYER_GRADE_NORMAL); // �Ϲ� �÷��̾�
	//	pPlayer->doDie();
	//	CHECK_EQUAL(true, pPlayer->IsDead());
	//	gsys.pPlayerSystem->Rebirth(pPlayer, pPlayer->GetPos(), true);
	//	CHECK_EQUAL(false, pPlayer->IsDead());
	//	CHECK_EQUAL(true, pPlayer->HasBuff(GConst::REBIRTH_AFTEREFFECT_BUFFID));	// ��Ȱ ������ ���� ����
	//	CHECK_EQUAL(true, pPlayer->HasBuff(GConst::INVINCIBLE_BUFF_ID));	// ���� ���� ����
	//}
}