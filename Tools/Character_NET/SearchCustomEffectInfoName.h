#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Character_NET {

	/// <summary>
	/// SearchCustomEffectInfoName�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class SearchCustomEffectInfoName : public System::Windows::Forms::Form
	{
	public:
		String^			m_strSelectCustoEffectInfoName;

	private:
		void SearchEffectList();
		bool CompareEffectName(string& strSource, string& strTarget);

	public:
		SearchCustomEffectInfoName(void)
		{
			InitializeComponent();
			//
			//TODO: ������ �ڵ带 ���⿡ �߰��մϴ�.
			//
		}

		void Init();
		void SearchEffectList(string& strSearchText, vector<string>& vecResult);

	protected:
		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		~SearchCustomEffectInfoName()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListBox^  search_listBox;
	private: System::Windows::Forms::Button^  search_button;
	protected: 

	private: System::Windows::Forms::TextBox^  search_textBox;


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
			this->search_listBox = (gcnew System::Windows::Forms::ListBox());
			this->search_button = (gcnew System::Windows::Forms::Button());
			this->search_textBox = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// search_listBox
			// 
			this->search_listBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->search_listBox->FormattingEnabled = true;
			this->search_listBox->ItemHeight = 12;
			this->search_listBox->Location = System::Drawing::Point(13, 41);
			this->search_listBox->Name = L"search_listBox";
			this->search_listBox->Size = System::Drawing::Size(245, 244);
			this->search_listBox->TabIndex = 0;
			this->search_listBox->DoubleClick += gcnew System::EventHandler(this, &SearchCustomEffectInfoName::search_listBox_DoubleClick);
			// 
			// search_button
			// 
			this->search_button->Location = System::Drawing::Point(206, 8);
			this->search_button->Name = L"search_button";
			this->search_button->Size = System::Drawing::Size(52, 23);
			this->search_button->TabIndex = 1;
			this->search_button->Text = L"�˻�";
			this->search_button->UseVisualStyleBackColor = true;
			this->search_button->Click += gcnew System::EventHandler(this, &SearchCustomEffectInfoName::search_button_Click);
			// 
			// search_textBox
			// 
			this->search_textBox->Location = System::Drawing::Point(12, 10);
			this->search_textBox->Name = L"search_textBox";
			this->search_textBox->Size = System::Drawing::Size(188, 21);
			this->search_textBox->TabIndex = 2;
			this->search_textBox->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &SearchCustomEffectInfoName::search_textBox_KeyDown);
			// 
			// SearchCustomEffectInfoName
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(267, 306);
			this->Controls->Add(this->search_textBox);
			this->Controls->Add(this->search_button);
			this->Controls->Add(this->search_listBox);
			this->Name = L"SearchCustomEffectInfoName";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"����Ʈ(effect.xml) �˻�â";
			this->Load += gcnew System::EventHandler(this, &SearchCustomEffectInfoName::SearchCustomEffectInfoName_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void search_textBox_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
				 if(e->KeyCode == Keys::Enter)
				 {
					 SearchEffectList();
				 }
			 }
private: System::Void search_button_Click(System::Object^  sender, System::EventArgs^  e) {
			 SearchEffectList();
		 }
private: System::Void SearchCustomEffectInfoName_Load(System::Object^  sender, System::EventArgs^  e);
private: System::Void search_listBox_DoubleClick(System::Object^  sender, System::EventArgs^  e);
};
}
