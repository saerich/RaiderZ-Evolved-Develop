// AniBlendDurationDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AniBlendDurationDlg.h"


// AniBlendDurationDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(AniBlendDurationDlg, CDialog)

AniBlendDurationDlg::AniBlendDurationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(AniBlendDurationDlg::IDD, pParent)
	, m_editDuration(_T(""))
{

	EnableAutomation();

}

AniBlendDurationDlg::~AniBlendDurationDlg()
{
}

void AniBlendDurationDlg::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ�
	// OnFinalRelease�� ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void AniBlendDurationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ANIBLEND_DURATION, m_editDuration);
}


BEGIN_MESSAGE_MAP(AniBlendDurationDlg, CDialog)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(AniBlendDurationDlg, CDialog)
END_DISPATCH_MAP()

// ����: IID_IAniBlendDurationDlg�� ���� ������ �߰��Ͽ�
//  VBA���� ���� ���� ���ε��� �����մϴ�. 
//  �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {90B0A9E5-67FA-4356-A2CC-0E69FD563DE7}
static const IID IID_IAniBlendDurationDlg =
{ 0x90B0A9E5, 0x67FA, 0x4356, { 0xA2, 0xCC, 0xE, 0x69, 0xFD, 0x56, 0x3D, 0xE7 } };

BEGIN_INTERFACE_MAP(AniBlendDurationDlg, CDialog)
	INTERFACE_PART(AniBlendDurationDlg, IID_IAniBlendDurationDlg, Dispatch)
END_INTERFACE_MAP()


// AniBlendDurationDlg �޽��� ó�����Դϴ�.
