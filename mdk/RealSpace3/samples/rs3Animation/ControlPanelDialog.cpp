// ontrolPanelDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ControlPanelDialog.h"


// ControlPanelDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(ControlPanelDialog, CDialog)

ControlPanelDialog::ControlPanelDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ControlPanelDialog::IDD, pParent)
{

}

ControlPanelDialog::~ControlPanelDialog()
{
}

void ControlPanelDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ControlPanelDialog, CDialog)
END_MESSAGE_MAP()


// ControlPanelDialog �޽��� ó�����Դϴ�.
