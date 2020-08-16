#ifndef _MSVECTOR_H
#define _MSVECTOR_H

#include "RLib.h"

class MVector3;

// �ε��Ҽ������� ������ ����. size = 6byte
// �������� ũ�Ƿ� �ʹ� ū ���� ����ϸ� �ȵȴ�. �ַ� normal vector ��� ����ϱ� �ٶ�.
// SEEE EMMM MMMM MMMM
class CML2_API MSVector
{
private:
	unsigned short _32To16(float t)
	{
		int s = ((*((unsigned long*)&t) & 0x80000000) >> 31);
		int e = ((*((unsigned long*)&t) & 0x7F800000) >> 23) - 127;
		int m = (*((unsigned long*)&t) & 0x007FFFFF);

		if (e < -7) return 0;

		return (unsigned short)( (s << 15) | ((e + 7) << 11) | (m >> (23 - 11)) );
	}
	float _16To32(unsigned short t)
	{
		int s = ((t & 0x8000) >> 15);
		int e = ((t & 0x7800) >> 11) - 7;
		int m = (t & 0x007FF);

		if (t == 0) return 0.0f;

		unsigned long f = (s << 31) | ((e + 127) << 23) | (m << (23 - 11));
		return *((float*)&f);
	}
	union {
		struct {
			unsigned short m_x, m_y, m_z;
		} ;
		unsigned short m_v[3];
	};
public:
	MSVector() { }
	MSVector(const MSVector& other) : m_x(other.m_x), m_y(other.m_y), m_z(other.m_z) {}
	MSVector(float x, float y, float z);
	MSVector(float *f);
	MSVector(const float *f);

	void SetX(float x) { m_x = _32To16(x); }
	void SetY(float y) { m_y = _32To16(y); }
	void SetZ(float z) { m_z = _32To16(z); }
	float X() { return _16To32(m_x); }
	float Y() { return _16To32(m_y); }
	float Z() { return _16To32(m_z); }

	bool IsEqual(const MSVector& other) const;

	MSVector& operator=(const MSVector& other)	{ m_x = other.m_x; m_y = other.m_y; m_z = other.m_z; return *this; }
	MSVector& operator=(const MVector3& other);
	bool operator==(const MSVector& other) const { return IsEqual(other); }
};

// 2���� MSVector
class CML2_API MSVector2
{
private:
	unsigned short _32To16(float t)
	{
		int s = ((*((unsigned long*)&t) & 0x80000000) >> 31);
		int e = ((*((unsigned long*)&t) & 0x7F800000) >> 23) - 127;
		int m = (*((unsigned long*)&t) & 0x007FFFFF);

		if (e < -7) return 0;

		return (unsigned short)( (s << 15) | ((e + 7) << 11) | (m >> (23 - 11)) );
	}
	float _16To32(unsigned short t)
	{
		int s = ((t & 0x8000) >> 15);
		int e = ((t & 0x7800) >> 11) - 7;
		int m = (t & 0x007FF);

		if (t == 0) return 0.0f;

		unsigned long f = (s << 31) | ((e + 127) << 23) | (m << (23 - 11));
		return *((float*)&f);
	}
	union {
		struct {
			unsigned short m_x, m_y;
		} ;
		unsigned short m_v[2];
	};
public:
	MSVector2() { }
	MSVector2(const MSVector& other) : m_x(other.m_x), m_y(other.m_y) {}
	MSVector2(float x, float y);
	MSVector2(float *f);
	MSVector2(const float *f);

	void SetX(float x) { m_x = _32To16(x); }
	void SetY(float y) { m_y = _32To16(y); }
	float X() { return _16To32(m_x); }
	float Y() { return _16To32(m_y); }

	bool IsEqual(const MSVector2& other) const;

	MSVector2& operator=(const MSVector2& other)	{ m_x = other.m_x; m_y = other.m_y; return *this; }
	MSVector2& operator=(const MVector3& other);
	bool operator==(const MSVector2& other) const { return IsEqual(other); }
};

#endif