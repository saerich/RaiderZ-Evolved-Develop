#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Character_NET {

	/// <summary>
	/// EditText_One�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class EditText_One : public System::Windows::Forms::Form
	{
	public:
		EditText_One(void)
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
		~EditText_One()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::TextBox^  Edit_textBox;
	protected: 
	private: System::Windows::Forms::Button^  OK_button;
	public: 
	private: System::Windows::Forms::Button^  Cancle_button;

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
			this->Edit_textBox = (gcnew System::Windows::Forms::TextBox());
			this->OK_button = (gcnew System::Windows::Forms::Button());
			this->Cancle_button = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// Edit_textBox
			// 
			this->Edit_textBox->Location = System::Drawing::Point(55, 12);
			this->Edit_textBox->Name = L"Edit_textBox";
			this->Edit_textBox->Size = System::Drawing::Size(160, 21);
			this->Edit_textBox->TabIndex = 0;
			// 
			// OK_button
			// 
			this->OK_button->Location = System::Drawing::Point(45, 42);
			this->OK_button->Name = L"OK_button";
			this->OK_button->Size = System::Drawing::Size(75, 23);
			this->OK_button->TabIndex = 1;
			this->OK_button->Text = L"OK";
			this->OK_button->UseVisualStyleBackColor = true;
			this->OK_button->Click += gcnew System::EventHandler(this, &EditText_One::OK_button_Click);
			// 
			// Cancle_button
			// 
			this->Cancle_button->Location = System::Drawing::Point(151, 42);
			this->Cancle_button->Name = L"Cancle_button";
			this->Cancle_button->Size = System::Drawing::Size(75, 23);
			this->Cancle_button->TabIndex = 2;
			this->Cancle_button->Text = L"Cancle";
			this->Cancle_button->UseVisualStyleBackColor = true;
			this->Cancle_button->Click += gcnew System::EventHandler(this, &EditText_One::Cancle_button_Click);
			// 
			// EditText_One
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(269, 77);
			this->Controls->Add(this->Cancle_button);
			this->Controls->Add(this->OK_button);
			this->Controls->Add(this->Edit_textBox);
			this->Name = L"EditText_One";
			this->Text = L"EditText_One";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void OK_button_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->DialogResult = System::Windows::Forms::DialogResult::OK;
				 this->Close();
			 }
	private: System::Void Cancle_button_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->Close();
			 }
	};
}
