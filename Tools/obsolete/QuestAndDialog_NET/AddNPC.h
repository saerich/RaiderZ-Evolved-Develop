#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace QuestAndDialog_NET {

	/// <summary>
	/// AddNPC�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class AddNPC : public System::Windows::Forms::Form
	{
	public:
		AddNPC(void)
		{
			InitializeComponent();
			//
			//TODO: ������ �ڵ带 ���⿡ �߰��մϴ�.
			//
		}

	protected:
		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		~AddNPC()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::TextBox^  textBox_NPCID;
	protected: 

	private: System::Windows::Forms::Label^  Label_NPCID;
	private: System::Windows::Forms::Button^  button_OK;
	private: System::Windows::Forms::Button^  button_CANCEL;

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
			this->textBox_NPCID = (gcnew System::Windows::Forms::TextBox());
			this->Label_NPCID = (gcnew System::Windows::Forms::Label());
			this->button_OK = (gcnew System::Windows::Forms::Button());
			this->button_CANCEL = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// textBox_NPCID
			// 
			this->textBox_NPCID->Location = System::Drawing::Point(144, 21);
			this->textBox_NPCID->Name = L"textBox_NPCID";
			this->textBox_NPCID->Size = System::Drawing::Size(100, 21);
			this->textBox_NPCID->TabIndex = 0;
			// 
			// Label_NPCID
			// 
			this->Label_NPCID->AutoSize = true;
			this->Label_NPCID->Location = System::Drawing::Point(34, 21);
			this->Label_NPCID->Name = L"Label_NPCID";
			this->Label_NPCID->Size = System::Drawing::Size(42, 12);
			this->Label_NPCID->TabIndex = 1;
			this->Label_NPCID->Text = L"NPCID";
			// 
			// button_OK
			// 
			this->button_OK->Location = System::Drawing::Point(36, 59);
			this->button_OK->Name = L"button_OK";
			this->button_OK->Size = System::Drawing::Size(75, 23);
			this->button_OK->TabIndex = 2;
			this->button_OK->Text = L"Ȯ��";
			this->button_OK->UseVisualStyleBackColor = true;
			this->button_OK->Click += gcnew System::EventHandler(this, &AddNPC::button_OK_Click);
			// 
			// button_CANCEL
			// 
			this->button_CANCEL->Location = System::Drawing::Point(169, 59);
			this->button_CANCEL->Name = L"button_CANCEL";
			this->button_CANCEL->Size = System::Drawing::Size(75, 23);
			this->button_CANCEL->TabIndex = 3;
			this->button_CANCEL->Text = L"���";
			this->button_CANCEL->UseVisualStyleBackColor = true;
			this->button_CANCEL->Click += gcnew System::EventHandler(this, &AddNPC::button_CANCEL_Click);
			// 
			// AddNPC
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 111);
			this->Controls->Add(this->button_CANCEL);
			this->Controls->Add(this->button_OK);
			this->Controls->Add(this->Label_NPCID);
			this->Controls->Add(this->textBox_NPCID);
			this->Name = L"AddNPC";
			this->Text = L"AddNPC";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button_OK_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->DialogResult = System::Windows::Forms::DialogResult::OK;
				 this->Close();
			 }
	private: System::Void button_CANCEL_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->Close();
			 }
};
}
