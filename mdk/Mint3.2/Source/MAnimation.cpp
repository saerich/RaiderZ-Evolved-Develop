/************************************************************************
MAnimation

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : Animation ���� Ŭ���� �Դϴ�.
*************************************************************************/



#include "stdafx.h"
#include "MAnimation.h"



// namespace : mint3
namespace mint3
{


// �÷��� ����
enum MPLAY_DIRECTION
{
	MPD_FORWARD		= 1,
	MPD_BACKWARD	= -1
};



/*	MAnimation
	������
*/
MAnimation::MAnimation( const char* szName /*=NULL*/, MWidget* pParent /*=NULL*/, MListener* pListener /*=NULL*/)
: MPicture( szName, pParent, pListener), m_pAnimationTemplate(NULL)
{
	// ���� �� ��ȣ �ʱ�ȭ
	m_nSceneNum = 0;

	// Ÿ�̸� �ʱ�ȭ
	m_dwTimer = timeGetTime();

	// �ִϸ��̼��� �����ϴ��� ����
	m_bPlay = true;

	// �÷��� ����� ������ �ݺ� ���
	m_nPlayMode = MPM_FORWARD;

	// �ִϸ��̼� ���� ����
	m_nPlayDir = MPD_FORWARD;
}


/*	~MAnimation
	�Ҹ���
*/
MAnimation::~MAnimation()
{
}

/*	SetCurrScene
	���� �� ��ȣ�� �����ϴ� �Լ�
*/
void MAnimation::SetCurrScene( int nNum)
{
	if ( (nNum >= 0) || (nNum < m_pAnimationTemplate->GetSplitCount() ))
		m_nSceneNum = nNum;
}


/*	GetCurrScene
	���� �� ��ȣ�� ���ϴ� �Լ�
*/
int MAnimation::GetCurrScene()
{
	return m_nSceneNum;
}


/*	SetPlay
	�ִϸ��̼��� �÷��� ��Ű�� �Լ�
*/
void MAnimation::SetPlay( bool bPlay /*=true*/)
{
	m_bPlay = bPlay;
}

/*	SetPlayMode
	�÷��� ����� �����ϴ� �Լ�
*/
void MAnimation::SetPlayMode( MPLAYMODE mode)
{
	m_nPlayMode = mode;


	switch ( m_nPlayMode)
	{
		case MPM_FORWARD :
		case MPM_FORWARDONCE :
			m_nPlayDir = MPD_FORWARD;
			break;

		case MPM_BACKWARD :
		case MPM_BACKWARDONCE :
			m_nPlayDir = MPD_BACKWARD;
			break;

		default :
			break;
	}
}


/*	IsPlaying
	�ִϸ��̼��� �÷��� ������ Ȯ���ϴ� �Լ�
*/
bool MAnimation::IsPlaying()
{
	return m_bPlay;
}


/*	OnEvent
	OnEvent �̺�Ʈ �ڵ鷯
*/
bool MAnimation::OnEvent( MEvent* pEvent, MListener* pListener)
{
	return false;
}


/*	OnRun
OnRun �޽��� �ڵ鷯
*/
void MAnimation::OnRun( DWORD dwCurrTime)
{
	MWidget::OnRun( dwCurrTime);

	if(m_pAnimationTemplate)
		m_pAnimationTemplate->UpdateSceneNum( m_nPlayMode, m_nSceneNum, m_bPlay, m_nPlayDir, m_dwTimer );
}


/*	OnDraw
	OnDraw �̺�Ʈ �ڵ鷯
*/
void MAnimation::OnDraw( MDrawContext* pDC)
{
	// ��Ʈ���� ������ �׳� ����
	if ( m_pAnimationTemplate == NULL)
		return;

	MBitmap* pBitmap = m_pAnimationTemplate->GetBitmap();
	
	if(!pBitmap)
		return;

	
	// �׸��� ��� ����
	DWORD tmpDrawMode = pBitmap->GetDrawMode();
	pBitmap->SetDrawMode( m_dwRotate);


	// Colorize ���� ����
	MCOLOR tmpColorize = pDC->GetBitmapColor();
	pDC->SetBitmapColor( m_Colorize);


	// ��Ʈ�� ����
	pDC->SetBitmap( pBitmap);


	// �ش� ���� ��Ʈ���� ���Ѵ�
	
	int nSplitCount = m_pAnimationTemplate ? m_pAnimationTemplate->GetSplitCount() : 1;

	MRECT rectScene;
	rectScene.w = pBitmap->GetWidth() / nSplitCount;
	rectScene.h = pBitmap->GetHeight();
	rectScene.x = rectScene.w * m_nSceneNum;
	rectScene.y = 0;


	// ��Ʈ���� �÷���´ٸ� ���� Ŭ���̾�Ʈ ���� ��ü�� �׸���
	if ( m_bStretch)
	{
		MRECT rect = GetClientRect();
		pDC->Draw( rect.x, rect.y, rect.w, rect.h, rectScene.x, rectScene.y, rectScene.w, rectScene.h);
	}

	// �̹����� �״�� ��´ٸ�...
	else
		pDC->Draw( 0, 0, rectScene.w, rectScene.h, rectScene.x, rectScene.y, rectScene.w, rectScene.h);


	// Colorize ���� ����
	pDC->SetBitmapColor( tmpColorize);


	// �׸��� ��� ����
	pBitmap->SetDrawMode( tmpDrawMode);
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