#pragma once

#include "../../../sdk/pathengine/i_pathengine.h"
#include "MVector3.h"
#include "MVector2.h"

class MPathFinder;
class MPath;
class MCollisionInfo;
class MContext;

/**
 MAgent�� iAgent�� �����Ű�� ���� ó������ ����� ���� �� ���� ������ ,
 iAgent�� setUserData�� ��ü�� �Ǵ� MAgent�� Ŭ���� �����͸� �����������μ� , ���� ������ �����ϴ�.

 Ȥ�� MAgent�� ���� ���� ������ �����͸� �ް� ������ MAgent�� Ȯ���϶�.

 All agents placed on a mesh get destroyed automatically when the mesh is destroyed. 
 
 �̹Ƿ� MAgent�� �Ҹ��ڿ��� Release�� ȣ������ ���Ѵ�.
*/

enum PATHENGINE_RESULT_TYPE;
class MAgent
{
	friend MPathFinder;
	friend MContext;

private:
	int m_nMeshID;

	bool AddToContext(MContext *pContext);
	bool RemoveFromContext(MContext *pContext);
	
	// link-error ���� ���� ����. = ������ �� ���� �����ڴ� ����� �� ����.
	MAgent operator =(const MAgent &agent);
	MAgent(const MAgent &agent);
public:
	iAgent *m_pAgent;
	MContext *m_pContext;

public:
	MAgent();
	virtual ~MAgent();
	virtual void Release();		// ����� ����, �ݵ�� ��������� Release ȣ��

	bool GetPosition(MVector3 *pOut) const;

	/**
	 Agent�� ������ ��ġ�� �����ϴ� �Լ�
	 @param pos			3D ��ǥ ��ġ
	 @param bForceMove	context�� Ȱ��ȭ ��ų ������ �ƴ��� ����, �⺻ �� false�� context�� Ȱ��ȭ ��Ų��.
	 @return PathEngine PATH_ENGINE_RESULT ��� ��
	*/
	PATHENGINE_RESULT_TYPE MoveTo(const MVector3 &pos, bool bForceMove);
	PATHENGINE_RESULT_TYPE MoveTo(const MVector3 &pos);
	void MakeCurvePath(MPath &basePath, int sectionLength = 300, float turnRatio1 = 0.3f, float turnRation2 = 0.4f);

	bool IsInContext() const;

	bool IsValid() const
	{
		return (m_pAgent != NULL ? true : false);
	}

	void SetMeshID(int nID) { m_nMeshID = nID; }
	void SetTraverseCost(float fCost);
	float GetTraverseCost();
	MContext* GetContext() const { return m_pContext; }
};

