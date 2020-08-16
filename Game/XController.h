#ifndef _XCONTROLLER_H
#define _XCONTROLLER_H

#include "XGlobal.h"
#include "XGameInput.h"
#include "XUISystem.h"
#include <string>
using namespace std;

/// ������� �Է��� ����ϴ� Ŭ����
class XController : public XGameInput
{
protected:
	bool			m_bCreated;

	bool			m_bActiveJustNow;


public:
	XController();
	virtual ~XController();

	virtual bool Create();
	virtual void Destroy();

	virtual bool OnEvent( MEvent& e)											{ return false; }
	virtual bool OnFoceEvent( MEvent& e)										{ return false; }
	virtual void OnUpdate( float fDelta);
	virtual void OnActive( bool bActive);
	virtual void OnChat( CSMsgType nMsgType, const wstring& strMsg)				{}

	virtual bool CheckEnableKey( XVirtualKey nVKey)								{ return false; }					///< Ű ��� ���� ���� Ȯ��

	virtual void UpdateCameraAfterGameUpdate(float fDelta)						{ global.ui->UpdateDInputMouse(); }

	virtual bool IsVirtualKeyPressed( XVirtualKey nKey);															///< Ű ���۷� Ű ���� Ȯ��
	virtual bool IsVirtualKeyPressedAsync( XVirtualKey nKey);														///< Async�� Ű ���� Ȯ��

	virtual void SetBackViewCameraDir( const vec3& dir)							{}
	virtual vec3 GetBackViewCameraDir()											{ return vec3::ZERO; }
	virtual bool IsMainBackViewCamera()											{ return false; }
	virtual bool IsCameraLockView()												{ return false; }
	virtual bool IsCameraPvPFreeView()											{ return false; }

	virtual void SetFreeLookCamera( bool bSync)									{}

	virtual void LimitDegreeOnFreeLook( float fTolerRad)						{}
	virtual void LimitDegreeOnFreeLook2( float fTolerRad)						{}
	virtual void ReleaseLimitDegreeOnFreeLook()									{}

};



#endif // _XCONTROLLER_H