// QuestAndDialog_NET.cpp : �⺻ ������Ʈ �����Դϴ�.

#include "stdafx.h"
#include "TQuestConfig.h"
#include "TDataBase.h"
#include "TDBManager.h"
#include "TStringResMgr.h"
#include "TDialogController.h"
#include "TQuestController.h"

[STAThread]
int main(array<System::String ^> ^args)
{
	// ��Ʈ���� ��������� ���� Windows XP �ð� ȿ���� Ȱ��ȭ�մϴ�.
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	TQuestConfig::Init();

	TNPCInfoMgr* pNim = new TNPCInfoMgr();
	TDialogController* pDc = new TDialogController(pNim, TStringResMgr::GetInstance());
	int nNPCID = -1;
	if (1 == args->Length)
	{
		nNPCID = MStringToInt(args[0]);
	}
	
	TDatabase* pDB = new TDatabase();
	TDBManager* pDBm = new TDBManager(pDB);
	TQuestController* pQc = new TQuestController(pDBm, TStringResMgr::GetInstance());
	
	if (false == pDc->Begin(pQc, nNPCID)) _ASSERT(0);
	if (false == pQc->Begin(pDc)) _ASSERT(0);
	
	QuestForm^ qf = safe_cast<QuestForm^>(pQc->GetQuestForm());
	if (nullptr == qf) _ASSERT(0);
	Application::Run(qf);
	pQc->RunDialogTool();

	pQc->End();
	pDc->End();
	
	delete pQc;
	delete pDc;

	return 0;
}