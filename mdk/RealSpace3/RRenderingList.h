#pragma once

#include "RRenderChain.h"
namespace rs3
{

	class RTreeResource;
	class RTreeSceneNode;
	class RActorNode;
	class RMeshNode;

	class RActorNodeKey		
	{
	public:
		RActorNodeKey(void);
		~RActorNodeKey(void);

	public:
		bool operator<( const RActorNodeKey& Right) const;	//std::map�� Ű�� ���� ������ �񱳿����ڸ� ���� ���־����

	public:
		RMaterial*		m_pMaterial;
		int				m_nSubMaterial;
		int				m_nNodeType;
		// ���� ��ȸ �� VF������ ��ȭ�� ������ �ϱ� ����
		HVERTEXFORMAT	m_hVF;
		// �ν��Ͻ� ���� �� �� �޽� �����θ� �̷�� ������.
		RMeshNode*		m_pMeshNode;
	};

	class RShadowActorNodeKey : public RActorNodeKey
	{
	public:
		bool operator<( const RShadowActorNodeKey& Right) const;	//std::map�� Ű�� ���� ������ �񱳿����ڸ� ���� ���־����
	};

	typedef std::map<RActorNodeKey,SCENE_SET*>		MAP_ACTORNODE;
	typedef std::pair<RActorNodeKey,SCENE_SET*>		PAIR_ACTORNODE;

	typedef std::map<RTreeResource*,SCENE_SET*>		MAP_TREECHAIN;
	typedef std::pair<RTreeResource*,SCENE_SET*>	PAIR_TREECHAIN;

	typedef std::multimap<RSceneNode*,SCENE_SET*>	MULTIMAP_ACTORNODE_TO_SET;
	typedef std::pair<RSceneNode*,SCENE_SET*>		PAIR_ACTORNODE_TO_SET;
	typedef std::pair<MULTIMAP_ACTORNODE_TO_SET::iterator,MULTIMAP_ACTORNODE_TO_SET::iterator> PAIR_ITER_ACTORNODE_TO_SET;

	

	class RRenderingList
	{
	public:
		RRenderingList(void);
		~RRenderingList(void);

	public://��ƿ��Ƽ
		void	Clear(void);
		bool	IsEmpty(void);
		void	Update(void);
		bool	Equal(RRenderingList& rhs);

	//-------------------------------------------------------------------------------[ �Ϲݳ��� ]
	public:
		void				Add(RSceneNode *pNode,RRENDERPASS pass);
		void				Remove(RSceneNode *pNode,RRENDERPASS pass);
		SCENE_SET*			GetRenderingList(RRENDERPASS pass)	{	return &m_Set[pass];			}
	private:
		SCENE_SET		m_Set[RRP_MAX];

	//-------------------------------------------------------------------------------[ ���Ϳ� ]
	//���ҽ��� ���� �ε����� ���� ���Ϳ�
	public:
		void				AddNullActor(RSceneNode* pNode);
		void				RemoveNullActor(RSceneNode* pNode);
		bool				IsInNullActor(RSceneNode* pNode);
	private:
		std::set<RSceneNode*>		m_setNullActor;				//���ҽ��� ���� �ε����� ���� ���͵�

	public:
		//����2�н� ���Ϳ�
		void				AddAlpha2PassActor(RSceneNode* pNode);
		void				RemoveAlpha2PassActor(RSceneNode* pNode);

		SCENE_SET*			GetAlpha2PassActor(void)	{	return &m_Alpha2PassActorSet;	}

	private:
		SCENE_SET			m_Alpha2PassActorSet;


	//-------------------------------------------------------------------------------[ ���ͳ��� ]
	public:
		void				RemoveActorNode(RSceneNode* pNode);
		virtual void		AddActorNode(RSceneNode* pNode,RActorNodeKey& key)					{	m_ActorNodeRenderChain.AddSceneNode(pNode,key);					}
		virtual void		AddAlphaAdditiveActorNode(RSceneNode* pNode,RActorNodeKey& key)		{	m_AlphaAdditiveActorNodeRenderChain.AddSceneNode(pNode,key);	}
		virtual void		AddAlphaActorNode(RSceneNode* pNode,RActorNodeKey& key)				{	m_mapAlphaActorNodeRenderChain.AddSceneNode(pNode,key);			}
		virtual void		AddShadowActorNode(RSceneNode* pNode,RShadowActorNodeKey& key)		{}
		virtual void		AddDoubleShadowActorNode(RSceneNode* pNode,RShadowActorNodeKey& key)	{}
		
		RRenderChain<RActorNodeKey>*	GetActorNodeRenderChain(void)				{	return &m_ActorNodeRenderChain;					}
		RRenderChain<RActorNodeKey>*	GetAlphaAdditiveActorNodeRenderChain(void)	{	return &m_AlphaAdditiveActorNodeRenderChain;	}
		RRenderChain<RActorNodeKey>*	GetAlphaActorNodeRenderChain(void)			{	return &m_mapAlphaActorNodeRenderChain;			}
		RRenderChain<RShadowActorNodeKey>*	GetShadowActorNodeRenderChain(void)			{	return &m_mapShadowActorNodeRenderChain;			}
		RRenderChain<RShadowActorNodeKey>*	GetDoubleShadowActorNodeRenderChain(void)			{	return &m_mapDoubleShadowActorNodeRenderChain;			}

	private:
		RRenderChain<RActorNodeKey>	m_ActorNodeRenderChain;
		RRenderChain<RActorNodeKey>	m_AlphaAdditiveActorNodeRenderChain;
		RRenderChain<RActorNodeKey>	m_mapAlphaActorNodeRenderChain;
	protected:
		RRenderChain<RShadowActorNodeKey>	m_mapShadowActorNodeRenderChain;
		RRenderChain<RShadowActorNodeKey>	m_mapDoubleShadowActorNodeRenderChain;

	//-------------------------------------------------------------------------------[ Ʈ���� ]
	public:
		void	AddTreeChain(RTreeSceneNode *pNode);
		void	RemoveTreeChain(RTreeSceneNode* pNode);
		MAP_TREECHAIN*		GetTreeRenderChain(void)			{	return &m_mapTreeChain;			}
		
	private:
		SCENE_SET*	FindTreeChainSet(RTreeResource* pResource);
		SCENE_SET*  CreateTreeChainSet(RTreeResource* pResource);
		void		ClearTreeChain(void);
	private:
		MAP_TREECHAIN				m_mapTreeChain;				
	};

	class RShadowRenderingList : public RRenderingList
	{
	public:
		virtual void		AddActorNode(RSceneNode* pNode,RActorNodeKey& key)					{}
		virtual void		AddAlphaAdditiveActorNode(RSceneNode* pNode,RActorNodeKey& key)		{}
		virtual void		AddAlphaActorNode(RSceneNode* pNode,RActorNodeKey& key)				{}
		virtual void		AddShadowActorNode(RSceneNode* pNode,RShadowActorNodeKey& key)		{ m_mapShadowActorNodeRenderChain.AddSceneNode(pNode,key); }
		virtual void		AddDoubleShadowActorNode(RSceneNode* pNode,RShadowActorNodeKey& key)		{ m_mapDoubleShadowActorNodeRenderChain.AddSceneNode(pNode,key); }
	};
}
