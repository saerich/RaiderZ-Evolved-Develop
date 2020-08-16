#pragma once

#include "GHateTableDef.h"

class GEntityNPC;

// å��:	������-�̺�Ʈ�� �°� Ÿ�ټ����͸� �������ش�.
//			������ ������� �ǵ����� å�ӵ� �ô´�.
//
// ����:	GTargetSelectionEvent, GHateTable
class GHateTableSwapper
{
public:
	GHateTableSwapper(GEntityNPC* pOwner, HatePointMap& mapAggroScore);
	
	bool ApplyEvent(HATE_TARGET_SWAP_EVENT nEventType, float fDuration);
	void Update(float fDelta);
	MUID GetTargetByEvent() const;
	void ResetTarget();
	bool HasAppled() const;
private:
	bool Enter(HATE_TARGET_SWAP_EVENT nEventType);
	void Exit();
	void CompleateFlip(HATE_TARGET_SWAP_EVENT nEventType);
	void OnFilp(HATE_TARGET_SWAP_EVENT nEventType, MUID& outuidTop, MUID& outuidBottom );
	void _SortByScore(HateReverseMap& mapScore);
	bool IsValidTarget(MUID id);

	void OnApply_Flip(const HateReverseMap& mapScore, MUID& outuidTop, MUID& outuidBottom);
	void OnApply_Random(const HateReverseMap& mapScore, MUID& outuidTop, MUID& outuidBottom);
	void OnApply_Near(const HateReverseMap& mapScore, MUID& outuidTop, MUID& outuidBottom);
	void OnApply_Far(const HateReverseMap& mapScore, MUID& outuidTop, MUID& outuidBottom);
	void OnApply_Short(const HateReverseMap& mapScore, MUID& outuidTop, MUID& outuidBottom);
private:
	HATE_TARGET_SWAP_EVENT	m_nEventType;
	MUID					m_uidTarget;
	MRegulator				m_rgrExpirer;
	HatePointMap&			m_mapAggroScore;
	GEntityNPC*				m_pOwner;
};
