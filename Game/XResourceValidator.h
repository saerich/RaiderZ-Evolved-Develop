#pragma once

#include "XTalentInfoValidator.h"
#include "XNPCInfoValidator.h"
#include "XAnimationEventValidator.h"
#include "XIsAnimationValidator.h"
#include "XItemValidator.h"
//#include "XSoundValidator.h"				// <<<rsound3

/// ���Ϸκ��� �о���̴� ���� ������ �ùٸ��� üũ�ϴ� Ŭ����
class XResourceValidator
{
private:
	XTalentInfoValidator		m_TalentInfoValidator;
	XNPCInfoValidator			m_NPCInfoValidator;
	XAnimationEventValidator	m_AnimationValidator;
	XIsAnimationValidator		m_ISAnimationValidator;
	XItemValidator				m_ItemValidator;
//	XSoundValidator				m_SoundValidator;			<<<rsound3

	void InitOutputWidget( bool ret );
public:
	bool CheckAll();

	bool CheckTalentInfo();
	bool CheckNPCInfo();
	bool CheckAnimaionEvent();
	bool CheckISAnimaion();
	bool CheckItem();
	bool CheckSound();
	bool CheckXML();
};