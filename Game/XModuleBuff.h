#ifndef _XMODULE_BUFF_H
#define _XMODULE_BUFF_H

#include "XModule.h"
#include "XModEffector.h"
#include "XBuffAttribute.h"

enum CSMsgType;

class XBuffEnchant
{
private:
	vector<int>				m_vecBuffEnchant;

public:
	XBuffEnchant() {}
	virtual ~XBuffEnchant() {}

	void					Push(int nBuffID);
	void					Pop(int nBuffID);

	void					ParseBuffAttr(XBuffAttribute& buffAttribue);

	bool					CheckEnchantBuffID(int nBuffID);
};

class XBuffInstant
{
private:
	map<int, int>			m_mapBuffInstantCount;

public:
	XBuffInstant() {}
	virtual ~XBuffInstant() {}

	void					PushInstant(int nBuffID);
	void					PopInstant(int nBuffID);
	
	void					ParseBuffAttr(XBuffAttribute& buffAttribue);

	int						GetInstantCount(int nBuffID);
};

class XModuleBuff : public XModule, public MMemPool<XModuleBuff>
{
	DECLARE_ID(XMID_BUFF);
private:
	XModEffector			m_ModEffector;

	XBuffInstant			m_BuffInstant;
	XBuffEnchant			m_BuffEnchant;

	map<int, float>			m_TempRemainBuffList;

	bool					m_bLoadingComplete;

	PALETTE_NUM				m_eSavePaletteIndex;

private:
	void					SetGainBuffAnimation( XBuffInfo * pBuffInfo );
	void					UseAnimation(XBuffInfo * pBuffInfo);									// �ִϸ��̼� ó��
	void					StopAnimation(XBuffInfo * pBuffInfo);									// �ִϸ��̼� ����

	void					AddBuff(int nBuffID, BUFF_STACK_TYPE stackType, float fLostRemained);
	void					SubBuff(int nBuffID);
	void					ClearBuff();

	void					BuffInfoDecrease( int nBuffID );
	void					IncreaseBuff(int nBuffID, float fLostRemained);
	void					DecreaseBuff(int nBuffID);

	void					GainMaintainEffect(XBuffInfo * pBuffInfo);
	void					LostMaintainEffect(XBuffInfo * pBuffInfo, int nStack = 1);

	void					CheckInvisibilityGain(XBuffInfo* pBuffInfo);
	void					CheckInvisibilityLost(XBuffInfo* pBuffInfo);

	void					ActivedBuffUI(XBuffInfo * pBuffInfo, float fLostRemained);
	void					DeactivedBuffUI(XBuffInfo * pBuffInfo);

	void					DestroyBuff();

	void					ReSetRemainBuffList();

	void					CheckBuffDuplication(int nBuffID);
	bool					ExistBuffGain(int nBuffID);

	void					ChangePalette( XBuffInfo * pBuffInfo );
	void					RestorePalette( XBuffInfo * pBuffInfo );
protected:
	virtual void			OnInitialized();
	virtual void			OnFinalized()							{}
	virtual void			OnSubscribeEvent();
	virtual XEventResult	OnEvent(XEvent& msg);
	virtual void			OnUpdate(float fDelta);

public:
	XModuleBuff(XObject* pOwner = NULL);
	virtual ~XModuleBuff(void);

	void					BuffGain(int nBuffID, float fLostRemained, bool bRemainBuffGain = false);

	void					BuffLost(int nBuffID);
	void					BuffHit(int nBuffID);	
	void					BuffDie();					// ���� ����...(ĳ���͵��� �׾����� ó���ϴ� ����)

	void					BuffIncrease(int nBuffID, float fLostRemained, bool bRemainBuffGain = false);
	void					BuffDecrease(int nBuffID);

	void					BuffEnchantGain(int nBuffID, bool bEffect, int nPartsSlotType);	// Plyaer�� ���
	void					BuffEnchantLost(int nBuffID, bool bEffect, int nPartsSlotType);	// Player�� ���

	bool					BuffExist(int nID);
	bool					IsFocusGain(int* pOutBuffID = NULL, int* pOutFocusType = NULL);

	void					GainInstantEffect(int nBuffID);

	// ���� �ɷ��ִ� ������ �Ӽ��� ���Ѵ�.
	XBuffAttribute			GetBuffAttribute();
	int						GetStackBuffCount(int nBuffID);

	void					SetRemainBuffList(int nBuffID, float fRemainTime);

	void					MyBuffAllDelete();
	bool					CheckTempRemainBuffList(int nBuffID);

	void					Init_ForChangeField();

	
};

#endif