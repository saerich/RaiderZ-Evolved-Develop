#ifndef _XCUTSCENE_CONTROLLER_H
#define _XCUTSCENE_CONTROLLER_H

#include "XController.h"

class XCutSceneState;


/// ������� �Է��� ����ϴ� Ŭ����
class XCutSceneController : public XController
{
protected:
	XCutSceneState*				m_pCutSceneState;

public:
	XCutSceneController(XCutSceneState* pState);
	virtual ~XCutSceneController();
	virtual void OnUpdate( float fDelta);

	virtual bool OnEvent( MEvent& e);
};



#endif // _XGAME_PLAY_CONTROLLER_H