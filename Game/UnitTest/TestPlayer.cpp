#include "stdafx.h"
#include "XPlayer.h"
#include "XMyPlayer.h"
#include "XNetPlayer.h"

SUITE(Player)
{
	TEST(TestPlayer_IsCollidableTo)
	{
		XNetPlayer		player1(MUID(34,4), 0);
		XMyPlayer		player2(MUID(34,5), 0);

		// �⺻�� �浹 ����
		CHECK_EQUAL(false, player1.IsCollidableTo(&player2));
		CHECK_EQUAL(false, player2.IsCollidableTo(&player1));

		// Duel ���̸� �浹üũ ��
		player1.SetDuel(true, player2.GetUID());
		player2.SetDuel(true, player1.GetUID());

		CHECK_EQUAL(true, player1.IsCollidableTo(&player2));
		CHECK_EQUAL(true, player2.IsCollidableTo(&player1));

		player1.SetDuel(false, MUID::ZERO);
		player2.SetDuel(false, MUID::ZERO);

		CHECK_EQUAL(false, player1.IsCollidableTo(&player2));
		CHECK_EQUAL(false, player2.IsCollidableTo(&player1));


		// battle arena�� ������ �浹 üũ ��
		gvar.MyInfo.BattleArenaInfo.SetInBattle();

		CHECK_EQUAL(true, player1.IsCollidableTo(&player2));
		CHECK_EQUAL(true, player2.IsCollidableTo(&player1));

		gvar.MyInfo.BattleArenaInfo.SetOutBattle();
		CHECK_EQUAL(false, player1.IsCollidableTo(&player2));
		CHECK_EQUAL(false, player2.IsCollidableTo(&player1));

	}
}