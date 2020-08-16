#pragma once

#include <tchar.h>
#include "RMeshAnimationMgr.h"
#include "MTstring.h"

struct BLEND_DATA 
{
	float				fDurationTime;
	BLEND_DATA(float _fTime) : fDurationTime(_fTime) {}
	BLEND_DATA() { fDurationTime = 0.0f; }

	static const float				DEFAULT_BLEND_DURATION;
};

class XAnimationBlendInfo
{
public:
	XAnimationBlendInfo();
	virtual ~XAnimationBlendInfo();

	void Clear();

	/**
	@brief �ִϸ��̼� ���� Ÿ���� �߰��ϰų� �����Ѵ�.
	@param source ���� �ִϸ��̼� �̸�
	@param target ��� �ִϸ��̼� �̸�
	@param aniTransType ���� �� �ִϸ��̼� ���� Ÿ��
	@param nDuration �ִϸ��̼� ���� �ð�
	@return Ÿ���� �߰��ϰų� ���濡 �����ϸ� true�� ��ȯ�Ѵ�.
	*/
	bool SetAnimationTransData( std::tstring& source, std::tstring& target, float fDurationTime );

	/**
	@brief �ִϸ��̼� ���� Ÿ���� ���´�.
	@param source ���� �ִϸ��̼� �̸�
	@param target ��� �ִϸ��̼� �̸�
	@return Ÿ���� ��ȯ�Ѵ�. �ش� ���̽��� ���� �����Ͱ� ������ �⺻ Ÿ���� ��ȯ�Ѵ�.
	*/
	BLEND_DATA GetAnimationTransData( std::tstring& source, std::tstring& target, RMeshAnimationMgr* AnimationMgr );

	/**
	@brief �ִϸ��̼� ���� Ÿ���� �����Ѵ�.
	@param source ���� �ִϸ��̼� �̸�
	@param target ��� �ִϸ��̼� �̸�
	@return ������ �����ϸ� true�� ��ȯ�Ѵ�.
	*/
	bool DeleteAnimationTransType( std::tstring& source, std::tstring& target );

	bool CheckExistAnimation(RMeshAnimationMgr* AnimationMgr, std::tstring& source, std::tstring& target);

	/// �⺻ ��ȯ Ÿ�� ���
	static const BLEND_DATA					DEFAULT_ANIMATION_TRANSITION_DATA;

public:
	typedef std::pair<std::tstring, std::tstring> AnimationBlendPair;
	typedef map<AnimationBlendPair, BLEND_DATA> AnimationBlendData;

private:
	bool IsAnimationNameMatch( const TCHAR* source, const TCHAR* target );

public:
	bool IsNotDefaultValue( BLEND_DATA blendData );
	AnimationBlendData* GetAnimationBlendData() { return &m_AnimationBlendData; }

protected:
	AnimationBlendData m_AnimationBlendData;
};
