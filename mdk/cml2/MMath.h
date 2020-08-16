#ifndef _MMATH_H
#define _MMATH_H

#include <math.h>
#include <crtdbg.h>
#include <stdlib.h>
#include <time.h>

#include "RLib.h"
#include "MVector2.h"
#include "MVector3.h"
#include "MPlane.h"
#include "MSphere.h"

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

// ���� ���� Ŭ����
class CML2_API MMath
{
private:
	static int LineClippingGetCode(MVector2 &p, const MRectF *pRect);

public:
	static const float PI;
	static const float TWOPI;
	static const float HALFPI;
	static const float EPSILON;
	static const float TOLER;

	/// log2
	static float Log2(float x)
	{
		return logf(x)/logf(2.0f); 
	}

	static void Randomize()
	{
		srand( (unsigned int)time(NULL) );
	}

	/// min���� max������ ������ ���Ѵ�. (nMin, nMax�� ���� ���Եȴ�.)
	static int RandomNumber(int nMin, int nMax)
	{
		if (nMin == nMax)			return nMin;
		if (0 == (nMax - nMin + 1)) return nMin;

		// ������ ũ�Ⱑ 131,072�̻��̸� �����÷ο찡 �߻��Ѵ�.
		_ASSERT((nMax-nMin) < 131072);
		//return (rand() * (nMax - nMin + 1) / (RAND_MAX+1) + nMin );
		return (rand() % (nMax - nMin + 1)) + nMin;
	}

	/// min���� max������ ������ ���Ѵ�.
	static float RandomNumber(float fMin, float fMax)
	{
		float fRandom = (float)rand() / (float)RAND_MAX;
		return(fRandom*(fMax-fMin)+fMin);
	}

	/// d&d�� �ֻ����� ������ ���Ѵ�. - sides��ü �ֻ��� n���� ������ mod�� ���ؼ� ������ �� ��ȯ
	static long Dice(unsigned char n, unsigned char sides, short mod)
	{
		int result = mod;
		for(int i = 0; i < n; i++)
		{
			result += RandomNumber(1, sides);
		}
		return result;
	}

	static float Roundf(float x)
	{
		return floorf(x + .5f);
	}

	static double Round(double x)
	{
		return floor(x + 0.5);
	}

	static int TruncateToInt(double x)
	{
		if (x < 0.0 && x < static_cast<int>(x)) return static_cast<int>(x) - 1;
		return static_cast<int>(x);
	}

	static float Pow( float x, float y ) 
	{
		return powf( x, y );
	}

	static float Pow( float x, int y ) 
	{
		return pow( x, y );
	}

	static double Pow64( float x, float y ) 
	{
		return pow( x, y );
	}


	/// ��ȣ�� ���ϱ�
	static MVector2 GetArcPosition(float fRadius, float fDistance)
	{
		return MVector2(cosf(fRadius)*fDistance, sinf(fRadius)*fDistance);
	}

	/// ������ ���Ȱ����� ��ȯ
	static float DegreeToRadian(float deg)
	{
		return (PI * deg / 180.0f);
	}

	static float RadianToDegree(float rad)
	{
		return (rad * 180.0f / PI);

	}

	static bool Equals(float a, float b, float tolerance=TOLER)
	{
		if (fabs(b-a) <= tolerance)	return true;
		else return false;
	}

	static bool Equals(MVector3 a, MVector3 b, float tolerance=TOLER)
	{
		if (fabs(b.x-a.x) > tolerance)	return false;
		if (fabs(b.y-a.y) > tolerance)	return false;
		if (fabs(b.z-a.z) > tolerance)	return false;

		return true;
	}

	static bool IsZero(float a, float tolerance=TOLER)
	{
		if (fabs(a) <= tolerance)	return true;
		else return false;
	}

	// http://www.acm.org/jgt/papers/MollerTrumbore97/ 
	// Fast, Minimum Storage Ray-Triangle Intersection
	/// ������ v0-v1-v2 �������� �����ϴ����� �˻��Ѵ�. �޸��� ���������ʴ°ɷ��Ѵ�.
	static bool IsIntersect( const MVector3& orig, const MVector3& dir, MVector3& v0, MVector3& v1, MVector3& v2, MVector3 *pIntPos )
	{
		// culling �� ���Եȴ�.

		MVector3 edge1, edge2, tvec, pvec, qvec;
		float det,inv_det;
		float u,v,t;

		/* find vectors for two edges sharing v0 */
		edge1 = v1 - v0;
		edge2 = v2 - v0;

		/* begin calculating determinant - also used to calculate U parameter */
		pvec = CrossProduct(dir, edge2);

		/* if determinant is near zero, ray lies in plane of triangle */
		det = DotProduct(edge1, pvec);

		if (det < MMath::EPSILON)
			return false;

		/* calculate distance from v0 to ray origin */
		tvec = orig - v0;

		/* calculate U parameter and test bounds */
		u = DotProduct(tvec, pvec);
		if (u < 0.0 || u > det)
			return false;

		/* prepare to test V parameter */
		qvec = CrossProduct(tvec, edge1);

		/* calculate V parameter and test bounds */
		v = DotProduct(dir, qvec);
		if (v < 0.0 || u + v > det)
			return false;

		/* calculate t, scale parameters, ray intersects triangle */
		t = DotProduct(edge2, qvec);
		inv_det = 1.0f / det;
		t *= inv_det;
		u *= inv_det;
		v *= inv_det;

		if(pIntPos) {
			*pIntPos = v0 + u*edge1 + v*edge2;
		}

		return true;
	}

	/// ������ v0-v1-v2 �������� �����ϴ����� �˻��Ѵ�.
	static bool IsIntersectNoCull( const MVector3& orig, const MVector3& dir, MVector3& v0, MVector3& v1, MVector3& v2, MVector3 *pIntPos )
	{
		MVector3 edge1, edge2, tvec, pvec, qvec;
		float det,inv_det;
		float u,v,t;

		/* find vectors for two edges sharing v0 */
		edge1 = v1 - v0;
		edge2 = v2 - v0;

		/* begin calculating determinant - also used to calculate U parameter */
		pvec = CrossProduct(dir, edge2);

		/* if determinant is near zero, ray lies in plane of triangle */
		det = DotProduct(edge1, pvec);

		if (det > -MMath::EPSILON && det < MMath::EPSILON)
			return false;
		inv_det = 1.f / det;

		/* calculate distance from vert0 to ray origin */
		tvec = orig - v0;

		/* calculate U parameter and test bounds */
		u = DotProduct(tvec, pvec) * inv_det;
		if (u < 0.0 || u > 1.0)
			return false;

		/* prepare to test V parameter */
		qvec = CrossProduct( tvec, edge1);

		/* calculate V parameter and test bounds */
		v = DotProduct(dir, qvec) * inv_det;
		if (v < 0.0 || u + v > 1.0f)
			return false;

		/* calculate t, ray intersects triangle */
		t = DotProduct(edge2, qvec) * inv_det;

		if(pIntPos) {
			*pIntPos = v0 + u*edge1 + v*edge2;
		}

		return true;
	}


	/// ����(a,aa) ���� �������� ���� ����� �������� ��.
	static MVector3 GetNearestPoint(const MVector3 &a,const MVector3 &aa,const MPlane &plane)
	{
		MVector3 b=aa-a;

		float fDot=plane.DotNormal(b);
		if(IsZero(fDot, TOLER))	// ��鿡 ������ ������ ���
			return a;

		float t=-plane.GetDistanceTo(a)/fDot;
		t = max(0,min(1, t));

		return a+t*b;
	}

	/// ��(p) ���� ������� ���� ����� ��.
	static MVector3 GetNearestPoint(const MVector3 p, const MPlane &plane)
	{
		return p - plane.Normal() * plane.GetDistanceTo(p);
	}

	/// ��(p) ���� ����(a,aa) �� ���� ����� �������� ��
	static MVector3 GetNearestPointOnLine(const MVector3 &p, const MVector3 &a,const MVector3 &aa)
	{
		MVector3 ap = p - a;

		MVector3 v = aa - a;
		float fatob = v.Length();
		v.Normalize();

		float t = DotProduct(v ,ap);

		if(t<0) return a;
		if(t>fatob) return aa;

		return a + t*v;
	}

	/// �������� ���б����� �Ÿ�
	static float GetDistancePointLineSegment(const MVector3 &p,const MVector3 &a,const MVector3 &b)
	{
		return (GetNearestPointOnLine(p,a,b)-p).Length();
	}

	/// ��(p) ���� �������� ���� ����� �������� ��
	static MVector3 GetNearestPointOnRay(const MVector3 &p, const MVector3 &o,const MVector3 &dir)
	{
		MVector3 ap = p - o;

		MVector3 v = dir;
		v.Normalize();

		float t = DotProduct(v ,ap);

		if(t<0) return o;

		return o + t*v;
	}

	// ���� a,b���� ������ o,dir�� ���� ����� �������� ��
	static MVector3 GetNearestPointOnLineRay(const MVector3 &a, const MVector3 &b,const MVector3 &o,const MVector3 &dir, bool* bParallel = NULL);

	// ���� a,b���� ���� c,d�� ���� ����� �������� ��
	static MVector3 GetNearestPointOnLine(const MVector3 &a, const MVector3 &b,const MVector3 &c,const MVector3 &d, bool* bParallel = NULL);


	/// �������� ������������ �Ÿ�
	static float GetDistancePointRay(const MVector3 &p,const MVector3 &o,const MVector3 &dir)
	{
		return (GetNearestPointOnRay(p,o,dir)-p).Length();
	}

	// �� ���� ������ �ִܰŸ�
	static float GetDistanceSegment(const MVector3 &A, const MVector3 &B,const MVector3 &C,const MVector3 &D, bool* bParallel = NULL);

	static float GetDistanceSegmentRay(const MVector3 &a, const MVector3 &b,const MVector3 &o,const MVector3 &dir, bool* bParallel = NULL);

	/// ������ ���� �浹, ���ϰ� outDistance �� origin���� ������ �浹 �Ÿ�
	static bool IsIntersectRaySphere( MVector3& rayOrigin, const MVector3& rayDir, const MVector3& sphereCenter, const float fSphereRadius, float &outDistance)
	{
		return MMath::IsIntersectRaySphere(rayOrigin, rayDir, MSphere(sphereCenter, fSphereRadius), outDistance);
	}
	static bool IsIntersectRaySphere( MVector3& rayOrigin, const MVector3& rayDir, const MSphere& sphere, float &outDistance)
	{
		MVector3 originToCenter = sphere.center - rayOrigin;

		// originToCenter�� �������� ������ �Ÿ�
		float fRayProjOriginToCenter = DotProduct(rayDir,originToCenter);

		// ������ ���� center���� �Ÿ��� ����
		float distanceRayToCenterSq = originToCenter.LengthSq() - fRayProjOriginToCenter*fRayProjOriginToCenter;

		// ������ ���� �߽ɰ� ���尡����� - �浹������ �Ÿ��� ����
		float d = sphere.radius * sphere.radius - distanceRayToCenterSq;

		if(d < 0 ) 
		{
			outDistance = fRayProjOriginToCenter;
			return false;
		}
		else
		{
			outDistance = fRayProjOriginToCenter - sqrt(d);
			return true;
		}

		return true;
	}

	/// ��ī��Ʈ ��ǥ�� -> ����ǥ��
	/// http://kwon3d.com/theory/crdsys/polar.html
	static void CartesianToSpherical( vec3 vec, float& out_r, float& out_phi, float& out_theta )
	{
		out_r = vec.Length();
		float x = vec.x;
		float y = vec.y;
		float z = vec.z;

		if( x > 0 )
		{
			out_phi = (float)atan( y / x );
		}
		else if( x < 0 && y > 0 )
		{
			out_phi = (float)atan( y / x ) + MMath::PI;
		}
		else if( x < 0 && y < 0 )
		{
			out_phi = (float)atan( y / x ) - MMath::PI;
		}
		else if( x == 0 && y > 0 )
		{
			out_phi = MMath::PI / 2;
		}
		else if( x == 0 && y < 0 )
		{
			out_phi = - (float)MMath::PI / 2;
		}

		if( z > 0 )
		{     
			out_theta = (float)atan( sqrt( x*x + y*y ) / z );
		}
		else if( z < 0 )
		{
			out_theta = (float)atan( sqrt( x*x + y*y ) / z ) + (float)MMath::PI;
		}
		else
		{
			// z == 0
			out_theta = MMath::PI / 2;
		}
	}

	/// ����ǥ�� -> ��ī��Ʈ ��ǥ��
	static vec3 SphericalToCartesian( float r, float phi, float theta )
	{
		float x = r * (float)sin(theta) * (float)cos(phi); // = r * cos(phi) * sin(theta)
		float y = r * (float)sin(theta) * (float)sin(phi); // = r * sin(phi) * sin(theta)
		float z = r * (float)cos(theta);
		return (vec3(x,y,z));
	}

	/// ���� ����
	template<typename T> 
	static T LinearInterpolation(T& A, T& B, float t)
	{
		return (T)(A * (1.0f - t) + (B * t));
	}

	/// ���� ����
	template<typename T> 
	static T LinearInterpolation(const T& A, const T& B, float t)
	{
		return (T)(A * (1.0f - t) + (B * t));
	}

	/// cubic spline ����
	/// v0 - ������, v3 - ����, t - 0~1������ ��
	template<typename T>
	static T CubicInterpolation(T& v0, T& v1,  T& v2, T& v3, float t)
	{
		T A = v3 - (3 * v2) + (3 * v1) - v0;
		T B = (3 * v2) - (6 * v1) + (3 * v0);
		T C = (3 * v1) - (3 * v0);
		T D = v0;

		return (A * (t * t * t)) + (B * (t * t)) + (C * t) + D;
	}

	/// Catmull-Rom Inteplation
	/// p1 : ������, p4 : ����, t : p2-p3������ 0~1������ ��
	template<typename T>
	static T CatmullRomInterpolation(const T& p1, const T& p2, const T& p3, const T& p4, float t)
	{
		float t2 = t * t;
		float t3 = t2 * t;

		float b1 = 0.5f * (  -t3 + 2*t2 - t);
		float b2 = 0.5f * ( 3*t3 - 5*t2 + 2);
		float b3 = 0.5f * (-3*t3 + 4*t2 + t);
		float b4 = 0.5f * (   t3 -   t2    );

		return (p1*b1 + p2*b2 + p3*b3 + p4*b4); 
	}

	/// cardianl matrix
	static MMatrix GetCardinalMatrix(float a)
	{
		MMatrix m;

		m._12 = 2.0f - a;
		m._13 = a - 2.0f;
		m._21 = 2.0f * a;
		m._22 = a - 3.0f;
		m._23 = 3.0f - 2.0f * a;
		m._42 = 1.0f;
		m._14 = m._33 = a;
		m._11 = m._24 = m._31 = -a;
		m._32 = m._34 = m._41 = m._43 = m._44 = 0.0f;

		return m;
	}

	/// matrix
	static float CardinalInterpoltationMatrix(float a, float b, float c, float d, float fAlpha, MMatrix& m)
	{
		float p0, p1, p2, p3;
		p0 = m._11 * a + m._12 * b + m._13 * c + m._14 * d;
		p1 = m._21 * a + m._22 * b + m._23 * c + m._24 * d;
		p2 = m._31 * a + m._32 * b + m._33 * c + m._34 * d;
		p3 = m._41 * a + m._42 * b + m._43 * c + m._44 * d;
		return (p3 + fAlpha * (p2 + fAlpha * (p1 + fAlpha * p0)));
	}

	/// cardinal spline ����
	static void CardinalInterpoltation(vector<MVector3>& knots, float tension, int grain, vector<MVector3>& outSpline)
	{
		int n = (int)knots.size();
		MMatrix matCadinal = GetCardinalMatrix(tension);

		vector<float> vecAlpha;
		for(int k = 0; k < grain; k++)
		{
			vecAlpha.push_back((float)((float)k / (float)grain));
		}

		for(int i = 0; i < n - 3; i++)
		{
			for(int j = 0; j < grain; j++)
			{
				vec3 s;
				s.x = CardinalInterpoltationMatrix(knots[i].x, knots[i+1].x, knots[i+2].x, knots[i+3].x, vecAlpha[j], matCadinal);
				s.y = CardinalInterpoltationMatrix(knots[i].y, knots[i+1].y, knots[i+2].y, knots[i+3].y, vecAlpha[j], matCadinal);
				s.z = CardinalInterpoltationMatrix(knots[i].z, knots[i+1].z, knots[i+2].z, knots[i+3].z, vecAlpha[j], matCadinal);

				outSpline.push_back(s);
			}
		}
	}

	static int IntersectSegment2D(const MVector2& p1, const MVector2& p2, const MVector2& p3, const MVector2 &p4, MVector2& p);

	/// ������ ������
	static bool IsEqualDirection( vec3& vTarDir, vec3& vCurDir, float fTolerRad=0.01f );
	
	/// MRectF ������ ���� �� ������ Ŭ���� �Ѵ�.
	static bool LineClipping2D(MVector2 &vP, MVector2 &vQ, const MRectF *pRect);

	/**
	@brief �������� ����� �������� ���Ѵ�.
	@param pOut		�������� ���� ���ͺ����� ������
	@param pos		�������� ���� ��ġ
	@param dir		�������� ����
	@return		�������� ���� �� ������ true��, �׷��� ������ false�� �����Ѵ�.
	*/
	static bool IsIntersectionPlane(MVector3 &outVec, const MVector3 &pos, const MVector3 &dir, const MPlane &plane);

	static const int _MAX_PORTAL_PLANES = 128;
	/**
	@brief ���ؽ� ������ ������� �߶� �� ���ؽ� ������ ���Ѵ�.
	@param pDest			������� �߶� �� ���ؽ� �迭�� ���� ������
	@param pnDestVertices	�� ���ؽ��� ������ ���� ������
	@param pSrc				������� �߶� �� ���� ���ؽ� �迭
	@param nSrcVertices		������� �߶� �� ���� ���ؽ��� ����
	@param plane			���ؽ��� �߶� �� ���
	@return ���ؽ��� ��� �ȿ� ������ true�� ��ȯ.
	*/
	static bool ClipVertices(MVector3 *pDest, int *pnDestVertices, const MVector3 *pSrc, int nSrcVertices, const MPlane &plane);
	static bool ClipVertices(vector<MVector3>& dest, const vector<MVector3>& src, const MPlane &plane);

	// ȸ��
	static vec3 RotationAxisZ(float fRot, vec3& vec)
	{
		vec3 ret = vec;

		ret.x = vec.x * cos( fRot) - vec.y * sin( fRot);
		ret.y = vec.y * cos( fRot) + vec.x * sin( fRot);
		ret.Normalize();

		return ret;
	}

	static MVector3 RightHandedNormal(const MVector3& P0,const MVector3& P1,const MVector3& P2);
};


/// ����ȭ
inline void Randomize()
{
	MMath::Randomize();
}

/// min���� max������ ������ ���Ѵ�. (nMin, nMax�� ���� ���Եȴ�.)
inline int RandomNumber(int nMin, int nMax)
{
	return MMath::RandomNumber(nMin, nMax);
}

/// min���� max������ ������ ���Ѵ�.
inline float RandomNumber(float fMin, float fMax)
{
	return MMath::RandomNumber(fMin, fMax);
}

inline MVector3 RandomNumber(MVector3 vMin, MVector3 vMax)
{
	float x = RandomNumber(vMin.x, vMax.x);
	float y = RandomNumber(vMin.y, vMax.y);
	float z = RandomNumber(vMin.z, vMax.z);
	return(MVector3(x,y,z));
}

/// d&d�� �ֻ����� ������ ���Ѵ�. - sides��ü �ֻ��� n���� ������ mod�� ���ؼ� ������ �� ��ȯ
inline long Dice(unsigned char n, unsigned char sides, short mod)
{
	return MMath::Dice(n, sides, mod);
}

inline float Roundf(float x)
{
	return MMath::Roundf(x);
}

/// ������ ���Ȱ����� ��ȯ
inline float DegreeToRadian(float deg)
{
	return MMath::DegreeToRadian(deg);
}

inline float RadianToDegree(float rad)
{
	return MMath::RadianToDegree(rad);
}

inline bool MMath::IsEqualDirection( vec3& vTarDir, vec3& vCurDir, float fTolerRad )
{
	if (vTarDir == vCurDir) return true;

	float fAngleRadian = vCurDir.AngleBetween(vTarDir);
	float fAngleDegree = RadianToDegree(fAngleRadian);
	if (fAngleDegree > fTolerRad) return false;

	return true;
}

// ������ ���� ----------------------------------

#endif
