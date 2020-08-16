#include "stdafx.h"
#include "GFieldGrid.h"

//////////////////////////////////////////////////////////////////////////
//
//	Selector
//
//////////////////////////////////////////////////////////////////////////
namespace
{
	void CorrectIfOverHugeSectorHalfSideCount(long& count)
	{
		if (count > HUGE_SECTOR_SCALE)
		{
			count = HUGE_SECTOR_SCALE;
		}
	}
}

GFieldGrid::SectorSelector::SectorSelector( const GFieldGrid& grid )
: m_Grid(grid)
{
}

GFieldGrid::Sector* GFieldGrid::SectorSelector::Lookup( int x, int y ) const
{
	return m_Grid.Lookup(x, y);
}

GFieldGrid::Sector* GFieldGrid::SectorSelector::LookupHugeSector( int x, int y ) const
{
	return m_Grid.LookupHugeSector(MPoint(x, y));
}

void GFieldGrid::SectorSelector::VisitNeighbors(GFieldGrid::SectorSelector::IVisitor& visitor,const MPoint& sectorNo, long halfSideCnt) const
{
	CorrectIfOverHugeSectorHalfSideCount(halfSideCnt);

	for (int y = sectorNo.y - halfSideCnt; y <= sectorNo.y + halfSideCnt; ++y)
	{
		for (int x = sectorNo.x - halfSideCnt; x <= sectorNo.x + halfSideCnt; ++x)
		{
			Sector* const sector = m_Grid.Lookup(x, y);
			if (NULL != sector)
			{
				visitor.OnVisit(*sector);
			}
		}
	}
}

void GFieldGrid::SectorSelector::VisitNeighbors(IRefusableVisitor& visitor,const MPoint& sectorNo,long halfSideCnt) const
{
	CorrectIfOverHugeSectorHalfSideCount(halfSideCnt);

	bool continueVisit = true;
	for (int y = sectorNo.y - halfSideCnt; y <= sectorNo.y + halfSideCnt; ++y)
	{
		for (int x = sectorNo.x - halfSideCnt; x <= sectorNo.x + halfSideCnt; ++x)
		{
			Sector* const sector = m_Grid.Lookup(x, y);
			if (NULL == sector)
				continue;

			visitor.OnVisit(*sector, continueVisit);

			if (false == continueVisit)
				return;
		}
	}
}

void GFieldGrid::SectorSelector::VisitNeighborsByPos(IVisitor& visitor,const vec3& position, long halfSideCnt) const
{
	CorrectIfOverHugeSectorHalfSideCount(halfSideCnt);
	VisitNeighbors(visitor, m_Grid.ConvertToIdx(position), halfSideCnt);
}

void GFieldGrid::SectorSelector::VisitNeighborsByRadius(IVisitor& visitor, const vec3& position, const float& radius) const
{
	VALID(0.0f < radius);
	long halfSideCnt = 1 + scast<int>(radius / SECTOR_SIZE);
	CorrectIfOverHugeSectorHalfSideCount(halfSideCnt);
	MPoint ptPoint = m_Grid.ConvertToIdx(position);

	for (int y = ptPoint.y - halfSideCnt; y <= ptPoint.y + halfSideCnt; ++y)
	{
		for (int x = ptPoint.x - halfSideCnt; x <= ptPoint.x + halfSideCnt; ++x)
		{
			Sector* const sector = m_Grid.Lookup(x, y);
			if (NULL == sector)
				continue;

			// ���� ������ �������� radius��ŭ ���� ������ ����� �ش� ������ ���� �ȿ� �ִٸ� ok
			MBox cell_box(
				m_Grid.GetFieldMinX() + (x * SECTOR_SIZE) - radius,		// min_x
				m_Grid.GetFieldMinY() + (y * SECTOR_SIZE) - radius,		// min_y
				position.z,												// min_z
				m_Grid.GetFieldMinX() + ((x+1) * SECTOR_SIZE) + radius,	// max_x
				m_Grid.GetFieldMinY() + ((y+1) * SECTOR_SIZE) + radius,	// max_y
				position.z);											// max_z

			if (cell_box.IsOut(position))
				continue;

			visitor.OnVisit(*sector);
		}
	}
}

void GFieldGrid::SectorSelector::VisitNeighborsIntersectWithAABB(IVisitor& visitor, const vec3& position, const MBox& aabb) const
{
	VALID(aabb.minx < aabb.maxx);
	VALID(aabb.miny < aabb.maxy);
	MBox aabbAtPos(
		aabb.minx + position.x,
		aabb.miny + position.y,
		-1.0f,	// z�� ������� ����
		aabb.maxx + position.x,
		aabb.maxy + position.y,
		1.0f	// z�� ������� ����
		);

	MPoint ptPoint = m_Grid.ConvertToIdx(position);

	long halfSideCnt = 1 + scast<int>(aabbAtPos.GetRadius() / SECTOR_SIZE);
	CorrectIfOverHugeSectorHalfSideCount(halfSideCnt);
	for (int y = ptPoint.y - halfSideCnt; y <= ptPoint.y + halfSideCnt; ++y)
	{
		for (int x = ptPoint.x - halfSideCnt; x <= ptPoint.x + halfSideCnt; ++x)
		{
			Sector* const sector = m_Grid.Lookup(x, y);
			if (NULL == sector)
				continue;

			MBox sectorBox(
				m_Grid.GetFieldMinX() + (x * SECTOR_SIZE),		// min_x
				m_Grid.GetFieldMinY() + (y * SECTOR_SIZE),		// min_y
				0.0f,											// min_z
				m_Grid.GetFieldMinX() + ((x+1) * SECTOR_SIZE),	// max_x
				m_Grid.GetFieldMinY() + ((y+1) * SECTOR_SIZE),	// max_y
				0.0f);											// max_z

			if (sectorBox.IsOut(vec3(aabbAtPos.minx, aabbAtPos.miny, 0.0f)) &&
				sectorBox.IsOut(vec3(aabbAtPos.minx, aabbAtPos.maxy, 0.0f)) &&
				sectorBox.IsOut(vec3(aabbAtPos.maxx, aabbAtPos.miny, 0.0f)) &&
				sectorBox.IsOut(vec3(aabbAtPos.maxx, aabbAtPos.maxy, 0.0f)) &&
				!aabbAtPos.ContainsAABB(sectorBox))
			{
				continue;
			}

			visitor.OnVisit(*sector);
		}
	}
}

void GFieldGrid::SectorSelector::VisitNeighborsIntersectWithRect(IVisitor& visitor,const MRect& rect) const
{
	VALID(rect.left < rect.right);
	MPoint ptStart = m_Grid.ConvertToIdx(vec3(static_cast<float>(rect.left), static_cast<float>(rect.top), 0));
	MPoint ptEnd = m_Grid.ConvertToIdx(vec3(static_cast<float>(rect.right), static_cast<float>(rect.bottom), 0));

	long halfSideCnt = 1 + scast<int>((rect.right - rect.left) / SECTOR_SIZE);
	CorrectIfOverHugeSectorHalfSideCount(halfSideCnt);

	for (int x = ptStart.x - halfSideCnt; x <= ptEnd.x + halfSideCnt; ++x)
	{
		for (int y = ptStart.y - halfSideCnt; y <= ptEnd.y + halfSideCnt; ++y)
		{
			Sector* const sector = m_Grid.Lookup(x, y);
			if (NULL == sector)
				continue;

			visitor.OnVisit(*sector);
		}
	}
}

void GFieldGrid::SectorSelector::VisitHugeNeighbors(IVisitor& visitor,const MPoint& sectorNo) const
{
	for (int y = sectorNo.y - HUGE_SECTOR_SCALE; y < sectorNo.y + HUGE_SECTOR_SCALE * 2; ++y)
	{
		for (int x = sectorNo.x - HUGE_SECTOR_SCALE; x < sectorNo.x + HUGE_SECTOR_SCALE * 2; ++x)
		{
			Sector* const sector = m_Grid.LookupHugeSector(MPoint(x, y));
			if (NULL == sector)
				continue;

			visitor.OnVisit(*sector);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//	EntitySelector
//
//////////////////////////////////////////////////////////////////////////

GFieldGrid::EntitySelector::EntitySelector( const SectorSelector& sectorSelector )
: m_sectorSelector(sectorSelector)
{
}

namespace
{
	class SectorEntitiesPusher : public GFieldGrid::Sector::IVisitor
	{
	public:
		SectorEntitiesPusher(vector<MUID>& vecEntities) : m_vecEntities(vecEntities) {}

	private:
		void OnVisit(const MUID& uid)
		{
			m_vecEntities.push_back(uid);
		}
	private:
		vector<MUID>& m_vecEntities;
	};

	class VisitingSector : public GFieldGrid::SectorSelector::IVisitor
	{
	public:
		VisitingSector(GFieldGrid::Sector::IVisitor& visitor,const ENTITY_TYPEID& type)
			: m_visitor(visitor), m_type(type) { }

	private:
		void OnVisit(GFieldGrid::Sector& sector) { sector.Visit(m_visitor, m_type); }

	private:
		GFieldGrid::Sector::IVisitor&	m_visitor;
		const ENTITY_TYPEID&			m_type;
	};
}

void GFieldGrid::EntitySelector::Visit(Sector::IVisitor& visitor,ENTITY_TYPEID type,const MPoint& sectorNo,const long& halfSideCnt) const
{
	m_sectorSelector.VisitNeighbors( VisitingSector(visitor, type), sectorNo, halfSideCnt );
}

void GFieldGrid::EntitySelector::VisitByPos(Sector::IVisitor& visitor,ENTITY_TYPEID type,const vec3& position,const long& halfSideCnt) const
{
	m_sectorSelector.VisitNeighborsByPos( VisitingSector(visitor, type), position, halfSideCnt );
}

void GFieldGrid::EntitySelector::VisitByRadius(Sector::IVisitor& visitor,ENTITY_TYPEID type,const vec3& position,const float& radius) const
{
	m_sectorSelector.VisitNeighborsByRadius( VisitingSector(visitor, type), position, radius );
}

void GFieldGrid::EntitySelector::VisitIntersectWithAABB(Sector::IVisitor& visitor,ENTITY_TYPEID type,const vec3& position,const MBox& aabb) const
{
	m_sectorSelector.VisitNeighborsIntersectWithAABB( VisitingSector(visitor, type), position, aabb );
}

void GFieldGrid::EntitySelector::VisitIntersectWithRect(Sector::IVisitor& visitor,ENTITY_TYPEID type,const MRect& rect) const
{
	m_sectorSelector.VisitNeighborsIntersectWithRect( VisitingSector(visitor, type), rect );
}

void GFieldGrid::EntitySelector::VisitInHugeSectors(Sector::IVisitor& visitor,ENTITY_TYPEID type,const MPoint& sectorNo) const
{
	m_sectorSelector.VisitHugeNeighbors( VisitingSector(visitor, type), sectorNo );
}

void GFieldGrid::EntitySelector::VisitInHugeSectorsByPos(GFieldGrid::Sector::IVisitor& visitor,ENTITY_TYPEID type,const vec3& position) const
{
	VisitInHugeSectors(visitor, type, m_sectorSelector.Grid().ConvertToIdx(position));
}

bool GFieldGrid::EntitySelector::IsEmptyPlayers( MPoint ptSector, int nSize/*=1*/ ) const
{
	bool isEmptyPlayers = true;
	class PlayerCounter : public SectorSelector::IRefusableVisitor
	{
	public:
		PlayerCounter(bool& result) : m_result(result) {}

	private:
		void OnVisit(const Sector& sector,bool& continueVisit)
		{
			if (0 == sector.GetEntityQty(ETID_PLAYER))
				return;

			m_result = false;
			continueVisit = false;
		}

	private:
		bool& m_result;
	};

	m_sectorSelector.VisitNeighbors( PlayerCounter(isEmptyPlayers), ptSector, nSize );

	return isEmptyPlayers;
}

bool GFieldGrid::EntitySelector::IsEmptyPlayers( vec3 vPoint, int nSize/*=1*/ ) const
{
	return IsEmptyPlayers(m_sectorSelector.Grid().ConvertToIdx(vPoint), nSize);
}

//////////////////////////////////////////////////////////////////////////
//
//	DeltaSelector
//
//////////////////////////////////////////////////////////////////////////

GFieldGrid::DeltaSelector::DeltaSelector( const SectorSelector& sectorSelector)
: m_sectorSelector(sectorSelector)
{
}

GFieldGrid::MovingDirection 
GFieldGrid::DeltaSelector::CalcMovingDirection( MPoint ptOld, MPoint ptNew ) const
{
	int nIntervalX = ptNew.x - ptOld.x;
	int nIntervalY = ptNew.y - ptOld.y;
	MovingDirection nDirction = MD_NONE;

	if (ptNew.x == -1 && ptNew.y == -1)
		return MD_FULL;

	if (ptOld.x == -1 && ptOld.y == -1)
		return MD_FULL;

	if (nIntervalX > 1 || nIntervalY > 1 ||
		nIntervalX < -1 || nIntervalY < -1)				
	{
		return MD_FULL;
	}

	if (nIntervalX == -1 && nIntervalY == 0)		
		return MD_LEFT;
	if (nIntervalX == -1 && nIntervalY == -1)		
		return MD_TOPLEFT;
	if (nIntervalX == 0 && nIntervalY == -1)		
		return MD_TOP;
	if (nIntervalX == 1 && nIntervalY == -1)		
		return MD_TOPRIGHT;
	if (nIntervalX == 1 && nIntervalY == 0)		
		return MD_RIGHT;
	if (nIntervalX == 1 && nIntervalY == 1)		
		return MD_BOTTOMRIGHT;
	if (nIntervalX == 0 && nIntervalY == 1)		
		return MD_BOTTOM;
	if (nIntervalX == -1 && nIntervalY == 1)		
		return MD_BOTTOMLEFT;
	if (nIntervalX == 0 && nIntervalY == 0)		
		return MD_SAME;

	return MD_NONE;
}

void GFieldGrid::DeltaSelector::VisitNewSectors(Sector::IVisitor& visitor, ENTITY_TYPEID entityType, const MPoint& oldSectorPt, const MPoint& newSectorPt, int nSize ) const
{
	VisitImpl(visitor, entityType, &SectorSelector::Lookup, false, CalcMovingDirection(oldSectorPt, newSectorPt), newSectorPt, nSize);
}

void GFieldGrid::DeltaSelector::VisitOldSectors(Sector::IVisitor& visitor,ENTITY_TYPEID entityType,const MPoint& oldSectorPt,const MPoint& newSectorPt,int nSize) const
{
	VisitImpl(visitor, entityType, &SectorSelector::Lookup, true, CalcMovingDirection(oldSectorPt, newSectorPt), oldSectorPt, nSize);
}

void GFieldGrid::DeltaSelector::VisitNewHugeSectors(Sector::IVisitor& visitor,ENTITY_TYPEID entityType,const MPoint& oldSectorPt,const MPoint& newSectorPt) const
{
	VisitImpl(visitor, entityType, &SectorSelector::LookupHugeSector, false, CalcMovingDirection(oldSectorPt, newSectorPt), newSectorPt, HUGE_SECTOR_SCALE);
}

void GFieldGrid::DeltaSelector::VisitOldHugeSectors(Sector::IVisitor& visitor,ENTITY_TYPEID entityType,const MPoint& oldSectorPt,const MPoint& newSectorPt) const
{
	VisitImpl(visitor, entityType, &SectorSelector::LookupHugeSector, true, CalcMovingDirection(oldSectorPt, newSectorPt), oldSectorPt, HUGE_SECTOR_SCALE);
}

void GFieldGrid::DeltaSelector::GetDeltaNewSectors( vector<Sector*>& vecSectors, 
												   MPoint ptOld, 
												   MPoint ptNew,
												   int nSize/*=1*/ ) const
{
	MovingDirection nDirection = CalcMovingDirection(ptOld, ptNew);
	GetDeltaSectorsImpl(false,	vecSectors, nDirection, ptNew, nSize);
}


void GFieldGrid::DeltaSelector::GetDeltaNewHugeSectors( vector<Sector*>& vecSectors, 
													   MPoint ptOld, 
													   MPoint ptNew) const
{
	MovingDirection nDirection = CalcMovingDirection(ptOld, ptNew);
	GetDeltaSectorsImpl(false,	vecSectors, nDirection, ptNew, HUGE_SECTOR_SCALE, MD_NONE, true);
}

void GFieldGrid::DeltaSelector::GetDeltaSectorsImpl(	bool bOld,
													vector<Sector*>& vecSectors, 
													MovingDirection nDirection, 
													MPoint ptSector, 
													int nRealSize,
													MovingDirection nInnerCallType,
													bool bIsHuge
													) const
{
	if (nInnerCallType==MD_NONE)
	{
		vecSectors.clear();
	}

	int nStartX = ptSector.x-nRealSize;
	int nEndX = ptSector.x+(nRealSize*2);
	int nStartY = ptSector.y-nRealSize;
	int nEndY = ptSector.y+(nRealSize*2);

	// ���ο� ���� �ݴ� ��ġ�� ����
	int sign = (bOld)?1: -1;

	// �̵� ���� old �������� ����
	switch(nDirection)
	{
	case MD_SAME:
		return;
	case MD_FULL:
		break;
	case MD_TOP:
		nStartY = ptSector.y+(sign*nRealSize);
		nEndY = nStartY+nRealSize;
		break;
	case MD_BOTTOM:
		nStartY = ptSector.y-(sign*nRealSize);
		nEndY = nStartY+nRealSize;
		break;
	case MD_LEFT:
		nStartX = ptSector.x+(sign*nRealSize);
		nEndX = nStartX+nRealSize;
		break;
	case MD_RIGHT:
		nStartX = ptSector.x-(sign*nRealSize);
		nEndX = nStartX+nRealSize;
		break;
	case MD_TOPLEFT:
		GetDeltaSectorsImpl(bOld, vecSectors, MD_TOP, ptSector, nRealSize, MD_NONE, bIsHuge);
		GetDeltaSectorsImpl(bOld, vecSectors, MD_LEFT, ptSector, nRealSize, MD_TOP, bIsHuge);
		return; 
	case MD_TOPRIGHT:
		GetDeltaSectorsImpl(bOld, vecSectors, MD_TOP, ptSector, nRealSize, MD_NONE, bIsHuge);
		GetDeltaSectorsImpl(bOld, vecSectors, MD_RIGHT, ptSector, nRealSize, MD_TOP, bIsHuge);
		return; 
	case MD_BOTTOMLEFT:
		GetDeltaSectorsImpl(bOld, vecSectors, MD_BOTTOM, ptSector, nRealSize, MD_NONE, bIsHuge);
		GetDeltaSectorsImpl(bOld, vecSectors, MD_LEFT, ptSector, nRealSize, MD_BOTTOM, bIsHuge);
		return; 
	case MD_BOTTOMRIGHT:
		GetDeltaSectorsImpl(bOld, vecSectors, MD_BOTTOM, ptSector, nRealSize, MD_NONE, bIsHuge);
		GetDeltaSectorsImpl(bOld, vecSectors, MD_RIGHT, ptSector, nRealSize, MD_BOTTOM, bIsHuge);
		return; 
	default:
		VALID(false);
	}

	switch(nInnerCallType)
	{
	case MD_TOP:
		if (nDirection == MD_LEFT ||
			nDirection == MD_RIGHT)
		{
			if (bOld)	nEndY -= nRealSize;
			else		nStartY += nRealSize;
		}
		break;
	case MD_BOTTOM:
		if (nDirection == MD_LEFT ||
			nDirection == MD_RIGHT)
		{
			if (bOld)	nStartY += nRealSize;
			else		nEndY -= nRealSize;
		}
		break;
	}

	for (int y=nStartY; y<nEndY; ++y)
	{
		if (y < 0) 
			continue;

		for (int x=nStartX; x<nEndX; ++x)
		{
			if (x < 0)
				continue;

			Sector* pSector = NULL;
			if (bIsHuge)
				pSector = m_sectorSelector.LookupHugeSector(x, y);
			else
				pSector = m_sectorSelector.Lookup(x, y);

			if (!pSector)
				continue;

			vecSectors.push_back(pSector);
		}
	}
}

void GFieldGrid::DeltaSelector::VisitImpl(Sector::IVisitor& visitor, ENTITY_TYPEID entityType, SectorLookupFunc sectorLookUp, bool bOld,MovingDirection nDirection,const MPoint& ptSector,int nRealSize,MovingDirection nInnerCallType) const
{
	VALID(NULL!=sectorLookUp);

	int nStartX = ptSector.x-nRealSize;
	int nEndX = ptSector.x+(nRealSize*2);
	int nStartY = ptSector.y-nRealSize;
	int nEndY = ptSector.y+(nRealSize*2);

	// ���ο� ���� �ݴ� ��ġ�� ����
	int sign = (bOld)?1: -1;

	// �̵� ���� old �������� ����
	switch(nDirection)
	{
	case MD_SAME:
		return;
	case MD_FULL:
		break;
	case MD_TOP:
		nStartY = ptSector.y+(sign*nRealSize);
		nEndY = nStartY+nRealSize;
		break;
	case MD_BOTTOM:
		nStartY = ptSector.y-(sign*nRealSize);
		nEndY = nStartY+nRealSize;
		break;
	case MD_LEFT:
		nStartX = ptSector.x+(sign*nRealSize);
		nEndX = nStartX+nRealSize;
		break;
	case MD_RIGHT:
		nStartX = ptSector.x-(sign*nRealSize);
		nEndX = nStartX+nRealSize;
		break;
	case MD_TOPLEFT:
		VisitImpl(visitor, entityType, sectorLookUp, bOld, MD_TOP, ptSector, nRealSize, MD_NONE);
		VisitImpl(visitor, entityType, sectorLookUp, bOld, MD_LEFT, ptSector, nRealSize, MD_TOP);
		return; 
	case MD_TOPRIGHT:
		VisitImpl(visitor, entityType, sectorLookUp, bOld, MD_TOP, ptSector, nRealSize, MD_NONE);
		VisitImpl(visitor, entityType, sectorLookUp, bOld, MD_RIGHT, ptSector, nRealSize, MD_TOP);
		return; 
	case MD_BOTTOMLEFT:
		VisitImpl(visitor, entityType, sectorLookUp, bOld, MD_BOTTOM, ptSector, nRealSize, MD_NONE);
		VisitImpl(visitor, entityType, sectorLookUp, bOld, MD_LEFT, ptSector, nRealSize, MD_BOTTOM);
		return; 
	case MD_BOTTOMRIGHT:
		VisitImpl(visitor, entityType, sectorLookUp, bOld, MD_BOTTOM, ptSector, nRealSize, MD_NONE);
		VisitImpl(visitor, entityType, sectorLookUp, bOld, MD_RIGHT, ptSector, nRealSize, MD_BOTTOM);
		return; 
	default:
		VALID(false);
	}

	switch(nInnerCallType)
	{
	case MD_TOP:
		if (nDirection == MD_LEFT ||
			nDirection == MD_RIGHT)
		{
			if (bOld)	nEndY -= nRealSize;
			else		nStartY += nRealSize;
		}
		break;
	case MD_BOTTOM:
		if (nDirection == MD_LEFT ||
			nDirection == MD_RIGHT)
		{
			if (bOld)	nStartY += nRealSize;
			else		nEndY -= nRealSize;
		}
		break;
	}

	for (int y=nStartY; y<nEndY; ++y)
	{
		if (y < 0) 
			continue;

		for (int x=nStartX; x<nEndX; ++x)
		{
			if (x < 0)
				continue;

			Sector* const pSector = (m_sectorSelector.*sectorLookUp)(x, y);
			if (NULL == pSector)
				continue;

			pSector->Visit(visitor, entityType);
		}
	}
}