#ifndef _XFRAME_H
#define _XFRAME_H

#include <map>
#include <string>
#include "MEvent.h"

using namespace std;
using namespace mint3;


/// ���� ������ �⺻Ŭ����
class XGameFrameManger;
class XBaseFrame
{
protected:
	friend class XGameFrameManager;
	XGameFrameManager* m_pFrameManager;
public:
						XBaseFrame() : m_pFrameManager(NULL) {}			///< ������
	virtual				~XBaseFrame()			{}						///< �ı���
	virtual void		Update(float fDelta);							///< ������Ʈ
	virtual void		OnPreRender();									///< ���� ����
	virtual void		OnRender();										///< ȭ�鿡 �׸���
	virtual void		OnPreFlip();									///< Flip ����
	virtual bool		OnEvent(MEvent& e);								///< �̺�Ʈ ó��
	virtual bool		Open( int nPrevFrameID, const wchar_t* szArgs)	{ return true; }		///< ���������� ���Խ� �ʿ��� ������ �ֵ��� �Ѵ�.
	virtual void		Close( int nNextFrameID) {}						///< ���������� ������ �ʿ��� ������ �ֵ��� �Ѵ�.
	virtual void		OnActive(bool bActive)	{}						///< Ȱ��ȭ��, ��Ȱ��ȭ��
	virtual void		Message(const wchar_t* args) {}					///< �����ӿ� �޼��� ����
	virtual void		OnPreLoading()		{}							///< �ε� ���� ����
	virtual void		OnLoading()			{}							///< �ε� ����
	virtual void		OnPaused()			{}							///< �ε� �Ϸ� �� ����
	virtual void		OnLoaded()			{}							///< �ε� ����
	virtual bool		CheckCloseState()		{ return true;}			///< ���� �� �ִ� �����ΰ�?
	virtual int			GetID() = 0;									///< ������ ID
	virtual const wchar_t* GetName()				{ return L""; }			///< ������ �̸�, �׽�Ʈ �������� ��� ���� ���� �ڿ� ���ڷ� �� �̸��� �־��ָ� �� ���������� �ٷ� �����Ѵ�.
};



#endif