#pragma once

#include "VErrorMsg.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace ValidatorApp {

	/// <summary>
	/// DataLoad_Validator�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class DataLoad_Validator : public System::Windows::Forms::Form
	{
	public:
		DataLoad_Validator(void)
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
		~DataLoad_Validator()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  OK_button;
	protected: 
	private: System::Windows::Forms::ListBox^  LoadingErrorResult_listBox;

	private:
		/// <summary>
		/// �ʼ� �����̳� �����Դϴ�.
		/// </summary>
		System::ComponentModel::Container ^components;

	public:
		void						AddErrorMsg(VErrorMsg& errorMsg);

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����̳� ������ �ʿ��� �޼����Դϴ�.
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
		/// </summary>
		void InitializeComponent(void)
		{
			this->OK_button = (gcnew System::Windows::Forms::Button());
			this->LoadingErrorResult_listBox = (gcnew System::Windows::Forms::ListBox());
			this->SuspendLayout();
			// 
			// OK_button
			// 
			this->OK_button->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->OK_button->Location = System::Drawing::Point(203, 150);
			this->OK_button->Name = L"OK_button";
			this->OK_button->Size = System::Drawing::Size(76, 25);
			this->OK_button->TabIndex = 0;
			this->OK_button->Text = L"Ȯ��";
			this->OK_button->UseVisualStyleBackColor = true;
			this->OK_button->Click += gcnew System::EventHandler(this, &DataLoad_Validator::OK_button_Click);
			// 
			// LoadingErrorResult_listBox
			// 
			this->LoadingErrorResult_listBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->LoadingErrorResult_listBox->Font = (gcnew System::Drawing::Font(L"����", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(129)));
			this->LoadingErrorResult_listBox->FormattingEnabled = true;
			this->LoadingErrorResult_listBox->HorizontalScrollbar = true;
			this->LoadingErrorResult_listBox->Location = System::Drawing::Point(9, 6);
			this->LoadingErrorResult_listBox->Name = L"LoadingErrorResult_listBox";
			this->LoadingErrorResult_listBox->Size = System::Drawing::Size(478, 134);
			this->LoadingErrorResult_listBox->TabIndex = 1;
			// 
			// DataLoad_Validator
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Window;
			this->ClientSize = System::Drawing::Size(496, 187);
			this->Controls->Add(this->LoadingErrorResult_listBox);
			this->Controls->Add(this->OK_button);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"DataLoad_Validator";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"�ε� ���� ����Ʈ";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void OK_button_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->Close();
			 }

	};
}
