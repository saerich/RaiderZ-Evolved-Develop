#pragma once

#include "GPositionChecker.h"
#include "GDef.h"

class GEntityPlayer;


//
//		GCheatChecker
//
//	ġ�� ���� Ŭ�������� �����ϴ� Ŭ����
//
class GCheatChecker
{
public:
	GCheatChecker(void);
	virtual ~GCheatChecker(void);

	// �÷��̾� �ν��Ͻ��� �ʱ�ȭ�ɶ� ���� �ʱ�ȭ�Ǵ� �Լ�
	void Init(GEntityPlayer* pPlayer);

	// �̵������� ���� �� �ҷ�����
	void CheckPosition(POSITION_CHECK_TYPE nType, GEntityPlayer* pPlayer);

	void Update(float fDelta);

protected:
	// ġ��
	virtual void OnLog_Disconnect( CHEAT_TYPE nCheatType, int nCheatSubType, GEntityPlayer* pPlayer );
	virtual void OnLog_Warning( CHEAT_TYPE nCheatType, int nCheatSubType, GEntityPlayer* pPlayer );

	// ó���ϱ�
	void OnTreat( CHEAT_TYPE nCheatType, int nCheatSubType, CHEAT_TREAT_TYPE nTreat, GEntityPlayer* pPlayer );
	
private:
	// �̵����� ġ��üũ�� ���� �ν��Ͻ�
	GPositionChecker	m_PositionChecker; 
	MRegulator			m_rgrTick;
	bool				m_bCheckable;
};
