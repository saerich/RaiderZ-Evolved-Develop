#pragma once

#include "GDef.h"

class MockField;
class GEntityNPC;
class GEntityActor;
class GNPCInfo;

class GUTHelper_NPC
{
public:
	// NPCInfo�� ����
	static GNPCInfo*	NewNPCInfo(int nNPCId=INVALID_ID);
	// �ش� ���ʵ忡 NPC�� ���� ��Ų��.
	static GEntityNPC* SpawnNPC(MockField* pField, int nNPCId=INVALID_ID, const vec3& vPos=vec3::ZERO);
	// �ش� ���ʵ忡 MockNPC�� ���� ��Ų��.
	template<typename T>
	static T* NewMockNPC(MockField* pField, int nNPCId=INVALID_ID, const vec3& vPos=vec3::ZERO)
	{
		GNPCInfo* pNPCInfo = NewNPCInfo(nNPCId);
		return pField->NewMockNPC<T>(pNPCInfo, vPos);
	}
	// ��ǥ ���Ͱ� ���� ���¸� ����
	static void MakeCombat(GEntityNPC* pNPC, GEntityActor* pOpponent);
	static void MakeCombat(GEntityNPC* pNPC);
	static GNPCInfo*	NewNPCInfo_DeulFlag();
};
