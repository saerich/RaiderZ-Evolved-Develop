#pragma once

#include "Mint.h"

namespace mint3 {

#define DEFAULT_BASIC_WIDTH	1600

class MScalableLook {

public:

	MScalableLook() { m_bScale = false; m_nBasicWidth = DEFAULT_BASIC_WIDTH; }

	bool		m_bScale;		// �ػ󵵿� ���� (��������) �������Ѵ�
	int			m_nBasicWidth;	// ���� �ػ� Width

	void SetScaleEnable(bool b) {
		m_bScale = b;
	}

	void SetBasicWidth(int n) {
		m_nBasicWidth = n;
	}


	// ���� �ػ󵵿� ���� �����ػ��� ����
	float GetScale() { 
		return m_bScale ? 
			(float)MGetWorkspaceWidth()/(float)m_nBasicWidth : 
			1.f ; 
	}
};

} // namespace mint3