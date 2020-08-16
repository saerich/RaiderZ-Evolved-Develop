#ifndef _RCAMERA_ACCUM_MOTION_BLUR_CONTROLLER_H
#define _RCAMERA_ACCUM_MOTION_BLUR_CONTROLLER_H

#include "RCameraSceneNodeController.h"
#include "RShaderFX.h"
#include "RListener.h"



// Motion Blur ( Accum buffer) �� ó���ϴ� Ŭ����
// �⺻���� ������ RCameraScreenMotionBlurController�� C&P�ؼ� ����...


// ���� �����ӿ��� Ȱ��ȭ �� ���۵��� ����Ʈ�� �Ź� �����Ѵ�.
// ���ʹ� �ڽ��� ���� ���� ���� �Ǹ� ī�޶� ��Ʈ�ѷ��� �����Ѵ�.
// ī�޶� ��Ʈ�ѷ��� ������ ������ ���Ϳ� ������ ¦�� �����( �̹� ������ ����~) ���Ϳ� �Ҵ� �� ���ۿ� ���� ����� �׸���.
// ��ó�� ������ �ܰ迡���� ���۵��� ���� ȭ�鿡 �����.
// ���� ���۸� ���ͺ��� �� �� �ְԵ�����~
// 
// ���ʹ� �Ҹ� �� ī�޶� ��Ʈ�ѷ��� �̸� �˸��� ���۸� ������ �� �ְ� ����~

namespace rs3 {

typedef std::map< RSceneNode*, RTexture*>	ACCUM_BUFFER_MAP;
typedef std::list< RTexture*>				DRAW_BUFFER_LIST;

class RS_API RCameraAccumMotionBlurController : public RCameraSceneNodeController, public RListener
{
public:
	RCameraAccumMotionBlurController( RCameraSceneNode* pCameraSceneNode );
	virtual ~RCameraAccumMotionBlurController();

protected:

	RShaderFX*		m_pShader;	

	// Render Target Texture
	RTexture* 		m_pColorBuffer;
	// Render Target Texture
	RTexture* 		m_pAccumBuffer[2];				
	// ������
	RTexture* 		m_pColorBufferOrg;
	// ���� �������� ���͵��� �ӽ÷� �׷����� �� ����
	RTexture* 		m_pTmpBuffer;
	// �׷����� �ִ� ���� �� ��
	RSceneNode*		m_pRenderNode;

	// RT width and height
	DWORD m_dwWidth;
	DWORD m_dwHeight;

	DWORD m_dwWidthSmall;
	DWORD m_dwHeightSmall;

	bool	m_bActive;
	bool	m_bLockFlag;
	bool	m_id;

	float m_fAccumRate;
	DWORD m_dwAccumRate;
	float m_fDecreaseAlpha;
	DWORD m_dwTimeAccum;
	float m_fLastAlphaValue;

	RListener*		m_pEventListener;			///< OnRestoreDevice / OnLostDevice �̺�Ʈ ���Ű�ü

public:
	RCameraSceneNode*		m_pCameraSceneNode;

	virtual void	UpdateCameraController(RMatrix& _rMatApplyToCamera) {}
	virtual void	OnClear();		
	virtual void	OnPostRender();	
	virtual void	OnRemoveFromCamera();
	virtual void	OnPreRender( RInferredRenderingStrategy* pInferredRenderingStrategy);	// ���� Ÿ���� ����

	bool 			Create();
	void 			Destroy();
	void			Restore();
	void			FrameMove();	// ī�޶� ��Ʈ�ѷ��� ������ ���갡 ���� ����. �������� ȣ�� �ǵ��� ����.

	// ����
	void 			DrawFullScreenQuad(float fLeftU, float fTopV, float fRightU, float fBottomV, int b = 0);
	void			Render();
	void			RenderOverlay();
	void			RenderColorFill();
	virtual void 	OnRender();

	inline void		SetActive( bool b)			{ if(!m_bLockFlag) m_bActive = b;	}
	inline bool		IsActive()					{ return m_bActive;					}

	inline void		SetDecreaseAlpha( float f)	{ m_fDecreaseAlpha = f;			}
	inline float	GetDecreaseAlpha()			{ return m_fDecreaseAlpha;		}

	inline void		SetAccumRate( float f)		{ m_fAccumRate = f;	m_dwAccumRate = (DWORD)(m_fAccumRate*1000.0f);	}
	inline float	GetAccumRate()				{ return m_fAccumRate;		}

	bool			DeleteActorNode( RSceneNode* pActorNode);
	bool			BeginRender( RSceneNode* pActorNode);
	bool			EndRender( RSceneNode* pActorNode);

	inline bool		IsBeingBlur()				{ return (bool)(m_fLastAlphaValue>0);	}

protected:
	RTexture*		GetRenderTarget( RSceneNode* pActorNode);
};

}

#endif
