#pragma once

class GEntityNPC;
struct SPAWN_INFO;

//////////////////////////////////////////////////////////////////////////
//
// GNPCSerializer
//		- ���� Reload�� NPC�� ���Ӱ� ������ �����ϱ� ���� �뵵�� ����
//		- Actor ������ �ø������¡�� �ʿ��� ���� ��ӱ����� ��������
//		- Field Instance�� ������� �ʴ´ٴ� �������� ����Ǿ���
//
//////////////////////////////////////////////////////////////////////////

class GNPCSerializer
{
	struct Info
	{
		int nNPCID;
		MUID uidField;
		vec3 vPos;
		vec3 vDir;
		vec3 vFacingDir;
		const SPAWN_INFO* pSpawnInfo;
	};
public:
	GNPCSerializer(void);
	~GNPCSerializer(void);

	void SaveFromInstance( GEntityNPC* pNPC, bool bDelete=true );
	void SaveFromManager(int nNPCID = INVALID_ID, bool bDelete=true );
	void RestoreAll();
private:
	vector<Info> m_vecNPCs;
};

