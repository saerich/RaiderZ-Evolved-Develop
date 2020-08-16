#pragma once

#include "RActor.h"
#include "RActorNode.h"
#include "RClothSimulator.h"

namespace rs3 
{
	class RS_API RClothActorNode : public RActorNode,public RClothSimulator
	{
		MDeclareRTTI;

	public:
		RClothActorNode(void);
		~RClothActorNode(void);

		static RClothActorNode* Create(std::string meshName,std::string fileName);
		
	//--[ RActorNode�� �Լ� ]
	public:
		virtual bool	OnCreateDeviceDependentPart(void);
		virtual	void	UpdateTransformAndVertex(void);
		virtual void	SetVertexStream( bool b2ndBufferMng = true ) override;
		virtual	void	RenderPrimitive(int index);

	//--[ RClothSimulator�� �Լ� ]
	protected:
		virtual		void		simulate(float timeStep) = 0;

	//--[ RClothActorNode�� �Լ� ]
	protected:
		//�ùķ��̼�
		void		simulateByFixedTime(void);
		
		//����׿� �븻 �׸���
		void			RenderNormal();

		//����׿� �浹 �޽� �׸���
		virtual void	RenderCollision(int index) {}

		//�� ���� world, inv world ��Ʈ���� ���ϴ� �Լ�
		void			getClothWorldTransformMatrix(RMatrix *pWorldTransformMatrix);	
		void			getClothInvWorldTransfromMatrix(RMatrix *pInvWorldTransformMatrix);

		//���󰡴� ���� TransformMatrix���ϴ� �Լ���
		RActorNode*		getBoneForWorldTransform(void);
		RActorNode*		getBoneForWorldTransformFromBoneName(const string& boneName);
		string			getBoneNameForWorldTransformFromProperty(void);

		bool			isAttachedToBone(void);
	
		void			getLastTranslation(RVector *pLastTranslation);
	
		void		calculateNormals(void);
		void		makeSortedCollision(void);
		void		applyWind(RVector *pWindVector);

		int			isValidModeling(void);
		bool		isValid(void)	{	return m_bValidModel;	}

	public:
		string		getNodeName(void)				{	return m_strNodeName;	}
		void		setFileName(string fileName)	{	m_fileName = fileName;	}
		std::string getFileName(void)				{	return m_fileName;		}

	protected:
		vector<int>	m_vSortedCollisionIndices;
		string		m_fileName;
		float		m_beforeDuaration;
		float		m_currentDuration;

		RActorNode* m_pBoneForWorldTransform;
		bool		m_bValidModel;

		RMatrix		m_beforeClothWorldMatrix;
		RMatrix		m_currentClothWorldMatrix;

		unsigned int m_nLastUpdatedFrame;

	public://--;; cml2 include���谡 ����ؼ�..���⿡..
		static	void	AABB2OBB(RBox& aabb,ROBB& obb);
	};
}//rs3