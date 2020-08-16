#pragma once

#include "RMeshUtil.h"

namespace rs3 {

const float ANIMATION_FRAME_PER_SECOND = 4800.f;

class RAnimationResource;

enum AnimationLoopType {
	RAniLoopType_Loop = 0,
	RAniLoopType_HoldLastFrame,
};

enum RANIMATIONTRANSITIONTYPE
{
	RAT_NONE = 0,				// ���� ����
	RAT_IMMEDIATE_BLEND,	// ���� �ִϸ��̼��� ����, ��þִϸ��̼��� ���۵Ǹ� �����ð� ����
	RAT_DELAYED_BLEND,		// ���� �ִϸ��̼��� ����, �����ð� ������ �ִϸ��̼��� ���۵ȴ�
	RAT_CROSS_FADE,			// ���� �ִϸ��̼��� �÷���, ��þִϸ��̼��� ���۵Ǹ� �����ð� ����

	RAT_COUNT
};

class RAnimationEvent
{
public:
	string			m_strEvent;		///< �̺�Ʈ �̸�
	unsigned int	m_nFrame;		///< �̺�Ʈ�� �߻��ϴ� ������
	string			m_strParam1;	///< �Ķ���� 1
	string			m_strParam2;	///< �Ķ���� 2
	string			m_strParam3;	///< �Ķ���� 3
	bool			m_bUnconditional; ///< ������ �߻��ؾ��ϴ� �̺�Ʈ
};

class RS_API RAnimation 
{
public:
	RAnimation(string& strName, RAnimationResource* pAnimation, int nID);
	~RAnimation();
	void				SetResource(RAnimationResource* pResource)	{	m_pResource = pResource;	}
	RAnimationResource*	GetResource();
	void				SetAliasName(const char* name);
	const string&		GetAliasName();

	int					GetID();
	void				SetSpeed(const float speed);
	float				GetSpeed();

	AnimationLoopType	GetAnimationLoopType();
	void				SetAnimationLoopType(AnimationLoopType type);

	void				LoadEvent(MXmlElement* pElement);
	void				SaveEvent(MXmlElement* pElement);

	// ��� �̺�Ʈ�� �����
	void				ClearEvent();

	vector<RAnimationEvent*> m_animationEvents;

protected:
	string					m_strAliasName;
	RAnimationResource*		m_pResource;
	int						m_nID;
	float					m_fSpeed;
	AnimationLoopType		m_AniLoopType;
};

//////////////////////////////////////////////////////////////////////////
// inlines
inline RAnimationResource*	RAnimation::GetResource()				{ return m_pResource; }

inline void				RAnimation::SetAliasName(const char* name) { m_strAliasName = name; }
inline const string&	RAnimation::GetAliasName()				{ return m_strAliasName; }

inline int				RAnimation::GetID()						{ return m_nID; }
inline void				RAnimation::SetSpeed(const float speed)	{ m_fSpeed = speed;}
inline float			RAnimation::GetSpeed()					{ return m_fSpeed;}

inline AnimationLoopType RAnimation::GetAnimationLoopType()		{	return m_AniLoopType;	}
inline void				RAnimation::SetAnimationLoopType(AnimationLoopType type)	{	m_AniLoopType = type;	}


}