#pragma once

// use coherent culling
#define USE_CC

namespace rs3
{
	class ROctreeItem;
	class ROctreeNode;
	class ROctreeManager;
	class ROMT;

	//----------------------------------------------------------------------
	// �ø��Լ� �̸�
	// Cull vs CullC : 
	//									CullC�� coherent list�� ����� ���� �ø� �Լ�
	// Cullx vs Cullx_M vs Cullx_MC : 
	//									_M�� masking����ȭ ����, _MC�� masking����ȭ plane coherency ����ȭ ����


	//----------------------------------------------------------------------
	// class ROctreeCallBack
	class ROctreeCallBack
	{
	public:
		virtual void CallBack_Cull(const RPlane* pPlanes,int planesNum,ROctreeItem* pItem,CULL_STATE state) = 0;	//���� culling�� �ù�
		virtual void CallBack_Cull(const RPlane* pPlanes,int planesNum,ROctreeItem* pItem,CULL_STATE before_state,CULL_STATE current_state) = 0; //coherent culling�� �ù�

		CULL_STATE CheckStateChangeForCoherentCulling(CULL_STATE before_state,CULL_STATE current_state);
		CULL_STATE CheckStateChangeForOcclusionCoherentCulling(CULL_STATE before_state,CULL_STATE current_state);
	};

	//----------------------------------------------------------------------
	// class ROctreeItem
	class ROctreeItem
	{
		friend ROMT;
		friend ROctreeNode;
		friend ROctreeManager;
	public:
		ROctreeItem(void);
		~ROctreeItem(void);

	public:
		bool	FitsInOctreeNode(ROctreeNode* pOctreeNode);

		float	X(void)	{	return m_pSceneNode->GetSphere().center.x;	}
		float	Y(void)	{	return m_pSceneNode->GetSphere().center.y;	}
		float	Z(void)	{	return m_pSceneNode->GetSphere().center.z;	}
		float	R(void)	{	return m_pSceneNode->GetSphere().radius;	}

		RSceneNode*	GetSceneNode(void)	{	return m_pSceneNode;	}

	private:
		inline void			SetState(CULL_STATE state)	{	m_State = state;	}
		inline CULL_STATE	GetState(void)				{	return m_State;		}

	private:
		ROctreeItem*	m_pPrev;
		ROctreeItem*	m_pNext;
		ROctreeNode*	m_pOtreeNode;
		RSceneNode*		m_pSceneNode;
	
	private://����ȭ�� ���� �ʿ��� ����
		int			m_DeterminantPlaneIndex;	//�� Item�� outside���������ϴ� ����� �ε���

	private://Coherenct �ø�����Ʈ�� ����� ���� �ʿ��� ����
		CULL_STATE	m_State;
	};


	//----------------------------------------------------------------------
	// class ROctreeNode
	static const int O_HEIGHT = 2;	//��Ʈ���� 2������ 4���� ������ ������ ����. ��, 2*4 = 8 ���� ��带 ����
	static const int O_WIDTH  = 4;

	class ROctreeNode
	{
		friend ROMT;
		friend ROctreeItem;
		friend ROctreeManager;
	public:
		ROctreeNode(ROctreeNode* pParent, float x, float y,float z, float radius, int d);
		~ROctreeNode(void);
	public:
		static	ROctreeNode*	Create(ROctreeNode* pParent, float x, float y,float z, float radius,int d);
		static	void			Release(ROctreeNode* pNode);

	private:
		void	DetachFromParent(void);
		inline	bool	IsEmpty(void);

		//Normal��
		void	Cull_M(const RPlane* pPlanes,int planesNum,CULL_STATE state,ROctreeCallBack *pCallBack,int Masking);
		void	CallBackItems_M(const RPlane* pPlanes,int planesNum,CULL_STATE state,ROctreeCallBack *pCallBack,int Masking);

		//Coherent List�� ����� �ø��Լ���
		void	CullC_MC(const RPlane* pPlanes,int planesNum,CULL_STATE state,ROctreeCallBack *pCallBack,int Masking);
		void	CallBackItemsC_MC(const RPlane* pPlanes,int planesNum,CULL_STATE state,ROctreeCallBack *pCallBack,int Masking);

	protected:
		float	m_X,m_Y,m_Z;
		float	m_Radius;
		int		m_Depth;
		int		m_ItemsCount;

	private:
		ROctreeNode*	m_pParent;
		ROctreeNode*	m_pChildren[O_HEIGHT][O_WIDTH];
		ROctreeItem*	m_pItemsChain;

	private:
		int			m_DeterminantPlaneIndex;	//�̳�带 outside���������ϴ� ����� �ε���
		CULL_STATE	m_State;
	};

	//----------------------------------------------------------------------
	// class ROctreeNode
	class ROctreeManager
	{
	public:
		ROctreeManager(ROctreeCallBack *pCallBack);
		~ROctreeManager(void);

	public:
		ROctreeItem*	Add(RSceneNode* pSceneNode);
		void			Remove(ROctreeItem *pItem);
		void			Move(ROctreeItem *pItem,RSphere &Sphere);
		void			Move(ROctreeItem *pItem);

	public:
		void	Cull(const RPlane* pPlanes,int planesNum,ROctreeCallBack *pCallBack);
		void	Cull_MC(const RPlane* pPlanes,int planesNum,ROctreeCallBack *pCallBack);

	private:
		int		Insert(ROctreeNode* pOctreeNode,ROctreeItem *pItem);
		void	Detach(ROctreeItem *pItem);	//SPT���� ������Ŵ
		void	DestroyEmptyNodeRecursively(ROctreeNode* pOctreeNode);

	protected://����
		ROctreeNode*	m_pRootNode;

	protected://�Ӽ�
		float	m_WorldHalfRadius;	//������ ������
		int		m_MaxDepth;			//Ʈ���� ���� ����
		float	m_LooseK;			//loose ��� K

		int		m_ItemsCount;	//�� ��Ʈ���� ���Ե� ������� ���� (���� : �ڽĿ�Ʈ����忡 ���Ե� ������� ������ �����Ѱ� �ƴ�)

	protected://coherent list�� ����� ����
		typedef struct tagStateItem
		{
			CULL_STATE		state;
			ROctreeItem*	pItem;
			bool operator<( const tagStateItem& Right) const
			{
				if(pItem		< Right.pItem)			return true;
				if(pItem		> Right.pItem)			return false;
				return false;
			}
		}STATE_ITEM;
		std::set<STATE_ITEM>	m_AddedStateItemSet;
		std::set<ROctreeItem*>	m_RemovedItemSet;

	protected:
		ROctreeCallBack *m_pCallBack;
	};

	void	CheckStateChangeAndCallBack(const RPlane* pPlanes,int planesNum,ROctreeItem* pItem,CULL_STATE before_state,CULL_STATE current_state,ROctreeCallBack *pCallBack);
}