#pragma once

#include "GDef.h"
#include "CSChrInfo.h"
#include "MMemPool.h"
#include "GFieldInfo.h"

class GFieldInfo;
class GSectorAIEventInvokerMgr;
class GPacketRouteLimiterMgr;

TEST_FORWARD_FT(FieldGrid, FGrid, Init);
TEST_FORWARD_FT(FieldGrid, FGrid, LookupForCell);
TEST_FORWARD_FT(FieldGrid, FGrid, HugeCell);

//////////////////////////////////////////////////////////////////////////
//
//	GFieldGrid
//
//////////////////////////////////////////////////////////////////////////

class GFieldGrid : public MMemPool<GFieldGrid>
{
	TEST_FRIEND_FT(FieldGrid, FGrid, Init);
	TEST_FRIEND_FT(FieldGrid, FGrid, LookupForCell);
	TEST_FRIEND_FT(FieldGrid, FGrid, HugeCell);
public:

	enum MovingDirection
	{
		MD_NONE=-1,
		MD_LEFT=0,
		MD_TOPLEFT,
		MD_TOP,
		MD_TOPRIGHT,
		MD_RIGHT,
		MD_BOTTOMRIGHT,
		MD_BOTTOM,
		MD_BOTTOMLEFT,
		MD_FULL,
		MD_SAME,
	};

	/// Selection�� �� ����ϴ� ����
// 	class GSelectorFilter
// 	{
// 	public:
// 		virtual bool Check(const MUID& uidTarget) = 0;
// 	};

	//////////////////////////////////////////////////////////////////////////
	//
	//	Sector
	//
	//////////////////////////////////////////////////////////////////////////

	class Sector : public MTestMemPool<GFieldGrid::Sector>
	{
	public:
		class IVisitor
		{
			friend class Sector;
			virtual void OnVisit(const MUID& uid) = 0;
		};

		typedef set<MUID>	EntitySet;
	public:
		Sector();
		~Sector();

		// ��ƼƼ �߰�
		void AddEntity(ENTITY_TYPEID nEntityTypeID, const MUID& uid);
		// ��ƼƼ ����
		void RemoveEntity(ENTITY_TYPEID nEntityTypeID, const MUID& uid);
		// ��ƼƼ ��� �ʱ�ȭ
		void ClearEntity();
		// ��ƼƼ�� �����ϴ��� ����
		bool IsExist(const MUID& uid) const;
		// ��ƼƼ ����
		size_t GetEntityQty(ENTITY_TYPEID type) const;
		// ��ƼƼ ����
		size_t GetEntityQty() const;

		// ��ƼƼ�� �湮
		void Visit(IVisitor& visitor,const ENTITY_TYPEID& type) const;

		// ��ǥ
		void			SetPosition(int x,int y) { m_position.x = x; m_position.y = y; }
		const MPoint&	Position() const { return m_position; }

	private:
		void VisitAllTypes(IVisitor& visitor) const;

	private:
		// ��ƼƼ Ÿ�Ժ� ��
		EntitySet	m_setEntities[ETID_MAX];
		// ��ƼƼ ��ü ��
		EntitySet	m_setAllEntities;
		MPoint		m_position;
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//	Selector
	//
	//////////////////////////////////////////////////////////////////////////

	class SectorSelector
	{
	public:
		class IVisitor
		{
			friend class SectorSelector;
			//todo virtual void OnVisit(const Sector& sector) = 0;
			virtual void OnVisit(Sector& sector) = 0;
		};

	public:
		SectorSelector(const GFieldGrid& grid);
		
		// ���� ���� ��ȯ
		GFieldGrid::Sector* Lookup( int x, int y ) const;
		// ���� �Ŵ� ���� ��ȯ
		GFieldGrid::Sector* LookupHugeSector( int x, int y ) const;

		// �Ʒ� �޼��� ���� ���� halfSideCnt(���� ��� �� ���� �ݰ�. �� - 5 x 5������ ��� ��������� �� ����2��, ������2��)��?
		// ���� �˻��� ���� �߾��� �������� ��4, ��4�� �̻� ���� �ʴ´�. �� ���� ���� �˻��� ����, �ٸ� �䱸������ ���� ��� �ٽ� ������ ����� �Ѵ�.
		// (huge���ʹ� �Ϲ� ������ HUGE_SECTOR_SCALE���̰�, �Ѹ鿡 huge���� 3���̴�. �׸��� �߽��� �������� �˻��ϱ� ������ HUGE_SECTOR_SCALE * 3 / 2�� �ȴ�.)
		// aabb�� radius���� ���������� ��� ����� �������� ���͸� �䱸�ϴ��� 5x5�̻� ���� �ʴ´�.

		// Ư�� ���� ��ǥ �α��� ������ �湮
		void VisitNeighbors(IVisitor& visitor, const MPoint& sectorNo, long halfSideCnt) const;
		// Ư�� ��ǥ �α��� ���͵��� �湮
		void VisitNeighborsByPos(IVisitor& visitor, const vec3& position, long halfSideCnt) const;
		// Ư�� ��ǥ ���� �� �α��� ���͵��� �湮
		void VisitNeighborsByRadius(IVisitor& visitor, const vec3& position, const float& radius) const;
		// Ư�� ��ǥ���� �簢���� ��ġ�� ���͵��� �湮
		void VisitNeighborsIntersectWithAABB(IVisitor& visitor, const vec3& position, const MBox& aabb) const;
		// Ư�� �簢�� ���� ���͵��� �湮
		void VisitNeighborsIntersectWithRect(IVisitor& visitor, const MRect& rect) const;
		// Ư�� �� ��ǥ �α��� �Ŵ뼽�͵� �湮
		void VisitHugeNeighbors(IVisitor& visitor, const MPoint& sectorNo) const;

		// Ư�� �� ��ǥ �α��� ������ ��ȯ (��û�� ���� ����)
//		void GetAllNeighborSectorsExceptThis( vector<Sector*>& vecSectors, MPoint ptPoint, int nSize ) const;

		const GFieldGrid& Grid() const { return m_Grid; }

	protected:
		const GFieldGrid& m_Grid;
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//	EntitySelector
	//
	//////////////////////////////////////////////////////////////////////////

	class EntitySelector : public MTestMemPool<EntitySelector>
	{
	public:
		EntitySelector(const SectorSelector& sectorSelector);

		// Ư�� ������ǥ�� �ش��ϴ� ���Ϳ� �α� ���͵��� ��ƼƼ�� �湮
		void Visit(Sector::IVisitor& visitor,ENTITY_TYPEID type, const MPoint& sectorNo, const long& halfSideCnt ) const;
		// Ư�� ��ǥ�� �ش��ϴ� ���Ϳ� �α� ���͵��� ��ƼƼ�� �湮
		void VisitByPos(Sector::IVisitor& visitor, ENTITY_TYPEID type, const vec3& position, const long& halfSideCnt) const;
		// Ư�� ��ǥ ���� �� �α��� ���͵��� ��ƼƼ�� �湮
		void VisitByRadius(Sector::IVisitor& visitor, ENTITY_TYPEID type, const vec3& position, const float& radius) const;
		// Ư�� ��ǥ���� �簢���� ��ġ�� ���͵��� �湮
		void VisitIntersectWithAABB(Sector::IVisitor& visitor, ENTITY_TYPEID type, const vec3& position, const MBox& aabb) const;
		// Ư�� �簢�� ���� ���͵��� �湮
		void VisitIntersectWithRect(Sector::IVisitor& visitor, ENTITY_TYPEID type, const MRect& rect) const;
		// Ư�� �� ��ǥ �α��� �Ŵ뼽�͵� �湮
		void VisitInHugeSectors(Sector::IVisitor& visitor, ENTITY_TYPEID type, const MPoint& sectorNo) const;

		// Ư�� �� ��ǥ �α� ������ ��ƼƼ�� ���� ��ȯ
		//void GetEntitiesEx( vector<MUID>& vecEntities, ENTITY_TYPEID type, const vector<Sector*>& vecSectors, GSelectorFilter* pFilter, MUID uidExcept=MUID::Invalid() ) const;

		// Ư�� �� ��ǥ �α� ������ ���͵� ���� ��ȯ
		void GetActors( vector<MUID>& vecEntities, vec3 vPoint, int nSize=1 ) const;
		void GetActors( vector<MUID>& vecEntities, const MRect& rect, int nSize=1 ) const;
		void GetActors( vector<MUID>& vecEntities, MPoint ptSector, int nSize=1 ) const;
		// Ư�� �� ��ǥ �α� ������ �÷��̾�� ���� ��ȯ
		void GetPlayers( vector<MUID>& vecEntities, vec3 vPoint, int nSize=1 ) const;
		void GetPlayers( vector<MUID>& vecEntities, const MRect& rect, int nSize=1 ) const;
		void GetPlayers( vector<MUID>& vecEntities, MPoint ptSector, int nSize=1 ) const;
		void GetPlayers( vector<MUID>& vecEntities, MPoint ptSector, int nSize, MUID uidExcept ) const;
		void GetPlayers( vector<MUID>& vecEntities, vec3 vPoint, int nSize, MUID uidExcept ) const;
		// Ư�� �� ��ǥ �α� ������ NPC�� ���� ��ȯ
		void GetNPCs( vector<MUID>& vecEntities, vec3 vPoint, int nSize=1 ) const;
		void GetNPCs( vector<MUID>& vecEntities, const MRect& rect, int nSize=1 ) const;
		void GetNPCs( vector<MUID>& vecEntities, MPoint ptSector, int nSize=1 ) const;
		//void GetNPCsEx( vector<MUID>& vecEntities, const vector<Sector*>& vecSectors, GSelectorFilter* pFilter, MUID uidExcept=MUID::Invalid() ) const;

		void GetHugeEntities( vector<MUID>& vecEntities, vec3 vPoint, ENTITY_TYPEID type ) const;
		// Ư�� �� ��ǥ �α� ������ �������� ���� ��ȯ
		void GetBuffEntities( vector<MUID>& vecEntities, vec3 vPoint, int nSize=1 ) const;
		// Ư�� �� �ֺ��� �÷��̾ ������ �˻�
		bool IsEmptyPlayers(MPoint ptSector, int nSize=1) const;
		bool IsEmptyPlayers(vec3 vPoint, int nSize=1) const;



		void GetEntities( vector<MUID>& vecEntities, ENTITY_TYPEID type, const vector<Sector*>& vecSectors, MUID uidExcept = MUID::ZERO ) const;	//todo ���ſ���
		void GetNPCs( vector<MUID>& vecEntities, const vector<Sector*>& vecSectors, MUID uidExcept = MUID::ZERO) const;//todo ���ſ���
		void GetActors(vector<MUID>& vecEntities,const vector<Sector*>& vecSectors,MUID uidExcept= MUID::ZERO) const;
		void GetPlayers(vector<MUID>& vecEntities,const vector<Sector*>& vecSectors,MUID uidExcept= MUID::ZERO) const;
		void GetBuffEntities(vector<MUID>& vecEntities,const vector<Sector*>& vecSectors,MUID uidExcept= MUID::ZERO) const;



	private:
		const SectorSelector& m_sectorSelector;
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//	DeltaSelector
	//
	//////////////////////////////////////////////////////////////////////////

	class DeltaSelector : public MTestMemPool<DeltaSelector>
	{
	public:
		DeltaSelector(const SectorSelector& sectorSelector);

		// ����� �� ��ǥ�� ���� �̵����� ��ȯ
		MovingDirection CalcMovingDirection(MPoint ptOld, MPoint ptNew) const;

		void VisitNewSectors(Sector::IVisitor& visitor, ENTITY_TYPEID entityType, const MPoint& oldSectorPt, const MPoint& newSectorPt, int nSize ) const;
		void VisitOldSectors(Sector::IVisitor& visitor, ENTITY_TYPEID entityType, const MPoint& oldSectorPt, const MPoint& newSectorPt, int nSize ) const;
		void VisitNewHugeSectors(Sector::IVisitor& visitor, ENTITY_TYPEID entityType, const MPoint& oldSectorPt, const MPoint& newSectorPt ) const;
		void VisitOldHugeSectors(Sector::IVisitor& visitor, ENTITY_TYPEID entityType, const MPoint& oldSectorPt, const MPoint& newSectorPt ) const;

		// �� �̵����⿡ ���� �� ��ȯ
//		void GetDeltaSectors(vector<Sector*>& vecOldSectors, vector<Sector*>& vecNewSectors, MPoint ptOld, MPoint ptNew, int nSize=1) const;
		// �� �̵����⿡ ���� �ű� �� ��ȯ
		void GetDeltaNewSectors( vector<Sector*>& vecSectors, MPoint ptOld, MPoint ptNew, int nSize=1 ) const;
		// �� �̵����⿡ ���� ���ŵ� �� ��ȯ
		void GetDeltaOldSectors( vector<Sector*>& vecSectors, MPoint ptOld, MPoint ptNew, int nSize=1 ) const;
		// �� �̵����⿡ ���� �ű� �Ŵ� �� ��ȯ
		void GetDeltaNewHugeSectors( vector<Sector*>& vecSectors, MPoint ptOld, MPoint ptNew) const;
	
	private:
		// �� �̵����⿡ ���� �� ��ȯ ��� �����Լ�
		void GetDeltaSectorsImpl( bool bOld, vector<Sector*>& vecSectors, MovingDirection nDirection, MPoint ptSector, int nSize, MovingDirection nInnerCallType=MD_NONE, bool bIsHuge=false) const;
		
		typedef GFieldGrid::Sector* (GFieldGrid::SectorSelector::*SectorLookupFunc)(int,int) const;
		void VisitImpl( Sector::IVisitor& visitor, ENTITY_TYPEID entityType, SectorLookupFunc sectorLookUp, bool bOld, MovingDirection nDirection, const MPoint& ptSector, int nSize, MovingDirection nInnerCallType=MD_NONE) const;

	private:
		const SectorSelector& m_sectorSelector;
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//	BoundChecker
	//
	//////////////////////////////////////////////////////////////////////////

	class BoundChecker : public MTestMemPool<BoundChecker>
	{
	public:
		class Lisener
		{
		public:
			virtual void OnChangeOutters(int nPlayerQtyInBoundary) = 0;
		};

		BoundChecker(GFieldGrid::SectorSelector& selector);

		void SetLisener(Lisener* pLisener);
		void SetBoundary(MRect rtBoundary);
		void OnEnterSector(const MPoint& sectorPt, MUID uidPlayer);
		void OnLeaveSector(const MPoint& sectorPt, MUID uidPlayer);

	private:
		bool IsBoundary(const MPoint& sectorPt) const;
		int GetPlayerQtyInBoundary();
		void GetPlayerQtyInBoundaryImpl(int sx, int sy, int& outnPlayerQty);
	private:
		MRect m_rtBoundary;
		Lisener* m_pLisener;
		set<MUID> m_setOutters;
		GFieldGrid::SectorSelector& m_Selector;
	};

public:
	GFieldGrid();
	~GFieldGrid(void);

	// �ʱ� ����
	void Init(const MUID uidField, const GFieldInfo* pFieldInfo);
	// �ʱ�ȭ
	void Clear();
	// ��ƼƼ �߰�
	void AddEntity(Sector& ptSector, ENTITY_TYPEID nEntityTypeID, const MUID& uid, bool bIsHuge=false);
	// ��ƼƼ ����
	bool RemoveEntity(const MPoint& ptSector, ENTITY_TYPEID nEntityTypeID, const MUID& uid, bool bIsHuge=false);
	// ��ƼƼ �̵�
	void MoveEntity( const MPoint& ptOld, const MPoint& ptNew, ENTITY_TYPEID nEntityTypeID, const MUID& uid, bool bIsHuge=false );
	// ������ ��ȯ
	SectorSelector* GetSectorSelector();
	// ��ƼƼ ������ ��ȯ
	EntitySelector* GetEntitySelector();
	// ��Ÿ ������ ��ȯ
	DeltaSelector* GetDeltaSelector();
	// �ٿ��üĿ ����
	void CreateBoundChecker(BoundChecker::Lisener* const pLisener);

	const GFieldInfo* const GetFieldInfo() const;

	// �׸��� ���� ����
	inline int GetSizeX() const;
	// �׸��� ���� ����
	inline int GetSizeY() const;
	// �� ã��
	inline Sector* Lookup(int x, int y) const;
	inline Sector* Lookup(MPoint ptSector) const;
	// ��ȿ�� ������ �˻�
	inline bool IsValidIndex(int x, int y) const;
	inline bool IsValidIndex(MPoint ptSector) const;
	// 1���� ��ǥ��� ����
	inline uint32 CalcFlatPoint(int x, int y) const;
	inline uint32 CalcFlatPoint(MPoint ptSector) const;
	// 3���� ��ǥ�� �� �ε����� ����
	inline MPoint ConvertToIdx(vec3 vPoint) const;
	// �ʵ� �ٿ��ڽ� �ּ���ǥ
	inline float GetFieldMinX() const;
	// �ʵ� �ٿ��ڽ� �ּ���ǥ
	inline float GetFieldMinY() const;
	// �ʵ� �ٿ��ڽ� �ִ���ǥ
	inline float GetFieldMaxX() const;
	// �ʵ� �ٿ��ڽ� �ִ���ǥ
	inline float GetFieldMaxY() const;

	// ��Ŷ ���Ʈ ���� ������ ��ȯ
	GPacketRouteLimiterMgr* GetPacketRouteLimiterMgr();

private:
	void AddEntitiyToHugeSector( const MPoint& ptSector, ENTITY_TYPEID nEntityTypeID, MUID uidEntity );
	void RemoveEntityFromHugeSector( const MPoint& ptSector, ENTITY_TYPEID nEntityTypeID, MUID uidEntity );
	GFieldGrid::Sector* const LookupHugeSector( MPoint ptSector ) const;
private:
	typedef map<uint32,Sector> HugeSectorMap;	// pair<flat_point, huge_mob_qty>

	const GFieldInfo*	m_pFieldInfo;
	Sector*		m_Sectors;
	int			m_nSizeX;
	int			m_nSizeY;
	GSectorAIEventInvokerMgr*	m_pAIInvokerMgr;
	HugeSectorMap	m_mapHugeSectors;
	SectorSelector* m_pSelector;
	EntitySelector* m_pEntitySelector;
	DeltaSelector* m_pDeltaSelector;
	GPacketRouteLimiterMgr* m_pRouteLimiterMgr;
	BoundChecker*			m_pBoundChecker;
	
};


//
// Inline Implements
//

int GFieldGrid::GetSizeX() const
{
	return m_nSizeX;
}

int GFieldGrid::GetSizeY() const
{
	return m_nSizeY;
}

float GFieldGrid::GetFieldMinX() const
{
	return m_pFieldInfo->MinX();
}

float GFieldGrid::GetFieldMinY() const
{
	return m_pFieldInfo->MinY();
}

float GFieldGrid::GetFieldMaxX() const
{
	return m_pFieldInfo->MaxX();
}

float GFieldGrid::GetFieldMaxY() const
{
	return m_pFieldInfo->MaxY();
}

uint32 GFieldGrid::CalcFlatPoint(int x, int y) const
{
	_ASSERT(0 <= x && x < m_nSizeX);
	_ASSERT(0 <= y && y < m_nSizeY);
	return	(y*m_nSizeX)+x;
}

uint32 GFieldGrid::CalcFlatPoint(MPoint ptSector) const
{
	return	CalcFlatPoint(ptSector.x, ptSector.y);
}

MPoint GFieldGrid::ConvertToIdx( vec3 vPoint ) const
{
	return MPoint(
		(int)((vPoint.x - GetFieldMinX()) / SECTOR_SIZE), 
		(int)((vPoint.y - GetFieldMinY()) / SECTOR_SIZE)
		);
}

GFieldGrid::Sector* GFieldGrid::Lookup(int x, int y) const
{
	PFC_THISFUNC;
	if (!IsValidIndex(x, y)) 
		return NULL;

	return &m_Sectors[CalcFlatPoint(x,y)];
}

GFieldGrid::Sector* GFieldGrid::Lookup(MPoint ptSector) const
{
	return Lookup(ptSector.x, ptSector.y);
}

bool GFieldGrid::IsValidIndex(int x, int y) const
{
	if ( x<0 || y<0 || x>=m_nSizeX || y>=m_nSizeY ) 
	{
		return false;
	}
	return true;
}

bool GFieldGrid::IsValidIndex(MPoint ptSector) const
{
	return IsValidIndex(ptSector.x, ptSector.y);
}