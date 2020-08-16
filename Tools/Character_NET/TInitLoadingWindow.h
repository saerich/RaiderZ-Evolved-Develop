#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Character_NET {

	/// <summary>
	/// TInitLoadingWindow�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class TInitLoadingWindow : public System::Windows::Forms::Form
	{
	private:
		System::Drawing::Graphics^		m_formGraphics;
		System::Drawing::SolidBrush^	m_fontBrush;	
		System::Drawing::Font^			m_drawFont;			
		System::Drawing::StringFormat^	m_drawFormat;
		System::Drawing::RectangleF		m_drawRect;


	public:
		TInitLoadingWindow(void)
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
		~TInitLoadingWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  InitLoadingPictureBox;
	protected: 

	private:
		/// <summary>
		/// �ʼ� �����̳� �����Դϴ�.
		/// </summary>
		System::ComponentModel::Container ^components;
	private: System::Windows::Forms::Label^  InitLoadInfoLabel;


		Image^ m_imgInitLoading;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����̳� ������ �ʿ��� �޼����Դϴ�.
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
		/// </summary>
		void InitializeComponent(void)
		{
			this->InitLoadingPictureBox = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->InitLoadingPictureBox))->BeginInit();
			this->SuspendLayout();
			// 
			// InitLoadingPictureBox
			// 
			this->InitLoadingPictureBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->InitLoadingPictureBox->Location = System::Drawing::Point(0, 0);
			this->InitLoadingPictureBox->Name = L"InitLoadingPictureBox";
			this->InitLoadingPictureBox->Size = System::Drawing::Size(292, 266);
			this->InitLoadingPictureBox->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->InitLoadingPictureBox->TabIndex = 0;
			this->InitLoadingPictureBox->TabStop = false;
			this->InitLoadingPictureBox->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &TInitLoadingWindow::InitLoadingPictureBox_Paint);
			// 
			// TInitLoadingWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 266);
			this->ControlBox = false;
			this->Controls->Add(this->InitLoadingPictureBox);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Name = L"TInitLoadingWindow";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Loading";
			this->Load += gcnew System::EventHandler(this, &TInitLoadingWindow::TInitLoadingWindow_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->InitLoadingPictureBox))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void TInitLoadingWindow_Load(System::Object^  sender, System::EventArgs^  e);
	private: System::Void InitLoadingPictureBox_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
	public:  System::Void InitLoadingInfoText(string strLoadingIndo);
	};
}
