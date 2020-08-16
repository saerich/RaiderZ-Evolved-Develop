#ifndef _XNPCICON_EFFECT_H
#define	_XNPCICON_EFFECT_H

#define ICON_EFFECT_TRIGGER				L"ef_head_icon_switch"
#define ICON_EFFECT_ITEM_UNUSABLE		L"ef_head_icon_unuse"
#define ICON_EFFECT_ITEM_USABLE			L"ef_head_icon_use"
#define ICON_EFFECT_HELP				L"ef_head_icon_h00"
#define ICON_EFFECT_GUILD_CREATE		L"ef_head_icon_l00"
#define ICON_EFFECT_TALK				L"ef_head_icon_l00"
#define ICON_EFFECT_QUEST_NOT_BEGIN		L"ef_head_icon_q99"		//����Ʈ ������ ���� ����
#define ICON_EFFECT_QUEST_BEGIN			L"ef_head_icon_q00"		//����Ʈ ����
#define ICON_EFFECT_QUEST_INTERACTION	L"ef_head_icon_q01"		//����Ʈ ���ͷ��� �ϱ�
#define ICON_EFFECT_QUEST_ACCEPT		L"ef_head_icon_q01_1"	//����Ʈ ����. -> CONTINUE��
#define ICON_EFFECT_QUEST_END			L"ef_head_icon_q02"		//����Ʈ �Ϸ�
#define ICON_EFFECT_QUEST_CONTINUE		L"ef_head_icon_q02_1"	//����Ʈ ������
#define ICON_EFFECT_QUEST_REWARD		L"ef_head_icon_q03"		//���� ����.
#define ICON_EFFECT_COMBAT				L"ef_head_icon_s00"
#define ICON_EFFECT_CRAFT				L"ef_head_icon_meister"
#define ICON_EFFECT_SHOP				L"ef_head_icon_store"
#define ICON_EFFECT_CUTSCENE			L"ef_head_icon_cutscene"
#define ICON_EFFECT_GUARD_DISABLE		L"ef_head_icon_nguard01" // ���� �Ұ� ����Ʈ


class XNPCIConEffect
{
protected:
	MUID			m_uidEffect;
	void			CreateNPCIConEffect(XObject * pObject, NPC_ICON_STATE NIT);

public:
	XNPCIConEffect();
	virtual ~XNPCIConEffect();

	void			SetNPCICon(XObject * pObject, NPC_ICON_STATE NIS);
	void			Destroy(XObject * pObject=NULL);
	MUID			CreateEffect(XObject* pObject, wstring& strEffectName, bool	bLaseframe);
};

#endif // _XNPCICON_EFFECT_H
