#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Character_NET {

	/// <summary>
	/// NPCListTabPage�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class NPCListTabPage : public System::Windows::Forms::UserControl
	{
	public:
		NPCListTabPage(void)
		{
			InitializeComponent();
			//
			//TODO: ������ �ڵ带 ���⿡ �߰��մϴ�.
			//
		}
		void ReLoadNPCList();
		void FindNPCGroup(System::String^ NPCID);

		void SetNPCListTreeView( TNPCInfo* pNPCInfo );
		bool FindNPCList(vector<string>& vecNPCList, TNPCInfo* pInfo);
		bool FindTALENTList(vector<string>& vecTalentList, TTalentInfo* pInfo);

	private:
		void InitNPCList();

	protected:
		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		~NPCListTabPage()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label16;
	protected: 
	private: System::Windows::Forms::TextBox^  NPCGroupIDtextBox;
	public: System::Windows::Forms::TreeView^  NPCListtreeView;
	private: System::Windows::Forms::Button^  Find_Button;
	public: 

	public: 
	private: 
		System::String^			m_CurrentFindNPC;

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
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->NPCGroupIDtextBox = (gcnew System::Windows::Forms::TextBox());
			this->NPCListtreeView = (gcnew System::Windows::Forms::TreeView());
			this->Find_Button = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(3, 7);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(59, 12);
			this->label16->TabIndex = 5;
			this->label16->Text = L"NPC Find";
			// 
			// NPCGroupIDtextBox
			// 
			this->NPCGroupIDtextBox->Location = System::Drawing::Point(62, 3);
			this->NPCGroupIDtextBox->Name = L"NPCGroupIDtextBox";
			this->NPCGroupIDtextBox->Size = System::Drawing::Size(90, 21);
			this->NPCGroupIDtextBox->TabIndex = 4;
			this->NPCGroupIDtextBox->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &NPCListTabPage::NPCGroupIDtextBox_KeyDown);
			// 
			// NPCListtreeView
			// 
			this->NPCListtreeView->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->NPCListtreeView->HideSelection = false;
			this->NPCListtreeView->Location = System::Drawing::Point(3, 31);
			this->NPCListtreeView->Name = L"NPCListtreeView";
			this->NPCListtreeView->Size = System::Drawing::Size(192, 537);
			this->NPCListtreeView->TabIndex = 3;
			this->NPCListtreeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &NPCListTabPage::NPCListtreeView_AfterSelect);
			this->NPCListtreeView->NodeMouseClick += gcnew System::Windows::Forms::TreeNodeMouseClickEventHandler(this, &NPCListTabPage::NPCListtreeView_NodeMouseClick);
			// 
			// Find_Button
			// 
			this->Find_Button->Font = (gcnew System::Drawing::Font(L"����", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(129)));
			this->Find_Button->Location = System::Drawing::Point(154, 3);
			this->Find_Button->Name = L"Find_Button";
			this->Find_Button->Size = System::Drawing::Size(40, 20);
			this->Find_Button->TabIndex = 6;
			this->Find_Button->Text = L"Find";
			this->Find_Button->UseVisualStyleBackColor = true;
			this->Find_Button->Click += gcnew System::EventHandler(this, &NPCListTabPage::Find_Button_Click);
			// 
			// NPCListTabPage
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->Find_Button);
			this->Controls->Add(this->label16);
			this->Controls->Add(this->NPCGroupIDtextBox);
			this->Controls->Add(this->NPCListtreeView);
			this->Name = L"NPCListTabPage";
			this->Size = System::Drawing::Size(198, 573);
			this->Load += gcnew System::EventHandler(this, &NPCListTabPage::NPCListTabPage_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		private: void FindNPC()
				 {
					 if(m_CurrentFindNPC != NPCGroupIDtextBox->Text)
					 {
						 FindNPCGroup(NPCGroupIDtextBox->Text);
					 }

					 m_CurrentFindNPC = NPCGroupIDtextBox->Text;
				 }
	private: System::Void NPCListtreeView_NodeMouseClick(System::Object^  sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^  e) {
				 g_pMainApp->NPCListtreeView_NodeMouseClick(sender, e);
			 }
	private: System::Void NPCListtreeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
				 g_pMainApp->NPCListtreeView_AfterSelect(sender, e);
			 }
	private: System::Void NPCGroupIDtextBox_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
				 if(e->KeyCode == Keys::Enter)
				 {
					 FindNPC();
				 }
			 }
private: System::Void Find_Button_Click(System::Object^  sender, System::EventArgs^  e) {
			 FindNPC();
		 }
private: System::Void NPCListTabPage_Load(System::Object^  sender, System::EventArgs^  e) {
			InitNPCList();
		 }
};
}
