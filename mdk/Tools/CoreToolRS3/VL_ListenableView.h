#pragma once
#include "CoreTypes.h"

class CControlMediator;
class CWorkSpaceManager;

struct CoreMessage
{
	CoreMessage(VIEW_MESSAGE::ID _id) : nID(_id), pLogicObject(NULL) {}
	CoreMessage(VIEW_MESSAGE::ID _id, CLogicObject* _pLogicObject) : nID(_id), pLogicObject(_pLogicObject) {}

	VIEW_MESSAGE::ID	nID;
	CLogicObject*		pLogicObject;
};

/** @brief Listenable View */
class CORETOOL_API CListenableView
{
public:
							CListenableView(void){}
	virtual					~CListenableView(void){}

	virtual int				GetViewID()							= 0;	///< ���� ���̵�
	virtual const char*		GetViewName()						= 0;	///< ���� �̸�
	virtual void			Listen(CoreMessage& _message)		= 0;	///< �޽����� ����
	
protected:
	friend class			CWorkSpaceManager;
	CControlMediator*		m_pControlMediator_;						///< �信�� ������ ��Ʈ�� �߰���
	HINSTANCE				m_hResourceHandle;							///< �䰡 ���� ��� ���ҽ� �ڵ�
};

/** @brief Listenable View Impl for ViewID */
template <typename T>
class CListenableViewImpl : public CListenableView
{
public:
	virtual int GetViewID()
	{
		return GetID();
	}

	static const int& GetID()
	{
		static int i = -1;
		return i;
	}
};


/** @brief Listenable View Impl general */
template <typename T>
class CListenableViewImpl_General : public CListenableViewImpl<T>
{
public:
	static const CORE_VIEW_TYPE	GetType()	{ return CVT_GENERAL; }
	enum { IDD = 0 };	// �ǹ̾����� �������� ���� ����
};

/** @brief Listenable View Impl Rollup */
template <typename T>
class CListenableViewImpl_Rollup : public CListenableViewImpl<T>
{
public:
	static const CORE_VIEW_TYPE	GetType()	{ return CVT_ROLLUP; }
};

/** @brief Listenable View Impl dialog */
template <typename T>
class CListenableViewImpl_Dialog : public CListenableViewImpl<T>
{
public:
	static const CORE_VIEW_TYPE	GetType()	{ return CVT_DIALOG; }
};


/** @brief Listenable View Impl fixed dialog */
template <typename T>
class CListenableViewImpl_Fixed : public CListenableViewImpl<T>
{
public:
	static const CORE_VIEW_TYPE	GetType()	{ return CVT_FIXED; }
};