#ifndef _RMesh_h
#define _RMesh_h

#include "RPreDefine.h"
#include <vector>
#include <unordered_map>

#include "RMeshUtil.h"
#include "RMeshAnimationMgr.h"
#include "RResource.h"
#include "ROccluderContext.h"
#include "RLODMetric.h"

namespace rs3 {

////////////////////////////////////////////////////////////////////////////////////////////////////

//#pragma warning(disable : 4996)

class RSceneManager;
class RMeshMgr;
class RActor;

class RMeshNode;
class RActorNode;
class RActorNodePhysique;

// for collision
class RMeshCollision;

// for m_NodeTable / m_nDataNum
class RMaterialResource;

typedef RHashList<RMeshNode*>			RMeshNodeHashList;
typedef RHashList<RMeshNode*>::iterator	RMeshNodeHashList_Iter;

////////////////////////////////////////////////////////////////////////////////////////////////////




class RS_API RMesh : public RResource, public RLODMetric
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// friend
	//
	friend RMeshMgr;			// resource manager

	friend RActor;
	friend RMeshNode;			// m_hVF �迭, m_nFileVersion

	friend RActorNode;			// for m_NodeTable
	friend RActorNodePhysique;	// for m_NodeTable 

public:

	RMesh(const char* _pSzResourceName, RSceneManager* _pSceneManager);
	virtual ~RMesh();

	DECLARE_USING_RES_MGR(RMeshMgr);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// shared resource : vertex format, default material ...
	//
	static bool					InitSharedResources(RDevice *pDevice);
	static bool					DestroySharedResources();


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���ҽ� �޸� ��뷮
	//
	virtual int					GetUsingSystemMemory()	{ return m_nMaterialUsingSystemMemory + m_nVertexUsingSystemMemory; }
	virtual int					GetUsingVideoMemory()	{ return m_nMaterialUsingVideoMemory + m_nVertexUsingVideoMemory; }
	

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// �޽� ��� ����Ʈ
	//
	RMeshNode*					GetNode(const char* name){ return m_NodeHashList.Find(name); }
	RMeshNode*					GetNode(int i) { return m_NodeTable[i]; }
	size_t						GetNodeCount() { return m_NodeTable.size(); }
	void						DelMeshList();


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Animation
	//
	RMeshAnimationMgr*			GetAnimationMgr(void);
	RMeshAnimationMgr			m_AniMgr;											// ��ǰ�����


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// BoundingBox
	//
	bool						GetStaticSkeletonBoundingBox(RBox& box);
	RBoundingBox&				GetStaticBBox(){ return m_staticBBox; }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ĳ���������� �����ϴ� ������
	//
	RMeshCollision*				m_pMeshCollision;							// �浹
	unsigned int				GetMaterialResourceCount();					///< ������ ���� �����ϴ�
	const RMaterialResource*	GetMaterialResource(int nIndex)	;			///< ������ �����ϴ�
	int							GetFileVersion(){ return m_nFileVersion; }	// ���Ϲ���
	void						SetFileVersion(int nVersion){ m_nFileVersion = nVersion; }
	// boolean

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������ ��� �ϱ� ���ؿܺο��� �����͸� ����
	// RMesh�� �����͸� �� �ε� �Ѵٰ� �ؼ� ���������� �۵����� �ʴ´�.
	// �޽������ ������ ���ŵǾ� �ؼ� �޽���带 ������ ���͸� �ٽ� ���� �Ͽ�����.
	// bool						ReFill()		{ return ( Erase() && Fill()); };	// �켱�� RMesh�� ��������� �۵��ϴ� ���̽��� �ȳ���.. - 090608, OZ

public:
	static	RMeshNode*			loadClothMeshNodeELUFile(std::string fileName);	//Fill()�Լ��� �ݵ�� ������ ���¸� ����� �Ѵ�.
																				//Fill()�Լ��� �����ϸ� �� �Լ��� �����ؾ� �Ѵ�.
	bool						IsAlpha2PassRendering() { return m_isAlpha2PassRendering; }
	bool						IsAutoAnimation() { return m_bAutoAnimation; }
	bool						IsLoadedAnimaitonList() { return IsExsitAnimationListFile(); }
	bool						IsExsitAnimationListFile() { return m_bExistAnimationListFile; }
protected:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���ҽ� �ε�
	//
	virtual bool				Fill() override;
	virtual bool				Erase() override;
	virtual LOAD_RESULT			Load() override;

	void						PushNode( RMeshNode* pMeshNode );

	vector<RMaterialResource*>	m_vecMtrlList;								// ����������
protected:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Read File(XML)
	//
	bool						ReadDescXml(const char* fname,bool bBackground);
	bool						ReadUserPropertyElements(MXmlElement* pSceneElement);
	bool						ReadClothElements(MXmlElement* pClothListElement);

private:
	//ELU����
	bool						LoadEluFile(const string& EluFileName);

	//����������Ƽ
	void						FillMeshNodeUserPropertyInfo(void);

	int							m_nFileVersion;
	vector<pair<string, string>> m_vecUserProperty;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// "fill" (io) ����
	//
	void						ConnectMatrix();
	void						CalcStaticBoundingBox();

	RBoundingBox				m_staticBBox;		// �⺻ ���� bounding box

	RMeshNodeHashList			m_NodeHashList;
	vector<RMeshNode*>			m_NodeTable;
	int							m_nDataNum;			// m_NodeTable �޽� ����� ����
	int							m_nID;				// node�� ���̵�


//	RMeshEffectList				m_meshEffectList;
//	vector<RSceneNode*>			m_meshLightList;
	RPickType					m_PickingType;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// "load" (device) ����
	//
	void						MakeAllNodeVertexBuffer();

	static RMaterialResource*	m_pDefaultMaterial;	// �⺻ material
	bool						m_isPhysiqueMesh;
	bool						m_isAlpha2PassRendering; // Alpha2Pass������ ����
	bool						m_bAutoAnimation;
	bool						m_bExistAnimationListFile;	// �ִϸ��̼� ����Ÿ�� ���� ����

	RSceneManager*		m_pSceneManager;


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// �޸� ���
	//
	int							m_nMaterialUsingSystemMemory;
	int							m_nMaterialUsingVideoMemory;
	int							m_nVertexUsingSystemMemory;
	int							m_nVertexUsingVideoMemory;
};

//////////////////////////////////////////////////////////////////////////
inline unsigned int RMesh::GetMaterialResourceCount() { return m_vecMtrlList.size(); }

}



#endif//_RMesh_h
