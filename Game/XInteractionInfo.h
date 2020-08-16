#pragma once

#include "XTargetActorInfo.h"

/// ���ͷ��ǰ� ���õ� ������ ��� �̰��� �Ӵϴ�. - birdkr
class XInteractionInfo
{
private:
	MUID		m_InteractionSearchUID;		// Search�� ObjectUID 
public:
	XInteractionInfo();
	~XInteractionInfo();


	XTargetActorInfo	stTargetActorInfo;			///< Ÿ��
	MUID				InteractionTargetUID;		///< Interaction ���� ObjectUID

	MUID				RequestPlayerUID;			///< ������ ��û�ϴ� ������ UID

	MUID				VSPlayerUID;				///< �������� ������ UID

	bool		m_bNPCInteractionState;		// NPC�� ���ͷ��� ����

	void		Init_ForChangeField();

	UIID		GetSearchUIID()				{ return stTargetActorInfo.nSearchUIID; }
	void		SetSearchUIID(UIID uiid)	{ stTargetActorInfo.nSearchUIID = uiid; };

	UIID		GetTargetUIID()				{ return stTargetActorInfo.nTargetUIID; }
	void		SetTargetUIID(UIID uiid)	{ stTargetActorInfo.nTargetUIID = uiid; };

	MUID		GetInteractionSearchUID()	{ return m_InteractionSearchUID; }
	void		SetInteractionSearchUID(MUID uid)	{ m_InteractionSearchUID = uid; }

	MUID		GetRequestPlayerUID()				{ return RequestPlayerUID; }
	void		SetRequestPlayerUID(MUID uid)		{ RequestPlayerUID = uid; }

};