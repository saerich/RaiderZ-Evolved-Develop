#include "stdafx.h"
#include "MAnimationTemplate.h"

// namespace : mint3
namespace mint3
{


// �÷��� ����
enum MPLAY_DIRECTION
{
	MPD_FORWARD		= 1,
	MPD_BACKWARD	= -1
};

MAnimationTemplate::MAnimationTemplate()
{
	// �� ī��Ʈ�� �ʱ�ȭ
	m_nSplitCount = 1;

	// �� ������ ����
	m_Delay.push_back( 200);
}

MAnimationTemplate::~MAnimationTemplate()
{
	if ( !m_Delay.empty())
		m_Delay.clear();
}


/*	SetDelay
	���� �����̸� �����ϴ� �Լ�
*/
void MAnimationTemplate::SetDelay( string strDelay)
{
	// ��Ʈ���� ���Ḧ Ȯ���� ǥ���Ѵ�
	strDelay += ',';


	// ������ ����Ʈ �ʱ�ȭ
	if ( !m_Delay.empty())
		m_Delay.clear();

	// �� ���� ���� �ʱ�ȭ
	m_nSplitCount = 0;


	// ������ ���� �о����
	char buffer[ 10];
	memset( buffer, 0, sizeof( buffer));
	int count = 0;

	for ( int i = 0;  i < (int)strDelay.size();  i++)
	{
		char ch = strDelay.c_str()[ i];

		if ( (ch >= '0') && (ch <= '9') && (count < 10))
		{
			buffer[ count++] = ch;
		
			continue;
		}


		if ( count > 0)
		{
			int nDelay = atoi( buffer);
			m_Delay.push_back( nDelay);
			m_nSplitCount++;
		}


		// ���� �ʱ�ȭ
		memset( buffer, 0, sizeof( buffer));
		count = 0;
	}
}


/*	SetDelay
	������ ���� �����̸� �����ϴ� �Լ�
*/
void MAnimationTemplate::SetDelay( int nSceneNum, int nDelay)
{
	if ( (nSceneNum < 0) || (nSceneNum >= m_nSplitCount))
		return;


	m_Delay[ nSceneNum] = nDelay;
}


void MAnimationTemplate::UpdateSceneNum( MPLAYMODE ePlayMode, int& inoutSceneNum , bool& inoutbPlay, int& inoutPlayDir,DWORD& inoutTimer)
{
	DWORD curr = timeGetTime();

	// ����� �ð��� ���Ѵ�
	DWORD diff = curr - inoutTimer;

	// ���� 2�� �̻� ���� �ʰų� �÷��� ���°� �ƴϸ� �׳� �����Ѵ�
	if ( (m_nSplitCount < 2) || !inoutbPlay)
	{
		inoutTimer = curr;
		return;
	}

	while ( 1)
	{
		// ���� ���� ������ ���� ���Ѵ�
		DWORD dwDelay = (DWORD)m_Delay[ inoutSceneNum];


		// ���� ���� ���� ������ ��ŭ �ð��� ���������� ����
		if ( diff < dwDelay)
			break;


		// �ð��� �������� ���� ������ �̵�
		inoutSceneNum += inoutPlayDir;


		switch ( ePlayMode)
		{
			case MPM_FORWARD :
				if ( inoutSceneNum >= m_nSplitCount)
					inoutSceneNum = 0;
				break;

			case MPM_BACKWARD :
				if ( inoutSceneNum < 0)
					inoutSceneNum = m_nSplitCount - 1;
				break;

			case MPM_FORWARDONCE :
				if ( inoutSceneNum >= m_nSplitCount)
				{
					inoutSceneNum = 0;
					inoutbPlay = false;
				}
				break;
				
			case MPM_BACKWARDONCE :
				if ( inoutSceneNum < 0)
				{
					inoutSceneNum = m_nSplitCount - 1;
					inoutbPlay = false;
				}
				break;

			case MPM_FORWARDBACKWARD :
				if ( inoutSceneNum >= m_nSplitCount)
				{
					inoutSceneNum = m_nSplitCount - 2;
					inoutPlayDir = MPD_BACKWARD;
				}

				else if ( inoutSceneNum < 0)
				{
					inoutSceneNum = 1;
					inoutPlayDir = MPD_FORWARD;
				}
				break;
		}

		// Ÿ�̸� ���� ������Ʈ ��
		inoutTimer += dwDelay;
		diff -= dwDelay;
	}
}


} // namespace mint3














/************************************************************************************
���⼭���� �����ڵ��Դϴ�



#include "stdafx.h"
#include "MAnimation.h"

namespace mint3 {

#define DELAY_CONSTANT	1

void MAnimation::OnDraw(MDrawContext* pDC)
{
	if ( m_pAniBitmap == NULL)
		return;

	// Get current time
	int nCurTime = timeGetTime();

	// Get current frame
	int nCurFrame = GetCurrentFrame();
	
	// Get frame count
	int nFrameCount = m_pAniBitmap->GetFrameCount();


	if ( m_nPlayMode == MAPM_FORWARDONCE)
	{
		m_nCurrFrame = nCurFrame;

		if ( nCurFrame >= nFrameCount)
            m_nCurrFrame = nFrameCount - 1;
	}

	else if ( m_nPlayMode == MAPM_REPETITION)
		m_nCurrFrame = nCurFrame % nFrameCount;

	else if ( m_nPlayMode == MAPM_FORWARDNBACKWARD)
	{
		int nIterCount = nCurFrame / nFrameCount;
		m_nCurrFrame = nCurFrame % nFrameCount;
		if ( (nIterCount % 2) == 1)			// Ȧ����° Play�̸� Backward
			m_nCurrFrame = nFrameCount - m_nCurrFrame - 1;
	}

	MBitmap* pBitmap = m_pAniBitmap->Get( m_nCurrFrame);
	pDC->SetBitmap( pBitmap);
	MRECT r = GetClientRect();
	pDC->Draw( r.x, r.y, r.w, r.h);
}

MBitmap* MAnimation::GetBitmap()
{
	if(m_pAniBitmap == NULL) return NULL;
	int nCurTime = timeGetTime();
	int nCurFrame;
	if(m_nDelay!=0) nCurFrame = (nCurTime-m_nStartTime) / (m_nDelay*DELAY_CONSTANT);
	else nCurFrame = (nCurTime-m_nStartTime) / DELAY_CONSTANT;
	int nFrameCount = m_pAniBitmap->GetFrameCount();
	if(nFrameCount==0) return NULL;

	if(m_nPlayMode==MAPM_FORWARDONCE){
		if(nCurFrame>=nFrameCount) nCurFrame = nFrameCount-1;
	}
	else if(m_nPlayMode==MAPM_REPETITION){
		nCurFrame = nCurFrame % nFrameCount;
	}
	else if(m_nPlayMode==MAPM_FORWARDNBACKWARD){
		int nIterCount = nCurFrame/nFrameCount;
		nCurFrame = (nCurFrame%nFrameCount);
		if(nIterCount%2==1){	// Ȧ����° Play�̸� Backward
			nCurFrame = nFrameCount - nCurFrame - 1;
		}
	}

	return m_pAniBitmap->Get(nCurFrame);
}

MAnimation::MAnimation(const char* szName, MAniBitmap* pAniBitmap, MWidget* pParent)
: MWidget(szName, pParent, NULL)
{
	m_pAniBitmap = pAniBitmap;
	if(pAniBitmap!=NULL) m_nDelay = pAniBitmap->GetDelay();
	else m_nDelay = 1;	// Default
	m_nStartTime = timeGetTime();

	m_nPlayMode = MAPM_FORWARDONCE;
	m_nCurrFrame = 0;
	m_bRunAnimation = true;
}

void MAnimation::SetAniBitmap(MAniBitmap* pAniBitmap)
{
	m_pAniBitmap = pAniBitmap;
	if(pAniBitmap!=NULL) m_nDelay = pAniBitmap->GetDelay();
}

void MAnimation::InitStartTime()
{
	m_nStartTime = timeGetTime();
}


void MAnimation::SetRunAnimation( bool bRun)
{
	m_bRunAnimation = bRun;

	SetCurrentFrame( m_nCurrFrame);
}

int MAnimation::GetCurrentFrame( void)
{
	if ( !m_bRunAnimation)
		return m_nCurrFrame;

	// Get current time
	int nCurrTime = timeGetTime();

	// Get current frame
	int nCurrFrame;

	if ( m_nDelay != 0)
		nCurrFrame = (nCurrTime - m_nStartTime) / (m_nDelay * DELAY_CONSTANT);
	else
		nCurrFrame = (nCurrTime - m_nStartTime) / DELAY_CONSTANT;

	return nCurrFrame;
}

void MAnimation::SetCurrentFrame( int nFrame)
{
	m_nCurrFrame = nFrame;

	if ( nFrame < m_pAniBitmap->GetFrameCount())
        m_nStartTime = timeGetTime() - ( nFrame * m_nDelay * DELAY_CONSTANT);
}

} // namespace mint3
*/