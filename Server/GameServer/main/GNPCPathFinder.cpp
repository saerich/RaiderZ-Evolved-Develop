#include "stdafx.h"
#include "GNPCPathFinder.h"
#include "GPathFinder.h"
#include "GEntityNPC.h"
#include "GConst.h"
#include "MPathCollision.h"
#include "GGlobal.h"
#include "GTestSystem.h"

GNPCPathFinder::GNPCPathFinder(GEntityNPC* pOwner)
: m_pOwner(pOwner)
, m_nZoneID(0)
, m_pPathMesh(NULL)
{

}

GNPCPathFinder::~GNPCPathFinder()
{
	Destroy();
}

void GNPCPathFinder::OnEnterToCombat()
{
	if (m_nZoneID == 0) return;
	if (m_pPathMesh == NULL) return;
	if (m_PathAgent.IsValid()) return;

	// agent ���
	gsys.pPathFinder->NewPlaceAgent(m_PathAgent, m_pOwner->GetPos(), m_nZoneID, m_pPathMesh);
}

void GNPCPathFinder::OnLeaveFromCombat()
{
	if (m_PathAgent.IsValid() == false) return;

	// agent ����
	m_PathAgent.Release();
}

bool GNPCPathFinder::MoveAgentTo( const vec3& pos )
{
	if (m_PathAgent.IsValid() == false) return false;

	return (m_PathAgent.MoveTo(pos)==PATH_SUCCESS);
}

void GNPCPathFinder::Create( int nZoneID )
{
	m_nZoneID = nZoneID;

	m_pPathMesh = gsys.pPathFinder->GetMesh(nZoneID);
}

void GNPCPathFinder::Destroy()
{
	OnLeaveFromCombat();

	m_Path.Release();
}

void GNPCPathFinder::RegisterToContext( MContext& context )
{
	if (m_PathAgent.IsValid() == false) return;

	if (m_PathAgent.GetContext())
	{
		m_PathAgent.GetContext()->RemoveAgent(m_PathAgent);
	}

	context.AddAgent(m_PathAgent);
}

void GNPCPathFinder::RevokeFromContext()
{
	if (m_PathAgent.IsValid() == false) return;

	if (m_PathAgent.GetContext())
	{
		m_PathAgent.GetContext()->RemoveAgent(m_PathAgent);
	}
}

bool GNPCPathFinder::FindPath( const vec3 vStartPos, const vec3 vDestPos, vector<vec3>& outvPathList )
{
	gsys.pTestSystem->CheckNaN(__FUNCTIONW__, vStartPos);
	gsys.pTestSystem->CheckNaN(__FUNCTIONW__, vDestPos);

	if (GConst::LOG_PATHFIND)
	{
		mlog("��ã��\n");
	}

	MContext* pCollisionContext = m_PathAgent.GetContext();
	GPathFinder* path_finder = gsys.pPathFinder;
	vec3 vValidStartPos = vStartPos;
	vec3 vValidDestPos = vDestPos;

	outvPathList.clear();

	// +-XY ���� +-Z �������� ���� �������� ã���ִ� ���̴�. ���� +- �ִ� ������ ���� 25~30������ �����ؾ��ҵ�
	// �� ���϶� ������ ����� ���� �� ������ ��Ȯ���� �ʴ� ��찡 ���� �� �����ϱ� ...
	// context - null �� �ؼ� �˻��ؾ��Ѵ�. �׷��� ������, ������ = �ڱ��ڽ��� ��ġ = �ڽ��� agent collision ���� �Ǿ ..
	if (!path_finder->IsValidPosition(m_pPathMesh, vStartPos))
	{
		if (GConst::LOG_PATHFIND)
		{
			mlog("\t��ã�����-������ġ\n");
		}

		//vec3 vNearPos;
		//if ( path_finder->FindNearPosition(&vNearPos, 
		//	m_pPathMesh, 
		//	pCollisionContext, 
		//	vStartPos, 
		//	GConst::PATH_NEAR_HORIZONE_RANGE, 
		//	GConst::PATH_NEAR_VERTICAL_RANGE) == PATH_SUCCESS)
		//{
		//	vec3 vDir1 = vNearPos - vStartPos;
		//	vDir1.Normalize();
		//	vec3 vDir2 = vDestPos - vStartPos;
		//	vDir2.Normalize();

		//	if (vDir1.DotProduct(vDir2) > 0.0f)
		//	{
		//		vValidStartPos = vNearPos;
		//		outvPathList.push_back(vValidStartPos);
		//	}
		//}

		//if (GConst::LOG_PATHFIND)
		//{
		//	mlog("\t��ã��-������ġ��ȿ(������): %.1f %.1f %1.f -> %.1f %.1f %.1f\n", 
		//		vStartPos.x, vStartPos.y, vStartPos.z,
		//		vValidStartPos.x, vValidStartPos.y, vValidStartPos.z
		//		);
		//}

//		return true;
	}
	
	//if (path_finder->IsValidPosition(NULL, m_pPathMesh, pCollisionContext, vDestPos) != PATH_SUCCESS)
	if (!path_finder->IsValidPosition(m_pPathMesh, vDestPos))
	{
		if (GConst::LOG_PATHFIND)
		{
			mlog("\t��ã�����-��ǥ��ġ\n");
		}

//		return true;
		//path_finder->FindNearPosition(&vValidDestPos, 
		//	m_pPathMesh, 
		//	pCollisionContext, 
		//	vDestPos, 
		//	GConst::PATH_NEAR_HORIZONE_RANGE, 
		//	GConst::PATH_NEAR_VERTICAL_RANGE);

		//if (GConst::LOG_PATHFIND)
		//{
		//	mlog("\t��ã��-��ǥ��ġ��ȿ(������): %.1f %.1f %1.f -> %.1f %.1f %.1f\n", 
		//		vDestPos.x, vDestPos.y, vDestPos.z,
		//		vValidDestPos.x, vValidDestPos.y, vValidDestPos.z);
		//}
	}

	if (!m_PathAgent.IsValid())
	{
		if (GConst::LOG_PATHFIND)
		{
			mlog("\t��ã��-������Ʈ ����\n");
		}

		VALID_RET(m_pOwner->GetFieldInfo(), false);
		if (!path_finder->NewPlaceAgent(m_PathAgent, vValidStartPos, m_nZoneID, m_pPathMesh))
		{
			if (GConst::LOG_PATHFIND)
			{
				mlog("\t��ã�����-������Ʈ ���� ����\n");
			}
		}
	}

	m_Path.Release();
	GPathFinder::QueryResult nResult = path_finder->FindPath(m_Path, m_pPathMesh, m_PathAgent, vValidDestPos);

	if (nResult == GPathFinder::QUERY_SUCCESS || nResult == GPathFinder::QUERY_SUCCESS_BUT_ABORTED)
	{
		if (m_Path.IsValid())
		{
			PathTo3DPoints(outvPathList);
			if (GConst::LOG_PATHFIND)
			{
				mlog("\t��ã�� ���� (����: %d)\n", outvPathList.size());
			}
		}
		else
		{
			if (GConst::LOG_PATHFIND)
			{
				mlog("\t��ã�����-��ȿ�� ���\n");
			}
		}
	}
	else if (nResult == GPathFinder::QUERY_FAILED)
	{
		if (GConst::LOG_PATHFIND)
		{
			mlog("��ã�� ���� (QUERY_FAILED)\n");
		}

		path_finder->FindPath_NoAgent(m_pPathMesh, vStartPos, vDestPos, outvPathList);	
	}

	return true;
}

void GNPCPathFinder::PathTo3DPoints( std::vector<MVector3> &out3DPoints, int offset /*= 0*/ )
{
	gsys.pPathFinder->PathTo3DPoints(out3DPoints, m_pPathMesh, m_Path, offset);
}

void GNPCPathFinder::PathTo3DPoint( MVector3 &outPos, int index )
{
	gsys.pPathFinder->PathTo3DPoint(outPos, m_pPathMesh, m_Path, index);
}

int GNPCPathFinder::GetGroupAgentCount()
{
	if (m_PathAgent.IsValid() && m_PathAgent.GetContext() && m_PathAgent.GetContext()->IsValid())
	{
		return m_PathAgent.GetContext()->GetAgentNum();
	}
	return 1;
}

float GNPCPathFinder::GetHeightAtPosition( vec3& vPos )
{
	GPathFinder* path_finder = gsys.pPathFinder;

	SPathMesh *pPMesh = path_finder->GetMesh(m_nZoneID);
	if ( pPMesh == NULL || pPMesh->m_pMesh == NULL) return vPos.z;

	iMesh* mesh = pPMesh->m_pMesh;

	cPosition cPos;
	cPos = mesh->positionFor3DPointF((const float*)&vPos);

	if (cPos.cell == -1) return vPos.z;

	return mesh->heightAtPositionF(cPos);
}


