#ifndef _MVECTOR3_H
#define _MVECTOR3_H

#include <math.h>
#include "RLib.h"

class MSVector;
class MSVector2;

// visual assist ���� MVector3, �� �ִ� union �ȿ� �ִ�
// �̸����� struct�� ����� �ν� ���ϴ� ���� ������, va forum ���� ������ �ذ�å��.
#define ANONYMOUS_STRUCT

class CML2_API MVector3
{
public:
	union {
		float v[3];
		struct ANONYMOUS_STRUCT {
			float x, y, z;
		} ;
	};

	MVector3() { }
	MVector3(int i) : x(static_cast<float>(i)), y(static_cast<float>(i)), z(static_cast<float>(i)) { }
	MVector3(float f) : x(f), y(f), z(f) { }
	MVector3(float xx, float yy, float zz): x(xx), y(yy), z(zz) { }
	MVector3(float *f) : x(f[0]),y(f[1]),z(f[2]) { }
	MVector3(const float *f) : x(f[0]),y(f[1]),z(f[2]) { }

	// operator
	MVector3 operator+() const { return MVector3( x, y, z);		}
	MVector3 operator-() const { return MVector3(-x, -y, -z);   }
 	MVector3& operator=(const MVector3& other)	{ x = other.x; y = other.y; z = other.z; return *this; }
	MVector3& operator=(const MSVector& other);
	MVector3& operator=(const MSVector2& other);

	MVector3 operator+(const MVector3& other) const { return MVector3(x + other.x, y + other.y, z + other.z);	}
	MVector3& operator+=(const MVector3& other)	{ x+=other.x; y+=other.y; z+=other.z; return *this; }

	MVector3 operator-(const MVector3& other) const { return MVector3(x - other.x, y - other.y, z - other.z);	}
	MVector3& operator-=(const MVector3& other)	{ x-=other.x; y-=other.y; z-=other.z; return *this; }

	MVector3 operator*(const MVector3& other) const { return MVector3(x * other.x, y * other.y, z * other.z);	}
	MVector3& operator*=(const MVector3& other)	{ x*=other.x; y*=other.y; z*=other.z; return *this; }
	MVector3 operator*(const float v) const { return MVector3(x * v, y * v, z * v);	}
	MVector3& operator*=(const float v) { x*=v; y*=v; z*=v; return *this; }

	MVector3 operator/(const MVector3& other) const { return MVector3(x / other.x, y / other.y, z / other.z);	}
	MVector3& operator/=(const MVector3& other)	{ x/=other.x; y/=other.y; z/=other.z; return *this; }
	MVector3 operator/(const float v) const { float i=(float)1.0/v; return MVector3(x * i, y * i, z * i);	}
	MVector3& operator/=(const float v) { float i=(float)1.0/v; x*=i; y*=i; z*=i; return *this; }

	bool operator<=(const MVector3&other) const { return x<=other.x && y<=other.y && z<=other.z;};
	bool operator< (const MVector3&other) const { return x< other.x && y< other.y && z< other.z;};
	bool operator>=(const MVector3&other) const { return x>=other.x && y>=other.y && z>=other.z;};
	bool operator> (const MVector3&other) const { return x> other.x && y> other.y && z> other.z;};
	//bool operator==(const MVector3& other) const { return other.x==x && other.y==y && other.z==z; }
	bool operator==(const MVector3& other) const { return IsEqual(other); }
	bool operator!=(const MVector3& other) const { return other.x!=x || other.y!=y || other.z!=z; }

	friend MVector3 operator * ( float f, const class MVector3& v) { return MVector3( f*v.x , f*v.y , f*v.z ); }

    // casting
    operator float* ()				{ return v; }
    operator const float* () const	{ return (const float *) v; }

	// function
	void Set(float x, float y, float z) { MVector3::x = x; MVector3::y = y; MVector3::z = z; }
	void Set(const MVector3& p)			{ MVector3::x = p.x; MVector3::y = p.y; MVector3::z = p.z; }
	float Length() const;
	float LengthSq() const;
	float DistanceTo(const MVector3& tar) const;		// tar������ �Ÿ�
	float XYDistanceTo(const MVector3& tar) const;		// tar������ XY�Ÿ�
	float XYDistanceToSq(const MVector3& tar) const;		// tar������ XY�Ÿ�SQ
	float ZDistanceTo(const MVector3& tar) const;		// tar������ Z�Ÿ�
	float DistanceToSq(const MVector3& tar) const;	// tar������ �Ÿ�SQ
	float DotProduct(const MVector3& other) const;
	MVector3 CrossProduct(const MVector3& p) const;
	MVector3& Normalize();
	MVector3& FastNormalize();
	void UnsafeFastNormalize();

	void SetLength(float newlength);
	void Invert();
	MVector3 GetInterpolated(const MVector3& other, float d) const;
	void InterpolateTo(const MVector3& other, float d, MVector3& out) const;		// ���� �ȸ���� ó���ϰ�.. �ӵ� ����. 
	float AngleBetween(const MVector3& other) const;			///< �� ���� ���� ������ ���� ���� ��ȯ(���⼺ ����)
	float AngleToXY(const MVector3& other) const;					///< XY���󿡼� other������ ���� ���� ��ȯ(���⼺ ����)
	float GetAngleXY() const;										///< XY���󿡼� x�����κ��� y�� ���������� ���� ����
	bool IsEqual(const MVector3& other) const;


	static const MVector3 ZERO;
	static const MVector3 AXISX;
	static const MVector3 AXISY;
	static const MVector3 AXISZ;
};

// inline functions ////////////////////////////////////////////////////////////////////
inline float Length(const MVector3 &v)
{
	return v.Length();
}

inline float LengthSq(const MVector3 &v)
{
	return v.LengthSq();
}

inline float DotProduct(const MVector3 &v1, const MVector3 &v2)
{
	return v1.DotProduct(v2);
}

inline MVector3 CrossProduct(const MVector3 &v1, const MVector3 &v2)
{
	return v1.CrossProduct(v2);
}

inline float MVector3::Length() const
{
	return (float)sqrt(x*x+y*y+z*z);
}

inline float MVector3::LengthSq() const
{
	return (x*x+y*y+z*z);
}

inline float MVector3::DistanceTo(const MVector3& tar) const
{
	float x1 = tar.x - x;
	float y1 = tar.y - y;
	float z1 = tar.z - z;

	return (float)sqrt(x1*x1+y1*y1+z1*z1);
}

inline float MVector3::XYDistanceTo(const MVector3& tar) const
{
	float x1 = tar.x - x;
	float y1 = tar.y - y;

	return (float)sqrt(x1*x1+y1*y1);
}

inline float MVector3::XYDistanceToSq(const MVector3& tar) const
{
	float x1 = tar.x - x;
	float y1 = tar.y - y;

	return (x1*x1+y1*y1);
}

inline float MVector3::ZDistanceTo(const MVector3& tar) const
{
	float z1 = tar.z - z;

	return z1;
}

inline float MVector3::DistanceToSq(const MVector3& tar) const
{
	float x1 = tar.x - x;
	float y1 = tar.y - y;
	float z1 = tar.z - z;

	return (x1*x1+y1*y1+z1*z1);
}

inline float MVector3::DotProduct(const MVector3& other) const
{
	return x*other.x + y*other.y + z*other.z;
}

inline MVector3 MVector3::CrossProduct(const MVector3& p) const
{
	return MVector3(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
}

//
// MSM: Fast float inverse square root using SSE.
// Accurate to within 1 LSB.
//
inline float appInvSqrt( float F )
{
	const float fThree = 3.0f;
	const float fOneHalf = 0.5f;
	float temp;

	__asm
	{
		movss	xmm1,[F]
		rsqrtss	xmm0,xmm1			// 1/sqrt estimate (12 bits)

		// Newton-Raphson iteration (X1 = 0.5*X0*(3-(Y*X0)*X0))
		movss	xmm3,[fThree]
		movss	xmm2,xmm0
		mulss	xmm0,xmm1			// Y*X0
		mulss	xmm0,xmm2			// Y*X0*X0
		mulss	xmm2,[fOneHalf]		// 0.5*X0
		subss	xmm3,xmm0			// 3-Y*X0*X0
		mulss	xmm3,xmm2			// 0.5*X0*(3-Y*X0*X0)
		movss	[temp],xmm3
	}

	return temp;
}

inline void MVector3::UnsafeFastNormalize()
{
	float scale = appInvSqrt(x*x+y*y+z*z);
	x *= scale;
	y *= scale;
	z *= scale;
}

inline MVector3& MVector3::Normalize()
{
	float scale = (float)Length();

	if (scale == 0)
		return *this;

	scale = (float)1.0f / scale;
	x *= scale;
	y *= scale;
	z *= scale;
	return *this;
}

inline MVector3& MVector3::FastNormalize()
{
	float fx = LengthSq();
	{	// fast invert-sqrtf code
		float vhalf = 0.5f*fx;
		int i = *(int*)&fx;
		i = 0x5f3759df - (i >> 1);
		fx = *(float*)&i;
		fx = fx*(1.5f - vhalf*fx*fx);
	}

	x *= fx;
	y *= fx;
	z *= fx;
	
	return *this;
}

inline MVector3 Normalize(const MVector3 &v)
{
	MVector3 vector = v;
	return vector.Normalize();
}

inline void MVector3::SetLength(float newlength)
{
	Normalize();
	*this *= newlength;
}

inline void MVector3::Invert()
{
	x *= -1.0f;
	y *= -1.0f;
	z *= -1.0f;
}

inline MVector3 MVector3::GetInterpolated(const MVector3& other, float d) const
{
	float inv = 1.0f - d;
	return MVector3(other.x*inv + x*d,
						other.y*inv + y*d,
						other.z*inv + z*d);
}

inline void MVector3::InterpolateTo(const MVector3& other, float d, MVector3& out) const		// ���� �ȸ���� ó���ϰ�.. �ӵ� ����. 
{
	out.x = (other.x-x)*d + x;
	out.y = (other.y-y)*d + y;
	out.z = (other.z-z)*d + z;
}

inline float MVector3::AngleBetween(const MVector3& other) const
{
	float lenProduct = Length() * other.Length();

	// Divide by zero check
	if(lenProduct < 1e-6f)
		lenProduct = 1e-6f;

	float fDot = this->DotProduct(other) / lenProduct;

	if (fDot < -1.0f) fDot = -1.0f;
	if (fDot > 1.0f) fDot = 1.0f;
	return acosf(fDot);
}

inline float MVector3::AngleToXY(const MVector3& other) const
{
	if((this->x==0.0f) && (this->y==0.0f) && (this->z==0.0f)) return 0;
	if((other.x==0.0f) && (other.y==0.0f) && (other.z==0.0f)) return 0;

	MVector3 a = *this;
	MVector3 b = other;

	a.z=0; a.Normalize();
	b.z=0; b.Normalize();

	float aa = a.GetAngleXY();

	float x = (float)(b.x*cos(aa)+b.y*sin(aa));
	float y = (float)(b.x*(-sin(aa))+b.y*cos(aa));

	MVector3 ret = MVector3(x,y,0);
	return ret.GetAngleXY();
}

inline float MVector3::GetAngleXY() const
{
	if(x >= 1.0f) return 0.0f;
	if(x <= -1.0f) return -3.1415926535f;
	if(y>0.f)
		return static_cast<float>(acos(x));
	else
		return static_cast<float>(-acos(x));
}

#endif