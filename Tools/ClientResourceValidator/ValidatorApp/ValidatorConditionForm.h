#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Threading;

class VValidatorInfoMgr;
class VReportMgr;

namespace ValidatorApp {

	/// <summary>
	/// ValidatorConditionForm�� ���� ����Դϴ�.
	///
	/// ���: �� Ŭ������ �̸��� �����ϸ� �� Ŭ������ �����ϴ�
	///          ��� .resx ���ϰ� ���õ� �����Ǵ� ���ҽ� �����Ϸ� ������
	///          '���ҽ� ���� �̸�' �Ӽ��� �����ؾ� �մϴ�. �׷��� ���� ���
	///          �����̳ʴ� �� ���� ���õ� ����ȭ�� ���ҽ���
	///          �ùٸ��� ��ȣ �ۿ��� �� �����ϴ�.
	/// </summary>
	public ref class ValidatorConditionForm : public System::Windows::Forms::Form
	{
	public:
		ValidatorConditionForm(void)
		{
			InitializeComponent();
			//
			//TODO: ������ �ڵ带 ���⿡ �߰��մϴ�.
			//

			m_pInfoMgr = NULL;
			m_pReportMgr = NULL;
		}

	protected:
		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		~ValidatorConditionForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
	private: System::Windows::Forms::ListBox^  ValidatorCondition_listBox;
	private: System::Windows::Forms::ProgressBar^  ValidatorCondition_progressBar;
	protected: 

		VValidatorInfoMgr *					m_pInfoMgr;
		VReportMgr *						m_pReportMgr;

	private:
		/// <summary>
		/// �ʼ� �����̳� �����Դϴ�.
		/// </summary>
		System::ComponentModel::Container ^components;

	private:
		long 			LoadingMethod(BackgroundWorker^ worker, DoWorkEventArgs ^ e, int& nIndex);
		long 			ValidatorConditionReportMethod(BackgroundWorker^ worker, DoWorkEventArgs ^ e, int& nIndex);

		void			ValidatorCondition_ListBoxText(int nIndex, String^ strText);

	public:
		void			SetValidatorInfoMgr(VValidatorInfoMgr* pt)	{ m_pInfoMgr = pt; }
		void			SetReportMgr(VReportMgr* pt)	{ m_pReportMgr = pt; }

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����̳� ������ �ʿ��� �޼����Դϴ�.
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
		/// </summary>
		void InitializeComponent(void)
		{
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->ValidatorCondition_listBox = (gcnew System::Windows::Forms::ListBox());
			this->ValidatorCondition_progressBar = (gcnew System::Windows::Forms::ProgressBar());
			this->SuspendLayout();
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->WorkerReportsProgress = true;
			this->backgroundWorker1->WorkerSupportsCancellation = true;
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &ValidatorConditionForm::backgroundWorker1_DoWork);
			this->backgroundWorker1->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &ValidatorConditionForm::backgroundWorker1_RunWorkerCompleted);
			this->backgroundWorker1->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &ValidatorConditionForm::backgroundWorker1_ProgressChanged);
			// 
			// ValidatorCondition_listBox
			// 
			this->ValidatorCondition_listBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->ValidatorCondition_listBox->FormattingEnabled = true;
			this->ValidatorCondition_listBox->ItemHeight = 12;
			this->ValidatorCondition_listBox->Location = System::Drawing::Point(10, 6);
			this->ValidatorCondition_listBox->Name = L"ValidatorCondition_listBox";
			this->ValidatorCondition_listBox->Size = System::Drawing::Size(475, 220);
			this->ValidatorCondition_listBox->TabIndex = 0;
			// 
			// ValidatorCondition_progressBar
			// 
			this->ValidatorCondition_progressBar->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->ValidatorCondition_progressBar->Location = System::Drawing::Point(10, 232);
			this->ValidatorCondition_progressBar->Name = L"ValidatorCondition_progressBar";
			this->ValidatorCondition_progressBar->Size = System::Drawing::Size(474, 20);
			this->ValidatorCondition_progressBar->TabIndex = 1;
			// 
			// ValidatorConditionForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(494, 259);
			this->Controls->Add(this->ValidatorCondition_progressBar);
			this->Controls->Add(this->ValidatorCondition_listBox);
			this->Name = L"ValidatorConditionForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Validator Condition";
			this->Shown += gcnew System::EventHandler(this, &ValidatorConditionForm::ValidatorConditionForm_Shown);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
				 BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);

				 int nIndex = 0;

				 e->Result = LoadingMethod(worker, e, nIndex);
				 e->Result = ValidatorConditionReportMethod(worker, e, nIndex);
			 }
	private: System::Void backgroundWorker1_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e) {
				 this->ValidatorCondition_progressBar->Value = e->ProgressPercentage;

				 ValidatorCondition_ListBoxText(e->UserState->GetHashCode(), e->UserState->ToString());
			 }
	private: System::Void backgroundWorker1_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) {
				 //�۾��� ������ �߻��ϸ� �޼����� ����
				 if (e->Error != nullptr)
				 {
					 MessageBox::Show(e->Error->Message);
				 }
				 //�۾��� ��ҵǾ����� ��ҵǾ��ٴ� �޼����� �����ش�
				 else if (e->Cancelled)
				 {                
					 //lblResult.Text = "�۾��� ��ҵǾ����ϴ�";
				 }
				 //�۾��� �Ϸ�Ǹ� ������� �����ش�
				 else
				 {
					 //����� ���
				 }

				 if(ValidatorCondition_progressBar->Value == 100)
				 {
					 Thread::Sleep(1000);
					 this->DialogResult = System::Windows::Forms::DialogResult::OK;
					 this->Close();
				 }
			 }
	private: System::Void ValidatorConditionForm_Shown(System::Object^  sender, System::EventArgs^  e) {
				 backgroundWorker1->RunWorkerAsync();
			 }
};
}
