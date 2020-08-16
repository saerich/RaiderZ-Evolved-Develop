#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Character_NET {

	/// <summary>
	/// OverlayAnimationTestTabPage�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class OverlayAnimationTestTabPage : public System::Windows::Forms::Form
	{
	public:
		OverlayAnimationTestTabPage(void)
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
		~OverlayAnimationTestTabPage()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	public: System::Windows::Forms::TextBox^  UpperPartAnimation_textBox;
	private: 
	protected: 

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  LowerPartAnimation_textBox;
	private: System::Windows::Forms::Button^  Play_button;
	private: System::Windows::Forms::Button^  Stop_button;

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
			this->UpperPartAnimation_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->LowerPartAnimation_textBox = (gcnew System::Windows::Forms::TextBox());
			this->Play_button = (gcnew System::Windows::Forms::Button());
			this->Stop_button = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 22);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(29, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"��ü";
			// 
			// UpperPartAnimation_textBox
			// 
			this->UpperPartAnimation_textBox->AllowDrop = true;
			this->UpperPartAnimation_textBox->Location = System::Drawing::Point(52, 16);
			this->UpperPartAnimation_textBox->Name = L"UpperPartAnimation_textBox";
			this->UpperPartAnimation_textBox->Size = System::Drawing::Size(211, 21);
			this->UpperPartAnimation_textBox->TabIndex = 1;
			this->UpperPartAnimation_textBox->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &OverlayAnimationTestTabPage::UpperPartAnimationTest_textBox_DragDrop);
			this->UpperPartAnimation_textBox->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &OverlayAnimationTestTabPage::UpperPartAnimationTest_textBox_DragEnter);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 59);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(29, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"��ü";
			// 
			// LowerPartAnimation_textBox
			// 
			this->LowerPartAnimation_textBox->AllowDrop = true;
			this->LowerPartAnimation_textBox->Location = System::Drawing::Point(52, 56);
			this->LowerPartAnimation_textBox->Name = L"LowerPartAnimation_textBox";
			this->LowerPartAnimation_textBox->Size = System::Drawing::Size(210, 21);
			this->LowerPartAnimation_textBox->TabIndex = 3;
			this->LowerPartAnimation_textBox->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &OverlayAnimationTestTabPage::LowerPartAnimationTest_textBox_DragDrop);
			this->LowerPartAnimation_textBox->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &OverlayAnimationTestTabPage::LowerPartAnimationTest_textBox_DragEnter);
			// 
			// Play_button
			// 
			this->Play_button->Location = System::Drawing::Point(17, 97);
			this->Play_button->Name = L"Play_button";
			this->Play_button->Size = System::Drawing::Size(75, 23);
			this->Play_button->TabIndex = 4;
			this->Play_button->Text = L"Play";
			this->Play_button->UseVisualStyleBackColor = true;
			this->Play_button->Click += gcnew System::EventHandler(this, &OverlayAnimationTestTabPage::Play_button_Click);
			// 
			// Stop_button
			// 
			this->Stop_button->Location = System::Drawing::Point(17, 126);
			this->Stop_button->Name = L"Stop_button";
			this->Stop_button->Size = System::Drawing::Size(75, 23);
			this->Stop_button->TabIndex = 5;
			this->Stop_button->Text = L"Stop";
			this->Stop_button->UseVisualStyleBackColor = true;
			this->Stop_button->Click += gcnew System::EventHandler(this, &OverlayAnimationTestTabPage::Stop_button_Click);
			// 
			// OverlayAnimationTestTabPage
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Window;
			this->ClientSize = System::Drawing::Size(292, 266);
			this->Controls->Add(this->Stop_button);
			this->Controls->Add(this->Play_button);
			this->Controls->Add(this->LowerPartAnimation_textBox);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->UpperPartAnimation_textBox);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Name = L"OverlayAnimationTestTabPage";
			this->Text = L"OverlayAnimationTestTabPage";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion




		private: System::Void UpperPartAnimationTest_textBox_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e);
		private: System::Void UpperPartAnimationTest_textBox_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e);
		private: System::Void LowerPartAnimationTest_textBox_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e);
		private: System::Void LowerPartAnimationTest_textBox_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e);

		public: System::Void SetLowerPartAnimation(string strAniName);

		private: System::Void Play_button_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Stop_button_Click(System::Object^  sender, System::EventArgs^  e);
};
}
