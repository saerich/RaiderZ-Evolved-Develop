#include "StdAfx.h"
#include "GNPCAssist.h"
#include "GConst.h"
#include "GField.h"
#include "GEntityNPC.h"
#include "GRelationChecker.h"

GNPCAssist::GNPCAssist(GEntityNPC* pOwner)
: m_pOwner(pOwner)
{
	m_rgrTick.SetTickTime(GConst::NPC_ASSIST_INTERVAL);
	m_rgrTick.Start();
}

GNPCAssist::~GNPCAssist(void)
{
}

class GAssistFilter //: public GFieldGrid::GSelectorFilter
{
private:
	GField*	m_pField;
	GEntityNPC*	m_pRequestor;

	bool IsCheckAssist(const MUID& uidAssistorNPC)
	{
		PFC_THISFUNC;

		GEntityNPC* pAssistor = m_pField->FindNPC(uidAssistorNPC);
		if (!pAssistor) 		
			return false;

		if (!GNPCAssistHelper::IsAssistable(pAssistor))
		{
			return false;
		}

		if (m_pRequestor == pAssistor) 
			return false;

		if (!GNPCAssistHelper::CheckFaction(pAssistor, m_pRequestor))
		{
			return false;
		}

		if (!GNPCAssistHelper::CheckRange(pAssistor, m_pRequestor))
		{
			return false;
		}

		return true;
	}

public:
	GAssistFilter(GEntityNPC* pRequestor, GField* pField) : m_pRequestor(pRequestor), m_pField(pField) {}
	virtual bool Check(const MUID& uidTarget)
	{
		return IsCheckAssist(uidTarget);
	}
};



void GNPCAssist::Update( float fDelta )
{
	PFC_THISFUNC;

	if (!m_rgrTick.IsReady(fDelta))
		return; // �ֱⰡ ���� ����

	if (!GNPCAssistHelper::IsRequestable(m_pOwner))
	{
		return; // ���� ��û�� �ʿ䰡 ���� ����
	}

	RequestAssistanceToNeighbors();
}


float GNPCAssist::GetAssistRange() const
{
	return GNPCAssistHelper::GetAssistRange(m_pOwner);
}


void GNPCAssist::OnYell( GEntityNPC* pTarget )
{
	VALID(pTarget);

	if (!GNPCAssistHelper::IsAssistable(m_pOwner))
	{
		return;
	}

	// �Ʊ� ����
	m_pOwner->doAssist(pTarget);
}

void GNPCAssist::RequestAssistanceToNeighbors()
{
// 	GField* pField = m_pOwner->GetField();
// 	VALID(pField);
// 
// 	GFieldGrid::EntitySelector* grid_selector = 
// 		pField->GetGrid().GetEntitySelector();
// 
// 	GVectorMUID vecNPCs;
// 	vector<GFieldGrid::Sector*> vecCells;
// 
// 	class CollectSectors : public GFieldGrid::SectorSelector::IVisitor
// 	{
// 	public:
// 		CollectSectors(vector<GFieldGrid::Sector*>& sectors) : m_sectors(sectors)
// 		{
// 			m_sectors.clear();
// 		}
// 
// 	private:
// 		void OnVisit(GFieldGrid::Sector& sector)
// 		{
// 			m_sectors.push_back(&sector);
// 		}
// 
// 	private:
// 		vector<GFieldGrid::Sector*>& m_sectors;
// 	};
// 	grid_selector->VisitNeighborsByRadius(CollectSectors(vecCells), m_pOwner->GetPos(), GetAssistRange());
// 
// 
// 	GAssistFilter filter(m_pOwner, pField);
// 	grid_selector->GetNPCsEx(vecNPCs.Vector(), vecCells, &filter);
// 
// 	for each (const MUID& uid in vecNPCs.Vector())
// 	{
// 		GEntityNPC* pAssistor = pField->FindNPC(uid);
// 		if (!pAssistor) 		
// 			continue;
// 
// 		// �Ʊ� ����
// 		pAssistor->doAssist(m_pOwner);
// 	}



	VALID(NULL!=m_pOwner);
	GField* const pField = m_pOwner->GetField();
	VALID(pField);

	GFieldGrid::EntitySelector* grid_selector = 
		pField->GetGrid().GetEntitySelector();

	GVectorMUID vecNPCs;
	vector<GFieldGrid::Sector*> vecCells;

	class RequestAssist : public GFieldGrid::Sector::IVisitor
	{
	public:
		RequestAssist(GEntityNPC& requester,GAssistFilter& filter,GField& field)
			: m_requester(requester)
			, m_field(field)
			, m_filter(filter) {}

	private:
		void OnVisit(const MUID& uid)
		{
			if (false == m_filter.Check(uid))
				return;

			GEntityNPC* const pAssistor = m_field.FindNPC(uid);
			if (NULL == pAssistor)
				return;

			// �Ʊ� ����
			pAssistor->doAssist(&m_requester);
		}

	private:
		GEntityNPC&		m_requester;
		GAssistFilter&	m_filter;
		GField&			m_field;
	};

	grid_selector->VisitByRadius(
		RequestAssist( *m_pOwner, GAssistFilter(m_pOwner, pField), *pField),
		ETID_NPC,
		m_pOwner->GetPos(),
		GetAssistRange()
	);
}

bool GNPCAssistHelper::IsAssistable( GEntityNPC* pNPC )
{
	if (pNPC->IsObject())
		return false; // ������Ʈ�� ����

	if (!pNPC->GetNPCInfo()->m_bAutoAssist)
		return false; // �ڵ����� ������� ����

	if (pNPC->IsDead())
		return false;	// �׾���

	if (pNPC->IsNowCombat())
		return false; // �̹� ������

	if (pNPC->IsNowReturn())
		return false; // ��ȯ��

	return true;
}

bool GNPCAssistHelper::IsRequestable( GEntityNPC* pNPC )
{
	if (!pNPC->IsNowCombat())
		return false;	// �������� �ƴϸ� ��û���� ����

	if (pNPC->IsDead())
		return false;	// �׾����� ��û���� ����

	if (pNPC->IsNowReturn())
		return false; // ��ȯ��

	return true;
}

float GNPCAssistHelper::GetAssistRange( GEntityNPC* pNPC )
{
	return pNPC->GetNPCInfo()->m_fAutoAssistRange;
}

bool GNPCAssistHelper::CheckRange( GEntityNPC* pAssistor, GEntityNPC* pTarget )
{
	vec3 vTargetPos = pTarget->GetPos();
	float fDistance = pAssistor->GetPos().DistanceTo(vTargetPos);

	return GetAssistRange(pAssistor) >= fDistance;
}

bool GNPCAssistHelper::CheckFaction( GEntityNPC* pAssistor, GEntityNPC* pTarget )
{
	GRelationChecker relationChecker;
	return relationChecker.IsAlly(pAssistor, pTarget);
}