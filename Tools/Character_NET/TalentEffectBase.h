#pragma once

#include "XTalentInfo.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Character_NET {

	/// <summary>
	/// TalentEffectBase�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class TalentEffectBase : public System::Windows::Forms::Form
	{
	public:
		TalentEffectBase(void)
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
		~TalentEffectBase()
		{
			if (components)
			{
				delete components;
			}
		}










	private: System::Windows::Forms::Button^  button1;
	protected: 

	private:
		/// <summary>
		/// �ʼ� �����̳� �����Դϴ�.
		bool						m_bTextChange;
		TTalentEffectMgr *			m_pEffectMgr;
		TALENT_EFFECT_BASE_TYPE		m_nEffectBaseType;
	private: System::Windows::Forms::PropertyGrid^  TalentEffectase_Att;
			 /// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����̳� ������ �ʿ��� �޼����Դϴ�.
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->TalentEffectase_Att = (gcnew System::Windows::Forms::PropertyGrid());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(85, 451);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 10;
			this->button1->Text = L"OK";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &TalentEffectBase::button1_Click);
			// 
			// TalentEffectase_Att
			// 
			this->TalentEffectase_Att->Location = System::Drawing::Point(12, 12);
			this->TalentEffectase_Att->Name = L"TalentEffectase_Att";
			this->TalentEffectase_Att->Size = System::Drawing::Size(238, 433);
			this->TalentEffectase_Att->TabIndex = 11;
			this->TalentEffectase_Att->PropertyValueChanged += gcnew System::Windows::Forms::PropertyValueChangedEventHandler(this, &TalentEffectBase::TalentEffectase_Att_PropertyValueChanged);
			// 
			// TalentEffectBase
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(262, 486);
			this->Controls->Add(this->TalentEffectase_Att);
			this->Controls->Add(this->button1);
			this->Name = L"TalentEffectBase";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"TalentEffectBase";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void		SetPropertyTalentEffectBase();
	private: System::Void		SetTalentEffectBaseToProperty(XTalentEffectBase * pEffectBase);
	private: System::Void		SetPropertyToTalentEffectBase(XTalentEffectBase * pEffectBase);

	private: System::Void		SetEffectBaseType(string& strBuff, TALENT_EFFECT_BASE_TYPE nBaseType);
	private: TALENT_EFFECT_BASE_TYPE GetEffectBaseType(string& strBuff);
	private: System::Void		SetTextImpactPositionList(string& strBuff, IMPACTPOS nImapctPos);
	private: IMPACTPOS			GetEffectImpactPositionList(string& strBuff);

	public: System::Void		SetTalentEffectBaseInfo(TTalentEffectMgr *	pEffectMgr);
	public: bool				GetTalentEffectBaseInfo(TTalentEffectMgr *	pOutEffectMgr);

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->DialogResult = System::Windows::Forms::DialogResult::OK;
				 this->Close();
			 }

	private: System::Void TalentEffectase_Att_PropertyValueChanged(System::Object^  s, System::Windows::Forms::PropertyValueChangedEventArgs^  e) 
			 {
				 SetPropertyTalentEffectBase();
			 }
	};
}
