#pragma once

#include "GAIEventHandler.h"

enum AI_LOD_LEVEL 
{
	AI_LOD_INVALID_LEVEL = -1,
	AI_LOD_LEVEL_1 = 0,				///< Combat				(������)
	AI_LOD_LEVEL_2,					///< Neighbor Sector	(�ֺ� ���Ϳ� �÷��̾� ����)
	AI_LOD_LEVEL_3,					///< Major Agent		(LOD�� ������� �׻� AI�� ������ �ϴ� ���� - ���� ������)
	AI_LOD_LEVEL_4,					///< Field				(�ʵ忡 �÷��̾ ����)
	AI_LOD_LEVEL_5,					///< out of sight		(�ʵ忡 �÷��̾ ����)

	MAX_AI_LOD_LEVEL
};


class GNPCAILod: public GAIEventHandler, public MTestMemPool<GNPCAILod>
{
public:
	GNPCAILod();
	virtual ~GNPCAILod();

	virtual void OnNotify(GAIEvent aiEvent);
	AI_LOD_LEVEL GetLodLevel(void) const;
	wstring GetName(AI_LOD_LEVEL aiLod) const;

private:	
	AI_LOD_LEVEL m_aiLod;	
	AI_LOD_LEVEL m_aiOldLod;
};
