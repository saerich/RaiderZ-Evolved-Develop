#include "stdafx.h"

#include "MtrlEditDlg.h"

using namespace rs3;

int g_sel = 0;

IMPLEMENT_DYNAMIC(CMtrlEditDlg, CDialog)
CMtrlEditDlg::CMtrlEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMtrlEditDlg::IDD, pParent)
	, m_texture_name(_T(""))
	, m_alpha_texture_name(_T(""))
	, m_nAlphaRefValue(0)
{
}

CMtrlEditDlg::~CMtrlEditDlg()
{
}

void CMtrlEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MTRLLIST, m_mtrl_list_box);
	DDX_Text(pDX, IDC_TEX_NAME, m_texture_name);
	DDX_Text(pDX, IDC_ALPHA_TEX_NAME, m_alpha_texture_name);
	DDX_Control(pDX, IDOKSAVE, m_SaveButton);
	DDX_Text(pDX, IDC_ALPHA_REF, m_nAlphaRefValue);
	DDV_MinMaxByte(pDX, m_nAlphaRefValue, 0, 255);
}


BEGIN_MESSAGE_MAP(CMtrlEditDlg, CDialog)
	ON_BN_CLICKED(IDOKSAVE, OnBnClickedOksave)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_MTRLLIST, OnLbnSelchangeMtrllist)
END_MESSAGE_MAP()

void CMtrlEditDlg::ClearListBox()
{
	int cnt = m_mtrl_list_box.GetCount();

	// clear listbox

	for(int i=0;i<cnt;i++) {
		m_mtrl_list_box.DeleteString(0);
	}
}

// save �Ѵ�..

void CMtrlEditDlg::OnBnClickedOksave()
{

	GetMainView()->ToggleMrtlEditDlg();
	
	// model ������ ��� �պκ� mtrl �� �����...

/*
	if(GetMainView()->GetLastOpenEluFile()[0]) {

		// �������� ������ ��� ����... ���߿� �����...

		//<---------------------------------------------------

		ex_hd_t t_hd;
		
		FILE* fp = fopen(GetMainView()->GetLastOpenEluFile(), "rb");

		if(fp==NULL) {
			::MessageBox(NULL,"������ Ȯ���ϼ���.","�˸�",MB_OK);
			return;
		}

		fread(&t_hd,sizeof(ex_hd_t),1,fp);

		fclose(fp);

		//<----------------------------------------------------

		fp = fopen(GetMainView()->GetLastOpenEluFile(), "rb+");

		if(fp==NULL) {
			::MessageBox(NULL,"������ �б� �������� Ȯ���ϼ���.","�˸�",MB_OK);
			return;
		}

		fseek(fp, 16, SEEK_SET);//��� skip

		RMtrlMgr* pMtrlMgr = GetMtrlMgr();

		int mtrl_cnt = pMtrlMgr->size();
		
		RMtrl* pMtrl = NULL;

		int nSize = 0;

		for(int i=0;i<mtrl_cnt;i++) {

			pMtrl = pMtrlMgr->m_NodeTable[i];
			
			if(pMtrl == NULL) continue;

			// ���� �������� ���� �ϳ��� ���

//			fwrite(&pMtrl->m_mtrl_id    ,4,1,fp);
//			fwrite(&pMtrl->m_sub_mtrl_id,4,1,fp);

			fwrite(&pMtrl->m_ambient ,sizeof(D3DXCOLOR),1,fp);
			fwrite(&pMtrl->m_diffuse ,sizeof(D3DXCOLOR),1,fp);
			fwrite(&pMtrl->m_specular,sizeof(D3DXCOLOR),1,fp);

			fwrite(&pMtrl->m_power,4,1,fp);

//			fwrite(&pMtrl->m_sub_mtrl_num,4,1,fp);

			nSize = pMtrl->m_Name.size();
			fwrite(&nSize,4,1,fp);
			fwrite(&pMtrl->m_Name[0],nSize,1,fp);

			nSize = pMtrl->m_TextureName.size();
			fwrite(&nSize,4,1,fp);
			fwrite(&pMtrl->m_TextureName[0],nSize,1,fp);

			nSize = pMtrl->m_OpaTextureName.size();
			fwrite(&nSize,4,1,fp);
			fwrite(&pMtrl->m_OpaTextureName[0],nSize,1,fp);

			int nIn = 0;

			if(pMtrl->m_bTwoSided)	nIn = 1;
			else					nIn = 0;

			fwrite(&nIn,sizeof(int),1,fp);

			if(pMtrl->m_bAdditive)	nIn = 1;
			else					nIn = 0;

			fwrite(&nIn,sizeof(int),1,fp);

			if(t_hd.ver > EXPORTER_MESH_VER7) {

				fwrite(&pMtrl->m_nAlphaTestValue,sizeof(int),1,fp);
			}
			
		}
		
		fclose(fp);
	}
	else {
		::MessageBox(NULL,"xml ������ �������� �ʽ��ϴ�.","�˸�",MB_OK);
	}

	g_sel = 0;

	::MessageBox(NULL,"����Ϸ�","�˸�",MB_OK);

//	OnOK();
*/
}

void CMtrlEditDlg::OnBnClickedCancel()
{
	GetMainView()->ToggleMrtlEditDlg();

	g_sel = 0;

	OnCancel();
}

void CMtrlEditDlg::Begin()
{
	// ������ ������ �ʱ�ȭ

	ClearListBox();

/*
	RActor* pActor = GetMainView()->GetActorMgr()->GetFast(0);

	if(pActor) {

		if(pActor->m_pMesh) {

			RMtrlMgr* pMtrlMgr = &pActor->m_pMesh->m_MtrlMgr;
			int mtrl_size = pMtrlMgr->size();

			static char temp[256];

			RMtrl* pMtrl = NULL;

			for(int i=0;i<mtrl_size;i++) {

				pMtrl = pMtrlMgr->m_NodeTable[i];

				if(pMtrl) {
					if( !pMtrl->m_TextureName.empty() ){
						sprintf(temp,"%s",pMtrl->GetTextureName());
					}
					else {
						sprintf(temp,"mtrl %d",i);
					}

					m_mtrl_list_box.AddString( temp );
				}
			}
		}
	}
*/
}

void CMtrlEditDlg::End()
{
	
}

void CMtrlEditDlg::OnLbnSelchangeMtrllist()
{
}
