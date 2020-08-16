#pragma once

class GLuaInfoMgr
{
	// Calling Lua 
	// ��ƿ��� ����� id�� �� NPC���� ���� �����Ѵ�
	// C++���� NPC��ü�� Field ������ �и��Ǿ� ���������� ��ƿ��� ���������� �����ǹǷ�
	// id�� �ʵ� ���������� �����
typedef map<int, int>			ID_MANAGER_MAP;
typedef map<int, vector<int>>	ID_CONTAINER_MAP;
typedef map<int, MUID>			ID_PAIR_MAP;
typedef map<int, ID_PAIR_MAP>	ID_PAIR_CONTAINER_MAP;

	ID_MANAGER_MAP				m_mapNPCIDMgr;				// <npcid, lua id ����>
	ID_CONTAINER_MAP			m_mapNPCIDContainer;		// <npcid, npcid�� �ش��ϴ� npc���� id vector>
	ID_PAIR_CONTAINER_MAP		m_mapNPCIDPairContainer;	// <npcid, luaid-muid map>


public:
	GLuaInfoMgr(void);
	~GLuaInfoMgr(void);

	static GLuaInfoMgr& GetInstance();

	void	InitNPC(int nNPCId);
	int		AddNPC(int nNPCId, const MUID uid);
	void	RemoveNPC(int nNPCId, int nLuaId);

	MUID	GetNPCUID(int nNPCID, int nLuaID);
};
