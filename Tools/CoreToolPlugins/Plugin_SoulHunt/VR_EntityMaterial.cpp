// V_EditEntityMaterial.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "VR_EntityMaterial.h"


// CRollupEntityMaterial ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CRollupEntityMaterial, CDialog)

CRollupEntityMaterial::CRollupEntityMaterial(CWnd* pParent /*=NULL*/)
	: CDialog(CRollupEntityMaterial::IDD, pParent)
{

}

CRollupEntityMaterial::~CRollupEntityMaterial()
{
}

void CRollupEntityMaterial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRollupEntityMaterial, CDialog)
END_MESSAGE_MAP()


// CRollupEntityMaterial �޽��� ó�����Դϴ�.

BOOL CRollupEntityMaterial::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}

void CRollupEntityMaterial::Listen(CoreMessage& _message)
{

}