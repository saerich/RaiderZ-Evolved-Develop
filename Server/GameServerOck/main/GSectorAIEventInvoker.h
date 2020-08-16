#pragma once

#include "MMemPool.h"
#include "GAIEventInvoker.h"
#include <queue>

class GSectorAIEventInvoker: public GAIEventInvoker, public MMemPool<GSectorAIEventInvoker>
{
public:
	GSectorAIEventInvoker(MUID uidField);
	virtual ~GSectorAIEventInvoker();

	void EnterPlayer(MPoint ptCell);
	void LeavePlayer(MPoint ptCell, int nPlayerQty);
	void MoveNPC(MUID uidNPC, bool bPlayerEmptyInNeighborCells);
	// �����̵�ó�� Leave-Enter ������ �۵��ϴ� �̺�Ʈ ���̿�, ���� ���Ϳ��� �÷��̾ �ϳ��� ���� ��Ȳ�� ���������.
	// ���� �� �� ��Ȳ���� �̺�Ʈ�� �˻��� ��� LOD�� Ƣ�� ������ �������� �Լ�.
	void LazedNotifyBegin(void);	// ��� �뺸���� �ʰ� �׾Ƶд�.
	void LazedNotifyEnd(void);		// ���� �̺�Ʈ�� �ϰ� �뺸�Ѵ�.

	void Invoke(GAIEvent aiEvent);

	void InvokeEnterPlayer(MUID uidField, int nSectorX, int nSectorY);
	void InvokeLeavePlayer(MUID uidField, int nSectorX, int nSectorY);
	void InvokeMoveNPCBePlayer(MUID uidNPC);
	void InvokeMoveNPCEmptyPlayer(MUID uidNPC);

private:
	MUID m_uidField;
	bool m_isPlayerEmpty;

	bool m_isLazedNotify;
	queue<GAIEvent> m_queueEvent;
};
