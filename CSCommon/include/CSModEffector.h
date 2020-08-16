#pragma once

#include "CSCommonLib.h"
#include "CSModifier.h"

class CSCOMMON_API CSModEffector
{
public:
	// ���Ϳ� ������̾�(�����Ӽ�)�� ���� 
	void AddActorModifier(CSActorModifier& actorModifier, const CSActorModifier& Modifier);
	// ���Ϳ� ������̾�(�����Ӽ�)�� ����
	void EraseActorModifier(CSActorModifier& actorModifier, const CSActorModifier& Modifier);
	// �÷��̾�� ������̾�(�����Ӽ�)�� ����
	void AddPlayerModifier(CSPlayerModifier& playerModifier, const CSPlayerModifier& Modifier);
	// �÷��̾�� ������̾�(�����Ӽ�)�� ����
	void ErasePlayerModifier(CSPlayerModifier& playerModifier, const CSPlayerModifier& Modifier);
};
