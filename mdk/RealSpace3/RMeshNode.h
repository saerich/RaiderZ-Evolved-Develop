#ifndef _RMeshNode_h
#define _RMeshNode_h

#include "RPreDefine.h"
#include "RMeshNodeData.h"
#include "RSkeleton.h"
#include "RMaterial.h"
#include "MMemPool.h"

namespace rs3 {




class RActorNodeInstancingBuffer
{
#define MAX_MESH_INSTANCE	20		// instancing.rsc�� �Ͱ� ����3~

private:
	HVERTEXBUFFER			m_hInstancingBuffer;
	RDevice*				m_pDevice;

public:
	RActorNodeInstancingBuffer() :
	  m_hInstancingBuffer(R_NONE), m_pDevice(NULL)
	  {
	  }
	  ~RActorNodeInstancingBuffer()
	  {
		  Destroy();
	  }

public:
	void	Destroy()
	{
		if( m_hInstancingBuffer != R_NONE && m_pDevice != NULL)
		{
			m_pDevice->DeleteVertexBuffer(m_hInstancingBuffer);
			m_hInstancingBuffer = R_NONE;
			m_pDevice = NULL;
		}
	}

	void	Create( RDevice* pDevice)
	{
		m_pDevice = pDevice;
		if(!m_hInstancingBuffer)
		{
			int nInstancingCnt = MAX_MESH_INSTANCE;
			int nStructSize = sizeof(float);	// �ε������� ����.
			int bufferSize = nInstancingCnt * nStructSize;
			// ������ ä��. ������ �������� �ε����� ������ ��.
			float* pData = new float[ nInstancingCnt];
			for( int i=0; i<nInstancingCnt; i++)
				pData[i] = (float)i;
			// ������ �ε����θ� �ѹ��� ä���ְ� �� �ķδ� �ٲ� ���� �����Ƿ� ���̳������� ���� �ʴ´�.
			m_hInstancingBuffer = m_pDevice->CreateVertexBuffer( bufferSize, nStructSize, pData);
			// ������ ������,
			SAFE_DELETE_ARRAY( pData);
		}
	}

	HVERTEXBUFFER	GetBuffer()					{ return m_hInstancingBuffer;	}
	int				GetMaxSize()				{ return MAX_MESH_INSTANCE;	}
};

/// LOD ������ �����ϴ� Class
class RS_API RMeshLODNode
{
public:
	RMeshLODNode() : m_nLODLevel(0)	{}
	virtual ~RMeshLODNode()			{}

private:
	int	m_nLODLevel;

public:
	void	SetLODLevel( int nLv)	{ m_nLODLevel = nLv;	}
	int		GetLODLevel()			{ return m_nLODLevel;	}
};	



enum RMESH_ALIGN : int {
	RMA_NONE = 0,
	RMA_NORMAL = 1,
	RMA_Z_FIXED = 2
};

//////////////////////////////////////////////////////////
// ��尡 ������ �Ӽ��� �����ϴ� Class

class RS_API RMeshNodeInfo
{
public:
	RMeshNodeInfo();
	virtual ~RMeshNodeInfo();

	bool	isVisibleMesh()		{ return !GetFlag(RM_FLAG_HIDE); }
	bool	isCollisionMesh()	{ return GetFlag(RM_FLAG_COLLISION_MESH); }
	bool	isDummyMesh()		{ return GetFlag(RM_FLAG_DUMMY_MESH); }
	bool	isClothMesh()		{ return GetFlag(RM_FLAG_CLOTH_MESH); }
	bool	isBone()			{ return GetFlag(RM_FLAG_BONE_MESH); }

	void	SetFlag(DWORD dwFlag) { m_dwFlag = dwFlag; }
	bool	GetFlag(DWORD dwFlag) { return (m_dwFlag&dwFlag) ? true:false; }
	void	AddFlag(DWORD dwFlag) { m_dwFlag |= dwFlag; }

public:

	DWORD					m_dwFlag;				
	RMESH_ALIGN				m_nAlign;				// Align Type

	//////////////////////////////////////////////////////////////////////////
	// CLOTH // TODO
	//////////////////////////////////////////////////////////////////////////
	int						m_nClothWidth;			// �ʰ� ũ��
	int						m_nClothHeight;			//
	int						m_nSeam[4];				// Seam ���� Vertex�� Index // Max�� �ε��� -1 �� ��ȯ�Ұ�!

	float					cCloth_fGravity;		// �߷�
	bool					cCloth_bShear;			// �밢�� ���
	float					cCloth_fRelax;			// ������
	float					cCloth_fWindForce;		// �ٶ�����
	float					cCloth_fWindHead;		// �ٶ��� ����(Y�� Degree)
	float					cCloth_fWindAmp;		// �ٶ��� ����(Noise Texture * Amp)

	void					SetWidth(int nWidth){ m_nClothWidth = nWidth;}
	void					SetHeight(int nHeight){ m_nClothHeight = nHeight;}
	int						GetWidth(){ return m_nClothWidth;}
	int						GetHeight(){ return m_nClothHeight;}

};

////////////////////////////////////////////////////////////////////////
// ����� ���� ������ ����Ѵ�. ( �ε��� ���ۿ� , ���ؽ� ���� )
// TODO: ������ RMeshNode �� ����
class RS_API RBatch
{
public:
	RBatch();
	virtual ~RBatch();

	bool CreateVertexBuffer(char* pVert, const RVertexOffsetInfo& info, HVERTEXFORMAT hvf, HVERTEXFORMAT hvfOnlyStream0, int vert_num, DWORD flag);
	
	bool UpdateVertexBuffer(char* pVert);
	bool UpdateVertexBufferSoft(char* pVert);

	bool CreateIndexBuffer(WORD* pIndex,int _size);

	// Vertex ���� ���ϱ�
	inline void SetVertexStream()
	{
		m_pVb->SetVertexBuffer();
		m_pIb->SetIndices();
	}

	int GetUsingSystemMemory();
	int GetUsingVideoMemory();

public:

	RMeshIndexBuffer*		m_pIb;
	RMeshVertexBuffer*		m_pVb;

#ifdef MESH_STRIP_TEST
	RMeshIndexBuffer*		m_pIbDefault;
	RMeshIndexBuffer*		m_pIbOptimized;
	RMeshIndexBuffer*		m_pIbStrips;
#endif
	bool				m_bIsMakeVertexBuffer;

	static bool			m_pUseHWShader;
	static bool			m_bisToolApp;
	static RVector3		m_vToolLightPos;
};
//----------------------------------------------------------------------------------------------------
inline int RBatch::GetUsingSystemMemory()
{
	int nSize = 0;
	if (m_pIb)
		nSize += m_pIb->GetSize();

	if (m_pVb)
		nSize += m_pVb->GetUsingSystemMemory();

	return nSize;
}
//----------------------------------------------------------------------------------------------------
inline int RBatch::GetUsingVideoMemory()
{
	int nSize = 0;
	if (m_pIb)
		nSize += m_pIb->GetSize();

	if (m_pVb)
		nSize += m_pVb->GetUsingVideoMemory();

	return nSize;
}
//----------------------------------------------------------------------------------------------------

typedef std::map< string, HVERTEXFORMAT > MAP_VERTEX_FORMAT;

class RS_API RMeshNode :public RMeshNodeData , public RMeshNodeMtrl, public RMeshNodeInfo , public RBatch, public RMeshLODNode
{
	friend RMesh;

public:
	RMeshNode();
	virtual ~RMeshNode();

	bool MakeNodeBuffer(DWORD flag);					// ���ؽ� ������ ����.

private:
	bool ConnectMtrl();									// ������ ������ ������ ����.

	void MakeVertexInfo(RVertexOffsetInfo& info, HVERTEXFORMAT* pVF, HVERTEXFORMAT* pVFOnlyStream0);

	static MAP_VERTEX_FORMAT	m_mapVertexFormat;

public:
	bool isRenderAble();								// �׷������ϴ� ����ΰ�?
	bool isPhysique();									// physique �޽��ΰ� ?
	bool isHWPhysique();								// �ϵ���� physique �ΰ�..
	bool hasAlphaMaterial();							// ���� (additive �� blending) material�� �����ϴ°�
	bool hasAdditiveMaterial();							// additive material �� �����ϴ°�
	bool hasTranslucentMaterial();						// ������ material �� �����ϴ°�

	bool CheckMaterialFlag(DWORD dwFlag);				//�ش� flag�� ������ �ִ� ���׸����� �ִ��� �˻�

	void UpdateNodeBuffer();							// ���ؽ� ������ ����.

	///////////////////////////////////////////
	// reference pointer

	RMesh*			m_pParentMesh;						// �ڽ��� �θ� �� ����...
	RMesh*			m_pBaseMesh;						// ������ �⺻ �� ����...
	RMeshNode*		m_pParent;							// �θ� ���..

	RMatrix			m_matBase;		// local-> max���� world���� ��ȯ��� (�θ��� ��ȯ ����)
	RMatrix			m_matInvBase;		// matBase �� inverse

	RBIPID			GetBipId();

	int				GetUserPropertyCount();
	const string&	GetUserPropertyString(int nIndex);
	bool			FindUserProperty(const std::string& value);

public:
	RBIPID			m_bipID;

protected:
	vector<string>	m_userPropertyString;
};

/////////////////////////////////////////////////
inline RBIPID RMeshNode::GetBipId() { return m_bipID; }
inline bool RMeshNode::isPhysique() { return (m_nPhysiqueCnt>0);}
inline bool RMeshNode::isHWPhysique() { return ( isPhysique() && RBatch::m_pUseHWShader); }
inline bool RMeshNode::isRenderAble() { return ( !isDummyMesh() || (m_nFaceCnt>0)); }

inline int	RMeshNode::GetUserPropertyCount()	{ return (int)m_userPropertyString.size(); }
inline const string& RMeshNode::GetUserPropertyString(int nIndex) { return m_userPropertyString[nIndex]; }
inline bool RMeshNode::FindUserProperty(const std::string& value)
{
	for ( vector<string>::iterator itr = m_userPropertyString.begin(); itr != m_userPropertyString.end(); ++itr )
	{
		if ( *itr == value)
			return true;
	}

	return false;
}


class RS_API RMeshNodeMLeaf : public RMeshNode, public MMemPool<RMeshNodeMLeaf>
{
	friend RMesh;
public:
	RMeshNodeMLeaf() : RMeshNode() {}
	virtual ~RMeshNodeMLeaf() {}
};

}

#endif//_RMeshNode_h
