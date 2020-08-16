#ifndef _XSOUL_OBJECT_H
#define _XSOUL_OBJECT_H

#include "XObject.h"

using namespace rs3;


//	#define TEST_SOUL									// �׽�Ʈ ��� ����� �� ���

//	#define SHOW_SPLASH_EFFECT							// �ҿ��� ���� ������ ����Ʈ ���
	#define SHOW_ABSORB_EFFECT							// �ҿ��� ��� �� �� ������ ����Ʈ ���




// SoulUpdateInfo
struct SoulUpdateInfo
{
	XObject*	pTarget;
	vec3		dest_pos;
	vec3		effect_pos;
	DWORD		curr_time;
	DWORD		elipsed;
	float		radian;
	vec3		toler;
};



// XSoulObject
class XSoulObject : public XObject, public MMemPool<XSoulObject>
{
	MDeclareRTTI;

private:
	XObject*		m_pNPC;								// �ҿ��� ������ NPC ������
	MUID			m_uidTarget;						// Ÿ��
	MUID			m_uidEffectSoul;					// �ҿ� ����Ʈ

#ifdef SHOW_SPLASH_EFFECT
	MUID			m_uidEffectSplash;					// �ҿ� ���� ����Ʈ
#endif

#ifdef SHOW_ABSORB_EFFECT
	MUID			m_uidEffectAbsorb;					// �ҿ� ��� ����Ʈ
#endif

	int				m_nSoulType;						// �ҿ� Ÿ��
	vec3			m_vPos;								// �ҿ� ���� ��ġ ��ǥ

	bool			m_bWaiting;							// ó�� ���� �� ���� �ð� ����ϰ� �ִ��� ����
	bool			m_bArrival;							// �ҿ��� �������� �����ߴ��� ����
	bool			m_bBackRush;						// �ҿ��� �������� ����� �Ÿ����� ������ ��� �������� ���ư����� ����

	vec3			m_vFlyingFactor;					// �ҿ��� ��� ���� ���� ����

	DWORD			m_dwTime;							// Ÿ�̸�

	float			m_fMoveSpeed;						// ���� �ӵ�

	vec3			m_vStartPos;						// ó�� ���� ��ġ
	vec3			m_vStartDir;						// ó�� ���� ����

	unsigned int	m_nWaitTime;						// �ҿ� ���� ���� ��� �� �ð�(ms����)


public:
	XSoulObject(MUID uid);
	virtual ~XSoulObject();
	virtual XObjectID				GetType() { return XOBJ_SWORD_TRAIL; }


	bool Create( XObject* pOwnerNpc, const MUID& uidTarget, int nType, unsigned int nWaitTime =0);


protected:
	void GetRandomFlyingFactor();
	void GetBackRushState();

	bool MakeSoulUpdateInfo( SoulUpdateInfo& info);

	virtual void OnDestroy();
	virtual void OnUpdate( float fDelta);

	bool OnWaiting( float fDelta, SoulUpdateInfo& info);
	bool OnUpdateMoveSoul( float fDelta, SoulUpdateInfo& info);
	bool OnUpdateEffect( float fDelta, SoulUpdateInfo& info);


#ifdef TEST_SOUL
	void RebirthTest();
#endif

};



#endif