// ChildView.h : CChildView Ŭ������ �������̽�
//


#pragma once

#include "RDevice.h"
#include "RDeviceD3D.h"
#include "d3exutil.h"
#include "toolCamera.h"
#include "RTerrain.h"
#include "RSceneManager.h"
#include "RTreeSystem.h"
#include "RTreeInstanceSceneNode.h"

// CChildView â

class CChildView : public CWnd
{
// ����
public:
	CChildView();

// Ư��
public:

// �۾�
public:
	//static LPDIRECT3D9 m_lpD3D9;
	//static LPDIRECT3DDEVICE9 m_lpd3dDevice;
	//D3DPRESENT_PARAMETERS m_kd3dpp;
	static rs3::RDeviceD3D *m_pdeviced3d;

	L3DCamera *m_pCamera;
	LPD3DXFONT m_pFont;

	rs3::RTerrain *m_pTerrain;
	rs3::RSceneManager *m_pSceneManager;
	rs3::RTreeSystem *m_pTreeSystem;
	rs3::RSceneNodeList m_kNodeList;
	
	LPD3DXMESH m_pMesh;
	DWORD m_dwNumSubset;

	rs3::RVector m_vBoxPosition;
	//

//	DirectX ������ ����
	void InitState();
	void RenderState();
	void RenderScene();

// ������
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
public:
	virtual ~CChildView();

	// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

