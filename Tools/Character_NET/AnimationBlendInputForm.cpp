#include "StdAfx.h"
#include "AnimationBlendInputForm.h"
#include "AnimationSelectEditForm.h"

void Character_NET::AnimationBlendInputForm::SetAnimationBlendData( vector<string>& vecSourceAniName, vector<string>& vecTargetAniName, float fBlendTime )
{
	//m_arrSourceAniName->CreateInstance(System::Object^, vecSourceAniName.size());
	m_arrSourceAniName = gcnew cli::array<String^>(vecSourceAniName.size());

	int i = 0;
	for(vector<string>::iterator itSource = vecSourceAniName.begin(); itSource != vecSourceAniName.end(); ++itSource, ++i)
	{
		m_arrSourceAniName->SetValue(gcnew String(itSource->c_str()), i);
	}

	SetTargetAnimationText(vecTargetAniName);

	char szSubName[16] = {0, };
	sprintf_s(szSubName, "%.4f", fBlendTime);
	BlendValue_textBox->Text = gcnew String(szSubName); 
}

System::Void Character_NET::AnimationBlendInputForm::TargetAnimationSelect_button_Click( System::Object^ sender, System::EventArgs^ e )
{
	vector<string> vecAnimationList;
	GetTargetAnimation(vecAnimationList);

	AnimationSelectEditForm animationEditForm;
	animationEditForm.SetEditAnimationList(vecAnimationList);
	animationEditForm.ShowDialog();

	System::Windows::Forms::DialogResult res = animationEditForm.DialogResult;
	if (res == System::Windows::Forms::DialogResult::OK)
	{
		vector<string>	vecTargetAniList;
		animationEditForm.GetEditAnimationList(vecTargetAniList);

		// �ߺ� üũ
		if(CheckDuplicationSourceAnimation(vecTargetAniList))
			System::Windows::Forms::MessageBox::Show(L"���� �ִϸ��̼����� ������ �� �� �����ϴ�. �ڵ����� �����ϴ�.", "�Է� ����!");

		SetTargetAnimationText(vecTargetAniList);
	}
}

void Character_NET::AnimationBlendInputForm::SetTargetAnimationText( vector<string>& vecTargetAniList )
{
	string strShowText = TStrings::GetAnimationLinkListText(vecTargetAniList);

	BlendAnimation_textBox->Text = gcnew String(strShowText.c_str());
}

void Character_NET::AnimationBlendInputForm::GetTargetAnimation( vector<string>& vecTargetAniList )
{
	string aniText = MStringToCharPointer(BlendAnimation_textBox->Text);

	TStrings::SetAnimationLinkListText(aniText, vecTargetAniList);
}

bool Character_NET::AnimationBlendInputForm::CheckDuplicationSourceAnimation( vector<string>& vecTargetAniList )
{
	bool bDuplication = false;

	System::Collections::IEnumerator^ myEnumerator = m_arrSourceAniName->GetEnumerator();
	while ( (myEnumerator->MoveNext()) && (myEnumerator->Current != nullptr) )
	{
		String^ strAni = safe_cast<String^>(myEnumerator->Current);

		vector<string>::iterator itCheck = vecTargetAniList.begin();
		while(itCheck != vecTargetAniList.end())
		{
			if(MStringToCharPointer(strAni) == *itCheck)
			{
				itCheck = vecTargetAniList.erase(itCheck);
				bDuplication = true;
				continue;
			}

			++itCheck;
		}
	}

	return bDuplication;
}

void Character_NET::AnimationBlendInputForm::SetDiableForm( bool bDisable )
{
	// ����� �ϳ����̴�. ���Ŀ� �߰��� �ʿ��ϸ�... �˾Ƽ�...
	if(bDisable)
	{
		BlendAnimation_textBox->Enabled = false;
		TargetAnimationSelect_button->Enabled = false;
	}
}