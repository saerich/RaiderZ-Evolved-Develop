#include "stdafx.h"
#include "XMoveCollision.h"
#include "MMath.h"
#include "RCollisionTree.h"
#include "XGameState.h"
#include "XCollisionTree.h"

//#ifdef _DEBUG
#include "RDebugPassRenderer.h"
#include "RCameraSceneNode.h"
//#endif

//const float SLIDING_LIMIT = 0.76f;	// AXISZ.dot(poly.normal) �� ��ġ. �� �̻��̸� �̲�������.
//const float	HOVER_HEIGHT = 20.0f;	// �̸�ŭ ����� �̵��Ѵ�. �� ���� + Radius ������ ���� �Ѿ.
//const float	STEEPPLANE_CUTOFF_HEIGHT = 50.0f;	// ���ĸ� �����̸鼭 �̰ͺ��� ���̰� ���� �������� �߶󳻹�����.
#define BUILD_RECT_SIZE 1000.0f
//////////////////////////////////////////////////////////////////////////
XMoveCollision::XMoveCollision()
{
	m_bMoveWater		= false;
	m_bRestrictedArea	= false;

	m_fMin_x			= 0.0f;
	m_fMin_y			= 0.0f;
	m_fMax_x			= 0.0f;
	m_fMax_y			= 0.0f;

	m_strMaterialName.clear();

	m_pvecTerrainColObjectPolygon	= NULL;
}

XMoveCollision::~XMoveCollision()
{
}

void XMoveCollision::SetRadius( float fRadius )
{
	m_fRadius = fRadius;

	m_CollisionDebug.SetRadius(fRadius);
}

float XMoveCollision::GetRadius()
{
	return m_fRadius;
}

void XMoveCollision::SetMoveAllowedHeight( float fHeight )
{
	m_fMoveAllowedHeight = fHeight;
}

float XMoveCollision::GetMoveAllowedHeight()
{
	return m_fMoveAllowedHeight;
}

bool XMoveCollision::IsIntersectMovingSphereTriangle( const MVector3 &from,const MVector3 &to,const float fRadius,const MVector3 &v0,const MVector3 &v1,const MVector3 &v2, MVector3 &outNearest, int &debugFlag )
{
	const float INTERSECT_TOLER	= 0.1f;

	// �ﰢ���� �����ϴ� ���
	MPlane plane(v0,v1,v2);

	// ����� �������ΰ�� �˻����� �ʴ´�.
	float fDistFromPlane = plane.GetDistanceTo(from);
	if(fDistFromPlane<0) return false;

	float fDistToPlane = plane.GetDistanceTo(to);
	if(fabs(fDistToPlane-fDistFromPlane) < INTERSECT_TOLER ) return false;	// �����ϰ� �̵��ϸ� �˻����� �ʴ´�.
	if(fDistToPlane>fDistFromPlane) return false;	// �־����� �����̸� �˻����� �ʴ´�.

	// 1. from-to ������ ���� sphere�� �ﰢ�������� �����϶� �浹�ϴ� ���� �ﰢ�� ���ο� ���̴� ���

	//if(fDistFromPlane < fRadius) // �̹� �浹���ִ� 
	//{
	//	outNearest = GetNearestPoint(from,plane);
	//	return true;
	//}

	MVector3 nearestOnFromTo;
	MVector3 planeIntersectionPoint;
	MVector3 sphereIntersectionPoint;

	if(fDistFromPlane < fRadius) // �̹� �浹���ִ� ( plane embedded )
	{
		planeIntersectionPoint = MMath::GetNearestPoint(from,plane);
		nearestOnFromTo = planeIntersectionPoint + fRadius*plane.Normal();
	}
	else
	{
		vec3 vMoveDir = to - from;
		vec3 vSphereFace = from - (plane.Normal() * fRadius);

		// from-to �������� ��鿡 ���尡��� ��
		nearestOnFromTo = MMath::GetNearestPoint(vSphereFace, vSphereFace + vMoveDir, plane);

		// nearestPtOnFromTo �� �ﰢ������� �Ÿ��� �������� �ʰ��ϸ� �ε������� ����.
		if(plane.GetDistanceTo(nearestOnFromTo)>fRadius) return false;

		// nearestPtOnFromTo�� ���� ����� ������� �� ( = from-to ���а� ���� ����� �ﰢ�� ������� ��)
		planeIntersectionPoint = MMath::GetNearestPoint(nearestOnFromTo,plane);
		nearestOnFromTo = planeIntersectionPoint + fRadius*plane.Normal();
	}

	// �� ���� �ﰢ�� �ȿ� �ִ��� �˻�
	// ������ edge�� �����ϰ� ������ �� ������ �ٶ󺸴� ���鿡 ���� ��� + �����̸�
	if(MPlane(CrossProduct(plane.Normal(),v1-v0),v0).GetDistanceTo(planeIntersectionPoint)>0 &&
		MPlane(CrossProduct(plane.Normal(),v2-v1),v1).GetDistanceTo(planeIntersectionPoint)>0 &&
		MPlane(CrossProduct(plane.Normal(),v0-v2),v2).GetDistanceTo(planeIntersectionPoint)>0 ) 
	{ // nearestPtOnPlane�� �ﰢ�� ������ ���̴�
		vec3 vOrgDir = (to - from).Normalize();
		vec3 vRealDir = (nearestOnFromTo - from).Normalize();

		if(LengthSq(nearestOnFromTo-from)<LengthSq(to-from) ||
			vOrgDir.DotProduct(vRealDir) < 0 )						// Ȥ�� �ڷ� �����ϱ�?
		{
			outNearest = planeIntersectionPoint;
			debugFlag = 0;
			return true;
		}
	}
	//	*/




	// 2. from-to �� ���� ����� ���� �ﰢ���� edge�� ���̴°��

	// ���� ����� �ﰢ���� edge���� ���� ã�´� = polygonIntersectionPoint.
	float fNearest = FLT_MAX;
	float fDistToEdge;
	MVector3 polygonIntersectionPoint;
	MVector3 edgePoint;

	edgePoint = MMath::GetNearestPointOnLine(planeIntersectionPoint,v0,v1);
	fDistToEdge = (edgePoint -  planeIntersectionPoint).Length();
	if(fDistToEdge < fNearest) { fNearest = fDistToEdge; polygonIntersectionPoint = edgePoint; }
 
	edgePoint = MMath::GetNearestPointOnLine(planeIntersectionPoint,v1,v2);
	fDistToEdge = (edgePoint -  planeIntersectionPoint).Length();
	if(fDistToEdge < fNearest) { fNearest = fDistToEdge; polygonIntersectionPoint = edgePoint; }

	edgePoint = MMath::GetNearestPointOnLine(planeIntersectionPoint,v2,v0);
	fDistToEdge = (edgePoint -  planeIntersectionPoint).Length();
	if(fDistToEdge < fNearest) { fNearest = fDistToEdge; polygonIntersectionPoint = edgePoint; }

	// polygonIntersectionPoint�������� sphere�� reverse-intersection 
	float colDistance;
	MVector3 revDir = Normalize(from-to);

	// �־����� �����̸� �˻����� �ʴ´�
	if(DotProduct(revDir,(polygonIntersectionPoint-from))>- 0.001f) return false;

	if(MMath::IsIntersectRaySphere( polygonIntersectionPoint, revDir, from, fRadius, colDistance)) {
		if(colDistance < (to-from).Length()) {
			outNearest = polygonIntersectionPoint;
			debugFlag = 1;
			return true;
		}
	}

	return false;
}

bool XMoveCollision::IsCollidable( const MBox &box )
{
	return m_bbMovement.ContainsAABB(box);
}

bool XMoveCollision::CheckPolygon( const MVector3 &lv0,const MVector3 &lv1,const MVector3 &lv2, const DWORD dwAttribute /*= 0*/ )
{
	MPlane plane(lv0,lv1,lv2);

	MVector3 dir = m_destination - m_source;
	dir.Normalize();

	// �̵����⿡ ���� backface �̸� üũ���� �ʴ´�.
	if(plane.DotNormal(dir)>0) return false;

	int nDebugFlag = -1;
	MVector3 nearestPoint;
	if(IsIntersectMovingSphereTriangle(m_source,m_destination,m_fRadius,lv0,lv1,lv2,nearestPoint,nDebugFlag)) 
	{
		vec3 vPlaneNor = plane.Normal();
		vPlaneNor.Normalize();
		m_colPolygons.push_back(RCollisionPolygon(lv0, lv1, lv2, nearestPoint, vPlaneNor, nDebugFlag, dwAttribute));
		return true;
	}

	return false;
}

bool XMoveCollision::CheckTangency( const MVector3& vPos, const RCollisionPolygon& poly )
{
	float fDist = (vPos - poly.nearestPoint).Length();
	return (fDist <= GetRadius() + 0.01f);	// 0.01�� FP������ ���� ����
}

bool XMoveCollision::CheckAllowedHeight( const MVector3& v1, const MVector3& v2, const MVector3& v3, const MVector3* pOrgFrom, const float* pAllowedHeight )
{
	if(pOrgFrom == NULL || pAllowedHeight == NULL)
		return false;

	// float���� ��� ���������� �ణ�� +���� �����Ѵ�.
	if((v1.z - pOrgFrom->z) <= *pAllowedHeight + 0.1f &&
		(v2.z - pOrgFrom->z) <= *pAllowedHeight + 0.1f &&
		(v3.z - pOrgFrom->z) <= *pAllowedHeight + 0.1f)
		return true;

	return false;
}

void XMoveCollision::GetBoundingBox( MBox &outBox )
{
	outBox.minx = outBox.miny = outBox.minz = -m_fRadius;
	outBox.maxx = outBox.maxy = outBox.maxz = m_fRadius;
}

float XMoveCollision::GetWalkableDistance( const RCollisionPolygon &collisionPolygon )
{
	return GetMovableDistance(m_source, m_destination, collisionPolygon.nearestPoint);
}

float XMoveCollision::GetMovableDistance( const MVector3 &from,const MVector3 &to,const MVector3 &collisionPoint )
{
	// (reverse-intersecting the sphere) 
	// sphere �� source->destination ���� �̵��ϴ� ���� collisionPoint �ɸ��� ������ ã�� ���̹Ƿ�
	// collisionPoint�� �߽�, ���� �������� ���� sphere �� source->destination ������ �����ϴ� ������ ����.


	MVector3 dir = to - from;
	float fDistance = dir.Length();
	dir.Normalize();		

	float fCollisionDistance;
	if(MMath::IsIntersectRaySphere(m_source, dir,collisionPoint,m_fRadius,fCollisionDistance)) {
		const float CONTACTTOLER = 0.01f;

		if(fabs(fCollisionDistance)<CONTACTTOLER) fCollisionDistance = 0;	// ���������� �� �����Ҽ� ����
		return min(fCollisionDistance , fDistance);
	}



	// �浹���� ������ ������ �����ִ�.
	return fDistance;
}

MVector3 XMoveCollision::GetContactPoint( const MVector3& vPos, const MVector3& vNorm )
{
	return vPos - (vNorm * (m_fRadius - 10));
}

void XMoveCollision::GetCollisionPolygon( const MVector3 &from,const MVector3 &to, bool bIgnoreCheckHeight )
{
	MVector3 ret;

	// ������-���� ����
	m_source = from;
	m_destination = to;

	// �̵� ������ ���� �浹 �������� �����´�.
	MVector3 vDir = m_destination - m_source;
	float fDist = vDir.Length();
	vDir.Normalize();

	m_colPolygons.clear();

	CalcBoundingBox();

	m_pTargetCollisionTree->GetCollidablePolygons(*this, ~RCollisionTriangle::WATER);

	GetTerrainColObjectCollisionPolygon();

	// �ɸ��� �������� ������
	if(m_colPolygons.empty() == false) 
	{
		float fMinMove = fDist;

		for(int i=0; i<(int)m_colPolygons.size(); i++ )
		{
			RCollisionPolygon &poly = m_colPolygons[i];


			bool bCheck = CheckAllowedHeight(poly.v[0], poly.v[1], poly.v[2], &m_vOrgfrom, &m_fMoveAllowedHeight);

			if(bCheck == false || bIgnoreCheckHeight == true)
			{
				// �� ������ ������ ���������� �����ִ� �ִ� �Ÿ�
				float fMove = GetMovableDistance(m_source, m_destination, poly.nearestPoint);

				// �ּҰ��� ã�´�
				if(fDist > fMove) 
				{
					fMinMove = fMove;
		
					//if(m_bMoveWater)
					//	ReBuildWaterCollisionPolygon(poly);
					//else
						ReBuildGroundCollisionPolygon(poly);
				}
			}
		}
	}
}

bool XMoveCollision::MoveTestFront( const MVector3 &from,const MVector3 &to, MVector3& vOut, bool bUpCol )
{
	vec3 ret;
	bool bMoveCol = false;

	vec3 source = from;
	vec3 destination = to;

	for(int itr = 0; ; itr++)
	{
		MVector3 vDir = destination - source;
		float fDist = vDir.Length();
		vDir.Normalize();

		m_vecFrontColPolygon.clear();
		vec3 vTestSource = m_source;
		for(vector<vec3>::iterator itSphere = m_vecCollisionSphere.begin(); itSphere != m_vecCollisionSphere.end(); itSphere++)
		{
			(*itSphere).x = m_source.x;
			(*itSphere).y = m_source.y;

			// ����
			vec3 vTempTo = (*itSphere) + (vDir * fDist);
			GetCollisionPolygon((*itSphere) - vDir, vTempTo,  bUpCol);
		}

		GetRestrictedAreaPoly(source, destination, m_vecFrontColPolygon);

		m_source = source;
		m_destination = destination;

		float fMinMove = fDist;
		RCollisionPolygon* pPlaneNear = NULL;

		if(m_vecFrontColPolygon.empty())
		{
			ret = m_destination;
			break;
		}

		vec3 vNearPos = vec3(0, 0, 0);
		for(vector<RCollisionPolygon>::iterator itPolygon = m_vecFrontColPolygon.begin(); itPolygon != m_vecFrontColPolygon.end(); itPolygon++)
		{
			vec3 vNearPoint;

			float fDot = vDir.DotProduct((*itPolygon).normal);
			if(fDot < 0)
			{
				int nTemp;
				if(IsIntersectMovingSphereTriangle(m_source, m_destination, m_fRadius, (*itPolygon).v[0], (*itPolygon).v[1], (*itPolygon).v[2], vNearPoint, nTemp))
				{
					// �� ������ ������ ���������� �����ִ� �ִ� �Ÿ�
					float fMove = GetMovableDistance(m_source, m_destination, vNearPoint);

					// �ּҰ��� ã�´�
					if(fMinMove > fMove) 
					{
						fMinMove = fMove;
						pPlaneNear = &(*itPolygon);
						vNearPos = vNearPoint;
						bMoveCol = true;
						m_CollisionDebug.AddCollisionInvestigateData_Front(*itPolygon);
					}
				}
				else
					m_CollisionDebug.AddCollisionInvestigateData_CheckFront(*itPolygon);
			}
		}

		ret = m_source + (vDir * fMinMove);

		if(pPlaneNear)
		{
			// �����̵��� ������ �븻�ΰ�?
			//vec3 vMoveDir = vDir;
			//vMoveDir.z = 0.0f;
			//vMoveDir.Normalize();
			//pPlaneNear->normal.Normalize();
			//float fDot = pPlaneNear->normal.DotProduct(vMoveDir);
			if(itr == 0)
			{
				// �����̵��� �ؾ� �Ѵ�.
				//bSliding = true;

				vec3 vPlaneSlidingPos = m_destination + pPlaneNear->normal.DotProduct(ret - m_destination) * pPlaneNear->normal;
				source = ret;
				destination = vPlaneSlidingPos;
			}
			else
			{
				// �����̵� ���̻� ����
				break;
			}
		}
		else
		{
			break;
		}
	}

	vOut = ret;

	return bMoveCol;
}

bool XMoveCollision::MoveTestDown( const MVector3 &from,const MVector3 &to, MVector3& vOut, MVector3& vOutNor, bool& bSliding )
{
	MVector3 ret;
	MVector3 nor;
	bool bMoveCol = false;

	// ������-���� ����
	m_source = from;
	m_destination = to;
	nor.Set(vec3::ZERO);

	RCollisionPolygon* pPolygonTest = NULL;
	for(int itr = 0; ; itr++)
	{
		// �̵� ������ ���� �浹 �������� �����´�.
		MVector3 vDir = m_destination - m_source;
		float fDist = vDir.Length();
		vDir.Normalize();

		m_colPolygons.clear();

		CalcBoundingBox();

		m_pTargetCollisionTree->GetCollidablePolygons(*this, ~RCollisionTriangle::WATER);

		GetRestrictedAreaPoly(m_source, m_destination, m_colPolygons);

		// �ɸ��� �������� ������
		if(m_colPolygons.empty()) 
		{
			// �ɸ��µ��� ��3
			ret = m_destination;
			//bFreeMove = true;
			break;
		}
		else
		{
			float fMinMove = fDist;

			RCollisionPolygon* pPolygon = NULL;

			for(int i=0; i<(int)m_colPolygons.size(); i++ )
			{
				RCollisionPolygon &poly = m_colPolygons[i];

				m_CollisionDebug.AddDownDebugColTestPolygon(poly);

				// �� ������ ������ ���������� �����ִ� �ִ� �Ÿ�
				float fMove = GetMovableDistance(m_source, m_destination, poly.nearestPoint);

				// �ּҰ��� ã�´�
				if(fMinMove>fMove) 
				{
					fMinMove = fMove;
					pPolygon = &poly;
					bMoveCol = true;
					pPolygonTest = pPolygon;
					m_CollisionDebug.AddCollisionInvestigateData_Down(poly);
				}
			}

			// �ϴ� ������ �Ÿ���ŭ ������
			ret = m_source + vDir * (fMinMove);
			bSliding = false;

			if(pPolygon)
			{
				nor = pPolygon->normal;
				float fDot = pPolygon->normal.DotProduct(vec3::AXISZ);

				if(fDot <= SLIDING_LIMIT &&
					CheckAllowedDownSliding(pPolygon->v[0], pPolygon->v[1], pPolygon->v[2]))		// �����̵��� ������ ���ΰ�?
				{
					// �����̵��� �ؾ� �Ѵ�.
					bSliding = true;

					MPlane plane(pPolygon->normal, pPolygon->nearestPoint);
					vec3 vPlaneSlidingPos = m_destination + plane.DotNormal(ret- m_destination) * plane.Normal();
					m_source = ret;
					m_destination = vPlaneSlidingPos/* + plane.Normal() * m_fRadius*/;
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	}

	if(gg.currentgamestate && pPolygonTest)
	{
		m_CollisionDebug.AddDownDebugColPolygon(*pPolygonTest);

		// ���͸��� ���� ��������
		vec3 vCheckPos = ret;
		if(pPolygonTest->nDebugFlag == 1)
		{
			vCheckPos = pPolygonTest->v[0] + (((pPolygonTest->v[2] + ((pPolygonTest->v[1] - pPolygonTest->v[2]) / 2.0f)) - pPolygonTest->v[0]) / 2.0f);
		}

		m_strMaterialName = gg.currentgamestate->GetWorld()->GetMaterialName(vCheckPos);

		// ���� ��ġ ���� �븻���� �����´�. ���� �ִ� �ٴ��� �븻���� �ƴ� �� �ִ�.
		gg.currentgamestate->GetWorld()->PickCollision(ret, vec3(ret.x, ret.y, to.z - 500.0f ), NULL, &vOutNor);
		vOutNor.Normalize();
	}

	vOut = ret;

	return bMoveCol;
}

void XMoveCollision::GetCollisionSphere( MCapsule capsuleCollision, float fUpValue, vector<vec3>& vecSphereCenter )
{
	vecSphereCenter.clear();

	// ������ �־�� �ϴ� ĸ��
	vec3 vCapseuleUpDwonDir = (capsuleCollision.top - capsuleCollision.bottom).Normalize();
	vec3 vUpSphere = capsuleCollision.top - (vCapseuleUpDwonDir * (m_fRadius - capsuleCollision.radius));
	vec3 vDownSphere = capsuleCollision.bottom + (vCapseuleUpDwonDir * (m_fRadius - capsuleCollision.radius));

	vDownSphere.z += fUpValue;

	vecSphereCenter.push_back(vDownSphere);

	float fCenterHeight = (vUpSphere - vDownSphere).Length();
	if(fCenterHeight <= 0)
		return;

	int nSphereCount = (int)(fCenterHeight / (m_fRadius * 2));

	if(nSphereCount <= 0)
		return;

	if(fCenterHeight - (nSphereCount * m_fRadius * 2) >= 0)
		nSphereCount++;

	nSphereCount++;

	float fHeightGap = fCenterHeight / nSphereCount;
	vec3 vSphereCenter = vDownSphere;
	for(int i = 0; i < nSphereCount; i++)
	{
		vSphereCenter += (vCapseuleUpDwonDir * fHeightGap);
		vecSphereCenter.push_back(vSphereCenter);
	}
}

bool XMoveCollision::GroundMoveTest( const MVector3 &from,const MVector3 &to, MVector3 orgForm, MCapsule capsuleCollision, MVector3& vOut, MVector3& vOutNor, bool *pSliding /*= NULL*/, bool *pFrontCol /*= NULL*/ )
{
	SetMapCollisionInfo();

	m_bMoveWater = false;
	m_strMaterialName.clear();

	// �̵� �߿� ��򰡿� �ɷ����� False, �ƴϸ� True
	m_vecFrontColPolygon.clear();

	m_CollisionDebug.Init();

	m_vOrgfrom = orgForm;

	MVector3 vTempFrom, vTempTo, vTempUp, vTempFront, vTempDown;
	MVector3 vDownColNor;
	MVector3 diff = to - from;
	MVector3 vDir = diff;
	vDir.z = 0.0f;
	float ft = vDir.Length();
	vDir.Normalize();

	// ���� �浹�� ��ġ ���
	vec3 vCapseuleUpDwonDir = (capsuleCollision.top - capsuleCollision.bottom).Normalize();
	m_vUpSphere = capsuleCollision.top - (vCapseuleUpDwonDir * (m_fRadius - capsuleCollision.radius));

	// ���� �ѵ��� ���ؼ� �˻��� �ִ� ���� ����
	bool bUpMove = false;
	vTempFrom = m_vUpSphere;
	if((m_fMoveAllowedHeight - m_fRadius) > (to.z - from.z))
		vTempFrom.z += (m_fMoveAllowedHeight - m_fRadius);
	else
	{
		vTempFrom.z += (to.z - from.z);
		bUpMove = true;
	}

	// ��
	bool bUpCol = MoveTest(m_vUpSphere - vec3(0, 0, 1), vTempFrom, vTempUp);

	if(vTempUp.z <= from.z)
		vTempUp.z = from.z;

	float fUpGap = vTempUp.z - m_vUpSphere.z;

	if(bUpCol)
		fUpGap = 0.0f;

	// �浹�� �����
	GetCollisionSphere(capsuleCollision, fUpGap, m_vecCollisionSphere);

	m_CollisionDebug.SetCollisionSphere(m_vecCollisionSphere);

	// ����
	vTempTo = m_vecCollisionSphere[0] + (vDir * ft);
	bool bFrontCol = MoveTestFront(m_vecCollisionSphere[0] - vDir, vTempTo, vTempFront, bUpCol);
	m_CollisionDebug.SetCollisionInvestigateData_Front(m_vecCollisionSphere[0] - vDir, vTempTo, vTempFront, bFrontCol);

	if(bFrontCol)
	{
		// �ݴ�?
		vec3 vFrontMoveDir = (vTempFront - m_vecCollisionSphere[0]).Normalize();
		vec3 vFrontColDir = (vTempFront - vTempTo).Normalize();
		if(vFrontColDir.DotProduct(vFrontMoveDir) >= 0.0f)
		{
			// �ѹ���!!! ������ �ʱ� ���ؼ�
			vec3 vFrontBack = vTempFront;
			bFrontCol = MoveTestFront(m_vecCollisionSphere[0] + vDir, vFrontBack, vTempFront, bUpCol);
			m_CollisionDebug.SetCollisionInvestigateData_FrontBack(m_vecCollisionSphere[0] + vDir, vFrontBack, vTempFront, bFrontCol);
		}
	}

	// �ؿ�
	vec3 vDownStart = vTempFront;
	if(from.z > vTempFront.z)
		vDownStart.z = from.z;

	vec3 vDownTo = vTempFront;
	vDownTo.z = to.z;

	// �������� �浹�� ���ٸ� �˻��� z�� ����
	if(bUpMove && bUpCol)
		vDownTo.z = from.z + (vTempUp.z - m_vUpSphere.z);

	bool bSliding = false;
	vDownColNor.Set(vec3::AXISZ);

	bool bDownCol = MoveTestDown(vDownStart + vec3(0, 0, 1), vDownTo, vTempDown, vDownColNor, bSliding);

	m_CollisionDebug.SetCollisionInvestigateData_Down(vDownStart + vec3(0, 0, 1), vDownTo, vTempDown, bDownCol);

	// �����̵� üũ
	if(pSliding &&
		(from.z != vTempDown.z))
	{
		*pSliding = bSliding;

		// �����̵��ε� �ö󰡹�����?
		if(bSliding && vTempDown.z > from.z)
		{
			vec3 vReTempDown = vTempDown;
			vReTempDown.z = from.z;
			vDownStart.z = from.z;
			bFrontCol = MoveTestFront(vDownStart, vReTempDown, vTempDown, bUpCol);


			// ���� ��ġ ���� �븻���� �����´�. ���� �ִ� �ٴ��� �븻���� �ƴ� �� �ִ�.
			vec3 vFinalDir = (vReTempDown - vDownStart).Normalize();
			gg.currentgamestate->GetWorld()->PickCollision(vDownStart, vDownStart + (vDir * 100.0f), NULL, &vDownColNor);
			vDownColNor.Normalize();
		}
	}

	// ���� �浹
	if(pFrontCol)
		*pFrontCol = bFrontCol;

	vOut = vTempDown;
	vOutNor = vDownColNor;

	// ���� ����׿�
	m_CollisionDebug.CalTerrainColReal(vOut, m_pTargetCollisionTree);

	m_CollisionDebug.Flip();
	return bDownCol;
}

bool XMoveCollision::WaterMoveTest( const MVector3 &from, const MVector3 &to, MVector3 orgForm, MCapsule capsuleCollision, float fWaterHeightLimit, MVector3& vOut, MVector3& vOutNor )
{
	SetMapCollisionInfo();

	m_bMoveWater = true;

	// �̵� �߿� ��򰡿� �ɷ����� False, �ƴϸ� True
	m_vecFrontColPolygon.clear();
	m_vecCollisionSphere.clear();

	m_CollisionDebug.Init();

	m_vOrgfrom = orgForm;

	bool bSliding;
	MVector3 vTempFrom, vTempTo, vTempUp, vTempFront, vTempDown;
	MVector3 vDownColNor;
	MVector3 diff = to - from;
	MVector3 vDir = diff;
	vDir.z = 0.0f;
	float ft = vDir.Length();
	vDir.Normalize();

	// �浹�� �����...
	GetCollisionSphere(capsuleCollision, m_fMoveAllowedHeight - m_fRadius, m_vecCollisionSphere);

	m_CollisionDebug.SetCollisionSphere(m_vecCollisionSphere);

	// ����
	vTempTo = m_vecCollisionSphere[0] + (vDir * ft);
	bool bFronCol = MoveTestFront(m_vecCollisionSphere[0] - vDir, vTempTo, vTempFront, false);

	// �ؿ�
	vec3 vDownStart = vTempFront;
	vec3 vDownTo = vTempFront;
	vDownTo.z = to.z;

	vDownColNor.Set(vec3::ZERO);
	bool bDownCol = MoveTestDown(vDownStart + vec3(0, 0, 1), vDownTo, vTempDown, vDownColNor, bSliding);

	vTempFront.z -= (m_fMoveAllowedHeight - m_fRadius);
	vOut = vTempFront;
	vOutNor = vDownColNor;

	m_CollisionDebug.Flip();
	return (bDownCol || bFronCol);
}

void XMoveCollision::ReBuildGroundCollisionPolygon( RCollisionPolygon& colPolygon )
{
	float fPandoraDot = colPolygon.normal.DotProduct(vec3::AXISZ);						

	m_CollisionDebug.AddFrontDebugColPolygon(colPolygon);

	// �浹�� ������ ����
	if(colPolygon.dwPolygonAttribute & RCollisionTriangle::TERRAIN ||
		fPandoraDot > SLIDING_LIMIT)
	{
		m_vecFrontColPolygon.push_back(colPolygon);
	}
	else
	{
		BuildRectCollisionPolygon(colPolygon, m_vecFrontColPolygon);
	}
}

void XMoveCollision::ReBuildWaterCollisionPolygon( RCollisionPolygon& colPolygon )
{
	float fPandoraDot = colPolygon.normal.DotProduct(vec3::AXISZ);						

	m_CollisionDebug.AddFrontDebugColPolygon(colPolygon);

	// �浹�� ������ ����
	if(fPandoraDot > SLIDING_LIMIT/* &&
		fPandoraDot > -SLIDING_LIMIT*/)
	{
		m_vecFrontColPolygon.push_back(colPolygon);
	}
	else
	{
		//BuildRectCollisionPolygon(colPolygon, m_vecFrontColPolygon);
	}
}

void XMoveCollision::BuildRectCollisionPolygon( RCollisionPolygon &colPolygon, vector<RCollisionPolygon>& frontColPolygon )
{
	vec3 vX = colPolygon.normal.CrossProduct(vec3::AXISZ);
	vX.Normalize();
	vec3 vResult = vec3::AXISZ.CrossProduct(vX);
	vResult.Normalize();

	vec3 v[3];
	v[0] = colPolygon.v[0];
	v[1] = colPolygon.v[1];
	v[2] = colPolygon.v[2];

	vec3 vMax = vec3(-100000.0f, -100000.0f, -100000.0f);
	vec3 vMin = vec3(100000.0f, 100000.0f, 100000.0f);
	vec3 vCal[3];
	MMatrix matWorld;
	MMatrix matWorldInver;

	matWorld.SetLocalMatrix(v[0], -colPolygon.normal, vec3::AXISZ);
	matWorld.GetInverse(&matWorldInver);

	for(int i = 0; i < 3; i++)
	{
		matWorldInver.TransformVect(v[i], vCal[i]);

		if(vCal[i].x > vMax.x)
			vMax.x = vCal[i].x;
		if(vCal[i].y > vMax.y)
			vMax.y = vCal[i].y;
		if(vCal[i].z > vMax.z)
			vMax.z = vCal[i].z;

		if(vCal[i].x < vMin.x)
			vMin.x = vCal[i].x;
		if(vCal[i].y < vMin.y)
			vMin.y = vCal[i].y;
		if(vCal[i].z < vMin.z)
			vMin.z = vCal[i].z;
	}

	vMax.z += BUILD_RECT_SIZE;
	vMin.z -= BUILD_RECT_SIZE;

	vec3 vHalfLine = (vMin - vMax) / 2.0f;
	vec3 vRectCenter = vMax + vHalfLine;

	vec3 vTempNor = vHalfLine;
	vTempNor.Normalize();
	vec3 vRightDir = vec3(-vTempNor.x, -vTempNor.y, vTempNor.z);
	float fLen = vHalfLine.Length();
	vec3 vRight = vRectCenter + (vRightDir * vHalfLine.Length());

	float fHalfWid = (vMin - vRight).Length() / 2.0f;
	float fHalfHei = (vMax - vRight).Length() / 2.0f;

	vec3 vTempRecCenter = vRectCenter;
	matWorld.TransformVect(vTempRecCenter, vRectCenter);

	// ���Ǿ� �浹 ��ġ ���ϱ�
	vec3 vSphereColPos = colPolygon.nearestPoint;
	// ���� ������ ������쿡�� �̷� �������.
	if(colPolygon.normal.DotProduct(vec3::AXISZ) < 0)
	{
		vec3 vSphereCenter = colPolygon.nearestPoint + (colPolygon.normal * m_fRadius);
		vec3 vCalDir = -colPolygon.normal;
		XMath::NormalizeZ0(vCalDir);
		vSphereColPos = vSphereCenter + (vCalDir * m_fRadius);
	}

	// ���� ��ġ ���ϱ�
	vec3 vColNearRectCenter;
	vec3 vCenterToNearPoint = (vSphereColPos - vRectCenter).Normalize();
	float fReCenterDot = vResult.DotProduct(vCenterToNearPoint);
	vec3 vCenterMoveDir = vResult;
	if(fReCenterDot < 0.0f)
	{
		// �ڷ�
		vCenterMoveDir = -vResult;
	}

	vec3 v2DNearPos = vSphereColPos;
	vec3 v2DRectCenter = vRectCenter;
	v2DNearPos.z = v2DRectCenter.z = 0.0f;
	vec3 vMoveNewCenter = MMath::GetNearestPointOnRay(v2DNearPos, v2DRectCenter, vCenterMoveDir);
	float fCenterMoveLen = (v2DRectCenter - vMoveNewCenter).Length();

	vColNearRectCenter = vRectCenter + (vCenterMoveDir * fCenterMoveLen);

	vec3 vRect[4];
	vRect[0] = vColNearRectCenter + vX * - fHalfWid + vec3::AXISZ * + fHalfHei;
	vRect[1] = vColNearRectCenter + vX * + fHalfWid + vec3::AXISZ * + fHalfHei;
	vRect[2] = vColNearRectCenter + vX * - fHalfWid + vec3::AXISZ * - fHalfHei;
	vRect[3] = vColNearRectCenter + vX * + fHalfWid + vec3::AXISZ * - fHalfHei;

	frontColPolygon.push_back(RCollisionPolygon(vRect[0], vRect[1], vRect[2], vSphereColPos, vResult, colPolygon.nDebugFlag, colPolygon.dwPolygonAttribute));
	frontColPolygon.push_back(RCollisionPolygon(vRect[1], vRect[3], vRect[2], vSphereColPos, vResult, colPolygon.nDebugFlag, colPolygon.dwPolygonAttribute));

	m_CollisionDebug.AddFrontDebugColReBuildPolygon(RCollisionPolygon(vRect[0], vRect[1],vRect[2], vSphereColPos, vResult, colPolygon.nDebugFlag, colPolygon.dwPolygonAttribute));
	m_CollisionDebug.AddFrontDebugColReBuildPolygon(RCollisionPolygon(vRect[1], vRect[3], vRect[2], vSphereColPos, vResult, colPolygon.nDebugFlag, colPolygon.dwPolygonAttribute));
}

void XMoveCollision::BuildRectCollisionPolygon( const MVector3 &from,const MVector3 &to, vec3 v1, vec3 v2, vector<RCollisionPolygon>& frontColPolygon, vector<RCollisionPolygon>* pDebugColPolygon )
{
	float fCenter_z = abs(from.z + (abs(from.z - to.z) / 2.0f));

	vec3 vDir = (v2 - v1).Normalize();
	vec3 vNormal = vDir.CrossProduct(vec3::AXISZ);

	vec3 vZ = vec3(0, 0, fCenter_z);
	vec3 vRect[4];

	vRect[0] = v2 + vZ * + BUILD_RECT_SIZE;
	vRect[1] = v1 + vZ * + BUILD_RECT_SIZE;
	vRect[2] = v2 + vZ * - BUILD_RECT_SIZE;
	vRect[3] = v1 + vZ * - BUILD_RECT_SIZE;

	vec3 vNearPoint;
	int nTemp;
	
	if(IsIntersectMovingSphereTriangle(from, to, m_fRadius, vRect[0], vRect[1], vRect[2], vNearPoint, nTemp))
		frontColPolygon.push_back(RCollisionPolygon(vRect[0], vRect[1], vRect[2], vNearPoint, vNormal, 0, RCollisionTriangle::TERRAIN));
	if(IsIntersectMovingSphereTriangle(from, to, m_fRadius, vRect[1], vRect[3], vRect[2], vNearPoint, nTemp))
		frontColPolygon.push_back(RCollisionPolygon(vRect[1], vRect[3], vRect[2], vNearPoint, vNormal, 0, RCollisionTriangle::TERRAIN));

	if(pDebugColPolygon)
	{
		pDebugColPolygon->push_back(RCollisionPolygon(vRect[0], vRect[1],vRect[2], vRect[1], vNormal, 0, RCollisionTriangle::TERRAIN));
		pDebugColPolygon->push_back(RCollisionPolygon(vRect[1], vRect[3], vRect[2], vRect[1], vNormal, 0, RCollisionTriangle::TERRAIN));
	}
}

void XMoveCollision::SetMapCollisionInfo()
{
	// �浹 ���� �Է�
	if (gg.currentgamestate)
		SetTargetCollisionTree(gg.currentgamestate->GetWorld()->GetCollisionTree());
}

bool XMoveCollision::CheckAllowedDownSliding( const MVector3& v1, const MVector3& v2, const MVector3& v3 )
{
	// ���� ���� m_fRadius�� ���� �۴ٸ�... �����̵��� ��������!!
	// ���� �۾Ƽ�... �����ص� �ɰ� ���Ƽ�...

	float fMaxZ = max(max(v1.z, v2.z), v3.z);
	float fMinZ = min(min(v1.z, v2.z), v3.z);

	float fCheckRadius = m_fRadius;
	if(m_fMoveAllowedHeight > m_fRadius)
		fCheckRadius = m_fMoveAllowedHeight;

	if((fMaxZ - fMinZ) > fCheckRadius)
		return true;

	return false;
}

void XMoveCollision::SetRestrictedArea( float fMin_x, float fMin_y, float fMax_x, float fMax_y )
{
	m_bRestrictedArea = false;

	if(fMin_x == 0.0f &&
		fMin_y == 0.0f &&
		fMax_x == 0.0f &&
		fMax_y == 0.0f)
		return;

	m_bRestrictedArea = true;

	m_fMin_x = fMin_x;
	m_fMin_y = fMin_y;
	m_fMax_x = fMax_x;
	m_fMax_y = fMax_y;
}

void XMoveCollision::GetRestrictedAreaPoly( const MVector3 &from, const MVector3 & to, vector<RCollisionPolygon>& vecColPolygon )
{
	// ���������ΰ�?
	if(m_bRestrictedArea)
	{
		const float fSectorSize = 30000.0f;
		if(m_fMin_x + fSectorSize >= from.x)
			BuildRectCollisionPolygon(from, to, vec3(m_fMin_x, m_fMin_y, 0), vec3(m_fMin_x, m_fMax_y, 0), vecColPolygon, m_CollisionDebug.GetRestrictedAreaDebugColPolygon());
		if(m_fMax_x - fSectorSize <= from.x)
			BuildRectCollisionPolygon(from, to,vec3(m_fMax_x, m_fMax_y, 0), vec3(m_fMax_x, m_fMin_y, 0), vecColPolygon, m_CollisionDebug.GetRestrictedAreaDebugColPolygon());
		if(m_fMin_y + fSectorSize >= from.y)
			BuildRectCollisionPolygon(from, to, vec3(m_fMax_x, m_fMin_y, 0), vec3(m_fMin_x, m_fMin_y, 0), vecColPolygon, m_CollisionDebug.GetRestrictedAreaDebugColPolygon());
		if(m_fMax_y - fSectorSize <= from.y)
			BuildRectCollisionPolygon(from, to, vec3(m_fMin_x, m_fMax_y, 0), vec3(m_fMax_x, m_fMax_y, 0), vecColPolygon, m_CollisionDebug.GetRestrictedAreaDebugColPolygon());
	}
}

bool XMoveCollision::FreeMoveTest( const MVector3 &from, const MVector3 &to, MVector3 orgForm, MCapsule capsuleCollision, MVector3& vOut, MVector3& vOutNor, bool *pSliding /*= NULL*/, bool *pFrontCol /*= NULL*/ )
{
	SetMapCollisionInfo();

	m_bMoveWater = false;
	m_strMaterialName.clear();

	// �̵� �߿� ��򰡿� �ɷ����� False, �ƴϸ� True
	m_vecFrontColPolygon.clear();

	m_CollisionDebug.Init();

	m_vOrgfrom = orgForm;

	MVector3 vTempFrom, vTempTo, vTempUp, vTempFront, vTempDown;
	MVector3 vDownColNor;
	MVector3 diff = to - from;
	MVector3 vDir = diff;
	vDir.z = 0.0f;
	float ft = vDir.Length();
	vDir.Normalize();

	// ���� �浹�� ��ġ ���
	vec3 vCapseuleUpDwonDir = (capsuleCollision.top - capsuleCollision.bottom).Normalize();
	m_vUpSphere = capsuleCollision.top - (vCapseuleUpDwonDir * (m_fRadius - capsuleCollision.radius));

	// ���� �ѵ��� ���ؼ� �˻��� �ִ� ���� ����
	bool bUpMove = false;
	vTempFrom = m_vUpSphere;
	if((m_fMoveAllowedHeight - m_fRadius) > (to.z - from.z))
		vTempFrom.z += (m_fMoveAllowedHeight - m_fRadius);
	else
	{
		vTempFrom.z += (to.z - from.z);
		bUpMove = true;
	}

	// ��
	bool bUpCol = MoveTest(m_vUpSphere - vec3(0, 0, 1), vTempFrom, vTempUp);

	if(vTempUp.z <= from.z)
		vTempUp.z = from.z;

	float fUpGap = vTempUp.z - m_vUpSphere.z;

	if(bUpCol)
		fUpGap = 0.0f;

	// �浹�� �����
	GetCollisionSphere(capsuleCollision, fUpGap, m_vecCollisionSphere);

	m_CollisionDebug.SetCollisionSphere(m_vecCollisionSphere);

	// ����
	vTempTo = m_vecCollisionSphere[0] + (vDir * ft);
	bool bFrontCol = MoveTestFront(m_vecCollisionSphere[0] - vDir, vTempTo, vTempFront, bUpCol);
	m_CollisionDebug.SetCollisionInvestigateData_Front(m_vecCollisionSphere[0] - vDir, vTempTo, vTempFront, bFrontCol);

	if(bFrontCol)
	{
		// �ݴ�?
		vec3 vFrontMoveDir = (vTempFront - m_vecCollisionSphere[0]).Normalize();
		vec3 vFrontColDir = (vTempFront - vTempTo).Normalize();
		if(vFrontColDir.DotProduct(vFrontMoveDir) >= 0.0f)
		{
			// �ѹ���!!! ������ �ʱ� ���ؼ�
			vec3 vFrontBack = vTempFront;
			bFrontCol = MoveTestFront(m_vecCollisionSphere[0] + vDir, vFrontBack, vTempFront, bUpCol);
			m_CollisionDebug.SetCollisionInvestigateData_FrontBack(m_vecCollisionSphere[0] + vDir, vFrontBack, vTempFront, bFrontCol);
		}
	}

	// �ؿ�
	vec3 vDownStart = vTempFront;
	if(from.z > vTempFront.z)
		vDownStart.z = from.z;

	vec3 vDownTo = vTempFront;
	vDownTo.z = to.z;

	// �������� �浹�� ���ٸ� �˻��� z�� ����
	if(bUpMove && bUpCol)
		vDownTo.z = from.z + (vTempUp.z - m_vUpSphere.z);

	bool bSliding = false;
	vDownColNor.Set(vec3::AXISZ);

	bool bDownCol = MoveTestDown(vDownStart + vec3(0, 0, 1), vDownTo, vTempDown, vDownColNor, bSliding);

	m_CollisionDebug.SetCollisionInvestigateData_Down(vDownStart + vec3(0, 0, 1), vDownTo, vTempDown, bDownCol);

	// �����̵� üũ
	if(pSliding)
		*pSliding = bSliding;

	// ���� �浹
	if(pFrontCol)
		*pFrontCol = bFrontCol;

	vOut = vTempDown;
	vOutNor = vDownColNor;

	m_CollisionDebug.Flip();
	return (bDownCol || bFrontCol);
}

void XMoveCollision::GetTerrainColObjectCollisionPolygon()
{
	// npc�� ������Ʈ �浹 ó��
	// npc���� �ڽ��ε� �浹ó���� �������� ó���Ѵ�.
	// ĸ���� ���� ������Ʈ �浹���� ó��...
	if(m_pvecTerrainColObjectPolygon == NULL)
		return;

	for(vector<RCollisionPolygon>::iterator it = m_pvecTerrainColObjectPolygon->begin(); it != m_pvecTerrainColObjectPolygon->end(); ++it)
	{				
		CheckPolygon(it->v[0], it->v[1], it->v[2]);
	}

}

bool XMoveCollision::CameraMoveTest( const MVector3 &from,const MVector3 &to, MVector3& vOut, float *pMoved /*= NULL*/ )
{
	if (m_pTargetCollisionTree == NULL) return false;

	// ���� �浹 ��������� ����
	Clear();

	m_source = from;
	m_destination = to;

	CalcBoundingBox();

	m_pTargetCollisionTree->GetCollidablePolygons(*this, ~RCollisionTriangle::WATER);

	GetTerrainColObjectCollisionPolygon();

	if(m_colPolygons.empty()) 
	{
		vOut = to;
		return false;
	}

	MVector3 diff = m_destination - m_source;

	float fMinMove = diff.Length();
	float fLen = fMinMove;

	// �� ��鿡 ���� �ִ�� �����ִ� �Ÿ����� �ּҰ��� �����ִ� �Ÿ���.
	for(int i=0; i<(int)m_colPolygons.size(); i++ )
	{
		RCollisionPolygon &poly = m_colPolygons[i];

		// �� ������ ������ ���������� �����ִ� �ִ� �Ÿ�
		float fMove = GetWalkableDistance(poly);

		fMinMove = min(fMinMove ,fMove);
	}

	fMinMove = max(0, fMinMove);

	vOut = m_source + Normalize(diff) * (fMinMove);

	if (pMoved != NULL)
		*pMoved = fMinMove / fLen;

	return true;
}
