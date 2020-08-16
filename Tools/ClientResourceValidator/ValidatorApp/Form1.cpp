#include "stdafx.h"
#include "Form1.h"
#include <fstream>
#include "MLocale.h"

void ValidatorApp::Form1::SetValidatorReport( VReportMgr * pReport )
{
	bool bError = false;

	ofstream f(GetOutputFileName());
	MMaietXmlTestReporter maietReport(f);

	ReportLogFirstInfo(pReport, maietReport);

	// �ִϸ��̼� ����Ʈ
	if(pReport->m_pMeshDataReport->GetAnimationListErrorMsg().IsError())
	{
		maietReport.AddMessage("");
		maietReport.AddMessage(MLocale::ConvAnsiToUTF8("[-- �ִϸ��̼�(Animation.xml) --------------------------------------------------------]", LANG_KOREAN));
	
		AddTabPage("�ִϸ��̼�(Animation.xml)", AnimationResult_tabPage, AnimationResult_listBox, TP_ANIMATION);
		SetData(AnimationResult_listBox, pReport->m_pMeshDataReport->GetAnimationListErrorMsg(), maietReport);
	
		bError = true;
	}

	// �ִϸ��̼� �̺�Ʈ
	if(pReport->m_pMeshDataReport->GetAnimationEventErrorMsg().IsError())
	{
		maietReport.AddMessage("");
		maietReport.AddMessage(MLocale::ConvAnsiToUTF8("[-- �ִϸ��̼� �̺�Ʈ(AnimationEvent.xml) --------------------------------------------------------]", LANG_KOREAN));
		
		AddTabPage("�ִϸ��̼� �̺�Ʈ(AnimationEvent.xml)", AnimationEventResult_tabPage, AnimationEventResult_listBox, TP_ANIMATION_EVENT);
		SetData(AnimationEventResult_listBox, pReport->m_pMeshDataReport->GetAnimationEventErrorMsg(), maietReport);
	
		bError = true;
	}

	// �ִϸ��̼� ����
	if(pReport->m_pMeshDataReport->GetAnimationInfoErrorMsg().IsError())
	{
		maietReport.AddMessage("");
		maietReport.AddMessage(MLocale::ConvAnsiToUTF8("[-- �ִϸ��̼� ����(AnimationInfo.xml) --------------------------------------------------------]", LANG_KOREAN));
	
		AddTabPage("�ִϸ��̼� ����(AnimationInfo.xml)", AnimationInfoResult_tabPage, AnimationInfoResult_listBox, TP_ANIMATION_INFO);
		SetData(AnimationInfoResult_listBox, pReport->m_pMeshDataReport->GetAnimationInfoErrorMsg(), maietReport);
	
		bError = true;
	}

	// �ŷ�Ʈ �̺�Ʈ
	if(pReport->m_pTalentDataReport->GetTalentEventErrorMsg().IsError())
	{
		maietReport.AddMessage("");
		maietReport.AddMessage(MLocale::ConvAnsiToUTF8("[-- �ŷ�Ʈ �̺�Ʈ(talent_ext.xml) --------------------------------------------------------]", LANG_KOREAN));
	
		AddTabPage("�ŷ�Ʈ �̺�Ʈ(talent_ext.xml)", TalentEventResult_tabPage, TalentEventResult_listBox, TP_TALENT_EVENT);
		SetData(TalentEventResult_listBox, pReport->m_pTalentDataReport->GetTalentEventErrorMsg(), maietReport);
	
		bError = true;
	}

	// ���� 

	// ����Ʈ ����
	if(pReport->m_pEffectDataReport->GetEffectInfoErrorMsg().IsError())
	{
		maietReport.AddMessage("");
		maietReport.AddMessage(MLocale::ConvAnsiToUTF8("[-- ����Ʈ(effect.xml) --------------------------------------------------------]", LANG_KOREAN));
	
		AddTabPage("����Ʈ(effect.xml)", EffectResult_tabPage, EffectResult_listBox, TP_EFFECT);
		SetData(EffectResult_listBox, pReport->m_pEffectDataReport->GetEffectInfoErrorMsg(), maietReport);
		
		bError = true;
	}

	// �ŷ�Ʈ����Ʈ ����
	if(pReport->m_pEffectDataReport->GetTalentEffectInfoErrorMsg().IsError())
	{
		maietReport.AddMessage("");
		maietReport.AddMessage(MLocale::ConvAnsiToUTF8("[-- �ŷ�Ʈ ����Ʈ(talent_Effect_info.xml) --------------------------------------------------------]", LANG_KOREAN));
	
		AddTabPage("�ŷ�Ʈ ����Ʈ(talent_Effect_info.xml)", TalentEffectResult_tabPage, TalentEffectResult_listBox, TP_TALENT_EFFECT);
		SetData(TalentEffectResult_listBox, pReport->m_pEffectDataReport->GetTalentEffectInfoErrorMsg(), maietReport);
		
		bError = true;
	}

	// ���� ����Ʈ ����
	if(pReport->m_pEffectDataReport->GetBuffEffectInfoErrorMsg().IsError())
	{
		maietReport.AddMessage("");
		maietReport.AddMessage(MLocale::ConvAnsiToUTF8("[-- ���� ����Ʈ(buff_Effect_info.xml) --------------------------------------------------------]", LANG_KOREAN));
		
		AddTabPage("���� ����Ʈ(buff_Effect_info.xml)", BuffEffectResult_tabPage, BuffEffectResult_listBox, TP_BUFF_EFFECT);
		SetData(BuffEffectResult_listBox, pReport->m_pEffectDataReport->GetBuffEffectInfoErrorMsg(), maietReport);
	
		bError = true;
	}

	// Npc ����
	if(pReport->m_pNpcDataReport->GetNpcInfoErrorMsg().IsError())
	{
		maietReport.AddMessage("");
		maietReport.AddMessage(MLocale::ConvAnsiToUTF8("[-- Npc ����(npc.xml) --------------------------------------------------------]", LANG_KOREAN));
		
		AddTabPage("Npc ����(npc.xml)", NpcInfoResult_tabPage, NpcInfoResult_listBox, TP_NPC_INFO);
		SetData(NpcInfoResult_listBox, pReport->m_pNpcDataReport->GetNpcInfoErrorMsg(), maietReport);
		
		bError = true;
	}

	// ������ ����
	if(pReport->m_pItemDataReport->GetItemInfoErrorMsg().IsError())
	{
		maietReport.AddMessage("");
		maietReport.AddMessage(MLocale::ConvAnsiToUTF8("[-- ������ ����(item.xml) --------------------------------------------------------]", LANG_KOREAN));

		AddTabPage("������ ����(item.xml)", ItemInfoResult_tabPage, ItemInfoResult_listBox, TP_ITEM_INFO);
		SetData(ItemInfoResult_listBox, pReport->m_pItemDataReport->GetItemInfoErrorMsg(), maietReport);

		bError = true;
	}

	// ���� ����
	if(pReport->m_pSoundDataReport->GetSoundInfoErrorMsg().IsError())
	{
		maietReport.AddMessage("");
		maietReport.AddMessage(MLocale::ConvAnsiToUTF8("[-- ���� ����(sound.xml) --------------------------------------------------------]", LANG_KOREAN));

		AddTabPage("���� ����(sound.xml)", SoundInfoResult_tabPage, SoundInfoResult_listBox, TP_SOUND_INFO);
		SetData(SoundInfoResult_listBox, pReport->m_pSoundDataReport->GetSoundInfoErrorMsg(), maietReport);

		bError = true;
	}


	// ���
	if(bError)
		GlobalObjects::g_ValidatorResult = CCNET_FAILED;

	maietReport.ReportSummary(0, 0, 0, 0, MMaietTestResultList());
}

System::Void ValidatorApp::Form1::AddTabPage( string strTabPageName, [Runtime::InteropServices::Out] System::Windows::Forms::TabPage^ %tabPage, [Runtime::InteropServices::Out] System::Windows::Forms::ListBox^ %listBox, TAB_PAGE eType )
{
	static bool bTabPageAdd[TP_MAX] = {0,};
	if(bTabPageAdd[eType])
		return;

	tabPage = gcnew TabPage(gcnew String(strTabPageName.c_str()));
	listBox = gcnew ListBox();

	ResourceValidator_tabControl->Controls->Add(tabPage);

	listBox->Dock = System::Windows::Forms::DockStyle::Fill;
	listBox->HorizontalScrollbar = true;
	tabPage->Controls->Add(listBox);

	bTabPageAdd[eType] = true;
}

System::Void ValidatorApp::Form1::SetData( [Runtime::InteropServices::Out] System::Windows::Forms::ListBox^ %listBox, VErrorMsg& errorMsg, MMaietXmlTestReporter& maietReporter )
{
	listBox->Items->Clear();

	// ����Ʈ �ڽ��� ���� �Է�
	for(vector<stERRORMSG>::iterator itResult = errorMsg.m_strErrorMsg.begin(); itResult != errorMsg.m_strErrorMsg.end(); ++itResult)
	{
		listBox->Items->Add(gcnew String(itResult->strErrorMsg.c_str()));

		string stringLog = itResult->strErrorMsg;
		stringLog = MLocale::ConvAnsiToUTF8(stringLog.c_str(), LANG_KOREAN);
		maietReporter.AddMessage(stringLog);
	}
}

System::Void ValidatorApp::Form1::ReportLogFirstInfo( VReportMgr * pReport, MMaietXmlTestReporter& maietReporter )
{
	int nErrorCount = 0;
	nErrorCount += pReport->m_pMeshDataReport->GetAnimationListErrorMsg().m_strErrorMsg.size();
	nErrorCount += pReport->m_pMeshDataReport->GetAnimationEventErrorMsg().m_strErrorMsg.size();
	nErrorCount += pReport->m_pMeshDataReport->GetAnimationInfoErrorMsg().m_strErrorMsg.size();
	nErrorCount += pReport->m_pTalentDataReport->GetTalentEventErrorMsg().m_strErrorMsg.size();
	nErrorCount += pReport->m_pEffectDataReport->GetEffectInfoErrorMsg().m_strErrorMsg.size();
	nErrorCount += pReport->m_pEffectDataReport->GetTalentEffectInfoErrorMsg().m_strErrorMsg.size();
	nErrorCount += pReport->m_pEffectDataReport->GetBuffEffectInfoErrorMsg().m_strErrorMsg.size();
	nErrorCount += pReport->m_pNpcDataReport->GetNpcInfoErrorMsg().m_strErrorMsg.size();
	nErrorCount += pReport->m_pItemDataReport->GetItemInfoErrorMsg().m_strErrorMsg.size();
	nErrorCount += pReport->m_pSoundDataReport->GetSoundInfoErrorMsg().m_strErrorMsg.size();

	maietReporter.AddMessage("[-- Validate Log Info --------------------------------------------------------]");
	char chError[256] = {0,};
	sprintf(chError, "1. Full Error Count : %d", nErrorCount);
	maietReporter.AddMessage(chError);
	maietReporter.AddMessage("[-----------------------------------------------------------------------------]");
}