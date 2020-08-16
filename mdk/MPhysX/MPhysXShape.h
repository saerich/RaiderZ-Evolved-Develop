#pragma once

#include "MPhysXDef.h"

namespace physx {

class MPhysX;

//////////////////////////////////////////////////////////////////////////
//
// MPhysXShape
//  - Named / Referece-counting Resource
//  - MPhysX���� ���� �� ����
//
//////////////////////////////////////////////////////////////////////////

class MPhysXShape
{
public:

	//////////////////////////////////////////////////////////////////////////
	// Shape Ÿ�� (PSHT : PhysX Shape Type)
	//
	enum PHYSX_SHAPE_TYPE
	{
		PSHT_GENERAL = 0,

		PSHT_CONVEX,
		PSHT_STATIC_MESH,

		PSHT_NUM
	};

	bool				CreateStaticMeshShape( std::vector<NxVec3>& _refVertices, std::vector<NxU32>& _refFaces, bool bHeightField );
	bool				CreateConvexShape( std::vector<NxVec3>& _refVertices, std::vector<NxU32>& _refFaces);
	bool				CreateSphereCCD(const MSphere &sphere, const int nRings = 20, const int nSegments = 20);

	NxShapeDesc*		GetNxShapeDesc(){ return m_pShapeDesc; }
	static void			MakeUniqueName(std::string& _strOut){ char text[256]; sprintf_s(text, "unnamed_%d", m_nUnNamedShapeCnt++); _strOut = text; }

private:

	friend class MPhysX;

	MPhysXShape(PHYSX_SHAPE_TYPE _eType, const std::string& _strShapeName, MPhysX* _pPhysX)
	{
		m_eShapeType = _eType;
		m_strShapeName = _strShapeName;
		m_pPhysX = _pPhysX;

		m_pShapeDesc = NULL;

		InitShapeInfo();
	}

	MPhysXShape(NxShapeDesc* _pShapeDesc, MPhysX* _pPhysX)
	{
		m_pShapeDesc = _pShapeDesc;
		m_pPhysX = _pPhysX;

		m_eShapeType = PSHT_GENERAL;
		MakeUniqueName(m_strShapeName);

		InitShapeInfo();
	}

	void InitShapeInfo()
	{
		m_nRefCnt = 1;
		m_pCCDVB = NULL;
		m_pCCDIB = NULL;
	}

	virtual ~MPhysXShape();

	NxShapeDesc*		m_pShapeDesc;				///< �ʰ��� ������ Convex Shape

	// CCD
	NxVec3*				m_pCCDVB;					///< ���� ����
	NxU32*				m_pCCDIB;					///< �ε��� ����

	PHYSX_SHAPE_TYPE	m_eShapeType;
	int					m_nRefCnt;
	std::string			m_strShapeName;
	MPhysX*				m_pPhysX;

	static unsigned int	m_nUnNamedShapeCnt;
};

} // namespace physx
