/************************************************************************
MAnimation

Prog. : Lim Dong Hwan
Date  : 10.Sep.2007
Desc. : Animation ���� Ŭ���� �Դϴ�.
*************************************************************************/

#pragma once

#include "MPicture.h"
#include "MAnimationTemplate.h"

// namespace : mint3
namespace mint3
{

class MAnimationTemplate;

/*	MAnimation
	Animation ���� Ŭ����
*/
class MAnimation : public MPicture
{
// Member variables
protected:
	MAnimationTemplate*	m_pAnimationTemplate;	// �ִϸ��̼� ���ø�

	int				m_nSceneNum;			// ���� �� ��ȣ

	DWORD			m_dwTimer;				// Ÿ�̸� ����

	bool			m_bPlay;				// �ִϸ��̼��� �����ϴ��� ����

	MPLAYMODE		m_nPlayMode;			// �÷��� ���

	int				m_nPlayDir;				// �ִϸ��̼� ���� ����

// Member functions
public:
	// ������
	MAnimation( const char* szName=NULL, MWidget* pParent=NULL, MListener* pListener=NULL);

	// �Ҹ���
	virtual ~MAnimation();

	void SetAnimationTemplate( MAnimationTemplate* pAnimationTemplate );

	// ���� �� ��ȣ�� �����ϴ� �Լ�
	void SetCurrScene( int nNum);

	// ���� �� ��ȣ�� ���ϴ� �Լ�
	int GetCurrScene();

	// �÷��� ����� �����ϴ� �Լ�
	void SetPlayMode( MPLAYMODE mode);

	// �ִϸ��̼��� �÷��� ��Ű�� �Լ�
	void SetPlay( bool bPlay =true);

	// �ִϸ��̼��� �÷��� ������ Ȯ���ϴ� �Լ�
	bool IsPlaying();



// Event handler
protected:
	// OnDraw �̺�Ʈ �ڵ鷯
	virtual void OnDraw( MDrawContext* pDC);

	// OnEvent �̺�Ʈ �ڵ鷯
	virtual bool OnEvent( MEvent* pEvent, MListener* pListener);

	// OnRun �޽��� �ڵ鷯
	void OnRun(void);


#define MINT_ANIMATION		"Animation"
	virtual const char* GetClassName(void)		{ return MINT_ANIMATION; }
};

//////////////////////////////////////////////////////////////////////////
inline void MAnimation::SetAnimationTemplate( MAnimationTemplate* pAnimationTemplate ) { m_pAnimationTemplate = pAnimationTemplate; }


} // namespace mint3















/*****************************************************************************
���⼭���� �����ڵ� �Դϴ�

#pragma once

#include "MWidget.h"

namespace mint3 {

enum MAPlayMode{
	MAPM_FORWARDONCE,
	MAPM_FORWARDNBACKWARD,
	MAPM_REPETITION,
};

class MAnimation : public MWidget{
private:
	unsigned int	m_nStartTime;
	MAniBitmap*		m_pAniBitmap;
public:
	int				m_nDelay;
	MAPlayMode		m_nPlayMode;
protected:
	virtual void OnDraw(MDrawContext* pDC);
public:
	MAnimation(const char* szName=NULL, MAniBitmap* pAniBitmap=NULL, MWidget* pParent=NULL);
	void SetAniBitmap(MAniBitmap* pAniBitmap);
	void InitStartTime();
	MBitmap* GetBitmap();

#define MINT_ANIMATION	"Animation"
	virtual const char* GetClassName(void){ return MINT_ANIMATION; }



//	��ȯ���� �߰� �κ� :
//	�Ʒ� �߰��� �κ��� Ư�� ��Ȳ���� �̹����� ���÷� �����ϴ� ��� �̸� �ִϸ��̼� ��������
//	�ذ��ϱ� ���ؼ� �߰��� ���̴�. ���� ���ؼ� �ִϸ��̼��� ������Ų ���¿��� ������ ������
//	�̹��� ��ȣ�� �ٲ��ִ� �����̴�.

protected:
	int				m_nCurrFrame;;			// ���� �������� �ִ� �ִϸ��̼� �̹��� ��ȣ

public:
	bool			m_bRunAnimation;		// ���� �ִϸ��̼��� �ǰ��ִ��� ����
	bool			GetRunAnimation( void)  { return m_bRunAnimation;}
	void			SetRunAnimation( bool bRun);
	int				GetCurrentFrame( void);
	void			SetCurrentFrame( int nFrame);
};

} // namespace mint3
*/