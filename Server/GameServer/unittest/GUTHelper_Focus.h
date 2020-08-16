#pragma once

#include "GUTHelper_Buff.h"
#include "GUTHelper_Talent.h"
#include "GUTHelper_Item.h"

class GEntityPlayer;
class GEntityActor;

class GUTHelper_Focus
{
private:
	GUTHelper_Buff		m_HelperBuff;
	GUTHelper_Talent	m_HelperTalent;
	GUTHelper_Item		m_HelperItem;
public:
	// ��Ŀ���� �ʱ�ȭ��
	void ClearFocus(GEntityPlayer* pUser);
	// ���� ��Ŀ���� ����, ��Ʈ ī��Ʈ�� -1�� ��쿡�� ��Ŀ�� �ɸ���ŭ ������
	void GainBerserk(GEntityPlayer* pUser, bool bLearnLicense=true, int nHitCounter=-1);
	// �ݰ� ��Ŀ���� ����
	void GainCounter(GEntityPlayer* pUser, bool bLearnLicense=true);
	// �ݰݿ� ���̼��� ���
	void LearnLicense_Counter( GEntityPlayer* pUser );
	//������ ���̼��� ���
	void LearnLicense_Berserk( GEntityPlayer* pUser );
	// ���� ��Ŀ���� �����
	void ToggleAdvent(GEntityActor* pUser);
	// ���� ��Ŀ���� �����
	void ToggleSneak(GEntityActor* pUser);
	// ���� ��Ŀ���� �����
	void TogglePrecision(GEntityActor* pUser);
	// ���� ��Ŀ���� ����, ��Ʈ ī��Ʈ�� -1�� ��쿡�� ��Ŀ�� �ɸ���ŭ ������
	void GainEnlighten(GEntityPlayer* pUser, int nHitCounter=-1);
	// ���� ��Ŀ���� ��� ���� �ʿ��� �������� ���
	void GainItemForEnlighten(GEntityPlayer* pUser);
	// ���� ��Ŀ���� ��� ���� �ʿ��� �������� ���
	void GainItemForAdvent( GEntityPlayer* pUser );
	// �ݰ� ��Ŀ���� ��� ���� �ʿ��� �������� ���
	void GainItemForCounter( GEntityPlayer* pUser );
	// ���� ��Ŀ���� ��� ���� �ʿ��� �������� ���
	void GainItemForBerserk( GEntityPlayer* pUser );
	// ���� ��Ŀ���� ��� ���� �ʿ��� �������� ���
	void GainItemForPrecision( GEntityPlayer* pUser );
	// ���� ��Ŀ���� ��� ���� �ʿ��� �������� ���
	void GainItemForSneak( GEntityPlayer* pUser );
};
