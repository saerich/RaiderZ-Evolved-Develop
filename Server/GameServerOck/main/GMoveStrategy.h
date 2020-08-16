#pragma once

class GEntityNPC;

//////////////////////////////////////////////////////////////////////////
//
//		GMoveStrategy
//
class GMoveStrategy
{
protected:
	// �̵� ����
	GEntityNPC*		m_pOwner;
	vec3			m_vGoalPos;
	vec3			m_vGoalDir;
	float			m_fLeastGap;
	vec3			m_vOldPos;

protected:
	GEntityNPC* GetOwnerNPC();
	vec3	GetGoalPos();
	vec3	GetGoalDir();
	bool	IsReachPos(const vec3& vDestPos, float fDelta);	
	bool	IsReachGoalPos(float fDelta);
	
	bool	SetValidateZPos(vec3& vResultPos);
	
	void	RouteMoveStop();
	void	RouteMove(vec3 vDestPos);
public:
	GMoveStrategy(GEntityNPC* pOwner, float fLeastGap);
	virtual ~GMoveStrategy();
	
	/// �������۽� ȣ��Ǵ� �Լ�.  ��ȯ���� ������ �������� ���� (false�� ����)
	virtual bool		Start(vec3 vGoalPos, vec3 vGoalDir, float fLeastGap);

	/// ���� ������Ʈ �Լ�.  ��ȯ���� ��� ó���� ����ƴ��� ����
	virtual void		Update(float fDelta)=0;

	bool	IsPathfindFailed();	
	bool	IsReachGoalPos();	

	virtual GTaskMoveStrategy GetType() = 0;
};

//////////////////////////////////////////////////////////////////////////
//
//		GDefiniteMoveStrategy (������ �̵�)
//

class GDefiniteMoveStrategy : public GMoveStrategy, public MTestMemPool<GDefiniteMoveStrategy>
{
protected:
	int				m_nMoveIndex;
	vec3			m_vWaypoint;
	vec3			m_vNextDir;
	vector<vec3>	m_vecWaypoints;
protected:
	bool	FindPath(  vec3 vStartPos, vec3& vDestPos);
	bool	IsReachWaypoint(float fDelta);
	bool	IsEndOfWaypoint();
	void	SetNextWaypoint();

	void SetNextWaypointDir();
	void	RouteDebugPathInfo();
	void	RouteMoveNextWayPoint();
public:
	GDefiniteMoveStrategy(GEntityNPC* pOwner, float fLeastGap);
	virtual ~GDefiniteMoveStrategy();

	/// �������۽� ȣ��Ǵ� �Լ�.  ��ȯ���� ������ �������� ���� (false�� ����)
	virtual bool		Start(vec3 vGoalPos, vec3 vGoalDir, float fLeastGap);

	/// ���� ������Ʈ �Լ�.  ��ȯ���� ��� ó���� ����ƴ��� ����
	virtual void		Update(float fDelta);		

	virtual GTaskMoveStrategy GetType() { return MS_DEFINITE; }
	
};

//////////////////////////////////////////////////////////////////////////
//
//		GStraightMoveStrategy (��ã�� ���� ���� �̵�)
//

class GStraightMoveStrategy : public GMoveStrategy, public MTestMemPool<GStraightMoveStrategy>
{
public:
	GStraightMoveStrategy(GEntityNPC* pOwner, float fLeastGap);
	virtual ~GStraightMoveStrategy();

	/// �������۽� ȣ��Ǵ� �Լ�.  ��ȯ���� ������ �������� ���� (false�� ����)
	virtual bool		Start(vec3 vGoalPos, vec3 vGoalDir, float fLeastGap);

	/// ���� ������Ʈ �Լ�.  ��ȯ���� ��� ó���� ����ƴ��� ����
	virtual void		Update(float fDelta);		

	virtual GTaskMoveStrategy GetType() { return MS_STRAIGHT; }
	
};




