#include "stdafx.h"
#include "GNPCAILodEventSelector.h"
#include "GEntityNPC.h"
#include "GField.h"

GNPCAILodEventSelector::GNPCAILodEventSelector(GEntityNPC* pOwnerNPC): m_pOwnerNPC(pOwnerNPC)
{
	// do nothing
}

GNPCAILodEventSelector::~GNPCAILodEventSelector()
{
	// do nothing
}

bool GNPCAILodEventSelector::IsSelectEvent(GAIEvent aiEvent)
{
	switch(aiEvent.eEventID)
	{
	case GAIEvent::EVENT_ID_SECTOR_LEAVE_PLAYER:		
		return IsPlayerEmptyInNeighborSector();		// �ֺ� ��� Sector���� player���� ������� ��쿡�� ó���Ѵ�.
	default:
		// do nothing
		break;
	}

	return true;
}

bool GNPCAILodEventSelector::IsPlayerEmptyInNeighborSector(void)
{		
	GField* pField = m_pOwnerNPC->GetField();
	
	if (pField == NULL)
	{
		return true;
	}

	GFieldGrid::EntitySelector* grid_selector = 
		pField->GetGrid().GetEntitySelector();

	return grid_selector->IsEmptyPlayers(m_pOwnerNPC->GetPos());
}
