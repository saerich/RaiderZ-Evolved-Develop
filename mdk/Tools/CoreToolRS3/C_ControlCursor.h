/// @file C_ControlCursor.h
#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <RDeviceD3D.h>
#include "RTerrain.h"
#include "M_ToolSceneManager.h"
#include "MXml.h"

namespace rs3
{
	class RMesh;
	class RActor;
};

class CControlMediator;

/**
	@brief Ŀ�� ���� Ŭ����

	WorkBench ���� ������ Ŀ���� �����ϰ� �����ִ� �̱��� Ŭ���� �Դϴ�.
*/
class CORETOOL_API L3DCursor
{
public:
	enum TYPE
	{
		SPHERE = 0,		// ��
		BOX,			// ����
		CYLINDER,		// �Ǹ���
		RCONE,			// �Ųٷε� ����
		DX_MESH_NUM,	// ������� �޽�(D3DXMESH)
		AXIS,			// ������
		BOX2D,			// �巡�� �� ǥ�õǴ� ����(��)
		NONE,			// ����
		NODE,			// RSceneNode ��ü
	};
protected:
	L3DCursor();
	~L3DCursor();

	LPD3DXMESH m_pCursorObject[DX_MESH_NUM];
	LPD3DXMESH m_pMesh;

	bool m_bVolumeType;
	D3DXMATRIX matWorld;
	rs3::RVector m_vPos;
	rs3::RVector m_vScale;

	rs3::RToolSceneManager *m_pSceneManager;
	rs3::RSceneNode *m_pCursorNode;

	TYPE m_eType;
	rs3::RDeviceD3D *m_pd3dDevice;
	CControlMediator* m_pControlMediator;

	DWORD m_dwColor;

	void RenderBandMesh(rs3::RDeviceD3D *pd3dDevice);
public:
	void Init(CControlMediator* pControlMediator, rs3::RDeviceD3D *pd3dDevice, rs3::RToolSceneManager *pSceneManager);
	void Release();

	void SetMeshNode(rs3::RSceneNode *pNode);
	void SetTransform(const rs3::RVector &pos, const rs3::RVector &scale);

	void SetType(TYPE eType, bool bVolume);
	TYPE GetType(){ return m_eType; }
	void Render(rs3::RDeviceD3D *pd3dDevice);

	void ClearCursorNode();
	void SetNode(rs3::RSceneNode *pSceneNode);
	void SetNodeFile(const char *szFilename, const char *szNodename);
	rs3::RSceneNode* GetNode() { return m_pCursorNode; }

	rs3::RVector* GetPos()	{	return &m_vPos;		}
	rs3::RVector* GetScale(){	return &m_vScale;	}
	float GetNodeHeight();
	rs3::RVector GetNodeHalfRange();

	rs3::RDeviceD3D *GetDevice()	{	return m_pd3dDevice; }
	void SetColor(DWORD color);

	static L3DCursor* GetInstance()
	{
		static L3DCursor kCursor;
		return &kCursor;
	}
};
