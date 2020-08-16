#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace ColorPickerControls {

	/// <summary>
	/// ColorPicker�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>

	public ref class ColorPickerDlg : public System::Windows::Forms::Form
	{
	public:
		ColorPickerDlg(void)
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
		~ColorPickerDlg()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  Close_button;
	private: Sano::PersonalProjects::ColorPicker::Controls::ColorPanel^  colorPanel;
	public:	System::Drawing::Color pColor;

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
			this->Close_button = (gcnew System::Windows::Forms::Button());
			this->colorPanel = (gcnew Sano::PersonalProjects::ColorPicker::Controls::ColorPanel());
			this->SuspendLayout();
			// 
			// Close_button
			// 
			this->Close_button->Location = System::Drawing::Point(211, 289);
			this->Close_button->Name = L"Close_button";
			this->Close_button->Size = System::Drawing::Size(181, 29);
			this->Close_button->TabIndex = 1;
			this->Close_button->Text = L"�ݱ�";
			this->Close_button->UseVisualStyleBackColor = true;
			this->Close_button->Click += gcnew System::EventHandler(this, &ColorPickerDlg::Close_button_Click);
			// 
			// colorPanel
			// 
			this->colorPanel->AllowDrop = true;
			this->colorPanel->Location = System::Drawing::Point(10, 12);
			this->colorPanel->Name = L"colorPanel";
			this->colorPanel->Size = System::Drawing::Size(600, 271);
			this->colorPanel->TabIndex = 4;
			// 
			// ColorPickerDlg
			// 
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
			this->ClientSize = System::Drawing::Size(621, 351);
			this->ControlBox = false;
			this->Controls->Add(this->colorPanel);
			this->Controls->Add(this->Close_button);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"ColorPickerDlg";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Text = L"ColorPicker";
			this->TopMost = true;
			this->ResumeLayout(false);

		}

#pragma endregion
	private: System::Void Close_button_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->DialogResult = System::Windows::Forms::DialogResult::OK;
				 this->Close();
			 }
	public: DWORD GetColor()
			{
				return colorPanel->picCurrentColor->BackColor.ToArgb();
			}
	public: void SetInitColor(System::Drawing::Color color)
			{
				colorPanel->picCurrentColor->BackColor = color;
			}
	public: void SetInitColor(DWORD dwcolor)
			{
				System::Drawing::Color color;
				color.FromArgb(dwcolor);
				SetInitColor(color);
			}
	};
}
