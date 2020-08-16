/************************************************************************
MAnimationTemplate
 �ִϸ��̼� ���� �����͸��� ��� �ִ� Ŭ����
*************************************************************************/

#pragma once

#include "MResource.h"

namespace mint3
{

/*	MPLAYMODE
	�ִϸ��̼� �÷��� ��带 ����
*/
enum MPLAYMODE
{
	MPM_FORWARD			= 0,				// ���������� �ݺ�
	MPM_BACKWARD,							// ���������� �ݺ�
	MPM_FORWARDONCE,						// ���������� �ѹ���
	MPM_BACKWARDONCE,						// ���������� �ѹ���
	MPM_FORWARDBACKWARD						// �յڷ� �ݺ�
};


/*	MAnimationTemplate
	Animation ���ø� Ŭ����
*/
class MAnimationTemplate : public MResource 
{
protected:
	MBitmap*		m_pBitmap;				// ��Ʈ�� �̹��� ������
	int				m_nSplitCount;			// �̹����� ��� ������ �������� �����ϴ� ����
	vector<int>		m_Delay;				// ������ ���� �������� �ð� �����̸� �����ϴ� ����

public:
	MAnimationTemplate();
	virtual ~MAnimationTemplate();

	// ���� �����̸� �����ϴ� �Լ�
	void SetDelay( string strDelay);

	// ������ ���� �����̸� �����ϴ� �Լ�
	void SetDelay( int nSceneNum, int nDelay);

	// ��ü �� ���� ���ϴ� �Լ�
	int GetSplitCount();

	MBitmap* GetBitmap();
	void SetBitmap(MBitmap* pBitmap);

	// ������Ʈ
	void UpdateSceneNum( MPLAYMODE ePlayMode, int& inoutSceneNum , bool& inoutbPlay, int& inoutPlayDir, DWORD& inoutTimer);
};

//////////////////////////////////////////////////////////////////////////
// inline functions

inline int MAnimationTemplate::GetSplitCount() { return m_nSplitCount; }
inline MBitmap* MAnimationTemplate::GetBitmap() { return m_pBitmap; }
inline void MAnimationTemplate::SetBitmap(MBitmap* pBitmap) { m_pBitmap = pBitmap; }

} // namespace mint3