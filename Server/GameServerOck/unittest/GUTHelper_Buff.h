#pragma once

#include "GDef.h"

class GTalentInfo;
class GBuffInfo;
struct CSBuffEnchantInfo;
class GEntityActor;

class GUTHelper_Buff
{
public:
	// ���� �ŷ�Ʈ �⺻���� �Է�
	void SetBuffTalentInfo(GTalentInfo* pTalentInfo, int nID=INVALID_ID, CSBuffEnchantInfo* pBuffEnchantInfo=NULL);
	// ���� �⺻���� �Է�
	void SetBuffInfo(GBuffInfo* pInfo, int nID=INVALID_ID);
	// ���������� �����ϰ�,TalentMgr�� �߰��Ѵ�.  nID�� INVALID�� ���, ���ο� ID�� �Ҵ����ش�.
	GBuffInfo* NewBuffInfo(int nID=INVALID_BUFF_ID);
	// ���� �ŷ�Ʈ������ �����ϰ�,TalentMgr�� �߰��Ѵ�.  nID�� INVALID�� ���, ���ο� ID�� �Ҵ����ش�.
	GTalentInfo* NewBuffTalentInfo(int nID=INVALID_ID, CSBuffEnchantInfo* pBuffEnchantInfo=NULL);
	// ������ ������ �� �ִ� �ŷ�Ʈ�� ����� TalentMgr�� �߰��Ѵ�.
	GTalentInfo* NewBuffTalent(GBuffInfo*& pBuff, float fDuration=-1.0f, int nTalentID=INVALID_TALENT_ID, int nBuffID=INVALID_BUFF_ID );
	// ������ ������ �� �ִ� �ŷ�Ʈ�� ����� TalentMgr�� �߰��Ѵ�.
	GTalentInfo* NewBuffTalent(GBuffInfo*& pBuff, CSBuffEnchantInfo* pBuffEnchantInfo);
	// ���������� �ް�(������ ���� ����), �ش� ������ pUser���� �ɸ��� �Ѵ�.
	void GainBuffSelf( GEntityActor* pUser, GBuffInfo* pBuff=NULL, float fDuration=BUFF_DURATION_INFINITY);
	// ���������� �ް�(������ ���� ����), �ش� ������ pUser���� �ɸ��� �Ѵ�.
	void GainBuffSelf( GEntityActor* pUser, GBuffInfo* pBuff, CSBuffEnchantInfo* pBuffEnchantInfo);
	// Ư�� ������ ����
	void LostBuff( GEntityActor* pUser, int nBuffID);
	// Ư�� ���� ���� ���θ� ����
	void LostBuffStack( GEntityActor* pUser, int nBuffIDSlot);
};
