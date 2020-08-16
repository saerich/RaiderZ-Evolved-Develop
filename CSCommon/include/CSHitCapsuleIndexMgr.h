#pragma once

#include "MTypes.h"
#include "CSDef.h"
#include "CSCommonLib.h"

class CSCOMMON_API CSHitCapsuleIndexMgr
{
public:
	CSHitCapsuleIndexMgr();

	// ���� ��Ȳ�� �´� ĸ���׷� ���̵� ��ȯ
	int8	GetGroupIndex();
	// NPC��忡 �´� ĸ���׷� ���̵� �ֱ�
	void	InsertNPCModeGroupIndex(int nMode, int8 nGroupID);
	// �ŷ�Ʈ�� ���� ����
	void	Change_ByTalent(int8 nGroupID);
	// �ŷ�Ʈ ����� ���� ��� ���ϰ� ����
	void	Change_ByTalentComplete();
	// ��庯ȯ�� ���� ����
	void	ChangeNPCMode(int nMode);
	// Ʈ����(COLT,Lua) ���� ����, ���� ����� ĸ���׷� ���̵� ������ �ٲ۴�.
	void	Change_ByTrigger(int8 nGroupID);
	// ��� ��� ������ �ʱ�ȭ
	void	Clear();
private:
	// �����忡 �´� ĸ���׷� ���̵� ��ȯ
	int8	GetModeGroupID();
private:
	typedef map<int, int8> MAP_MODE;	// pair<nModeID, nCapsuleGroupID>
	// ��� ��ȯ�� ���� �Ǵ� ĸ���׷� ��
	MAP_MODE	m_mapMode;
	// NPC�� ���� ���
	int			m_nCurNPCMode;
	// �ŷ�Ʈ ���� ����Ǵ� ĸ���׷�
	bool		m_bUsingTalent;
	int8		m_nTalent;
};
