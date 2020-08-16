#ifndef _XMODULE_UI_H
#define _XMODULE_UI_H

#include "XInteractionEffect.h"

class XNPCInfo;
class XCharacterCaption;
class XUIBalloonTextBox;
class XDebugScreenDialog;
class XMiniHpBar;
class XHeadIconPlacer;
class XCheckTargetableEffect;

class XModuleUI : public XModule, public MMemPool<XModuleUI>
{
	DECLARE_ID(XMID_UI)

protected:
	XModuleEntity*			m_pModuleEntity;

	XHeadIconPlacer*		m_pHeadIconPlacer;
	//pane
	XCharacterCaption*		m_pNameCaption;	///< �̸� ǥ�ÿ�
	XCharacterCaption*		m_pGuildCaption;		///< ���� ǥ�ÿ�
	XMiniHpBar*				m_pMiniBar;

	MRegulator				m_PaneRegulator;

	XCheckTargetableEffect*	m_pCheckTargetableEffect;	///< ���ͷ��� �ε�������
	XUIBalloonTextBox*		m_pBalloonTextBox;		///< ��ǳ��
	XDebugScreenDialog*		m_pDebugScreenDialog;	///< npc colt ������ ��ǳ��

	bool					m_bShowPartyMiniHpBar;

protected:
	virtual void			OnActivate();
	virtual void			OnDeactivate();

	virtual void			OnInitialized();
	virtual void			OnUpdate(float fDelta);
	virtual XEventResult	OnEvent(XEvent& msg);
	virtual void			OnSubscribeEvent();

	void			SetCaptionVisible(bool bShow);

	void			UpdateNPCPaneVisible( float fDelta, XNPCInfo* pThisNPCInfo );
	void			UpdatePlayerPaneVisible(float fDelta);
public:
	XModuleUI(XObject* pOwner=NULL);
	virtual ~XModuleUI(void);

	void			Destroy();

	void			UpdateCharacterPane(float fDelta);		// �̸� ���� ��ġ ������Ʈ

	void			UpdateNPCNameCaption(wstring strTitleBoneName, float fTitleHeight);	// �̸� ���� ��ġ ������ ������...
	void			UpdatePlayerNameCaption();	// �̸� ���� ��ġ �⺻
	bool			UpdateNPCMiniHpBar();
	bool			UpdatePlayerMiniHpBar();
	void			UpdatePartyMiniHpBar();

	void			SetNameColor(DWORD dwColor);
	void			SetNPCNamePane();
	void			SetCharacterPane( wstring& strName, wstring &strClan, DWORD dwColor);
	void			SetCharacterPane_NPC(bool bShowName, wstring strName, wstring strClan, const int& nLevel, bool bMonster);


	void			SetShowPartyMiniHpBar(bool bForce) { m_bShowPartyMiniHpBar = bForce; }

	vec3			GetNamePos();

	// NPC ICon
	void			SetNPCIConState(NPC_ICON_STATE val);

	void			OnBallonText(wstring szID, wstring szChat, float fMaintainTime);
	void			OnBallonText_Debug(wstring szID, wstring szChat);
	bool			IsShowBallonText();
	float			GetBallonLifeTime(float fMaintainTime, wstring &szChat);

	bool			IsShowLootbleTargetEffect() const;

	XHeadIconPlacer* GetHeadIconPlacer()	{ return m_pHeadIconPlacer; }		// NULL�� �� �����Ƿ� NULLüũ�� �ؾ� �մϴ�.
};
#endif //_XMODULE_UI_H
