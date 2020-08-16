#include "stdafx.h"

#include "XItem.h"
#include "XMyInventory.h"

using namespace minet;
#include "XDummyBot_FillInventory.h"
#include "XBirdDummyAgent.h"

#pragma warning(disable:4996)
#include "XDummyMaster.h"
#pragma warning(default:4996)

SUITE(XDummyBot_FillInventory)
{
	TEST(XDummyBot_FillInventory)
	{
		const int BOT_ID = 1;

		MCommandCommunicator		cmdCommunicator;
		XBirdDummyAgent				agent;
		XDummyAgentInfo				agentInfo;
		XBirdDummyAgentMgr			agentMgr;
		XDummyBot_FillInventory*	fillInventory = new XDummyBot_FillInventory(&agent, &agentInfo);
		assert(NULL!=fillInventory);

		AddCommonCommandTable();

		agent.Create(agentInfo, BOT_ID, L"testman", L"testserver", L"testlogin", &agentMgr);
		agent.AddBot(fillInventory);

		//todo �������� �׽�Ʈ ���̽��� ����� ���� �� ��(������ ���� �پ��ϰ� ��)
		TD_MYINFO					info;
		vector<TD_ITEM>				inventory;
		vector<TD_ITEM>				equipments;
		vector<TD_ITEM_SLOT_INFO>	reservedDummySlotInfos;
		vector<int>					talents;
		vector<TD_PLAYERQUEST>		quests;
		vector<TD_PALETTE>			palette;
		vector<TD_FACTION>			factions;
		vector<int>					recipes;

		MCommand* cmd = cmdCommunicator.NewCommand(MC_CHAR_MYINFO, MUID::ZERO);
		assert(NULL!=cmd);
		cmd->AddParameter(new MCommandParameterSingleBlob(info));
		cmd->AddParameter(new MCommandParameterBlob(inventory));
		cmd->AddParameter(new MCommandParameterBlob(equipments));
		cmd->AddParameter(new MCommandParameterBlob(reservedDummySlotInfos));
		cmd->AddParameter(new MCommandParameterBlob(talents));
		cmd->AddParameter(new MCommandParameterBlob(quests));
		cmd->AddParameter(new MCommandParameterBlob(palette));
		cmd->AddParameter(new MCommandParameterBlob(factions));
		cmd->AddParameter(new MCommandParameterBlob(recipes));

		fillInventory->OnCommand(&agent, cmd);

		XBirdClient* client = agent.GetClient();
		client->Run();	//todo Ŭ�󸮾�Ʈ �Ǵ� ������Ʈ�� ������ ���� Ŀ�ǵ尡 �� ���Դ��� Ȯ��

		delete cmd;
		MGetCommandDescMap()->Clear();
		delete fillInventory;
	}
}