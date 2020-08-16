#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Character_NET {

	/// <summary>
	/// DereferenceTalentEffectIDViewer�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	enum DEREF_TYPE
	{
		DT_NONE,
		DT_EFFECT,
		DT_EVENT,
		DT_HIT
	};

	public ref class DereferenceTalentIDViewer : public System::Windows::Forms::Form
	{
	private:
		int						m_nTalentID;

		DEREF_TYPE				m_eDerefType;

	public:
		DereferenceTalentIDViewer(void)
		{
			InitializeComponent();
			//
			//TODO: ������ �ڵ带 ���⿡ �߰��մϴ�.
			//

			m_nTalentID = 0;
			m_eDerefType = DT_NONE;
		}

		void					SetTalentID(int nID) { m_nTalentID = nID; }
		void					SetDerefType(DEREF_TYPE eType) { m_eDerefType = eType; }

	protected:
		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		~DereferenceTalentIDViewer()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListBox^  DereferenceTalentEffectID_listBox;
	private: System::Windows::Forms::Button^  button1;
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
			this->DereferenceTalentEffectID_listBox = (gcnew System::Windows::Forms::ListBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// DereferenceTalentEffectID_listBox
			// 
			this->DereferenceTalentEffectID_listBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->DereferenceTalentEffectID_listBox->FormattingEnabled = true;
			this->DereferenceTalentEffectID_listBox->ItemHeight = 12;
			this->DereferenceTalentEffectID_listBox->Location = System::Drawing::Point(12, 10);
			this->DereferenceTalentEffectID_listBox->Name = L"DereferenceTalentEffectID_listBox";
			this->DereferenceTalentEffectID_listBox->Size = System::Drawing::Size(202, 244);
			this->DereferenceTalentEffectID_listBox->TabIndex = 0;
			// 
			// button1
			// 
			this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->button1->Location = System::Drawing::Point(64, 278);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(99, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Ȯ��";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &DereferenceTalentIDViewer::button1_Click);
			// 
			// DereferenceTalentIDViewer
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(226, 309);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->DereferenceTalentEffectID_listBox);
			this->Name = L"DereferenceTalentIDViewer";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"�ŷ�Ʈ ����Ʈ ������ �ŷ�Ʈ ���̵�";
			this->Load += gcnew System::EventHandler(this, &DereferenceTalentIDViewer::DereferenceTalentEffectIDViewer_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void DereferenceTalentEffectIDViewer_Load(System::Object^  sender, System::EventArgs^  e);
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->Close();
			 }
	};
}
