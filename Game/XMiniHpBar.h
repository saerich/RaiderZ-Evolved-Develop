#ifndef _XMINI_HP_BAR_H
#define _XMINI_HP_BAR_H

#pragma once

#include "XCharacterCaption.h"

class XMiniHpBar : public XCharacterCaption, public MMemPool<XMiniHpBar>
{
	friend class XCaptionMgr;	// ���� / �Ҹ��� �Ŵ����� ����
private:
	RTexture*				m_hHpTexture;			//< �ؽ���.
	RTexture*				m_hBarTexture;			//< �ؽ���.

	RLVertex				m_BarVertices[4];
	RLVertex				m_HpVertices[4];

	bool					m_bActiveBar;

	RMatrix					m_matForBillBoard;

private:
	void					InitVertexBuffer();
	void					Render(RMatrix& matWorld, DWORD dwColor);

protected:
	virtual bool			Update(vec3& vCamPos, RMatrix& viewInverse);
	virtual void			postRender();

public:
	XMiniHpBar(MID nID, rs3::RSceneNode* pSceneNode);
	~XMiniHpBar();

	void					Create();
	void					Destroy();

	void					SetActive(bool bActive)		{ m_bActiveBar = bActive; }

	void					UpdateHpData(int nHPRatio);
};

#endif //_XMINI_HP_BAR_H
