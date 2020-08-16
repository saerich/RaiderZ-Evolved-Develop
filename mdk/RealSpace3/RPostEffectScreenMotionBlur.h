#pragma once

#include "RPostEffectInterface.h"
#include "RDevice.h"

enum SCREEN_MOTION_BLUR_TYPE {
	SMB_BY_CAM_MOVE = 0,
	SMB_BY_EPICENTER
};

enum SCAT_ANI_TYPE {
	SAT_NORMAL = 0,
	SAT_FAST_SCAT,
	SAT_SLOW_SCAT,
	SAT_GATTER,
};


// Motion Blur ( Screen) �� ó���ϴ� Ŭ����
// �⺻���� ������ RCameraHDRController�� C&P�ؼ� ����...
namespace rs3 {

class RShaderFX;

class RS_API RPostEffectScreenMotionBlur : public RPostEffectInterface
{
public:
	RPostEffectScreenMotionBlur();
	virtual ~RPostEffectScreenMotionBlur();

protected:

	RShaderFX*		m_pShader;	

	RMatrix	m_matPrevVP;						// ���� �������� ����������. ���嵵 ���� �ؾ� �ϴ°�?
	RMatrix	m_matInvV;							// ���� �ι���.

	RMatrix		m_matPrevV;						// ���� �� ��Ʈ������ �״�� ��� �ϴ°� �ƴ϶� ������ �ӷ����� ��������..
	RVector3	m_vPrePos;
	RVector3	m_vPreUp;
	RVector3	m_vPreDir;
	RVector3	m_vMoveDir;

	int		m_nSamplerCnt;						// ���÷���. �ȼ� ���̴������� ���ø� Ƚ���� ����
	float	m_fSpeed;							// ���ǵ�. �̵� ���ǵ�. ������ ����. 
	bool	m_invalidateRotate;					// ȸ�� ������ ���� ���� �������� ���� �� ������ �ƴ���

	BYTE	m_nType;

	RVector4	m_vEpicenter;					// �ؽ��� ��ǥ�� ���� 0~1�� ������
	DWORD	m_dwScatStartTime;					// GetLastElapsedTime()�� �� ���� 
	DWORD	m_dwPlayTime;						// Scat ���ϸ��̼� �� �ð�
	DWORD	m_dwScatTime;						// ���ϸ��̼� �� �ð� �� ������� �κ� �ð�. �������� ������� �ð�
	float	m_fScatSpeed;						// ���ϸ��̼� �� �ְ����� ���ǵ�. �� ���� ������� m_fSpeed�� ����
	BYTE	m_nScarFactorType;					// ���ϸ��̼��� scat ����

	RListener*		m_pEventListener;			///< OnRestoreDevice / OnLostDevice �̺�Ʈ ���Ű�ü

public:
	virtual	bool	Init( int nW, int nH) override;
	virtual	void	Destroy() override;

	virtual void	Render( RCameraSceneNode* pCamera, RTexture* pColorBuffer, RTexture* pNormalBuffer, RTexture* pDepthBuffer);

	void			FrameMove();	// ī�޶� ��Ʈ�ѷ��� ������ ���갡 ���� ����. �������� ȣ�� �ǵ��� ����.

	// ����
	void			RenderOverlay( RCameraSceneNode* pCamera, RTexture* pColorBuffer, RTexture* pNormalBuffer, RTexture* pDepthBuffer);

	void			SetEpicenterScreenCoord( float x, float y)		{ m_vEpicenter = RVector4( x/(float)REngine::GetDevice().GetScreenWidth(), y/(float)REngine::GetDevice().GetScreenHeight(), 0, 0);	}
	void			SetEpicenterProjectionCoord( float x, float y);
	void			SetEpicenterWorld( RCameraSceneNode* pCurrentCamera, RVector3& v);
	// By Epicenter�Ͻ� ���� ���� �ڵ� ���ϸ��̼�. m_fSpeed�� �ش� �ð����� ���� ������ �ȵȴ�.
	void			PlayScat( float fPlayTime, float fScatTime, float fSpeed, BYTE nScarFactorType);

	// ���ǵ�� ���� ī�޶� ��ġ�� ���� �������� ī�޶� ��ġ ���� ���ǵ常ŭ ���. m_fSpeed�� 0�̸� ���� �������� ��ġ �״�� ���
	inline void		SetSpeed( float f)		{ m_fSpeed = f;				}
	inline float	GetSpeed()				{ return m_fSpeed;			}
	// ���÷����´� 15���� �����Ѵ�. ���� 15 �̻����� �ᵵ �۵��� ������ ���̴����� %15�������� ��� �Ѵ�.
	// ������ �������� ������ ������ ����� ����.
	inline void		SetSamplerCount( int n)	{ m_nSamplerCnt = n;			}
	inline int		GetSamplerCount()		{ return m_nSamplerCnt;			}
	// ȸ�� ������ ���� ���� �������� ���� �� ������ �ƴ���
	inline void		SetInvalidateRotate( bool b)	{ m_invalidateRotate = b;		}
	inline bool		GetInvalidateRotate()			{ return m_invalidateRotate;	}

	inline void		SetBasisByCamMove()				{ m_nType = SMB_BY_CAM_MOVE;	m_dwScatStartTime = 0;}
	inline void		SetBasisByEpicenter()			{ m_nType = SMB_BY_EPICENTER;	}

	
};

}

