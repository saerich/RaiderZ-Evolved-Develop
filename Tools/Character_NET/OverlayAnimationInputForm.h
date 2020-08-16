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
	/// OverlayAnimationInputForm�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class OverlayAnimationInputForm : public System::Windows::Forms::Form
	{
	private:
		ProgressBarForm	m_loadProgressBar;
		int				m_nFullCount;
		int				m_nCurrCount;

	private:
		void		SetProgressBar(int nCount);

	public:
		void		SetAnimation();
		void		SetBoneName();

		void		SetOverlayAnimationData(String^ strOverlayAniName, String^ strBoneName);

		void		OverlayAnimationInputForm_DataLoad();

		OverlayAnimationInputForm(void)
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
		~OverlayAnimationInputForm()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::ComboBox^  OverlayAnimation_comboBox;
	protected: 
	public: System::Windows::Forms::ComboBox^  BoneSelect_comboBox;
	private: System::Windows::Forms::Label^  label1;
	public: 
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  OK_button;
	private: System::Windows::Forms::Button^  Cancel_button;

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
			this->OverlayAnimation_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->BoneSelect_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->OK_button = (gcnew System::Windows::Forms::Button());
			this->Cancel_button = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// OverlayAnimation_comboBox
			// 
			this->OverlayAnimation_comboBox->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
			this->OverlayAnimation_comboBox->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
			this->OverlayAnimation_comboBox->FormattingEnabled = true;
			this->OverlayAnimation_comboBox->Location = System::Drawing::Point(154, 22);
			this->OverlayAnimation_comboBox->Name = L"OverlayAnimation_comboBox";
			this->OverlayAnimation_comboBox->Size = System::Drawing::Size(212, 20);
			this->OverlayAnimation_comboBox->Sorted = true;
			this->OverlayAnimation_comboBox->TabIndex = 0;
			// 
			// BoneSelect_comboBox
			// 
			this->BoneSelect_comboBox->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
			this->BoneSelect_comboBox->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
			this->BoneSelect_comboBox->FormattingEnabled = true;
			this->BoneSelect_comboBox->Location = System::Drawing::Point(154, 57);
			this->BoneSelect_comboBox->Name = L"BoneSelect_comboBox";
			this->BoneSelect_comboBox->Size = System::Drawing::Size(212, 20);
			this->BoneSelect_comboBox->Sorted = true;
			this->BoneSelect_comboBox->TabIndex = 1;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 25);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(117, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"�������� �ִϸ��̼�";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 60);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(45, 12);
			this->label2->TabIndex = 3;
			this->label2->Text = L"�� ����";
			// 
			// OK_button
			// 
			this->OK_button->Location = System::Drawing::Point(90, 99);
			this->OK_button->Name = L"OK_button";
			this->OK_button->Size = System::Drawing::Size(75, 23);
			this->OK_button->TabIndex = 4;
			this->OK_button->Text = L"����";
			this->OK_button->UseVisualStyleBackColor = true;
			this->OK_button->Click += gcnew System::EventHandler(this, &OverlayAnimationInputForm::OK_button_Click);
			// 
			// Cancel_button
			// 
			this->Cancel_button->Location = System::Drawing::Point(204, 99);
			this->Cancel_button->Name = L"Cancel_button";
			this->Cancel_button->Size = System::Drawing::Size(75, 23);
			this->Cancel_button->TabIndex = 5;
			this->Cancel_button->Text = L"���";
			this->Cancel_button->UseVisualStyleBackColor = true;
			this->Cancel_button->Click += gcnew System::EventHandler(this, &OverlayAnimationInputForm::Cancel_button_Click);
			// 
			// OverlayAnimationInputForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(379, 136);
			this->Controls->Add(this->Cancel_button);
			this->Controls->Add(this->OK_button);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->BoneSelect_comboBox);
			this->Controls->Add(this->OverlayAnimation_comboBox);
			this->Name = L"OverlayAnimationInputForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"OverlayAnimationInputForm";
			this->Shown += gcnew System::EventHandler(this, &OverlayAnimationInputForm::OverlayAnimationInputForm_Shown);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void OK_button_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->DialogResult = System::Windows::Forms::DialogResult::OK;
				 this->Close();
			 }
private: System::Void Cancel_button_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }
private: System::Void OverlayAnimationInputForm_Shown(System::Object^  sender, System::EventArgs^  e) {
			 m_loadProgressBar.Hide();
		 }
};
}
