#include "stdafx.h"
#include "RCameraSceneNode.h"
#include "RPostEffectInterface.h"

using namespace rs3;

class CMockPostEffectInterface : public RPostEffectInterface
{
public:
	CMockPostEffectInterface() :RPostEffectInterface(PE_VOID, RRP_NONE),
		m_bCallCreateBuffers(false), m_bCallDestroyBuffers(false)
	{}

	virtual void Render( RCameraSceneNode* pCamera, RTexture* pColorBuffer, RTexture* pNormalBuffer, RTexture* pDepthBuffer) override
	{
		return;
	}

	bool IsCallCreateBuffers() { return m_bCallCreateBuffers; }
	bool IsCallDestroyBuffers() { return m_bCallDestroyBuffers; }

protected:
	virtual void CreateBuffers()  override { m_bCallCreateBuffers=true; }
	virtual void DestroyBuffers()  override { m_bCallDestroyBuffers=true; }	

private:
	bool m_bCallCreateBuffers;
	bool m_bCallDestroyBuffers;
};

TEST( TestPostEffectInterface, TestInvalidate_ShouldCallDestroyBuffers)
{
	//��ġ
	CMockPostEffectInterface posteffect;


	//����
	posteffect.Invalidate();


	//����
	EXPECT_TRUE(posteffect.IsCallDestroyBuffers());
}

TEST( TestPostEffectInterface, TestRestore_ShouldCallCreateBuffers)
{
	//��ġ
	CMockPostEffectInterface posteffect;


	//����
	posteffect.Restore();


	//����
	EXPECT_TRUE(posteffect.IsCallCreateBuffers());
}