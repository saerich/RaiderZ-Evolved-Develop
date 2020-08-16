#pragma once

#include "CSHitCapsuleIndexMgr.h"
#include "CSChrInfo.h"

class GEntityNPC;
class GNPCInfo;

class GNPCHitcapsuleGroup : public MTestMemPool<GNPCHitcapsuleGroup>
{
public:
	GNPCHitcapsuleGroup(GEntityNPC* pOwner);
	~GNPCHitcapsuleGroup(void);

	// ��Ʈĸ���׷� �ʱ�ȭ
	void Init( GNPCInfo* pNPCInfo );
	// ���� ��Ʈĸ���׷� �ε��� ��ȯ
	int8 GetCapsuleGroupIndex();

	// Ʈ����(��Ʈ,��ũ��Ʈ)�� ���� ��Ʈĸ���׷� ����
	bool ChangeCapsuleGroup_ByTrigger( int8 val );
	// �ŷ�Ʈ ��뿡 ���� ��Ʈĸ���׷� ����
	bool ChangeCapsuleGroup_ByTalent( int8 val );
	// �ŷ�Ʈ ���Ϸῡ ���� ��Ʈĸ���׷� �������
	void ChangeCapsuleGroup_ByTalentComplete();
	// ��庯�濡 ���� ��Ʈĸ���׷� ����
	void ChangeCapsuleGroup_ByMode(NPC_MODE nMode);

private:
	// ��ȿ�� ĸ���׷� ID���� Ȯ��
	bool IsValidCapsuleGroupID( int8 val ) const;
	// ĸ���׷� ������ �˸��� ��Ŷ
	void RouteChangeCapsule(int8 nHitCapsuleGroup);
	
private:
	GEntityNPC* m_pOwner;
	CSHitCapsuleIndexMgr		m_HitCapsuleIndexMgr;
};
