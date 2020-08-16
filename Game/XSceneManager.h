#ifndef _XSCENE_MANAGER_H
#define _XSCENE_MANAGER_H

#include "RSceneManager.h"
using namespace rs3;

extern const char *RSID_XCHARACTER;

class XSceneManager : public RSceneManager
{
private:
	MDeclareRTTI;
public:
	XSceneManager();
	virtual ~XSceneManager();


	//////////////////////////////////////////////////////////////////////////
	// �� �Ŵ��� ������
	//
	virtual bool				Create(RDevice* pDevice) override;


	//////////////////////////////////////////////////////////////////////////
	// ������Ʈ / ������
	//
	virtual void				Update();
	virtual void				RenderPrimaryCamera();				// �����̸Ӹ� ī�޶� ������


	//////////////////////////////////////////////////////////////////////////
	// ���ҽ� �����
	//
	virtual void				Clear();


	//////////////////////////////////////////////////////////////////////////
	// ���丮
	//
	virtual RSceneNode*				CreateSceneNode(const char* pChar);
	virtual RSceneNodeController*	CreateSceneNodeController(const char* _pSzControllerName);

	void						DeleteActor(XCharacter* pActor);
	void						ReserveActorToDelete(XCharacter* pActor);


	std::vector<XCharacter*>	m_vecReserveRemoveToDelete;
};

#endif // _XSCENE_MANAGER_H