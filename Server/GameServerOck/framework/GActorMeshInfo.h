#pragma once

#include "CSMeshInfo.h"
#include "GFrameworkLib.h"

/// ���� ��ƼƼ�� ���� ����
class GFRAMEWORK_API GActorMeshInfo : public MTestMemPool<GActorMeshInfo>
{
private:
	CSMeshInfo		m_MeshInfo;		///< Actor�� Scale���� ����� ���̴�.
	bool			m_bSimple;			/// ���� ĸ���� �� �ϳ��� ���� ����
	MCapsule		m_capSimple;
	float			m_fMaxRadius;			///< �ִ� ������ - �������� ������ ���� �ձ����� �Ÿ�(����ó�� ���� �� �� ��츦 ����)
	float			m_fMinRadius;			///< �ּ� ������ - �������� ������ �����ϴ� ���� ���� �Ÿ�(����ó�� ���� �� �� ��츦 ����)
	void SetSimpleCapsule(MCapsule cap);
	void SetupRadius();
public:
	GActorMeshInfo();
	void Init(CSMeshInfo* pMeshInfo, float fScale);
	void SetDefault(float fScale=1.0f);
	void SetForPlayer();
	void SetRadius(float fMinRadius, float fMaxRadius);

	bool IsSimple() { return m_bSimple; }
	MCapsule GetSimpleCapsule() { return m_capSimple; }
	size_t GetHitGroupCount();
	CSHitGroup& GetHitGroup(int nGroupIndex=0);
	void ReserveHitGroupForTest(int nGroupCount=0);
	float GetColRadius() const { return m_fMaxRadius; }
	float GetHitRadius() const { return m_fMinRadius; }
	float GetAnimationTime(wstring strAnimationName);
	bool HasHitGroup(int nGroupIndex);
};

