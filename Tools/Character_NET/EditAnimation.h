#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Character_NET {

	/// <summary>
	/// EditAnimation�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class EditAnimation : public System::Windows::Forms::Form
	{
	public:
		EditAnimation(void)
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
		~EditAnimation()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	public: System::Windows::Forms::TextBox^  NametextBox;
	public: System::Windows::Forms::TextBox^  FileNametextBox;
	private: System::Windows::Forms::Button^  Okbutton;
	private: System::Windows::Forms::Button^  Canclebutton;
	public: System::Windows::Forms::CheckBox^  LoopcheckBox;





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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->NametextBox = (gcnew System::Windows::Forms::TextBox());
			this->FileNametextBox = (gcnew System::Windows::Forms::TextBox());
			this->Okbutton = (gcnew System::Windows::Forms::Button());
			this->Canclebutton = (gcnew System::Windows::Forms::Button());
			this->LoopcheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(71, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Name       :";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 41);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(71, 12);
			this->label2->TabIndex = 1;
			this->label2->Text = L"File Name :";
			// 
			// NametextBox
			// 
			this->NametextBox->Location = System::Drawing::Point(89, 7);
			this->NametextBox->Name = L"NametextBox";
			this->NametextBox->Size = System::Drawing::Size(224, 21);
			this->NametextBox->TabIndex = 2;
			// 
			// FileNametextBox
			// 
			this->FileNametextBox->Location = System::Drawing::Point(89, 38);
			this->FileNametextBox->Name = L"FileNametextBox";
			this->FileNametextBox->ReadOnly = true;
			this->FileNametextBox->Size = System::Drawing::Size(224, 21);
			this->FileNametextBox->TabIndex = 3;
			// 
			// Okbutton
			// 
			this->Okbutton->Location = System::Drawing::Point(319, 5);
			this->Okbutton->Name = L"Okbutton";
			this->Okbutton->Size = System::Drawing::Size(75, 23);
			this->Okbutton->TabIndex = 4;
			this->Okbutton->Text = L"Ȯ��";
			this->Okbutton->UseVisualStyleBackColor = true;
			this->Okbutton->Click += gcnew System::EventHandler(this, &EditAnimation::Okbutton_Click);
			// 
			// Canclebutton
			// 
			this->Canclebutton->Location = System::Drawing::Point(319, 38);
			this->Canclebutton->Name = L"Canclebutton";
			this->Canclebutton->Size = System::Drawing::Size(75, 23);
			this->Canclebutton->TabIndex = 5;
			this->Canclebutton->Text = L"���";
			this->Canclebutton->UseVisualStyleBackColor = true;
			this->Canclebutton->Click += gcnew System::EventHandler(this, &EditAnimation::Canclebutton_Click);
			// 
			// LoopcheckBox
			// 
			this->LoopcheckBox->AutoSize = true;
			this->LoopcheckBox->Location = System::Drawing::Point(89, 65);
			this->LoopcheckBox->Name = L"LoopcheckBox";
			this->LoopcheckBox->Size = System::Drawing::Size(52, 16);
			this->LoopcheckBox->TabIndex = 6;
			this->LoopcheckBox->Text = L"Loop";
			this->LoopcheckBox->UseVisualStyleBackColor = true;
			// 
			// EditAnimation
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(402, 93);
			this->ControlBox = false;
			this->Controls->Add(this->LoopcheckBox);
			this->Controls->Add(this->Canclebutton);
			this->Controls->Add(this->Okbutton);
			this->Controls->Add(this->FileNametextBox);
			this->Controls->Add(this->NametextBox);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"EditAnimation";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"EditAnimation";
			this->Load += gcnew System::EventHandler(this, &EditAnimation::EditAnimation_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void Okbutton_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->DialogResult = System::Windows::Forms::DialogResult::OK;
			 this->Close();
		 }
private: System::Void EditAnimation_Load(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void Canclebutton_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }
};
}
