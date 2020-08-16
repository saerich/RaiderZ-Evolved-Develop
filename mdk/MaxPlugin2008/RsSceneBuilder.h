#ifndef _RSBSPEXPORTER_H
#define _RSBSPEXPORTER_H

#pragma warning( disable : 4002 )

#include <windows.h>
#include <stdio.h>
#include <math.h>

#include "max.h"
#include "export.h"

#include "cmlist.h"

#include "RTypes.h"
#include "RMaxMaterial.h"
#include "RsBuilder.h"

#include <list>
#include <vector>
#include <string>
#include <map>

//using namespace std;
using namespace rs3;

typedef CMLinkedList<rpolygon> RSPolygonList;
typedef list<INode*> NODELIST;

class RCell;

/// �� �� ������ ��
class RPortal
{
public:
	string		m_strName;
	int			nCellID1;
	int			nCellID2;

	int			nVertices;
	Point3		*m_pVertices;

	// ���� cellid �� ���� �����صδ°�
	string		m_strParentCellName;
	string		m_strTargetCellName;

	// �θ� Cell�� �����ϱ� ����
	RCell*		m_pParentCell;
};

class RGeometry
{
public:
	DWORD			m_dwFVF;
	RPolygonList	m_sourcePolygons;

	typedef pair<int,int>					MIDLIDPAIR;	// materialID ,lightmapID pair 
	typedef multimap< MIDLIDPAIR , RsBuilder*>	IDBUILDERMAP;
	IDBUILDERMAP	m_trees;

	RGeometry();
	~RGeometry();
	RsBuilder* GetTree( MIDLIDPAIR idPair);
};

struct RCellFace
{
	RCellFace() : nVerts(0), pVerts(NULL) {}
	~RCellFace() { SAFE_DELETE_ARRAY( pVerts ); }

	int			nVerts;
	RVector*	pVerts;
};

/// cell �� scene�� ���ҵ� �������� max���� ������(���)���� �����Ѵ�
class RCell
{
public:
	string				m_strName;
	Box3				m_bb;
	list<RPlane>		m_planes;			// ��踦 �̷�� ����
	vector<RCellFace*>	m_cellFaces;

	typedef map<DWORD, RGeometry*>	IDGEOMETRYMAP;
	IDGEOMETRYMAP		m_mapGeometryList;
	
	~RCell();
	RGeometry* GetGeometry(DWORD idFVF);
	bool Contains(const RVector &v) 
	{ 
		for( list<RPlane>::iterator i = m_planes.begin(); i!=m_planes.end(); i++)
		{
            if((*i).GetDistanceTo(v)>=0)
				return false;
		}
		return true; // ��� ����� ���ʿ� ������ �����̴�
	}

	bool IsAlign( int nVertices, const Point3* pVertices )
	{
		for( list<RPlane>::iterator i = m_planes.begin(); i!=m_planes.end(); i++)
		{
			bool bAlign = true;
			for( int j = 0; j < nVertices; j++)
			{
				const Point3& pV = pVertices[j];
				float dist = (*i).GetDistanceTo( RVector(pV.x, pV.y, pV.z) );
				if( !MMath::Equals( 0.0f, dist, 0.1f ) )
				{
					bAlign = false;
					break;
				}
			}

			if( bAlign )
				return true;
		}

		return false;
	}
};

typedef list<RPortal*> RPortalList;

class RCellList : public list<RCell*> 
{
public:
	pair<int, RCell*> Find(const char *szName) {
		int nIndex = 0;
		for( iterator i = begin(); i!=end(); i++,nIndex++) {
			if(strcmp(szName,(*i)->m_strName.c_str())==0)
				return pair<int, RCell*>(nIndex, *i);
		}
		return pair<int, RCell*>(-1, NULL);
	}
};

class RsSceneBuilder  
{
public:
	RsSceneBuilder();
	virtual ~RsSceneBuilder();

	Interface*	ip;

	RVector			m_AmbientLightColor;	
	NODELIST		m_ObjectsList;

	char			MapName[256];
	char			MaxFileName[256];

	RSPolygonList	faceNavigation;
	
//	RPolygonList	m_sourcePolygons;	// �ͽ����Ϳ��� �̸��� �������� �����Ѵ�
	RCellList		m_cells;			// sector : ���ҵ� ����
	RPortalList		m_portals;			// portal : ������ �����ϴ� ��

	bool Build(Interface *ip, RMaxMaterialKeeper* pMaterials, int nPolygonPerNodes);
	bool SaveRS(const char *fileName);
	void SaveRS_Tree(RGeometry* pGeometry, FILE *file);
	void AddCell(RCell*);
	void Destroy();

	bool IsEmpty();				///< ����ֳ�

private:
	void RelocatePolygons();	// �������� ������ ���� ��ġ�Ѵ�
	void ConnectPortals();		// ��Ż�� ���� �����Ѵ�
	void ValidatePortals();		// ��Ż ������ ����

	RVector ViewPoint;
	MPoint	m_lightMapSize;
};

#endif // !defined(AFX_RsSceneBuilder_H__A7395ED0_81F3_11D2_9B5F_00AA006E4A4E__INCLUDED_)
