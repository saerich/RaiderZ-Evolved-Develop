#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Character_NET {

	/// <summary>
	/// ModelTabPage�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class ModelTabPage : public System::Windows::Forms::Form
	{
	public:
		ModelTabPage(void)
		{
			InitializeComponent();
			//
			//TODO: ������ �ڵ带 ���⿡ �߰��մϴ�.
			//
		}

		void SetModelTreeView();

		void SetMapObjTreeView( tstring &subDir, TreeNode^ temp, TreeNode^ node, tstring &Path);
		TreeNodeCollection^ GetNodes()	{ return ModeltreeView->Nodes;}

	protected:
		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		~ModelTabPage()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::TreeView^  ModeltreeView;
	protected: 

	protected: 

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
			System::Windows::Forms::TreeNode^  treeNode1 = (gcnew System::Windows::Forms::TreeNode(L"male"));
			System::Windows::Forms::TreeNode^  treeNode2 = (gcnew System::Windows::Forms::TreeNode(L"female"));
			System::Windows::Forms::TreeNode^  treeNode3 = (gcnew System::Windows::Forms::TreeNode(L"human", gcnew cli::array< System::Windows::Forms::TreeNode^  >(2) {treeNode1, 
				treeNode2}));
			System::Windows::Forms::TreeNode^  treeNode4 = (gcnew System::Windows::Forms::TreeNode(L"male"));
			System::Windows::Forms::TreeNode^  treeNode5 = (gcnew System::Windows::Forms::TreeNode(L"female"));
			System::Windows::Forms::TreeNode^  treeNode6 = (gcnew System::Windows::Forms::TreeNode(L"nyahbit", gcnew cli::array< System::Windows::Forms::TreeNode^  >(2) {treeNode4, 
				treeNode5}));
			System::Windows::Forms::TreeNode^  treeNode7 = (gcnew System::Windows::Forms::TreeNode(L"male"));
			System::Windows::Forms::TreeNode^  treeNode8 = (gcnew System::Windows::Forms::TreeNode(L"female"));
			System::Windows::Forms::TreeNode^  treeNode9 = (gcnew System::Windows::Forms::TreeNode(L"���", gcnew cli::array< System::Windows::Forms::TreeNode^  >(2) {treeNode7, 
				treeNode8}));
			System::Windows::Forms::TreeNode^  treeNode10 = (gcnew System::Windows::Forms::TreeNode(L"Player", gcnew cli::array< System::Windows::Forms::TreeNode^  >(3) {treeNode3, 
				treeNode6, treeNode9}));
			System::Windows::Forms::TreeNode^  treeNode11 = (gcnew System::Windows::Forms::TreeNode(L"����"));
			System::Windows::Forms::TreeNode^  treeNode12 = (gcnew System::Windows::Forms::TreeNode(L"NPC"));
			this->ModeltreeView = (gcnew System::Windows::Forms::TreeView());
			this->SuspendLayout();
			// 
			// ModeltreeView
			// 
			this->ModeltreeView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ModeltreeView->HideSelection = false;
			this->ModeltreeView->Location = System::Drawing::Point(0, 0);
			this->ModeltreeView->Name = L"ModeltreeView";
			treeNode1->Name = L"���4";
			treeNode1->Text = L"male";
			treeNode2->Name = L"���5";
			treeNode2->Text = L"female";
			treeNode3->Name = L"���0";
			treeNode3->Text = L"human";
			treeNode4->Name = L"���6";
			treeNode4->Text = L"male";
			treeNode5->Name = L"���7";
			treeNode5->Text = L"female";
			treeNode6->Name = L"���1";
			treeNode6->Text = L"nyahbit";
			treeNode7->Name = L"���8";
			treeNode7->Text = L"male";
			treeNode8->Name = L"���9";
			treeNode8->Text = L"female";
			treeNode9->Name = L"���2";
			treeNode9->Text = L"���";
			treeNode10->Name = L"���1";
			treeNode10->Text = L"Player";
			treeNode11->Name = L"���0";
			treeNode11->Text = L"����";
			treeNode12->Name = L"���3";
			treeNode12->Text = L"NPC";
			this->ModeltreeView->Nodes->AddRange(gcnew cli::array< System::Windows::Forms::TreeNode^  >(3) {treeNode10, treeNode11, treeNode12});
			this->ModeltreeView->Size = System::Drawing::Size(197, 498);
			this->ModeltreeView->TabIndex = 1;
			this->ModeltreeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &ModelTabPage::ModeltreeView_AfterSelect);
			// 
			// ModelTabPage
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(197, 498);
			this->Controls->Add(this->ModeltreeView);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Name = L"ModelTabPage";
			this->Text = L"ModelTabPage";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void ModeltreeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
				 if( e->Node->Parent == nullptr ) return;

				 if (g_pMainApp->GetActor())
				 {
					 const char* nodename = MStringToCharPointer(e->Node->Text);
					 int nSexType = TStrings::StringToSexType(nodename);
					 MFreeCharPointer(nodename);

					 if (nSexType != -1)
					 {
						 int nSex = TStrings::StringToSexType(g_pMainApp->GetActor()->CurrentSexType().c_str());
						 if (nSex == nSexType) return;
					 }
				 }

				 g_pMainApp->ModeltreeView_AfterSelect(e);
			 }
};
}
