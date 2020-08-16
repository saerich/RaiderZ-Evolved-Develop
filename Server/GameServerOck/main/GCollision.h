#pragma once

#include "GTalentDef.h"
using namespace cscommon_server;

class GEntityActor;
class GTalentInfo;
class GTalent;


/// �浹, ���� ���� ���� Ŭ����
/// TODO: GCollision �ȿ��� �ƿ� GActor::IsHittable�� üũ�ϸ� ������ �� ��������. - birdkr
class GCollision
{
protected:
	static bool IsCheckableOverlappedCol(GEntityActor* pAttacker, GEntityActor* pVictim);
public:
	static bool HitTest(GEntityActor* pThisEntity, MSphere& tarSphere);
	
	static bool HitTest(GEntityActor* pThisEntity, GEntityActor* pTarEntity, float fToler = 0.0f);

	static bool HitTest1(GEntityActor* pThisEntity, GTalentInfo* pTalentInfo, int nTalentColIndex, const vec3& vOwnerPos, const vec3& vOwnerDir, int8& nHitCapsuleGroup, int8& nHitCapsuleindex);
	static bool HitTest2(GEntityActor* pThisEntity, MCapsule& tarCapsule, int8& noutHitCapsuleGroup, int8& noutHitCapsuleindex);		// nHitCapsuleindex�� ������ ������ �浹ĸ���� �ε���
	static bool HitTest3(GEntityActor* pThisEntity, const MVector3& rayOrigin, const MVector3& rayDir, float &outDistance);

	static void HitTestTalent(GTalent* pAttackTalent, int nHitColIndex, set<MUID>* psetAlreadyVictim, vector<TALENT_TARGET_INFO>& outsetVictim);	// psetAlreadyVictim�� NULL�� �� �ִ�.
};


