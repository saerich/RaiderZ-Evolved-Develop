#pragma once

#include "GTalentInfo.h"

class GEntityActor;
class GEntityPlayer;
class GTalentInfo;

class GUTHelper_Talent
{
public:
	// �÷��̾ �ŷ�Ʈ�� ���
	void Train(GEntityPlayer* pPlayer, GTalentInfo* pTalentInfo);
	// �÷��̾ ��� ��� �ŷ�Ʈ�� ����
	void UntrainAll( GEntityPlayer* pPlayer );
	// �ŷ�Ʈ�� �������� �Է��Ѵ�.
	void SetTalentDamage( GTalentInfo* pTalentInfo, int nDamage );
	// �ŷ�Ʈ�� �ǰ� ������ ������ �浹������ �Է��Ѵ�.
	void AddSimpleTalentInfoHitInfo( GTalentInfo* pTalentInfo, float fCheckTime=0.0f );
	// �ŷ�Ʈ������ �����ϰ�,TalentMgr�� �߰��Ѵ�.  nID�� INVALID�� ���, ���ο� ID�� �Ҵ����ش�.
	GTalentInfo* NewTalentInfo(int nID=INVALID_ID, int nTalentLine=0, int8 nRank=0, int nCooltimeLine=INVALID_ID);
	// ���߸� ��� ������ �ŷ�Ʈ������ �����ϰ�,TalentMgr�� �߰��Ѵ�.  nID�� INVALID�� ���, ���ο� ID�� �Ҵ����ش�.
	GTalentInfo* NewTrainingNeedTalentInfo(int nID=INVALID_ID, int nTalentLine=0, int8 nRank=0);
	// ���� �ŷ�Ʈ������ �����ϰ�,TalentMgr�� �߰��Ѵ�.  nID�� INVALID�� ���, ���ο� ID�� �Ҵ����ش�.
	GTalentInfo* NewMeleeTalentInfo(int nID=INVALID_ID);
	// �нú� �ŷ�Ʈ������ �����ϰ�,TalentMgr�� �߰��Ѵ�.  nID�� INVALID�� ���, ���ο� ID�� �Ҵ����ش�.
	GTalentInfo* NewPassiveTalentInfo(int nID=INVALID_ID);
	// �нú� �ŷ�Ʈ������ �����ϰ�,TalentMgr�� �߰��Ѵ�.  nID�� INVALID�� ���, ���ο� ID�� �Ҵ����ش�.
	GTalentInfo* NewExtraPassiveTalentInfo( TALENT_EXTRA_PASSIVE_TYPE nExtraPassiveType, int nID=INVALID_ID, int nTalentLine=INVALID_ID);
	// ��� �ŷ�Ʈ������ �����ϰ�,TalentMgr�� �߰��Ѵ�.  nID�� INVALID�� ���, ���ο� ID�� �Ҵ����ش�.
	GTalentInfo* NewGuardTalentInfo(int nID=INVALID_ID);
	// Ȱ �ŷ�Ʈ������ �����ϰ�,TalentMgr�� �߰��Ѵ�.  nID�� INVALID�� ���, ���ο� ID�� �Ҵ����ش�.
	GTalentInfo* NewArcheryTalentInfo(int nID=INVALID_ID);
	// ���� �ŷ�Ʈ������ �����ϰ�,TalentMgr�� �߰��Ѵ�.  nID�� INVALID�� ���, ���ο� ID�� �Ҵ����ش�.
	GTalentInfo* NewMagicTalentInfo(int nID=INVALID_ID);
	// �����ۻ�� �ŷ�Ʈ������ �����ϰ�,TalentMgr�� �߰��Ѵ�.  nID�� INVALID�� ���, ���ο� ID�� �Ҵ����ش�.
	GTalentInfo* NewItemTalentInfo(int nID=INVALID_ID);
	// ���� �ŷ�Ʈ������ �����ϰ�, �������� �Է��� �� TalentMgr�� �߰��Ѵ�.  
	// nTalentID�� INVALID�� ���, ���ο� ID�� �Ҵ����ش�.
	GTalentInfo* MakeDamageTalent( int nDamage, int nTalentID=INVALID_ID );
	// �и� �ŷ�Ʈ �⺻���� �Է�
	void SetMeleeTalentInfo( GTalentInfo* pTalentInfo, int nID=INVALID_ID );
	// �ͽ�Ʈ���Ƽ�� �ŷ�Ʈ �⺻���� �Է�
	void SetExtraActiveTalentInfo( GTalentInfo* pTalentInfo, int nID, TALENT_EXTRA_ACTIVE_TYPE nExtraActiveType );
	// �ͽ�Ʈ���нú� �ŷ�Ʈ �⺻���� �Է�
	void SetExtraPassiveTalentInfo( GTalentInfo* pTalentInfo, int nID, TALENT_EXTRA_PASSIVE_TYPE nExtraPassiveType );
	// �нú� �ŷ�Ʈ �⺻���� �Է�
	void SetPassiveTalentInfo(GTalentInfo* pTalentInfo, int nID=INVALID_ID);
	// Ȱ �ŷ�Ʈ �⺻���� �Է�
	void SetArcheryTalentInfo(GTalentInfo* pTalentInfo, int nID=INVALID_ID);
	// ���� �ŷ�Ʈ �⺻���� �Է�
	void SetMagicTalentInfo(GTalentInfo* pTalentInfo, int nID=INVALID_ID);
	// �̻����� ���Ե� ���� �ŷ�Ʈ �⺻���� �Է�
	void SetMagicMissileTalentInfo(GTalentInfo* pTalentInfo, int nID=INVALID_ID);
	// Ÿ���� ���� �ŷ�Ʈ �⺻���� �Է�
	void SetMagicTargetTalentInfo(GTalentInfo* pTalentInfo, int nID=INVALID_ID);
	// �������� �ŷ�Ʈ �⺻���� �Է�
	void SetMagicAreaTalentInfo( GTalentInfo* pTalentInfo, int nID=INVALID_ID);
	// �ŷ�Ʈ �̺�Ʈ �߰�
	void AddTalentEventInfo(GTalentInfo* pTalentInfo, TALENT_EVENT nEvent, float fTime);
	// �ŷ�Ʈ ��Ʈ �̺�Ʈ �߰�
	void AddTalentActEventInfo(GTalentInfo* pTalentInfo, TALENT_EVENT nEvent, float fTime);
	// TALENT_TARGET_INFO�� ������� �ŷ�Ʈ�� ����ϰ�, �ùٷ� ���� �� �� �ְ� �ʵ带 update��Ŵ
	void UseTalent(GEntityActor* pUser, GTalentInfo* pTalentInfo, TALENT_TARGET_INFO target_info=TALENT_TARGET_INFO(), bool bCancelCurTalent=true);
	// MUID�� ������� �ŷ�Ʈ�� ����ϰ�, �ùٷ� ���� �� �� �ְ� �ʵ带 update��Ŵ
	void UseTalent(GEntityActor* pUser, GTalentInfo* pTalentInfo, MUID uidTarget, bool bCancelCurTalent=true);
	// �� �� (�� �ʿ��� �ŷ�Ʈ ����)
	void Guard( GEntityActor* pUser );
	// ���� ������� �ŷ�Ʈ�� ���
	void CancelTalent( GEntityActor* pUser );
	// ��� �������� ���̼��� ����
	void TrainEquipUsableAllTalent(GEntityPlayer* pPlayer);	
	// �������� ���̼��� ����
	void LearnExtraPassiveTalent(GEntityPlayer* pPlayer, TALENT_EXTRA_PASSIVE_TYPE nExtraPassive);
};
