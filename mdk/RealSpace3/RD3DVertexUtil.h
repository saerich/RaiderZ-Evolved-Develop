#ifndef _RD3DVertexUtil_h
#define _RD3DVertexUtil_h

// max plugins �� ���� ���

#include <list>
#include <set>
#include <string>
#include <unordered_map>

#include <D3D9TYPES.h>

#include "RTypes.h"


namespace rs3 {

#define		MAX_NAME_LEN		40
#define		MAX_PATH_NAME_LEN	256
#define		MAX_ANI_KEY			100
#define		MAX_MESH_NODE_TABLE	300
#define		MAX_PHYSIQUE_KEY	3

#ifndef USING_VERTEX_SHADER
#define USING_VERTEX_SHADER
#endif

using namespace std;

/////////////////////////////////////////////////////
// max plugins ���� ���� ����..

#define EXPORTER_MESH_VER1	0x00000011
#define EXPORTER_MESH_VER2	0x00005001
#define EXPORTER_MESH_VER3	0x00005002
#define EXPORTER_MESH_VER4	0x00005003
#define EXPORTER_MESH_VER5	0x00005004
#define EXPORTER_MESH_VER6	0x00005005
#define EXPORTER_MESH_VER7	0x00005006
#define EXPORTER_MESH_VER8	0x00005007
#define EXPORTER_MESH_VER9	0x00005008
#define EXPORTER_MESH_VER10	0x00005009
#define EXPORTER_MESH_VER11	0x0000500a		// �⺻ Visibility �߰�
#define EXPORTER_MESH_VER12	0x0000500b		// triangle -> polygon ����, trilist -> tristrips
#define EXPORTER_MESH_VER13 0x0000500c		// ��ó�� ���� BoundingBox ������ �߰�
#define EXPORTER_MESH_VER14 0x0000500d		// FVF ������ �߰�
#define EXPORTER_MESH_VER15 0x0000500e		// ActorLightMap Channel & FVF �߰�
#define EXPORTER_MESH_VER16 0x0000500f		// Tangent RVector3 -> RVector4 w ���� Face Parity�� ����Ѵ�.
#define EXPORTER_MESH_VER17 0x00005010		// ����Ʈ�� ����
#define EXPORTER_MESH_VER18 0x00005011		// Extra UV ä�� ���

#define EXPORTER_CURRENT_MESH_VER	EXPORTER_MESH_VER18

#define EXPORTER_ANI_VER1	0x00000012
#define EXPORTER_ANI_VER2	0x00001001
#define EXPORTER_ANI_VER3	0x00001002
#define EXPORTER_ANI_VER4	0x00001003	// ���ϸ��̼��� ȸ������ �̸�����ؼ� �ѱ� : �ε��ӵ� ����
#define EXPORTER_ANI_VER5	0x00001004	// scale key ����, �ʿ���� key ����
#define EXPORTER_ANI_VER6	0x00001005	// �������� ����
#define EXPORTER_ANI_VER7	0x00001006	// vertex animation ���� bounding box ������ ����
#define EXPORTER_ANI_VER8	0x00001007	// animation �� bounding box ������ (0~MaxFrame������ ��ü AABB)
#define EXPORTER_ANI_VER9	0x00001008	// �� ���� ������ visibility���� 0���� ���� ��찡 �����Ƿ� ���� üũ�� �ʿ��ϴ�

#define EXPORTER_CURRENT_ANI_VER	EXPORTER_ANI_VER9

#define EXPORTER_SIG		0x0107f060

typedef struct {
	DWORD	sig;
	DWORD	ver;
	int		mtrl_num;
	int		mesh_num;
} ex_hd_t;

typedef struct {
	DWORD	sig;
	DWORD	ver;
	int		maxframe;
	int		model_num;
	int		ani_type;
} ex_ani_t;

enum RPickType {
	pick_bbox = 0,
	pick_collision_mesh,
	pick_real_mesh,
	pick_end
};


enum RShaderBlendInput {
	VPOSITION,
	WEIGHT2,
	MATRIX_INDEX,
	NORMAL,
	TEXTURE_UV
};

///////////////////////////////////////////////////

enum RFVF_TYPE
{
	RFVF_XYZ,
//	RFVF_XYZRHW,
//	RFVF_XYZW,
	RFVF_XYZB3_LASTBETA_UBYTE4,
	RFVF_XYZB4_LASTBETA_UBYTE4,			// blend weight 5��
	RFVF_NORMAL,
//	RFVF_PSIZE,
	RFVF_DIFFUSE,
//	RFVF_SPECULAR,
	RFVF_TEX1,	// �ؽ��� 1�� Diffuse
	RFVF_TEX2,	// �ؽ��� 2�� Diffuse + Extra
	RFVF_TEX2_TEX2SIZE4, // �ؽ��� 2�� Diffuse + NormalMap(vector4)
	RFVF_TEX3_TEX3SIZE4, // �ؽ��� 3�� Diffuse + Extra + NormalMap(vector4)

	RFVF_NUM
};

struct RFVFPair
{
	DWORD	fvf;
	int		size;
};
static const RFVFPair RFVF[RFVF_NUM] =
{
	{ D3DFVF_XYZ,		sizeof(RVector3) },
//	{ D3DFVF_XYZRHW,	sizeof(RVector4) },
//	{ D3DFVF_XYZW,		sizeof(RVector4) },
	{ D3DFVF_XYZB3 | D3DFVF_LASTBETA_UBYTE4, sizeof(RVector3) }, // RFVF_XYZũ�⸦ ����
	{ D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4, (sizeof(float)*4) }, // RFVF_XYZũ�⸦ ���� blend weight 4�� 
	{ D3DFVF_NORMAL,	sizeof(RVector3) },
//	{ D3DFVF_PSIZE,		sizeof(float) },
	{ D3DFVF_DIFFUSE,	sizeof(DWORD) },
//	{ D3DFVF_SPECULAR,	sizeof(DWORD) },
	{ D3DFVF_TEX1,		sizeof(RVector2) },
	{ D3DFVF_TEX2,		sizeof(RVector2) },
	{ D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE4(1), sizeof(RVector4) }, // Tex0:Diffuse(Vector2):ũ�� ���� / Tex1:NormalMap(Vector4)
	{ D3DFVF_TEX3 | D3DFVF_TEXCOORDSIZE4(2), sizeof(RVector4) } // Tex0:Diffuse(Vector2):ũ�� ���� / Tex1:LightMap(Vector2) / Tex2:NormalMap(Vector4)
};

// ���ؽ� ������ �������� �� ��ҵ��� ��ġ��
struct RVertexOffsetInfo
{
	RVertexOffsetInfo() : size(0), pos(-1), weight(-1), normal(-1), color(-1), texCoord(-1) {}

	int size;		// ���� ���ؽ� ũ��
	int pos;		// Position�� ������
	int weight;		// Blend Weight�� ������
	int normal;		// Normal�� ������
	int color;		// Vertex Color�� ������
	int texCoord;	// Texture Coordinate�� ������
};

struct	RLVertex { 
	RVector p;
	DWORD color;
	FLOAT tu, tv;
};
#define RLVertexType ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

struct	RTestVertex { 
	RVector p;   
	DWORD color; 
};
#define RTestVertexType	( D3DFVF_XYZ | D3DFVF_DIFFUSE )


// new face data type

struct RFaceSubData
{
	WORD p;		// pos		index	
	WORD uv;	// tex uv	index
	WORD uv2;	// extra uv index
	WORD n;		// normal	index
	WORD n_tan;	// tangent	index
	WORD n_bin;	// binormal  index
};

struct RFaceData {
	RFaceSubData index[3];
	short		 mtrl_id;
};

struct RMeshPolygonData {
	int			nVertices;
	RFaceSubData *index;
	short		 mtrl_id;
//	unsigned char *triIndicies;	// �� �������� �ﰢ������ ���� (nVertices-2)*3 ���� 0 base index
};

struct RPhysiqueSubData {
	string		name;
	float		weight;
	WORD		cid;
	WORD		pid;
};

struct RMtrlTableInfo
{
	int		mtrlid;
	WORD	offset;
	WORD	count;

	// 3D�ƽ����� ������ ������ ���� ���׸��� ID�� ���� �� �� �ִ�. �� ���� ������ ��.
	// ���������� �� ���� ������ �޽� �Ϻκ��� �׷��� ���� �Ǵ��ϴ� �뵵�� ����Ѵ�.
	// RActorNode::SetSubMaterialMask(), RActorNode::GetSubMaterialMask() ����.
	int		nSubMaterialIDForDrawMasking;	// version 9���� �߰�
};


#define PHYSIQUE_MAX_WEIGHT		3		// ����Ʈ�� ������ ���� �����ϰ� ������ȭ 
										// ���ؽ� ������ ���� �پ��ִ� ���·� �̸� ������ �� ������ �̻��ϰ� ���̴µ� �̿� ������ ��������... 
struct RPhysiqueInfo {

	RPhysiqueInfo()	{
		m_num = 0;
		m_Data = NULL;
	};

	~RPhysiqueInfo() {
		Destroy();
	}

	void Create(int num) {
		m_num = num;
		if(num)
			m_Data = new RPhysiqueSubData[num];
	}

	void Destroy() {
		SAFE_DELETE_ARRAY(m_Data);
		m_num = 0;
	}

	void SetName(int index,string& name) {	if(m_Data)	m_Data[index].name = name;	}
	void SetWeight(int index,float weight) { if(m_Data) m_Data[index].weight = weight;	}
	void SetParentID(int index,int pid) {	if(m_Data)	m_Data[index].pid = pid; }
	void SetBoneID(int index,int cid) {	if(m_Data)	m_Data[index].cid = cid; }

	char* GetName(int index) {	return (char*)m_Data[index].name.c_str(); }
	float GetWeight(int index) { return m_Data[index].weight; }
	int   GetParentID(int index) { return m_Data[index].pid; }
	int   GetBoneID(int index) { return m_Data[index].cid; }

	RPhysiqueSubData* m_Data;

	int		m_num;
};

#define _ALMOST_ZERO 1.0e-3f

inline BOOL Equal(RVector& p1,RVector& p2)
{
	if (fabs(p1.x - p2.x) > _ALMOST_ZERO)	return FALSE;
	if (fabs(p1.y - p2.y) > _ALMOST_ZERO)	return FALSE;
	if (fabs(p1.z - p2.z) > _ALMOST_ZERO)	return FALSE;

	return TRUE;
}

// table ���� o(nlog(n))�� MCTable.h �� ��ü

//class RIntTable : public vector<int>
//{
//public:
//	RIntTable() {}
//
//	int IsExist(int n) {
//		int nCnt = size();
//		for(int i=0;i<nCnt;i++) {
//			if(at(i)==n)
//				return i;
//		}
//		return -1;
//	}
//
//	int Add(int n) {
//
//		// �������� ���ٸ�..
//		int nPos = IsExist(n);
//
//		if(  nPos != -1 ) // �̹��ִٸ�..
//			return nPos;
//
//		push_back(n);
//
//		return size()-1;
//	}
//};

//class RVectorTable : public vector<RVector>
//{
//public:
//	RVectorTable() {}
//	~RVectorTable() {}
//
//	int IsExist(RVector& v)	{
//		int nCnt = size();
//
//		for(int i=0;i<nCnt;i++) {
//			if(Equal(at(i),v))
//				return i;
//		}
//		return -1;
//	}
//
//	int Add(RVector& v) {
//
//		// �������� ���ٸ�..
//		int nPos = IsExist(v);
//
//		if(  nPos != -1 ) // �̹��ִٸ�..
//			return nPos;
//
//		push_back(v);
//
//		return size()-1;
//	}
//};

struct RVertexIndex
{
	union {
		struct{
			WORD p;
			WORD n;
			WORD uv;
			WORD uv2;	// Extra UV
			WORD n_tan;
			WORD n_bin;
		};
		WORD index[6];
	};
};

//class RVertexIndexTable : public vector<RVertexIndex>
//{
//public:
//	RVertexIndexTable() {}
//	~RVertexIndexTable() {}
//
//	bool isEqual(RVertexIndex& pSrc,RVertexIndex& pDest) {
//		if(pSrc.p == pDest.p)
//			if(pSrc.n == pDest.n)
//				if(pSrc.uv == pDest.uv)
//					return true;
//		return false;
//	}
//
//	int IsExist(RVertexIndex& p) {
//		int nCnt = size();
//
//		for(int i=0;i<nCnt;i++) {
//			if(isEqual(at(i),p))
//				return i;
//		}
//		return -1;
//	}
//
//	int Add(RVertexIndex& p,bool& bExist) {
//
//		// �������� ���ٸ�..
//		int nPos = IsExist(p);
//
//		bExist = false;
//
//		if(  nPos != -1 ) { // �̹��ִٸ�..
//			bExist = true;
//			return nPos;
//		}
//
//		push_back(p);
//
//		return size()-1;
//	}
//};

}

#endif//_RD3DVertexUtil_h
