#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace EffectTool_NET {

	/// <summary>
	/// PropertyForm�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class PropertyForm : public System::Windows::Forms::Form
	{
	public:
		PropertyForm(void)
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
		~PropertyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::PropertyGrid^  sceneNodePropertyGrid;
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
			this->sceneNodePropertyGrid = (gcnew System::Windows::Forms::PropertyGrid());
			this->SuspendLayout();
			// 
			// sceneNodePropertyGrid
			// 
			this->sceneNodePropertyGrid->Dock = System::Windows::Forms::DockStyle::Fill;
			this->sceneNodePropertyGrid->Location = System::Drawing::Point(0, 0);
			this->sceneNodePropertyGrid->Name = L"sceneNodePropertyGrid";
			this->sceneNodePropertyGrid->Size = System::Drawing::Size(292, 273);
			this->sceneNodePropertyGrid->TabIndex = 0;
			// 
			// PropertyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 273);
			this->Controls->Add(this->sceneNodePropertyGrid);
			this->Name = L"PropertyForm";
			this->Text = L"PropertyForm";
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
