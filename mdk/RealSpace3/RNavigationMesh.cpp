#include "stdafx.h"

#include <crtdbg.h>
#include <float.h>

#include "RNavigationMesh.h"
#include "RNavigationNode.h"
#include "MFileSystem.h"
#include "RVersions.h"

namespace rs3 {

	RNavigationMesh::RNavigationMesh() : m_pStartNode(NULL), m_pGoalNode(NULL)
{
	m_nVertCount = 0;
	m_nFaceCount = 0;
	m_vertices = NULL;
	m_faces = NULL;
}

RNavigationMesh::~RNavigationMesh()
{
	Clear();
}

void RNavigationMesh::Clear()
{
	if (m_vertices != NULL) delete [] m_vertices;
	if (m_faces != NULL) delete [] m_faces;

	m_nVertCount = 0;
	m_nFaceCount = 0;
	m_vertices = NULL;
	m_faces = NULL;

	ClearNodes();
}

void RNavigationMesh::ClearNodes()
{
	for (RNodeArray::iterator itor = m_NodeArray.begin(); itor != m_NodeArray.end(); ++itor)
	{
		delete (*itor);
	}
	m_NodeArray.clear();
}

void RNavigationMesh::AddNode(int nID, const RVector& PointA, const RVector& PointB, const RVector& PointC)
{
	RNavigationNode* pNewNode = new RNavigationNode;

	pNewNode->Init(nID, PointA, PointB, PointC);
	m_NodeArray.push_back(pNewNode);
}

void RNavigationMesh::MakeNodes()
{
	if (!m_NodeArray.empty()) ClearNodes();

	for (int i = 0; i < m_nFaceCount; i++)
	{
		RVector* vp[3];
		vp[0] = &m_vertices[m_faces[i].v1];
		vp[1] = &m_vertices[m_faces[i].v2];
		vp[2] = &m_vertices[m_faces[i].v3];

		RVector temp = m_vertices[m_faces[i].v3];

		_ASSERT( (*vp[0] != *vp[1]) && (*vp[1] != *vp[2]) && (*vp[2] != *vp[1]) );
		AddNode(i, *vp[0], *vp[1], *vp[2]);		// �ݽð����
		//AddNode(i, *vp[0], *vp[2], *vp[1]);		// �ð����
	}
}

void RNavigationMesh::LinkNodes()
{
	for (RNodeArray::iterator itorA = m_NodeArray.begin(); itorA != m_NodeArray.end(); ++itorA)
	{
		RNavigationNode* pNodeA = (*itorA);
		for (RNodeArray::iterator itorB = m_NodeArray.begin(); itorB != m_NodeArray.end(); ++itorB)
		{
			if (itorA == itorB) continue;

			RNavigationNode* pNodeB = (*itorB);

			if (!pNodeA->GetLink(RNavigationNode::SIDE_AB) && pNodeB->RequestLink(pNodeA->Vertex(0), pNodeA->Vertex(1), pNodeA))
			{
				pNodeA->SetLink(RNavigationNode::SIDE_AB, pNodeB);
			}
			else if (!pNodeA->GetLink(RNavigationNode::SIDE_BC) && pNodeB->RequestLink(pNodeA->Vertex(1), pNodeA->Vertex(2), pNodeA))
			{
				pNodeA->SetLink(RNavigationNode::SIDE_BC, pNodeB);
			}
			else if (!pNodeA->GetLink(RNavigationNode::SIDE_CA) && pNodeB->RequestLink(pNodeA->Vertex(2), pNodeA->Vertex(0), pNodeA))
			{
				pNodeA->SetLink(RNavigationNode::SIDE_CA, pNodeB);
			}
		}
	}
}

RNavigationNode* RNavigationMesh::FindClosestNode(const RVector& point) const
{
	float ClosestDistance = FLT_MAX;
	float ClosestHeight = FLT_MAX;
	bool bFoundHomeNode = false;
	float ThisDistance;
	RNavigationNode* pClosestNode=NULL;

		
	for (RNodeArray::const_iterator itorNode = m_NodeArray.begin(); itorNode != m_NodeArray.end(); ++itorNode)
	{
		RNavigationNode* pNode = (*itorNode);

		if (pNode->IsPointInNodeColumn(point))
		{
			RVector NewPosition(point);
			pNode->MapVectorHeightToNode(NewPosition);

			// ���� ����� ������ ��带 ã�´�.
			ThisDistance = fabs(NewPosition.z - point.z);

			if (bFoundHomeNode)
			{
				if (ThisDistance < ClosestHeight)
				{
					pClosestNode = pNode;
					ClosestHeight = ThisDistance;
				}
			}
			else
			{
				pClosestNode = pNode;
				ClosestHeight = ThisDistance;
				bFoundHomeNode = true;
			}
		}

		if (!bFoundHomeNode)
		{
			RVector2 Start(pNode->CenterVertex().x, pNode->CenterVertex().y);
			RVector2 End(point.x, point.y);
			rline2d MotionPath(Start, End);

			RNavigationNode* pNextNode;
			RNavigationNode::NODE_SIDE WallHit;
			RVector2 PointOfIntersection;

			RNavigationNode::PATH_RESULT Result = pNode->ClassifyPathToNode(MotionPath, &pNextNode, WallHit, &PointOfIntersection);

			if (Result == RNavigationNode::EXITING_NODE)
			{
				RVector ClosestPoint3D(PointOfIntersection.x, PointOfIntersection.y, 0.0f);
				pNode->MapVectorHeightToNode(ClosestPoint3D);

				ClosestPoint3D -= point;

				ThisDistance = Length(ClosestPoint3D);

				if (ThisDistance<ClosestDistance)
				{
					ClosestDistance=ThisDistance;
					pClosestNode = pNode;
				}
			}
		}
	}
	


	return pClosestNode;



}

RVector RNavigationMesh::SnapPointToNode(RNavigationNode* pNode, const RVector& Point)
{
	RVector PointOut = Point;

	//if (!pNode->IsPointInNodeColumn(PointOut))
	{
		pNode->ForcePointToNodeColumn(PointOut);
	}

	pNode->MapVectorHeightToNode(PointOut);
	return (PointOut);
}

RVector RNavigationMesh::SnapPointToMesh(RNavigationNode** NodeOut, const RVector& Point)
{
	RVector PointOut = Point;
	*NodeOut = FindClosestNode(PointOut);
	return (SnapPointToNode(*NodeOut, PointOut));
}

bool RNavigationMesh::BuildNavigationPath(RNavigationNode* pStartNode, 
							const RVector& StartPos, RNavigationNode* pEndNode, const RVector& EndPos)
{
	m_pStartNode = pStartNode;
	m_pGoalNode = pEndNode;

	bool ret = m_AStar.Search(pStartNode, pEndNode);
	if (ret == false) return false;

	m_WaypointList.clear();
	m_WaypointList.push_back(EndPos);


	RNavigationNode* pVantageNode = NULL;
	RVector vantagePos;

	pVantageNode = pEndNode;
	vantagePos = EndPos;

	RNavigationNode* pLastNode = NULL;
	RVector lastPos;

	list<RAStarNode*>* pPath = &m_AStar.m_ShortestPath;

	bool bPushed = true;
	for (list<RAStarNode*>::iterator itor = pPath->begin(); itor != pPath->end(); ++itor)
	{
		RNavigationNode* pTestNode = (RNavigationNode*)(*itor);

		RVector testPos = pTestNode->GetWallMidPoint(pTestNode->GetArrivalLink());
		testPos = SnapPointToNode(pTestNode, testPos);

		if (LineOfSightTest(pVantageNode, vantagePos, pTestNode, testPos))
		{
			pLastNode = pTestNode;
			lastPos = testPos;
			bPushed = false;
		}
		else
		{
			_ASSERT(pLastNode != NULL);
			m_WaypointList.push_back(lastPos);
			pVantageNode = pLastNode;
			vantagePos = lastPos;
			bPushed = true;
		}
	}
	
	if (!bPushed) 
	{
		if (!LineOfSightTest(pVantageNode, vantagePos, pStartNode, StartPos))
		{
			m_WaypointList.push_back(lastPos);
		}
	}

	//m_WaypointList.push_back(StartPos);


	// ������ �ڹٲ۴�.
	m_WaypointList.reverse();

	return ret;
}

bool RNavigationMesh::BuildNavigationPath(const RVector& vStartPos, const RVector& vGoalPos)
{
	RNavigationNode* pStartNode = FindClosestNode(vStartPos);
	if (pStartNode == NULL) return false;

	RNavigationNode* pGoalNode = FindClosestNode(vGoalPos);
	if (pGoalNode==NULL) return false;

	return BuildNavigationPath(pStartNode, vStartPos, pGoalNode, vGoalPos);
}


bool RNavigationMesh::LineOfSightTest(RNavigationNode* pStartNode, const RVector& StartPos, RNavigationNode* pGoalNode, const RVector& EndPos)
{
	if ((pStartNode == NULL) || (pGoalNode == NULL)) return false;
	if (pStartNode == pGoalNode) return true;

	rline2d MotionPath(RVector2(StartPos.x,StartPos.y), RVector2(EndPos.x,EndPos.y));

	RNavigationNode* pNextNode = pStartNode;
	RNavigationNode::NODE_SIDE WallNumber;
	RNavigationNode::PATH_RESULT Result;

	while((Result = pNextNode->ClassifyPathToNode(MotionPath, &pNextNode, WallNumber, 0)) == RNavigationNode::EXITING_NODE)
	{
		if (!pNextNode) return(false);
	}


	return (Result == RNavigationNode::ENDING_NODE);
}

bool RNavigationMesh::Open(const char* szFileName)
{
	MFile file;
	if(!file.Open(szFileName)) return false;

	// header -------------
	RHEADER header;
	file.Read(&header,sizeof(RHEADER));
	if(header.dwID!=R_NAV_ID || header.dwVersion!=R_NAV_VERSION)
	{
		file.Close();
		return false;
	}

	int nVertCount,nFaceCount;

	// vertex -------------
	file.Read(&nVertCount,sizeof(int));
	InitVertices(nVertCount);
	for (int i = 0; i < nVertCount; ++i)
	{
		file.Read(&m_vertices[i], sizeof(RVector));
	}

	// face ---------------
	file.Read(&nFaceCount,sizeof(int));
	InitFaces(nFaceCount);
	for (int i = 0; i < nFaceCount; ++i)
	{
		file.Read(&m_faces[i], sizeof(RNavFace));
	}

	MakeNodes();

	// link ---------------
	for (RNodeArray::iterator itor = m_NodeArray.begin(); itor != m_NodeArray.end(); ++itor)
	{
		RNavigationNode* pNode = (*itor);

		for (int i = 0; i < 3; ++i)
		{
			int nSideIndex = -1;

			if (file.Read(&nSideIndex, sizeof(int)))
			{
				if (nSideIndex >= 0)
				{
					pNode->SetLink(RNavigationNode::NODE_SIDE(i), m_NodeArray[nSideIndex]);
				}
			}
			else
			{
				_ASSERT(0);
			}
		}
	}

	file.Close();

	return true;
}

bool RNavigationMesh::Save(const char* szFileName)
{
	if (m_nVertCount <= 0) return false;

	FILE* file = fopen(szFileName, "wb");
	if (!file) return false;

	// header -------------
	RHEADER header(R_NAV_ID, R_NAV_VERSION);
	fwrite(&header, sizeof(RHEADER), 1, file);

	// vertex -------------
	fwrite(&m_nVertCount, sizeof(int), 1, file);
	for (int i = 0; i < m_nVertCount; i++)
	{
		fwrite(&m_vertices[i], sizeof(RVector), 1, file);
	}

	// face ---------------
	fwrite(&m_nFaceCount, sizeof(int), 1, file);
	for (int i = 0; i < m_nFaceCount; i++)
	{
		fwrite(&m_faces[i], sizeof(RNavFace), 1, file);
	}

	MakeNodes();
	LinkNodes();

	// link ---------------
	for (RNodeArray::iterator itor = m_NodeArray.begin(); itor != m_NodeArray.end(); ++itor)
	{
		RNavigationNode* pNode = (*itor);

		for (int i = 0; i < 3; i++)
		{
			int nSideIndex = -1;
			if (pNode->GetLink(i))
			{
				nSideIndex = pNode->GetLink(i)->GetID();
			}
			fwrite(&nSideIndex, sizeof(int), 1, file);
		}
	}



	fclose(file);

	return true;
}


void RNavigationMesh::ClearAllNodeWeight()
{
	for (RNodeArray::iterator itor = m_NodeArray.begin(); itor != m_NodeArray.end(); ++itor)
	{
		RNavigationNode* pNode = (*itor);
		pNode->SetWeight(1.0f);
	}
}

}
