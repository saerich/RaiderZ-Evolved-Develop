#include "stdafx.h"
#include "XPostEffect.h"

XPostEffect::XPostEffect()
{

}

XPostEffect::~XPostEffect()
{

}


void XPostEffect::SetMotionBlur( float fPlayTime, float fScatTime, float fSpeed, BYTE nScarFactorType )
{
	if(REngine::GetSceneManagerPtr())
	{
		if(fSpeed < MOTIONBLUR_MIN_SPEED)
			fSpeed = MOTIONBLUR_MIN_SPEED;
		if(fSpeed > MOTIONBLUR_MAX_SPEED)
			fSpeed = MOTIONBLUR_MAX_SPEED;

		if(fPlayTime == -1.0f)
		{
			// ��å�� �ʿ��մϴ�.
			//global.camera->GetPrimaryCamera()->SetScreenMotionBlurBasisByEpicenter();
			//global.camera->GetPrimaryCamera()->SetScreenMotionBlurSpeed(fSpeed);
		}
		else 
		{
			if(fPlayTime < fScatTime)
			{
				// Scat �ð����� Play �ð������� ũ�� �ȵȴ�.
				fScatTime = fPlayTime;
			}

			REngine::GetSceneManagerPtr()->SetScreenMotionBlurPlayScat(fPlayTime, fScatTime, fSpeed, nScarFactorType);
		}
	}
}

void XPostEffect::SetObjectMotionBlur( RActor* pActor, float fSpeed, bool bActive )
{
	if(pActor == NULL)
		return;

	if(fSpeed > OBJECTMOTIONBLUR_MAX_SPEED)
		fSpeed = OBJECTMOTIONBLUR_MAX_SPEED;
	if(fSpeed < OBJECTMOTIONBLUR_MIN_SPEED)
		fSpeed = OBJECTMOTIONBLUR_MIN_SPEED;

	pActor->SetVertexMotionBlur(bActive);
	pActor->SetVertexMotionBlurBasisFrame(fSpeed);
}

void XPostEffect::SetObjectAccumMotionBlur( RActor* pActor, bool bActive )
{
	if(pActor == NULL)
		return;

	pActor->SetAccumMotionBlur(bActive);
}

void XPostEffect::SetPixelObjectMotionBlur( RActor* pActor, float fSpeed, int nblurType )
{
	if(pActor == NULL)
		return;

	if(fSpeed > OBJECTMOTIONBLUR_MAX_SPEED)
		fSpeed = OBJECTMOTIONBLUR_MAX_SPEED;
	if(fSpeed < OBJECTMOTIONBLUR_MIN_SPEED)
		fSpeed = OBJECTMOTIONBLUR_MIN_SPEED;

	pActor->SetPixelMotionBlur(nblurType);
	pActor->SetMotionBlurBasisFrame(fSpeed);
}
