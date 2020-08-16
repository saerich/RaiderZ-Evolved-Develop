#pragma once

class iCollisionInfo;
class iCollisionContext;
class MPathFinder;
class MPath;
class MAgent;

/**
	All iCollisionInfo objects relating to a given mesh get destroyed automatically when that mesh is destroyed. 
	�̹Ƿ� �Ҹ��ڿ��� m_pCollisionInfo�� �������� �ʴ´�.
*/
class MCollisionInfo
{
	friend MPathFinder;
	friend MPath;
	

	MCollisionInfo operator =(const MCollisionInfo &collisioninfo);
	MCollisionInfo(MCollisionInfo &collisionInfo);
public:
	MCollisionInfo();
	virtual ~MCollisionInfo();

	bool IsValid()
	{
		return ( m_pCollisionInfo ? true : false );
	}
	void Release(); // ����� ����, �ݵ�� ��������� Release ȣ��
	
	MAgent* GetCollisionAgent();
	iCollisionInfo *m_pCollisionInfo;
};

/**
*/
class MContext
{
	friend MPathFinder;
	friend MAgent;

	MContext operator =(const MContext &context);
	MContext(const MContext &context);
public:
	iCollisionContext *m_pContext;

	MContext();
	virtual ~MContext();

	bool IsValid()
	{
		return ( m_pContext ? true : false );
	}

	void Release();
	/// AddAgent, RemoveAgent�� bool ���� true�̸�, ���������� �Լ��� ����Ǿ����� �ǹ��Ѵ�.
	bool AddAgent(MAgent &agent);
	bool RemoveAgent(MAgent &agent);

	int GetAgentNum();

	iCollisionContext* GetCollisionContext()
	{
		return m_pContext;
	}
};
