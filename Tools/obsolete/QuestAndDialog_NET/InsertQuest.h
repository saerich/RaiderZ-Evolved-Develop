#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace QuestAndDialog_NET {

	/// <summary>
	/// InsertQuest�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class InsertQuest : public System::Windows::Forms::Form
	{
	public:
		InsertQuest(void)
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
		~InsertQuest()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label_InsertQuest;
	protected: 

	private: System::Windows::Forms::TextBox^  textBox_InsertQuest;

	private: System::Windows::Forms::Button^  button_Confirm;
	private: System::Windows::Forms::Button^  button_Cancel;
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
			this->label_InsertQuest = (gcnew System::Windows::Forms::Label());
			this->textBox_InsertQuest = (gcnew System::Windows::Forms::TextBox());
			this->button_Confirm = (gcnew System::Windows::Forms::Button());
			this->button_Cancel = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label_InsertQuest
			// 
			this->label_InsertQuest->AutoSize = true;
			this->label_InsertQuest->Location = System::Drawing::Point(41, 29);
			this->label_InsertQuest->Name = L"label_InsertQuest";
			this->label_InsertQuest->Size = System::Drawing::Size(49, 12);
			this->label_InsertQuest->TabIndex = 0;
			this->label_InsertQuest->Text = L"QuestID";
			// 
			// textBox_InsertQuest
			// 
			this->textBox_InsertQuest->Location = System::Drawing::Point(106, 26);
			this->textBox_InsertQuest->Name = L"textBox_InsertQuest";
			this->textBox_InsertQuest->Size = System::Drawing::Size(100, 21);
			this->textBox_InsertQuest->TabIndex = 1;
			this->textBox_InsertQuest->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &InsertQuest::textBox_InsertQuest_KeyDown);
			// 
			// button_Confirm
			// 
			this->button_Confirm->Location = System::Drawing::Point(43, 70);
			this->button_Confirm->Name = L"button_Confirm";
			this->button_Confirm->Size = System::Drawing::Size(75, 23);
			this->button_Confirm->TabIndex = 2;
			this->button_Confirm->Text = L"Ȯ��";
			this->button_Confirm->UseVisualStyleBackColor = true;
			this->button_Confirm->Click += gcnew System::EventHandler(this, &InsertQuest::button_Confirm_Click);
			// 
			// button_Cancel
			// 
			this->button_Cancel->Location = System::Drawing::Point(131, 70);
			this->button_Cancel->Name = L"button_Cancel";
			this->button_Cancel->Size = System::Drawing::Size(75, 23);
			this->button_Cancel->TabIndex = 3;
			this->button_Cancel->Text = L"���";
			this->button_Cancel->UseVisualStyleBackColor = true;
			this->button_Cancel->Click += gcnew System::EventHandler(this, &InsertQuest::button_Cancel_Click);
			// 
			// InsertQuest
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(242, 116);
			this->Controls->Add(this->button_Cancel);
			this->Controls->Add(this->button_Confirm);
			this->Controls->Add(this->textBox_InsertQuest);
			this->Controls->Add(this->label_InsertQuest);
			this->Name = L"InsertQuest";
			this->Text = L"InsertQuest";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public: int GetQuestID() {
				int nQuestID = MStringToInt(this->textBox_InsertQuest->Text->ToString());
				return nQuestID;
			 }

	//�ڵ��߰�------------------------------------------------------------------------------------

	private: System::Void button_Confirm_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->DialogResult = System::Windows::Forms::DialogResult::OK;
				 this->Close();
			 }
	private: System::Void button_Cancel_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->Close();
			 }

	

	private: System::Void textBox_InsertQuest_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
				 if (Keys::Enter == e->KeyCode)
				 {
					 this->button_Confirm_Click(sender, e);
				 }
				 else if (Keys::Escape == e->KeyCode)
				 {
					 this->Close();
				 }
			 }
};
}
