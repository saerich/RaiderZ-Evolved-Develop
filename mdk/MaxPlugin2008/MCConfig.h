#pragma once

//////////////////////////////////////////////////////////////////////////
// �ͽ���Ʈ �ɼǵ�

enum DDS_FORMAT {
	DDS_FORMAT_NOCOMPRESS = 0,
	DDS_FORMAT_DXT1,
	DDS_FORMAT_DXT3,
	DDS_FORMAT_DXT5,

	DDS_FORMAT_END
};

const D3DFORMAT D3D_DDS_FORMAT[DDS_FORMAT_END] = { 
	D3DFMT_UNKNOWN, 
	D3DFMT_DXT1, 
	D3DFMT_DXT3,
	D3DFMT_DXT5,
};


class MCConfig {
public:
	BOOL		m_bMeshOut;
	BOOL		m_bStaticMesh;
	int			m_nPolygonPerNodes;
	BOOL		m_bKeyAnimationOut;
	BOOL		m_bVertexAnimationOut;
	BOOL		m_bOptimize_keys;	// �ִϸ��̼� Ű�� ����ȭ �Ұ��ΰ� ����
	// �̰� ���������� Actor�� �����ɶ� ���� effect �� light �� �׻� ���� �����ȴ�

	BOOL		m_bAttachMesh;

	BOOL		m_bUseDDS;			// dds ���
	BOOL		m_bExportTextures;	// �ؽ��� ó��
	DDS_FORMAT	m_fmtPlain;
	DDS_FORMAT	m_fmtAlpha;
	DDS_FORMAT	m_fmtAlphaTestPartsColor;
	string		m_strTextureExportFolder;	/// �ؽ��� �ͽ���Ʈ ����

	bool		ReadConfig(TSTR szFileName);
	void		WriteConfig(TSTR szFileName);

	void		UpdateEluDlgEnableItems(HWND hWnd);

	static int CALLBACK		BrowseCallbackProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK	EluDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
