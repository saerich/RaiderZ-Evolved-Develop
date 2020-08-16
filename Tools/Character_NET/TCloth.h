#pragma once

#include <string>
#include <vcclr.h>
#include <set>
#include "RClothActorNode.h"
#include ".\reflectiveProperty\ReflectiveRClothInfo.h"
#include ".\reflectiveProperty\ClothReflectivePropertyForm.h"

class TCloth
{
public:
	TCloth(int slot,int ItemID,std::string ItemName,std::string ItemMeshName,RClothActorNode* pRClothActorNode);
	~TCloth(void);

public:
	bool	createClothMenu(std::string menuName);

public:
	//-[ ���� ActorNode ]
	void					setRClothActorNode(RClothActorNode* pRClothActorNode);
	RClothActorNode*		getRClothActorNode(void)	{	return m_pRClothActorNode;	}

	//-[ ���� �ĺ��� �� ���� ]
	void					setIdentifier(int ItemID,std::string ItemName,std::string ItemMeshName);
	int						getID(void)				{	return m_ItemID;		}
	std::string				getItemName(void)		{	return m_ItemName;		}
	std::string				getItemMeshName(void)	{	return m_ItemMeshName;	}

	//-[ ���� �ĺ��� �� ���� (from 'MainApp.cpp') ]
	void					setSlotNumber(int slot)	{	m_slot = slot;	}
	int						getSlotNumber(void)		{	return m_slot;	}

public:
	typedef std::set<int> SetOfID;

protected:
	//-[ c# Form ]
	gcroot<Character_NET::ClothReflectivePropertyForm^> m_ClothReflectiveProperty;

	//-[ Reflection ����� ���� ���� ]
	ReflectiveRClothInfo	m_ReflectiveRClothInfo;

	//-[ ���� ActorNode ]
	RClothActorNode*	m_pRClothActorNode;

	//-[ ���� �ĺ��� �� ���� (from 'CSItemData.h') ]  : ������ �ĺ��Ҷ� ������ �����ϱ� ���� ����
	int				m_ItemID;			//�������� ������ ID
	string			m_ItemName;			//�������� �̸�
	string			m_ItemMeshName;		//������ �޽��̸�

	//-[ ���� �ĺ��� �� ���� (from 'MainApp.cpp') ]
	int				m_slot;	//�� ���� ��ȣ

};

class TClothMgr
{
public:
	TClothMgr();
	~TClothMgr();
public:
	typedef list<TCloth*>					CLOTH_CONTAINER;
	typedef CLOTH_CONTAINER::iterator		CLOTH_ITERATOR;

public:
	//ĳ���� ���� ��
	bool	createClothMenuForCharacterItem(int slot,int ItemID,std::string ItemName,std::string ItemMeshName,RClothActorNode* pRClothActorNode);
	void	releaseClothMenuForCharacterItem(void);
	int		removeClothIfSlotIsSame(int slot);	//������ �ʾ������� ID�� ��ȯ

	//Static �޽��� ����� ��
	bool	createClothMenuForStaticMesh(RClothActorNode* pRClothActorNode);
	void	releaseClothMenuForStaticMesh(void);

	// Review�� ���� ���� ��
	bool	createClothMenuForCharacterReview(int slot,std::string MeshName,RClothActorNode* pRClothActorNode);


private:
	//ĳ���� ���� ��
	TCloth* addClothForCharacterItem(int Slot,int ItemID,std::string ItemName,std::string ItemMeshName,RClothActorNode* pRClothActorNode);
	void	removeClothForCharacterItem(CLOTH_ITERATOR iter);

	//Static �޽��� ����� ��
	TCloth* addClothForStaticMesh(RClothActorNode* pRClothActorNode);

private:
	CLOTH_CONTAINER		m_clothesForCharacterItem;	//ĳ���� ���� ��
	CLOTH_CONTAINER		m_clothesForStaticMesh;		//Static �޽��� ����� ��
};	