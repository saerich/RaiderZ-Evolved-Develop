#pragma once

#include "GDef.h"

enum POSITION_CHECK_TYPE
{
	PCT_INVALID = -1,
	PCT_WALK	= 0,
	PCT_MOTIONFACTOR,
	PCT_WARP,
};

//
//		GPositionChecker
//
//	�̵� ġ��(���ǵ��ٵ�) ������ ���� Ŭ����.
//	�÷��̾�� �ϳ��� �ν��Ͻ��� ���´�.
//
class GPositionChecker
{
public:
	GPositionChecker();

	// �÷��̾� �ν��Ͻ��� �ʱ�ȭ�ɶ� ���� �ʱ�ȭ�Ǵ� �Լ�
	void Init(vec3 vCurPos);

	// �̵��� ��ȿ���� Ȯ��.  Ȯ���ϰ� ���������ν� ����.  
	void Check(POSITION_CHECK_TYPE nType, vec3 vCurPos, float fMoveSpeed, POSITION_CHEAT_TYPE& outnPosCheatType, CHEAT_TREAT_TYPE& outnTreatType );
private:
	// �÷��̾ ���� �ɾ �̵������� ȣ��.  ��ȯ���� ġ���� �ߴ��� ����
	POSITION_CHEAT_TYPE Check_Walk( vec3 vCurPos, float fMoveSpeed, CHEAT_TREAT_TYPE& outnTreatType );
	// ������ ���ؼ� �̵� ������ ȣ��.  ��ȯ���� ġ���� �ߴ��� ����.
	// ������ ���� �����̹Ƿ� �׻� �ùٸ���.
	POSITION_CHEAT_TYPE Check_Warp(vec3 vCurPos, float fMoveSpeed);

	// ������͸� ���ؼ� �̵� ������ ȣ��.  ��ȯ���� ġ���� �ߴ��� ����.
	// ������ ���� �����̹Ƿ� �׻� �ùٸ���.
	POSITION_CHEAT_TYPE Check_MotionfactorMove(vec3 vCurPos, float fMoveSpeed);

	// ���������� ����ߴ� ġ�������� ����
	void SaveLastCheckInfo(vec3 vCurPos);

	// �ɰ��� ġ�� �õ����� Ȯ��
	bool CheckDeepSpeedhack();
	// ���� ġ�� �õ����� Ȯ��
	bool CheckShallowSpeedhack();
	// �������� ��Ű��
	void Disconnect();
	
private:
	// ���������� ġ�� üũ�� Ȯ���� �ð�
	DWORD	m_dwLastCheckTime;

	// ���������� ġ�� üũ�� �������� �÷��̾� ��ġ
	vec3	m_vLastPos;

	// ���������� �ɰ��� ġ�� ����� �ð�
	DWORD	m_dwLastShallowSpeedhackTime;
	// �ֱ� �ɰ��� ġ�� ����� Ƚ��
	int		m_nShallowSpeedhackCount;

	// ���������� �ɰ��� ġ�� ����� �ð�
	DWORD	m_dwLastDeepSpeedhackTime;
	// �ֱ� �ɰ��� ġ�� ����� Ƚ��
	int		m_nDeepSpeedhackCount;
};
