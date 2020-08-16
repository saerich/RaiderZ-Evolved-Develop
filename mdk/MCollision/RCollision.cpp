#include "StdAfx.h"
#include "RCollision.h"
#include "RCollisionTree.h"

const float SLIDING_LIMIT = 0.76f;	// AXISZ.dot(poly.normal) �� ��ġ. �� �̻��̸� �̲�������.
const float	HOVER_HEIGHT = 20.0f;	// �̸�ŭ ����� �̵��Ѵ�. �� ���� + Radius ������ ���� �Ѿ.
const float	STEEPPLANE_CUTOFF_HEIGHT = 50.0f;	// ���ĸ� �����̸鼭 �̰ͺ��� ���̰� ���� �������� �߶󳻹�����.

namespace rs3 {

RCollision::RCollision(void) : m_pTargetCollisionTree(NULL)
{

}

RCollision::~RCollision(void)
{
}

void RCollision::Clear()
{
	m_colPolygons.erase(m_colPolygons.begin(),m_colPolygons.end());
}

bool RCollision::MoveTest(const MVector3 &from,const MVector3 &to, MVector3& vOut, float *pMoved)
{
	if (m_pTargetCollisionTree == NULL) return false;

	// ���� �浹 ��������� ����
	Clear();

	m_source = from;
	m_destination = to;

	CalcBoundingBox();
	
	m_pTargetCollisionTree->GetCollidablePolygons(*this, ~RCollisionTriangle::WATER);

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

const float COLLISION_TOLER	= 0.1f;


bool RCollision::MoveTestGravity( const MVector3 &from,const MVector3 &to, MVector3& vOut, bool *pSliding /*= NULL*/, MVector3* pOrgForm /*= NULL*/, float* pAllowedHeight /*= NULL*/ )
{
	MVector3 diff = to - from;
	MVector3 vDir = diff;

	vDir.z = 0;
	float ft = vDir.Length();

	MVector3 vRet, vTemp;

	vDir.Normalize();
	// �ϴ� �������, z���������� �̵��� ���߿� �ϵ��� �����Ѵ�.

	// �̵� �߿� ��򰡿� �ɷ����� False, �ƴϸ� True
	bool bFreeMove = false;

	vRet = from;

	// �ϴ� ���� �̻� ����� ���������� �̵��غ���. 
	bFreeMove = MoveTestTerrain(from, vDir, ft, vTemp, pOrgForm, pAllowedHeight);

	
	if (!bFreeMove)
	{	
		// ��򰡿� �ɸ��ų� �ε巴�� �̲������� �� �ʿ䰡 �������� 
 		float fMinMove = diff.Length();
		vDir = diff;
		vDir.Normalize();

		// ����� �̲����� �����ؼ� �̵�
		bFreeMove = MoveTestReal(vTemp, vDir, fMinMove, vRet);
	}
	else
		// �ƴϸ� �׳� �̵��Ѵ�.
		bFreeMove = MoveTest(vTemp, vTemp + MVector3(0, 0, diff.z), vRet);


	vOut = vRet;

	// ��簡 ���� �ٴ��̶� �ٴڹ�� �̲����� �������� �ϴ°��� �˻�.
	if (pSliding != NULL)  *pSliding = true;
	// �ܼ��� -1��ŭ ���������� �ɸ��� ����߿� �ް�簡 ������ �̲������ٰ� �����Ѵ�.
	MoveTest(vRet, vRet + MVector3(0, 0, -1), vTemp);
	for(int i=0; i<(int)m_colPolygons.size(); i++ )
	{
		if (m_colPolygons[i].normal.DotProduct(MVector3::AXISZ) > SLIDING_LIMIT ||
			CheckAllowedHeight(m_colPolygons[i].v[0], m_colPolygons[i].v[1], m_colPolygons[i].v[2], pOrgForm, pAllowedHeight))		
		{
			if (pSliding != NULL) *pSliding = false;				
		}
	}


	return bFreeMove;

}

bool RCollision::MoveTestReal(const MVector3& vFrom, const MVector3& vDir, float fDist, MVector3& vOut)
{
	if (m_pTargetCollisionTree == NULL) return false;

	// �ϴ� ������ �������� �Էµ� ������
	MVector3 now = vFrom;

	for (int idx = 0; ; idx++)
	{
		// ���� �浹 ��������� ����
		Clear();

		// ������-���� ����
		m_source = now;
		m_destination = now + vDir * fDist;

		CalcBoundingBox();

		// ������ �����￡ �ɸ����� �˾ƺ���.
		m_pTargetCollisionTree->GetCollidablePolygons(*this, ~RCollisionTriangle::WATER);

		// �ɸ��� �������� ������
		if(m_colPolygons.empty()) 
		{
			// �ɸ��µ��� ��3
			vOut = m_destination;
			return false;
		}

		float fMinMove = fDist;

		vector<MPlane>	vPlanes;
		MVector3 *pContactPoint = NULL;

		for(int i=0; i<(int)m_colPolygons.size(); i++ )
		{
			RCollisionPolygon &poly = m_colPolygons[i];

			// �� ������ ������ ���������� �����ִ� �ִ� �Ÿ�
			float fMove = GetWalkableDistance(poly);

			// �ּҰ��� ã�´�
			if(fMinMove>fMove) {
				fMinMove = fMove;
				pContactPoint = &(m_colPolygons[i].nearestPoint);
			}

			if (fMove <= COLLISION_TOLER)					// �� ��� ���� �����ִٸ�
			{
				if (vDir.DotProduct(poly.normal) < 0)		// �� �������� �̵������� �� ��� �ε����°�
				{
					MVector3 tmpNormal;
					if (poly.nDebugFlag == 0)				// �̵��Ҷ� �鿡 �ɸ��°�
						tmpNormal = poly.normal;			// �鿡 �ɸ��� �׳� ���� ������ ����
					else
					{
						// ���� �ɸ��� ���� ��� (���� �ɸ���?)							
						tmpNormal = m_source - poly.nearestPoint;
						tmpNormal.Normalize();
					}

					// �̲����� ����� ����صд�
					vPlanes.push_back(MPlane(tmpNormal, poly.nearestPoint));
				}
			}
		}

		// �ϴ� ������ �Ÿ���ŭ ������
		MVector3 ret = m_source + vDir * (fMinMove);			
		// ���� ��ġ�� ����
		m_source = ret;										
		// �� �Ÿ���ŭ �� �Ÿ����� ����.
		fDist -= fMinMove;									

		if(pContactPoint) 
		{
			MVector3 contactPointCenter = ret;

			MVector3 planeNormal;
			MVector3 ContactPoint;
			MVector3 slidingDestination;

			// �ϴ� �ɸ��� ���� ����
			size_t colPolys = vPlanes.size();	

			if (colPolys <= 1)		// �ɸ��� ���� �Ѱ����̸�
			{
				planeNormal = (contactPointCenter - *pContactPoint).Normalize();

				// �׳� ����ϰ� �̲�������
				ContactPoint = *pContactPoint;
				MPlane plane(planeNormal ,ContactPoint);
				slidingDestination = m_destination + plane.DotNormal(contactPointCenter - m_destination) * plane.Normal();
				
			}
			else					// �ɸ��� ���� �ΰ� �̻��̸� (��ġ����)
			{
				// x��° �� ���� �̲��������� y��° �鿡 �ɸ��°�? �ɸ��� true
				vector< vector<bool> >	vvIntersects(colPolys, vector<bool>(colPolys));
				// ���� ������ų� �ִ� �������� �ϳ����� �� ���õȴ�. true�ΰ͵鸸 üũ
				vector<bool>	vValid(colPolys, true);
	
				// ��� �� ���� �̲��������� ��� ��� �ε����°� �˻��Ѵ�.
				for (size_t i=0; i<colPolys; i++) 
				{
					// i��° ����� ���� �̲��������� ����������� �̲������°�
					MVector3 moveVec, out;
					moveVec = m_destination + vPlanes[i].DotNormal(contactPointCenter - m_destination) * vPlanes[i].Normal();
					moveVec -= contactPointCenter;
					moveVec.Normalize();

					for (size_t j=0; j<colPolys; j++) 
					{
						// i��° ����� ���� �̲��������� j��° ���� �����ϴ°�
						if (i != j)
						{
							// �̵��������� �� �����غ���
							MVector3 t;
							t = contactPointCenter + moveVec * 10000;	
							// �� ��� �����°�
							vvIntersects[i][j] = vPlanes[j].Intersect(out, contactPointCenter, t);
						}
						else	
							vvIntersects[i][j] = false;	// ������ �ȸ�����
					}
				}

				// ��� ����� ���� �̲������� �� �̲������� ���ϰ� �ٸ� ��鿡 �ɸ��� �����
				// ��ȿ�� ������� ó���Ѵ�.
				for (size_t i=0; i<colPolys; i++)
				{
					if (vValid[i])
					{
						for (size_t j=0; j<colPolys; j++) 
						{
							if ((i!=j) && (vValid[j]) && !vvIntersects[i][j])
								vValid[j] = false;
						}
					}
				}

				// ��ȿ�� ����� ������ ������� üũ�Ѵ�.	
				vector<int> vValidIndex;
				for (size_t i=0; i<colPolys; i++)
				{
					if (vValid[i])
						vValidIndex.push_back(static_cast<int>(i));
				}

				if (vValidIndex.size() == 1)	// ��ȿ�� ����� �Ѱ����̸�
				{
					// �� ����� ���� �̲�������.
					slidingDestination = m_destination + vPlanes[vValidIndex[0]].DotNormal(contactPointCenter - m_destination) * vPlanes[vValidIndex[0]].Normal();
				}
				else if (vValidIndex.size() == 2)	// �ΰ��̸�
				{
					// �� ����� ������ ���� �̲�������.
					MVector3 vIntersect = vPlanes[vValidIndex[0]].Normal().CrossProduct(vPlanes[vValidIndex[1]].Normal()); 
					vIntersect.Normalize();

					float fNextMove = vIntersect.DotProduct(vDir) * fDist;
					slidingDestination = contactPointCenter + vIntersect * fNextMove;
				}
				else	// 3�� �̻��̸� �ɷ��� �� �̻� �ȿ����δ�. ���⼭ ����
				{
					// 3�� �̻��� ����� ������ ��쵵 �ֱ� �ϰ����� �ϴ� ����.
					// ���߿� �ʿ������� ���� üũ�� �ϰ���
					vOut = ret;
					return true;

					// �� ����� ������ ���� �̲�������.
// 					MVector3 vIntersect = vPlanes[vValidIndex[0]].Normal().CrossProduct(vPlanes[vValidIndex[2]].Normal()); 
// 					vIntersect.Normalize();
// 
// 					float fNextMove = vIntersect.DotProduct(vDir) * fDist;
// 					slidingDestination = contactPointCenter + vIntersect * fNextMove;
				}
			}

			MVector3 slidingOut;
			float f;

			// ������ �˾Ƴ� �̲������� ���⿡ ���� �����δ�.
			// f���� ���� ������ �Ÿ��� ��%�� ������������ 0~1���� ��ġ�� ����.
			bool b = MoveTest(contactPointCenter,slidingDestination,slidingOut, &f);
			vOut = slidingOut;

			// �����̸鼭 �ƹ����� �ɸ��� �ʾҴٸ� �̴�� ����
			if (!b)
				return true;

			// �̲������� �ٷ� ������ �ɷ����� �ű⼭ ����
			// �� �������� �����ؼ� ���� �̲������°� ���� ������ ������������ �߾ȵ�
			if (f <= 0.01)
				return true;

			// ���� ���� ������ �Ÿ���ŭ(Ȥ�� �׷��� ������ �ϴ� �ɷ��� ������ �̲������� �Ÿ���ŭ)
			// ���� ������ ��� ��� �̵��� �Ÿ��� �̵��� �Ÿ����� ���� �̵��� ��ġ���� �ٽ� �����Ѵ�.
			fDist *= (1 - f);
			now = slidingOut;

			// 0.1���� �������� ���Ѱ� �ֱ��� ������.
			if (fDist < 0.1)
				return true;

		}
		else
		{
			// �浹 �ǽ� �������� ������ ���� �浹�ϴ� ���� ������� �׳� ������ ����.
			vOut = m_destination;
			// �浹üũ�� ù��° ����(idx == 0)���� �ɸ��� �ƿ� ���� �浹���� �ʾұ� ������ false ����
			// idx�� 0�� �ƴ� ��� true����
			return (idx != 0);
		}
	}

	// �浹��3
	return true;
}


bool RCollision::MoveTestTerrain( const MVector3& vFrom, const MVector3& vDir, float fDist, MVector3& vOut, MVector3* pOrgForm /*= NULL*/, float* pAllowedHeight /*= NULL*/ )
{
	if (m_pTargetCollisionTree == NULL) return false;

	MVector3 tmpOrigin = vFrom;

	// ���̸� �÷��ٶ�... ���� ���̰��� ������ �����ϰ� �ƴϸ� �������� �����Ѵ�.
	float fUpHeight = HOVER_HEIGHT;
	if(pAllowedHeight && pOrgForm)
	{
		float fGap = tmpOrigin.z - pOrgForm->z;
		fUpHeight = *pAllowedHeight /*+ 3.0f*/ - fGap;
	}

	tmpOrigin.z += fUpHeight;

	// ���� �浹 ��������� ����
	Clear();

	// ������-���� ����
	m_source = tmpOrigin;
	m_destination = tmpOrigin + vDir * fDist;

	CalcBoundingBox();

	// ������ �����￡ �ɸ����� �˾ƺ���.
	m_pTargetCollisionTree->GetCollidablePolygons(*this, ~RCollisionTriangle::WATER);

	// �ƹ����� �Ȱɸ��� �����Ӱ� �����̴°�?
	bool bFreeMove;

	MVector3 ret;

	// �ɸ��� �������� ������
	if(m_colPolygons.empty()) 
	{
		// �ɸ��µ��� ��3
		ret = m_destination;
		bFreeMove = true;
	}
	else
	{
		float fMinMove = fDist;

		MVector3 *pContactPoint = NULL;

		bFreeMove = true;

		for(int i=0; i<(int)m_colPolygons.size(); i++ )
		{
			RCollisionPolygon &poly = m_colPolygons[i];

			bool bCheck = CheckAllowedHeight(poly.v[0], poly.v[1], poly.v[2], pOrgForm, pAllowedHeight);

			if(bCheck == false)
			{
				// �� ������ ������ ���������� �����ִ� �ִ� �Ÿ�
				float fMove = GetWalkableDistance(poly);

				// �ּҰ��� ã�´�
				if(fMinMove>fMove) {
					fMinMove = fMove;
					pContactPoint = &(m_colPolygons[i].nearestPoint);
				}

				if ((poly.normal.DotProduct(MVector3::AXISZ) < SLIDING_LIMIT) && (fMove < 0.5f))
				{
					bFreeMove = false;
					vOut = vFrom;
					return bFreeMove;
				}
			}
		}

		// �ϴ� ������ �Ÿ���ŭ ������
		ret = m_source + vDir * (fMinMove);	

	}

	// ���� ����� �̵������� ���� �Ʒ��� ��������.
	MVector3 t = ret + MVector3(0, 0, -(fUpHeight+1));
	
	MVector3 tmpOut;

	MoveTest(ret, t, tmpOut);

	for(int i=0; i<(int)m_colPolygons.size(); i++ )
	{
		RCollisionPolygon &poly = m_colPolygons[i];
		if (poly.normal.DotProduct(MVector3::AXISZ) < SLIDING_LIMIT)		
		{
			bool bCheck = CheckAllowedHeight(poly.v[0], poly.v[1], poly.v[2], pOrgForm, pAllowedHeight);

			if (CheckTangency(tmpOut, poly) && bCheck == false)	// ����������
				bFreeMove = false;
		}
	}

	vOut = tmpOut;

	if (!bFreeMove)
	{
		vOut = vFrom;
		return bFreeMove;
	}

	return bFreeMove;
}

void RCollision::CalcBoundingBox()
{
	MBox box;
	GetBoundingBox(box);

	m_bbMovement.minx = min( m_source.x + box.minx, m_destination.x + box.minx);
	m_bbMovement.miny = min( m_source.y + box.miny, m_destination.y + box.miny);
	m_bbMovement.minz = min( m_source.z + box.minz, m_destination.z + box.minz);
	m_bbMovement.maxx = max( m_source.x + box.maxx, m_destination.x + box.maxx);
	m_bbMovement.maxy = max( m_source.y + box.maxy, m_destination.y + box.maxy);
	m_bbMovement.maxz = max( m_source.z + box.maxz, m_destination.z + box.maxz);

}

bool RCollision::CheckAllowedHeight( const MVector3& v1, const MVector3& v2, const MVector3& v3, const MVector3* pOrgFrom, const float* pAllowedHeight )
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


// ===========================  sphere
RCollisionSphere::RCollisionSphere(float fRadius) : RCollision()
{
	SetRadius(fRadius);
}

RCollisionSphere::~RCollisionSphere()
{
}


void RCollisionSphere::SetRadius(float fRadius)
{ 
	m_fRadius = fRadius;
}

float RCollisionSphere::GetRadius()
{
	return m_fRadius;
}

MVector3 RCollisionSphere::GetContactPoint(const MVector3 &vPos, const MVector3 &vNorm)
{
	return vPos - (vNorm * (m_fRadius - 10));
}


float RCollisionSphere::GetWalkableDistance(const RCollisionPolygon &collisionPolygon)
{
	// (reverse-intersecting the sphere) 
	// sphere �� source->destination ���� �̵��ϴ� ���� collisionPoint �ɸ��� ������ ã�� ���̹Ƿ�
	// collisionPoint�� �߽�, ���� �������� ���� sphere �� source->destination ������ �����ϴ� ������ ����.

	
	MVector3 dir = m_destination - m_source;
	float fDistance = dir.Length();
	dir.Normalize();		

	float fCollisionDistance;
	if(MMath::IsIntersectRaySphere(m_source, dir,collisionPolygon.nearestPoint,m_fRadius,fCollisionDistance)) {
		const float CONTACTTOLER = 0.01f;

		if(fabs(fCollisionDistance)<CONTACTTOLER) fCollisionDistance = 0;	// ���������� �� �����Ҽ� ����
		return min(fCollisionDistance , fDistance);
    }

	// �浹���� ������ ������ �����ִ�.
	return fDistance;
}

bool RCollisionSphere::IsCollidable(const MBox &box)
{
	return m_bbMovement.ContainsAABB(box);
}

bool RCollisionSphere::CheckPolygon(const MVector3 &v0,const MVector3 &v1,const MVector3 &v2, const DWORD dwAttribute /*= 0*/)
{
	MPlane plane(v0,v1,v2);

	MVector3 dir = m_destination - m_source;
	dir.Normalize();

	// �̵����⿡ ���� backface �̸� üũ���� �ʴ´�.
	if(plane.DotNormal(dir)>=0) return false;

	int nDebugFlag;
	MVector3 nearestPoint;
	if(IsIntersectMovingSphereTriangle(m_source,m_destination,m_fRadius-0.1f,v0,v1,v2,nearestPoint,nDebugFlag)) {
		float dot = plane.Normal().DotProduct(MVector3::AXISZ);
		if ((dot < SLIDING_LIMIT) && (dot > 0.2))	// ���������ﵵ �߶󳻸� ������ ��찡 �����.
		{											// ������ ���߰��� �ֵ鸸 �߶󳻰� 0.2 üũ (������ �ϵ��ڵ�- _-)
			float fMaxHeight = FLT_MIN;
			if (fabs(v0.z - v1.z) > fMaxHeight) fMaxHeight = fabs(v0.z - v1.z);
			if (fabs(v0.z - v2.z) > fMaxHeight) fMaxHeight = fabs(v0.z - v2.z);
			if (fabs(v2.z - v1.z) > fMaxHeight) fMaxHeight = fabs(v2.z - v1.z);
 			if (fMaxHeight < STEEPPLANE_CUTOFF_HEIGHT) 
			{
				return true;
			}
		}
		m_colPolygons.push_back(RCollisionPolygon(v0, v1, v2, nearestPoint, plane.Normal(), nDebugFlag, dwAttribute));
	}

	return true;
}

void RCollisionSphere::GetBoundingBox(MBox &outBox)
{
	outBox.minx = outBox.miny = outBox.minz = -m_fRadius;
	outBox.maxx = outBox.maxy = outBox.maxz = m_fRadius;
}

/// �浹 �˻� ���
// fRadius�� �ݰ��� ���� sphere�� from���� to�� �̵��Ҷ� �ﰢ��(v0,v1,v2)�� �浹�ϴ��� �Ǵ��Ѵ�.
bool RCollisionSphere::IsIntersectMovingSphereTriangle(const MVector3 &from,const MVector3 &to,const float fRadius,const MVector3 &v0,const MVector3 &v1,const MVector3 &v2, MVector3 &outNearest, int &debugFlag)
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
		// from-to �������� ��鿡 ���尡��� ��
		nearestOnFromTo = from + min(1.0f,(fDistFromPlane - fRadius) / (fDistFromPlane - fDistToPlane)) * (to-from);

		// nearestPtOnFromTo �� �ﰢ������� �Ÿ��� �������� �ʰ��ϸ� �ε������� ����.
		if(plane.GetDistanceTo(nearestOnFromTo)>fRadius+INTERSECT_TOLER) return false;

		// nearestPtOnFromTo�� ���� ����� ������� �� ( = from-to ���а� ���� ����� �ﰢ�� ������� ��)
		planeIntersectionPoint = MMath::GetNearestPoint(nearestOnFromTo,plane);
	}

	// �� ���� �ﰢ�� �ȿ� �ִ��� �˻�
	// ������ edge�� �����ϰ� ������ �� ������ �ٶ󺸴� ���鿡 ���� ��� + �����̸�
	if(MPlane(CrossProduct(plane.Normal(),v1-v0),v0).GetDistanceTo(planeIntersectionPoint)>0 &&
		MPlane(CrossProduct(plane.Normal(),v2-v1),v1).GetDistanceTo(planeIntersectionPoint)>0 &&
		MPlane(CrossProduct(plane.Normal(),v0-v2),v2).GetDistanceTo(planeIntersectionPoint)>0 ) { // nearestPtOnPlane�� �ﰢ�� ������ ���̴�
			if(LengthSq(nearestOnFromTo-from)<LengthSq(to-from)) {
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
	if(DotProduct(revDir,(polygonIntersectionPoint-from))>- MMath::TOLER) return false;

	if(MMath::IsIntersectRaySphere( polygonIntersectionPoint, revDir, from, fRadius, colDistance)) {
		if(colDistance < (to-from).Length()) {
			outNearest = polygonIntersectionPoint;
			debugFlag = 1;
			return true;
		}
	}

	return false;

}



bool RCollisionSphere::CheckTangency(const MVector3& vPos, const RCollisionPolygon& poly)
{
	float fDist = (vPos - poly.nearestPoint).Length();
	return (fDist <= GetRadius() + 0.01f);	// 0.01�� FP������ ���� ����
}

//////////////////////////////////////////////////////////////////////////
//Cone

RCollisionMovingCone::RCollisionMovingCone(float fHeght, float fRadius, float fDistance, MMatrix mView)
{
	m_fRadius = fRadius;
	m_fHeight = fHeght;
	m_View = mView;
	m_fInitDistance = m_fDistance = fDistance;
}

void RCollisionMovingCone::GetBoundingBox(MBox &outBox)
{
	outBox.minx = outBox.miny = -m_fRadius;
	outBox.minz = -m_fHeight;
	outBox.maxx = outBox.maxy = m_fRadius;
	outBox.maxz = 0.0f;
}

bool RCollisionMovingCone::IsCollidable(const MBox &box)
{
	MBox ConeBox;
	GetBoundingBox(ConeBox);

	MBox out;
	TransformBox(&out, box, m_View);

	return ConeBox.ContainsAABB(out);
}

bool RCollisionMovingCone::CheckPolygon(const MVector3 &lv0,const MVector3 &lv1,const MVector3 &lv2, const DWORD dwAttribute /*= 0*/)
{
	MVector3 v0Transe, v1Transe, v2Transe;
	
	v0Transe = lv0 * m_View;
	v1Transe = lv1 * m_View;
	v2Transe = lv2 * m_View;

	float Dist;
	if(v0Transe.z <=0 || v1Transe.z <=0)
	{
		Dist = GetNearestDistance(v0Transe, v1Transe);
		if(Dist < m_fDistance)
		{
			if(Dist >0)
				m_fDistance = Dist;
		}
	}

	if(v1Transe.z <=0 || v2Transe.z <=0)
	{
		Dist = GetNearestDistance(v1Transe, v2Transe);
		if(Dist < m_fDistance)
		{
			if(Dist >0)
				m_fDistance = Dist;
		}
	}
	if(v2Transe.z <=0 || v0Transe.z <= 0)
	{
		Dist = GetNearestDistance(v2Transe, v0Transe);
		if(Dist < m_fDistance)
		{
			if(Dist >0)
				m_fDistance = Dist;
		}
	}
	return true;

}

float RCollisionMovingCone::GetNearestDistance(MVector3& vBegin, MVector3& vEnd)
{
	MVector3 vDirection = vEnd - vBegin;
	MVector3 vNearestPoint;

	MVector3 vProjBegin, vProjEnd, vProjDir;
	vProjBegin = MVector3(vBegin.x, vBegin.y, 0.0f);
	vProjEnd = MVector3(vEnd.x, vEnd.y, 0.0f);
	vProjDir = MVector3(vDirection.x, vDirection.y, 0.0f);

	if(vProjEnd == vProjBegin)	//z�ุ �ٸ��� x,y���� ���� ��� (�������� ��Ű�� ������ ���δ�.)
	{
		float d = max(vProjBegin.z, vProjEnd.z);
		if(d > 0) d = 0.0f;  // ū���� 0���� ũ�� 0�� �����ش�. 
		return vProjBegin.Length() * m_fHeight / m_fRadius - d;
	}

	MVector3 vProjBeginNorm = vProjBegin;
	vProjBeginNorm.Normalize();
	MVector3 vProjDirNorm = vProjDir;
	vProjDirNorm.Normalize();

	if(vProjDirNorm == vProjBeginNorm)		//���� ������ �������� �� ���� �����ִٸ�
	{
		MVector3 vDirNorm = vDirection;
		vDirNorm.Normalize();
		float fDirCos = DotProduct(MVector3(0,0,1), vDirNorm);
		float fConeCos = m_fHeight / sqrtf(m_fHeight*m_fHeight + m_fRadius* m_fRadius);
		if(fabs(fDirCos) > fConeCos) //�� ���ĸ��ٸ�
		{
			vNearestPoint = vBegin.z > vEnd.z ? vBegin : vEnd;
		}
		else
		{
			vNearestPoint = vBegin.z < vEnd.z ? vBegin : vEnd;
		}
	}
	else
	{
		float t = - (vBegin.x * vDirection.x + vBegin.y * vDirection.y) / (vDirection.x * vDirection.x + vDirection.y * vDirection.y);

		if(t <=0)
		{
			vNearestPoint = vBegin;
		}
		else if(t >=1)
		{
			vNearestPoint = vEnd;
		}
		else
		{
			vNearestPoint = vBegin + vDirection * t;
		}
	}

	if(vNearestPoint.z <=0 )	//begin end�� z ���� ���� ���غ���. 
	{
		float fDist = MVector3(vNearestPoint.x, vNearestPoint.y, 0.0f).Length() * m_fHeight / m_fRadius - vNearestPoint.z;

		MVector3 UpperPoint  = vBegin.z > vEnd.z ? vBegin : vEnd;
		MVector3 LowerPoint = vBegin.z <= vEnd.z ? vBegin : vEnd;
		if(UpperPoint.z > 0)
		{
			MVector3 vDir = vNearestPoint - UpperPoint;
			float k = - UpperPoint.z / vDir.z;
			UpperPoint = UpperPoint + vDir * k;		
		}

		float fUpperPointRaius = MVector3(UpperPoint.x, UpperPoint.y, 0.0f).Length();
		if(fUpperPointRaius <=  m_fRadius)
		{
			float fUpperPointDist = fUpperPointRaius * m_fHeight / m_fRadius - UpperPoint.z;
			fDist = min(fDist, fUpperPointDist);
		}
		else
		{
			float r1 = MVector3(LowerPoint.x, LowerPoint.y,0).Length();
			float r2 = m_fRadius;
			float r3 = fUpperPointRaius;
			if(r1 > m_fRadius)
			{
				return fDist;
			}
			float zt = (r2 - r1) * (fabs(LowerPoint.z) - fabs(UpperPoint.z)) / (r3 - r1) + fabs(UpperPoint.z);
			float fResult = m_fHeight + zt;
			fDist = min(fDist, fResult);
		}

		return fDist;
	}
	else		//ã�� ���� ī�޶� �տ� �ִٸ� ��� ī�޶�� z ���� ���� ��ġ�� ���� �󿡼� ã���ش�. 
	{
		MVector3 vUnderZVector = vBegin.z < vEnd.z ? vBegin : vEnd;
		if(vUnderZVector.z > 0) //���� �ȵǴ� ������� Ȥ�ó� �ϴ� ������..�� ���� z���� �ִ� ���� ��ŵ�ع��ȱ� ������..��ŵ�߾���??
		{
			return m_fInitDistance;	
		}
		MVector3 vDir = vNearestPoint - vUnderZVector;
		float k = - vUnderZVector.z / vDir.z;
		MVector3 vZeroZVector = vUnderZVector + vDir * k;
		return MVector3(vZeroZVector.x, vZeroZVector.y, 0.0f).Length() * m_fHeight / m_fRadius;
	}
}

}

