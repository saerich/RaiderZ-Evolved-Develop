#pragma once

class GEntityPlayer;
class GDuel;
class GField;

//
// GDuelMgr
//
class GDuelMgr : public MTestMemPool<GDuelMgr>
{
public:
	GDuelMgr(GField* pField);
	~GDuelMgr(void);

	// �Ŵ����� �Ҹ�ɶ� ȣ��
	void Fini();

	// ���� ��û.	������ �Ұ����� ��쿡�� RouteFailed() �Լ��� �÷��̾�� ������ �˷��ش�.
	//				��ȯ���� ��û�� �������� ����
	bool DuelRequest(GEntityPlayer* pRequester, MUID uidTargetPlayer);
	// ���� ��û.	������ �Ұ����� ��쿡�� RouteFailed() �Լ��� �÷��̾�� ������ �˷��ش�.
	//				��ȯ���� ��û�� �������� ����
	bool DuelRequest(const vector<GEntityPlayer*>& pFighters1, const vector<GEntityPlayer*>& pFighters2);
	// ���� ��û�� ���� ����.  ��û ���Ŀ��� ���� �� �ִ�.
	void DuelQuestionRespond(GDuel* pDuel, bool bAnswer);

	// �����߿� ���� ��� ȣ���ϴ� �Լ�, ��ȯ���� ������뿡�� �׾����� ����
	bool DuelKilled(GEntityPlayer* pDeadMan, MUID uidKiller);

	// �������� �����ڰ� ������ ���������� ȣ��Ǵ� �Լ�
	void Disconnect(GEntityPlayer* pDisconnector);

	// ���� ��ü�� ������ ��ȯ
	int	 GetCount();

	// ƽ���� ȣ���
	void Update(float fDelta);

	// ���� ��������� ����
	bool IsDuelOpponent(GEntityPlayer* pPlayer1, GEntityPlayer* pPlayer2) const;

	void InsertDuel(const MUID& uidDuel, GDuel* pDuel);

	// ���� ��ü�� ã���� ȣ��
	GDuel* FindDuel(MUID uidDuel);
private:
	
	// ���� ��ü�� �����ϰ� �Ŵ��� ��Ͽ� ��Ͻ�Ŵ
	GDuel* CreateDuel(GEntityPlayer* pRequester, GEntityPlayer* pTargetPlayer);
	GDuel* CreateDuel(const vector<GEntityPlayer*>& pFighters1, const vector<GEntityPlayer*>& pFighters2);
	// ������û�� �����Ҷ� ȣ��
	void DuelAccept( GDuel* pDuel );
	
	// ������ �̷���� �� ���� ��쿡 ȣ��Ǵ� �Լ� (������û������)
	void DuelCancel(GDuel* pDuel);

	// ���� ��ü�� ��Ͽ��� ������Ŵ
	void EraseDuel(GDuel* pDuel);

	// ���� ��û�� �������� Ȯ���ϰ�, ��û�� �Ұ����ϸ� RouteFailed()�� ȣ��
	bool CheckDuelRequest( GEntityPlayer* pRequester, GEntityPlayer* pTargetPlayer );
	bool CheckPartyDuelRequest( GEntityPlayer* pRequester, const vector<GEntityPlayer*>& pFighters1, const vector<GEntityPlayer*>& pFighters2 );
	// ���� ��û Ŀ�ǵ�����
	void RouteQuestion(GEntityPlayer* pRequester, GEntityPlayer* pTargetPlayer);	
	
protected:
	typedef std::map<MUID, GDuel*>	MAP_DUEL;
	MAP_DUEL		m_mapDuels;	
	GField*			m_pField;
};
