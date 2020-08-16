#pragma once

#include "car.h"

namespace Demo {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace ReflectivePropertyFormNameSpace;

	/// <summary>
	/// Form1�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
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
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	protected: 

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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(40, 32);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(204, 64);
			this->button1->TabIndex = 0;
			this->button1->Text = L"ReflectivePropertyGrid�� �߰��ϴ� ��ư";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 266);
			this->Controls->Add(this->button1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
			 {//���Ⱑ ReflectivePropertyGrid��ư �������� ȣ��Ǵ� ���̴� ~

				ReflectivePropertyForm^ reflectivePropertyForm = gcnew ReflectivePropertyForm;	//������
				reflectivePropertyForm->InitializeComponent();									//���ʱ�ȭ����
	
				static ReCar reflectiveCar;															//���÷��� Ŭ���� ��ü ����		
				reflectivePropertyForm->setText("������â���� ���̴� �ؽ�Ʈ");	
				reflectivePropertyForm->setBaseObject((void*)&reflectiveCar);					//���� ���÷��� Ŭ��������
				
				reflectivePropertyForm->addAllReflection();										//����� ���÷���Ŭ���� ����ȭ

				reflectivePropertyForm->Show();													//�����̱�
				reflectivePropertyForm->CompleteToInitializeComponent();						//���ʱ�ȭ ��
			 }
	};
}

