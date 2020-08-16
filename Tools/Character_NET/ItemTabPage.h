#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Character_NET {

	/// <summary>
	/// ItemTabPage�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class ItemTabPage : public System::Windows::Forms::Form
	{
	public:
		ItemTabPage(void)
		{
			InitializeComponent();
			//
			//TODO: ������ �ڵ带 ���⿡ �߰��մϴ�.
			//
		}

		void InitItemList();

	protected:
		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		~ItemTabPage()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListBox^  ItemInfolistBox;
	protected: 
	public: System::Windows::Forms::TreeView^  ItemtreeView;
	private: 

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
			this->ItemInfolistBox = (gcnew System::Windows::Forms::ListBox());
			this->ItemtreeView = (gcnew System::Windows::Forms::TreeView());
			this->SuspendLayout();
			// 
			// ItemInfolistBox
			// 
			this->ItemInfolistBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->ItemInfolistBox->FormattingEnabled = true;
			this->ItemInfolistBox->ItemHeight = 12;
			this->ItemInfolistBox->Location = System::Drawing::Point(6, 454);
			this->ItemInfolistBox->Name = L"ItemInfolistBox";
			this->ItemInfolistBox->Size = System::Drawing::Size(251, 52);
			this->ItemInfolistBox->TabIndex = 5;
			// 
			// ItemtreeView
			// 
			this->ItemtreeView->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->ItemtreeView->HideSelection = false;
			this->ItemtreeView->Location = System::Drawing::Point(4, 18);
			this->ItemtreeView->Name = L"ItemtreeView";
			this->ItemtreeView->Size = System::Drawing::Size(254, 425);
			this->ItemtreeView->TabIndex = 4;
			this->ItemtreeView->NodeMouseClick += gcnew System::Windows::Forms::TreeNodeMouseClickEventHandler(this, &ItemTabPage::ItemtreeView_NodeMouseClick);
			// 
			// ItemTabPage
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(262, 524);
			this->Controls->Add(this->ItemInfolistBox);
			this->Controls->Add(this->ItemtreeView);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Name = L"ItemTabPage";
			this->Text = L"ItemTabPage";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void ItemtreeView_NodeMouseClick(System::Object^  sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^  e) {
				 if( e->Node->Parent == nullptr ) return;

				 Object^ ptag = e->Node->Tag;
				 int itemid;
				 if( ptag!= nullptr)
					 itemid = (int)ptag;

				 g_pMainApp->SelectItemInItemList(e->Node->Parent->Index);
				 bool bEquipItem = g_pMainApp->m_EquipItems.EquipItemID(itemid, e->Node->Parent->Index);

				 if( bEquipItem)
					 ItemInfolistBox->Items->Add(e->Node->Text);
				 else
					 ItemInfolistBox->Items->Add("��������");

				 if( ptag!= nullptr)
				 {
					 if(g_pMainApp->GetActor())
					 {
						 if( bEquipItem )	// �������� �϶��� ���´�
						 {
							 // ���� ������ BaseMesh�� ���� ����Ʈ�� �о� ���δ�.
							 g_pMainApp->m_Mtrl.SetCurrentMaterialList(itemid);
							 g_pMainApp->m_Mtrl.SetItemDataSubMtrl(itemid);
							 g_pMainApp->m_Mtrl.SetMeshTreeView();
							 g_pMainApp->m_Mtrl.SetMtrlList();
						 }
					 }
				 }
			 }
	};
}
