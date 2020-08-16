#ifndef _RMeshUtil_h
#define _RMeshUtil_h

#include <string>
#include <map>

#include "RDevice.h"
#include "RD3DVertexUtil.h"

namespace rs3 {

//#define MESH_STRIP_TEST

// ���ۻ����� �����ϴ� ��� soft �� ������..���������� �˾Ƽ� ���ư�����...

#define USE_VERTEX_SW 1
#define USE_VERTEX_HW 1<<1

class RS_API RMeshIndexBuffer {
public:
	RMeshIndexBuffer();
	virtual ~RMeshIndexBuffer();

	void Lock();
	void Unlock();

	void Update(int size,WORD* pData);
	bool Create(int size,WORD* pData,DWORD flag=USE_VERTEX_HW|USE_VERTEX_SW,DWORD Usage=RUSAGE_WRITEONLY,D3DPOOL Pool=D3DPOOL_MANAGED);


	int GetFaceCnt();
	int GetSize(){ return m_size; }

	void SetIndices();

public:

	bool	m_bUseSWVertex;	// ���ؽ� ���۰� ��� �Ұ����� ����� ���ΰ�? Ȥ�� ���ؽ� ���ϸ��̼�..
	bool	m_bUseHWVertex;

	DWORD	m_dwUsage;
	D3DPOOL	m_dwPool;
	DWORD	m_dwLockFlag;

	WORD*	m_pIndex;
	WORD*	m_i;

	int m_size;

	HINDEXBUFFER	m_hIB;

	RPRIMITIVETYPE m_PrimitiveType;
};

class RS_API RMeshVertexBuffer {
public:
	RMeshVertexBuffer();
	virtual ~RMeshVertexBuffer();

	void Init();
	void Clear();

	bool Create(void *pVertex, HVERTEXFORMAT hvf, HVERTEXFORMAT hvfOnlyStream0, int nPosOffset, int nNormalOffset, int nVertexSize, int nVertexCnt, DWORD flag, DWORD Usage = D3DUSAGE_WRITEONLY, D3DPOOL pool = D3DPOOL_MANAGED);

	bool Update(char* pVertex,int VertexSize,int VertexCnt);
	bool UpdateData(char* pVertex);		
	bool UpdateDataSW(char* pVertex);
	bool UpdateDataHW(char* pVertex);
	bool UpdateData(RVector* pVec);

	void Lock();
	void Unlock();

	void SetStreamSource();

	void Render();
	void Render(RMeshIndexBuffer* ib );
	void RenderSoft();
	void RenderIndexSoft(WORD* pIndex,int nCount,RPRIMITIVETYPE type);
	void RenderIndexBuffer(int nOffset,int nCount,RPRIMITIVETYPE type);
	void RenderVertexNormal();

	void SetVertexBuffer();

	void ConvertSilhouetteBuffer(float fLineWidth);
	void ReConvertSilhouetteBuffer(float fLineWidth);

	// �޸� ��뷮
	int	GetUsingSystemMemory()	{ return m_nBufferSize; }
	int GetUsingVideoMemory()	{ return m_nBufferSize; }

private:
	void UpdateDataSub( char* pDest, RVector* pVec );

public:

	bool	m_is_init;
	bool	m_bUseSWVertex;	// ���ؽ� ���۰� ��� �Ұ����� ����� ���ΰ�? Ȥ�� ���ؽ� ���ϸ��̼�..
	bool	m_bUseHWVertex;
	char*	m_pVert;
	char*	m_v;

	HVERTEXFORMAT m_hVF;
	HVERTEXFORMAT m_hVFOnlyStream0;

	DWORD	m_dwUsage;
	D3DPOOL	m_dwPool;
	DWORD	m_dwLockFlag;
	int		m_nPosOffset;	// Position ���� ������
	int		m_nNormalOffset;// Normal ���� ������
	int		m_nVertexSize;
	int		m_nVertexCnt;
	int		m_nBufferSize;
	int		m_nRealBufferSize;

	int		m_nRenderCnt;

	RPRIMITIVETYPE m_PrimitiveType;

	HVERTEXBUFFER	m_hVB;
};


//////////////////////////////////////////////////////////////
// help func

class RS_API RBaseObject
{
public:
	RBaseObject() {

	}
	virtual ~RBaseObject() {

	}

public:

	char* GetName();
	void  SetName(const char* name);

	bool  CheckName(const char* name);
	bool  CheckName(string& name);

public:

	string	m_Name;
};

/////////////////////////////////////////////////////////////////////////

//#pragma warning(disable : 4996)

template<class T>
class RHashList : public vector<T>
{
protected:
	map<string,T>	m_HashMap;
public:
	void PushBack(T pNode) {
		push_back(pNode);
		m_HashMap.insert(map<string,T>::value_type(string(pNode->GetName()),pNode));
	}

	void PushBack(const string& strName, T pNode) {
		push_back(pNode);
		m_HashMap.insert(map<string,T>::value_type(strName,pNode));
	}

	void Clear() {
		m_HashMap.clear();
		clear();
	}

	void EraseAll()
	{
		while(!empty())
		{
			iterator it = begin();
			delete (*it);
			erase(it);
		}

		while(!m_HashMap.empty())
		{
			map<string,T>::iterator hash_map_itor = m_HashMap.begin();

			if (hash_map_itor != m_HashMap.end()) {
				m_HashMap.erase(hash_map_itor);
			}
		}
	}

	void Delete(const char* name)
	{
		map<string,T>::iterator itor = m_HashMap.find( string(name) );

		if (itor == m_HashMap.end()) return;

		T toDelete = itor->second;

		delete toDelete;

		iterator new_end = remove(begin(), end(), toDelete);
		erase (new_end, end( ) );

		m_HashMap.erase(itor);
	}

	/*
	iterator Erase(iterator where) {

		iterator itor = erase(where);

		if (itor != end()) {

			map<string,T>::iterator hash_map_itor = m_HashMap.find( string( (*itor)->GetName() ) );

			if (hash_map_itor != m_HashMap.end()) {
				m_HashMap.erase(hash_map_itor);
			}
		}
		return itor;
	}

	T Find(int id) {
		return NULL;//�����߻��ϵ���. id�� ã�� ���� ������ ���� ����.
	}
	*/

	T Find(const char *name) {

		map<string,T>::iterator itor = m_HashMap.find( string(name) );

		if (itor != m_HashMap.end()) {
			return (*itor).second;
		}
		return NULL;
	}
};

void SubCalcBBox(RVector* max,RVector* min,RVector* v);


}

#endif//_RMeshUtil_h
