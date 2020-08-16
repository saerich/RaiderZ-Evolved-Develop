#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Character_NET {

	/// <summary>
	/// AnimationHitSoundTabPage�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class AnimationHitSoundTabPage : public System::Windows::Forms::Form
	{
	private:
		bool	m_bSoundLoad;

	public:
		AnimationHitSoundTabPage(void)
		{
			InitializeComponent();
			//
			//TODO: ������ �ڵ带 ���⿡ �߰��մϴ�.
			//
			m_bSoundLoad = false;
		}

		void InitEffectList();

	protected:
		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		~AnimationHitSoundTabPage()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::ComboBox^  HitSound_comboBox;
	private: System::Windows::Forms::Button^  OK_button;
	public: System::Windows::Forms::Label^  AnimationName_label;
	private: 
	public: 
	protected: 

	protected: 

	protected: 


	public: 
	protected: 

	protected: 

	public: 

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
			this->HitSound_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->OK_button = (gcnew System::Windows::Forms::Button());
			this->AnimationName_label = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// HitSound_comboBox
			// 
			this->HitSound_comboBox->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
			this->HitSound_comboBox->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
			this->HitSound_comboBox->FormattingEnabled = true;
			this->HitSound_comboBox->Location = System::Drawing::Point(16, 32);
			this->HitSound_comboBox->Name = L"HitSound_comboBox";
			this->HitSound_comboBox->Size = System::Drawing::Size(190, 20);
			this->HitSound_comboBox->Sorted = true;
			this->HitSound_comboBox->TabIndex = 0;
			this->HitSound_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &AnimationHitSoundTabPage::HitSound_comboBox_SelectedIndexChanged);
			this->HitSound_comboBox->TextChanged += gcnew System::EventHandler(this, &AnimationHitSoundTabPage::HitSound_comboBox_TextChanged);
			// 
			// OK_button
			// 
			this->OK_button->Location = System::Drawing::Point(74, 68);
			this->OK_button->Name = L"OK_button";
			this->OK_button->Size = System::Drawing::Size(75, 23);
			this->OK_button->TabIndex = 1;
			this->OK_button->Text = L"����";
			this->OK_button->UseVisualStyleBackColor = true;
			this->OK_button->Click += gcnew System::EventHandler(this, &AnimationHitSoundTabPage::OK_button_Click);
			// 
			// AnimationName_label
			// 
			this->AnimationName_label->AutoSize = true;
			this->AnimationName_label->Location = System::Drawing::Point(14, 9);
			this->AnimationName_label->Name = L"AnimationName_label";
			this->AnimationName_label->Size = System::Drawing::Size(0, 12);
			this->AnimationName_label->TabIndex = 2;
			// 
			// AnimationHitSoundTabPage
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Window;
			this->ClientSize = System::Drawing::Size(231, 112);
			this->ControlBox = false;
			this->Controls->Add(this->AnimationName_label);
			this->Controls->Add(this->OK_button);
			this->Controls->Add(this->HitSound_comboBox);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"AnimationHitSoundTabPage";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->Text = L"AnimationHitSoundTabPage";
			this->Load += gcnew System::EventHandler(this, &AnimationHitSoundTabPage::AnimationHitSoundTabPage_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void HitSoundAddbutton_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void AnimationHitSoundTabPage_Load(System::Object^  sender, System::EventArgs^  e);
	private: System::Void HitSound_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void HitSound_comboBox_TextChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void OK_button_Click(System::Object^  sender, System::EventArgs^  e);
};
}
