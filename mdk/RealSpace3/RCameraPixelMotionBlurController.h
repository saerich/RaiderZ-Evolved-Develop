#ifndef _RCAMERA_PIXEL_MOTION_BLUR_CONTROLLER_H
#define _RCAMERA_PIXEL_MOTION_BLUR_CONTROLLER_H

#include "RCameraSceneNodeController.h"
#include "RShaderFX.h"
#include "RListener.h"


// Motion Blur ( Pixel) �� ó���ϴ� Ŭ����
// �⺻���� ������ RCameraScreenMotionBlurController�� C&P�ؼ� ����...


namespace rs3 {

	class RS_API RCameraPixelMotionBlurController : public RCameraSceneNodeController, public RListener
	{
	public:
		RCameraPixelMotionBlurController( RCameraSceneNode* pCameraSceneNode );
		virtual ~RCameraPixelMotionBlurController();

	protected:

		RShaderFX*		m_pShader;	

		// Render Target Texture. ����Ƽ�� ���̱� ���� ���� ������ ���鼭 ���θ� �����Ͽ� ���ø� �ݺ�.
		RTexture* 		m_pColorBuffer[2];
		// Render Target Texture
		RTexture* 		m_pVelocityBuffer;				
		// ������
		RTexture* 		m_pColorBufferOrg;
		// �׷����� �ִ� ���� �� ��
		RSceneNode*		m_pRenderNode;

		// RT width and height
		DWORD m_dwWidth;
		DWORD m_dwHeight;

		DWORD m_dwWidthSmall;
		DWORD m_dwHeightSmall;

		BOOL	m_bActive;
		BOOL	m_bLockFlag;
		float	m_fBias;		// �ӵ� ���ۿ� �޽��� �׸� �� �÷� ���ۿ� ������ �޽��� z-fight�� ���ϱ� ���� ���̾�� �Ǵ�.

		int		m_nSamplerCnt;						// ���÷���. �ȼ� ���̴������� ���ø� Ƚ���� ����
		int		m_nImgSamCnt;						// �̹����� ��ä�� �� �׸��� Ƚ��.

		RListener*		m_pEventListener;			///< OnRestoreDevice / OnLostDevice �̺�Ʈ ���Ű�ü

	public:
		RCameraSceneNode*		m_pCameraSceneNode;

		virtual void	UpdateCameraController(RMatrix& _rMatApplyToCamera) {}
		virtual void	OnClear();		
		virtual void	OnPostRender();	
		virtual void	OnRemoveFromCamera();
		virtual void	OnPreRender( RInferredRenderingStrategy* pInferredRenderingStrategy);

		bool 			Create();
		void 			Destroy();
		void			Restore();
		void			FrameMove();	// ī�޶� ��Ʈ�ѷ��� ������ ���갡 ���� ����. �������� ȣ�� �ǵ��� ����.

		// ����
		void 			DrawFullScreenQuad(float fLeftU, float fTopV, float fRightU, float fBottomV, int b = 0);
		void			Render();
		void			RenderOverlay();
		virtual void 	OnRender();

		inline void		SetActive( BOOL b)			{ if(!m_bLockFlag) m_bActive = b;	}
		inline BOOL		IsActive()					{ return m_bActive;					}

		// ���� ������ �� ó���� ���ϰ� �ϸ�, �̹��� �ݺ� ���� �� ������ �پ��� �̻��ϰ� �ȴ�. ������ ������ ���ֱ� ���� �Լ� �����⸸ �д�.
		inline void		SetImgSamCnt( int n)		{ /*m_nImgSamCnt = n;*/				}
		inline int		GetImgSamCnt()				{ return 1;/*m_nImgSamCnt*/;		}

		// ������ �������� ������ ������ ����� ����.
		inline void		SetSamplerCount( int n)	{ m_nSamplerCnt = n;			}
		inline int		GetSamplerCount()		{ return m_nSamplerCnt;			}

		bool			BeginRender( RSceneNode* pActorNode);
		bool			EndRender( RSceneNode* pActorNode);
	};

}

#endif
