#pragma once

#include "RTypes.h"

namespace rs3 {

class RAnimationKey {
public:
	int frame;
};

class RVec3Key : public RVector, public RAnimationKey  {
public:
	// prevKey �� nextKey �� ������ Ű������ �����Ѵ�
	void SetInterpolatedKey(const RVec3Key& prevKey, const RVec3Key& nextKey, float s)
	{
		RVector vecInt = s*nextKey + (1.f-s)*prevKey;
		x = vecInt.x;
		y = vecInt.y;
		z = vecInt.z;
	}
};

class RQuatKey : public RQuaternion, public RAnimationKey {
public:
	// prevKey �� nextKey �� ������ Ű������ �����Ѵ�
	void SetInterpolatedKey(const RQuatKey& prevKey, const RQuatKey& nextKey, float s)
	{
		Slerp(prevKey,nextKey,s);
	}
};

typedef RVec3Key RPosKey;
typedef RVec3Key RScaleKey;

class RVisKey : public RAnimationKey {
public:
	float v;

	// prevKey �� nextKey �� ������ Ű������ �����Ѵ�
	void SetInterpolatedKey(const RVisKey& prevKey, const RVisKey& nextKey, float s)
	{
		v = s*nextKey.v + (1.f-s)*prevKey.v;
	}
};

class RVertexAniKey : public RVector, public RAnimationKey {
public:
	void SetInterpolatedKey(const RVertexAniKey& prevKey, const RVertexAniKey& nextKey, float s)
	{
		RVector vecInt = s*nextKey + (1.f-s)*prevKey;
		x = vecInt.x;
		y = vecInt.y;
		z = vecInt.z;
	}

};

// ��Ÿ�ӿ��� ����� keytrack
/*
template < class _KEYTYPE >
class RAnimationTrack : public vector<_KEYTYPE>
{
	void MoveIterator(int nFrame, unsigned int& keyIterator)
	{
		if(at(keyIterator).frame <= nFrame)
		{
			if(keyIterator>=size()-1) return;	// ���� ������ �� Ű
			while(keyIterator<size()-1 && nFrame >= at(keyIterator+1).frame )
				keyIterator++;
		}else
		{
			if(keyIterator==0) return;
			while( keyIterator>0 && at(keyIterator).frame > nFrame )
				keyIterator--;
		}
	}

public:
	void GetInterpolatedKey(_KEYTYPE* pOut, int nFrame, unsigned int& keyIterator)
	{
		MoveIterator(nFrame,keyIterator);
		if(keyIterator==size()-1)
		{
			*pOut = at(size()-1);
			return;
		}

		float s = float(nFrame - at(keyIterator).frame) / float(at(keyIterator+1).frame - at(keyIterator).frame);

		pOut->SetInterpolatedKey(at(keyIterator),at(keyIterator+1),s);
	}
};
*/

template < class _KEYTYPE >
class RAnimationTrackTest
{
	_KEYTYPE*		m_pData;
	unsigned int	m_nCount;
public:

	RAnimationTrackTest() : m_nCount(0), m_pData(NULL) { }

	RAnimationTrackTest(const RAnimationTrackTest& src) : m_nCount(0), m_pData(NULL)
	{ 
		*this = src;
	}

	void Create(int nCount)
	{
		Clear();

		m_pData = new _KEYTYPE[nCount];
 		m_nCount = nCount;
	}

	RAnimationTrackTest& operator=(const RAnimationTrackTest& src)
	{
		Clear();

		m_nCount =src.m_nCount;
		if(m_nCount>0)
		{
			m_pData = new _KEYTYPE[m_nCount];	
			memcpy(m_pData, src.m_pData, sizeof(_KEYTYPE)*m_nCount);
		}
		
		return *this;
	}

	virtual ~RAnimationTrackTest()
	{
		Clear();
	}

	void Clear() 
	{
		m_nCount =0;

		if(m_pData)
		{
			delete[] m_pData;
			m_pData = NULL;
		}
	}

	_KEYTYPE& at(int nIndex) { return m_pData[nIndex]; }
	_KEYTYPE& operator[](int nIndex) { return m_pData[nIndex]; }
	_KEYTYPE& front() { return m_pData[0]; }
	_KEYTYPE& back() { return m_pData[m_nCount-1]; }

	unsigned int size() { return m_nCount; }
	bool empty() { return m_nCount==0; }

	void MoveIterator(int nFrame, unsigned int& keyIterator)
	{
		// ������ ������� �ʱ�ȭ
		if(keyIterator>=size()) 
			keyIterator = 0;

		if(at(keyIterator).frame <= nFrame)
		{
			if(keyIterator>=size()-1) return;	// ���� ������ �� Ű
			while(keyIterator<size()-1 && nFrame >= at(keyIterator+1).frame )
				keyIterator++;
		}else
		{
			if(keyIterator==0) return;
			while( keyIterator>0 && at(keyIterator).frame > nFrame )
				keyIterator--;
		}
	}

public:
	void GetInterpolatedKey(_KEYTYPE* pOut, int nFrame, unsigned int& keyIterator)
	{
		MoveIterator(nFrame,keyIterator);
		if(keyIterator==size()-1)
		{
			*pOut = at(size()-1);
			return;
		}

		float s = float(nFrame - at(keyIterator).frame) / float(at(keyIterator+1).frame - at(keyIterator).frame);

		pOut->SetInterpolatedKey(at(keyIterator),at(keyIterator+1),s);
	}
};

};