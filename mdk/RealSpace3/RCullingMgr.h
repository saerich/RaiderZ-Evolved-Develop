#ifndef _RCullingMGR_H_ 
#define _RCullingMGR_H_

#include "ROctreeManager.h"
#include "RRenderingList.h"
#include "RCullingList.h"
#include "ROccluderList.h"

#ifdef _DEBUG
#define VALIDATE_RENDERINGLIST
#endif

namespace	rs3
{
	//���漱��
	class RCullingMgrObserver;	
	class RCullingMgr;

	//----------------------------------------------------------------------
	// class RCullingCallBack
	// �ø��ù��Լ� ������
	class RCullingCallBack : public ROctreeCallBack
	{
	public:
		RCullingCallBack(RCullingMgr* pCullingMgr);
		~RCullingCallBack(void);

	public:
		virtual void	CallBack_Cull(const RPlane* pPlanes,int planesNum,ROctreeItem* pItem,CULL_STATE state) override {};	//���� culling�� �ù�
		virtual void	CallBack_Cull(const RPlane* pPlanes,int planesNum,ROctreeItem* pItem,CULL_STATE before_state,CULL_STATE current_state) override; //coherent culling�� �ù�
		RCullingMgr*	GetCullingMgr(void)	{	return m_pCullingMgr;	}

	private:
		RCullingMgr*	m_pCullingMgr;
	};

	class ROcclusionCullingCallBack : public ROctreeCallBack
	{
	public:
		ROcclusionCullingCallBack(RCullingMgr* pCullingMgr);
		~ROcclusionCullingCallBack(void);

	public:
		virtual void	CallBack_Cull(const RPlane* pPlanes,int planesNum,ROctreeItem* pItem,CULL_STATE state) override {};	//���� culling�� �ù�
		virtual void	CallBack_Cull(const RPlane* pPlanes,int planesNum,ROctreeItem* pItem,CULL_STATE before_state,CULL_STATE current_state) override; //coherent culling�� �ù�
		RCullingMgr*	GetCullingMgr(void)	{	return m_pCullingMgr;	}

	private:
		RCullingMgr*	m_pCullingMgr;
	};

	//----------------------------------------------------------------------
	// class RCullingMgr
	class RCullingMgr
	{
		friend RCullingCallBack;		//RCullingCallBack�� �Լ����� �ø�����Ʈ�� ����带 ��ƾ� �ϴµ�, �� �����Լ��� ���Լ��� private��
		friend ROcclusionCullingCallBack;
		friend RCullingMgrObserver;		//?
	public:
		//����/�Ҹ���
		RCullingMgr(void);
		virtual ~RCullingMgr(void);

	public:

		//�ø�����
		void						Cull(RWorldSceneNode* pWorldScene,const RPlane *pPlanes, int planesNum,bool bEnableToOverwriteSPT = false);
		void						CullSubNode(RCameraSceneNode* pCamera, const vector<RCULLDESC>& cullDescs, const RPlane* pPlaneNear, const RPlane* pPlaneFar);

		//��Ŭ�����ø�
		void						OcclusionCull(RWorldSceneNode* pWorldScene,const RPlane *pPlanes, int planesNum,bool bEnableToOverwriteSPT = false);

		//�ø��� ������Ʈ
		void						CulledListUpdate(RCameraSceneNode* pCamera);

		//������
		SCENE_SET*				GetCulledList(RRENDERPASS pass)			{	return m_CullingList.GetCulledList(pass);	}
		SCENE_SET*				GetRenderingList(RRENDERPASS pass)		{	return m_pRenderingList->GetRenderingList(pass);					}
		RRenderChain<RActorNodeKey>*			GetActorNodeChain(void)					{	return m_pRenderingList->GetActorNodeRenderChain();				}
		RRenderChain<RActorNodeKey>*			GetAlphaActorNodeChain(void)			{	return m_pRenderingList->GetAlphaActorNodeRenderChain();			}
		RRenderChain<RActorNodeKey>*			GetAlphaAdditiveActorNodeChain(void)	{	return m_pRenderingList->GetAlphaAdditiveActorNodeRenderChain();	}
		SCENE_SET*								GetAlpha2PassActor(void)				{ return m_pRenderingList->GetAlpha2PassActor();	}
		RRenderChain<RShadowActorNodeKey>*			GetActorNodeShadowChain(void)	{	return m_pRenderingList->GetShadowActorNodeRenderChain();	}
		RRenderChain<RShadowActorNodeKey>*			GetActorNodeDoubleShadowChain(void)	{	return m_pRenderingList->GetDoubleShadowActorNodeRenderChain();	}

		MAP_TREECHAIN*			GetTreeChain(void)						{	return m_pRenderingList->GetTreeRenderChain();					}

		//�����
		void					RenderCulledListForDebug(DWORD color);

		//��ƿ
		void					MakeRenderingListFromCullingList(RRenderingList* pValidatedRenderingList);

		template<class RenderingList>
		void SetRenderingList()
		{
			if(m_pRenderingList)
				delete m_pRenderingList;
			m_pRenderingList = new RenderingList;

#ifdef VALIDATE_RENDERINGLIST
			if(m_pValidatedRenderingList)
				delete m_pValidatedRenderingList;
			m_pValidatedRenderingList = new RenderingList;
#endif
		}

	private:
		//BVH����
		void						AddSceneNodeToSPT(RSceneNode *pNode);
		void						RemoveSceneNodeFromSPT(RSceneNode *pNode);
		void						MoveSceneNodeInSPT(RSceneNode *pNode);
		void						NotifyedThatWorldSceneRemoved(void);

		//CulledList����
		void						AddSceneNodeToCulledList(RSceneNode *pNode);
		void						RemoveSceneNodeFromCulledList(RSceneNode* pNode);
		bool						IsSceneNodeInCulledList(RSceneNode* pNode);
		void						ClearCulledList(void);	

		void						PrepareToCulling(RWorldSceneNode* pWorldScene,bool bEnableToOverwriteSPT = false);
		void						BuildSPT( RWorldSceneNode* pWorldScene);

		//������
		void						RegisterObserverTo(void);
		void						UnRegsiterObserverTo(void);

		//RenderingList����
		void						AddSceneNodeToRenderingList(RSceneNode *pNode);
		void						RemoveSceneNodeFromRenderingList(RSceneNode *pNode);
		void						ClearRederingList(void);
		void						RefreshRenderingList(RSceneNode* pNode);

	private:	//�ø�����Ʈ
		RCullingList		m_CullingList;

	private:	//����������Ʈ
		RRenderingList*		m_pRenderingList;
		RRenderingList*		m_pValidatedRenderingList;

	protected:	//��������
		typedef std::map<RSceneNode*,ROctreeItem*> RSCENENODE_TO_ITEM_MAP;
		RSCENENODE_TO_ITEM_MAP	m_MapRSceneNodeToItem;

		ROctreeManager*		m_pSPT;
		ROctreeCallBack*	m_pCullingCallBack;
		RWorldSceneNode*	m_pRegisteredWorldRootScene;
	};

	//----------------------------------------------------------------------
	// class RCullingMgrObserver
	// ������
	class RCullingMgrObserver
	{
	public:
		void	OnWorldRootSceneDestroyed(void);
		void	OnSceneNodeAddedToSPT(RSceneNode* pSceneNode);
		void	OnSceneNodeRemovedFromSPT(RSceneNode* pSceneNode);
		void	OnSceneNodeMoveInSPT(RSceneNode* pSceneNode);
		void	OnRefreshRenderingList(RSceneNode* pNode );

	public:
		void	Register(RCullingMgr* pCullingMgr);
		void	UnRegister(RCullingMgr* pCullingMgr);
		bool	IsRegisteredCullingMGR(RCullingMgr* pCullingMgr);

		void	Clear(void);

	private:
		typedef std::set<RCullingMgr*> CULLING_OBSERVER;
		std::set<RCullingMgr*>		m_Observer;
	};

}

#endif