#pragma once

#include "MUID.h"
#include "SCommObject.h"
#include "STypes.h"
#include "SAccountInfo.h"
#include "LSelMyCharTimeoutHandler.h"
//#include "PmDef.h"
#include <map>
using namespace std;

class LPlayerObjectManager;
class LServer;
class LMasterServer;

/// �÷��̾� ������Ʈ�� ����
enum class PlayerObjectState
{
	POS_INVALID				= -1,
	POS_WAITING_FOR_LOGIN	= 0,	///< �α��� �����
	POS_SELECT_CHAR,				///< ĳ���� ���� ��
	POS_LOGIN_GAME_SERVER,			///< ���Ӽ��� �α��� ��û ��
	POS_MOVE_SERVER,				///< ���Ӽ����� �̵� ��
};


/// �÷��̾� ������Ʈ
class LPlayerObject : public SCommObject
{	
protected:
	friend class LPlayerObjectManager;

	ACCOUNT_INFO					m_AccountInfo;		///< ���� ����
	PlayerObjectState				m_nState;			///< �÷��̾� ������Ʈ�� ����
		
	vector<SAccountCharFieldInfo>	m_vecAccountCharFieldInfo;
	LMasterServer*					m_pConnectingMasterServer;	

	SServerType						m_nServerType;
	int								m_nServerID;	
	
	int								m_nSelectedCharIndex;
	LSelMyCharTimeoutHandler		m_SelMyCharTimeoutHandler;

	/// PmUserData
	//PmStatIndex						m_statIndex;
	//int								m_nPCCafeID;
	//bool							m_isSetStatIndex;

public:
	LPlayerObject(MUID& uidObject);
	virtual ~LPlayerObject();
	virtual void Create() override;							///< LPlayerObjectManager���� ȣ��
	virtual void Destroy() override;						///< LPlayerObjectManager���� ȣ��

	void InitAccountInfo(ACCOUNT_INFO& info);	
	void StartWatchingSelMyCharTimeout();

	void ClearCharFieldInfo();
	void AddCharFieldInfo(const SAccountCharFieldInfo& accountCharFieldInfo);
	void RemoveCharFieldInfo(size_t nIndex);
	int GetCharFieldInfoCount();
	SAccountCharFieldInfo* GetCharFieldInfo(size_t nIndex);	
	
	// ���º��� ó�� �޼ҵ�.
	void OnSelectChar();
	void OnLoginGameServer(int nCharIndex);
	void OnMoveServer();
	
	//void AddStatIndex(const PmStatIndex* statIndex, int nPCCafeID);
	//PmStatIndex* GetStatIndex();
	int GetPCCafeID();
	bool IsExistStatIndex();

	const MUID			GetUID(void) const		{ return m_UID; }
	ACCOUNT_INFO&		GetAccountInfo()		{ return m_AccountInfo; }
	PlayerObjectState	GetState()				{ return m_nState; }
	LMasterServer*		GetMasterServer()		{ return m_pConnectingMasterServer; }
	int					GetSelectedCharIndex()	{ return m_nSelectedCharIndex; }
};
