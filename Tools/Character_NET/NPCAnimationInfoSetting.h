#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "ProgressBarForm.h"

namespace Character_NET {

	/// <summary>
	/// NPCAnimationInfoSetting�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class NPCAnimationInfoSetting : public System::Windows::Forms::Form
	{
	private:
		String^	m_strSelectNPCName;

		ProgressBarForm	m_copyProgressBar;

		TCharacter*		m_phfPlayerModel;
		TCharacter*		m_phmPlayerModel;

		float	m_fCurrentProgressBarState;
		int		m_nCopyFullAniCount;

	private:
		void StartCopyAnimationInfo(string strNPCModelName, vector<SIMPLEANIMATION_INFO>& vecCopyAnimationList);
		int CollectionCopyNpcAnimationList(string strNpcName, vector<SIMPLEANIMATION_INFO>& vecCopyAniList, vector<string>& vecOutResult);
		void CalProgressBar();

		bool LoadNpcAnimationInfo(RMeshAnimationMgr& npcAnimationEventInfo, TAnimationInfoMgr& npcAnimationInfoMgr, string& strNPCAnimationFilePath, string& strNPCAnimationEventFileName, vector<SIMPLEANIMATION_INFO>& vecCopyAnimationList);
		void SaveNpcAnimationInfo(RMeshAnimationMgr& npcAnimationEventInfo, TAnimationInfoMgr& npcAnimationInfoMgr, string& strNPCAnimationEventFileName, vector<SIMPLEANIMATION_INFO>& vecCopyAnimationList);

		void CopyAnimationInfo(TCharacter* pCopyModel, vector<SIMPLEANIMATION_INFO>& vecCopySource, vector<SIMPLEANIMATION_INFO>& vecTargetSource, RMeshAnimationMgr& npcAnimationEventInfo, TAnimationInfoMgr& npcAnimationInfoMgr);
		void CopyAnimationEvent( TCharacter* pCopyModel, RMeshAnimationMgr &npcAnimationEventInfo, string& strSourceAnimationName, string& strTargetAnimationName );
		void CopyAnimationBlendInfo( TCharacter* pCopyModel, TAnimationInfoMgr &npcAnimationInfoMgr, string& strTargetAnimationName, vector<SIMPLEANIMATION_INFO>& vecTargetSource );
		void CopyAnimationOverlayInfo( TCharacter* pCopyModel, TAnimationInfoMgr &npcAnimationInfoMgr, string& strSourceAnimationName, string& strTargetAnimationName );
		void CopyAnimationReActionInfo( TCharacter* pCopyModel, TAnimationInfoMgr &npcAnimationInfoMgr, string& strSourceAnimationName, string& strTargetAnimationName );
		void CopyAnimationHitSound( TCharacter* pCopyModel, TAnimationInfoMgr &npcAnimationInfoMgr, string& strSourceAnimationName, string& strTargetAnimationName );
		
		void CopyAnimationSwordTrailEvent(RAnimationEvent& AniEvent, string& strTargetAnimationName);
		bool DoNotCopyAnimationEvent(RAnimationEvent* pAniEvent);


		RAnimation* GetNpcAnimation(RAnimationHashList& npcAnimationList, string strAniName);

	public:
		NPCAnimationInfoSetting(void)
		{
			InitializeComponent();
			//
			//TODO: ������ �ڵ带 ���⿡ �߰��մϴ�.
			//

			m_phfPlayerModel = NULL;
			m_phmPlayerModel = NULL;

			m_fCurrentProgressBarState = 0.0f;
			m_nCopyFullAniCount = 0;
		}

		void SetSelectNpcName(String^ strName);
		void Init(TCharacter* phf, TCharacter* phm);

	protected:
		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		~NPCAnimationInfoSetting()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 


	private: System::Windows::Forms::Button^  cancle_button;
	private: System::Windows::Forms::ListBox^  npc_listBox;



	private: System::Windows::Forms::Button^  copy_button;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ListBox^  logOut_listBox;




	private: System::Windows::Forms::Label^  label3;

	private: System::Windows::Forms::GroupBox^  groupBox2;

	private:
		/// <summary>
		/// �ʼ� �����̳� �����Դϴ�.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����̳� ������ �ʿ��� �޼����Դϴ�.
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
		/// </summary>
		void InitializeComponent(void)
		{
			this->cancle_button = (gcnew System::Windows::Forms::Button());
			this->npc_listBox = (gcnew System::Windows::Forms::ListBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->copy_button = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->logOut_listBox = (gcnew System::Windows::Forms::ListBox());
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// cancle_button
			// 
			this->cancle_button->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->cancle_button->Location = System::Drawing::Point(260, 423);
			this->cancle_button->Name = L"cancle_button";
			this->cancle_button->Size = System::Drawing::Size(75, 23);
			this->cancle_button->TabIndex = 3;
			this->cancle_button->Text = L"���";
			this->cancle_button->UseVisualStyleBackColor = true;
			this->cancle_button->Click += gcnew System::EventHandler(this, &NPCAnimationInfoSetting::cancle_button_Click);
			// 
			// npc_listBox
			// 
			this->npc_listBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->npc_listBox->FormattingEnabled = true;
			this->npc_listBox->HorizontalScrollbar = true;
			this->npc_listBox->ItemHeight = 12;
			this->npc_listBox->Location = System::Drawing::Point(6, 21);
			this->npc_listBox->Name = L"npc_listBox";
			this->npc_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->npc_listBox->Size = System::Drawing::Size(452, 220);
			this->npc_listBox->Sorted = true;
			this->npc_listBox->TabIndex = 4;
			this->npc_listBox->DoubleClick += gcnew System::EventHandler(this, &NPCAnimationInfoSetting::npc_listBox_DoubleClick);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->ForeColor = System::Drawing::Color::Red;
			this->label3->Location = System::Drawing::Point(12, 282);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(295, 12);
			this->label3->TabIndex = 9;
			this->label3->Text = L"(����) Player �� �ִϸ��̼� ������ ���縦 �մϴ�.";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->npc_listBox);
			this->groupBox2->Location = System::Drawing::Point(6, 12);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(464, 260);
			this->groupBox2->TabIndex = 11;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"���� �� NPC ����Ʈ";
			// 
			// copy_button
			// 
			this->copy_button->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->copy_button->Location = System::Drawing::Point(147, 423);
			this->copy_button->Name = L"copy_button";
			this->copy_button->Size = System::Drawing::Size(75, 23);
			this->copy_button->TabIndex = 12;
			this->copy_button->Text = L"����";
			this->copy_button->UseVisualStyleBackColor = true;
			this->copy_button->Click += gcnew System::EventHandler(this, &NPCAnimationInfoSetting::copy_button_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::Red;
			this->label1->Location = System::Drawing::Point(12, 303);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(317, 12);
			this->label1->TabIndex = 13;
			this->label1->Text = L"���� ����� ���� �ִϸ��̼� ������ ��� ������ �˴ϴ�. ";
			// 
			// logOut_listBox
			// 
			this->logOut_listBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->logOut_listBox->BackColor = System::Drawing::Color::Gainsboro;
			this->logOut_listBox->FormattingEnabled = true;
			this->logOut_listBox->ItemHeight = 12;
			this->logOut_listBox->Location = System::Drawing::Point(6, 324);
			this->logOut_listBox->Name = L"logOut_listBox";
			this->logOut_listBox->Size = System::Drawing::Size(464, 88);
			this->logOut_listBox->TabIndex = 14;
			// 
			// NPCAnimationInfoSetting
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(482, 458);
			this->Controls->Add(this->logOut_listBox);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->copy_button);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->cancle_button);
			this->Name = L"NPCAnimationInfoSetting";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"NPCAnimationInfoSetting";
			this->Load += gcnew System::EventHandler(this, &NPCAnimationInfoSetting::NPCAnimationInfoSetting_Load);
			this->groupBox2->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void NPCAnimationInfoSetting_Load(System::Object^  sender, System::EventArgs^  e);
private: System::Void copy_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void cancle_button_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }
private: System::Void npc_listBox_DoubleClick(System::Object^  sender, System::EventArgs^  e) {
			 copy_button_Click(sender, e);
		 }
};
}
