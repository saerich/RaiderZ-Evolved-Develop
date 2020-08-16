#pragma once

#include "MPhysXObject.h"
#include "MPxObjStaticSubActorBuilder.h"

namespace physx {

//////////////////////////////////////////////////////////////////////////
//
// MPxObjStatic
//  - Static Object (������ SubActors�� ������ ������Ʈ)
//  - �ѹ� ��������� �̵����� �ʴ´�.
//  - �� ƽ���� ActivateSubActor�� MBox ������ ������ �������־�� �Ѵ�.
//    (������ ������ Ȱ��ȭ �ϴ� ������ �ø��̴�)
//
//////////////////////////////////////////////////////////////////////////

class MPxObjStatic : public MPhysXObject
{

public:

	MPxObjStatic();
	virtual ~MPxObjStatic();


	//////////////////////////////////////////////////////////////////////////
	// Static SubActor �߰��ϱ�
	//
	bool AddStaticSubActorsFromBuilder(MPxObjStaticSubActorBuilder& _rStaticSubActorBuilder);


	//////////////////////////////////////////////////////////////////////////
	// Static SubActor Ȱ��ȭ �ϱ�
	//
	void ActivateStaticSubActorsByAreaList(const std::vector< const MBox* >& _rAreaList);
	void ActivateStaticSubActorsAll();


	//////////////////////////////////////////////////////////////////////////
	//
	// ������ �ʴ� Virtual �Լ� ó��
	//
	//////////////////////////////////////////////////////////////////////////

	virtual void NotifyResultOnSimulated()
	{
		_ASSERT(0);	// Rigid Static Object�� �� ����� ������� �����Ƿ� �Ҹ��� �ȵȴ�.
	}

	virtual const MBox* GetAreaBox()
	{
		_ASSERT(0); // Rigid Static Object�� �� ����� ������� �����Ƿ� �Ҹ��� �ȵȴ�.
		return NULL;
	}


protected:

	virtual void OnDoPreUpdateSubActorTransform() { _ASSERT(0); } //�Ⱥҷ�����
	virtual void OnDoPostUpdateSubActorTransform(){ _ASSERT(0); } //�Ⱥҷ�����


private:

	//////////////////////////////////////////////////////////////////////////
	// Sector�� ������ ������Ʈ�� ���� ����
	//  - ������Ʈ�� �ʹ� ũ�� �ӵ��� ���� ���ͷ� �ɰ���.
	//  - �׸��� / ť�긦 �����ؼ� ����Ҽ� �ִ�.
	//
	struct SECTOR_INFO
	{
		SECTOR_INFO()
			: m_NXInfo(PS_STATIC_TRIANGLE, MMatrix::IDENTITY, 0.0f, NULL)
			, m_nRefCount(0)
			, m_pVertices(NULL), m_pFaces(NULL)
			, m_bHeightField(false)
		{
		}

		~SECTOR_INFO()
		{
			SAFE_DELETE(m_pVertices);
			SAFE_DELETE(m_pFaces);
		}

		int						m_nRefCount;
		MBox					m_PieceBox;

		std::vector<NxVec3>*	m_pVertices;
		std::vector<NxU32>*		m_pFaces;
		bool					m_bHeightField;

		NX_INFO					m_NXInfo;
	};

	int							m_nUsingSquareGridMeshCount;
	int							m_nUsingCubeGridMeshCount;

	// cube sector
	int							m_nCubeXCount;
	int							m_nCubeYCount;
	int							m_nCubeZCount;

	float						m_fCubeXStart;
	float						m_fCubeYStart;
	float						m_fCubeZStart;

	float						m_fCubeXInterval;
	float						m_fCubeYInterval;
	float						m_fCubeZInterval;

	// square sector
	int							m_nSquareDivision;
	float						m_fSquareXInterval;
	float						m_fSquareYInterval;

	// sector buffer
	SECTOR_INFO**				m_pArraySectorCubeGrid;
	SECTOR_INFO**				m_pArraySectorSquareGrid;

	std::vector<SECTOR_INFO*>	m_vecSectorBuff_0;
	std::vector<SECTOR_INFO*>	m_vecSectorBuff_1;
	std::vector<SECTOR_INFO*>*	m_pSectorBuff_Old;
	std::vector<SECTOR_INFO*>*	m_pSectorBuff_Current;
	std::vector<SECTOR_INFO*>*	m_pSectorBuffTemp;

	void						CreateSectorActor(SECTOR_INFO* pSector, std::vector<SECTOR_INFO*>* pVecSectorBufferToAdd);
};

} // namespace physx