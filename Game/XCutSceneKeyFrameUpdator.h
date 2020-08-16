#pragma once

#include <vector>

template<typename T_Key, typename T_KeyContainer = std::vector<T_Key> >
class XCutSceneKeyFrameUpdator
{
public:
	XCutSceneKeyFrameUpdator() : m_fAccumulatedElapsed(0.f), m_nCurrentKey(0) {}
	virtual ~XCutSceneKeyFrameUpdator(){}

	template< typename T_Caller >
	bool UpdateKey(T_Caller* pCaller, float fElapsed, T_KeyContainer& _rKeys)
	{
		size_t nTotal = _rKeys.size();
		if (m_nCurrentKey >= nTotal)
			return false;

		// �ð� ����
		m_fAccumulatedElapsed += fElapsed;

		bool bProcStop = false;
		while (!bProcStop && m_nCurrentKey < nTotal)
		{
			T_Key* pPre = (m_nCurrentKey == 0) ? NULL : &_rKeys[m_nCurrentKey-1];
			T_Key* pCurrent = &_rKeys[m_nCurrentKey];
			T_Key* pNext = (m_nCurrentKey + 1 == nTotal) ? NULL : &_rKeys[ m_nCurrentKey + 1];

			// ����ִ°��
			// a - [pPre] - b - [pCurrent] - c - [pNext] - d
			// pNext�� ��
			// e - [pPre] - f - [pCurrent] - g
			// pPre�� ��
			// h - [pCurrent] - i - [pNext] - j
			// ���� ��� ��
			// k - [pCurrnet] - l

			// current�� �ð��� �������� �ٲ��
			// a, e : Ű�� ������Ʈ�ϰ� ������Ȳ(b,f)�� ����
			// b, f : Ű�� ������Ʈ�ϰ� ������Ȳ(d, g, j, l vs c, i)�� ����
			// h, k : ������Ʈ�� �����ش�
			// d, g, j, l vs c, i

			// [key 1] - 1 - [key 2] - 2
			T_Key* pProcKey = NULL;
			int nType = 0;

			// 2 �� �Ǵ� ��� : 
			// - d, j: pNext�� �ְ�, ���� �ð��� pNext�� �ð��� ������ ���, pProcKey2 = pNext, ����Ű�� �����ϰ� ������ �������� ����
			// - g, l : pNext�� ���� ���� �ð��� pCurrent�� �ð��� ������ ���, pProcKey2 = pCurrent ����Ű�� �����ϰ� ������ �������� ����
			if ( (pNext) && (m_fAccumulatedElapsed >= pNext->fTime) )
			{
				nType = 2;
				pProcKey = pNext;
			}
			else if ( (!pNext) && (m_fAccumulatedElapsed >= pCurrent->fTime) )
			{
				nType = 2;
				pProcKey = pCurrent;
			}
			// 1�� �Ǵ� ���
			// - c, i : pNext �� �ְ� ����ð��� pNext�� �ð����� �۰� pCurrent���� ũ�ų� ����, Ű ���� ����, ���� ����
			else if ( (pNext) && (m_fAccumulatedElapsed < pNext->fTime) && (m_fAccumulatedElapsed >= pCurrent->fTime) )
			{
				nType = 1;
			}
			// h, k : ������Ʈ�� �����ش�
			else if (NULL == pPre)
			{
				nType = 4;
			}

			switch(nType)
			{
			case 0:
				{
					// a,e,b,f
					m_nCurrentKey += 1;
					break;
				}
			case 1:
				{
					// c, i
					bProcStop = true;

					float fInterpolationRatio = (m_fAccumulatedElapsed - pCurrent->fTime) / ( pNext->fTime - pCurrent->fTime);
					T_Key* pNextNext = (m_nCurrentKey + 2 >= nTotal) ? NULL : &_rKeys[ m_nCurrentKey + 2 ];
					pCaller->OnUpdateBetweenKey(pCurrent, pNext, fInterpolationRatio, pPre, pNextNext);
					break;
				}
			case 2:
			 {
				 // d, g, j, l
				 m_nCurrentKey += 1;
				 if (pNext == NULL)
					 pCaller->OnUpdateOverKey(pProcKey);
				 break;
			 }
			case 4:
				{
					// h, k
					bProcStop = true;
					break;
				}
			}
		}

		return (m_nCurrentKey < nTotal);
	}

	void SortKey(T_KeyContainer& _rKeys, float* _pfStartTime = NULL, float* _pfEndTime = NULL)
	{
		struct SFuncKeyLess
		{
			bool operator()(const T_Key& _rLhs, const T_Key& _rRhs)
			{
				return _rLhs.fTime < _rRhs.fTime;
			}
		};

		std::sort( _rKeys.begin(), _rKeys.end(), SFuncKeyLess() );
		size_t nCount = _rKeys.size();
		if (nCount > 0 && _pfStartTime && _pfEndTime)
		{
			*_pfStartTime = _rKeys[0].fTime;
			*_pfEndTime = _rKeys[nCount - 1].fTime;
		}
	}

	void Reset()
	{
		m_fAccumulatedElapsed = 0.f;
		m_nCurrentKey = 0;
	}

	size_t	GetCurrentKey()			{ return m_nCurrentKey; }
	float	GetAccumulatedElapsed()	{ return m_fAccumulatedElapsed; }

	int	GetKeyFromTime(float fValue, const T_KeyContainer& _rKeys)
	{
		int nSize = (int)_rKeys.size();
		for (int i = 0; i < nSize; ++i)
		{
			float fError = abs(_rKeys[i].fTime - fValue);
			if (fError < 0.001f)
				return i;
		}

		return -1;
	}

protected:
	float	m_fAccumulatedElapsed;
	size_t	m_nCurrentKey;
};
