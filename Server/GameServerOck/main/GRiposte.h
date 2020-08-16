#pragma once

#include "CSTalentInfo.h"

class GRiposte
{
public:
	GRiposte(void);
	~GRiposte(void);

	// �ǹ޾�ġ�� ������ �߰�, Rate�� �ǹ޾�ĥ ����ġ, ���� ������ �ǹ޾�ġ��� ��ø��
	void	AddRiposte(TALENT_CATEGORY nTalentCategory, float fRate);
	// �ǹ޾�ġ�� ������ ����, Rate�� �ǹ޾�ĥ ����ġ
	void	EraseRiposte(TALENT_CATEGORY nTalentCategory, float fRate);
	// �ǹ޾�ġ�Ⱑ �ִ��� ���� ��ȯ
	bool	HasRiposte(TALENT_CATEGORY nTalentCategory);
	// �ǹ޾�ġ�� ����ġ�� ��ȯ, 1.0f�� ��ȯ�ϸ� ���� ���� �״�θ� �ǵ��� ��
	float	GetRiposteRate(TALENT_CATEGORY nTalentCategory);
	// �ǹ޾�ġ�� ���� �ʱ�ȭ
	void	Clear();

private:
	// �ŷ�Ʈ ī�װ��� �ǹ޾�ġ�� ��������
	float m_Rate[TC_MAX];
};
