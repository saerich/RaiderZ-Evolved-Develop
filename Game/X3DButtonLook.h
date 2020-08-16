#ifndef XScreenEffectLOOK_H
#define XScreenEffectLOOK_H

#include "RSceneManager.h"
#include "RAnimationDef.h"
#include "XScreenEffectManager.h"
#include "MBButtonLook.h"

class X3DButtonLook : public MBButtonLook
{
private:
	RSceneManager *					m_pSceneManager;
	XScreenEffectManager *			m_pScreenEffectManager;

protected:
	string							m_strScreenEffectName;
	string							m_strScreenEffectFileName;
	XSCREENEFFECTAniInfo			m_stAnimationInfo[MBS_MAX];

	MButtonState					m_eButtonState;

public:

protected:
	/// ��ư�� �Լ��̴�.
	virtual void					OnDownDraw(MButton* pButton, MDrawContext* pDC);
	virtual void					OnUpDraw(MButton* pButton, MDrawContext* pDC);
	virtual void					OnOverDraw(MButton* pButton, MDrawContext* pDC);
	virtual void					OnDisableDraw(MButton* pButton, MDrawContext* pDC);

	/// ���� �Լ��̴�.
	virtual void					OnDraw(MButton* pButton, MDrawContext* pDC );

	bool							CreateReady();

public:
	X3DButtonLook(string strScreenEffectName, string strFileName, RSceneManager * pManager, XScreenEffectManager * pScreenEffectManager); 
	virtual ~X3DButtonLook();

	void							SetAnimationFileName(int nIndex, string strAniFileName, int AniEndAtt);
	void							Create();
};

#endif