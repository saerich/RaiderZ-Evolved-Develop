#ifndef _G_PLAYER_INTERACTION_H_
#define _G_PLAYER_INTERACTION_H_

#include "MTime.h"
#include "GNPCInteractionInfo.h"

class GEntityPlayer;
class GEntityNPC;
class GItem;
class GDialogSayInfo;

enum ITRIGGER_TYPE;
enum ITARGET_TYPE;

class GPlayerInteraction : public MTestMemPool<GPlayerInteraction>
{
private:
	GEntityPlayer*				m_pOwner;

	ITRIGGER_TYPE	m_ITriggerType;				///< ���ͷ����� �ϰԵ� ����� ����
	ITARGET_TYPE		m_ITargetType;				///< ���ͷ������� ����� ����
	MUID						m_ITargetUID;				///< ���ͷ������� ����� UID
	MUID						m_IUseItemUID;				///< ���ͷ����� ���� ����� ������ UID

	map<int, GNPCInteractionElementInfo*>	m_mapSelectableElementInfo;	///< ���� ������ ���ͷ��� ������Ʈ
	GNPCInteractionElementInfo				m_SelectedIElementInfo;		///< ���õ� ���ͷ��� ������Ʈ

	int							m_nDilaogID;				///< ��ȭ���� ���̾�α� ID
	GDialogSayInfo*				m_pDialogSayInfo;			///< ��ȭ���� ����

	MRegulator					m_SleepRegulator;				///< IET_INN_SLEEP �ҿ�ð� Ÿ�̸�	
	MRegulator					m_InteractQObjectiveRegulator;	///< IET_QUEST_INTERACT �ҿ�ð� Ÿ�̸�	

public:
	GPlayerInteraction(GEntityPlayer* pOwner);
	~GPlayerInteraction();

	void Update(float fDelta);
	
	void BeginInteraction(ITRIGGER_TYPE nITrigType, ITARGET_TYPE nITarType, MUID uidITarget, const vector<GNPCInteractionElementInfo*>& vecIElementInfo, MUID uidUseItem=MUID::ZERO);	
	void SelectIElement(const GNPCInteractionElementInfo& IElementInfo);
	void EndInteraction();	

	void ConsumeIUseItem();

	ITARGET_TYPE GetITargetType()	{ return m_ITargetType; }
	ITRIGGER_TYPE GetITriggerType()	{ return m_ITriggerType; }
	GNPCInteractionElementInfo* GetSelectableIElementInfo(int nElementID);
	GNPCInteractionElementInfo& GetSelectedIElementInfo()	{ return m_SelectedIElementInfo; }
	GEntityNPC* GetInteractingNPC();
	MUID GetInteractingNPCUID();
	int GetInteractingNPCID();
	GItem* GetInteractingItem();

	void BeginDialog(int nDialogID, GDialogSayInfo* pDialogSayInfo);
	void ContinueDialog(GDialogSayInfo* pDialogSayInfo);
	void EndDialog();		
	
	GDialogSayInfo* GetDialogSayInfo()			{ return m_pDialogSayInfo; }
	bool IsDialogExit();
	int GetDialogID() { return m_nDilaogID; }
	
	void BeginCraft(int nRecipeID, float fTime);
	void BeginSleep(float fTime);	
	void BeginInteractQObjectvie(float fTime);
	void CancelInteractQObjectvie();

	bool IsInteractQObjectiveRegulatorActive();
	bool IsGuildStorageInteraction();
};

#endif//_G_PLAYER_INTERACTION_H_